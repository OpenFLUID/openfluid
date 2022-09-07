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
 */


#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/tools/FilesystemPath.hpp>


namespace openfluid { namespace waresdev {


openfluid::ware::SignatureDataItem readDataItemFromJSON(const openfluid::thirdparty::json& Item)
{
  openfluid::ware::SignatureDataItem Data;

  Data.Name = Item.value("name","");

  if (!openfluid::tools::isValidVariableName(Data.Name))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Missing or invalid data name");
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


std::vector<openfluid::ware::SignatureDataItem> 
readDataListFromJSON(const openfluid::thirdparty::json& Json)
{
  std::vector<openfluid::ware::SignatureDataItem> List;

  if (Json.is_array())
  {
    for (const auto& I : Json)
    {
      List.push_back(readDataItemFromJSON(I));
    }
  }

  return List;
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


void SimulatorSignatureSerializer::unserializeParametersFromJSON(const openfluid::thirdparty::json& Json, 
                                                                 openfluid::ware::SimulatorSignature& Sign) const
{
  if (Json.contains("used"))
  {
    Sign.HandledData.UsedParams = readDataListFromJSON(Json.at("used"));
  }

  if (Json.contains("required"))
  {
    Sign.HandledData.RequiredParams = readDataListFromJSON(Json.at("required"));
  }
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeAttributesFromJSON(const openfluid::thirdparty::json& Json, 
                                                                 openfluid::ware::SimulatorSignature& Sign) const
{
  if (Json.contains("produced"))
  {
    Sign.HandledData.ProducedAttribute = readSpatialDataListFromJSON(Json.at("produced"));
  }

  if (Json.contains("used"))
  {
    Sign.HandledData.UsedAttribute = readSpatialDataListFromJSON(Json.at("used"));
  }

  if (Json.contains("required"))
  {
    Sign.HandledData.RequiredAttribute = readSpatialDataListFromJSON(Json.at("required"));
  }
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeVariablesFromJSON(const openfluid::thirdparty::json& Json, 
                                                                openfluid::ware::SimulatorSignature& Sign) const
{
  if (Json.contains("produced"))
  {
    Sign.HandledData.ProducedVars = readSpatialDataListFromJSON(Json.at("produced"));
  }

  if (Json.contains("used"))
  {
    Sign.HandledData.UsedVars = readSpatialDataListFromJSON(Json.at("used"));
  }

  if (Json.contains("required"))
  {
    Sign.HandledData.RequiredVars = readSpatialDataListFromJSON(Json.at("required"));
  }

  if (Json.contains("updated"))
  {
    Sign.HandledData.UpdatedVars = readSpatialDataListFromJSON(Json.at("updated"));
  }
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeEventsFromJSON(const openfluid::thirdparty::json& Json, 
                                                             openfluid::ware::SimulatorSignature& Sign) const
{
  Sign.HandledData.UsedEventsOnUnits = Json.get<std::vector<std::string>>();
}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeExtrafilesFromJSON(const openfluid::thirdparty::json& Json, 
                                                                 openfluid::ware::SimulatorSignature& Sign) const
{
  if (Json.contains("used"))
  {
    Sign.HandledData.UsedExtraFiles = Json.at("used").get<std::vector<std::string>>();
  }

  if (Json.contains("required"))
  {
    Sign.HandledData.RequiredExtraFiles = Json.at("required").get<std::vector<std::string>>();
  }

}


// =====================================================================
// =====================================================================


void SimulatorSignatureSerializer::unserializeDataFromJSON(const openfluid::thirdparty::json& Json, 
                                                           openfluid::ware::SimulatorSignature& Sign) const
{
  if (Json.contains("parameters"))
  {
    unserializeParametersFromJSON(Json.at("parameters"),Sign);
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


openfluid::thirdparty::json serializeDataItemToJSON(const openfluid::ware::SignatureDataItem& Item)
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["name"] = Item.Name;
  Json["description"] = Item.Description;
  Json["siunit"] = Item.SIUnit;
  Json["type"] = openfluid::core::Value::getStringFromValueType(Item.DataType);

  return Json;
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
SimulatorSignatureSerializer::serializeParametersToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  auto JsonReq = openfluid::thirdparty::json::array();
  for (const auto& P : Sign.HandledData.RequiredParams)
  {
    JsonReq.push_back(serializeDataItemToJSON(P));
  }
  Json["required"] = JsonReq;

  auto JsonUs = openfluid::thirdparty::json::array();
  for (const auto& P : Sign.HandledData.UsedParams)
  {
    JsonUs.push_back(serializeDataItemToJSON(P));
  }
  Json["used"] = JsonUs;

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeAttributesToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  auto JsonReq = openfluid::thirdparty::json::array();
  for (const auto& A : Sign.HandledData.RequiredAttribute)
  {
    JsonReq.push_back(serializeSpatialDataItemToJSON(A));
  }
  Json["required"] = JsonReq;

  auto JsonUs = openfluid::thirdparty::json::array();
  for (const auto& A : Sign.HandledData.UsedAttribute)
  {
    JsonUs.push_back(serializeSpatialDataItemToJSON(A));
  }
  Json["used"] = JsonUs;

  auto JsonProd = openfluid::thirdparty::json::array();
  for (const auto& A : Sign.HandledData.ProducedAttribute)
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
  for (const auto& V : Sign.HandledData.ProducedVars)
  {
    JsonProd.push_back(serializeSpatialDataItemToJSON(V));
  }
  Json["produced"] = JsonProd;

  auto JsonReq = openfluid::thirdparty::json::array();
  for (const auto& V : Sign.HandledData.RequiredVars)
  {
    JsonReq.push_back(serializeSpatialDataItemToJSON(V));
  }
  Json["required"] = JsonReq;

  auto JsonUs = openfluid::thirdparty::json::array();
  for (const auto& V : Sign.HandledData.UsedVars)
  {
    JsonUs.push_back(serializeSpatialDataItemToJSON(V));
  }
  Json["used"] = JsonUs;

  auto JsonUp = openfluid::thirdparty::json::array();
  for (const auto& V : Sign.HandledData.UpdatedVars)
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
  openfluid::thirdparty::json Json = Sign.HandledData.UsedEventsOnUnits;

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeExtrafilesToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["required"] = Sign.HandledData.RequiredExtraFiles;
  Json["used"] = Sign.HandledData.UsedExtraFiles;

  return Json;
}


// =====================================================================
// =====================================================================


openfluid::thirdparty::json
SimulatorSignatureSerializer::serializeDataToJSON(const openfluid::ware::SimulatorSignature& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["parameters"] = serializeParametersToJSON(Sign);
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


std::string getCPPValueType(const openfluid::core::Value::Type ValueType)
{
  switch (ValueType)
  {
    case openfluid::core::Value::NONE:
      return "openfluid::core::Value::NONE";
    case openfluid::core::Value::DOUBLE:
      return "openfluid::core::Value::DOUBLE";
    case openfluid::core::Value::INTEGER:
      return "openfluid::core::Value::INTEGER";
    case openfluid::core::Value::BOOLEAN:
      return "openfluid::core::Value::BOOLEAN";
    case openfluid::core::Value::VECTOR:
      return "openfluid::core::Value::VECTOR";
    case openfluid::core::Value::MATRIX:
      return "openfluid::core::Value::MATRIX";
    case openfluid::core::Value::MAP:
      return "openfluid::core::Value::MAP";
    case openfluid::core::Value::TREE:
      return "openfluid::core::Value::TREE";
    case openfluid::core::Value::STRING:
      return "openfluid::core::Value::STRING";
    case openfluid::core::Value::NULLL:
      return "openfluid::core::Value::NULLL";
    default:
      return "";
  }
}


// =====================================================================
// =====================================================================


std::string SimulatorSignatureSerializer::getCPPDataString(const std::string Member, 
                                                           const std::vector<openfluid::ware::SignatureDataItem>& Data)
{
  std::string Str;
  
  for (const auto& D : Data)
  {
    Str += getCPPMethod(Member,"push_back",{"{"+getQuotedString(D.Name)+","+
                                                getQuotedString(D.Description)+","+
                                                getQuotedString(D.SIUnit)+","+
                                                getCPPValueType(D.DataType)+"}"});
  }

  return Str;
}


// =====================================================================
// =====================================================================


std::string SimulatorSignatureSerializer::getCPPSpatialDataString(
  const std::string Member,const std::vector<openfluid::ware::SignatureSpatialDataItem>& Data)
{
  std::string Str;
  
  for (const auto& D : Data)
  {
    Str += getCPPMethod(Member,"push_back",{"{"+getQuotedString(D.Name)+","+
                                                getQuotedString(D.UnitsClass)+","+
                                                getQuotedString(D.Description)+","+
                                                getQuotedString(D.SIUnit)+","+
                                                getCPPValueType(D.DataType)+"}"});
  }

  return Str;
}


// =====================================================================
// =====================================================================


std::string SimulatorSignatureSerializer::toWareCPP(const openfluid::ware::SimulatorSignature& Sign) const
{
  std::string CPP;
  
  CPP += getCPPHead("openfluid/ware/SimulatorSignature.hpp","openfluid::ware::SimulatorSignature");
  CPP += toWareCPPBase(Sign);
  
  CPP += "\n";
  
  // Parameters
  CPP += getCPPDataString("HandledData.UsedParams",Sign.HandledData.UsedParams);
  CPP += getCPPDataString("HandledData.RequiredParams",Sign.HandledData.RequiredParams);

  // Extrafiles
  CPP += getCPPAssignment("HandledData.UsedExtraFiles",
                          getCPPVectorString(Sign.HandledData.UsedExtraFiles,true));
  CPP += getCPPAssignment("HandledData.RequiredExtraFiles",
                          getCPPVectorString(Sign.HandledData.RequiredExtraFiles,true));

  // Attributes
  CPP += getCPPSpatialDataString("HandledData.UsedAttribute",Sign.HandledData.UsedAttribute);
  CPP += getCPPSpatialDataString("HandledData.RequiredAttribute",Sign.HandledData.RequiredAttribute);
  CPP += getCPPSpatialDataString("HandledData.ProducedAttribute",Sign.HandledData.ProducedAttribute);

  // Variables
  CPP += getCPPSpatialDataString("HandledData.UsedVars",Sign.HandledData.UsedVars);
  CPP += getCPPSpatialDataString("HandledData.RequiredVars",Sign.HandledData.RequiredVars);
  CPP += getCPPSpatialDataString("HandledData.UpdatedVars",Sign.HandledData.UpdatedVars);
  CPP += getCPPSpatialDataString("HandledData.ProducedVars",Sign.HandledData.ProducedVars);

  // Events
  CPP += getCPPAssignment("HandledData.UsedEventsOnUnits",getCPPVectorString(Sign.HandledData.UsedEventsOnUnits,true));

  // Spatial struct
  CPP += getCPPAssignment("HandledUnitsGraph.UpdatedUnitsGraph",
                          getQuotedString(openfluid::tools::escapeString(Sign.HandledUnitsGraph.UpdatedUnitsGraph)));
  std::vector<std::string> SpatialUpdateVect;
  for (const auto& U : Sign.HandledUnitsGraph.UpdatedUnitsClass)
  {
    SpatialUpdateVect.push_back(getCPPVectorString({U.UnitsClass,U.Description},true));
  }
  CPP += getCPPAssignment("HandledUnitsGraph.UpdatedUnitsClass",getCPPVectorString(SpatialUpdateVect));

  // Scheduling
  if (Sign.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT)
  {
    CPP += getCPPMethod("TimeScheduling","setAsDefaultDeltaT",{});
  }
  else if (Sign.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED)
  {
    CPP += getCPPMethod("TimeScheduling","setAsFixed",{std::to_string(Sign.TimeScheduling.Min)});
  }
  else if (Sign.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
  {
    CPP += getCPPMethod("TimeScheduling","setAsRange",{std::to_string(Sign.TimeScheduling.Min),
                                                       std::to_string(Sign.TimeScheduling.Max)});
  }
  else
  {
    CPP += getCPPMethod("TimeScheduling","setAsUndefined",{});
  }
  
  CPP += getCPPTail();

  return CPP;
}


// =====================================================================
// =====================================================================


std::string SimulatorSignatureSerializer::toWareCMake(const openfluid::ware::SimulatorSignature& Sign) const
{
  std::string CMake;

  CMake += getHead("#");
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
