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
  @file AttributesTableDescriptor.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/fluidx/AttributesTableDescriptor.hpp>
#include <openfluid/tools/ColumnTextParser.hpp>


namespace openfluid { namespace fluidx {


AttributesTableDescriptor::AttributesTableDescriptor() :
  m_UnitsClass("")
{

}


// =====================================================================
// =====================================================================


AttributesTableDescriptor::~AttributesTableDescriptor()
{

}


// =====================================================================
// =====================================================================


void AttributesTableDescriptor::parseDataBlob(const std::string& Data)
{
  m_Data.clear();

  openfluid::tools::ColumnTextParser DataParser("%");

  if (DataParser.setFromString(Data,m_ColumnsOrder.size()+1))
  {
    unsigned int i,j;
    bool IsOK = true;
    long ID;
    std::string Value;

    // parses data in file and loads it in the attribute table for each unit, ordered by columns
    i = 0;
    while (i<DataParser.getLinesCount() && IsOK)
    {
      IsOK = DataParser.getLongValue(i,0,&ID);

      if (IsOK)
      {
        for (j=1;j<DataParser.getColsCount();j++)
        {
          if (DataParser.getStringValue(i,j,&Value))
          {
            m_Data[ID][m_ColumnsOrder[j-1]] = Value;
          }
          else
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Attributes format error");
          }
        }
        i++;
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Attributes format error");
      }
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Error in attributes, cannot be parsed");
  }
}


} } // namespaces
