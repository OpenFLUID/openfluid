/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file RunReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <libxml/tree.h>
#include <libxml/parser.h>

#include "openfluid-tools.h"
#include "RunReader.h"


// =====================================================================
// =====================================================================


RunReader::RunReader()
{

}


// =====================================================================
// =====================================================================


RunReader::~RunReader()
{

}


// =====================================================================
// =====================================================================


RunDescriptor RunReader::readFromFile(std::string RunFilePath)
{
  RunDescriptor RD;

  xmlDocPtr Doc = NULL;
  xmlNodePtr Curr = NULL;
  xmlNodePtr CurrRun = NULL;
  xmlNodePtr CurrNode = NULL;

  bool FoundDeltaT = false;
  bool FoundPeriod = false;


  Doc = xmlParseFile(RunFilePath.c_str());

  if (Doc != NULL)
  {
    Curr =  xmlDocGetRootElement(Doc);

    if (Curr != NULL)
    {
      if (xmlStrcmp(Curr->name,(const xmlChar*)"openfluid") == 0)
      {

        CurrRun = Curr->xmlChildrenNode;
        while (CurrRun != NULL)
        {

          if (xmlStrcmp(CurrRun->name,(const xmlChar*)"run") == 0)
          {
            CurrNode = CurrRun->xmlChildrenNode;
            while (CurrNode != NULL)
            {

              if (xmlStrcmp(CurrNode->name,(const xmlChar*)"period") == 0)
              {
                xmlChar* xmlBegin= xmlGetProp(CurrNode,(const xmlChar*)"begin");
                xmlChar* xmlEnd= xmlGetProp(CurrNode,(const xmlChar*)"end");

                if (xmlBegin != NULL && xmlEnd != NULL)
                {
                  openfluid::core::DateTime ReadDate;

                  if (ReadDate.setFromISOString(std::string((char*)xmlBegin)))
                  {
                    RD.setBeginDate(ReadDate);
                  }
                  else
                    throw openfluid::base::OFException("kernel","RunReader::readFromFile","wrong format for begin date of period in run config file (" + RunFilePath + ")");

                  if (ReadDate.setFromISOString(std::string((char*)xmlEnd)))
                  {
                    RD.setEndDate(ReadDate);
                  }
                  else
                    throw openfluid::base::OFException("kernel","RunReader::readFromFile","wrong format for end date of period in run config file (" + RunFilePath + ")");

                  FoundPeriod = true;

                  if (!(RD.getEndDate() > RD.getBeginDate()))
                    throw openfluid::base::OFException("kernel","RunReader::readFromFile","end date must be strictly greater than begin date for period in run config file (" + RunFilePath + ")");

                }
                else
                  throw openfluid::base::OFException("kernel","RunReader::readFromFile","missing begin and/or end attributes for period tag in run config file (" + RunFilePath + ")");

              }

              if (xmlStrcmp(CurrNode->name,(const xmlChar*)"deltat") == 0)
              {
//                std::cout << "deltat str: " << CurrNode->type << " " << XML_TEXT_NODE <<  << std::endl;

                xmlChar *xmlDeltaT = xmlNodeGetContent(CurrNode);

                if (xmlDeltaT != NULL)
                {
                  int DeltaT;
                  std::string ReadDeltaTStr = std::string((const char*)xmlDeltaT);

//                  std::cout << "deltat : " << ReadDeltaTStr << std::endl;

                  if (!openfluid::tools::ConvertString(ReadDeltaTStr,&DeltaT))
                    throw openfluid::base::OFException("kernel","RunReader::readFromFile","empty or wrong value for deltat in run config file (" + RunFilePath + ")");

                  RD.setDeltaT(DeltaT);
                  FoundDeltaT = true;

                }
                else
                  throw openfluid::base::OFException("kernel","RunReader::readFromFile","deltat tag is empty in run config file (" + RunFilePath + ")");
              }

              if (xmlStrcmp(CurrNode->name,(const xmlChar*)"progressout") == 0)
              {
                xmlChar* xmlPacket= xmlGetProp(CurrNode,(const xmlChar*)"packet");
                xmlChar* xmlKeep= xmlGetProp(CurrNode,(const xmlChar*)"keep");

                if (xmlKeep != NULL && xmlPacket != NULL)
                {
                  unsigned int ReadPacket, ReadKeep;

                  if (!openfluid::tools::ConvertString(std::string((char*)xmlPacket),&ReadPacket))
                    throw openfluid::base::OFException("kernel","RunReader::readFromFile","wrong format for packet attribute for progressout tag in run config file (" + RunFilePath + ")");

                  if (!openfluid::tools::ConvertString(std::string((char*)xmlKeep),&ReadKeep))
                    throw openfluid::base::OFException("kernel","RunReader::readFromFile","wrong format for keep attribute for progressout tag in run config file (" + RunFilePath + ")");


                  if (!(ReadPacket > ReadKeep))
                    throw openfluid::base::OFException("kernel","RunReader::readFromFile","packet must be strictly greater than keep for progressout in run config file (" + RunFilePath + ")");

                  if (!(ReadKeep > 0))
                    throw openfluid::base::OFException("kernel","RunReader::readFromFile","keep must be strictly greater than 0 for progressout in run config file (" + RunFilePath + ")");

                  RD.setProgressiveOutput(ReadPacket,ReadKeep);

                }
                else
                  throw openfluid::base::OFException("kernel","RunReader::readFromFile","missing keep and/or packet attribute(s) for progressout tag in run config file (" + RunFilePath + ")");

              }

              if (xmlStrcmp(CurrNode->name,(const xmlChar*)"simid") == 0)
              {
                xmlChar *xmlSimID = xmlNodeGetContent(CurrNode);

                if (xmlSimID != NULL)
                {
                  RD.setSimulationID(std::string((char*)xmlSimID));
                }

              }

              CurrNode = CurrNode->next;
            }

          }
          CurrRun = CurrRun->next;
        }
      }
    }
    else
    {
      throw openfluid::base::OFException("kernel","RunReader::readFromFile","run config file (" + RunFilePath + ") is empty");
    }
  }
  else
  {
    throw openfluid::base::OFException("kernel","RunReader::readFromFile","run config file (" + RunFilePath + ") cannot be parsed");
  }

  if (!FoundPeriod)
    throw openfluid::base::OFException("kernel","RunReader::readFromFile","period not found in run config file (" + RunFilePath + ")");

  if (!FoundDeltaT)
    throw openfluid::base::OFException("kernel","RunReader::readFromFile","deltat not found in run config file (" + RunFilePath + ")");


  return RD;
}
