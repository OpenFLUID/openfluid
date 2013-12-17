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
  \file DistributionTables.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/tools/DistributionTables.hpp>
#include <openfluid/tools/ColTextParser.hpp>
#include <boost/filesystem/convenience.hpp>

#include <QDomDocument>
#include <QDomElement>
#include <QFile>

namespace openfluid { namespace tools {


void DistributionTables::build(const std::string& BasePath,
                               const std::string& SourcesFileName, const std::string& DistributionFileName)
{
  SourcesTable.clear();
  UnitsTable.clear();


  // Sources file

  QDomDocument Doc;
  QDomElement Root;

  std::string SourcesFilePath = boost::filesystem::path(BasePath+"/"+SourcesFileName).string();


  QFile File(QString(SourcesFilePath.c_str()));
  if (!File.open(QIODevice::ReadOnly))
  {
    throw openfluid::base::FrameworkException("DistributionTables::build",
        "Error opening " + SourcesFilePath);
  }

  bool Parsed = Doc.setContent(&File);
  File.close();


  if (Parsed)
  {
    Root = Doc.documentElement();

    if (!Root.isNull())
    {
      if (Root.tagName() == QString("openfluid"))
      {
        for(QDomElement CurrNode = Root.firstChildElement(); !CurrNode.isNull();
            CurrNode = CurrNode.nextSiblingElement())
        {
          if (CurrNode.tagName() == QString("datasources"))
          {
            for(QDomElement CurrNode2 = CurrNode.firstChildElement(); !CurrNode2.isNull();
                CurrNode2 = CurrNode2.nextSiblingElement())
            {
              if (CurrNode2.tagName() == QString("filesource"))
              {
                QString xmlID = CurrNode2.attributeNode(QString("ID")).value();
                QString xmlFile = CurrNode2.attributeNode(QString("file")).value();

                if (!xmlID.isNull() && !xmlFile.isNull())
                {
                  SourcesTable[xmlID.toStdString()] = boost::filesystem::path(BasePath + "/" + xmlFile.toStdString()).string();
                }
              }
            }
          }
        }
      }
      else
        throw openfluid::base::FrameworkException("DistributionTables::build","Cannot find <openfluid> tag in sources file " +
                                           SourcesFilePath);
    }
    else
      throw openfluid::base::FrameworkException("DistributionTables::build","Wrong formatted sources file " +
                                         SourcesFilePath);
  }
  else
    throw openfluid::base::FrameworkException("DistributionTables::build","Cannot open sources file " +
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
            throw openfluid::base::FrameworkException("DistributionTables::build","Error in distribution file " + DistributionFilePath + ", data source ID not found");
        }
        else
          throw openfluid::base::FrameworkException("DistributionTables::build","Error in distribution file " + DistributionFilePath + ", format error");
      }
    }
    else
      throw openfluid::base::FrameworkException("DistributionTables::build","Error in distribution file " + DistributionFilePath + ", file not found or format error");
  }
  else
    throw openfluid::base::FrameworkException("DistributionTables::build","Distribution file " + DistributionFilePath + " not found");

}


} }
