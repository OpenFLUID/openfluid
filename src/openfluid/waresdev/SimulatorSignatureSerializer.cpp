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
  @file SimulatorSignatureSerializer.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace waresdev {


openfluid::ware::SignatureSpatialDataItem readSpatialDataItemFromJSON(const openfluid::thirdparty::json& Item)
{
  openfluid::ware::SignatureSpatialDataItem Data;

  Data.Name = Item.value("name","");
  if (!openfluid::tools::isValidVariableName(Data.Name))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Missing or invalid data name");
  }

  Data.UnitsClass = Item.value("unitsclass","");
  if (Data.UnitsClass.empty())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Missing or invalid units class for data");
  }

  Data.Description = Item.value("description","");
  Data.SIUnit = Item.value("siunit","");
  
  openfluid::core::Value::Type VT;

  if (openfluid::core::Value::getValueTypeFromString(Item.value("type",""),VT))
  {
    Data.DataType = VT;
  }

  return Data;
}


// =====================================================================
// =====================================================================


std::vector<openfluid::ware::SignatureSpatialDataItem> 
readSpatialDataListFromJSON(const openfluid::thirdparty::json& Json)
{
  std::vector<openfluid::ware::SignatureSpatialDataItem> List;

  if (Json.is_array())
  {
    for (const auto& I : Json)
    {
      List.push_back(readSpatialDataItemFromJSON(I));
    }
  }

  return List;
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeAttributesFromJSON(const openfluid::thirdparty::json& Json, 
                                                                 openfluid::ware::SimulatorSignature& Sign) const
{
  if (Json.contains("produced"))
  {
    Sign.SimulatorHandledData.ProducedAttribute = readSpatialDataListFromJSON(Json.at("produced"));
  }

  if (Json.contains("used"))
  {
    Sign.SimulatorHandledData.UsedAttribute = readSpatialDataListFromJSON(Json.at("used"));
  }

  if (Json.contains("required"))
  {
    Sign.SimulatorHandledData.RequiredAttribute = readSpatialDataListFromJSON(Json.at("required"));
  }
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeVariablesFromJSON(const openfluid::thirdparty::json& Json, 
                                                                openfluid::ware::SimulatorSignature& Sign) const
{
  if (Json.contains("produced"))
  {
    Sign.SimulatorHandledData.ProducedVars = readSpatialDataListFromJSON(Json.at("produced"));
  }

  if (Json.contains("used"))
  {
    Sign.SimulatorHandledData.UsedVars = readSpatialDataListFromJSON(Json.at("used"));
  }

  if (Json.contains("required"))
  {
    Sign.SimulatorHandledData.RequiredVars = readSpatialDataListFromJSON(Json.at("required"));
  }

  if (Json.contains("updated"))
  {
    Sign.SimulatorHandledData.UpdatedVars = readSpatialDataListFromJSON(Json.at("updated"));
  }
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeEventsFromJSON(const openfluid::thirdparty::json& Json, 
                                                             openfluid::ware::SimulatorSignature& Sign) const
{
  Sign.SimulatorHandledData.UsedEventsOnUnits = Json.get<std::vector<std::string>>();
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeExtrafilesFromJSON(const openfluid::thirdparty::json& Json, 
                                                                 openfluid::ware::SimulatorSignature& Sign) const
{
  if (Json.contains("used"))
  {
    Sign.SimulatorHandledData.UsedExtraFiles = Json.at("used").get<std::vector<std::string>>();
  }

  if (Json.contains("required"))
  {
    Sign.SimulatorHandledData.RequiredExtraFiles = Json.at("required").get<std::vector<std::string>>();
  }

}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeDataFromJSON(const openfluid::thirdparty::json& Json, 
                                                           openfluid::ware::SimulatorSignature& Sign) const
{
  if (Json.contains("parameters"))
  {
   DataJsonConverter::unserializeParametersFromJSON(Json.at("parameters"),Sign.HandledData);
  }

  if (Json.contains("attributes"))
  {
    unserializeAttributesFromJSON(Json.at("attributes"),Sign);
  }

  if (Json.contains("variables"))
  {
    unserializeVariablesFromJSON(Json.at("variables"),Sign);
  }

  if (Json.contains("events"))
  {
    unserializeEventsFromJSON(Json.at("events"),Sign);
  }

  if (Json.contains("extrafiles"))
  {
    unserializeExtrafilesFromJSON(Json.at("extrafiles"),Sign);
  }
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeSpatialGraphFromJSON(const openfluid::thirdparty::json& Json, 
                                                                   openfluid::ware::SimulatorSignature& Sign) const
{
  Sign.HandledUnitsGraph.UpdatedUnitsGraph = Json.value("description","");

  if (Json.contains("details"))
  {
    const auto JsonDetails = Json.at("details");

    if (JsonDetails.is_array())
    {
      for (const auto& D : JsonDetails)
      {
        const auto Desc = D.value("description","");
        const auto UC = D.value("unitsclass","");

        if (!UC.empty())
        {
          Sign.HandledUnitsGraph.UpdatedUnitsClass.push_back({UC,Desc});
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeSchedulingFromJSON(const openfluid::thirdparty::json& Json, 
                                                                 openfluid::ware::SimulatorSignature& Sign) const
{
  Sign.TimeScheduling.setTypeFromString(Json.value("type",""));
  Sign.TimeScheduling.Min = Json.value("min",0);
  Sign.TimeScheduling.Max = Json.value("max",0);
}


// =====================================================================
// =====================================================================


openfluid::ware::SimulatorSignature
SimulatorSignatureSerializer::fromJSON(const openfluid::thirdparty::json& Json) const
{
  openfluid::ware::SimulatorSignature Sign = fromJSONBase(Json);

  if (!Json.contains("simulator"))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Missing simulator entry");
  }

  const auto JsonSim = Json.at("simulator");

  if (JsonSim.contains("data"))
  {
    unserializeDataFromJSON(JsonSim.at("data"),Sign);
  }

  if (JsonSim.contains("spatial_graph"))
  {
    unserializeSpatialGraphFromJSON(JsonSim.at("spatial_graph"),Sign);
  }

  if (JsonSim.contains("scheduling"))
  {
    unserializeSchedulingFromJSON(JsonSim.at("scheduling"),Sign);
  }

  return Sign;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json serializeSpatialDataItemToJSON(const openfluid::ware::SignatureSpatialDataItem& Item)
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["name"] = Item.Name;
  Json["unitsclass"] = Item.UnitsClass;
  Json["description"] = Item.Description;
  Json["siunit"] = Item.SIUnit;
  Json["type"] = openfluid::core::Value::getStringFromValueType(Item.DataType);

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeAttributesToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  auto JsonReq = openfluid::thirdparty::json::array();
  for (const auto& A : Sign.SimulatorHandledData.RequiredAttribute)
  {
    JsonReq.push_back(serializeSpatialDataItemToJSON(A));
  }
  Json["required"] = JsonReq;

  auto JsonUs = openfluid::thirdparty::json::array();
  for (const auto& A : Sign.SimulatorHandledData.UsedAttribute)
  {
    JsonUs.push_back(serializeSpatialDataItemToJSON(A));
  }
  Json["used"] = JsonUs;

  auto JsonProd = openfluid::thirdparty::json::array();
  for (const auto& A : Sign.SimulatorHandledData.ProducedAttribute)
  {
    JsonProd.push_back(serializeSpatialDataItemToJSON(A));
  }
  Json["produced"] = JsonProd;

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeVariablesToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  auto JsonProd = openfluid::thirdparty::json::array();
  for (const auto& V : Sign.SimulatorHandledData.ProducedVars)
  {
    JsonProd.push_back(serializeSpatialDataItemToJSON(V));
  }
  Json["produced"] = JsonProd;

  auto JsonReq = openfluid::thirdparty::json::array();
  for (const auto& V : Sign.SimulatorHandledData.RequiredVars)
  {
    JsonReq.push_back(serializeSpatialDataItemToJSON(V));
  }
  Json["required"] = JsonReq;

  auto JsonUs = openfluid::thirdparty::json::array();
  for (const auto& V : Sign.SimulatorHandledData.UsedVars)
  {
    JsonUs.push_back(serializeSpatialDataItemToJSON(V));
  }
  Json["used"] = JsonUs;

  auto JsonUp = openfluid::thirdparty::json::array();
  for (const auto& V : Sign.SimulatorHandledData.UpdatedVars)
  {
    JsonUp.push_back(serializeSpatialDataItemToJSON(V));
  }
  Json["updated"] = JsonUp;

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeEventsToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = Sign.SimulatorHandledData.UsedEventsOnUnits;

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeExtrafilesToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["required"] = Sign.SimulatorHandledData.RequiredExtraFiles;
  Json["used"] = Sign.SimulatorHandledData.UsedExtraFiles;

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeDataToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = DataJsonConverter::serializeDataToJSON(Sign.HandledData);
  Json["attributes"] = serializeAttributesToJSON(Sign);
  Json["variables"] = serializeVariablesToJSON(Sign);
  Json["events"] = serializeEventsToJSON(Sign);
  Json["extrafiles"] = serializeExtrafilesToJSON(Sign);

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeSpatialGraphToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["description"] = Sign.HandledUnitsGraph.UpdatedUnitsGraph;

  auto ClassesArr = openfluid::thirdparty::json::array();
  for (const auto& UC : Sign.HandledUnitsGraph.UpdatedUnitsClass)
  {
    auto UCObj = openfluid::thirdparty::json::object();
    UCObj["unitsclass"] = UC.UnitsClass;
    UCObj["description"] = UC.Description;
    ClassesArr.push_back(UCObj);
  }
  Json["details"] = ClassesArr;

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeSchedulingToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["type"] = Sign.TimeScheduling.getTypeAsString();
  Json["min"] = Sign.TimeScheduling.Min;
  Json["max"] = Sign.TimeScheduling.Max;

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json SimulatorSignatureSerializer::toJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = toJSONBase(Sign);

  Json["simulator"] = openfluid::thirdparty::json::object();

  Json["simulator"]["data"] = serializeDataToJSON(Sign);

  Json["simulator"]["spatial_graph"] = serializeSpatialGraphToJSON(Sign);

  Json["simulator"]["scheduling"] = serializeSchedulingToJSON(Sign);

  return Json;
}


// =====================================================================
// =====================================================================


std::string SimulatorSignatureSerializer::getCPPSpatialDataString(
  const std::string Member,const std::vector<openfluid::ware::SignatureSpatialDataItem>& Data)
{
  std::string Str;
  
  for (const auto& D : Data)
  {
    Str += CppWriter::getCPPMethod(Member,"push_back",{"{"+CppWriter::getQuotedString(D.Name)+","+
                                                CppWriter::getQuotedString(D.UnitsClass)+","+
                                                CppWriter::getQuotedString(D.Description)+","+
                                                CppWriter::getQuotedString(D.SIUnit)+","+
                                                CppWriter::getCPPValueType(D.DataType)+"}"});
  }

  return Str;
}


// =====================================================================
// =====================================================================


std::string SimulatorSignatureSerializer::toWareCPP(const openfluid::ware::SimulatorSignature& Sign) const
{
  std::string CPP;
  
  CPP += CppWriter::getCPPHead("openfluid/ware/SimulatorSignature.hpp","openfluid::ware::SimulatorSignature");
  CPP += toWareCPPBase(Sign);
  
  CPP += "\n";
  
  CPP += CppWriter::toWareCPPParams(Sign.HandledData);

  // Extrafiles
  CPP += CppWriter::getCPPAssignment("SimulatorHandledData.UsedExtraFiles",
                          CppWriter::getCPPVectorString(Sign.SimulatorHandledData.UsedExtraFiles,true));
  CPP += CppWriter::getCPPAssignment("SimulatorHandledData.RequiredExtraFiles",
                          CppWriter::getCPPVectorString(Sign.SimulatorHandledData.RequiredExtraFiles,true));

  // Attributes
  CPP += getCPPSpatialDataString("SimulatorHandledData.UsedAttribute",Sign.SimulatorHandledData.UsedAttribute);
  CPP += getCPPSpatialDataString("SimulatorHandledData.RequiredAttribute",Sign.SimulatorHandledData.RequiredAttribute);
  CPP += getCPPSpatialDataString("SimulatorHandledData.ProducedAttribute",Sign.SimulatorHandledData.ProducedAttribute);

  // Variables
  CPP += getCPPSpatialDataString("SimulatorHandledData.UsedVars",Sign.SimulatorHandledData.UsedVars);
  CPP += getCPPSpatialDataString("SimulatorHandledData.RequiredVars",Sign.SimulatorHandledData.RequiredVars);
  CPP += getCPPSpatialDataString("SimulatorHandledData.UpdatedVars",Sign.SimulatorHandledData.UpdatedVars);
  CPP += getCPPSpatialDataString("SimulatorHandledData.ProducedVars",Sign.SimulatorHandledData.ProducedVars);

  // Events
  CPP += CppWriter::getCPPAssignment("SimulatorHandledData.UsedEventsOnUnits",CppWriter::getCPPVectorString(Sign.SimulatorHandledData.UsedEventsOnUnits,true));

  // Spatial struct
  CPP += CppWriter::getCPPAssignment("HandledUnitsGraph.UpdatedUnitsGraph",
                          CppWriter::getQuotedString(openfluid::tools::escapeString(Sign.HandledUnitsGraph.UpdatedUnitsGraph)));
  std::vector<std::string> SpatialUpdateVect;
  for (const auto& U : Sign.HandledUnitsGraph.UpdatedUnitsClass)
  {
    SpatialUpdateVect.push_back(CppWriter::getCPPVectorString({U.UnitsClass,U.Description},true));
  }
  CPP += CppWriter::getCPPAssignment("HandledUnitsGraph.UpdatedUnitsClass",CppWriter::getCPPVectorString(SpatialUpdateVect));

  // Scheduling
  if (Sign.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT)
  {
    CPP += CppWriter::getCPPMethod("TimeScheduling","setAsDefaultDeltaT",{});
  }
  else if (Sign.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED)
  {
    CPP += CppWriter::getCPPMethod("TimeScheduling","setAsFixed",{std::to_string(Sign.TimeScheduling.Min)});
  }
  else if (Sign.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
  {
    CPP += CppWriter::getCPPMethod("TimeScheduling","setAsRange",{std::to_string(Sign.TimeScheduling.Min),
                                                       std::to_string(Sign.TimeScheduling.Max)});
  }
  else
  {
    CPP += CppWriter::getCPPMethod("TimeScheduling","setAsUndefined",{});
  }
  
  CPP += CppWriter::getCPPTail();

  return CPP;
}


// =====================================================================
// =====================================================================


std::string SimulatorSignatureSerializer::toWareCMake(const openfluid::ware::SimulatorSignature& Sign) const
{
  std::string CMake;

  CMake += CppWriter::getHead("#");
  CMake += toWareCMakeBase(Sign);

  CMake += "SET(WARE_TYPE \"simulator\")\n";
  CMake += "SET(WARE_IS_SIMULATOR TRUE)\n";

  return CMake;
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::writeToBuildFiles(const openfluid::ware::SimulatorSignature& Sign,
                                                     const std::string& Path) const
{
  writeToWareCPPFile(Sign,
                     openfluid::tools::Path({Path,openfluid::config::WARESDEV_BUILD_MAINSIGN}).toGeneric());
  writeToParamsUICPPFile(Sign,
                         openfluid::tools::Path({Path,openfluid::config::WARESDEV_BUILD_PARAMSUISIGN}).toGeneric());
  writeToWareCMakeFile(Sign,
                       openfluid::tools::Path({Path,openfluid::config::WARESDEV_BUILD_MAININFO}).toGeneric());
  writeToParamsUICMakeFile(Sign,
                           openfluid::tools::Path({Path,openfluid::config::WARESDEV_BUILD_PARAMSUIINFO}).toGeneric());
}

} }  // namespaces
