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
  @file Generator.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/machine/Generator.hpp>


namespace openfluid { namespace machine {


void Generator::setInfos(openfluid::tools::UnitVarTriplets_t Triplets,
                  openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GenMethod, 
                  openfluid::core::Dimensions VarDimensions)
{
  m_VarTriplets = Triplets;
  m_GenMethod = GenMethod;
  m_VarDimensions = VarDimensions;
}


// =====================================================================
// =====================================================================


openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Generator::getGeneratorMethod() const
{
  return m_GenMethod;
}


// =====================================================================
// =====================================================================


void MonoGenerator::setInfos(openfluid::tools::UnitVarTriplets_t Triplets,
              openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GenMethod, 
              openfluid::core::Dimensions VarDimensions)
{
  if (Triplets.size() != 1)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                          "Wrong number of columns for this generator");  
  }
  Generator::setInfos(Triplets, GenMethod, VarDimensions);
  m_VarName = getVariableName();
  m_UnitsClass = getUnitsClass();
}


// =====================================================================
// =====================================================================


openfluid::core::VariableName_t MonoGenerator::getVariableName() const
{
  return m_VarTriplets[0].VariableName;
}


// =====================================================================
// =====================================================================


openfluid::core::UnitsClass_t MonoGenerator::getUnitsClass() const
{
  return m_VarTriplets[0].UnitsClass;
}


// =====================================================================
// =====================================================================


/**
  @deprecated Since version 2.1.0. Use openfluid::machine::MonoGenerator::getUnitsClass() const instead
*/
[[deprecated]] openfluid::core::UnitsClass_t MonoGenerator::getUnitClass() const
{
  return getUnitsClass();
}


} } //namespaces

