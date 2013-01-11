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
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/erase.hpp>

#include <openfluid/fluidx/FunctionDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/ObserverDescriptor.hpp>
#include <openfluid/io/IOListener.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/fluidx/DomainDescriptor.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/fluidx/RunDescriptor.hpp>
#include <openfluid/base/OutputDescriptor.hpp>
#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/fluidx/ObserversListDescriptor.hpp>

#include <openfluid/core/UnstructuredValue.hpp>

namespace openfluid {
namespace fluidx {

FluidXDescriptor* FluidXDescriptor::mp_Instance = 0;

// =====================================================================
// =====================================================================

FluidXDescriptor::FluidXDescriptor() :
    mp_Listener(0)
{
}

// =====================================================================
// =====================================================================

FluidXDescriptor::~FluidXDescriptor()
{
}

// =====================================================================
// =====================================================================

FluidXDescriptor* FluidXDescriptor::getInstance()
{
  if (!mp_Instance)
    mp_Instance = new FluidXDescriptor();

  return mp_Instance;
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::setIOListener(openfluid::io::IOListener* Listener)
{
  mp_Listener = Listener;
}

// =====================================================================
// =====================================================================

openfluid::base::OutputFilesDescriptor FluidXDescriptor::extractFilesDecriptorFromNode(
    xmlNodePtr NodePtr)
{
  openfluid::base::OutputFilesDescriptor OFD;

  xmlChar* xmlColSep = xmlGetProp(NodePtr, (const xmlChar*) "colsep");
  if (xmlColSep != NULL)
  {
    std::string ColSep = std::string((const char*) xmlColSep);
    boost::algorithm::replace_all(ColSep, "\\t", "\t");
    OFD.setColSeparator(ColSep);
  }

  xmlChar* xmlDTFormat = xmlGetProp(NodePtr, (const xmlChar*) "dtformat");
  if (xmlDTFormat != NULL)
    OFD.setDateFormat(std::string((const char*) xmlDTFormat));

  xmlChar* xmlCommentChar = xmlGetProp(NodePtr, (const xmlChar*) "commentchar");
  if (xmlCommentChar != NULL)
  {
    std::string CommentChar = std::string((const char*) xmlCommentChar);
    boost::algorithm::replace_all(CommentChar, "\\t", "\t");
    OFD.setCommentChar(CommentChar);
  }

  if (OFD.getDateFormat() == "6cols")
    OFD.setDateFormat("%Y\t%m\t%d\t%H\t%M\t%S");
  if (OFD.getDateFormat() == "iso")
    OFD.setDateFormat("%Y%m%dT%H%M%S");

  xmlChar* xmlHeaderType = xmlGetProp(NodePtr, (const xmlChar*) "header");
  if (xmlHeaderType != NULL)
  {
    std::string HeaderType = std::string((const char*) xmlHeaderType);

    if (HeaderType == "none")
      OFD.setHeaderType(openfluid::base::OutputFilesDescriptor::None);
    else if (HeaderType == "colnames-as-data")
      OFD.setHeaderType(openfluid::base::OutputFilesDescriptor::ColnamesAsData);
    else if (HeaderType == "full")
      OFD.setHeaderType(openfluid::base::OutputFilesDescriptor::Full);
    else
      OFD.setHeaderType(openfluid::base::OutputFilesDescriptor::Info);
  }

  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "set") == 0)
    {
      OFD.getSets().push_back(extractSetDecriptorFromNode(CurrNode));
    }
    CurrNode = CurrNode->next;
  }

  return OFD;
}

// =====================================================================
// =====================================================================

openfluid::base::OutputSetDescriptor FluidXDescriptor::extractSetDecriptorFromNode(
    xmlNodePtr NodePtr)
{
  openfluid::base::OutputSetDescriptor OSD;

  xmlChar* xmlName = xmlGetProp(NodePtr, (const xmlChar*) "name");
  xmlChar* xmlUnitsClass = xmlGetProp(NodePtr, (const xmlChar*) "unitsclass");
  xmlChar* xmlUnitsIDs = xmlGetProp(NodePtr, (const xmlChar*) "unitsIDs");
  xmlChar* xmlVars = xmlGetProp(NodePtr, (const xmlChar*) "vars");
  xmlChar* xmlPrec = xmlGetProp(NodePtr, (const xmlChar*) "precision");

  if (xmlName != NULL && xmlUnitsClass != NULL && xmlUnitsIDs != NULL
      && xmlVars != NULL)
  {
    OSD.setName(std::string((const char*) xmlName));
    OSD.setUnitsClass(std::string((const char*) xmlUnitsClass));

    std::string UnitsIDsStr = std::string((const char*) xmlUnitsIDs);
    std::string VarsStr = std::string((const char*) xmlVars);

    if (UnitsIDsStr == "*")
      OSD.setAllUnits(true);
    else
    {
      OSD.setAllUnits(false);
      openfluid::core::UnitID_t UID;

      std::vector<std::string> StrArray = openfluid::tools::SplitString(
          UnitsIDsStr, ";");
      for (unsigned int i = 0; i < StrArray.size(); i++)
      {
        if (openfluid::tools::ConvertString(StrArray[i], &UID))
          OSD.getUnitsIDs().push_back(UID);
        else
          throw openfluid::base::OFException(
              "OpenFLUID framework",
              "FluidXDescriptor::extractSetDecriptorFromNode",
              "Wrong format of units IDs in output set config (" + m_CurrentFile
              + ")");
      }
    }

    if (VarsStr == "*")
    {
      OSD.setAllVariables(true);
    }
    else
    {
      OSD.setAllVariables(false);

      std::vector<std::string> StrArray = openfluid::tools::SplitString(VarsStr,
                                                                        ";");

      for (unsigned int i = 0; i < StrArray.size(); i++)
      {
        clearOldVectorNamedVar(StrArray[i]);

        OSD.getVariables().push_back(StrArray[i]);
      }

    }

  }
  else
    throw openfluid::base::OFException(
        "OpenFLUID framework",
        "FluidXDescriptor::extractSetDecriptorFromNode",
        "wrong output set definition (" + m_CurrentFile
        + "). missing attribute?");

  if (xmlPrec != NULL)
  {
    std::string PrecStr = std::string((const char*) xmlPrec);
    int Prec = 5;

    if (openfluid::tools::ConvertString(PrecStr, &Prec) && Prec >= 0)
    {
      OSD.setPrecision(Prec);
    }
    else
      throw openfluid::base::OFException(
          "OpenFLUID framework",
          "FluidXDescriptor::extractSetDecriptorFromNode",
          "Wrong value format of precision in output set config (" + m_CurrentFile
          + ")");
  }

  return OSD;

}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractOutputFromNode(xmlNodePtr NodePtr)
{
  xmlNodePtr CurrNode = NodePtr->xmlChildrenNode;

  openfluid::fluidx::ObserverDescriptor* OD;

  while (CurrNode != NULL)
  {
    if (xmlStrcmp(CurrNode->name, (const xmlChar*) "files") == 0)
    {
      m_OutputDescriptor.getFileSets().push_back(
          extractFilesDecriptorFromNode(CurrNode));
    }
    else if (xmlStrcmp(CurrNode->name, (const xmlChar*) "observer") == 0)
    {
      xmlChar* xmlID = xmlGetProp(CurrNode, (const xmlChar*) "ID");

      if (xmlID != NULL)
      {

        OD = new openfluid::fluidx::ObserverDescriptor((const char*) xmlID);
        OD->setParameters(extractParamsFromNode(CurrNode));
        m_ObserversListDescriptor.appendItem(OD);
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

  openfluid::ware::WareParams_t::const_iterator it;

  for (it = OverloadParams.begin(); it != OverloadParams.end(); ++it)
  {
    FinalParams.put((*it).first, (*it).second.data());
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
      xmlChar* xmlFileID = xmlGetProp(CurrNode, (const xmlChar*) "fileID");

      if (xmlFileID != NULL)
      {

        FD = new openfluid::fluidx::FunctionDescriptor((const char*) xmlFileID);
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

          if (xmlStrcmp(CurrNode->name, (const xmlChar*) "output") == 0)
          {
            extractOutputFromNode(CurrNode);
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
    mp_Listener = new openfluid::io::IOListener();

  if (!boost::filesystem::is_directory(boost::filesystem::path(DirPath)))
    throw openfluid::base::OFException(
        "OpenFLUID framework", "FluidXDescriptor::loadFromDirectory",
        "directory " + DirPath + " does not exist");

  std::vector<std::string> FluidXFilesToLoad = openfluid::tools::GetFilesByExt(
      DirPath, "fluidx", true);
//  std::vector<std::string> XMLFilesToLoad = openfluid::tools::GetFilesByExt(DirPath,"xml",true);
//
//  FluidXFilesToLoad.insert(FluidXFilesToLoad.end(),XMLFilesToLoad.begin(),XMLFilesToLoad.end());

  if (FluidXFilesToLoad.size() == 0)
    throw openfluid::base::OFException(
        "OpenFLUID framework", "FluidXDescriptor::loadFromDirectory",
        "no fluidx file found in directory " + DirPath);

  std::sort(FluidXFilesToLoad.begin(), FluidXFilesToLoad.end());

  m_ModelDescriptor = openfluid::fluidx::CoupledModelDescriptor();
  m_RunDescriptor = openfluid::fluidx::RunDescriptor();
  m_OutputDescriptor = openfluid::base::OutputDescriptor();
  m_DomainDescriptor = openfluid::fluidx::DomainDescriptor();
  m_DatastoreDescriptor = openfluid::fluidx::DatastoreDescriptor();
  m_ObserversListDescriptor = openfluid::fluidx::ObserversListDescriptor();

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
      //OStream << "    file: " << CurrentFilePath.leaf() << " ";
      mp_Listener->onFileLoad(CurrentFilePath.leaf());
      parseFile(CurrentFile);
      mp_Listener->onFileLoaded(openfluid::base::Listener::OK);
      //OStream << "[OK]" << std::endl;

    }
    catch (...)
    {
      mp_Listener->onFileLoaded(openfluid::base::Listener::ERROR);
      throw;
    }
  }

  //propagateGlobalParamsInModel();

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

}
} // namespaces
