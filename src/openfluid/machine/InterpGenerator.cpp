/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file
  @brief Implements ...

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <openfluid/machine/InterpGenerator.hpp>

#include <boost/math/special_functions/fpclassify.hpp>



namespace openfluid { namespace machine {


InterpGenerator::InterpGenerator() : Generator(),
  m_IsMin(false), m_IsMax(false), m_Min(0.0), m_Max(0.0),
  m_SourcesFile(""),m_DistriFile("")
{

}


// =====================================================================
// =====================================================================


InterpGenerator::~InterpGenerator()
{


}

// =====================================================================
// =====================================================================


void InterpGenerator::initParams(const openfluid::ware::WareParams_t& Params)
{
  if (!OPENFLUID_GetFunctionParameter(Params,"sources",m_SourcesFile))
    throw openfluid::base::OFException("OpenFLUID framework","InterpGenerator::initParams","missing sources value for generator");

  if (!OPENFLUID_GetFunctionParameter(Params,"distribution",m_DistriFile))
    throw openfluid::base::OFException("OpenFLUID framework","InterpGenerator::initParams","missing distribution value for generator");


  if (OPENFLUID_GetFunctionParameter(Params,"thresholdmin",m_Min)) m_IsMin = true;

  if (OPENFLUID_GetFunctionParameter(Params,"thresholdmax",m_Max)) m_IsMax = true;
}


// =====================================================================
// =====================================================================


void InterpGenerator::checkConsistency()
{
  if (m_IsMin && m_IsMax && m_Min > m_Max)
    throw openfluid::base::OFException("OpenFLUID framework","InterpGenerator::checkConsistency","threshold max value must be greater or equal to threshold min value for generator");
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest InterpGenerator::initializeRun()
{

  std::string InputDir;

  OPENFLUID_GetRunEnvironment("dir.input",InputDir);

  m_DataPool.setConfig(InputDir,m_SourcesFile,m_DistriFile,
                       //openfluid::tools::SERIEPREPCS_CUMULATE,
                       openfluid::tools::SERIEPREPCS_NONE,
                       OPENFLUID_GetBeginDate(),OPENFLUID_GetEndDate(),OPENFLUID_GetDefaultDeltaT());

  m_DataPool.loadAndPrepareData();

  m_CurrentStep = 0;


  openfluid::core::Unit* LU;
  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitClass,LU)
  {
    OPENFLUID_InitializeVariable(LU,m_VarName,0.0);
  }


  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest InterpGenerator::runStep()
{

  openfluid::core::Unit* LU;
  openfluid::core::DoubleValue CurrentValue;
  int ID;



  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitClass,LU)
  {
    ID = LU->getID();

    if (m_DataPool.getValue(ID,m_CurrentStep,&CurrentValue))
    {
      if (boost::math::isnan(CurrentValue))
        throw openfluid::base::OFException("OpenFLUID framework","InterpGenerator::runStep","interpolated value for variable " + m_VarName + " is NaN");

      if (m_IsMax && CurrentValue > m_IsMax) CurrentValue = m_Max;
      if (m_IsMin && CurrentValue < m_IsMin) CurrentValue = m_Min;

      if (isVectorVariable())
      {
        openfluid::core::VectorValue VV(m_VarSize,CurrentValue);
        OPENFLUID_AppendVariable(LU,m_VarName,VV);
      }
      else
        OPENFLUID_AppendVariable(LU,m_VarName,CurrentValue);
    }
    else
      throw openfluid::base::OFException("OpenFLUID framework","InterpGenerator::runStep","error interpolating value for variable " + m_VarName);
  }

  m_CurrentStep++;

  return OPENFLUID_GetDefaultDeltaT();
}


} } //namespaces


