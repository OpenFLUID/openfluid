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
  @file GeneratorDescriptor.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/tools/IDHelpers.hpp>


namespace openfluid { namespace fluidx {


GeneratorDescriptor::GeneratorDescriptor(openfluid::tools::UnitVarTriplets_t VarTriplets,
                        GeneratorMethod GenMethod, 
                        openfluid::core::Value::Type VarType, 
                        openfluid::core::Dimensions VarDimensions) :
    ModelItemDescriptor(openfluid::tools::buildGeneratorID(deduceVarPairs(VarTriplets), VarDimensions.strType()))
{
  m_WareType = openfluid::ware::WareType::GENERATOR;
  m_VarTriplets = VarTriplets;
  m_GenMethod = GenMethod;
  m_VarType = VarType;
  m_VarDimensions = VarDimensions;
}


// =====================================================================
// =====================================================================


openfluid::core::VariableName_t GeneratorDescriptor::getVariableName() const
{
  std::vector<std::string> Vars;
  for (const auto& Pair : m_VarTriplets)
  {
    if (std::count(Vars.begin(), Vars.end(), Pair.VariableName) == 0)
    {
      Vars.push_back(Pair.VariableName);
    }
  }
  
  return openfluid::tools::join(Vars, ", ");
}


// =====================================================================
// =====================================================================


openfluid::core::UnitsClass_t GeneratorDescriptor::getUnitsClass() const
{
  std::vector<std::string> UnitClasses;
  for (const auto& Pair : m_VarTriplets)
  {
    if (std::count(UnitClasses.begin(), UnitClasses.end(), Pair.UnitsClass) == 0)
    {
      UnitClasses.push_back(Pair.UnitsClass);
    }
  }
  return openfluid::tools::join(UnitClasses, ", ");
}


// =====================================================================
// =====================================================================


const openfluid::tools::UnitVarTriplets_t& GeneratorDescriptor::getVariableTriplets() const
{ 
  return m_VarTriplets;
}


// =====================================================================
// =====================================================================


GeneratorDescriptor::GeneratorMethod GeneratorDescriptor::getGeneratorMethod() const
{
  return m_GenMethod;
}


// =====================================================================
// =====================================================================


openfluid::core::Value::Type GeneratorDescriptor::getVariableType() const
{
    return m_VarType;
}

} } // namespaces

