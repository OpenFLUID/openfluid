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
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <fstream>

#include <QDomDocument>
#include <QFile>

#include <openfluid/base/IOListener.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/QtHelpers.hpp>


namespace openfluid { namespace fluidx {


FluidXDescriptor::FluidXDescriptor(openfluid::base::IOListener* Listener) :
    m_RunConfigDefined(false),m_ModelDefined(false),
    m_IndentStr(" "),
    mp_Listener(Listener)
{
  if (!mp_Listener)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Listener can not be NULL");
  }
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
    {
      return true;
    }
    else
    {
      return false;
    }
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


openfluid::ware::WareParams_t FluidXDescriptor::extractParamsFromNode(QDomElement& Node)
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
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "missing name and/or param attribute(s) in parameter definition (" + m_CurrentFile+ ")");
        }
      }
    }
  }

  return Params;
}


// =====================================================================
// =====================================================================


openfluid::ware::WareParams_t FluidXDescriptor::mergeParams(const openfluid::ware::WareParams_t& Params,
                                                            const openfluid::ware::WareParams_t& OverloadParams)
{
  openfluid::ware::WareParams_t FinalParams = Params;

  for (openfluid::ware::WareParams_t::const_iterator it = OverloadParams.begin(); it != OverloadParams.end(); ++it)
  {
    FinalParams[it->first] = it->second;
  }

  return FinalParams;
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::extractModelFromNode(QDomElement& Node)
{
  if (m_ModelDefined)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Duplicate model definition (" + m_CurrentFile + ")");
  }

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
      QString xmlUnitClass = CurrNode.attributeNode(QString("unitsclass")).value();
      if (xmlUnitClass.isEmpty())
      {
        xmlUnitClass = CurrNode.attributeNode(QString("unitclass")).value();
      }
      QString xmlMethod = CurrNode.attributeNode(QString("method")).value();
      QString xmlVarSize = CurrNode.attributeNode(QString("varsize")).value();
      unsigned int VarSize = 1;

      if (!xmlVarName.isNull() && !xmlUnitClass.isNull() && !xmlMethod.isNull())
      {
        openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GenMethod =
            openfluid::fluidx::GeneratorDescriptor::NoGenMethod;
        if (xmlMethod == QString("fixed"))
        {
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Fixed;
        }
        if (xmlMethod == QString("random"))
        {
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Random;
        }
        if (xmlMethod == QString("interp"))
        {
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Interp;
        }
        if (xmlMethod == QString("inject"))
        {
          GenMethod = openfluid::fluidx::GeneratorDescriptor::Inject;
        }

        if (GenMethod == openfluid::fluidx::GeneratorDescriptor::NoGenMethod)
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "unknown or missing generator method (" + m_CurrentFile + ")");
        }

        if (!xmlVarSize.isNull())
        {
          if (!openfluid::tools::convertString(xmlVarSize.toStdString(),
                                               &VarSize))
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "wrong variable size format in generator (" +m_CurrentFile+ ")");
          }
        }

        GD = new openfluid::fluidx::GeneratorDescriptor(
            xmlVarName.toStdString(), xmlUnitClass.toStdString(), GenMethod,
            VarSize);
        GD->setParameters(extractParamsFromNode(CurrNode));
        GD->setEnabled(extractWareEnabledFromNode(CurrNode));
        m_ModelDescriptor.appendItem(GD);
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "missing attribute(s) in generator description ("+m_CurrentFile+")");
      }
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
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Duplicate run configuration (" + m_CurrentFile + ")");
  }

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
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "wrong format for begin date of period (" + m_CurrentFile + ")");
        }

        if (ReadDate.setFromISOString(xmlEnd.toStdString()))
        {
          m_RunDescriptor.setEndDate(ReadDate);
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "wrong format for end date of period (" + m_CurrentFile + ")");
        }

        FoundPeriod = true;

        if (!(m_RunDescriptor.getEndDate() > m_RunDescriptor.getBeginDate()))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "end date must be strictly greater than begin date for period (" + m_CurrentFile + ")");
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
            "missing begin and/or end attributes for period tag (" + m_CurrentFile+ ")");
      }
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
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "empty or wrong value for deltat (" + m_CurrentFile + ")");
        }

        m_RunDescriptor.setDeltaT(DeltaT);
        FoundDeltaT = true;
      }

      if (!xmlConstraint.isNull())
      {
        std::string ReadConstraintStr = xmlConstraint.toStdString();
        if (ReadConstraintStr == "dt-checked")
        {
          m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_DTCHECKED);
        }
        else if (ReadConstraintStr == "dt-forced")
        {
          m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_DTFORCED);
        }
        else if (ReadConstraintStr == "none")
        {
          m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_NONE);
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "wrong value for scheduling constraint (" + m_CurrentFile + ")");
        }
      }
      else
      {
        m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_NONE);
      }
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
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "wrong format for size attribute for valuesbuffer tag (" + m_CurrentFile + ")");
        }

        m_RunDescriptor.setValuesBufferSize(ReadSteps);
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
            "missing size attribute for valuesbuffer tag (" + m_CurrentFile + ")");
      }
    }
  }

  if (!FoundPeriod)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"run period not found (" + m_CurrentFile + ")");
  }

  if (!FoundDeltaT)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"deltat not found (" + m_CurrentFile + ")");
  }

  m_RunConfigDefined = true;

  m_RunDescriptor.setFilled(true);
}


// =====================================================================
// =====================================================================


openfluid::core::UnitClassID_t FluidXDescriptor::extractUnitClassIDFromNode(QDomElement& Node)
{
  QString xmlUnitID = Node.attributeNode(QString("ID")).value();
  QString xmlUnitClass = Node.attributeNode(QString("class")).value();

  if (!xmlUnitID.isNull() && !xmlUnitClass.isNull())
  {
    openfluid::core::UnitID_t UnitID;

    if (!openfluid::tools::convertString(xmlUnitID.toStdString(),&UnitID))
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "wrong format for ID in unit definition (" + m_CurrentFile + ")");
    }

    return std::make_pair(xmlUnitClass.toStdString(), UnitID);
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
        "missing or wrong attribute(s) in units link definition (" + m_CurrentFile + ")");
  }
}


// =====================================================================
// =====================================================================


FluidXDescriptor::SpatialUnitsList_t FluidXDescriptor::extractDomainDefinitionFromNode(QDomElement& Node)
{
  SpatialUnitsList_t UnitsList;

  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("unit"))
    {
      QString xmlUnitID = CurrNode.attributeNode(QString("ID")).value();
      QString xmlUnitClass = CurrNode.attributeNode(QString("class")).value();
      QString xmlPcsOrd = CurrNode.attributeNode(QString("pcsorder")).value();

      if (!xmlUnitID.isNull() && !xmlUnitClass.isNull() && !xmlPcsOrd.isNull())
      {
        openfluid::fluidx::SpatialUnitDescriptor UnitDesc;
        openfluid::core::PcsOrd_t PcsOrder;
        openfluid::core::UnitID_t UnitID;

        UnitDesc.setUnitsClass(xmlUnitClass.toStdString());

        if (!openfluid::tools::convertString(xmlUnitID.toStdString(),&UnitID))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "wrong format for ID in unit definition (" + m_CurrentFile + ")");
        }

        if (!openfluid::tools::convertString(xmlPcsOrd.toStdString(),&PcsOrder))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "wrong format for process order in unit definition (" + m_CurrentFile + ")");
        }

        UnitDesc.setProcessOrder(PcsOrder);
        UnitDesc.setID(UnitID);


        for(QDomElement CurrLinkNode = CurrNode.firstChildElement();
            !CurrLinkNode.isNull(); CurrLinkNode = CurrLinkNode.nextSiblingElement())
        {
          if (CurrLinkNode.tagName() == QString("to"))
          {
            UnitDesc.toSpatialUnits().push_back(extractUnitClassIDFromNode(CurrLinkNode));
          }

          if (CurrLinkNode.tagName() == QString("childof"))
          {
            UnitDesc.parentSpatialUnits().push_back(extractUnitClassIDFromNode(CurrLinkNode));
          }
        }

        UnitsList.push_back(UnitDesc);
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
            "missing or wrong attribute(s) in unit definition (" + m_CurrentFile + ")");
      }
    }
  }

  return UnitsList;
}


// =====================================================================
// =====================================================================


AttributesTableDescriptor FluidXDescriptor::extractDomainAttributesFromNode(QDomElement& Node)
{

  QString xmlUnitClass = Node.attributeNode(QString("unitsclass")).value();

  if (xmlUnitClass.isEmpty())
  {
    xmlUnitClass = Node.attributeNode(QString("unitclass")).value();
  }

  QString xmlColOrder = Node.attributeNode(QString("colorder")).value();

  if (!xmlUnitClass.isNull() && !xmlColOrder.isNull())
  {
    openfluid::fluidx::AttributesTableDescriptor AttrsDesc;

    AttrsDesc.setUnitsClass(xmlUnitClass.toStdString());

    std::vector<std::string> ColOrder;

    ColOrder = openfluid::tools::splitString(xmlColOrder.toStdString(),
                                             ";");

    if (ColOrder.empty())
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
          "wrong or empty colorder attribute in domain attributes (" + m_CurrentFile + ")");
    }

    AttrsDesc.setColumnsOrder(ColOrder);

    QString xmlDataBlob = Node.text();

    if (!xmlDataBlob.isNull())
    {
      AttrsDesc.parseDataBlob(xmlDataBlob.toStdString());
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
          "wrong or empty data content in domain attributes (" + m_CurrentFile + ")");
    }

    return AttrsDesc;
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
        "missing or wrong unitsclass attribute(s) in domain attributes (" + m_CurrentFile + ")");
  }
}


// =====================================================================
// =====================================================================


FluidXDescriptor::EventsList_t FluidXDescriptor::extractDomainCalendarFromNode(QDomElement& Node)
{
  EventsList_t EventsList;

  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("event"))
    {
      QString xmlUnitID = CurrNode.attributeNode(QString("unitID")).value();
      QString xmlUnitClass = CurrNode.attributeNode(QString("unitsclass")).value();
      if (xmlUnitClass.isEmpty())
      {
        xmlUnitClass = CurrNode.attributeNode(QString("unitclass")).value();
      }
      QString xmlDate = CurrNode.attributeNode(QString("date")).value();

      if (!xmlUnitID.isNull() && !xmlUnitClass.isNull() && !xmlDate.isNull())
      {
        openfluid::fluidx::EventDescriptor EvDesc;

        openfluid::core::UnitID_t UnitID;

        if (!openfluid::tools::convertString(xmlUnitID.toStdString(),&UnitID))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "wrong format for unit ID in event (" + m_CurrentFile + ")");
        }

        openfluid::core::DateTime EventDate;

        if (!EventDate.setFromISOString(xmlDate.toStdString()))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "wrong format for date in event (" + m_CurrentFile + ")");
        }

        EvDesc.setUnitsClass(xmlUnitClass.toStdString());
        EvDesc.setUnitID(UnitID);
        EvDesc.setEvent(openfluid::core::Event(EventDate));


        for(QDomElement CurrInfoNode = CurrNode.firstChildElement(); !CurrInfoNode.isNull();
            CurrInfoNode = CurrInfoNode.nextSiblingElement())
        {
          if (CurrInfoNode.tagName() == QString("info"))
          {

            QString xmlKey = CurrInfoNode.attributeNode(QString("key")).value();
            QString xmlValue = CurrInfoNode.attributeNode(QString("value")).value();

            if (!xmlKey.isNull() && !xmlValue.isNull())
            {
              EvDesc.event().addInfo(xmlKey.toStdString(),xmlValue.toStdString());
            }
            else
            {
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                  "wrong or missing attribute(s) in domain calendar event info (" + m_CurrentFile + ")");
            }
          }

        }

        EventsList.push_back(EvDesc);
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
            "wrong or missing attribute(s) in domain calendar event (" + m_CurrentFile + ")");
      }
    }
  }

  return EventsList;
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::extractDomainFomNode(QDomElement& Node, FluidXDescriptor::LoadingTempData& TempData)
{
  for(QDomElement CurrNode = Node.firstChildElement(); !CurrNode.isNull(); CurrNode = CurrNode.nextSiblingElement())
  {
    if (CurrNode.tagName() == QString("definition"))
    {

      SpatialUnitsList_t List = extractDomainDefinitionFromNode(CurrNode);
      TempData.SpatiaUnits.insert(TempData.SpatiaUnits.end(),List.begin(),List.end());
    }

    if (CurrNode.tagName() == QString("attributes"))
    {
      TempData.Attributes.push_back(extractDomainAttributesFromNode(CurrNode));
    }

    if (CurrNode.tagName() == QString("calendar"))
    {
      EventsList_t List = extractDomainCalendarFromNode(CurrNode);
      TempData.Events.insert(TempData.Events.end(),List.begin(),List.end());
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
      QString xmlDataClass = CurrNode.attributeNode(QString("unitsclass")).value();
      if (xmlDataClass.isEmpty())
      {
        xmlDataClass = CurrNode.attributeNode(QString("unitclass")).value();
      }

      if (!xmlDataID.isNull() && !xmlDataType.isNull() && !xmlDataSrc.isNull())
      {
        std::string DataID;

        if (!openfluid::tools::convertString(xmlDataID.toStdString(),&DataID) || DataID.empty())
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "wrong format or missing data ID in datastore (" + m_CurrentFile + ")");
        }

        openfluid::core::UnstructuredValue::UnstructuredType DataType;

        if (!openfluid::core::UnstructuredValue::getValueTypeFromString(xmlDataType.toStdString(), DataType))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "unknown or missing datatype in dataitem definition (" + m_CurrentFile + ")");
        }

        openfluid::fluidx::DatastoreItemDescriptor* Item =
          new openfluid::fluidx::DatastoreItemDescriptor(DataID, m_CurrentDir, xmlDataSrc.toStdString(),DataType);

        if (!xmlDataClass.isNull())
        {
          Item->setUnitsClass(xmlDataClass.toStdString());
        }

        if (!m_DatastoreDescriptor.appendItem(Item))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "already existing dataitem ID: " + DataID + " (" + m_CurrentFile + ")");
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
            "missing or wrong attribute(s) in dataitem definition (" + m_CurrentFile + ")");
      }
    }

  }
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::parseFile(const std::string& Filename, FluidXDescriptor::LoadingTempData& TempData)
{
  QDomDocument Doc;
  QDomElement Root;

  m_CurrentFile = Filename;

  QFile File(QString(m_CurrentFile.c_str()));
  if (!File.open(QIODevice::ReadOnly))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"error opening " + m_CurrentFile);
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
        for(QDomElement CurrNode = Root.firstChildElement();
            !CurrNode.isNull();
            CurrNode = CurrNode.nextSiblingElement())
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
            extractDomainFomNode(CurrNode,TempData);
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
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "file " + m_CurrentFile + " is empty");
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "file " + m_CurrentFile + " cannot be parsed");
  }
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::rebuildSpatialDomainFromTemp(LoadingTempData& TempData)
{
  // structure

  for (const auto& SpatialUnit : TempData.SpatiaUnits)
  {
    m_DomainDescriptor.addUnit(SpatialUnit,false);
  }


  // attributes

  for (const auto& AttrTable : TempData.Attributes)
  {
    openfluid::core::UnitsClass_t UnitsClass = AttrTable.getUnitsClass();

    for (const auto& AttrName : AttrTable.columnsOrder())
    {
      m_DomainDescriptor.addAttribute(UnitsClass,AttrName,std::string(),false);
    }
  }

  for (const auto& AttrTable : TempData.Attributes)
  {
    openfluid::core::UnitsClass_t UnitsClass = AttrTable.getUnitsClass();

    for (const auto& UnitsAttrs : AttrTable.attributes())
    {
      for (const auto& Attr : UnitsAttrs.second)
      {
        m_DomainDescriptor.setAttribute(UnitsClass,UnitsAttrs.first,Attr.first,Attr.second);
      }
    }
  }


  // events

  for (const auto& Event : TempData.Events)
  {
    m_DomainDescriptor.addEvent(Event);
  }

  m_DomainDescriptor.checkUnitsRelations();
  m_DomainDescriptor.checkAttributesConsistency();
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::loadFromDirectory(const std::string& DirPath)
{
  if (!openfluid::tools::Filesystem::isDirectory(DirPath))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"directory " + DirPath + " does not exist");
  }

  std::vector<std::string> FluidXFilesToLoad = openfluid::tools::findFilesByExtension(DirPath, "fluidx", true);

  if (FluidXFilesToLoad.size() == 0)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"no fluidx file found in directory " + DirPath);
  }

  std::sort(FluidXFilesToLoad.begin(), FluidXFilesToLoad.end());

  m_ModelDescriptor = openfluid::fluidx::CoupledModelDescriptor();
  m_RunDescriptor = openfluid::fluidx::RunConfigurationDescriptor();
  m_DomainDescriptor = openfluid::fluidx::SpatialDomainDescriptor();
  m_DatastoreDescriptor = openfluid::fluidx::DatastoreDescriptor();
  m_MonitoringDescriptor = openfluid::fluidx::MonitoringDescriptor();

  m_RunConfigDefined = false;
  m_ModelDefined = false;

  LoadingTempData TempData;

  unsigned int i = 0;

  std::string CurrentFile;

  m_CurrentDir = DirPath;

  for (i = 0; i < FluidXFilesToLoad.size(); i++)
  {
    CurrentFile = FluidXFilesToLoad[i];
    try
    {
      mp_Listener->onFileLoad(openfluid::tools::Filesystem::filename(CurrentFile));
      parseFile(CurrentFile,TempData);
      mp_Listener->onFileLoaded(openfluid::base::Listener::LISTEN_OK);
    }
    catch (...)
    {
      mp_Listener->onFileLoaded(openfluid::base::Listener::LISTEN_ERROR);
      throw;
    }
  }

  rebuildSpatialDomainFromTemp(TempData);
}


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

  const std::list<openfluid::fluidx::ModelItemDescriptor*> Items = m_ModelDescriptor.items();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it = Items.begin(); it != Items.end(); ++it)
  {
    if ((*it)->isType(openfluid::ware::WareType::SIMULATOR))
    {
      openfluid::fluidx::SimulatorDescriptor* SimDesc =
          dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(*it);

      Contents << m_IndentStr << m_IndentStr << "<simulator ID=\""
               << SimDesc->getID() << "\" "
               << "enabled=\"" << SimDesc->isEnabled() <<  "\">\n";
      Contents << getParamsAsStr(SimDesc->getParameters());
      Contents << m_IndentStr << m_IndentStr << "</simulator>\n";
    }
    else if ((*it)->isType(openfluid::ware::WareType::GENERATOR))
    {
      openfluid::fluidx::GeneratorDescriptor* GenDesc =
          dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(*it);

      Contents << m_IndentStr << m_IndentStr << "<generator varname=\""
               << GenDesc->getVariableName() << "\" " << "unitsclass=\""
               << GenDesc->getUnitsClass() << "\" " << "method=\""
               << getGeneratorMethodAsStr(GenDesc->getGeneratorMethod())
               << "\"";

      if (GenDesc->getVariableSize() != 1)
      {
        Contents << " varsize=\"" << GenDesc->getVariableSize() << "\"";
      }


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


std::string FluidXDescriptor::getParamsAsStr(const openfluid::ware::WareParams_t& Params) const
{
  std::string ParamsStr = "";

  for (openfluid::ware::WareParams_t::const_iterator it = Params.begin();it != Params.end(); ++it)
  {
    std::string EscapedValueStr =
        openfluid::tools::escapeXMLEntities(QString::fromStdString(it->second.data())).toStdString();
    if (!EscapedValueStr.empty())
    {
      ParamsStr += (m_IndentStr + m_IndentStr + m_IndentStr + "<param name=\""
                   + it->first + "\" value=\"" + EscapedValueStr + "\"/>\n");
    }
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


template<class T>
std::string FluidXDescriptor::getSeparatedStrFromList(const T& Data,const std::string& Sep)
{
  bool isFirstItem = true;

  std::ostringstream ss;

  for (auto& Item : Data)
  {
    if (isFirstItem)
    {
      isFirstItem = false;
    }
    else
    {
      ss << Sep;
    }

    ss << Item;
  }

  return ss.str();
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

  for (const auto& UnitsClass : spatialDomain().spatialUnits())
  {
    for (const auto& Unit : UnitsClass.second)
    {
      Contents << m_IndentStr << m_IndentStr << m_IndentStr
               << "<unit class=\"" << UnitsClass.first << "\" ID=\"" << Unit.second.getID()
               << "\" pcsorder=\"" << Unit.second.getProcessOrder() << "\">\n";

      for (const auto& LinkedUnit : Unit.second.toSpatialUnits())
      {
        Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr
                 << "<to class=\"" << LinkedUnit.first << "\" ID=\"" << LinkedUnit.second << "\" />\n";
      }

      for (const auto& LinkedUnit : Unit.second.parentSpatialUnits())
      {
        Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr
                 << "<childof class=\"" << LinkedUnit.first << "\" ID=\"" << LinkedUnit.second << "\" />\n";
      }
      Contents << m_IndentStr << m_IndentStr << m_IndentStr << "</unit>\n";
    }
  }

  Contents << m_IndentStr << m_IndentStr << "</definition>\n";
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::writeDomainAttributesToStream(std::ostream& Contents)
{
  for (const auto& UnitsClass : spatialDomain().spatialUnits())
  {
    auto AttrsNames = spatialDomain().getAttributesNames(UnitsClass.first);

    if (!AttrsNames.empty())
    {

      Contents << m_IndentStr << m_IndentStr << "<attributes unitsclass=\"" << UnitsClass.first << "\" colorder=\"";

      Contents << getSeparatedStrFromList(AttrsNames,";");
      Contents << "\">\n";

      for (const auto& Unit : UnitsClass.second)
      {
        std::vector<std::string> Values;
        const auto& Attributes = Unit.second.attributes();
        transform(Attributes.begin(),Attributes.end(), back_inserter(Values),
                  [](const std::map<openfluid::core::AttributeName_t,std::string>::value_type& Val)
                     {
                        return Val.second;
                     });

        Contents << Unit.second.getID() << "\t" << getSeparatedStrFromList(Values,"\t") << "\n";
      }

      Contents << m_IndentStr << m_IndentStr << "</attributes>\n";
    }
  }
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::writeDomainCalendarToStream(std::ostream& Contents)
{
  Contents << m_IndentStr << m_IndentStr << "<calendar>\n";

  for (const auto& UnitsClass : spatialDomain().spatialUnits())
  {
    for (const auto& Unit : UnitsClass.second)
    {
      const auto& Events = Unit.second.events();

      for (const auto& EvDesc : Events)
      {
        Contents << m_IndentStr << m_IndentStr << m_IndentStr
                 << "<event unitsclass=\"" << UnitsClass.first << "\" " << "unitID=\"" << Unit.second.getID()
                 << "\" " << "date=\""<< EvDesc.event().getDateTime().getAsISOString() << "\">\n";

        const auto& Infos = EvDesc.event().getInfos();
        for (const auto& Inf : Infos)
        {
          Contents << m_IndentStr << m_IndentStr << m_IndentStr << m_IndentStr
                   << "<info key=\"" << Inf.first << "\" value=\"" << Inf.second << "\" />\n";
        }
        Contents << m_IndentStr << m_IndentStr << m_IndentStr << "</event>\n";
      }
    }
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
    {
      ConstraintStr = "dt-forced";
    }
    else if (m_RunDescriptor.getSchedulingConstraint() == openfluid::base::SimulationStatus::SCHED_DTCHECKED)
    {
      ConstraintStr = "dt-checked";
    }

    Contents << m_IndentStr << m_IndentStr << "<scheduling deltat=\""
             << m_RunDescriptor.getDeltaT() << "\" constraint=\""
             << ConstraintStr << "\" />\n";

    // period
    Contents << m_IndentStr << m_IndentStr << "<period begin=\""
             << m_RunDescriptor.getBeginDate().getAsISOString() << "\" end=\""
             << m_RunDescriptor.getEndDate().getAsISOString() << "\" />\n";

    // values buffer
    if (m_RunDescriptor.isUserValuesBufferSize())
    {
      Contents << m_IndentStr << m_IndentStr << "<valuesbuffer size=\""
               << m_RunDescriptor.getValuesBufferSize() << "\" />\n";
    }

  }

  Contents << m_IndentStr << "</run>\n";
}


// =====================================================================
// =====================================================================


void FluidXDescriptor::writeDatastoreToStream(std::ostream& Contents)
{
  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t& Items = m_DatastoreDescriptor.items();


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

    if (!(*it)->getUnitsClass().empty())
    {
      Contents << "unitsclass=\"" << (*it)->getUnitsClass() << "\" ";
    }

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


void FluidXDescriptor::writeToManyFiles(const std::string& DirPath)
{
  mp_Listener->onWrite();

  std::ofstream OutFile;
  std::string OutFilename;

  prepareFluidXDir(DirPath);


  // model
  OutFilename = openfluid::tools::Filesystem::joinPath({DirPath,"model.fluidx"});

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
  OutFilename = openfluid::tools::Filesystem::joinPath({DirPath,"domain.fluidx"});

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
  OutFilename = openfluid::tools::Filesystem::joinPath({DirPath,"datastore.fluidx"});

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
  OutFilename = openfluid::tools::Filesystem::joinPath({DirPath,"monitoring.fluidx"});

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
  OutFilename = openfluid::tools::Filesystem::joinPath({DirPath,"run.fluidx"});

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


void FluidXDescriptor::writeToSingleFile(const std::string& FilePath)
{
  mp_Listener->onWrite();

  std::ofstream OutFile;

  prepareFluidXDir(openfluid::tools::Filesystem::dirname(FilePath));

  std::string OutFilename = FilePath;

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


void FluidXDescriptor::prepareFluidXDir(const std::string& DirPath)
{

  if (!openfluid::tools::Filesystem::isDirectory(DirPath))
  {
    openfluid::tools::Filesystem::makeDirectory(DirPath);

    if (!openfluid::tools::Filesystem::isDirectory(DirPath))
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error creating output directory");
    }
  }
}


} } // namespaces
