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
 \file FluidXDescriptor.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "FluidXDescriptor.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/fluidx/FunctionDescriptor.hpp>

namespace openfluid {
namespace fluidx {

// =====================================================================
// =====================================================================

FluidXDescriptor::FluidXDescriptor(openfluid::base::IOListener* Listener) :
    mp_Listener(Listener), m_IndentStr(" ")
{
  if (!mp_Listener)
    mp_Listener = new openfluid::base::IOListener();
}

// =====================================================================
// =====================================================================

FluidXDescriptor::~FluidXDescriptor()
{
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractMonitoringFromNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  openfluid::fluidx::ObserverDescriptor* OD;

  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "observer") == 0)
    {
      xmlChar* xmlID = xmlGetProp(CurrNode, (const xmlChar*) "ID");

      if (xmlID != NULL)
      {
        OD = new openfluid::fluidx::ObserverDescriptor((const char*) xmlID);
        OD->setParameters(extractParamsFromNode(CurrNode));
        m_MonitoringDescriptor.appendItem(OD);
      }
    }
    CurrNode = CurrNode->next;
  }
}

// =====================================================================
// =====================================================================

openfluid::ware::WareParams_t FluidXDescriptor::extractParamsFromNode(
    xmlNodePtr NodePtr)
{
  openfluid::ware::WareParams_t Params;

  if (NodePtr != NULL)
  {
    xmlNodePtr Curr = NULL;

    Curr = NodePtr->xmlChildrenNode;
    while (Curr != NULL)
    {
      if (xmlStrcmp(Curr->name, (const xmlChar*) "param") == 0)
      {
        xmlChar* xmlKey = xmlGetProp(Curr, (const xmlChar*) "name");
        xmlChar* xmlValue = xmlGetProp(Curr, (const xmlChar*) "value");

        if (xmlKey != NULL && xmlValue != NULL)
        {
          Params.put((const char*) xmlKey,
                     openfluid::ware::WareParamValue_t((const char*) xmlValue));
          xmlFree(xmlKey);
          xmlFree(xmlValue);
        }
        else
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractParamsFromNode",
              "missing name and/or param attribute(s) in parameter definition (" + m_CurrentFile
              + ")");
      }
      Curr = Curr->next;
    }
  }

  return Params;
}

// =====================================================================
// =====================================================================

openfluid::ware::WareParams_t FluidXDescriptor::mergeParams(
    const openfluid::ware::WareParams_t& Params,
    const openfluid::ware::WareParams_t& OverloadParams)
{
  openfluid::ware::WareParams_t FinalParams = Params;

  std::map<std::string, std::string> OverloadParamsMap =
      openfluid::fluidx::WareDescriptor::getParamsAsMap(OverloadParams);
  std::map<std::string, std::string>::iterator it;

  for (it = OverloadParamsMap.begin(); it != OverloadParamsMap.end(); ++it)
  {
    FinalParams.put(it->first, it->second);
  }

  return FinalParams;
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractModelFromNode(xmlNodePtr NodePtr)
{
  if (m_ModelDefined)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "FluidXDescriptor::extractModelFromNode",
        "Duplicate model definition (" + m_CurrentFile + ")");

  openfluid::fluidx::FunctionDescriptor* FD;
  openfluid::fluidx::GeneratorDescriptor* GD;
  openfluid::ware::WareParams_t GParams;

  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;
  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "gparams") == 0)
    {
      GParams = mergeParams(GParams, extractParamsFromNode(CurrNode));
    }

    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "function") == 0)
    {
      xmlChar* xmlID = xmlGetProp(CurrNode, (const xmlChar*) "ID");

      if (xmlID != NULL)
      {

        FD = new openfluid::fluidx::FunctionDescriptor((const char*) xmlID);
        FD->setParameters(extractParamsFromNode(CurrNode));
        m_ModelDescriptor.appendItem(FD);
      }
    }

    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "generator") == 0)
    {
      xmlChar* xmlVarName = xmlGetProp(CurrNode, (const xmlChar*) "varname");
      xmlChar* xmlUnitClass = xmlGetProp(CurrNode,
                                         (const xmlChar*) "unitclass");
      xmlChar* xmlMethod = xmlGetProp(CurrNode, (const xmlChar*) "method");
      xmlChar* xmlVarSize = xmlGetProp(CurrNode, (const xmlChar*) "varsize");
      unsigned int VarSize = 1;

      if (xmlVarName != NULL && xmlUnitClass != NULL && xmlMethod != NULL)
      {
        openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GenMethod =
            openfluid::fluidx::GeneratorDescriptor::NoGenMethod;
        if (xmlStrcmp(xmlMethod, (const xmlChar*) "fixed") == 0)
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Fixed;
        if (xmlStrcmp(xmlMethod, (const xmlChar*) "random") == 0)
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Random;
        if (xmlStrcmp(xmlMethod, (const xmlChar*) "interp") == 0)
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Interp;
        if (xmlStrcmp(xmlMethod, (const xmlChar*) "inject") == 0)
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Inject;

        if (GenMethod == openfluid::fluidx::GeneratorDescriptor::NoGenMethod)
          throw openfluid::base::OFException(
              "OpenFLUID framework", "FluidXDescriptor::extractModelFromNode",
              "unknown or missing generator method (" + m_CurrentFile + ")");

        if (xmlVarSize != NULL)
        {
          if (!openfluid::tools::ConvertString(std::string((char*) xmlVarSize),
                                               &VarSize))
            throw openfluid::base::OFException(
                "OpenFLUID framework",
                "FluidXDescriptor::extractModelFromNode",
                "wrong variable size format in generator (" + m_CurrentFile
                + ")");
        }

        GD = new openfluid::fluidx::GeneratorDescriptor(
            (const char*) xmlVarName, (const char*) xmlUnitClass, GenMethod,
            VarSize);
        GD->setParameters(extractParamsFromNode(CurrNode));
        m_ModelDescriptor.appendItem(GD);
      }
      else
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractModelFromNode",
            "missing attribute(s) in generator description (" + m_CurrentFile
            + ")");
    }

    CurrNode = CurrNode->next;
  }

  m_ModelDescriptor.setGlobalParameters(GParams);
  m_ModelDefined = true;
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractRunFromNode(xmlNodePtr NodePtr)
{
  if (m_RunConfigDefined)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "FluidXDescriptor::extractRunFromNode",
        "Duplicate run configuration (" + m_CurrentFile + ")");

  bool FoundDeltaT = false;
  bool FoundPeriod = false;

  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "period") == 0)
    {
      xmlChar* xmlBegin = xmlGetProp(CurrNode, (const xmlChar*) "begin");
      xmlChar* xmlEnd = xmlGetProp(CurrNode, (const xmlChar*) "end");

      if (xmlBegin != NULL && xmlEnd != NULL)
      {
        openfluid::core::DateTime ReadDate;

        if (ReadDate.setFromISOString(std::string((char*) xmlBegin)))
        {
          m_RunDescriptor.setBeginDate(ReadDate);
        }
        else
          throw openfluid::base::OFException(
              "OpenFLUID framework", "FluidXDescriptor::extractRunFromNode",
              "wrong format for begin date of period (" + m_CurrentFile + ")");

        if (ReadDate.setFromISOString(std::string((char*) xmlEnd)))
        {
          m_RunDescriptor.setEndDate(ReadDate);
        }
        else
          throw openfluid::base::OFException(
              "OpenFLUID framework", "FluidXDescriptor::extractRunFromNode",
              "wrong format for end date of period (" + m_CurrentFile + ")");

        FoundPeriod = true;

        if (!(m_RunDescriptor.getEndDate() > m_RunDescriptor.getBeginDate()))
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractRunFromNode",
              "end date must be strictly greater than begin date for period (" + m_CurrentFile
              + ")");
      }
      else
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractRunFromNode",
            "missing begin and/or end attributes for period tag (" + m_CurrentFile
            + ")");
    }

    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "deltat") == 0)
    {
      xmlChar *xmlDeltaT = xmlNodeGetContent(CurrNode);

      if (xmlDeltaT != NULL)
      {
        int DeltaT;
        std::string ReadDeltaTStr = std::string((const char*) xmlDeltaT);

        if (!openfluid::tools::ConvertString(ReadDeltaTStr, &DeltaT))
          throw openfluid::base::OFException(
              "OpenFLUID framework", "FluidXDescriptor::extractRunFromNode",
              "empty or wrong value for deltat (" + m_CurrentFile + ")");

        m_RunDescriptor.setDeltaT(DeltaT);
        FoundDeltaT = true;
      }
      else
        throw openfluid::base::OFException(
            "OpenFLUID framework", "FluidXDescriptor::extractRunFromNode",
            "deltat tag is empty (" + m_CurrentFile + ")");
    }

    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "valuesbuffer") == 0)
    {
      xmlChar* xmlSteps = xmlGetProp(CurrNode, (const xmlChar*) "steps");

      if (xmlSteps != NULL)
      {
        unsigned int ReadSteps;

        if (!openfluid::tools::ConvertString(std::string((char*) xmlSteps),
                                             &ReadSteps))
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractRunFromNode",
              "wrong format for steps attribute for valuesbuffer tag (" + m_CurrentFile
              + ")");

        m_RunDescriptor.setValuesBufferSize(ReadSteps);
      }
      else
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractRunFromNode",
            "missing steps attribute for valuesbuffer tag (" + m_CurrentFile
            + ")");
    }

    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "filesbuffer") == 0)
    {
      xmlChar* xmlKBytes = xmlGetProp(CurrNode, (const xmlChar*) "kbytes");

      if (xmlKBytes != NULL)
      {
        unsigned int ReadKBytes;

        if (!openfluid::tools::ConvertString(std::string((char*) xmlKBytes),
                                             &ReadKBytes))
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractRunFromNode",
              "wrong format for kbytes attribute for filesbuffer tag (" + m_CurrentFile
              + ")");

        m_RunDescriptor.setFilesBufferSizeInKB(ReadKBytes);
      }
      else
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractRunFromNode",
            "missing kbytes attribute for filesbuffer tag (" + m_CurrentFile
            + ")");
    }

    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "simid") == 0)
    {
      xmlChar *xmlSimID = xmlNodeGetContent(CurrNode);

      if (xmlSimID != NULL)
      {
        m_RunDescriptor.setSimulationID(std::string((char*) xmlSimID));
      }

    }

    CurrNode = CurrNode->next;
  }

  if (!FoundPeriod)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "FluidXDescriptor::extractRunFromNode",
        "run period not found (" + m_CurrentFile + ")");

  if (!FoundDeltaT)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "FluidXDescriptor::extractRunFromNode",
        "deltat not found (" + m_CurrentFile + ")");

  m_RunConfigDefined = true;

  m_RunDescriptor.setFilled(true);
}

// =====================================================================
// =====================================================================

openfluid::core::UnitClassID_t FluidXDescriptor::extractUnitClassIDFromNode(
    xmlNodePtr NodePtr)
{
  xmlChar* xmlUnitID = xmlGetProp(NodePtr, (const xmlChar*) "ID");
  xmlChar* xmlUnitClass = xmlGetProp(NodePtr, (const xmlChar*) "class");

  if (xmlUnitID != NULL && xmlUnitClass != NULL)
  {
    openfluid::core::UnitID_t UnitID;

    if (!openfluid::tools::ConvertString(std::string((char*) xmlUnitID),
                                         &UnitID))
      throw openfluid::base::OFException(
          "OpenFLUID framework", "FluidXDescriptor::extractUnitsLinkFromNode",
          "wrong format for ID in unit definition (" + m_CurrentFile + ")");

    return std::make_pair(std::string((char*) xmlUnitClass), UnitID);
  }
  else
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "FluidXDescriptor::extractUnitsLinkFromNode",
        "missing or wrong attribute(s) in units link definition (" + m_CurrentFile
        + ")");
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractDomainDefinitionFromNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "unit") == 0)
    {
      xmlChar* xmlUnitID = xmlGetProp(CurrNode, (const xmlChar*) "ID");
      xmlChar* xmlUnitClass = xmlGetProp(CurrNode, (const xmlChar*) "class");
      xmlChar* xmlPcsOrd = xmlGetProp(CurrNode, (const xmlChar*) "pcsorder");

      if (xmlUnitID != NULL && xmlUnitClass != NULL && xmlPcsOrd != NULL)
      {
        openfluid::fluidx::UnitDescriptor* UnitDesc =
            new openfluid::fluidx::UnitDescriptor();
        openfluid::core::PcsOrd_t PcsOrder;
        openfluid::core::UnitID_t UnitID;

        UnitDesc->getUnitClass().assign((char*) xmlUnitClass);

        if (!openfluid::tools::ConvertString(std::string((char*) xmlUnitID),
                                             &UnitID))
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractDomainDefinitionFromNode",
              "wrong format for ID in unit definition (" + m_CurrentFile + ")");

        if (!openfluid::tools::ConvertString(std::string((char*) xmlPcsOrd),
                                             &PcsOrder))
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractDomainDefinitionFromNode",
              "wrong format for process order in unit definition (" + m_CurrentFile
              + ")");

        UnitDesc->getProcessOrder() = PcsOrder;
        UnitDesc->getUnitID() = UnitID;

        xmlNodePtr CurrLinkNode = CurrNode->xmlChildrenNode;

        while (CurrLinkNode != NULL)
        {
          if (xmlStrcmp(CurrLinkNode->name, (const xmlChar*) "to") == 0)
          {
            UnitDesc->getUnitsTos().push_back(
                extractUnitClassIDFromNode(CurrLinkNode));
          }

          if (xmlStrcmp(CurrLinkNode->name, (const xmlChar*) "childof") == 0)
          {
            UnitDesc->getUnitsParents().push_back(
                extractUnitClassIDFromNode(CurrLinkNode));
          }

          CurrLinkNode = CurrLinkNode->next;
        }

        m_DomainDescriptor.getUnits().push_back(*UnitDesc);
      }
      else
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractDomainDefinitionFromNode",
            "missing or wrong attribute(s) in unit definition (" + m_CurrentFile
            + ")");
    }
    CurrNode = CurrNode->next;
  }
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractDomainInputdataFromNode(xmlNodePtr NodePtr)
{
  xmlChar* xmlUnitClass = xmlGetProp(NodePtr, (const xmlChar*) "unitclass");
  xmlChar* xmlColOrder = xmlGetProp(NodePtr, (const xmlChar*) "colorder");

  if (xmlUnitClass != NULL)
  {
    openfluid::fluidx::InputDataDescriptor IDataDesc;

    IDataDesc.getUnitsClass().assign((char*) xmlUnitClass);

    if (xmlColOrder != NULL)
    {
      // case 1
      // <inputdata unitclass="someclass" colorder="colA;colB" >
      //    here is the data
      // </inputdata>

      std::vector<std::string> ColOrder;

      ColOrder = openfluid::tools::SplitString(std::string((char*) xmlColOrder),
                                               ";");

      if (ColOrder.empty())
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractDomainInputdataFromNode",
            "wrong or empty colorder attribute in domain input data (" + m_CurrentFile
            + ")");

      IDataDesc.getColumnsOrder() = ColOrder;

      xmlChar *xmlDataBlob = xmlNodeGetContent(NodePtr);

      if (xmlDataBlob != NULL)
        IDataDesc.parseDataBlob(std::string((char*) xmlDataBlob));
      else
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractDomainInputdataFromNode",
            "wrong or empty data content in domain input data (" + m_CurrentFile
            + ")");
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
        if (xmlStrcmp(CurrNode->name, (const xmlChar*) "columns") == 0 && !FoundColOrder)
        {
          FoundColOrder = true;

          xmlChar* xmlOrder = xmlGetProp(CurrNode, (const xmlChar*) "order");

          if (xmlOrder == NULL)
            throw openfluid::base::OFException(
                "OpenFLUID framework",
                "FluidXDescriptor::extractDomainInputdataFromNode",
                "missing or wrong order attribute for columns tag in domain input data (" + m_CurrentFile
                + ")");

          std::vector<std::string> Order;

          Order = openfluid::tools::SplitString(std::string((char*) xmlOrder),
                                                ";");

          if (Order.size() == 0)
            throw openfluid::base::OFException(
                "OpenFLUID framework",
                "FluidXDescriptor::extractDomainInputdataFromNode",
                "wrong or empty order attribute for columns tag for domain input data (" + m_CurrentFile
                + ")");

          IDataDesc.getColumnsOrder() = Order;
        }

        if (xmlStrcmp(CurrNode->name, (const xmlChar*) "data") == 0 && !FoundData)
        {
          FoundData = true;

          xmlChar *xmlDataBlob = xmlNodeGetContent(CurrNode);

          if (xmlDataBlob != NULL)
            IDataDesc.parseDataBlob(std::string((char*) xmlDataBlob));
        }

        CurrNode = CurrNode->next;
      }

      if (!(FoundColOrder && FoundData))
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractDomainInputdataFromNode",
            "missing or wrong domain input data format (" + m_CurrentFile
            + ")");
    }

    m_DomainDescriptor.getInputData().push_back(IDataDesc);
  }
  else
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "FluidXDescriptor::extractDomainInputdataFromNode",
        "missing or wrong unitclass attribute(s) in domain input data (" + m_CurrentFile
        + ")");
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractDomainCalendarFromNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "event") == 0)
    {
      xmlChar* xmlUnitID = xmlGetProp(CurrNode, (const xmlChar*) "unitID");
      xmlChar* xmlUnitClass = xmlGetProp(CurrNode,
                                         (const xmlChar*) "unitclass");
      xmlChar* xmlDate = xmlGetProp(CurrNode, (const xmlChar*) "date");

      if (xmlUnitID != NULL && xmlUnitClass != NULL && xmlDate != NULL)
      {
        openfluid::fluidx::EventDescriptor EvDesc;

        openfluid::core::UnitID_t UnitID;

        if (!openfluid::tools::ConvertString(std::string((char*) xmlUnitID),
                                             &UnitID))
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractDomainCalendarFromNode",
              "wrong format for unit ID in event (" + m_CurrentFile + ")");

        openfluid::core::DateTime EventDate;

        if (!EventDate.setFromISOString(std::string((char*) xmlDate)))
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractDomainCalendarFromNode",
              "wrong format for date in event (" + m_CurrentFile + ")");

        EvDesc.getUnitClass().assign((char*) xmlUnitClass);
        EvDesc.getUnitID() = UnitID;
        EvDesc.getEvent() = openfluid::core::Event(EventDate);

        xmlNodePtr CurrInfoNode = CurrNode->xmlChildrenNode;

        while (CurrInfoNode != NULL)
        {
          if (xmlStrcmp(CurrInfoNode->name, (const xmlChar*) "info") == 0)
          {
            xmlChar* xmlKey = xmlGetProp(CurrInfoNode, (const xmlChar*) "key");
            xmlChar* xmlValue = xmlGetProp(CurrInfoNode,
                                           (const xmlChar*) "value");

            if (xmlKey != NULL && xmlValue != NULL)
            {
              EvDesc.getEvent().addInfo(std::string((char*) xmlKey),
                                        std::string((char*) xmlValue));
            }
            else
              throw openfluid::base::OFException(
                  "OpenFLUID framework",
                  "FluidXDescriptor::extractDomainCalendarFromNode",
                  "wrong or missing attribute(s) in domain calendar event info (" + m_CurrentFile
                  + ")");
          }

          CurrInfoNode = CurrInfoNode->next;
        }

        m_DomainDescriptor.getEvents().push_back(EvDesc);
      }
      else
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractDomainCalendarFromNode",
            "wrong or missing attribute(s) in domain calendar event (" + m_CurrentFile
            + ")");
    }

    CurrNode = CurrNode->next;
  }
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractDomainFomNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;
  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "definition") == 0)
    {
      extractDomainDefinitionFromNode(CurrNode);
    }

    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "inputdata") == 0)
    {
      extractDomainInputdataFromNode(CurrNode);
    }

    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "calendar") == 0)
    {
      extractDomainCalendarFromNode(CurrNode);
    }

    CurrNode = CurrNode->next;
  }
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractDatastoreFromNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "dataitem") == 0)
    {
      xmlChar* xmlDataID = xmlGetProp(CurrNode, (const xmlChar*) "id");
      xmlChar* xmlDataType = xmlGetProp(CurrNode, (const xmlChar*) "type");
      xmlChar* xmlDataSrc = xmlGetProp(CurrNode, (const xmlChar*) "source");
      xmlChar* xmlDataClass = xmlGetProp(CurrNode,
                                         (const xmlChar*) "unitclass");

      if (xmlDataID != NULL && xmlDataType != NULL && xmlDataSrc != NULL)
      {
        std::string DataID;

        if (!openfluid::tools::ConvertString(std::string((char*) xmlDataID),
                                             &DataID)
            || DataID.empty())
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractDatastoreFromNode",
              "wrong format or missing data ID in datastore (" + m_CurrentFile
              + ")");

        openfluid::core::UnstructuredValue::UnstructuredType DataType;

        if (!openfluid::core::UnstructuredValue::getValueTypeFromString(
            std::string((char*) xmlDataType), DataType))
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractDatastoreFromNode",
              "unknown or missing datatype in dataitem definition (" + m_CurrentFile
              + ")");

        openfluid::fluidx::DatastoreItemDescriptor* Item =
            new openfluid::fluidx::DatastoreItemDescriptor(
                DataID, m_CurrentDir, std::string((char*) xmlDataSrc),
                DataType);

        if (xmlDataClass != NULL)
          Item->setUnitClass(std::string((char*) xmlDataClass));

        m_DatastoreDescriptor.appendItem(Item);
      }
      else
        throw openfluid::base::OFException(
            "OpenFLUID framework",
            "FluidXDescriptor::extractDatastoreFromNode",
            "missing or wrong attribute(s) in dataitem definition (" + m_CurrentFile
            + ")");
    }

    CurrNode = CurrNode->next;
  }
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::parseFile(std::string Filename)
{
  xmlDocPtr Doc = NULL;
  xmlNodePtr Root = NULL;
  xmlNodePtr CurrNode = NULL;

  m_CurrentFile = Filename;

  Doc = xmlParseFile(m_CurrentFile.c_str());

  if (Doc != NULL)
  {
    Root = xmlDocGetRootElement(Doc);

    if (Root != NULL)
    {
      if (xmlStrcmp(Root->name, (const xmlChar*) "openfluid") == 0)
      {
        CurrNode = Root->xmlChildrenNode;
        while (CurrNode != NULL)
        {

          if (xmlStrcmp(CurrNode->name, (const xmlChar*) "run") == 0)
          {
            extractRunFromNode(CurrNode);
          }

          if (xmlStrcmp(CurrNode->name, (const xmlChar*) "model") == 0)
          {
            extractModelFromNode(CurrNode);
          }

          if (xmlStrcmp(CurrNode->name, (const xmlChar*) "monitoring") == 0)
          {
            extractMonitoringFromNode(CurrNode);
          }

          if (xmlStrcmp(CurrNode->name, (const xmlChar*) "domain") == 0)
          {
            extractDomainFomNode(CurrNode);
          }

          if (xmlStrcmp(CurrNode->name, (const xmlChar*) "datastore") == 0)
          {
            extractDatastoreFromNode(CurrNode);
          }

          CurrNode = CurrNode->next;
        }
      }
    }
    else
    {
      throw openfluid::base::OFException("OpenFLUID framework",
                                         "FluidXDescriptor::parseFile",
                                         "file " + m_CurrentFile + " is empty");
    }
  }
  else
  {
    throw openfluid::base::OFException(
        "OpenFLUID framework", "FluidXDescriptor::parseFile",
        "file " + m_CurrentFile + " cannot be parsed");
  }

}

// =====================================================================
// =====================================================================

void FluidXDescriptor::loadFromDirectory(std::string DirPath)
{
  if (!mp_Listener)
    mp_Listener = new openfluid::base::IOListener();

  if (!boost::filesystem::is_directory(boost::filesystem::path(DirPath)))
    throw openfluid::base::OFException(
        "OpenFLUID framework", "FluidXDescriptor::loadFromDirectory",
        "directory " + DirPath + " does not exist");

  std::vector<std::string> FluidXFilesToLoad = openfluid::tools::GetFilesByExt(
      DirPath, "fluidx", true);

  if (FluidXFilesToLoad.size() == 0)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "FluidXDescriptor::loadFromDirectory",
        "no fluidx file found in directory " + DirPath);

  std::sort(FluidXFilesToLoad.begin(), FluidXFilesToLoad.end());

  m_ModelDescriptor = openfluid::fluidx::CoupledModelDescriptor();
  m_RunDescriptor = openfluid::fluidx::RunDescriptor();
  m_DomainDescriptor = openfluid::fluidx::DomainDescriptor();
  m_DatastoreDescriptor = openfluid::fluidx::DatastoreDescriptor();
  m_MonitoringDescriptor = openfluid::fluidx::MonitoringDescriptor();

  m_RunConfigDefined = false;
  m_ModelDefined = false;

  unsigned int i = 0;

  std::string CurrentFile;
  boost::filesystem::path CurrentFilePath;

  m_CurrentDir = DirPath;

  for (i = 0; i < FluidXFilesToLoad.size(); i++)
  {
    CurrentFilePath = boost::filesystem::path(FluidXFilesToLoad[i]);
    CurrentFile = CurrentFilePath.string();
    try
    {
      mp_Listener->onFileLoad(CurrentFilePath.filename().string());
      parseFile(CurrentFile);
      mp_Listener->onFileLoaded(openfluid::base::Listener::OK);
    }
    catch (...)
    {
      mp_Listener->onFileLoaded(openfluid::base::Listener::ERROR);
      throw;
    }
  }

}

// =====================================================================
// =====================================================================

void FluidXDescriptor::clearOldVectorNamedVar(std::string& VarName)
{
  if (boost::ends_with(VarName, "[]"))
    boost::erase_last(VarName, "[]");
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

std::string FluidXDescriptor::getModelToWrite()
{
  std::ostringstream Contents;

  Contents << m_IndentStr << "<model>\n";

  if (m_ModelDescriptor.getGlobalParameters().size() > 0)
  {
    Contents << m_IndentStr << m_IndentStr << "<gparams>\n";
    Contents << getParamsAsStr(m_ModelDescriptor.getGlobalParameters());
    Contents << m_IndentStr << m_IndentStr << "</gparams>\n";
  }

  const std::list<openfluid::fluidx::ModelItemDescriptor*> Items =
      m_ModelDescriptor.getItems();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::PluggedFunction))
    {
      openfluid::fluidx::FunctionDescriptor* FuncDesc =
          dynamic_cast<openfluid::fluidx::FunctionDescriptor*>(*it);

      Contents << m_IndentStr << m_IndentStr << "<function ID=\""
               << FuncDesc->getFileID() << "\">\n";
      Contents << getParamsAsStr(FuncDesc->getParameters());
      Contents << m_IndentStr << m_IndentStr << "</function>\n";
    }
    else if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator))
    {
      openfluid::fluidx::GeneratorDescriptor* GenDesc =
          dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it);

      Contents << m_IndentStr << m_IndentStr << "<generator varname=\""
               << GenDesc->getVariableName() << "\" " << "unitclass=\""
               << GenDesc->getUnitClass() << "\" " << "method=\""
               << getGeneratorMethodAsStr(GenDesc->getGeneratorMethod())
               << "\"";

      if (GenDesc->getVariableSize() != 1)
        Contents << " varsize=\"" << GenDesc->getVariableSize() << "\"";

      Contents << ">\n";
      Contents << getParamsAsStr(GenDesc->getParameters());
      Contents << m_IndentStr << m_IndentStr << "</generator>\n";
    }
  }

  Contents << m_IndentStr << "</model>\n";

  return Contents.str();
}

// =====================================================================
// =====================================================================

std::string FluidXDescriptor::getParamsAsStr(
    const openfluid::ware::WareParams_t& Params) const
{
  std::string ParamsStr = "";

  std::map<std::string, std::string> ParamsMap = WareDescriptor::getParamsAsMap(
      Params);

  for (std::map<std::string, std::string>::iterator itt = ParamsMap.begin();
      itt != ParamsMap.end(); ++itt)
    ParamsStr += (m_IndentStr + m_IndentStr + m_IndentStr + "<param name=\""
                  + itt->first + "\" value=\"" + itt->second + "\"/>\n");

  return ParamsStr;
}

// =====================================================================
// =====================================================================

std::string FluidXDescriptor::getGeneratorMethodAsStr(
    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method) const
{
  switch (Method)
  {
    case openfluid::fluidx::GeneratorDescriptor::Fixed:
      return "fixed";
    case openfluid::fluidx::GeneratorDescriptor::Random:
      return "random";
    case openfluid::fluidx::GeneratorDescriptor::Interp:
      return "interp";
    case openfluid::fluidx::GeneratorDescriptor::Inject:
      return "inject";
      break;
    default:
      break;
  }

  return "";
}

// =====================================================================
// =====================================================================

std::string FluidXDescriptor::getDomainToWrite()
{
  std::ostringstream Contents;

  Contents << m_IndentStr << "<domain>\n";

  appendDomainDefinition(Contents);

  appendDomainInputdata(Contents);

  appendDomainCalendar(Contents);

  Contents << m_IndentStr << "</domain>\n";

  return Contents.str();
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::appendDomainDefinition(std::ostringstream& Contents)
{
  Contents << m_IndentStr << m_IndentStr << "<definition>\n";

  std::list<openfluid::fluidx::UnitDescriptor>& Units =
      m_DomainDescriptor.getUnits();

  std::list<openfluid::core::UnitClassID_t>::iterator itRel;

  for (std::list<openfluid::fluidx::UnitDescriptor>::iterator it =
      Units.begin(); it != Units.end(); ++it)
  {
    Contents << m_IndentStr << m_IndentStr << m_IndentStr << "<unit class=\""
             << it->getUnitClass() << "\" ID=\"" << it->getUnitID()
             << "\" pcsorder=\"" << it->getProcessOrder() << "\">\n";

    std::list<openfluid::core::UnitClassID_t>& Tos = it->getUnitsTos();
    for (itRel = Tos.begin(); itRel != Tos.end(); ++itRel)
      Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr
               << "<to class=\"" << itRel->first << "\" ID=\"" << itRel->second
               << "\" />\n";

    std::list<openfluid::core::UnitClassID_t>& Parents = it->getUnitsParents();
    for (itRel = Parents.begin(); itRel != Parents.end(); ++itRel)
      Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr
               << "<childof class=\"" << itRel->first << "\" ID=\""
               << itRel->second << "\" />\n";

    Contents << m_IndentStr << m_IndentStr << m_IndentStr << "</unit>\n";
  }

  Contents << m_IndentStr << m_IndentStr << "</definition>\n";
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::appendDomainInputdata(std::ostringstream& Contents)
{
  std::list<InputDataDescriptor>& IData = m_DomainDescriptor.getInputData();

  openfluid::fluidx::InputDataDescriptor::UnitIDInputData_t::iterator itData;
  openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t::iterator itVal;

  for (std::list<InputDataDescriptor>::iterator it = IData.begin();
      it != IData.end(); ++it)
  {
    Contents << m_IndentStr << m_IndentStr << "<inputdata unitclass=\""
             << it->getUnitsClass() << "\" colorder=\"";

    std::vector<std::string> Cols = it->getColumnsOrder();
    if (!Cols.empty())
    {
      int Max = Cols.size() - 1;
      for (int i = 0; i < Max; i++)
        Contents << Cols[i] << ";";
      Contents << Cols[Max];
    }

    Contents << "\">\n";

    openfluid::fluidx::InputDataDescriptor::UnitIDInputData_t& DataMap =
        it->getData();
    for (itData = DataMap.begin(); itData != DataMap.end(); ++itData)
    {
      Contents << itData->first << "\t";

      openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t& DataVals =
          itData->second;

      unsigned int i=0;
      for(; i<Cols.size()-1;i++)
      {
        Contents << DataVals[Cols[i]] << "\t";
      }
      Contents << DataVals[Cols[i]];

      Contents << "\n";
    }

    Contents << m_IndentStr << m_IndentStr << "</inputdata>\n";
  }

}

// =====================================================================
// =====================================================================

void FluidXDescriptor::appendDomainCalendar(std::ostringstream& Contents)
{
  std::list<EventDescriptor>& Events = m_DomainDescriptor.getEvents();

  if (Events.empty())
    return;

  Contents << m_IndentStr << m_IndentStr << "<calendar>\n";

  openfluid::core::Event::EventInfosMap_t::iterator itInfos;

  for (std::list<EventDescriptor>::iterator it = Events.begin();
      it != Events.end(); ++it)
  {
    Contents << m_IndentStr << m_IndentStr << m_IndentStr
             << "<event unitclass=\"" << it->getUnitClass() << "\" "
             << "unitID=\"" << it->getUnitID() << "\" " << "date=\""
             << it->getEvent().getDateTime().getAsISOString() << "\">\n";

    openfluid::core::Event::EventInfosMap_t Infos = it->getEvent().getInfos();
    for (itInfos = Infos.begin(); itInfos != Infos.end(); ++itInfos)
      Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr
               << "<info key=\"" << itInfos->first << "\" value=\""
               << itInfos->second << "\" />\n";

    Contents << m_IndentStr << m_IndentStr << m_IndentStr << "</event>\n";
  }

  Contents << m_IndentStr << m_IndentStr << "</calendar>\n";
}

// =====================================================================
// =====================================================================

std::string FluidXDescriptor::getRunConfigurationToWrite()
{
  std::ostringstream Contents;

  Contents << m_IndentStr << "<run>\n";

  if (m_RunDescriptor.isFilled())
  {
    Contents << m_IndentStr << m_IndentStr << "<deltat>"
             << m_RunDescriptor.getDeltaT() << "</deltat>\n";
    Contents << m_IndentStr << m_IndentStr << "<period begin=\""
             << m_RunDescriptor.getBeginDate().getAsISOString() << "\" end=\""
             << m_RunDescriptor.getEndDate().getAsISOString() << "\" />\n";

    if (m_RunDescriptor.isSimulationID())
      Contents << m_IndentStr << m_IndentStr << "<simid>"
               << m_RunDescriptor.getSimulationID() << "</simid>\n";

    if (m_RunDescriptor.isUserValuesBufferSize())
      Contents << m_IndentStr << m_IndentStr << "<valuesbuffer steps=\""
               << m_RunDescriptor.getValuesBufferSize() << "\" />\n";

    Contents << m_IndentStr << m_IndentStr << "<filesbuffer kbytes=\""
             << m_RunDescriptor.getFilesBufferSizeInKB() << "\" />\n";
  }

  Contents << m_IndentStr << "</run>\n";

  return Contents.str();
}

// =====================================================================
// =====================================================================

std::string FluidXDescriptor::getDatastoreToWrite()
{
  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t& Items =
      m_DatastoreDescriptor.getItems();

  std::ostringstream Contents;

  Contents << m_IndentStr << "<datastore>\n";

  for (openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t::iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    Contents << m_IndentStr << m_IndentStr;
    Contents
        << "<dataitem id=\""
        << (*it)->getID()
        << "\" "
        << "type=\""
        << openfluid::core::UnstructuredValue::getStringFromValueType(
            (*it)->getType())
        << "\" " << "source=\"" << (*it)->getRelativePath() << "\" ";

    if (!(*it)->getUnitClass().empty())
      Contents << "unitclass=\"" << (*it)->getUnitClass() << "\" ";

    Contents << "/>\n";
  }

  Contents << m_IndentStr << "</datastore>\n";

  return Contents.str();
}

// =====================================================================
// =====================================================================

std::string FluidXDescriptor::getMonitoringToWrite()
{
  std::ostringstream Contents;

  Contents << m_IndentStr << "<monitoring>\n";

  std::list<ObserverDescriptor*> Items = m_MonitoringDescriptor.getItems();

  for (std::list<ObserverDescriptor*>::iterator it = Items.begin();
      it != Items.end(); ++it)
  {
    Contents << m_IndentStr << m_IndentStr << "<observer ID=\""
             << (*it)->getID() << "\">\n";

    Contents << getParamsAsStr((*it)->getParameters());

    Contents << m_IndentStr << m_IndentStr << "</observer>\n";
  }

  Contents << m_IndentStr << "</monitoring>\n";

  return Contents.str();
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::writeToManyFiles(std::string DirPath)
{
  mp_Listener->onWrite();

  std::ofstream OutFile;
  std::string OutFilename;

  prepareFluidXDir(DirPath);

  // model
  OutFilename = boost::filesystem::path(DirPath + "/model.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  OutFile << getModelToWrite() << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  // domain
  OutFilename = boost::filesystem::path(DirPath + "/domain.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  OutFile << getDomainToWrite() << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  // run
  OutFilename = boost::filesystem::path(DirPath + "/run.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  OutFile << getRunConfigurationToWrite() << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  // datastore
  OutFilename = boost::filesystem::path(DirPath + "/datastore.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  OutFile << getDatastoreToWrite() << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  // monitoring
  OutFilename =
      boost::filesystem::path(DirPath + "/monitoring.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  OutFile << getMonitoringToWrite() << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  mp_Listener->onWritten(openfluid::base::Listener::OK);
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::writeToSingleFile(std::string FilePath)
{
  mp_Listener->onWrite();

  std::ofstream OutFile;

  prepareFluidXDir(boost::filesystem::path(FilePath).branch_path().string());

  std::string OutFilename = boost::filesystem::path(FilePath).string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n\n\n";

  OutFile << getModelToWrite() << "\n\n";
  OutFile << getDomainToWrite() << "\n\n";
  OutFile << getRunConfigurationToWrite() << "\n\n";
  OutFile << getDatastoreToWrite() << "\n\n";
  OutFile << getMonitoringToWrite() << "\n\n";

  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::OK);

  mp_Listener->onWritten(openfluid::base::Listener::OK);
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::prepareFluidXDir(std::string DirPath)
{
  boost::filesystem::path OutputDirPath(DirPath);

  if (!boost::filesystem::exists(OutputDirPath))
  {
    boost::filesystem::create_directory(OutputDirPath);
    if (!boost::filesystem::exists(OutputDirPath))
      throw openfluid::base::OFException("OpenFLUID framework",
                                         "FluidXDescriptor::prepareOutputDir",
                                         "Error creating output directory");
  }
}

// =====================================================================
// =====================================================================

}
} // namespaces
