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
  \file RandomGenerator.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <boost/random.hpp>

#include <openfluid/machine/RandomGenerator.hpp>

namespace openfluid { namespace machine {


RandomGenerator::RandomGenerator() : Generator()
{
  m_Min = 0.0;
  m_Max = 0.0;

}


// =====================================================================
// =====================================================================


RandomGenerator::~RandomGenerator()
{


}


// =====================================================================
// =====================================================================


bool RandomGenerator::checkConsistency()
{
  if (m_GenDesc.getParameters().find("min") != m_GenDesc.getParameters().end())
  {
    if (!openfluid::tools::ConvertString(m_GenDesc.getParameters()["min"],&m_Min))
      throw openfluid::base::OFException("kernel","RandomGenerator::checkConsistency","wrong format for min value");
  }
  else
    throw openfluid::base::OFException("kernel","FixedGenerator::checkConsistency","missing min value for generator");


  if (m_GenDesc.getParameters().find("max") != m_GenDesc.getParameters().end())
  {
    if (!openfluid::tools::ConvertString(m_GenDesc.getParameters()["max"],&m_Max))
      throw openfluid::base::OFException("kernel","RandomGenerator::checkConsistency","wrong format for max value");
  }
  else
    throw openfluid::base::OFException("kernel","FixedGenerator::checkConsistency","missing max value for generator");


  if ( m_Min > m_Max)
    throw openfluid::base::OFException("kernel","FixedGenerator::checkConsistency","max value must be greater or equal to min value for generator");

  return true;
}


// =====================================================================
// =====================================================================


bool RandomGenerator::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool RandomGenerator::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

  openfluid::core::Unit* LU;
  openfluid::core::ScalarValue Value;

  boost::uniform_real<> Distribution(m_Min, m_Max);
  boost::mt19937 Engine;
  boost::variate_generator<boost::mt19937, boost::uniform_real<> > Random (Engine, Distribution);

  DECLARE_UNITS_ORDERED_LOOP(1);

  BEGIN_UNITS_ORDERED_LOOP(1,m_GenDesc.getUnitClass(),LU)

    Value = Random();

    if (m_GenDesc.isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_GenDesc.getVariableSize(),Value);
      OPENFLUID_AppendVariable(LU,m_GenDesc.getVariableName(),VV);
    }
    else
      OPENFLUID_AppendVariable(LU,m_GenDesc.getVariableName(),Value);

  END_LOOP

  return true;
}

// =====================================================================
// =====================================================================


bool RandomGenerator::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  return true;
}


} } //namespaces


