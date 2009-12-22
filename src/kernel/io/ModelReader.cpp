/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <libxml/parser.h>
#include <iostream>
#include "ModelReader.h"
#include "GeneratorDescriptor.h"
#include "FunctionDescriptor.h"


// =====================================================================
// =====================================================================


ModelReader::ModelReader()
{

}


// =====================================================================
// =====================================================================


ModelReader::~ModelReader()
{

}


// =====================================================================
// =====================================================================

openfluid::core::FuncParamsMap_t ModelReader::extractParamsFromNode(xmlNodePtr NodePtr)
{

  openfluid::core::FuncParamsMap_t Params;

  if (NodePtr!=NULL)
  {
    xmlNodePtr Curr = NULL;

    Curr = NodePtr->xmlChildrenNode;
    while (Curr != NULL)
    {
      if (xmlStrcmp(Curr->name,(const xmlChar*)"param") == 0)
      {
        xmlChar* xmlKey = xmlGetProp(Curr,(const xmlChar*)"name");
        xmlChar* xmlValue = xmlGetProp(Curr,(const xmlChar*)"value");

        if (xmlKey != NULL && xmlValue != NULL)
        {
          Params[(const char*)xmlKey] = (const char*)xmlValue;
          xmlFree(xmlKey);
          xmlFree(xmlValue);
        }
      }
      Curr = Curr->next;
    }
  }

  return Params;
}

// =====================================================================
// =====================================================================

openfluid::core::FuncParamsMap_t ModelReader::mergeParams(const openfluid::core::FuncParamsMap_t& Params,
                                                          const openfluid::core::FuncParamsMap_t& OverloadParams)
{
  openfluid::core::FuncParamsMap_t FinalParams = Params;

  openfluid::core::FuncParamsMap_t::const_iterator it;

  for (it=OverloadParams.begin();it!=OverloadParams.end();++it)
  {
    FinalParams[(*it).first] = (*it).second;
  }

  return FinalParams;
}



// =====================================================================
// =====================================================================


ModelDescriptor ModelReader::readFromFile(std::string ModelFilePath)
{
  ModelDescriptor MD;
  FunctionDescriptor *FD;
  GeneratorDescriptor *GD;

  openfluid::core::FuncParamsMap_t GlobalParams;

  xmlDocPtr Doc = NULL;
  xmlNodePtr Curr = NULL;
  xmlNodePtr CurrGParams = NULL;
  xmlNodePtr CurrModel = NULL;
  xmlNodePtr CurrItem = NULL;

  Doc = xmlParseFile(ModelFilePath.c_str());

  if (Doc != NULL)
  {
    Curr =  xmlDocGetRootElement(Doc);

    if (Curr != NULL)
    {
      if (xmlStrcmp(Curr->name,(const xmlChar*)"openfluid") == 0)
      {
        // first pass for global parameters
        CurrGParams = Curr->xmlChildrenNode;
        while (CurrGParams != NULL)
        {

          if (xmlStrcmp(CurrGParams->name,(const xmlChar*)"gparams") == 0)
          {
            GlobalParams = extractParamsFromNode(CurrGParams);
          }
          CurrGParams = CurrGParams->next;
        }

        // second pass for model
        CurrModel = Curr->xmlChildrenNode;
        while (CurrModel != NULL)
        {

          if (xmlStrcmp(CurrModel->name,(const xmlChar*)"model") == 0)
          {
            CurrItem = CurrModel->xmlChildrenNode;
            while (CurrItem != NULL)
            {

              if (xmlStrcmp(CurrItem->name,(const xmlChar*)"function") == 0)
              {
                xmlChar* xmlFileID = xmlGetProp(CurrItem,(const xmlChar*)"fileID");

                if (xmlFileID != NULL)
                {

                  FD = new FunctionDescriptor((const char*)xmlFileID);
                  FD->setParameters(mergeParams(GlobalParams,extractParamsFromNode(CurrItem)));
                  MD.appendItem(FD);
                }

              }

              if (xmlStrcmp(CurrItem->name,(const xmlChar*)"generator") == 0)
              {

                xmlChar* xmlVarName= xmlGetProp(CurrItem,(const xmlChar*)"varname");
                xmlChar* xmlUnitClass= xmlGetProp(CurrItem,(const xmlChar*)"unitclass");
                xmlChar* xmlMethod= xmlGetProp(CurrItem,(const xmlChar*)"method");

                if (xmlVarName != NULL && xmlUnitClass != NULL && xmlMethod != NULL)
                {
                  extractParamsFromNode(CurrItem);

                  GeneratorDescriptor::GeneratorMethod GenMethod = GeneratorDescriptor::NoGenMethod;
                  if (xmlStrcmp(xmlMethod,(const xmlChar*)"fixed") == 0) GenMethod = GeneratorDescriptor::Fixed;
                  if (xmlStrcmp(xmlMethod,(const xmlChar*)"random") == 0) GenMethod = GeneratorDescriptor::Random;
                  if (xmlStrcmp(xmlMethod,(const xmlChar*)"interp") == 0) GenMethod = GeneratorDescriptor::Interp;

                  GD = new GeneratorDescriptor((const char*)xmlVarName,(const char*)xmlUnitClass,GenMethod);
                  GD->setParameters(mergeParams(GlobalParams,extractParamsFromNode(CurrItem)));
                  MD.appendItem(GD);
                }

              }

              CurrItem = CurrItem->next;
            }

          }
          CurrModel = CurrModel->next;
        }
      }
    }
    else
    {
      throw openfluid::base::OFException("kernel","ModelReader::readFromFile","model config file (" + ModelFilePath + ") is empty");
    }
  }
  else
  {
    throw openfluid::base::OFException("kernel","ModelReader::readFromFile","model config file (" + ModelFilePath + ") cannot be parsed");
  }

  return MD;
}

