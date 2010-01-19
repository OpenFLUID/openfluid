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

#include "AppTools.h"
#include "openfluid-tools.h"
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
  OutputFilesDescriptor OFD;

  xmlChar* xmlColSep = xmlGetProp(NodePtr,(const xmlChar*)"colsep");
  if (xmlColSep != NULL) OFD.setColSeparator(std::string((const char*)xmlColSep));

  xmlChar* xmlDTFormat = xmlGetProp(NodePtr,(const xmlChar*)"dtformat");
  if (xmlDTFormat != NULL) OFD.setDateFormat(std::string((const char*)xmlDTFormat));

  xmlChar* xmlCommentChar = xmlGetProp(NodePtr,(const xmlChar*)"commentchar");
  if (xmlCommentChar != NULL) OFD.setCommentChar(std::string((const char*)xmlCommentChar));

  if (OFD.getDateFormat() == "6cols") OFD.setDateFormat("%Y\t%m\t%d\t%H\t%M\t%S");
  if (OFD.getDateFormat() == "iso") OFD.setDateFormat("%Y%m%dT%H%M%S");


  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"set") == 0)
    {
      OFD.getSets().push_back(extractSetDecriptorFromNode(CurrNode));
    }
    CurrNode = CurrNode->next;
  }

  return OFD;
}


// =====================================================================
// =====================================================================


OutputSetDescriptor OutputReader::extractSetDecriptorFromNode(xmlNodePtr NodePtr)
{
  OutputSetDescriptor OSD;


  xmlChar* xmlName = xmlGetProp(NodePtr,(const xmlChar*)"name");
  xmlChar* xmlUnitsClass = xmlGetProp(NodePtr,(const xmlChar*)"unitsclass");
  xmlChar* xmlUnitsIDs = xmlGetProp(NodePtr,(const xmlChar*)"unitsIDs");
  xmlChar* xmlVars = xmlGetProp(NodePtr,(const xmlChar*)"vars");


  if (xmlName != NULL && xmlUnitsClass != NULL && xmlUnitsIDs != NULL && xmlVars != NULL)
  {
    OSD.setName(std::string((const char*)xmlName));
    OSD.setUnitsClass(std::string((const char*)xmlUnitsClass));

    std::string UnitsIDsStr = std::string((const char*)xmlUnitsIDs);
    std::string VarsStr = std::string((const char*)xmlVars);

    if (UnitsIDsStr == "*") OSD.setAllUnits(true);
    else
    {
      OSD.setAllUnits(false);
      openfluid::core::UnitID_t UID;

      std::vector<std::string> StrArray = SplitString(UnitsIDsStr,";");
      for (unsigned int i=0;i<StrArray.size();i++)
      {
        if (openfluid::tools::ConvertString(StrArray[i],&UID)) OSD.getUnitsIDs().push_back(UID);
        else throw openfluid::base::OFException("kernel","OutputReader::extractSetDecriptorFromNode","Wrong format of units IDs in output config file (" + m_CurrentFilePath + ")");
      }
    }

    if (VarsStr == "*")
    {
      OSD.setAllScalars(true);
      OSD.setAllVectors(true);
    }
    else
    {
      OSD.setAllScalars(false);
      OSD.setAllVectors(false);

      std::vector<std::string> StrArray = SplitString(VarsStr,";");
      for (unsigned int i=0;i<StrArray.size();i++)
      {
        if (IsVectorNamedVariable(StrArray[i])) OSD.getVectors().push_back(GetVectorNamedVariableName(StrArray[i]));
        else OSD.getScalars().push_back(StrArray[i]);
      }

    }


  }
  else
    throw openfluid::base::OFException("kernel","OutputReader::extractSetDecriptorFromNode","wrong output set definition in output config file (" + m_CurrentFilePath + "). missing attribute?");

  return OSD;

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
               CurrFiles = CurrFiles->next;
             }
           }
           CurrOutput = CurrOutput->next;
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

