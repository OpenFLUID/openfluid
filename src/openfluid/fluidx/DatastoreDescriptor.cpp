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
  @file DatastoreDescriptor.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
 */


#include "DatastoreDescriptor.hpp"


namespace openfluid { namespace fluidx {


DatastoreDescriptor::DatastoreDescriptor()
{
}


// =====================================================================
// =====================================================================


DatastoreDescriptor::~DatastoreDescriptor()
{
  // TODO Manage deletion of items, in relation with advanced descriptors
}


// =====================================================================
// =====================================================================


DatastoreDescriptor::DatastoreDescription_t& DatastoreDescriptor::items()
{
  return m_DatastoreDescription;
}


// =====================================================================
// =====================================================================


const DatastoreDescriptor::DatastoreDescription_t& DatastoreDescriptor::items() const
{
  return m_DatastoreDescription;
}


// =====================================================================
// =====================================================================


bool DatastoreDescriptor::appendItem(DatastoreItemDescriptor* Item)
{
  if (isItemIDAlreadyExist(Item->getID()))
    return false;

  m_DatastoreDescription.push_back(Item);
  return true;
}


// =====================================================================
// =====================================================================


bool DatastoreDescriptor::isItemIDAlreadyExist(const std::string& ItemID) const
{
  for (std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator it =
      m_DatastoreDescription.begin(); it != m_DatastoreDescription.end(); ++it)
  {
    if ((*it)->getID() == ItemID)
      return true;
  }

  return false;
}


}  } // namespaces
