/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <libxml/parser.h>
#include <iostream>
#include "OutputReader.h"



// =====================================================================
// =====================================================================


OutputReader::OutputReader()
{
  m_CurrentFilePath = "";
}


// =====================================================================
// =====================================================================


OutputReader::~OutputReader()
{

}


// =====================================================================
// =====================================================================


OutputFilesDescriptor OutputReader::extractFilesDecriptorFromNode(xmlNodePtr NodePtr)
{
  throw openfluid::base::OFException("under construction");
}


// =====================================================================
// =====================================================================


OutputSetDescriptor OutputReader::extractSetDecriptorFromNode(xmlNodePtr NodePtr)
{
  throw openfluid::base::OFException("under construction");
}

// =====================================================================
// =====================================================================



OutputDescriptor OutputReader::readFromFile(std::string OutputFilePath)
{
  OutputDescriptor OD;

  xmlDocPtr Doc = NULL;
  xmlNodePtr Root = NULL;
  xmlNodePtr CurrOutput = NULL;
  xmlNodePtr CurrFiles = NULL;

  m_CurrentFilePath = OutputFilePath;

  Doc = xmlParseFile(OutputFilePath.c_str());

  if (Doc != NULL)
  {
    Root =  xmlDocGetRootElement(Doc);

    if (Root != NULL)
    {
      if (xmlStrcmp(Root->name,(const xmlChar*)"openfluid") == 0)
      {
        CurrOutput = Root->xmlChildrenNode;
        while (CurrOutput != NULL)
        {
           if (xmlStrcmp(CurrOutput->name,(const xmlChar*)"output") == 0)
           {
             CurrFiles = CurrOutput->xmlChildrenNode;
             while (CurrFiles != NULL)
             {
               if (xmlStrcmp(CurrFiles->name,(const xmlChar*)"files") == 0)
               {
                 OD.getFileSets().push_back(extractFilesDecriptorFromNode(CurrFiles));
               }
             }
           }
         }
       }
     }
     else
     {
       throw openfluid::base::OFException("kernel","OutputReader::readFromFile","output config file (" + OutputFilePath + ") is empty");
     }
   }
   else
   {
     throw openfluid::base::OFException("kernel","OutputReader::readFromFile","output config file (" + OutputFilePath + ") cannot be parsed");
   }

   return OD;
}

