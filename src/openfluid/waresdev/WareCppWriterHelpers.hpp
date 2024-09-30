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
  @file WareCppWriterHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEV_WARECPPWRITERHELPERS_HPP__
#define __OPENFLUID_WARESDEV_WARECPPWRITERHELPERS_HPP__


#include <string>
#include <vector>

#include <openfluid/dllexport.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/waresdev/WareSignatureSerializer.hpp>


namespace openfluid { namespace waresdev {


struct CppWriter {

  static std::string getHead(const std::string CommentChar)
  {
    std::string Head =
    CommentChar + " ======\n" +
    CommentChar + " AUTOMATICALLY GENERATED FILE. DO NOT EDIT MANUALLY\n" +
    CommentChar + " ======\n\n"; 

    return Head;
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPHead(const std::string& WareIncludeStr, const std::string& WareTypeStr)
  {
    std::string Head = getHead("//");
    
    Head += 
      "#include <"+WareIncludeStr+">\n\n"
      "extern \"C\" {\n"
      "OPENFLUID_PLUGIN "+WareTypeStr+"* "+std::string(WARESIGNATURE_PROC_NAME)+"()\n"
      "{\n"
      +WareTypeStr+"* Signature = new "+WareTypeStr+"();\n\n"; 

    return Head;
  }


  // =====================================================================
  // =====================================================================


  static std::string getQuotedString(const std::string& Str)
  {
    return "\""+Str+"\"";
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPDateTimeString(const openfluid::core::DateTime& DT) // TODO EXPORT AS UTILS?
  {
    std::string Str = "openfluid::core::DateTime(";

    Str += std::to_string(DT.getYear())+",";
    Str += std::to_string(DT.getMonth())+",";
    Str += std::to_string(DT.getDay())+",";
    Str += std::to_string(DT.getHour())+",";
    Str += std::to_string(DT.getMinute())+",";
    Str += std::to_string(DT.getSecond())+")";

    return Str;
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPVectorString(const std::vector<std::string>& StrVect,bool AutoQuote = false)
  {
    std::string Str;

    Str += "{";
    bool isFirst = true;
    for (const auto& S : StrVect)
    {
      if (!isFirst)
      {
        Str += ",";
      }
      if (AutoQuote)
      {
        Str += getQuotedString(S);
      }
      else
      {
        Str += S;
      }
      isFirst = false;
    }
    Str += "}";

    return Str;
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPEntry(const std::string& Member)
  {
    return "Signature->"+Member;
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPAssignment(const std::string& Member, const std::string& Value, bool AutoQuote = false)
  {
    std::string QuotedVal(Value);
    
    if (AutoQuote)
    {
      QuotedVal = getQuotedString(QuotedVal);
    }
    
    return getCPPEntry(Member)+" = "+QuotedVal+";\n";
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPMethod(const std::string& Member, const std::string& Method, 
                                  const std::vector<std::string>& Args, const std::string& Access = ".")
  {
    std::string ArgsStr;
    bool FirstArg = true;
    
    for (const auto& A : Args)
    {
      if (FirstArg)
      {
        FirstArg = false;
      }
      else
      {
        ArgsStr += ",";
      }

      ArgsStr += A;
    }

    return getCPPEntry(Member)+Access+Method+"("+ArgsStr+");\n";
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPLinkUIDProc(const std::string LinkUID)
  {
    std::string Str =
      "extern \"C\" {\n"
      "OPENFLUID_PLUGIN const std::string* "+std::string(WARELINKUID_PROC_NAME)+"()\n"
      "{\n"
      "return new std::string(\""+LinkUID+"\");\n"
      "}\n"
      "}"; 
    return Str;
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPSpatialDataString(
    const std::string Member,const std::vector<openfluid::ware::SignatureSpatialDataItem>& Data)
  {
    std::string Str;
    
    for (const auto& D : Data)
    {
      Str += getCPPMethod(Member,"push_back",{"{"+CppWriter::getQuotedString(D.Name)+","+
                                                  CppWriter::getQuotedString(D.UnitsClass)+","+
                                                  CppWriter::getQuotedString(D.Description)+","+
                                                  CppWriter::getQuotedString(D.SIUnit)+","+
                                                  CppWriter::getCPPValueType(D.DataType)+"}"});
    }

    return Str;
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPTail()
  {
    return "\nreturn Signature;\n}\n}\n";
  }


  // =====================================================================
  // =====================================================================


  static std::string getCPPValueType(const openfluid::core::Value::Type ValueType)
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


  static std::string getCPPDataString(const std::string Member, 
                                                      const std::vector<openfluid::ware::SignatureDataItem>& Data)
  {
    std::string Str;
    
    for (const auto& D : Data)
    {

      Str += CppWriter::getCPPMethod(Member,"push_back",{"{"+getQuotedString(D.Name)+","+
                          getQuotedString(D.Description)+","+
                          getQuotedString(D.SIUnit)+","+
                          getCPPValueType(D.DataType)+"}"});
  }

    return Str;
  }


  // =====================================================================
  // =====================================================================


  static std::string toWareCPPParams(const openfluid::ware::SignatureHandledData& HandledData)
  {
    std::string CPP;
    
    // Parameters
    CPP += getCPPDataString("HandledData.UsedParams", HandledData.UsedParams);
    CPP += getCPPDataString("HandledData.RequiredParams", HandledData.RequiredParams);
    
    return CPP;
  }
};


// =====================================================================
// =====================================================================


struct DataJsonConverter
{

  static openfluid::ware::SignatureDataItem readDataItemFromJSON(const openfluid::thirdparty::json& Item)
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


  static openfluid::thirdparty::json serializeDataItemToJSON(const openfluid::ware::SignatureDataItem& Item)
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


  std::vector<openfluid::ware::SignatureDataItem> 
  static readDataListFromJSON(const openfluid::thirdparty::json& Json)
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


  static openfluid::thirdparty::json serializeParametersToJSON(const openfluid::ware::SignatureHandledData& HandledData)
  {
    openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

    auto JsonReq = openfluid::thirdparty::json::array();
    for (const auto& P : HandledData.RequiredParams)
    {
      JsonReq.push_back(serializeDataItemToJSON(P));
    }
    Json["required"] = JsonReq;

    auto JsonUs = openfluid::thirdparty::json::array();
    for (const auto& P : HandledData.UsedParams)
    {
      JsonUs.push_back(serializeDataItemToJSON(P));
    }
    Json["used"] = JsonUs;

    return Json;
  }


  // =====================================================================
  // =====================================================================


  static openfluid::thirdparty::json serializeSpatialDataItemToJSON(const openfluid::ware::SignatureSpatialDataItem& 
                                                                    Item)
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


  static openfluid::thirdparty::json
  serializeAttributesToJSON(const openfluid::ware::SignatureHandledData& HandledData)
  {
    openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

    auto JsonReq = openfluid::thirdparty::json::array();
    for (const auto& A : HandledData.RequiredAttribute)
    {
      JsonReq.push_back(serializeSpatialDataItemToJSON(A));
    }
    Json["required"] = JsonReq;

    auto JsonUs = openfluid::thirdparty::json::array();
    for (const auto& A : HandledData.UsedAttribute)
    {
      JsonUs.push_back(serializeSpatialDataItemToJSON(A));
    }
    Json["used"] = JsonUs;

    return Json;
  }


  // =====================================================================
  // =====================================================================


  static openfluid::thirdparty::json
  serializeVariablesToJSON(const openfluid::ware::SignatureHandledData& HandledData)
  {
    openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

    auto JsonReq = openfluid::thirdparty::json::array();
    for (const auto& V : HandledData.RequiredVars)
    {
      JsonReq.push_back(serializeSpatialDataItemToJSON(V));
    }
    Json["required"] = JsonReq;

    auto JsonUs = openfluid::thirdparty::json::array();
    for (const auto& V : HandledData.UsedVars)
    {
      JsonUs.push_back(serializeSpatialDataItemToJSON(V));
    }
    Json["used"] = JsonUs;

    return Json;
  }


  // =====================================================================
  // =====================================================================


  static openfluid::thirdparty::json
  serializeExtrafilesToJSON(const openfluid::ware::SignatureHandledData& HandledData)
  {
    openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

    Json["required"] = HandledData.RequiredExtraFiles;
    Json["used"] = HandledData.UsedExtraFiles;

    return Json;
  }


  // =====================================================================
  // =====================================================================


  static openfluid::thirdparty::json serializeDataToJSON(const openfluid::ware::SignatureHandledData& HandledData)
  {
    openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

    Json["parameters"] = serializeParametersToJSON(HandledData);
    Json["attributes"] = serializeAttributesToJSON(HandledData);
    Json["variables"] = serializeVariablesToJSON(HandledData);
    Json["extrafiles"] = serializeExtrafilesToJSON(HandledData);


    return Json;
  }


  // =====================================================================
  // =====================================================================


  static void unserializeParametersFromJSON(const openfluid::thirdparty::json& Json, 
                                                              openfluid::ware::SignatureHandledData& HandledData)
  {
    if (Json.contains("used"))
    {
      HandledData.UsedParams = readDataListFromJSON(Json.at("used"));
    }

    if (Json.contains("required"))
    {
      HandledData.RequiredParams = readDataListFromJSON(Json.at("required"));
    }
  }


  // =====================================================================
  // =====================================================================


  static openfluid::ware::SignatureSpatialDataItem readSpatialDataItemFromJSON(const openfluid::thirdparty::json& Item)
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


  static std::vector<openfluid::ware::SignatureSpatialDataItem> 
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


  static void unserializeReadAttributesFromJSON(const openfluid::thirdparty::json& Json, 
                                                                  openfluid::ware::DataWareSignature& Sign)
  {

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


  static void unserializeReadVariablesFromJSON(const openfluid::thirdparty::json& Json, 
                                                                  openfluid::ware::DataWareSignature& Sign)
  {
    if (Json.contains("used"))
    {
      Sign.HandledData.UsedVars = readSpatialDataListFromJSON(Json.at("used"));
    }

    if (Json.contains("required"))
    {
      Sign.HandledData.RequiredVars = readSpatialDataListFromJSON(Json.at("required"));
    }
  }


  // =====================================================================
  // =====================================================================


  static void unserializeExtrafilesFromJSON(const openfluid::thirdparty::json& Json, 
                                                                  openfluid::ware::DataWareSignature& Sign)
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

};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARECPPWRITERHELPERS_HPP__ */
