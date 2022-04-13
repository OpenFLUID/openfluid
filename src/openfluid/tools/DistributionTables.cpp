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
  @file DistributionTables.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/thirdparty/XML.hpp>
#include <openfluid/tools/DistributionTables.hpp>
#include <openfluid/tools/ColumnTextParser.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace tools {


void DistributionTables::build(const std::string& BasePath,
                               const std::string& SourcesFileName, const std::string& DistributionFileName)
{
  SourcesTable.clear();
  UnitsTable.clear();


  // Sources file

  openfluid::thirdparty::xml::XMLDocument Doc;
  std::string SourcesFilePath = openfluid::tools::Filesystem::joinPath({BasePath,SourcesFileName});

  if (Doc.LoadFile(SourcesFilePath.c_str()) == openfluid::thirdparty::xml::XML_SUCCESS)
  {
    const auto Root = Doc.RootElement();

    if (Root != nullptr)
    {
      if (std::string(Root->Name()) == "openfluid")
      {
        for (auto DataElt = Root->FirstChildElement("datasources"); DataElt != nullptr; 
             DataElt = DataElt->NextSiblingElement("datasources"))
        {
          for (auto FileElt = DataElt->FirstChildElement("filesource"); FileElt != nullptr; 
               FileElt = FileElt->NextSiblingElement("filesource"))
          {
            auto ID = openfluid::thirdparty::getXMLAttribute(FileElt,"ID");
            auto File = openfluid::thirdparty::getXMLAttribute(FileElt,"file");

            if (!ID.empty() && !File.empty())
            {
              SourcesTable[ID] = openfluid::tools::Filesystem::joinPath({BasePath,File});
            }
          }
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Cannot find <openfluid> tag in sources file " + SourcesFilePath);
      }
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Wrong formatted sources file " + SourcesFilePath);
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot open sources file " + SourcesFilePath);
  }
 

  // Units distribution file

  long UnitID;
  std::string DataSrcID;

  ColumnTextParser DistriFileParser("%");

  std::string DistributionFilePath = openfluid::tools::Filesystem::joinPath({BasePath,DistributionFileName});

  if (openfluid::tools::FilesystemPath(DistributionFilePath).isFile())
  {
    if (DistriFileParser.loadFromFile(DistributionFilePath) &&
        DistriFileParser.getColsCount() == 2 &&
        DistriFileParser.getLinesCount() >0)
    {
      for (unsigned int i=0;i<DistriFileParser.getLinesCount();i++)
      {
        if (DistriFileParser.getLongValue(i,0,&UnitID) && DistriFileParser.getStringValue(i,1,&DataSrcID))
        {
          if (SourcesTable.find(DataSrcID) != SourcesTable.end())
          {
            UnitsTable[UnitID] = DataSrcID;
          }
          else
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Error in distribution file " + DistributionFilePath +
                                                      ", data source ID not found");
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "Error in distribution file " + DistributionFilePath +
                                                    ", format error");
        }
      }
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Error in distribution file " + DistributionFilePath +
                                                ", file not found or format error");
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Distribution file " + DistributionFilePath + " not found");
  }

}


} }
