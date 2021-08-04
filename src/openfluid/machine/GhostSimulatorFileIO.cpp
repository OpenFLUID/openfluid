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
  @file GhostSimulatorFileIO.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <fstream>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <openfluid/machine/GhostSimulatorFileIO.hpp>
#include <openfluid/tools/TinyXML2Helpers.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace machine {


class DataNodeFields
{
  public:

    std::string Name;

    std::string UnitsClass;

    std::string IOMode;

    std::string SIUnit;

    std::string Description;

    std::string Type;

    DataNodeFields(const tinyxml2::XMLElement* const Elt, const std::vector<std::string>& ExpectedIOModes = {})
    {     
      if (Elt != nullptr)
      {      
        Name = openfluid::tools::getXMLAttribute(Elt,"name");
        UnitsClass = openfluid::tools::getXMLAttribute(Elt,"unitsclass");
        IOMode = openfluid::tools::getXMLAttribute(Elt,"iomode");
        SIUnit = openfluid::tools::getXMLAttribute(Elt,"siunit");
        Type = openfluid::tools::getXMLAttribute(Elt,"type");
        Description = openfluid::tools::getXMLText(Elt);
      }

      if (std::find(ExpectedIOModes.begin(),ExpectedIOModes.end(),IOMode) == ExpectedIOModes.end())
      {
        IOMode.clear();
      }
    }
};


// =====================================================================
// =====================================================================


void insertParameter(const openfluid::ware::SignatureDataItem& Data,
                     const std::string& IoModeStr, tinyxml2::XMLElement* BaseElt)
{
  auto ParamElt = BaseElt->InsertNewChildElement("parameter");
  ParamElt->SetAttribute("name",Data.DataName.c_str());
  ParamElt->SetAttribute("iomode",IoModeStr.c_str());
  ParamElt->SetAttribute("siunit",Data.DataUnit.c_str());
  ParamElt->SetText(Data.Description.c_str());
}


// =====================================================================
// =====================================================================


void insertExtraFile(const std::string& Name,
                     const std::string& IoModeStr, tinyxml2::XMLElement* BaseElt)
{
  auto ParamElt = BaseElt->InsertNewChildElement("extrafile");
  ParamElt->SetAttribute("name",Name.c_str());
  ParamElt->SetAttribute("iomode",IoModeStr.c_str());
}


// =====================================================================
// =====================================================================


void insertVariable(const openfluid::ware::SignatureTypedSpatialDataItem& Data,
                    const std::string& IoModeStr, tinyxml2::XMLElement* BaseElt)
{
  auto VarElt = BaseElt->InsertNewChildElement("variable");
  VarElt->SetAttribute("name",Data.DataName.c_str());
  VarElt->SetAttribute("iomode",IoModeStr.c_str());
  VarElt->SetAttribute("unitsclass",Data.UnitsClass.c_str());
  if (Data.DataType != openfluid::core::Value::NONE)
  {
    VarElt->SetAttribute("type",openfluid::core::Value::getStringFromValueType(Data.DataType).c_str());
  }
  VarElt->SetAttribute("siunit",Data.DataUnit.c_str());
  VarElt->SetText(Data.Description.c_str());
}


// =====================================================================
// =====================================================================


void insertAttribute(const openfluid::ware::SignatureSpatialDataItem& Data,
                     const std::string& IoModeStr, tinyxml2::XMLElement* BaseElt)
{
  auto AttrElt = BaseElt->InsertNewChildElement("attribute");
  AttrElt->SetAttribute("name",Data.DataName.c_str());
  AttrElt->SetAttribute("iomode",IoModeStr.c_str());
  AttrElt->SetAttribute("unitsclass",Data.UnitsClass.c_str());
  AttrElt->SetAttribute("siunit",Data.DataUnit.c_str());
  AttrElt->SetText(Data.Description.c_str());
}


// =====================================================================
// =====================================================================


void insertEvents(const std::string& EventsClass, tinyxml2::XMLElement* BaseElt)
{
  auto EvElt = BaseElt->InsertNewChildElement("events");
  EvElt->SetAttribute("unitsclass",EventsClass.c_str());
}


// =====================================================================
// =====================================================================


bool GhostSimulatorFileIO::saveToFile(const openfluid::ware::SimulatorSignature& Signature, const std::string& DirPath)
{
  std::string FilePath = openfluid::tools::Filesystem::joinPath({DirPath,Signature.ID+
                                                                            openfluid::config::SIMULATORS_GHOSTS_SUFFIX+
                                                                            openfluid::config::GHOSTS_EXT});

  tinyxml2::XMLDocument Doc;
  auto OFElt = openfluid::tools::prepareOpenFLUIDXMLDoc(Doc,FormatVersion);

  // ghost-simulator
  auto GhostElt = OFElt->InsertNewChildElement("ghost-simulator");
  GhostElt->SetAttribute("ID",Signature.ID.c_str());
  
  
  // infos
  auto InfElt = GhostElt->InsertNewChildElement("infos");
  InfElt->InsertNewChildElement("name")->SetText(Signature.Name.c_str());
  InfElt->InsertNewChildElement("description")->SetText(Signature.Description.c_str());
  
  for (auto& Author: Signature.Authors)
  {
    auto AuthElt = InfElt->InsertNewChildElement("author");
    AuthElt->SetAttribute("name",Author.first.c_str());
    AuthElt->SetAttribute("email",Author.second.c_str());
  }

  std::string StatusStr = "experimental";
  if (Signature.Status == openfluid::ware::BETA)
  {
    StatusStr = "beta";
  }
  else if (Signature.Status == openfluid::ware::STABLE)
  {
    StatusStr = "stable";
  }
  InfElt->InsertNewChildElement("status")->SetText(StatusStr.c_str());
  
  InfElt->InsertNewChildElement("version")->SetText(Signature.Version.c_str());
  InfElt->InsertNewChildElement("domain")->SetText(Signature.Domain.c_str());
  InfElt->InsertNewChildElement("process")->SetText(Signature.Process.c_str());
  InfElt->InsertNewChildElement("method")->SetText(Signature.Method.c_str());


  // data
  auto DataElt = GhostElt->InsertNewChildElement("data");


  // parameters
  for (auto& Param: Signature.HandledData.RequiredParams)
  {
    insertParameter(Param,"required",DataElt);
  }

  for (auto& Param: Signature.HandledData.UsedParams)
  {
    insertParameter(Param,"used",DataElt);
  }


  // extrafiles
  for (auto& ExtraFile: Signature.HandledData.RequiredExtraFiles)
  {
    insertExtraFile(ExtraFile,"required",DataElt);
  }

  for (auto& ExtraFile: Signature.HandledData.UsedExtraFiles)
  {
    insertExtraFile(ExtraFile,"used",DataElt);
  }


  // variables
  for (auto& Var: Signature.HandledData.RequiredVars)
  {
    insertVariable(Var,"required",DataElt);
  }

  for (auto& Var: Signature.HandledData.UsedVars)
  {
    insertVariable(Var,"used",DataElt);
  }

  for (auto& Var: Signature.HandledData.ProducedVars)
  {
    insertVariable(Var,"produced",DataElt);
  }

  for (auto& Var: Signature.HandledData.UpdatedVars)
  {
    insertVariable(Var,"updated",DataElt);
  }


  // attributes
  for (auto& Var: Signature.HandledData.RequiredAttribute)
  {
    insertAttribute(Var,"required",DataElt);
  }

  for (auto& Var: Signature.HandledData.UsedAttribute)
  {
    insertAttribute(Var,"used",DataElt);
  }

  for (auto& Var: Signature.HandledData.ProducedAttribute)
  {
    insertAttribute(Var,"produced",DataElt);
  }


  // events
  for (auto& EventsClass: Signature.HandledData.UsedEventsOnUnits)
  {
    insertEvents(EventsClass,DataElt);
  }


  // spatial graph
  auto GraphElt = GhostElt->InsertNewChildElement("spatialgraph");

  GraphElt->InsertNewChildElement("description")->SetText(Signature.HandledUnitsGraph.UpdatedUnitsGraph.c_str());

  for (auto& UClass: Signature.HandledUnitsGraph.UpdatedUnitsClass)
  {
    auto GraphClassElt = GraphElt->InsertNewChildElement("unitsclass");
    GraphClassElt->SetAttribute("name",UClass.UnitsClass.c_str());
    GraphClassElt->SetText(UClass.Description.c_str());
  }


  // scheduling
  auto SchedElt = GhostElt->InsertNewChildElement("scheduling");

  if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::UNDEFINED)
  {
    SchedElt->SetAttribute("mode","undefined");
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT)
  {
    SchedElt->SetAttribute("mode","default");
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED)
  {
    SchedElt->SetAttribute("mode","fixed");
    SchedElt->SetAttribute("value",Signature.TimeScheduling.Min);
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
  {
    SchedElt->SetAttribute("mode","range");
    SchedElt->SetAttribute("min",Signature.TimeScheduling.Min);
    SchedElt->SetAttribute("max",Signature.TimeScheduling.Max);
  }


  Doc.SaveFile(FilePath.c_str());

  return true;
}


// =====================================================================
// =====================================================================


bool GhostSimulatorFileIO::loadFromFile(const std::string& FilePath, openfluid::ware::SimulatorSignature& Signature)
{
  Signature.clear();

  tinyxml2::XMLDocument Doc;

  if (Doc.LoadFile(FilePath.c_str()) == tinyxml2::XML_SUCCESS)
  {
    const auto Root = Doc.RootElement();

    if (Root != nullptr && std::string(Root->Name()) == "openfluid")
    {
      const auto GhostElt = Root->FirstChildElement("ghost-simulator");
      if (GhostElt != nullptr && GhostElt->Attribute("ID") != nullptr)
      {
        std::string GhostID = openfluid::tools::getXMLAttribute(GhostElt,"ID");

        if (!boost::starts_with(openfluid::tools::Filesystem::filename(FilePath),GhostID))
        {
          return false;
        }
        
        Signature.ID = GhostID;

        // infos
        const auto InfosElt = GhostElt->FirstChildElement("infos");
        if (InfosElt != nullptr)
        {
          for (auto Elt = InfosElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
          {
            std::string TagName(Elt->Name());
            std::string TagText = openfluid::tools::getXMLText(Elt);

            if (TagName == "name")
            {
              Signature.Name = TagText;
            }
            else if (TagName == "description")
            {
              Signature.Description = TagText;
            }
            else if (TagName == "author")
            {
              Signature.Authors.push_back(
                std::pair<std::string,std::string>(openfluid::tools::getXMLAttribute(Elt,"name"),
                                                   openfluid::tools::getXMLAttribute(Elt,"email"))
              );
            }
            else if (TagName == "status")
            {
              std::string StatusStr = openfluid::tools::toLowerCase(TagText);

              if (StatusStr == "experimental")
              {
                Signature.Status = openfluid::ware::EXPERIMENTAL;
              }
              else if (StatusStr == "beta")
              {
                Signature.Status = openfluid::ware::BETA;
              }
              else if (StatusStr == "stable")
              {
                Signature.Status = openfluid::ware::STABLE;
              }
              else
              {
                return false;
              }
            }
            else if (TagName == "domain")
            {
              Signature.Domain = TagText;
            }
            else if (TagName == "process")
            {
              Signature.Process = TagText;
            }
            else if (TagName == "method")
            {
              Signature.Method = TagText;
            }
            else if (TagName == "version")
            {
              Signature.Version = TagText;
            }
          }
        }

        // data
        const auto DataElt = GhostElt->FirstChildElement("data");
        if (DataElt != nullptr)
        {
          for (auto Elt = DataElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
          {
            std::string TagName(Elt->Name());

            if (TagName == "parameter")
            {
              DataNodeFields Data(Elt,{"required","used"});

              if (Data.IOMode.empty() || Data.Name.empty())
              {
                return false;
              }

              openfluid::ware::SignatureDataItem Param;
              Param.DataName = Data.Name;
              Param.DataUnit = Data.SIUnit;
              Param.Description = Data.Description;

              if (Data.IOMode == "required")
              {
                Signature.HandledData.RequiredParams.push_back(Param);
              }
              else if (Data.IOMode == "used")
              {
                Signature.HandledData.UsedParams.push_back(Param);
              }
            }
            else if (TagName == "extrafile")
            {
              DataNodeFields Data(Elt,{"required","used"});

              if (Data.IOMode.empty() || Data.Name.empty())
              {
                return false;
              }

              if (Data.IOMode == "required")
              {
                Signature.HandledData.RequiredExtraFiles.push_back(Data.Name);
              }
              else if (Data.IOMode == "used")
              {
                Signature.HandledData.UsedExtraFiles.push_back(Data.Name);
              }
            }
            else if (TagName == "events")
            {
              DataNodeFields Data(Elt);

              if (Data.UnitsClass.empty())
              {
                return false;
              }

              Signature.HandledData.UsedEventsOnUnits.push_back(Data.UnitsClass);
            }
            else if (TagName == "variable")
            {
              DataNodeFields Data(Elt,{"required","used","produced","updated"});

              if (Data.IOMode.empty() || Data.Name.empty() || Data.UnitsClass.empty())
              {
                return false;
              }

              openfluid::ware::SignatureTypedSpatialDataItem Variable;
              Variable.DataName = Data.Name;
              Variable.UnitsClass = Data.UnitsClass;
              Variable.Description = Data.Description;
              Variable.DataUnit = Data.SIUnit;
              if (!Data.Type.empty())
              {
                if (!openfluid::core::Value::getValueTypeFromString(Data.Type,Variable.DataType))
                {
                  return false;
                }
              }

              if (Data.IOMode == "required")
              {
                Signature.HandledData.RequiredVars.push_back(Variable);
              }
              else if (Data.IOMode == "used")
              {
                Signature.HandledData.UsedVars.push_back(Variable);
              }
              else if (Data.IOMode == "produced")
              {
                Signature.HandledData.ProducedVars.push_back(Variable);
              }
              else if (Data.IOMode == "updated")
              {
                Signature.HandledData.UpdatedVars.push_back(Variable);
              }
            }
            else if (TagName == "attribute")
            {
              DataNodeFields Data(Elt,{"required","used","produced"});

              if (Data.IOMode.empty() || Data.Name.empty() || Data.UnitsClass.empty())
              {
                return false;
              }

              openfluid::ware::SignatureSpatialDataItem Attribute;
              Attribute.DataName = Data.Name;
              Attribute.UnitsClass = Data.UnitsClass;
              Attribute.Description = Data.Description;
              Attribute.DataUnit = Data.SIUnit;

              if (Data.IOMode == "required")
              {
                Signature.HandledData.RequiredAttribute.push_back(Attribute);
              }
              else if (Data.IOMode == "used")
              {
                Signature.HandledData.UsedAttribute.push_back(Attribute);
              }
              else if (Data.IOMode == "produced")
              {
                Signature.HandledData.ProducedAttribute.push_back(Attribute);
              }
            }
          }
        }

        // spatial graph
        const auto SpatialElt = GhostElt->FirstChildElement("spatialgraph");
        if (SpatialElt != nullptr)
        {
          for (auto Elt = SpatialElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
          {
            std::string TagName(Elt->Name());
            std::string TagText = openfluid::tools::getXMLText(Elt);

            if (TagName == "description")
            {
              Signature.HandledUnitsGraph.UpdatedUnitsGraph = TagText;
            }
            else if (TagName == "unitsclass")
            {
              std::string NameStr = openfluid::tools::getXMLAttribute(Elt,"name");
              if (NameStr.empty())
              {
                return false;
              }

              openfluid::ware::SignatureUnitsClassItem UnitsClassItem;
              UnitsClassItem.UnitsClass = NameStr;
              UnitsClassItem.Description = TagText;
              Signature.HandledUnitsGraph.UpdatedUnitsClass.push_back(UnitsClassItem);
            }
          }
        }

        // scheduling
        const auto SchedElt = GhostElt->FirstChildElement("scheduling");
        if (SchedElt != nullptr)
        {
          std::string ModeStr = openfluid::tools::getXMLAttribute(SchedElt,"mode");

          if (ModeStr == "undefined")
          {
            Signature.TimeScheduling.setAsUndefined();
          }
          else if (ModeStr == "default")
          {
            Signature.TimeScheduling.setAsDefaultDeltaT();
          }
          else if (ModeStr == "fixed")
          {
            int Val = 0;

            if (SchedElt->QueryIntAttribute("value",&Val) == tinyxml2::XML_SUCCESS)
            {
              Signature.TimeScheduling.setAsFixed(Val);
            }
            else
            {
              return false;
            }

            Signature.TimeScheduling.setAsFixed(Val);
          }
          else if (ModeStr == "range")
          {
            int Min = 0;
            int Max = 0;

            if (SchedElt->QueryIntAttribute("min",&Min) == tinyxml2::XML_SUCCESS &&
                SchedElt->QueryIntAttribute("max",&Max) == tinyxml2::XML_SUCCESS)
            {
              Signature.TimeScheduling.setAsRange(Min,Max);
            }
            else
            {
              return false;
            }
          }
          else
          {
            return false;
          }
        }
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }

  return true;
}


} }  // namespaces

