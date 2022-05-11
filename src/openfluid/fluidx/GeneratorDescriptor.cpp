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
*/


#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/tools/IDHelpers.hpp>


namespace openfluid { namespace fluidx {


GeneratorDescriptor::GeneratorDescriptor(
    openfluid::core::VariableName_t VarName,
    openfluid::core::UnitsClass_t UnitsClass, GeneratorMethod GenMethod,
    unsigned int VarSize) :
    ModelItemDescriptor(openfluid::tools::buildGeneratorID(VarName,(VarSize > 1),UnitsClass))
{
  m_WareType = openfluid::ware::WareType::GENERATOR;
  m_VarName = VarName;
  m_UnitsClass = UnitsClass;
  m_GenMethod = GenMethod;
  m_VarSize = VarSize;

  if (m_VarSize < 1)
  {
    m_VarSize = 1;
  }
}


// =====================================================================
// =====================================================================


openfluid::core::VariableName_t GeneratorDescriptor::getVariableName() const
{
  return m_VarName;
}


// =====================================================================
// =====================================================================


openfluid::core::UnitsClass_t GeneratorDescriptor::getUnitsClass() const
{
  return m_UnitsClass;
}


// =====================================================================
// =====================================================================


GeneratorDescriptor::GeneratorMethod GeneratorDescriptor::getGeneratorMethod() const
{
  return m_GenMethod;
}


} } // namespaces

