/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/



/**
  @file PluggableSimulator.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */



#include <openfluid/config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


namespace openfluid { namespace ware {



PluggableSimulator::PluggableSimulator() : SimulationContributorWare(WareType::SIMULATOR),
    m_MaxThreads(1)
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
  if (m_Initialized)
    return;

  SimulationContributorWare::initializeWare(ID);

  m_MaxThreads = MaxThreads;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_IsSimulatorParameterExist(const openfluid::ware::WareParams_t& Params,
                                                             const openfluid::ware::WareParamKey_t& ParamName) const
{
  return (Params.find(ParamName) != Params.end());
}


// =====================================================================
// =====================================================================


openfluid::core::StringValue PluggableSimulator::OPENFLUID_GetSimulatorParameter(
                                                 const openfluid::ware::WareParams_t& Params,
                                                 const openfluid::ware::WareParamKey_t& ParamName) const
{
  openfluid::ware::WareParams_t::const_iterator it = Params.find(ParamName);

  if (it != Params.end())
    return it->second;
  else
    throw openfluid::base::FrameworkException(computeFrameworkContext(OPENFLUID_CODE_LOCATION),
                                              "Parameter "+ParamName+ " does not exist");
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
    return it->second.toVectorValue(Val);

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
    return it->second.toMatrixValue(Val);

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
    Value = it->second.data();

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

    openfluid::tools::tokenizeString(it->second.data(),Tokens,";");

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

    IsOK = it->second.toVectorValue(Vect);

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

    IsOK = it->second.toVectorValue(Vect);

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


void PluggableSimulator::OPENFLUID_SetSimulatorMaxThreads(const int& MaxNumThreads)
{
  if (MaxNumThreads > 0)
    m_MaxThreads = MaxNumThreads;
}


} } // namespaces
