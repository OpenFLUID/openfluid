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
 @file FluidXDescriptor.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include "FluidXDescriptor.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <openfluid/base/IOListener.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/tools/QtHelpers.hpp>

#include <QDomDocument>
#include <QFile>


#include <fstream>

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


bool FluidXDescriptor::extractWareEnabledFromNode(QDomElement& Node)
{

  // a ware is enabled if the enabled attribute is not present
  // or if it is equal to "1" or "true"
  QString xmlEnabled = Node.attributeNode(QString("enabled")).value();

  if (!xmlEnabled.isNull())
  {
    if (xmlEnabled == "1" || xmlEnabled == "true")
      return true;
    else return false;
  }
  return true;
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::extractMonitoringFromNode(QDomElement& Node)
{
  openfluid::fluidx::ObserverDescriptor* OD;

  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("observer"))
    {
      QString xmlID = CurrNode.attributeNode(QString("ID")).value();

      if (!xmlID.isNull())
      {
        OD = new openfluid::fluidx::ObserverDescriptor(xmlID.toStdString());
        OD->setParameters(extractParamsFromNode(CurrNode));
        OD->setEnabled(extractWareEnabledFromNode(CurrNode));
        m_MonitoringDescriptor.appendItem(OD);
      }
    }
  }
}

// =====================================================================
// =====================================================================

openfluid::ware::WareParams_t FluidXDescriptor::extractParamsFromNode(
    QDomElement& Node)
{
  openfluid::ware::WareParams_t Params;

  if (!Node.isNull())
  {
    for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
    {
      if (CurrNode.tagName() == QString("param"))
      {

        QString xmlKey = CurrNode.attributeNode(QString("name")).value();
        QString xmlValue = CurrNode.attributeNode(QString("value")).value();

        if (!xmlKey.isNull() && !xmlValue.isNull())
        {
          Params[xmlKey.toStdString()] = openfluid::ware::WareParamValue_t(
              xmlValue.toStdString());
        }
        else
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractParamsFromNode",
              "missing name and/or param attribute(s) in parameter definition (" + m_CurrentFile
              + ")");
      }
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

  for (openfluid::ware::WareParams_t::const_iterator it =
      OverloadParams.begin(); it != OverloadParams.end(); ++it)
    FinalParams[it->first] = it->second;

  return FinalParams;
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractModelFromNode(QDomElement& Node)
{
  if (m_ModelDefined)
    throw openfluid::base::FrameworkException(
        "FluidXDescriptor::extractModelFromNode",
        "Duplicate model definition (" + m_CurrentFile + ")");

  openfluid::fluidx::SimulatorDescriptor* SD;
  openfluid::fluidx::GeneratorDescriptor* GD;
  openfluid::ware::WareParams_t GParams;

  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("gparams"))
    {
      GParams = mergeParams(GParams, extractParamsFromNode(CurrNode));
    }

    if (CurrNode.tagName() == QString("simulator"))
    {
      QString xmlID = CurrNode.attributeNode(QString("ID")).value();

      if (!xmlID.isNull())
      {

        SD = new openfluid::fluidx::SimulatorDescriptor(xmlID.toStdString());
        SD->setParameters(extractParamsFromNode(CurrNode));
        SD->setEnabled(extractWareEnabledFromNode(CurrNode));
        m_ModelDescriptor.appendItem(SD);
      }
    }

    if (CurrNode.tagName() == QString("generator"))
    {
      QString xmlVarName = CurrNode.attributeNode(QString("varname")).value();
      QString xmlUnitClass = CurrNode.attributeNode(QString("unitclass")).value();
      QString xmlMethod = CurrNode.attributeNode(QString("method")).value();
      QString xmlVarSize = CurrNode.attributeNode(QString("varsize")).value();
      unsigned int VarSize = 1;

      if (!xmlVarName.isNull() && !xmlUnitClass.isNull() && !xmlMethod.isNull())
      {
        openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GenMethod =
            openfluid::fluidx::GeneratorDescriptor::NoGenMethod;
        if (xmlMethod == QString("fixed"))
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Fixed;
        if (xmlMethod == QString("random"))
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Random;
        if (xmlMethod == QString("interp"))
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Interp;
        if (xmlMethod == QString("inject"))
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Inject;

        if (GenMethod == openfluid::fluidx::GeneratorDescriptor::NoGenMethod)
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractModelFromNode",
              "unknown or missing generator method (" + m_CurrentFile + ")");

        if (!xmlVarSize.isNull())
        {
          if (!openfluid::tools::convertString(xmlVarSize.toStdString(),
                                               &VarSize))
            throw openfluid::base::FrameworkException(
                "FluidXDescriptor::extractModelFromNode",
                "wrong variable size format in generator (" + m_CurrentFile
                + ")");
        }

        GD = new openfluid::fluidx::GeneratorDescriptor(
            xmlVarName.toStdString(), xmlUnitClass.toStdString(), GenMethod,
            VarSize);
        GD->setParameters(extractParamsFromNode(CurrNode));
        GD->setEnabled(extractWareEnabledFromNode(CurrNode));
        m_ModelDescriptor.appendItem(GD);
      }
      else
        throw openfluid::base::FrameworkException(
            "FluidXDescriptor::extractModelFromNode",
            "missing attribute(s) in generator description (" + m_CurrentFile
            + ")");
    }

  }

  m_ModelDescriptor.setGlobalParameters(GParams);
  m_ModelDefined = true;
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractRunFromNode(QDomElement& Node)
{
  if (m_RunConfigDefined)
    throw openfluid::base::FrameworkException(
        "FluidXDescriptor::extractRunFromNode",
        "Duplicate run configuration (" + m_CurrentFile + ")");

  bool FoundDeltaT = false;
  bool FoundPeriod = false;


  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("period"))
    {
      QString xmlBegin = CurrNode.attributeNode(QString("begin")).value();
      QString xmlEnd = CurrNode.attributeNode(QString("end")).value();

      if (!xmlBegin.isNull() && !xmlEnd.isNull())
      {
        openfluid::core::DateTime ReadDate;

        if (ReadDate.setFromISOString(xmlBegin.toStdString()))
        {
          m_RunDescriptor.setBeginDate(ReadDate);
        }
        else
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractRunFromNode",
              "wrong format for begin date of period (" + m_CurrentFile + ")");

        if (ReadDate.setFromISOString(xmlEnd.toStdString()))
        {
          m_RunDescriptor.setEndDate(ReadDate);
        }
        else
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractRunFromNode",
              "wrong format for end date of period (" + m_CurrentFile + ")");

        FoundPeriod = true;

        if (!(m_RunDescriptor.getEndDate() > m_RunDescriptor.getBeginDate()))
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractRunFromNode",
              "end date must be strictly greater than begin date for period (" + m_CurrentFile
              + ")");
      }
      else
        throw openfluid::base::FrameworkException(
            "FluidXDescriptor::extractRunFromNode",
            "missing begin and/or end attributes for period tag (" + m_CurrentFile
            + ")");
    }


    // scheduling

    if (CurrNode.tagName() == QString("scheduling"))
    {
      QString xmlDeltaT = CurrNode.attributeNode(QString("deltat")).value();
      QString xmlConstraint = CurrNode.attributeNode(QString("constraint")).value();

      if (!xmlDeltaT.isNull())
      {
        int DeltaT;
        std::string ReadDeltaTStr = xmlDeltaT.toStdString();

        if (!openfluid::tools::convertString(ReadDeltaTStr, &DeltaT))
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractRunFromNode",
              "empty or wrong value for deltat (" + m_CurrentFile + ")");

        m_RunDescriptor.setDeltaT(DeltaT);
        FoundDeltaT = true;
      }

      if (!xmlConstraint.isNull())
      {
        std::string ReadConstraintStr = xmlConstraint.toStdString();
        if (ReadConstraintStr == "dt-checked")
          m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_DTCHECKED);
        else if (ReadConstraintStr == "dt-forced")
          m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_DTFORCED);
        else if (ReadConstraintStr == "none")
          m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_NONE);
        else
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractRunFromNode",
              "wrong value for scheduling constraint (" + m_CurrentFile + ")");
      }
      else
        m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_NONE);
    }


    // valuesbuffer

    if (CurrNode.tagName() == QString("valuesbuffer"))
    {
      QString xmlSteps = CurrNode.attributeNode(QString("size")).value();

      if (!xmlSteps.isNull())
      {
        unsigned int ReadSteps;

        if (!openfluid::tools::convertString(xmlSteps.toStdString(),
                                             &ReadSteps))
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractRunFromNode",
              "wrong format for size attribute for valuesbuffer tag (" + m_CurrentFile
              + ")");

        m_RunDescriptor.setValuesBufferSize(ReadSteps);
      }
      else
        throw openfluid::base::FrameworkException(
            "FluidXDescriptor::extractRunFromNode",
            "missing size attribute for valuesbuffer tag (" + m_CurrentFile
            + ")");
    }
  }

  if (!FoundPeriod)
    throw openfluid::base::FrameworkException(
        "FluidXDescriptor::extractRunFromNode",
        "run period not found (" + m_CurrentFile + ")");

  if (!FoundDeltaT)
    throw openfluid::base::FrameworkException(
        "FluidXDescriptor::extractRunFromNode",
        "deltat not found (" + m_CurrentFile + ")");

  m_RunConfigDefined = true;

  m_RunDescriptor.setFilled(true);
}

// =====================================================================
// =====================================================================

openfluid::core::UnitClassID_t FluidXDescriptor::extractUnitClassIDFromNode(
    QDomElement& Node)
{
  QString xmlUnitID = Node.attributeNode(QString("ID")).value();
  QString xmlUnitClass = Node.attributeNode(QString("class")).value();

  if (!xmlUnitID.isNull() && !xmlUnitClass.isNull())
  {
    openfluid::core::UnitID_t UnitID;

    if (!openfluid::tools::convertString(xmlUnitID.toStdString(),
                                         &UnitID))
      throw openfluid::base::FrameworkException(
          "FluidXDescriptor::extractUnitsLinkFromNode",
          "wrong format for ID in unit definition (" + m_CurrentFile + ")");

    return std::make_pair(xmlUnitClass.toStdString(), UnitID);
  }
  else
    throw openfluid::base::FrameworkException(
        "FluidXDescriptor::extractUnitsLinkFromNode",
        "missing or wrong attribute(s) in units link definition (" + m_CurrentFile
        + ")");
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractDomainDefinitionFromNode(QDomElement& Node)
{
  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("unit"))
    {
      QString xmlUnitID = CurrNode.attributeNode(QString("ID")).value();
      QString xmlUnitClass = CurrNode.attributeNode(QString("class")).value();
      QString xmlPcsOrd = CurrNode.attributeNode(QString("pcsorder")).value();

      if (!xmlUnitID.isNull() && !xmlUnitClass.isNull() && !xmlPcsOrd.isNull())
      {
        openfluid::fluidx::SpatialUnitDescriptor* UnitDesc =
            new openfluid::fluidx::SpatialUnitDescriptor();
        openfluid::core::PcsOrd_t PcsOrder;
        openfluid::core::UnitID_t UnitID;

        UnitDesc->setUnitsClass(xmlUnitClass.toStdString());

        if (!openfluid::tools::convertString(xmlUnitID.toStdString(),
                                             &UnitID))
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractDomainDefinitionFromNode",
              "wrong format for ID in unit definition (" + m_CurrentFile + ")");

        if (!openfluid::tools::convertString(xmlPcsOrd.toStdString(),
                                             &PcsOrder))
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractDomainDefinitionFromNode",
              "wrong format for process order in unit definition (" + m_CurrentFile
              + ")");

        UnitDesc->setProcessOrder(PcsOrder);
        UnitDesc->setID(UnitID);


        for(QDomElement CurrLinkNode = CurrNode.firstChildElement();
            !CurrLinkNode.isNull(); CurrLinkNode = CurrLinkNode.nextSiblingElement())
        {
          if (CurrLinkNode.tagName() == QString("to"))
          {
            UnitDesc->toSpatialUnits().push_back(
                extractUnitClassIDFromNode(CurrLinkNode));
          }

          if (CurrLinkNode.tagName() == QString("childof"))
          {
            UnitDesc->parentSpatialUnits().push_back(
                extractUnitClassIDFromNode(CurrLinkNode));
          }
        }

        m_DomainDescriptor.spatialUnits().push_back(*UnitDesc);
      }
      else
        throw openfluid::base::FrameworkException(
            "FluidXDescriptor::extractDomainDefinitionFromNode",
            "missing or wrong attribute(s) in unit definition (" + m_CurrentFile
            + ")");
    }
  }
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::extractDomainAttributesFromNode(QDomElement& Node)
{
  QString xmlUnitClass = Node.attributeNode(QString("unitclass")).value();
  QString xmlColOrder = Node.attributeNode(QString("colorder")).value();

  if (!xmlUnitClass.isNull() && !xmlColOrder.isNull())
  {
    openfluid::fluidx::AttributesDescriptor AttrsDesc;

    AttrsDesc.setUnitsClass(xmlUnitClass.toStdString());

    std::vector<std::string> ColOrder;

    ColOrder = openfluid::tools::splitString(xmlColOrder.toStdString(),
                                             ";");

    if (ColOrder.empty())
      throw openfluid::base::FrameworkException(
          "FluidXDescriptor::extractDomainAttributesFromNode",
          "wrong or empty colorder attribute in domain attributes (" + m_CurrentFile
          + ")");

    AttrsDesc.columnsOrder() = ColOrder;

    QString xmlDataBlob = Node.text();

    if (!xmlDataBlob.isNull())
      AttrsDesc.parseDataBlob(xmlDataBlob.toStdString());
    else
      throw openfluid::base::FrameworkException(
          "FluidXDescriptor::extractDomainAttributesFromNode",
          "wrong or empty data content in domain attributes (" + m_CurrentFile
          + ")");

    m_DomainDescriptor.attributes().push_back(AttrsDesc);
  }
  else
    throw openfluid::base::FrameworkException(
        "FluidXDescriptor::extractDomainAttributesFromNode",
        "missing or wrong unitclass attribute(s) in domain attributes (" + m_CurrentFile
        + ")");
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::extractDomainCalendarFromNode(QDomElement& Node)
{
  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("event"))
    {
      QString xmlUnitID = CurrNode.attributeNode(QString("unitID")).value();
      QString xmlUnitClass = CurrNode.attributeNode(QString("unitclass")).value();

      QString xmlDate = CurrNode.attributeNode(QString("date")).value();

      if (!xmlUnitID.isNull() && !xmlUnitClass.isNull() && !xmlDate.isNull())
      {
        openfluid::fluidx::EventDescriptor EvDesc;

        openfluid::core::UnitID_t UnitID;

        if (!openfluid::tools::convertString(xmlUnitID.toStdString(),
                                             &UnitID))
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractDomainCalendarFromNode",
              "wrong format for unit ID in event (" + m_CurrentFile + ")");

        openfluid::core::DateTime EventDate;

        if (!EventDate.setFromISOString(xmlDate.toStdString()))
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractDomainCalendarFromNode",
              "wrong format for date in event (" + m_CurrentFile + ")");

        EvDesc.setUnitClass(xmlUnitClass.toStdString());
        EvDesc.setUnitID(UnitID);
        EvDesc.event() = openfluid::core::Event(EventDate);


        for(QDomElement CurrInfoNode = CurrNode.firstChildElement();
            !CurrInfoNode.isNull(); CurrInfoNode = CurrInfoNode.nextSiblingElement())
        {
          if (CurrInfoNode.tagName() == QString("info"))
          {

            QString xmlKey = CurrInfoNode.attributeNode(QString("key")).value();
            QString xmlValue = CurrInfoNode.attributeNode(QString("value")).value();

            if (!xmlKey.isNull() && !xmlValue.isNull())
            {
              EvDesc.event().addInfo(xmlKey.toStdString(),
                                        xmlValue.toStdString());
            }
            else
              throw openfluid::base::FrameworkException(
                  "FluidXDescriptor::extractDomainCalendarFromNode",
                  "wrong or missing attribute(s) in domain calendar event info (" + m_CurrentFile
                  + ")");
          }

        }

        m_DomainDescriptor.events().push_back(EvDesc);
      }
      else
        throw openfluid::base::FrameworkException(
            "FluidXDescriptor::extractDomainCalendarFromNode",
            "wrong or missing attribute(s) in domain calendar event (" + m_CurrentFile
            + ")");
    }

  }
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractDomainFomNode(QDomElement& Node)
{
  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("definition"))
    {
      extractDomainDefinitionFromNode(CurrNode);
    }

    if (CurrNode.tagName() == QString("attributes"))
    {
      extractDomainAttributesFromNode(CurrNode);
    }

    if (CurrNode.tagName() == QString("calendar"))
    {
      extractDomainCalendarFromNode(CurrNode);
    }

  }
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::extractDatastoreFromNode(QDomElement& Node)
{
  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("dataitem"))
    {
      QString xmlDataID = CurrNode.attributeNode(QString("id")).value();
      QString xmlDataType = CurrNode.attributeNode(QString("type")).value();
      QString xmlDataSrc = CurrNode.attributeNode(QString("source")).value();
      QString xmlDataClass = CurrNode.attributeNode(QString("unitclass")).value();

      if (!xmlDataID.isNull() && !xmlDataType.isNull() && !xmlDataSrc.isNull())
      {
        std::string DataID;

        if (!openfluid::tools::convertString(xmlDataID.toStdString(),
                                             &DataID)
            || DataID.empty())
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractDatastoreFromNode",
              "wrong format or missing data ID in datastore (" + m_CurrentFile
              + ")");

        openfluid::core::UnstructuredValue::UnstructuredType DataType;

        if (!openfluid::core::UnstructuredValue::getValueTypeFromString(
            xmlDataType.toStdString(), DataType))
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractDatastoreFromNode",
              "unknown or missing datatype in dataitem definition (" + m_CurrentFile
              + ")");

        openfluid::fluidx::DatastoreItemDescriptor* Item =
            new openfluid::fluidx::DatastoreItemDescriptor(
                DataID, m_CurrentDir, xmlDataSrc.toStdString(),
                DataType);

        if (!xmlDataClass.isNull())
          Item->setUnitClass(xmlDataClass.toStdString());

        if (!m_DatastoreDescriptor.appendItem(Item))
          throw openfluid::base::FrameworkException(
              "FluidXDescriptor::extractDatastoreFromNode",
              "already existing dataitem ID: " + DataID + " (" + m_CurrentFile
              + ")");
      }
      else
        throw openfluid::base::FrameworkException(
            "FluidXDescriptor::extractDatastoreFromNode",
            "missing or wrong attribute(s) in dataitem definition (" + m_CurrentFile
            + ")");
    }

  }
}

// =====================================================================
// =====================================================================

void FluidXDescriptor::parseFile(std::string Filename)
{
  QDomDocument Doc;
  QDomElement Root;

  m_CurrentFile = Filename;

  QFile File(QString(m_CurrentFile.c_str()));
  if (!File.open(QIODevice::ReadOnly))
  {
    throw openfluid::base::FrameworkException("FluidXDescriptor::parseFile",
        "error opening " + m_CurrentFile);
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
        for(QDomElement CurrNode = Root.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
        {
          if (CurrNode.tagName() == QString("run"))
          {
            extractRunFromNode(CurrNode);
          }

          if (CurrNode.tagName() == QString("model"))
          {
            extractModelFromNode(CurrNode);
          }

          if (CurrNode.tagName() == QString("monitoring"))
          {
            extractMonitoringFromNode(CurrNode);
          }

          if (CurrNode.tagName() == QString("domain"))
          {
            extractDomainFomNode(CurrNode);
          }

          if (CurrNode.tagName() == QString("datastore"))
          {
            extractDatastoreFromNode(CurrNode);
          }
        }
      }
    }
    else
    {
      throw openfluid::base::FrameworkException(
                                         "FluidXDescriptor::parseFile",
                                         "file " + m_CurrentFile + " is empty");
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(
        "FluidXDescriptor::parseFile",
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
    throw openfluid::base::FrameworkException(
        "FluidXDescriptor::loadFromDirectory",
        "directory " + DirPath + " does not exist");

  std::vector<std::string> FluidXFilesToLoad = openfluid::tools::findFilesByExtension(DirPath, "fluidx", true);

  if (FluidXFilesToLoad.size() == 0)
    throw openfluid::base::FrameworkException("FluidXDescriptor::loadFromDirectory",
                                              "no fluidx file found in directory " + DirPath);

  std::sort(FluidXFilesToLoad.begin(), FluidXFilesToLoad.end());

  m_ModelDescriptor = openfluid::fluidx::CoupledModelDescriptor();
  m_RunDescriptor = openfluid::fluidx::RunDescriptor();
  m_DomainDescriptor = openfluid::fluidx::SpatialDomainDescriptor();
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
      mp_Listener->onFileLoaded(openfluid::base::Listener::LISTEN_OK);
    }
    catch (...)
    {
      mp_Listener->onFileLoaded(openfluid::base::Listener::LISTEN_ERROR);
      throw;
    }
  }

}


// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


void FluidXDescriptor::writeModelToStream(std::ostream& Contents)
{

  Contents << m_IndentStr << "<model>\n";

  if (m_ModelDescriptor.getGlobalParameters().size() > 0)
  {
    Contents << m_IndentStr << m_IndentStr << "<gparams>\n";
    Contents << getParamsAsStr(m_ModelDescriptor.getGlobalParameters());
    Contents << m_IndentStr << m_IndentStr << "</gparams>\n";
  }

  const std::list<openfluid::fluidx::ModelItemDescriptor*> Items =
      m_ModelDescriptor.items();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::PluggedSimulator))
    {
      openfluid::fluidx::SimulatorDescriptor* SimDesc =
          dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*it);

      Contents << m_IndentStr << m_IndentStr << "<simulator ID=\""
               << SimDesc->getID() << "\" "
               << "enabled=\"" << SimDesc->isEnabled() <<  "\">\n";
      Contents << getParamsAsStr(SimDesc->getParameters());
      Contents << m_IndentStr << m_IndentStr << "</simulator>\n";
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


      Contents << " enabled=\"" << GenDesc->isEnabled();
      Contents << "\">\n";
      Contents << getParamsAsStr(GenDesc->getParameters());
      Contents << m_IndentStr << m_IndentStr << "</generator>\n";
    }
  }

  Contents << m_IndentStr << "</model>\n";
}


// =====================================================================
// =====================================================================


std::string FluidXDescriptor::getParamsAsStr(
    const openfluid::ware::WareParams_t& Params) const
{
  std::string ParamsStr = "";

  for (openfluid::ware::WareParams_t::const_iterator it = Params.begin();
      it != Params.end(); ++it)
  {
    std::string EscapedValueStr =
        openfluid::tools::escapeXMLEntities(QString::fromStdString(it->second.data())).toStdString();

    ParamsStr += (m_IndentStr + m_IndentStr + m_IndentStr + "<param name=\""
                 + it->first + "\" value=\"" + EscapedValueStr + "\"/>\n");
  }

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


void FluidXDescriptor::writeDomainToStream(std::ostream& Contents)
{

  Contents << m_IndentStr << "<domain>\n";

  writeDomainDefinitionToStream(Contents);

  writeDomainAttributesToStream(Contents);

  writeDomainCalendarToStream(Contents);

  Contents << m_IndentStr << "</domain>\n";

}


// =====================================================================
// =====================================================================


void FluidXDescriptor::writeDomainDefinitionToStream(std::ostream& Contents)
{
  Contents << m_IndentStr << m_IndentStr << "<definition>\n";

  std::list<openfluid::fluidx::SpatialUnitDescriptor>& Units =
      m_DomainDescriptor.spatialUnits();

  std::list<openfluid::core::UnitClassID_t>::iterator itRel;

  for (std::list<openfluid::fluidx::SpatialUnitDescriptor>::iterator it =
      Units.begin(); it != Units.end(); ++it)
  {
    Contents << m_IndentStr << m_IndentStr << m_IndentStr << "<unit class=\""
             << it->getUnitsClass() << "\" ID=\"" << it->getID()
             << "\" pcsorder=\"" << it->getProcessOrder() << "\">\n";

    std::list<openfluid::core::UnitClassID_t>& Tos = it->toSpatialUnits();
    for (itRel = Tos.begin(); itRel != Tos.end(); ++itRel)
      Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr
               << "<to class=\"" << itRel->first << "\" ID=\"" << itRel->second
               << "\" />\n";

    std::list<openfluid::core::UnitClassID_t>& Parents = it->parentSpatialUnits();
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


void FluidXDescriptor::writeDomainAttributesToStream(std::ostream& Contents)
{
  std::list<AttributesDescriptor>& Attrs = m_DomainDescriptor.attributes();

  openfluid::fluidx::AttributesDescriptor::UnitIDAttribute_t::iterator itData;
  openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t::iterator itVal;

  for (std::list<AttributesDescriptor>::iterator it = Attrs.begin();
      it != Attrs.end(); ++it)
  {
    Contents << m_IndentStr << m_IndentStr << "<attributes unitclass=\""
             << it->getUnitsClass() << "\" colorder=\"";

    std::vector<std::string> Cols = it->columnsOrder();
    if (!Cols.empty())
    {
      int Max = Cols.size() - 1;
      for (int i = 0; i < Max; i++)
        Contents << Cols[i] << ";";
      Contents << Cols[Max];
    }

    Contents << "\">\n";

    openfluid::fluidx::AttributesDescriptor::UnitIDAttribute_t& DataMap =
        it->attributes();
    for (itData = DataMap.begin(); itData != DataMap.end(); ++itData)
    {
      Contents << itData->first << "\t";

      openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t& DataVals =
          itData->second;

      unsigned int i = 0;
      for (; i < Cols.size() - 1; i++)
      {
        Contents << DataVals[Cols[i]] << "\t";
      }
      Contents << DataVals[Cols[i]];

      Contents << "\n";
    }

    Contents << m_IndentStr << m_IndentStr << "</attributes>\n";
  }

}


// =====================================================================
// =====================================================================


void FluidXDescriptor::writeDomainCalendarToStream(std::ostream& Contents)
{
  std::list<EventDescriptor>& Events = m_DomainDescriptor.events();

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
             << it->event().getDateTime().getAsISOString() << "\">\n";

    openfluid::core::Event::EventInfosMap_t Infos = it->event().getInfos();
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


void FluidXDescriptor::writeRunConfigurationToStream(std::ostream& Contents)
{
  Contents << m_IndentStr << "<run>\n";

  if (m_RunDescriptor.isFilled())
  {

   // scheduling
    std::string ConstraintStr = "none";
    if (m_RunDescriptor.getSchedulingConstraint() == openfluid::base::SimulationStatus::SCHED_DTFORCED)
      ConstraintStr = "dt-forced";
    else if (m_RunDescriptor.getSchedulingConstraint() == openfluid::base::SimulationStatus::SCHED_DTCHECKED)
      ConstraintStr = "dt-checked";

    Contents << m_IndentStr << m_IndentStr << "<scheduling deltat=\""
             << m_RunDescriptor.getDeltaT() << "\" constraint=\""
             << ConstraintStr << "\" />\n";

    // period
    Contents << m_IndentStr << m_IndentStr << "<period begin=\""
             << m_RunDescriptor.getBeginDate().getAsISOString() << "\" end=\""
             << m_RunDescriptor.getEndDate().getAsISOString() << "\" />\n";

    // values buffer
    if (m_RunDescriptor.isUserValuesBufferSize())
      Contents << m_IndentStr << m_IndentStr << "<valuesbuffer size=\""
               << m_RunDescriptor.getValuesBufferSize() << "\" />\n";

  }

  Contents << m_IndentStr << "</run>\n";
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::writeDatastoreToStream(std::ostream& Contents)
{
  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t& Items =
      m_DatastoreDescriptor.items();


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

}


// =====================================================================
// =====================================================================


void FluidXDescriptor::writeMonitoringToStream(std::ostream& Contents)
{

  Contents << m_IndentStr << "<monitoring>\n";

  std::list<ObserverDescriptor*> Items = m_MonitoringDescriptor.items();

  for (std::list<ObserverDescriptor*>::iterator it = Items.begin();
      it != Items.end(); ++it)
  {
    Contents << m_IndentStr << m_IndentStr << "<observer ID=\""
             << (*it)->getID() << "\" " <<
             "enabled=\"" << (*it)->isEnabled() <<"\">\n";

    Contents << getParamsAsStr((*it)->getParameters());

    Contents << m_IndentStr << m_IndentStr << "</observer>\n";
  }

  Contents << m_IndentStr << "</monitoring>\n";
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
  writeModelToStream(OutFile);
  OutFile << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::LISTEN_OK);

  // domain
  OutFilename = boost::filesystem::path(DirPath + "/domain.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  writeDomainToStream(OutFile);
  OutFile << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::LISTEN_OK);

  // datastore
  OutFilename = boost::filesystem::path(DirPath + "/datastore.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  writeDatastoreToStream(OutFile);
  OutFile << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::LISTEN_OK);

  // monitoring
  OutFilename =
      boost::filesystem::path(DirPath + "/monitoring.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  writeMonitoringToStream(OutFile);
  OutFile << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::LISTEN_OK);


  // run
  OutFilename = boost::filesystem::path(DirPath + "/run.fluidx").string();
  mp_Listener->onFileWrite(OutFilename);
  OutFile.open(OutFilename.c_str(), std::ios::out);

  OutFile << "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  OutFile << "<openfluid>\n";
  writeRunConfigurationToStream(OutFile);
  OutFile << "\n\n";
  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::LISTEN_OK);



  mp_Listener->onWritten(openfluid::base::Listener::LISTEN_OK);
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

  writeModelToStream(OutFile);
  OutFile << "\n\n";
  writeDomainToStream(OutFile);
  OutFile << "\n\n";
  writeDatastoreToStream(OutFile);
  OutFile << "\n\n";
  writeMonitoringToStream(OutFile);
  OutFile << "\n\n";
  writeRunConfigurationToStream(OutFile);
  OutFile << "\n\n";

  OutFile << "</openfluid>\n";
  OutFile << "\n";

  OutFile.close();
  mp_Listener->onFileWritten(openfluid::base::Listener::LISTEN_OK);

  mp_Listener->onWritten(openfluid::base::Listener::LISTEN_OK);
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
      throw openfluid::base::FrameworkException(
                                         "FluidXDescriptor::prepareOutputDir",
                                         "Error creating output directory");
  }
}

// =====================================================================
// =====================================================================

}
} // namespaces
