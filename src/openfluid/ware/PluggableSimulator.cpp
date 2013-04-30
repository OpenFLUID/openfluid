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
  \file PluggableSimulator.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <openfluid/config.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


namespace openfluid { namespace ware {



PluggableSimulator::PluggableSimulator()
  : SimulationContributorWare(FUNCTION),
    m_MaxThreads(openfluid::config::SIMULATORS_MAXNUMTHREADS)
{

}


// =====================================================================
// =====================================================================


PluggableSimulator::~PluggableSimulator()
{
  finalizeWare();
}


// =====================================================================
// =====================================================================


void PluggableSimulator::initializeWare(const WareID_t& ID,const unsigned int& MaxThreads)
{
  if (m_Initialized) return;

  SimulationContributorWare::initializeWare(ID);

  m_MaxThreads = MaxThreads;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       openfluid::core::StringValue& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    Val = it->second;
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       openfluid::core::DoubleValue& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
    return it->second.toDoubleValue(Val);

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       openfluid::core::VectorValue& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
    return it->second.toVectorValue(Val.getStreamSeparator(1),Val);

  return false;
}



// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       openfluid::core::MatrixValue& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
    return it->second.toMatrixValue(Val.getStreamSeparator(1),Val.getStreamSeparator(2),Val);

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       double& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
    return it->second.toDouble(Val);

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       long& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
    return it->second.toInteger(Val);

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       float& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    double TmpDbl;

    if(it->second.toDouble(TmpDbl))
    {
      Val = static_cast<float>(TmpDbl);
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       int& Val) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    long TmpInt;

    if(it->second.toInteger(TmpInt))
    {
      Val = static_cast<int>(TmpInt);
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       std::string& Value) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    Value = it->second.get();

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      std::vector<std::string>& Vals) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    std::vector<std::string> Tokens;

    openfluid::tools::TokenizeString(it->second.get(),Tokens,";");

    Vals.clear();

    for (unsigned int i=0;i<Tokens.size();i++)
      Vals.push_back(Tokens[i]);

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      std::vector<double>& Vals) const
{
  bool IsOK = false;

  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    openfluid::core::VectorValue Vect;

    IsOK = it->second.toVectorValue(";",Vect);

    if(IsOK)
    {
      Vals.clear();

      for (unsigned long i=0;i<Vect.size();i++)
        Vals.push_back(Vect[i]);
    }
  }

  return IsOK;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      std::vector<long>& Vals) const
{
  bool IsOK = false;

  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
  {
    openfluid::core::VectorValue Vect;

    IsOK = it->second.toVectorValue(";",Vect);

    if(IsOK)
    {
      Vals.clear();

      for (unsigned long i=0;i<Vect.size();i++)
        Vals.push_back(static_cast<int>(Vect[i]));
    }
  }

  return IsOK;
}


// =====================================================================
// =====================================================================


void PluggableSimulator::OPENFLUID_SetSimulatorMaxThreads(const unsigned int& MaxNumThreads)
{
  if (MaxNumThreads > 0) m_MaxThreads = MaxNumThreads;
}


} } // namespaces
