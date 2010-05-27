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
  \file DataSrcFile.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <libxml/parser.h>

#include <openfluid/tools/DataSrcFile.hpp>
#include <openfluid/tools/SwissTools.hpp>



namespace openfluid { namespace tools {


DataSourcesFile::DataSourcesFile()
{
  m_Loaded = false;
  m_IDs.clear();
  m_Sources.clear();

}

// =====================================================================
// =====================================================================


DataSourcesFile::~DataSourcesFile()
{

}

// =====================================================================
// =====================================================================



bool DataSourcesFile::load(std::string Filename)
{

  xmlDocPtr Doc = NULL;
  xmlNodePtr Root = NULL;
  xmlNodePtr CurrNode = NULL;
  xmlNodePtr CurrNode2 = NULL;


  m_Loaded = true;
  m_IDs.clear();
  m_Sources.clear();

  Doc = xmlParseFile(Filename.c_str());

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
                int ID;

                if (xmlID != NULL && xmlFile != NULL && openfluid::tools::ConvertString(std::string((char*)xmlID),&ID))
                {
                  m_Sources[ID] = std::string((char*)xmlFile);
                  m_IDs.push_back(ID);
                }

              }
              CurrNode2 = CurrNode2->next;
            }
          }
          CurrNode = CurrNode->next;
        }
      }
      else m_Loaded = false;
    }
    else m_Loaded = false;
  }
  else m_Loaded = false;

  if (!m_Loaded)
  {
    m_IDs.clear();
    m_Sources.clear();
  }

  return m_Loaded;

}

// =====================================================================
// =====================================================================


std::vector<int> DataSourcesFile::getIDs()
{
  return m_IDs;
}

// =====================================================================
// =====================================================================


std::string DataSourcesFile::getSource(int ID)
{
  IDDataSourcesMap::iterator iter;

  iter = m_Sources.find(ID);

  if (iter != m_Sources.end()) return iter->second;
  else return "";
}




} } // namespace

