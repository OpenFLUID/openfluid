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
  @file DatastoreItemDescriptor.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DatastoreItemDescriptor.hpp"

namespace openfluid { namespace fluidx {


// =====================================================================
// =====================================================================


DatastoreItemDescriptor::DatastoreItemDescriptor(const std::string& ID,
                                                 const std::string& PrefixPath,
                                                 const std::string& RelativePath,
                                                 openfluid::core::UnstructuredValue::UnstructuredType Type) :
  m_ID(ID), m_PrefixPath(PrefixPath), m_RelativePath(RelativePath), m_Type(Type), m_UnitClass("")
{

}


// =====================================================================
// =====================================================================


void DatastoreItemDescriptor::setUnitClass(const std::string& UnitClass)
{
  m_UnitClass = UnitClass;
}


// =====================================================================
// =====================================================================


std::string DatastoreItemDescriptor::getID() const
{
  return m_ID;
}


// =====================================================================
// =====================================================================


std::string DatastoreItemDescriptor::getRelativePath() const
{
  return m_RelativePath;
}


// =====================================================================
// =====================================================================


std::string DatastoreItemDescriptor::getPrefixPath() const
{
  return m_PrefixPath;
}


// =====================================================================
// =====================================================================


openfluid::core::UnstructuredValue::UnstructuredType DatastoreItemDescriptor::getType() const
{
  return m_Type;
}


// =====================================================================
// =====================================================================


std::string DatastoreItemDescriptor::getUnitClass() const
{
  return m_UnitClass;
}


// =====================================================================
// =====================================================================

} } // namespaces
