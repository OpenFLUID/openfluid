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
#include <openfluid/tools/StringHelpers.hpp>
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
  {
    return;
  }

  SimulationContributorWare::initializeWare(ID);

  m_MaxThreads = MaxThreads;
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_IsSimulatorParameterExist(const openfluid::ware::WareParams_t& Params,
                                                             const openfluid::ware::WareParamKey_t& ParamName) const
{
  return OPENFLUID_IsWareParameterExist(Params, ParamName);
}


// =====================================================================
// =====================================================================


openfluid::core::StringValue PluggableSimulator::OPENFLUID_GetSimulatorParameter(
                                                 const openfluid::ware::WareParams_t& Params,
                                                 const openfluid::ware::WareParamKey_t& ParamName) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         openfluid::core::StringValue& Val) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Val);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         openfluid::core::DoubleValue& Val) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Val);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                       const openfluid::ware::WareParamKey_t& ParamName,
                                                       openfluid::core::VectorValue& Val) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Val);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         openfluid::core::MatrixValue& Val) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Val);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         bool& Val) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Val);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         double& Val) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Val);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         long& Val) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName,Val);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         float& Val) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Val);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         int& Val) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Val);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         std::string& Value) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Value);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         std::vector<std::string>& Vals) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Vals);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         std::vector<double>& Vals) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName, Vals);
}


// =====================================================================
// =====================================================================


bool PluggableSimulator::OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                         const openfluid::ware::WareParamKey_t& ParamName,
                                                         std::vector<long>& Vals) const
{
  return OPENFLUID_GetWareParameter(Params, ParamName,Vals);
}


// =====================================================================
// =====================================================================


void PluggableSimulator::OPENFLUID_SetSimulatorMaxThreads(const int& MaxNumThreads)
{
  if (MaxNumThreads > 0)
  {
    m_MaxThreads = MaxNumThreads;
  }
}


} } // namespaces
