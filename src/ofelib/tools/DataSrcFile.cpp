/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DataSrcFile.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <libxml/parser.h>

#include "DataSrcFile.h"
#include "SwissTools.h"



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

