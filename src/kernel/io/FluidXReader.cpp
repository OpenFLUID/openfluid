/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file FluidXReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>


#include "FluidXReader.h"


#include "openfluid-tools.h"
#include "AppTools.h"


// =====================================================================
// =====================================================================


FluidXReader::FluidXReader()
{

}


// =====================================================================
// =====================================================================


FluidXReader::~FluidXReader()
{

}


// =====================================================================
// =====================================================================


OutputFilesDescriptor FluidXReader::extractFilesDecriptorFromNode(xmlNodePtr NodePtr)
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


OutputSetDescriptor FluidXReader::extractSetDecriptorFromNode(xmlNodePtr NodePtr)
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
        else throw openfluid::base::OFException("kernel","FluidXReader::extractSetDecriptorFromNode","Wrong format of units IDs in output set config (" + m_CurrentFile + ")");
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
    throw openfluid::base::OFException("kernel","FluidXReader::extractSetDecriptorFromNode","wrong output set definition (" + m_CurrentFile + "). missing attribute?");

  return OSD;

}

// =====================================================================
// =====================================================================


void FluidXReader::extractOutputFromNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"files") == 0)
    {
      m_OutputDescriptor.getFileSets().push_back(extractFilesDecriptorFromNode(CurrNode));
    }
    CurrNode = CurrNode->next;
  }
}


// =====================================================================
// =====================================================================


openfluid::core::FuncParamsMap_t FluidXReader::extractParamsFromNode(xmlNodePtr NodePtr)
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
        else throw openfluid::base::OFException("kernel","FluidXReader::extractParamsFromNode","missing name and/or param attribute(s) in parameter definition (" + m_CurrentFile + ")");
      }
      Curr = Curr->next;
    }
  }

  return Params;
}


// =====================================================================
// =====================================================================


openfluid::core::FuncParamsMap_t FluidXReader::mergeParams(const openfluid::core::FuncParamsMap_t& Params,
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


void FluidXReader::propagateGlobalParamsInModel()
{
  ModelDescriptor::ModelDescription_t::iterator itM;

  ModelItemDescriptor* Item;

  for (itM=m_ModelDescriptor.getItems().begin();itM!=m_ModelDescriptor.getItems().end();++itM)
  {
    openfluid::core::FuncParamsMap_t::iterator itP;

    Item = (*itM);

    Item->setParameters(mergeParams(m_ModelGlobalParams,Item->getParameters()));


    // TODO remove below for cleaning
/*
    std::cout << Item->getParameters().size() << std::endl;

    for (itP = m_ModelGlobalParams.begin(); itP != m_ModelGlobalParams.end(); ++itP)
    {
      std::cout << "param to propagate: " << (*itP).first << "  " << (Item->getParameters().find((*itP).first) == Item->getParameters().end()) << std::endl;

      if (Item->getParameters().find((*itP).first) != Item->getParameters().end())
      {
        Item->getParameters()[(*itP).first] = (*itP).second;
        std::cout << "Propagated param : " << (*itP).first << " -> " << (*itP).second << std::endl;
      }
    }
    */
  }
}


// =====================================================================
// =====================================================================


void FluidXReader::extractModelFromNode(xmlNodePtr NodePtr)
{

  if (m_ModelDefined)
    throw openfluid::base::OFException("kernel","FluidXReader::extractModelFromNode","duplicate model definition (" + m_CurrentFile + ")");

  FunctionDescriptor* FD;
  GeneratorDescriptor* GD;
  openfluid::core::FuncParamsMap_t GParams;


  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;
  while (CurrNode != NULL)
  {

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"gparams") == 0)
    {
      m_ModelGlobalParams = mergeParams(m_ModelGlobalParams,extractParamsFromNode(CurrNode));
    }

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"function") == 0)
    {
      xmlChar* xmlFileID = xmlGetProp(CurrNode,(const xmlChar*)"fileID");

      if (xmlFileID != NULL)
      {

        FD = new FunctionDescriptor((const char*)xmlFileID);
        FD->setParameters(extractParamsFromNode(CurrNode));
        m_ModelDescriptor.appendItem(FD);
      }

    }

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"generator") == 0)
    {

      xmlChar* xmlVarName= xmlGetProp(CurrNode,(const xmlChar*)"varname");
      xmlChar* xmlUnitClass= xmlGetProp(CurrNode,(const xmlChar*)"unitclass");
      xmlChar* xmlMethod= xmlGetProp(CurrNode,(const xmlChar*)"method");
      xmlChar* xmlVarSize= xmlGetProp(CurrNode,(const xmlChar*)"varsize");
      unsigned int VarSize = 1;


      if (xmlVarName != NULL && xmlUnitClass != NULL && xmlMethod != NULL)
      {
        GeneratorDescriptor::GeneratorMethod GenMethod = GeneratorDescriptor::NoGenMethod;
        if (xmlStrcmp(xmlMethod,(const xmlChar*)"fixed") == 0) GenMethod = GeneratorDescriptor::Fixed;
        if (xmlStrcmp(xmlMethod,(const xmlChar*)"random") == 0) GenMethod = GeneratorDescriptor::Random;
        if (xmlStrcmp(xmlMethod,(const xmlChar*)"interp") == 0) GenMethod = GeneratorDescriptor::Interp;

        if (GenMethod == GeneratorDescriptor::NoGenMethod)
          throw openfluid::base::OFException("kernel","FluidXReader::extractModelFromNode","unknown or missing generator method (" + m_CurrentFile + ")");

        if (xmlVarSize != NULL)
        {
          if (!openfluid::tools::ConvertString(std::string((char*)xmlVarSize),&VarSize))
            throw openfluid::base::OFException("kernel","FluidXReader::extractModelFromNode","wrong variable size format in generator (" + m_CurrentFile + ")");
        }

        GD = new GeneratorDescriptor((const char*)xmlVarName,(const char*)xmlUnitClass,GenMethod,VarSize);
        GD->setParameters(extractParamsFromNode(CurrNode));
        m_ModelDescriptor.appendItem(GD);
      }
      else
        throw openfluid::base::OFException("kernel","FluidXReader::extractModelFromNode","missing attribute(s) in generator description (" + m_CurrentFile + ")");
    }

    CurrNode = CurrNode->next;
  }

  m_ModelDefined = true;

}


// =====================================================================
// =====================================================================


void FluidXReader::extractRunFromNode(xmlNodePtr NodePtr)
{

  if (m_RunConfigDefined)
    throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","duplicate run configuration (" + m_CurrentFile + ")");


  bool FoundDeltaT = false;
  bool FoundPeriod = false;

  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

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
          m_RunDescriptor.setBeginDate(ReadDate);
        }
        else
          throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","wrong format for begin date of period (" + m_CurrentFile + ")");

        if (ReadDate.setFromISOString(std::string((char*)xmlEnd)))
        {
          m_RunDescriptor.setEndDate(ReadDate);
        }
        else
          throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","wrong format for end date of period (" + m_CurrentFile + ")");

        FoundPeriod = true;

        if (!(m_RunDescriptor.getEndDate() > m_RunDescriptor.getBeginDate()))
          throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","end date must be strictly greater than begin date for period (" + m_CurrentFile + ")");

      }
      else
        throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","missing begin and/or end attributes for period tag (" + m_CurrentFile + ")");

    }

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"deltat") == 0)
    {

      xmlChar *xmlDeltaT = xmlNodeGetContent(CurrNode);

      if (xmlDeltaT != NULL)
      {
        int DeltaT;
        std::string ReadDeltaTStr = std::string((const char*)xmlDeltaT);


        if (!openfluid::tools::ConvertString(ReadDeltaTStr,&DeltaT))
          throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","empty or wrong value for deltat (" + m_CurrentFile + ")");

        m_RunDescriptor.setDeltaT(DeltaT);
        FoundDeltaT = true;

      }
      else
        throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","deltat tag is empty (" + m_CurrentFile + ")");
    }

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"progressout") == 0)
    {
      xmlChar* xmlPacket= xmlGetProp(CurrNode,(const xmlChar*)"packet");
      xmlChar* xmlKeep= xmlGetProp(CurrNode,(const xmlChar*)"keep");

      if (xmlKeep != NULL && xmlPacket != NULL)
      {
        unsigned int ReadPacket, ReadKeep;

        if (!openfluid::tools::ConvertString(std::string((char*)xmlPacket),&ReadPacket))
          throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","wrong format for packet attribute for progressout tag (" + m_CurrentFile + ")");

        if (!openfluid::tools::ConvertString(std::string((char*)xmlKeep),&ReadKeep))
          throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","wrong format for keep attribute for progressout tag (" + m_CurrentFile + ")");


        if (!(ReadPacket > ReadKeep))
          throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","packet must be strictly greater than keep for progressout (" + m_CurrentFile + ")");

        if (!(ReadKeep > 0))
          throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","keep must be strictly greater than 0 for progressout (" + m_CurrentFile + ")");

        m_RunDescriptor.setProgressiveOutput(ReadPacket,ReadKeep);

      }
      else
        throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","missing keep and/or packet attribute(s) for progressout tag (" + m_CurrentFile + ")");

    }

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"simid") == 0)
    {
      xmlChar *xmlSimID = xmlNodeGetContent(CurrNode);

      if (xmlSimID != NULL)
      {
        m_RunDescriptor.setSimulationID(std::string((char*)xmlSimID));
      }

    }

    CurrNode = CurrNode->next;
  }


  if (!FoundPeriod)
    throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","run period not found (" + m_CurrentFile + ")");

  if (!FoundDeltaT)
    throw openfluid::base::OFException("kernel","FluidXReader::extractRunFromNode","deltat not found (" + m_CurrentFile + ")");

  m_RunConfigDefined = true;


}


// =====================================================================
// =====================================================================


openfluid::core::UnitClassID_t FluidXReader::extractUnitClassIDFromNode(xmlNodePtr NodePtr)
{
  xmlChar* xmlUnitID = xmlGetProp(NodePtr,(const xmlChar*)"ID");
  xmlChar* xmlUnitClass= xmlGetProp(NodePtr,(const xmlChar*)"class");

  if (xmlUnitID != NULL && xmlUnitClass != NULL)
  {
    openfluid::core::UnitID_t UnitID;

    if (!openfluid::tools::ConvertString(std::string((char*)xmlUnitID),&UnitID))
      throw openfluid::base::OFException("kernel","FluidXReader::extractUnitsLinkFromNode","wrong format for ID in unit definition (" + m_CurrentFile + ")");

    return std::make_pair(std::string((char*)xmlUnitClass),UnitID);
  }
  else throw openfluid::base::OFException("kernel","FluidXReader::extractUnitsLinkFromNode","missing or wrong attribute(s) in units link definition (" + m_CurrentFile + ")");

}


// =====================================================================
// =====================================================================


void FluidXReader::extractDomainDefinitionFromNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  while (CurrNode != NULL)
  {

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"unit") == 0)
    {
      xmlChar* xmlUnitID= xmlGetProp(CurrNode,(const xmlChar*)"ID");
      xmlChar* xmlUnitClass= xmlGetProp(CurrNode,(const xmlChar*)"class");
      xmlChar* xmlPcsOrd= xmlGetProp(CurrNode,(const xmlChar*)"pcsorder");

      if (xmlUnitID != NULL && xmlUnitClass != NULL && xmlPcsOrd != NULL)
      {
        UnitDescriptor* UnitDesc = new UnitDescriptor();
        openfluid::core::PcsOrd_t PcsOrder;
        openfluid::core::UnitID_t UnitID;

        UnitDesc->getUnitClass().assign((char*)xmlUnitClass);

        if (!openfluid::tools::ConvertString(std::string((char*)xmlUnitID),&UnitID))
          throw openfluid::base::OFException("kernel","FluidXReader::extractDomainDefinitionFromNode","wrong format for ID in unit definition (" + m_CurrentFile + ")");

        if (!openfluid::tools::ConvertString(std::string((char*)xmlPcsOrd),&PcsOrder))
          throw openfluid::base::OFException("kernel","FluidXReader::extractDomainDefinitionFromNode","wrong format for process order in unit definition (" + m_CurrentFile + ")");

        UnitDesc->getProcessOrder() = PcsOrder;
        UnitDesc->getUnitID() = UnitID;

        xmlNodePtr CurrLinkNode = CurrNode->xmlChildrenNode;

        while (CurrLinkNode != NULL)
        {

          if (xmlStrcmp(CurrLinkNode->name,(const xmlChar*)"to") == 0)
          {
            UnitDesc->getUnitsTos().push_back(extractUnitClassIDFromNode(CurrLinkNode));
          }

          if (xmlStrcmp(CurrLinkNode->name,(const xmlChar*)"in") == 0)
          {
            UnitDesc->getUnitsIns().push_back(extractUnitClassIDFromNode(CurrLinkNode));
          }

          CurrLinkNode = CurrLinkNode->next;
        }

        m_DomainDescriptor.getUnits().push_back(*UnitDesc);

      }
      else throw openfluid::base::OFException("kernel","FluidXReader::extractDomainDefinitionFromNode","missing or wrong attribute(s) in unit definition (" + m_CurrentFile + ")");
    }
    CurrNode = CurrNode->next;
  }
}


// =====================================================================
// =====================================================================


void FluidXReader::extractDomainInputdataFromNode(xmlNodePtr NodePtr)
{

  xmlChar* xmlUnitClass= xmlGetProp(NodePtr,(const xmlChar*)"unitclass");
  xmlChar* xmlColOrder= xmlGetProp(NodePtr,(const xmlChar*)"colorder");

  if (xmlUnitClass != NULL)
  {

    InputDataDescriptor IDataDesc;

    IDataDesc.getUnitsClass().assign((char*)xmlUnitClass);

    if (xmlColOrder != NULL)
    {

      // case 1
      // <inputdata unitclass="someclass" colorder="colA;colB" >
      //    here is the data
      // </inputdata>

      std::vector<std::string> ColOrder;

      ColOrder = SplitString(std::string((char*)xmlColOrder),";");

      if (ColOrder.size() == 0)
        throw openfluid::base::OFException("kernel","FluidXReader::extractDomainInputdataFromNode","wrong or empty colorder attribute in domain input data (" + m_CurrentFile + ")");

      IDataDesc.getColumnsOrder() = ColOrder;

      xmlChar *xmlDataBlob = xmlNodeGetContent(NodePtr);

      if (xmlDataBlob != NULL)
        IDataDesc.getDataBlob().assign((char*)xmlDataBlob);
      else
        throw openfluid::base::OFException("kernel","FluidXReader::extractDomainInputdataFromNode","wrong or empty data content in domain input data (" + m_CurrentFile + ")");

    }
    else
    {

      // case 2 : old deprecated format
      // <inputdata unitclass="someclass">
      //   <columns order="colA;colB" />
      //   <data>
      //     here is the data
      //   </data>
      // </inputdata>


      bool FoundData = false;
      bool FoundColOrder = false;

      xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

      while (CurrNode != NULL)
      {

        if (xmlStrcmp(CurrNode->name,(const xmlChar*)"columns") == 0 && !FoundColOrder)
        {
          FoundColOrder = true;

          xmlChar* xmlOrder= xmlGetProp(CurrNode,(const xmlChar*)"order");

          if (xmlOrder == NULL)
            throw openfluid::base::OFException("kernel","FluidXReader::extractDomainInputdataFromNode","missing or wrong order attribute for columns tag in domain input data (" + m_CurrentFile + ")");

          std::vector<std::string> Order;

          Order = SplitString(std::string((char*)xmlOrder),";");

          if (Order.size() == 0)
            throw openfluid::base::OFException("kernel","FluidXReader::extractDomainInputdataFromNode","wrong or empty order attribute for columns tag for domain input data (" + m_CurrentFile + ")");

          IDataDesc.getColumnsOrder() = Order;

        }

        if (xmlStrcmp(CurrNode->name,(const xmlChar*)"data") == 0 && !FoundData)
        {
          FoundData = true;

          xmlChar *xmlDataBlob = xmlNodeGetContent(CurrNode);

          if (xmlDataBlob != NULL)
            IDataDesc.getDataBlob().assign((char*)xmlDataBlob);
        }

        CurrNode = CurrNode->next;
      }


      if (!(FoundColOrder && FoundData))
         throw openfluid::base::OFException("kernel","FluidXReader::extractDomainInputdataFromNode","missing or wrong domain input data format (" + m_CurrentFile + ")");

    }

    m_DomainDescriptor.getInputData().push_back(IDataDesc);

  }
  else throw openfluid::base::OFException("kernel","FluidXReader::extractDomainInputdataFromNode","missing or wrong unitclass attribute(s) in domain input data (" + m_CurrentFile + ")");


}


// =====================================================================
// =====================================================================


void FluidXReader::extractDomainCalendarFromNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  while (CurrNode != NULL)
  {

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"event") == 0)
    {
      xmlChar* xmlUnitID= xmlGetProp(CurrNode,(const xmlChar*)"unitID");
      xmlChar* xmlUnitClass= xmlGetProp(CurrNode,(const xmlChar*)"unitclass");
      xmlChar* xmlDate= xmlGetProp(CurrNode,(const xmlChar*)"date");

      if (xmlUnitID != NULL && xmlUnitClass != NULL && xmlDate != NULL)
      {

        EventDescriptor EvDesc;

        openfluid::core::UnitID_t UnitID;

        if (!openfluid::tools::ConvertString(std::string((char*)xmlUnitID),&UnitID))
          throw openfluid::base::OFException("kernel","FluidXReader::extractDomainCalendarFromNode","wrong format for unit ID in event (" + m_CurrentFile + ")");

        openfluid::core::DateTime EventDate;

        if (!EventDate.setFromISOString(std::string((char*)xmlDate)))
          throw openfluid::base::OFException("kernel","FluidXReader::extractDomainCalendarFromNode","wrong format for date in event (" + m_CurrentFile + ")");


        EvDesc.getUnitClass().assign((char*)xmlUnitClass);
        EvDesc.getUnitID() = UnitID;
        EvDesc.getEvent() = openfluid::core::Event(EventDate);

        xmlNodePtr CurrInfoNode = CurrNode->xmlChildrenNode;

        while (CurrInfoNode != NULL)
        {
          if (xmlStrcmp(CurrInfoNode->name,(const xmlChar*)"info") == 0)
          {
            xmlChar* xmlKey= xmlGetProp(CurrInfoNode,(const xmlChar*)"key");
            xmlChar* xmlValue= xmlGetProp(CurrInfoNode,(const xmlChar*)"value");

            if (xmlKey != NULL && xmlValue != NULL)
            {
              EvDesc.getEvent().addInfo(std::string((char*)xmlKey),std::string((char*)xmlValue));
            }
            else throw openfluid::base::OFException("kernel","FluidXReader::extractDomainCalendarFromNode","wrong or missing attribute(s) in domain calendar event info (" + m_CurrentFile + ")");
          }

          CurrInfoNode = CurrInfoNode->next;
        }

        m_DomainDescriptor.getEvents().push_back(EvDesc);

      }
      else throw openfluid::base::OFException("kernel","FluidXReader::extractDomainCalendarFromNode","wrong or missing attribute(s) in domain calendar event (" + m_CurrentFile + ")");

    }

    CurrNode = CurrNode->next;
  }
}


// =====================================================================
// =====================================================================


void FluidXReader::extractDomainFomNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;
  while (CurrNode != NULL)
  {

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"definition") == 0)
    {
      extractDomainDefinitionFromNode(CurrNode);
    }

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"inputdata") == 0)
    {
      extractDomainInputdataFromNode(CurrNode);
    }

    if (xmlStrcmp(CurrNode->name,(const xmlChar*)"calendar") == 0)
    {
      extractDomainCalendarFromNode(CurrNode);
    }


    CurrNode = CurrNode->next;

  }
}


// =====================================================================
// =====================================================================


void FluidXReader::parseFile(std::string Filename)
{
  xmlDocPtr Doc = NULL;
  xmlNodePtr Root = NULL;
  xmlNodePtr CurrNode = NULL;

  m_CurrentFile = Filename;


  Doc = xmlParseFile(m_CurrentFile.c_str());

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

          if (xmlStrcmp(CurrNode->name,(const xmlChar*)"run") == 0)
          {
            extractRunFromNode(CurrNode);
          }

          if (xmlStrcmp(CurrNode->name,(const xmlChar*)"model") == 0)
          {
            extractModelFromNode(CurrNode);
          }

          if (xmlStrcmp(CurrNode->name,(const xmlChar*)"output") == 0)
          {
            extractOutputFromNode(CurrNode);
          }

          if (xmlStrcmp(CurrNode->name,(const xmlChar*)"domain") == 0)
          {
            extractDomainFomNode(CurrNode);
          }

          CurrNode = CurrNode->next;
        }
      }
    }
    else
    {
      throw openfluid::base::OFException("kernel","FluidXReader::parseFile","file " + m_CurrentFile + " is empty");
    }
  }
  else
  {
    throw openfluid::base::OFException("kernel","FluidXReader::parseFile","file " + m_CurrentFile + " cannot be parsed");
  }

}


// =====================================================================
// =====================================================================


void FluidXReader::loadFromDirectory(std::string DirPath)
{

  if (!boost::filesystem::is_directory(boost::filesystem::path(DirPath)))
    throw openfluid::base::OFException("kernel","FluidXReader::loadFromDirectory","directory " + DirPath + "does not exist");

  std::vector<std::string> FluidXFilesToLoad = GetFilesByExt(DirPath,"fluidx",true);
  std::vector<std::string> XMLFilesToLoad = GetFilesByExt(DirPath,"xml",true);

  FluidXFilesToLoad.insert(FluidXFilesToLoad.end(),XMLFilesToLoad.begin(),XMLFilesToLoad.end());

  if (FluidXFilesToLoad.size() == 0)
    throw openfluid::base::OFException("kernel","FluidXReader::loadFromDirectory","no fluidx file found in directory " + DirPath);

  std::sort(FluidXFilesToLoad.begin(),FluidXFilesToLoad.end());

  m_ModelDescriptor = ModelDescriptor();
  m_RunDescriptor = RunDescriptor();
  m_OutputDescriptor = OutputDescriptor();
  m_DomainDescriptor = DomainDescriptor();

  m_RunConfigDefined = false;
  m_ModelDefined = false;

  unsigned int i=0;

  std::string CurrentFile;
  boost::filesystem::path CurrentFilePath;

  for (i=0;i<FluidXFilesToLoad.size();i++)
  {
    CurrentFilePath = boost::filesystem::path(FluidXFilesToLoad[i]);
    CurrentFile = CurrentFilePath.string();
    parseFile(CurrentFile);
  }

  propagateGlobalParamsInModel();

}


// =====================================================================
// =====================================================================



