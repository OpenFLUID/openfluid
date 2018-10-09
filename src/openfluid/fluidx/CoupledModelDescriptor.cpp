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
  @file CoupledModelDescriptor.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>


namespace openfluid { namespace fluidx {


openfluid::ware::WareID_t CoupledModelDescriptor::getID(ModelItemDescriptor* Item) const
{
  if (Item->isType(openfluid::ware::WareType::SIMULATOR))
    return (dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(Item))->getID();

  if (Item->isType(openfluid::ware::WareType::GENERATOR))
    return (dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(Item))->getGeneratedID();

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                            "Unknown ware type");
}


// =====================================================================
// =====================================================================


void CoupledModelDescriptor::getItemsCountByType(unsigned int& SimCount, unsigned int& GenCount) const
{
  SimCount = 0;
  GenCount = 0;

  for (auto it = m_SetDescription.begin(); it != m_SetDescription.end(); ++it)
  {
    if ((*it)->getType() == openfluid::ware::WareType::SIMULATOR)
    {
      SimCount++;
    }

    if ((*it)->getType() == openfluid::ware::WareType::GENERATOR)
    {
      GenCount++;
    }
  }
}


} } // namespaces

