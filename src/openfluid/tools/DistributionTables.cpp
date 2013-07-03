/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file DistributionTables.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <libxml/parser.h>

#include <openfluid/tools/DistributionTables.hpp>
#include <openfluid/tools/ColTextParser.hpp>
#include <boost/filesystem/convenience.hpp>

namespace openfluid { namespace tools {


void DistributionTables::build(const std::string& BasePath,
                               const std::string& SourcesFileName, const std::string& DistributionFileName)
{
  SourcesTable.clear();
  UnitsTable.clear();


  // Sources file

  xmlDocPtr Doc = NULL;
  xmlNodePtr Root = NULL;
  xmlNodePtr CurrNode = NULL;
  xmlNodePtr CurrNode2 = NULL;

  std::string SourcesFilePath = boost::filesystem::path(BasePath+"/"+SourcesFileName).string();


  Doc = xmlParseFile(SourcesFilePath.c_str());

  if (Doc != NULL)
  {
    Root =  xmlDocGetRootElement(Doc);

    if (Root != NULL)
    {
      if (xmlStrcmp(Root->name,(const xmlChar*)"openfluid") == 0)
      {
        CurrNode = Root->xmlChildrenNode;
        while (CurrNode != NULL)
        {
          if (xmlStrcmp(CurrNode->name,(const xmlChar*)"datasources") == 0)
          {
            CurrNode2 = CurrNode->xmlChildrenNode;
            while (CurrNode2 != NULL)
            {

              if (xmlStrcmp(CurrNode2->name,(const xmlChar*)"filesource") == 0)
              {
                xmlChar* xmlID= xmlGetProp(CurrNode2,(const xmlChar*)"ID");
                xmlChar* xmlFile= xmlGetProp(CurrNode2,(const xmlChar*)"file");

                if (xmlID != NULL && xmlFile != NULL)
                {
                  SourcesTable[std::string((char*)xmlID)] = boost::filesystem::path(BasePath + "/" + std::string((char*)xmlFile)).string();
                }
              }
              CurrNode2 = CurrNode2->next;
            }
          }
          CurrNode = CurrNode->next;
        }
      }
      else
        throw openfluid::base::OFException("OpenFLUID framework","DistributionTables::build","Cannot find <openfluid> tag in sources file " +
                                           SourcesFilePath);
    }
    else
      throw openfluid::base::OFException("OpenFLUID framework","DistributionTables::build","Wrong formatted sources file " +
                                         SourcesFilePath);
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework","DistributionTables::build","Cannot open sources file " +
                                       SourcesFilePath);





  // Units distribution file


  long UnitID;
  std::string DataSrcID;

  ColumnTextParser DistriFileParser("%");

  std::string DistributionFilePath = boost::filesystem::path(BasePath+"/"+DistributionFileName).string();

  if (boost::filesystem::exists(boost::filesystem::path(DistributionFilePath)))
  {
    if ((DistriFileParser.loadFromFile(DistributionFilePath)) && (DistriFileParser.getColsCount() == 2) && (DistriFileParser.getLinesCount() >0))
    {
      for (unsigned int i=0;i<DistriFileParser.getLinesCount();i++)
      {
        if (DistriFileParser.getLongValue(i,0,&UnitID) && DistriFileParser.getStringValue(i,1,&DataSrcID))
        {
          if (SourcesTable.find(DataSrcID) != SourcesTable.end())
            UnitsTable[UnitID] = DataSrcID;
          else
            throw openfluid::base::OFException("OpenFLUID framework","DistributionTables::build","Error in distribution file " + DistributionFilePath + ", data source ID not found");
        }
        else
          throw openfluid::base::OFException("OpenFLUID framework","DistributionTables::build","Error in distribution file " + DistributionFilePath + ", format error");
      }
    }
    else
      throw openfluid::base::OFException("OpenFLUID framework","DistributionTables::build","Error in distribution file " + DistributionFilePath + ", file not found or format error");
  }
  else
    throw openfluid::base::OFException("OpenFLUID framework","DistributionTables::build","Distribution file " + DistributionFilePath + " not found");

}


} }
