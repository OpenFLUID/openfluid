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
  @file FluidXIO.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <algorithm>
#include <fstream>
#include <functional>

#include <tinyxml2.h>

#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/TinyXML2Helpers.hpp>


namespace openfluid { namespace fluidx {


class FluidXReaderImplementation
{
  private:

    openfluid::fluidx::FluidXDescriptor& m_Descriptor;

    openfluid::base::IOListener* mp_Listener = nullptr;

    typedef std::list<SpatialUnitDescriptor> SpatialUnitsList_t;

    typedef std::list<AttributesTableDescriptor> AttributesTableList_t;

    typedef std::list<EventDescriptor> EventsList_t;


    struct LoadingTempData
    {
      SpatialUnitsList_t SpatiaUnits;

      AttributesTableList_t Attributes;

      EventsList_t Events;
    };


    std::string m_CurrentFile;

    std::string m_CurrentDir;

    bool m_ModelDefined = false;

    bool m_RunConfigDefined = false;


    // =====================================================================
    // =====================================================================


    bool extractWareEnabled(const tinyxml2::XMLElement* Elt)
    {
      std::string Enabled = openfluid::tools::attributeToString(Elt,"enabled");

      if (Enabled == "1" || Enabled == "true" || Enabled.empty())
      {
        return true;
      }

      return false;
    }


    // =====================================================================
    // =====================================================================


    void extractMonitoring(const tinyxml2::XMLElement* MonElt)
    { 

      for (auto Elt = MonElt->FirstChildElement("observer"); Elt != nullptr; 
           Elt = Elt->NextSiblingElement("observer"))
      {
        std::string ID = openfluid::tools::attributeToString(Elt,"ID");

        if (!ID.empty())
        {
          auto OD = new openfluid::fluidx::ObserverDescriptor(ID);
          OD->setParameters(extractParams(Elt));
          OD->setEnabled(extractWareEnabled(Elt));
          m_Descriptor.m_MonitoringDescriptor.appendItem(OD);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    openfluid::ware::WareParams_t extractParams(const tinyxml2::XMLElement* WareElt)
    {
      openfluid::ware::WareParams_t Params;

      if (WareElt != nullptr)
      {
        for (auto Elt = WareElt->FirstChildElement("param"); Elt != nullptr; 
             Elt = Elt->NextSiblingElement("param"))
        {
          std::string Name = openfluid::tools::attributeToString(Elt,"name");
          std::string Value = openfluid::tools::attributeToString(Elt,"value");

          if (!Name.empty())
          {
            Params[Name] = openfluid::ware::WareParamValue_t(Value);
          }
          else
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "missing name and/or param attribute(s) in parameter definition (" + m_CurrentFile+ ")");
          }
        }
      }

      return Params;
    }


    // =====================================================================
    // =====================================================================


    openfluid::ware::WareParams_t mergeParams(const openfluid::ware::WareParams_t& Params,
                                              const openfluid::ware::WareParams_t& OverloadParams)
    {
      openfluid::ware::WareParams_t FinalParams = Params;

      for (auto it = OverloadParams.begin(); it != OverloadParams.end(); ++it)
      {
        FinalParams[it->first] = it->second;
      }

      return FinalParams;
    }


    // =====================================================================
    // =====================================================================


    void extractModel(const tinyxml2::XMLElement* ModelElt)
    {
      openfluid::ware::WareParams_t GParams;

      for (auto Elt = ModelElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
      {
        std::string TagName(Elt->Name());

        if (TagName == "gparams")
        {
          GParams = mergeParams(GParams, extractParams(Elt));
        }
        else if (TagName == "simulator")
        {
          std::string ID = openfluid::tools::attributeToString(Elt,"ID");

          if (!ID.empty())
          {
            auto SD = new openfluid::fluidx::SimulatorDescriptor(ID);
            SD->setParameters(extractParams(Elt));
            SD->setEnabled(extractWareEnabled(Elt));
            m_Descriptor.m_ModelDescriptor.appendItem(SD);
          }
        }
        else if (TagName == "generator")
        {
          std::string VarName = openfluid::tools::attributeToString(Elt,"varname");
          std::string UnitsClass = openfluid::tools::attributeToString(Elt,"unitsclass");
          std::string Method = openfluid::tools::attributeToString(Elt,"method");
          std::string VarSize = openfluid::tools::attributeToString(Elt,"varsize");
          unsigned int VarSizeNum = 1;

          if (!VarName.empty() && !UnitsClass.empty() && !Method.empty())
          {
            openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GenMethod =
                openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::NONE;
            if (Method == "fixed")
            {
              GenMethod = openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED;
            }
            if (Method == "random")
            {
              GenMethod = openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM;
            }
            if (Method == "interp")
            {
              GenMethod = openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP;
            }
            if (Method == "inject")
            {
              GenMethod = openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT;
            }

            if (GenMethod == openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::NONE)
            {
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                        "unknown or missing generator method (" + m_CurrentFile + ")");
            }

            if (!VarSize.empty())
            {
              if (!openfluid::tools::convertString(VarSize,&VarSizeNum))
              {
                throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                          "wrong variable size format in generator ("+
                                                          m_CurrentFile+ ")");
              }
            }

            auto GD = new openfluid::fluidx::GeneratorDescriptor(VarName,UnitsClass,GenMethod,VarSizeNum);
            GD->setParameters(extractParams(Elt));
            GD->setEnabled(extractWareEnabled(Elt));
            m_Descriptor.m_ModelDescriptor.appendItem(GD);
          }
          else
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "missing attribute(s) in generator description ("+
                                                      m_CurrentFile+")");
          }
        }
      }

      m_Descriptor.m_ModelDescriptor.setGlobalParameters(GParams);
      m_ModelDefined = true;
    }


    // =====================================================================
    // =====================================================================


    void extractRun(const tinyxml2::XMLElement* RunElt)
    {
      bool FoundDeltaT = false;
      bool FoundPeriod = false;

      for (auto Elt = RunElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
      {
        std::string TagName(Elt->Name());

        if (TagName == "period")
        {
          std::string Begin = openfluid::tools::attributeToString(Elt,"begin");
          std::string End = openfluid::tools::attributeToString(Elt,"end");

          if (!Begin.empty() && !End.empty())
          {  
            openfluid::core::DateTime BeginDate;
            openfluid::core::DateTime EndDate;

            if (!BeginDate.setFromISOString(Begin) || !EndDate.setFromISOString(End))
            {
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                        "wrong format for begin or end date of run period ("+
                                                        m_CurrentFile + ")");
            }

            if (!(EndDate > BeginDate))
            {
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "end date must be strictly greater than begin date for period (" + m_CurrentFile + ")");
            }

            FoundPeriod = true;
            m_Descriptor.m_RunDescriptor.setBeginDate(BeginDate);
            m_Descriptor.m_RunDescriptor.setEndDate(EndDate);
          }
          else
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "missing begin and/or end attributes for period tag (" + m_CurrentFile + ")");
          }
        }
        else if (TagName == "scheduling")
        {
          std::string DeltaT = openfluid::tools::attributeToString(Elt,"deltat");
          std::string Constraint = openfluid::tools::attributeToString(Elt,"constraint");

          if (!DeltaT.empty())
          {
            int DeltaTNum;

            if (!openfluid::tools::convertString(DeltaT, &DeltaTNum))
            {
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                        "empty or wrong value for deltat (" + m_CurrentFile + ")");
            }

            m_Descriptor.m_RunDescriptor.setDeltaT(DeltaTNum);
            FoundDeltaT = true;
          }

          if (!Constraint.empty())
          {
            if (Constraint == "dt-checked")
            {
              m_Descriptor.m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_DTCHECKED);
            }
            else if (Constraint == "dt-forced")
            {
              m_Descriptor.m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_DTFORCED);
            }
            else if (Constraint == "none")
            {
              m_Descriptor.m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_NONE);
            }
            else
            {
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                        "wrong value for scheduling constraint ("+
                                                        m_CurrentFile + ")");
            }
          }
          else
          {
            m_Descriptor.m_RunDescriptor.setSchedulingConstraint(openfluid::base::SimulationStatus::SCHED_NONE);
          }
        }
        else if (TagName == "valuesbuffer")
        {
          std::string Size = openfluid::tools::attributeToString(Elt,"size");

          if (!Size.empty())
          {
            unsigned int SizeNum;

            if (!openfluid::tools::convertString(Size,&SizeNum))
            {
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                  "wrong format for size attribute for valuesbuffer tag (" + m_CurrentFile + ")");
            }

            m_Descriptor.m_RunDescriptor.setValuesBufferSize(SizeNum);
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
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"run period not found ("+
                                                                          m_CurrentFile + ")");
      }
      if (!FoundDeltaT)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"deltat not found (" + m_CurrentFile + ")");
      }

      m_Descriptor.m_RunDescriptor.setFilled(true);
      m_RunConfigDefined = true;
    }


    // =====================================================================
    // =====================================================================


    openfluid::core::UnitClassID_t extractUnitClassID(const tinyxml2::XMLElement* Elt)
    {
      std::string UnitID = openfluid::tools::attributeToString(Elt,"ID");
      std::string UnitClass = openfluid::tools::attributeToString(Elt,"class");

      if (!UnitID.empty() && !UnitClass.empty())
      {
        unsigned int UnitIDNum;
        if (!openfluid::tools::convertString(UnitID,&UnitIDNum))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "wrong format for ID in unit definition (" + m_CurrentFile + ")");
        }
        return std::make_pair(UnitClass,UnitIDNum);
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
            "missing or wrong attribute(s) in units link definition (" + m_CurrentFile + ")");
      }
    }


    // =====================================================================
    // =====================================================================


    SpatialUnitsList_t extractDomainDefinition(const tinyxml2::XMLElement* DefElt)
    {
      SpatialUnitsList_t UnitsList;

      for (auto UnitElt = DefElt->FirstChildElement("unit"); UnitElt != nullptr; 
           UnitElt = UnitElt->NextSiblingElement("unit"))
      {
        std::string ID = openfluid::tools::attributeToString(UnitElt,"ID");
        std::string UnitsClass = openfluid::tools::attributeToString(UnitElt,"class");
        std::string PcsOrd = openfluid::tools::attributeToString(UnitElt,"pcsorder");

        if (!ID.empty() && !UnitsClass.empty() && !PcsOrd.empty())
        {
          openfluid::fluidx::SpatialUnitDescriptor UnitDesc;
          openfluid::core::PcsOrd_t PcsOrdNum;
          openfluid::core::UnitID_t IDNum;

          UnitDesc.setUnitsClass(UnitsClass);

          if (!openfluid::tools::convertString(ID,&IDNum))
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "wrong format for ID in unit definition (" + m_CurrentFile + ")");
          }

          if (!openfluid::tools::convertString(PcsOrd,&PcsOrdNum))
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "wrong format for process order in unit definition (" + m_CurrentFile + ")");
          }

          UnitDesc.setProcessOrder(PcsOrdNum);
          UnitDesc.setID(IDNum);

          for (auto LinkElt = UnitElt->FirstChildElement(); LinkElt != nullptr; 
               LinkElt = LinkElt->NextSiblingElement())
          {
            std::string TagName(LinkElt->Name());

            if (TagName == "to")
            {
              UnitDesc.toSpatialUnits().push_back(extractUnitClassID(LinkElt));
            }

            if (TagName == "childof")
            {
              UnitDesc.parentSpatialUnits().push_back(extractUnitClassID(LinkElt));
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

      return UnitsList;
    }


    // =====================================================================
    // =====================================================================


    AttributesTableDescriptor extractDomainAttributes(const tinyxml2::XMLElement* AttrsElt)
    {
      std::string UnitsClass = openfluid::tools::attributeToString(AttrsElt,"unitsclass");
      std::string ColOrder = openfluid::tools::attributeToString(AttrsElt,"colorder");

      if (UnitsClass.empty() || ColOrder.empty())
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
            "missing or wrong unitsclass or colorder in domain attributes (" + m_CurrentFile + ")");
      }

      auto ColOrderVect = openfluid::tools::splitString(ColOrder,";");
      if (ColOrderVect.empty())
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
            "wrong or empty colorder attribute in domain attributes (" + m_CurrentFile + ")");
      }

      std::string DataBlob = openfluid::tools::textToString(AttrsElt);
      if (DataBlob.empty())
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
            "wrong or empty data content in domain attributes (" + m_CurrentFile + ")");
      }
      
      openfluid::fluidx::AttributesTableDescriptor AttrsDesc;

      AttrsDesc.setColumnsOrder(ColOrderVect);
      AttrsDesc.setUnitsClass(UnitsClass);
      AttrsDesc.parseDataBlob(DataBlob);

      return AttrsDesc;
    }


    // =====================================================================
    // =====================================================================


    EventsList_t extractDomainCalendar(const tinyxml2::XMLElement* CalElt)
    {
      EventsList_t EventsList;

      for (auto EvElt = CalElt->FirstChildElement("event"); EvElt != nullptr;
           EvElt = EvElt->NextSiblingElement("event"))
      {
        std::string ID = openfluid::tools::attributeToString(EvElt,"unitID");
        std::string UnitsClass = openfluid::tools::attributeToString(EvElt,"unitsclass");
        std::string Date = openfluid::tools::attributeToString(EvElt,"date");

        if (ID.empty() || UnitsClass.empty() || Date.empty())
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "wrong or missing attribute(s) in domain calendar event (" + m_CurrentFile + ")");
        }

        openfluid::core::UnitID_t IDNum;
        if (!openfluid::tools::convertString(ID,&IDNum))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "wrong format for unit ID in event (" + m_CurrentFile + ")");
        }

        openfluid::core::DateTime EventDate;
        if (!EventDate.setFromISOString(Date))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "wrong format for date in event (" + m_CurrentFile + ")");
        }

        openfluid::fluidx::EventDescriptor EvDesc;
        EvDesc.setUnitsClass(UnitsClass);
        EvDesc.setUnitID(IDNum);
        EvDesc.setEvent(openfluid::core::Event(EventDate));

        for (auto InfElt = EvElt->FirstChildElement("info"); InfElt != nullptr;
             InfElt = InfElt->NextSiblingElement("info"))
        {
          std::string Key = openfluid::tools::attributeToString(InfElt,"key");
          std::string Value = openfluid::tools::attributeToString(InfElt,"value");

          if (Key.empty())
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "wrong or missing key attribute(s) in domain calendar event info (" + m_CurrentFile + ")");
          }

          EvDesc.event().addInfo(Key,Value);
        }

        EventsList.push_back(EvDesc);
      }

      return EventsList;
    }


    // =====================================================================
    // =====================================================================


    void extractDomain(const tinyxml2::XMLElement* DomainElt, LoadingTempData& TempData)
    {
      for (auto Elt = DomainElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
      {
        std::string TagName(Elt->Name());

        if (TagName == "definition")
        {
          SpatialUnitsList_t List = extractDomainDefinition(Elt);
          TempData.SpatiaUnits.insert(TempData.SpatiaUnits.end(),List.begin(),List.end());
        }
        else if (TagName == "attributes")
        {
          TempData.Attributes.push_back(extractDomainAttributes(Elt));
        }
        else if (TagName == "calendar")
        {
          EventsList_t List = extractDomainCalendar(Elt);
          TempData.Events.insert(TempData.Events.end(),List.begin(),List.end());
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void extractDatastore(const tinyxml2::XMLElement* StoreElt)
    {
      for (auto Elt = StoreElt->FirstChildElement("dataitem"); Elt != nullptr; 
           Elt = Elt->NextSiblingElement("dataitem"))
      {
        std::string DataID = openfluid::tools::attributeToString(Elt,"id");
        std::string DataTypeStr = openfluid::tools::attributeToString(Elt,"type");
        std::string DataSrc = openfluid::tools::attributeToString(Elt,"source");
        std::string DataClass = openfluid::tools::attributeToString(Elt,"unitsclass");

        if (!DataID.empty() && !DataTypeStr.empty() && !DataSrc.empty())
        {
          openfluid::core::UnstructuredValue::UnstructuredType DataType;

          if (!openfluid::core::UnstructuredValue::getValueTypeFromString(DataTypeStr, DataType))
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "unknown or missing datatype in dataitem definition (" + m_CurrentFile + ")");
          }

          openfluid::fluidx::DatastoreItemDescriptor* Item =
            new openfluid::fluidx::DatastoreItemDescriptor(DataID,m_CurrentDir,DataSrc,DataType);

          if (!DataClass.empty())
          {
            Item->setUnitsClass(DataClass);
          }

          if (!m_Descriptor.m_DatastoreDescriptor.appendItem(Item))
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


    // =====================================================================
    // =====================================================================


    void parseFile(const std::string& FilePath, LoadingTempData& TempData)
    {
      tinyxml2::XMLDocument Doc;

      m_CurrentFile = FilePath;

      if (Doc.LoadFile(FilePath.c_str()) == tinyxml2::XML_SUCCESS)
      {
        const auto Root = Doc.RootElement();

        if (Root != nullptr && std::string(Root->Name()) == "openfluid")
        {
          auto FileFormatVersion = openfluid::tools::getOpenFLUIDXMLFormat(Root);

          for (auto Elt = Root->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
          {
            std::string TagName(Elt->Name());

            if (TagName == "run")
            {
              if (m_RunConfigDefined)
              {
                throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                          "Duplicate run configuration (" + FilePath + ")");
              }
              extractRun(Elt);
            }
            else if (TagName == "model")
            {
              if (m_ModelDefined)
              {
                  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                            "Duplicate model definition (" + FilePath + ")");
              }
              extractModel(Elt);
            }
            else if (TagName == "monitoring")
            {
              extractMonitoring(Elt);
            }
            else if (TagName == "domain")
            {
              extractDomain(Elt,TempData);
            }
            else if (TagName == "datastore")
            {
              extractDatastore(Elt);
            }
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "file is not well formed: "+ FilePath);
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "file cannot be loaded: "+ FilePath);
      }
    }


    // =====================================================================
    // =====================================================================


    void rebuildSpatialDomainFromTemp(LoadingTempData& TempData)
    {
      // structure
      for (const auto& SpatialUnit : TempData.SpatiaUnits)
      {
        m_Descriptor.m_DomainDescriptor.addUnit(SpatialUnit,false);
      }

      // attributes
      for (const auto& AttrTable : TempData.Attributes)
      {
        openfluid::core::UnitsClass_t UnitsClass = AttrTable.getUnitsClass();

        for (const auto& AttrName : AttrTable.columnsOrder())
        {
          m_Descriptor.m_DomainDescriptor.addAttribute(UnitsClass,AttrName,std::string(),false);
        }
      }

      for (const auto& AttrTable : TempData.Attributes)
      {
        openfluid::core::UnitsClass_t UnitsClass = AttrTable.getUnitsClass();

        for (const auto& UnitsAttrs : AttrTable.attributes())
        {
          for (const auto& Attr : UnitsAttrs.second)
          {
            m_Descriptor.m_DomainDescriptor.setAttribute(UnitsClass,UnitsAttrs.first,Attr.first,Attr.second);
          }
        }
      }

      // events
      for (const auto& Event : TempData.Events)
      {
        m_Descriptor.m_DomainDescriptor.addEvent(Event);
      }

      m_Descriptor.m_DomainDescriptor.checkUnitsRelations();
      m_Descriptor.m_DomainDescriptor.checkAttributesConsistency();
    }


    // =====================================================================
    // =====================================================================


  public:

    FluidXReaderImplementation(FluidXDescriptor& Desc, openfluid::base::IOListener* Listener):
      m_Descriptor(Desc),mp_Listener(Listener)
    { }


    // =====================================================================
    // =====================================================================


    void run(const std::string& DirPath)
    {
      if (!openfluid::tools::Filesystem::isDirectory(DirPath))
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"directory " + DirPath + " does not exist");
      }

      std::vector<std::string> FluidXFilesToLoad = 
        openfluid::tools::Filesystem::findFilesByExtension(DirPath, "fluidx", true);

      if (FluidXFilesToLoad.size() == 0)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"no fluidx file found in directory "+DirPath);
      }

      std::sort(FluidXFilesToLoad.begin(), FluidXFilesToLoad.end());

      FluidXDescriptor FXDesc;
      m_RunConfigDefined = false;
      m_ModelDefined = false;
      LoadingTempData TempData;

      m_CurrentDir = DirPath;

      for (const auto& CurrentFile : FluidXFilesToLoad)
      {
        try
        {
          mp_Listener->onFileLoad(openfluid::tools::Filesystem::filename(CurrentFile));
          parseFile(CurrentFile,TempData);
          mp_Listener->onFileLoaded(openfluid::base::Listener::Status::OK_STATUS);
        }
        catch (...)
        {
          mp_Listener->onFileLoaded(openfluid::base::Listener::Status::ERROR_STATUS);
          throw;
        }
      }

      rebuildSpatialDomainFromTemp(TempData);
    }
};


// =====================================================================
// =====================================================================


class FluidXWriterImplementation
{
  private:

    const openfluid::fluidx::FluidXDescriptor& m_Descriptor;

    openfluid::base::IOListener* mp_Listener;


    // =====================================================================
    // =====================================================================


    std::string getGeneratorMethodAsStr(
      openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method) const
    {
      switch (Method)
      {
        case openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED:
          return "fixed";
        case openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM:
          return "random";
        case openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP:
          return "interp";
        case openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT:
          return "inject";
          break;
        default:
          break;
      }

      return "";
    }


    // =====================================================================
    // =====================================================================


    void insertWareParams(const openfluid::ware::WareParams_t& Params, tinyxml2::XMLElement* BaseElt) const
    {
      for (const auto& P : Params)
      {
        auto ParamElt = BaseElt->InsertNewChildElement("param");
        ParamElt->SetAttribute("name",P.first.c_str());
        ParamElt->SetAttribute("value",P.second.get().c_str());
      }
    }


    // =====================================================================
    // =====================================================================


    void insertModel(tinyxml2::XMLElement* BaseElt) const
    {
      auto ModelElt = BaseElt->InsertNewChildElement("model");

      const auto& GParams = m_Descriptor.m_ModelDescriptor.getGlobalParameters();
      if (GParams.size() > 0)
      {
        auto GParamsElt = ModelElt->InsertNewChildElement("gparams");
        insertWareParams(GParams,GParamsElt);
      }

      const auto ModelItems = m_Descriptor.m_ModelDescriptor.items();
      for (const auto Item : ModelItems)
      {
        if (Item->isType(openfluid::ware::WareType::SIMULATOR))
        {
          openfluid::fluidx::SimulatorDescriptor* SimDesc =
              dynamic_cast<openfluid::fluidx::SimulatorDescriptor*>(Item);

          auto SimElt = ModelElt->InsertNewChildElement("simulator");
          SimElt->SetAttribute("ID",SimDesc->getID().c_str());
          SimElt->SetAttribute("enabled",SimDesc->isEnabled());
          insertWareParams(SimDesc->getParameters(),SimElt);
        }
        else if (Item->isType(openfluid::ware::WareType::GENERATOR))
        {
          openfluid::fluidx::GeneratorDescriptor* GenDesc =
              dynamic_cast<openfluid::fluidx::GeneratorDescriptor*>(Item);

          auto GenElt = ModelElt->InsertNewChildElement("generator");
          GenElt->SetAttribute("varname",GenDesc->getVariableName().c_str());

          if (GenDesc->getVariableSize() != 1)
          {
            GenElt->SetAttribute("varsize",GenDesc->getVariableSize());
          }

          GenElt->SetAttribute("unitsclass",GenDesc->getUnitsClass().c_str());
          GenElt->SetAttribute("method",getGeneratorMethodAsStr(GenDesc->getGeneratorMethod()).c_str());
          GenElt->SetAttribute("enabled",GenDesc->isEnabled());
          insertWareParams(GenDesc->getParameters(),GenElt);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    template<class T>
    std::string getSeparatedStrFromList(const T& Data,const std::string& Sep) const
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


    void insertDomain(tinyxml2::XMLElement* BaseElt) const
    {
      auto DomainElt = BaseElt->InsertNewChildElement("domain");

      insertDomainDefinition(DomainElt);
      insertDomainAttributes(DomainElt);
      insertDomainCalendar(DomainElt);
    }


    // =====================================================================
    // =====================================================================


    void insertDomainDefinition(tinyxml2::XMLElement* BaseElt) const
    {
      const auto& Units = m_Descriptor.m_DomainDescriptor.spatialUnits();

      auto DefElt = BaseElt->InsertNewChildElement("definition");

      for (const auto& UnitsClass : Units)
      {
        for (const auto& Unit : UnitsClass.second)
        {
          auto UnitElt = DefElt->InsertNewChildElement("unit");

          UnitElt->SetAttribute("class",UnitsClass.first.c_str());
          UnitElt->SetAttribute("ID",Unit.second.getID());
          UnitElt->SetAttribute("pcsorder",Unit.second.getProcessOrder());

          for (const auto& LinkedUnit : Unit.second.toSpatialUnits())
          {
            auto ToElt = UnitElt->InsertNewChildElement("to");
            ToElt->SetAttribute("class",LinkedUnit.first.c_str());
            ToElt->SetAttribute("ID",LinkedUnit.second);
          }

          for (const auto& LinkedUnit : Unit.second.parentSpatialUnits())
          {
            auto ChildElt = UnitElt->InsertNewChildElement("childof");
            ChildElt->SetAttribute("class",LinkedUnit.first.c_str());
            ChildElt->SetAttribute("ID",LinkedUnit.second);
          }
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void insertDomainAttributes(tinyxml2::XMLElement* BaseElt) const
    {
      const auto& DomainDesc = m_Descriptor.m_DomainDescriptor;
      
      for (const auto& UnitsClass : DomainDesc.spatialUnits())
      {
        auto AttrsNames = DomainDesc.getAttributesNames(UnitsClass.first);

        if (!AttrsNames.empty())
        {
          auto AttrsElt = BaseElt->InsertNewChildElement("attributes");
          AttrsElt->SetAttribute("unitsclass",UnitsClass.first.c_str());
          AttrsElt->SetAttribute("colorder",getSeparatedStrFromList(AttrsNames,";").c_str());

          std::ostringstream AttrsData;
          AttrsData << "\n";
          for (const auto& Unit : UnitsClass.second)
          {
            std::vector<std::string> Values;
            const auto& Attributes = Unit.second.attributes();
            transform(Attributes.begin(),Attributes.end(), back_inserter(Values),
                      [](const std::map<openfluid::core::AttributeName_t,std::string>::value_type& Val)
                      {
                        return Val.second;
                      });
            AttrsData << Unit.second.getID() << "\t" << getSeparatedStrFromList(Values,"\t") << "\n";
          }
          
          auto Text = AttrsElt->InsertNewText(AttrsData.str().c_str());
          Text->SetCData(true);  // enable CDATA mode for attributes text
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void insertDomainCalendar(tinyxml2::XMLElement* BaseElt) const
    {
      auto CalElt = BaseElt->InsertNewChildElement("calendar");

      for (const auto& UnitsClass : m_Descriptor.m_DomainDescriptor.spatialUnits())
      {
        for (const auto& Unit : UnitsClass.second)
        {
          const auto& Events = Unit.second.events();

          for (const auto& EvDesc : Events)
          {
            auto EvElt = CalElt->InsertNewChildElement("event");

            EvElt->SetAttribute("unitsclass",UnitsClass.first.c_str());
            EvElt->SetAttribute("unitID",Unit.second.getID());
            EvElt->SetAttribute("date",EvDesc.event().getDateTime().getAsISOString().c_str());

            const auto& Infos = EvDesc.event().getInfos();
            for (const auto& Inf : Infos)
            {
              auto InfElt = EvElt->InsertNewChildElement("info");
              InfElt->SetAttribute("key",Inf.first.c_str());
              InfElt->SetAttribute("value",Inf.second.get().c_str());
            }

          }
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void insertRunConfiguration(tinyxml2::XMLElement* BaseElt) const
    {
      const auto& RunConfig = m_Descriptor.m_RunDescriptor;

      auto RunElt = BaseElt->InsertNewChildElement("run");


      if (RunConfig.isFilled())
      {

      // scheduling
        std::string ConstraintStr = "none";
        if (RunConfig.getSchedulingConstraint() == 
            openfluid::base::SimulationStatus::SCHED_DTFORCED)
        {
          ConstraintStr = "dt-forced";
        }
        else if (m_Descriptor.m_RunDescriptor.getSchedulingConstraint() == 
                 openfluid::base::SimulationStatus::SCHED_DTCHECKED)
        {
          ConstraintStr = "dt-checked";
        }

        auto SchedElt = RunElt->InsertNewChildElement("scheduling");
        SchedElt->SetAttribute("deltat",RunConfig.getDeltaT());
        SchedElt->SetAttribute("constraint",ConstraintStr.c_str());

        auto PeriodElt = RunElt->InsertNewChildElement("period");
        PeriodElt->SetAttribute("begin",RunConfig.getBeginDate().getAsISOString().c_str());
        PeriodElt->SetAttribute("end",RunConfig.getEndDate().getAsISOString().c_str());

        if (RunConfig.isUserValuesBufferSize())
        {
          auto BuffElt = RunElt->InsertNewChildElement("valuesbuffer");
          BuffElt->SetAttribute("size",RunConfig.getValuesBufferSize());
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void insertDatastore(tinyxml2::XMLElement* BaseElt) const
    {
      const auto& StoreItems = m_Descriptor.m_DatastoreDescriptor.items();

      auto StoreElt = BaseElt->InsertNewChildElement("datastore");

      for (const auto& Item: StoreItems)
      {
        auto ItemElt = StoreElt->InsertNewChildElement("dataitem");

        ItemElt->SetAttribute("id",Item->getID().c_str());
        ItemElt->SetAttribute("type",
                              openfluid::core::UnstructuredValue::getStringFromValueType(Item->getType()).c_str());
        ItemElt->SetAttribute("source",Item->getRelativePath().c_str());

        auto UClass = Item->getUnitsClass();

        if (!UClass.empty())
        {
          ItemElt->SetAttribute("unitsclass",UClass.c_str());
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void insertMonitoring(tinyxml2::XMLElement* BaseElt) const
    {
      const auto& MonItems = m_Descriptor.m_MonitoringDescriptor.items();

      auto MonElt = BaseElt->InsertNewChildElement("monitoring");

      for (const auto Item : MonItems)
      {
        auto ObsElt = MonElt->InsertNewChildElement("observer");
        ObsElt->SetAttribute("ID",Item->getID().c_str());
        ObsElt->SetAttribute("enabled",Item->isEnabled());
        insertWareParams(Item->getParameters(),ObsElt);
      }
    }


    // =====================================================================
    // =====================================================================


    void prepareFluidXDir(const std::string& DirPath) const
    {

      if (!openfluid::tools::Filesystem::isDirectory(DirPath))
      {
        openfluid::tools::Filesystem::makeDirectory(DirPath);

        if (!openfluid::tools::Filesystem::isDirectory(DirPath))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error creating output directory: "+
                                                                             DirPath);
        }
      }
    }


    // =====================================================================
    // =====================================================================


  public:


    FluidXWriterImplementation(const FluidXDescriptor& Desc, openfluid::base::IOListener* Listener):
      m_Descriptor(Desc), mp_Listener(Listener)
    { }


    void runManyFiles(const std::string& DirPath) const
    {
      mp_Listener->onWrite();

      prepareFluidXDir(DirPath);

      // TODO use std::function instead
      using insertMethod = void (FluidXWriterImplementation::*)(tinyxml2::XMLElement*) const;

      // list of fluidx files to write with associated method to call (using pointer to function)
      std::vector<std::pair<std::string,insertMethod>> FilesFunctions =
      { 
        { std::string("model.fluidx"), &FluidXWriterImplementation::insertModel },
        { std::string("domain.fluidx"), &FluidXWriterImplementation::insertDomain },
        { std::string("monitoring.fluidx"), &FluidXWriterImplementation::insertMonitoring },
        { std::string("datastore.fluidx"), &FluidXWriterImplementation::insertDatastore },
        { std::string("run.fluidx"), &FluidXWriterImplementation::insertRunConfiguration }
      };

      for (auto FileFunc : FilesFunctions)
      {
        auto FilePath = openfluid::tools::Filesystem::joinPath({DirPath,FileFunc.first});
        
        mp_Listener->onFileWrite(FilePath);

        tinyxml2::XMLDocument Doc;
        auto OFElt = openfluid::tools::prepareOpenFLUIDXMLDoc(Doc,FluidXIO::FormatVersion);
        
        // calls the associated method
        (this->*FileFunc.second)(OFElt);
        
        Doc.SaveFile(FilePath.c_str());
        
        mp_Listener->onFileWritten(openfluid::base::Listener::Status::OK_STATUS);
      }

      mp_Listener->onWritten(openfluid::base::Listener::Status::OK_STATUS);
    }


    // =====================================================================
    // =====================================================================


    void runSingleFile(const std::string& FilePath) const
    {
      mp_Listener->onWrite();

      prepareFluidXDir(openfluid::tools::Filesystem::dirname(FilePath));

      mp_Listener->onFileWrite(FilePath);

      tinyxml2::XMLDocument Doc;
      auto OFElt = openfluid::tools::prepareOpenFLUIDXMLDoc(Doc,FluidXIO::FormatVersion);

      insertModel(OFElt);
      insertDomain(OFElt);
      insertDatastore(OFElt);
      insertMonitoring(OFElt);
      insertRunConfiguration(OFElt);

      Doc.SaveFile(FilePath.c_str());

      mp_Listener->onFileWritten(openfluid::base::Listener::Status::OK_STATUS);

      mp_Listener->onWritten(openfluid::base::Listener::Status::OK_STATUS);
    }

};


// =====================================================================
// =====================================================================


FluidXIO::FluidXIO(openfluid::base::IOListener* Listener) : mp_Listener(Listener)
{
  if (!mp_Listener)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Listener can not be NULL");
  }
}


// =====================================================================
// =====================================================================


FluidXDescriptor FluidXIO::loadFromDirectory(const std::string& DirPath) const
{
  FluidXDescriptor FXDesc;

  FluidXReaderImplementation FXReader(FXDesc,mp_Listener);
  FXReader.run(DirPath);

  return FXDesc;
}


// =====================================================================
// =====================================================================


void FluidXIO::writeToSingleFile(const FluidXDescriptor& Desc, const std::string& FilePath) const
{
  FluidXWriterImplementation FXWriter(Desc,mp_Listener);

  FXWriter.runSingleFile(FilePath);
}


// =====================================================================
// =====================================================================


void FluidXIO::writeToManyFiles(const FluidXDescriptor& Desc, const std::string& DirPath) const
{
  FluidXWriterImplementation FXWriter(Desc,mp_Listener);

  FXWriter.runManyFiles(DirPath);
}
 

} } // namespaces
