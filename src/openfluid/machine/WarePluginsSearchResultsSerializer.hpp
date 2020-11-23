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
  @file WarePluginsSearchResultsSerializer.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_WAREPLUGINSSEARCHRESULTSSERIALIZER_HPP__
#define __OPENFLUID_MACHINE_WAREPLUGINSSEARCHRESULTSSERIALIZER_HPP__


#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/ware/ObserverSignature.hpp>


namespace openfluid { namespace machine {


/**
  Class for management of search results of pluggable wares
  @tparam SignatureType class defining the container for ware signature only
*/
template<class SignatureInstanceType>
class OPENFLUID_API WarePluginsSearchResultsSerializer
{
  private:

    const WarePluginsSearchResults<SignatureInstanceType>& m_SearchResults;


    static std::string getAuthorsAsString(const openfluid::ware::WareSignature::AuthorsList_t& Authors)
    {
      std::string Str = "";
      bool First = true;

      for (auto Auth: Authors)
      {
        if (!First)
        {
          Str+= ", ";
        }
        First = false;

        Str += openfluid::tools::replaceEmptyString(Auth.first,("(unknown author)"));
        Str += " <";
        Str += openfluid::tools::replaceEmptyString(Auth.second,("(unknown author email)"));
        Str += ">";
      }

      return Str;
    }


    static std::string getSchedulingTypeAsString(openfluid::ware::SignatureTimeScheduling::SchedulingType Type)
    {
      std::string Str = "undefined";

      if (Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT)
      {
        Str = "default";
      }
      else if (Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED)
      {
        Str = "fixed";
      }
      else if (Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
      {
        Str = "range";
      }

      return Str;
    }

    static std::string getIndentedText(const unsigned int Level, 
                                       const std::string& Title, const std::string& Content = "")
    {
      std::string Text;

      if (Level == 1)
      {
        Text += "* ";
      }
      else if (Level == 2)
      {
        Text += "   - ";
      }
      else if (Level == 3)
      {
        Text += "     . ";
      }

      Text += Title;

      if (!Content.empty())
      {
        Text += ": "+Content;
      }

      return Text;
    }

    void writeListToStreamAsText(std::ostream& OutStm,bool WithErrors) const;

    void addWareMetaForText(const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const;

    void addWareMetaForText(const openfluid::ware::ObserverSignature* Sign, std::ostream& OutStm) const;

    void addSchedulingDetailsForText(const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const;

    void addDataForText(const openfluid::ware::SignatureDataItem& Item, const std::string& CatStr,
                        std::ostream& OutStm) const;

    void addSpatialDataForText(const openfluid::ware::SignatureSpatialDataItem& Item, const std::string& CatStr,
                               std::ostream& OutStm) const;

    void addTypedSpatialDataForText(const openfluid::ware::SignatureTypedSpatialDataItem& Item, 
                                    const std::string& CatStr, std::ostream& OutStm) const;

    void addDataDetailsForText(const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const;
    
    void addGraphDetailsForText(const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const;

    void addWareDynaForText(const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const;

    void addWareDynaForText(const openfluid::ware::ObserverSignature* Sign, std::ostream& OutStm) const;

    void writeToStreamAsText(std::ostream& OutStm,bool WithErrors) const;

    void addErrorsToJSONDoc(rapidjson::Document& Doc, rapidjson::Document::AllocatorType& Alloc) const;

    static std::string getJSONAsString(rapidjson::Document& Doc);

    void writeListToStreamAsJSON(std::ostream& OutStm,bool WithErrors) const;

    void addDataForJSON(const openfluid::ware::SignatureDataItem& Item,
                        rapidjson::Value& Arr,rapidjson::Document::AllocatorType& Alloc) const;

    void addSpatialDataForJSON(const openfluid::ware::SignatureSpatialDataItem& Item,
                               rapidjson::Value& Arr,rapidjson::Document::AllocatorType& Alloc) const;

    void addTypedSpatialDataForJSON(const openfluid::ware::SignatureTypedSpatialDataItem& Item,
                                    rapidjson::Value& Arr,rapidjson::Document::AllocatorType& Alloc) const;

    void addDataDetailsForJSON(const openfluid::ware::SimulatorSignature* Sign,
                               rapidjson::Value& Obj,rapidjson::Document::AllocatorType& Alloc) const;

    void addGraphDetailsForJSON(const openfluid::ware::SimulatorSignature* Sign,
                                rapidjson::Value& Obj,rapidjson::Document::AllocatorType& Alloc) const;

    void addWareDetailsForJSON(const openfluid::ware::SimulatorSignature* Sign,
                               rapidjson::Value& Obj,rapidjson::Document::AllocatorType& Alloc) const;

    void addWareDetailsForJSON(const openfluid::ware::ObserverSignature* Sign,
                               rapidjson::Value& Obj,rapidjson::Document::AllocatorType& Alloc) const;

    void writeToStreamAsJSON(std::ostream& OutStm,bool WithErrors) const;


  public:

    WarePluginsSearchResultsSerializer(const WarePluginsSearchResults<SignatureInstanceType>& SearchResults):
      m_SearchResults(SearchResults)
    {

    }

    void writeToStream(std::ostream& OutStm,const std::string& Format,bool Detailed, bool WithErrors) const;
 
};


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::writeListToStreamAsText(std::ostream& OutStm,
                                                                                        bool WithErrors) const
{
  for (auto& Plug : m_SearchResults.availablePlugins())
  {
    if (Plug->Verified && Plug->Signature)
    {
      OutStm << Plug->Signature->ID << "\n";
    }
  }

  if (WithErrors && !m_SearchResults.erroredFiles().empty())
  {
    for (auto& EFile : m_SearchResults.erroredFiles())
    {
      OutStm << "Error on file " << EFile.first <<  ": " << EFile.second << "\n";
    }
  }
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addWareMetaForText(
  const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const
{  
  OutStm << getIndentedText(2,"Domain",openfluid::tools::replaceEmptyString(Sign->Domain,"(unknown)")) << "\n";
  OutStm << getIndentedText(2,"Process",openfluid::tools::replaceEmptyString(Sign->Process,"(unknown)")) << "\n";
  OutStm << getIndentedText(2,"Method",openfluid::tools::replaceEmptyString(Sign->Method,"(unknown)")) << "\n";  
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addWareMetaForText(
  const openfluid::ware::ObserverSignature* /*Sign*/, std::ostream& /*OutStm*/) const
{
  // nothing to be done  
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addSchedulingDetailsForText(
  const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const
{
  std::ostringstream oss;

  if (Sign->TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::DEFAULT)
  {
    oss << "fixed to default deltaT";
  }
  else if (Sign->TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::FIXED)
  {
    oss << "fixed to " <<  Sign->TimeScheduling.Min << " seconds";
  }
  else if (Sign->TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::RANGE)
  {
    oss << "range between " <<  Sign->TimeScheduling.Min << " and " << Sign->TimeScheduling.Max << " seconds";
  }
  else
  {
    oss << "undefined";
  }

  OutStm << getIndentedText(2,"Time scheduling",oss.str()) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addDataForText(
  const openfluid::ware::SignatureDataItem& Item, const std::string& CatStr, std::ostream& OutStm) const
{
  std::ostringstream oss;

  std::string UnitStr;
  if (!Item.DataUnit.empty())
  {
    UnitStr = " ("+Item.DataUnit+")";
  }

  oss << CatStr << ". " << Item.Description << UnitStr;
  OutStm << getIndentedText(3,Item.DataName,oss.str()) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addSpatialDataForText(
  const openfluid::ware::SignatureSpatialDataItem& Item, const std::string& CatStr, std::ostream& OutStm) const
{
  std::ostringstream oss;

  std::string UnitStr;
  if (!Item.DataUnit.empty())
  {
    UnitStr = " ("+Item.DataUnit+")";
  }

  oss << CatStr << ". " << Item.Description << UnitStr;
  OutStm << getIndentedText(3,Item.DataName+" {"+Item.UnitsClass+"}",oss.str()) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addTypedSpatialDataForText(
  const openfluid::ware::SignatureTypedSpatialDataItem& Item, const std::string& CatStr, std::ostream& OutStm) const
{
  std::ostringstream oss;

  std::string UnitStr;
  if (!Item.DataUnit.empty())
  {
    UnitStr = " ("+Item.DataUnit+")";
  }

  std::string TypeStr;
  if (Item.DataType != openfluid::core::Value::NONE)
  {
    TypeStr = " ["+openfluid::core::Value::getStringFromValueType(Item.DataType)+"]";
  }

  oss << CatStr << ". " << Item.Description << UnitStr;
  OutStm << getIndentedText(3,Item.DataName+" {"+Item.UnitsClass+"}"+TypeStr,oss.str()) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addDataDetailsForText(
  const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const
{
  const auto& Data = Sign->HandledData;

  if (!Data.UsedParams.empty() || !Data.RequiredParams.empty() ||
      !Data.ProducedVars.empty() || !Data.UpdatedVars.empty() || 
      !Data.RequiredVars.empty() || !Data.UsedVars.empty() ||
      !Data.ProducedAttribute.empty() || !Data.RequiredAttribute.empty() || !Data.UsedAttribute.empty() ||
      !Data.RequiredExtraFiles.empty() || !Data.UsedExtraFiles.empty() ||
      !Data.UsedEventsOnUnits.empty())
  {
    OutStm << getIndentedText(2,"Handled data") << "\n";


    // ------ Parameters

    for (const auto& Item : Data.RequiredParams)
    {
      addDataForText(Item,"required parameter",OutStm);
    }

    for (const auto& Item : Data.UsedParams)
    {
      addDataForText(Item,"used parameter",OutStm);
    }

    
    // ------ Attributes

    for (const auto& Item : Data.RequiredAttribute)
    {
      addSpatialDataForText(Item,"required attribute",OutStm);
    } 

    for (const auto& Item : Data.UsedAttribute)
    {
      addSpatialDataForText(Item,"used attribute",OutStm);
    } 

    for (const auto& Item : Data.ProducedAttribute)
    {
      addSpatialDataForText(Item,"produced attribute",OutStm);
    } 


    // ------ Variables

    for (const auto& Item : Data.RequiredVars)
    {
      addTypedSpatialDataForText(Item,"required variable",OutStm);
    } 

    for (const auto& Item : Data.UsedVars)
    {
      addTypedSpatialDataForText(Item,"used variable",OutStm);
    } 

    for (const auto& Item : Data.UpdatedVars)
    {
      addTypedSpatialDataForText(Item,"updated variable",OutStm);
    } 

    for (const auto& Item : Data.ProducedVars)
    {
      addTypedSpatialDataForText(Item,"produced variable",OutStm);
    } 


    // ------ Events
    if (!Data.UsedEventsOnUnits.empty())
    {
      bool First = true;
      std::string EventsStr;

      for (const auto& Item : Data.UsedEventsOnUnits)
      {
        if (!First)
        {
          EventsStr += ", ";
        }
        First = false;
        EventsStr += Item;
      }
      OutStm << getIndentedText(3,"Events handled on",EventsStr) << "\n";
    }


    // ------ Extrafiles

    for (const auto& Item : Data.RequiredExtraFiles)
    {
      OutStm << getIndentedText(3,"Required extra file",Item) << "\n";
    }

    for (const auto& Item : Data.UsedExtraFiles)
    {
      OutStm << getIndentedText(3,"Used extra file",Item) << "\n";
    }

  }
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addGraphDetailsForText(
  const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const
{
  if (!Sign->HandledUnitsGraph.UpdatedUnitsGraph.empty() || !Sign->HandledUnitsGraph.UpdatedUnitsClass.empty())
  {
    OutStm << getIndentedText(2,"Handled units graph") << "\n";

    
    if (!Sign->HandledUnitsGraph.UpdatedUnitsGraph.empty())
    {
      OutStm << getIndentedText(3,"Global units graph updates",Sign->HandledUnitsGraph.UpdatedUnitsGraph) << "\n";
    }

    for (auto& UC : Sign->HandledUnitsGraph.UpdatedUnitsClass)
    {
      OutStm << getIndentedText(3,"Units graph update on class "+UC.UnitsClass,UC.Description) << "\n";
    }
  }
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addWareDynaForText(
  const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const
{
  addSchedulingDetailsForText(Sign,OutStm);
  addDataDetailsForText(Sign,OutStm);
  addGraphDetailsForText(Sign,OutStm);
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addWareDynaForText(
  const openfluid::ware::ObserverSignature* /*Sign*/, std::ostream& /*OutStm*/) const
{
  // nothing to be done  
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::writeToStreamAsText(std::ostream& OutStm,
                                                                                    bool WithErrors) const
{
  bool First = true;

  for (auto& Plug : m_SearchResults.availablePlugins())
  {
    if (Plug->Verified && Plug->Signature)
    {
      if (!First)
      {
        OutStm << "================================================================================\n";
      }
      First = false;

      const auto Sign = Plug->Signature;

      OutStm << getIndentedText(1,Sign->ID) << "\n";
      OutStm << getIndentedText(2,"Name",openfluid::tools::replaceEmptyString(Sign->Name,"(none)")) << "\n";
      OutStm << getIndentedText(2,"File",Plug->FileFullPath) << "\n";
      addWareMetaForText(Sign,OutStm);
      OutStm << 
        getIndentedText(2,"Description",openfluid::tools::replaceEmptyString(Sign->Description,"(none)")) << "\n";
      OutStm << getIndentedText(2,"Version",Sign->Version) << "\n";
      OutStm << getIndentedText(2,"SDK version used at build time",Sign->BuildInfo.SDKVersion) << "\n";
      OutStm << getIndentedText(2,"Build information") << "\n";
      OutStm << 
        getIndentedText(3,"Build type",openfluid::tools::replaceEmptyString(Sign->BuildInfo.BuildType,"(unknown)")) << 
        "\n";
      OutStm << 
        getIndentedText(3,"Compiler ID",openfluid::tools::replaceEmptyString(Sign->BuildInfo.CompilerID,"(unknown)")) <<
        "\n";
      OutStm << 
        getIndentedText(3,"Compiler version",
                        openfluid::tools::replaceEmptyString(Sign->BuildInfo.CompilerVersion,"(unknown)")) << 
        "\n";
      OutStm << 
        getIndentedText(3,"Compiler flags",
                        openfluid::tools::replaceEmptyString(Sign->BuildInfo.CompilerFlags,"(none specified)")) << 
        "\n";
      OutStm << 
        getIndentedText(2,"Development status",openfluid::ware::WareSignature::getStatusAsString(Sign->Status)) << "\n";
      OutStm << getIndentedText(2,"Author(s)",getAuthorsAsString(Sign->Authors)) << "\n";
      addWareDynaForText(Sign,OutStm);
    }
  }

  if (WithErrors && !m_SearchResults.erroredFiles().empty())
  {
    if (!First)
    {
      OutStm << "================================================================================\n";
    }
    First = false;

    for (auto& EFile : m_SearchResults.erroredFiles())
    {
      
      OutStm << "Error on file " << EFile.first <<  ": " << EFile.second << "\n";
    }
  }
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addErrorsToJSONDoc(
  rapidjson::Document& Doc, rapidjson::Document::AllocatorType& Alloc) const
{
  rapidjson::Value Errors(rapidjson::kArrayType);

  for (auto& EFile : m_SearchResults.erroredFiles())
  {
    rapidjson::Value EObj(rapidjson::kObjectType);
    EObj.AddMember("file_path",rapidjson::Value(EFile.first.c_str(),Alloc),Alloc);
    EObj.AddMember("message",rapidjson::Value(EFile.second.c_str(),Alloc),Alloc);
    Errors.PushBack(EObj,Alloc);
  }
  
  Doc.AddMember("errors",Errors,Alloc);
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
std::string WarePluginsSearchResultsSerializer<SignatureInstanceType>::getJSONAsString(rapidjson::Document& Doc)
{
  rapidjson::StringBuffer JSONbuffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> JSONwriter(JSONbuffer);
  JSONwriter.SetIndent(' ',2);
  Doc.Accept(JSONwriter);      

  return JSONbuffer.GetString();
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::writeListToStreamAsJSON(std::ostream& OutStm,
                                                                                        bool WithErrors) const
{
  rapidjson::Document JSON;
  JSON.SetObject();
  rapidjson::Document::AllocatorType& JSONalloc = JSON.GetAllocator();

  rapidjson::Value Available(rapidjson::kArrayType);

  for (auto& Plug : m_SearchResults.availablePlugins())
  {
    if (Plug->Verified && Plug->Signature)
    {
      Available.PushBack(rapidjson::Value(Plug->Signature->ID.c_str(),JSONalloc),JSONalloc);
    }
  }

  JSON.AddMember("available",Available,JSONalloc);

  if (WithErrors)
  {
    addErrorsToJSONDoc(JSON,JSONalloc);
  }


  OutStm << getJSONAsString(JSON) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addDataForJSON(
  const openfluid::ware::SignatureDataItem& Item, rapidjson::Value& Arr,rapidjson::Document::AllocatorType& Alloc) const
{
   rapidjson::Value Obj(rapidjson::kObjectType);
   Obj.AddMember("name",rapidjson::Value(Item.DataName.c_str(),Alloc),Alloc);
   Obj.AddMember("description",rapidjson::Value(Item.Description.c_str(),Alloc),Alloc);
   Obj.AddMember("si_unit",rapidjson::Value(Item.DataUnit.c_str(),Alloc),Alloc);
   Arr.PushBack(Obj,Alloc);
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addSpatialDataForJSON(
  const openfluid::ware::SignatureSpatialDataItem& Item, 
  rapidjson::Value& Arr,rapidjson::Document::AllocatorType& Alloc) const
{
  rapidjson::Value Obj(rapidjson::kObjectType);
   Obj.AddMember("name",rapidjson::Value(Item.DataName.c_str(),Alloc),Alloc);
   Obj.AddMember("units_class",rapidjson::Value(Item.UnitsClass.c_str(),Alloc),Alloc);
   Obj.AddMember("description",rapidjson::Value(Item.Description.c_str(),Alloc),Alloc);
   Obj.AddMember("si_unit",rapidjson::Value(Item.DataUnit.c_str(),Alloc),Alloc);
   Arr.PushBack(Obj,Alloc);
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addTypedSpatialDataForJSON(
  const openfluid::ware::SignatureTypedSpatialDataItem& Item,
  rapidjson::Value& Arr,rapidjson::Document::AllocatorType& Alloc) const
{
  std::string TypeStr = openfluid::core::Value::getStringFromValueType(Item.DataType);

  rapidjson::Value Obj(rapidjson::kObjectType);
   Obj.AddMember("name",rapidjson::Value(Item.DataName.c_str(),Alloc),Alloc);
   Obj.AddMember("type",rapidjson::Value(TypeStr.c_str(),Alloc),Alloc);
   Obj.AddMember("units_class",rapidjson::Value(Item.UnitsClass.c_str(),Alloc),Alloc);
   Obj.AddMember("description",rapidjson::Value(Item.Description.c_str(),Alloc),Alloc);
   Obj.AddMember("si_unit",rapidjson::Value(Item.DataUnit.c_str(),Alloc),Alloc);
   Arr.PushBack(Obj,Alloc);
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addDataDetailsForJSON(
  const openfluid::ware::SimulatorSignature* Sign,
  rapidjson::Value& Obj,
  rapidjson::Document::AllocatorType& Alloc) const
{
  // ------ Parameters

  rapidjson::Value ParamsObj(rapidjson::kObjectType);

  rapidjson::Value ReqParamsArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.RequiredParams)
  {
    addDataForJSON(Item,ReqParamsArray,Alloc);
  }

  rapidjson::Value UsedParamsArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.UsedParams)
  {
    addDataForJSON(Item,UsedParamsArray,Alloc);
  }

  ParamsObj.AddMember("required",ReqParamsArray,Alloc);
  ParamsObj.AddMember("used",UsedParamsArray,Alloc);
  Obj.AddMember("parameters",ParamsObj,Alloc);


  // ------ Attributes

  rapidjson::Value AttrsObj(rapidjson::kObjectType);

  rapidjson::Value ReqAttrsArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.RequiredAttribute)
  {
    addSpatialDataForJSON(Item,ReqAttrsArray,Alloc);
  } 

  rapidjson::Value UsedAttrsArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.UsedAttribute)
  {
    addSpatialDataForJSON(Item,UsedAttrsArray,Alloc);
  } 

  rapidjson::Value ProdAttrsArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.ProducedAttribute)
  {
    addSpatialDataForJSON(Item,ProdAttrsArray,Alloc);
  } 

  AttrsObj.AddMember("required",ReqAttrsArray,Alloc);
  AttrsObj.AddMember("used",UsedAttrsArray,Alloc);
  AttrsObj.AddMember("produced",ProdAttrsArray,Alloc);
  Obj.AddMember("attributes",AttrsObj,Alloc);


  // ------ Variables

  rapidjson::Value VarsObj(rapidjson::kObjectType);

  rapidjson::Value ReqVarsArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.RequiredVars)
  {
    addTypedSpatialDataForJSON(Item,ReqVarsArray,Alloc);
  } 

  rapidjson::Value UsedVarsArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.UsedVars)
  {
    addTypedSpatialDataForJSON(Item,UsedVarsArray,Alloc);
  } 

  rapidjson::Value UpVarsArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.UpdatedVars)
  {
    addTypedSpatialDataForJSON(Item,UpVarsArray,Alloc);
  } 

  rapidjson::Value ProdVarsArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.ProducedVars)
  {
    addTypedSpatialDataForJSON(Item,ProdVarsArray,Alloc);
  } 

  VarsObj.AddMember("required",ReqVarsArray,Alloc);
  VarsObj.AddMember("used",UsedVarsArray,Alloc);
  VarsObj.AddMember("updated",UpVarsArray,Alloc);
  VarsObj.AddMember("produced",ProdVarsArray,Alloc);  
  Obj.AddMember("variables",VarsObj,Alloc);


  // ------ Events

  rapidjson::Value EvArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.UsedEventsOnUnits)
  {
    EvArray.PushBack(rapidjson::Value(Item.c_str(),Alloc),Alloc);
  }
  Obj.AddMember("events",EvArray,Alloc);


  // ------ Extrafiles

  rapidjson::Value ExtraObj(rapidjson::kObjectType);

  rapidjson::Value ReqExtraArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.RequiredExtraFiles)
  {
    ReqExtraArray.PushBack(rapidjson::Value(Item.c_str(),Alloc),Alloc);
  }

  rapidjson::Value UsedExtraArray(rapidjson::kArrayType);
  for (const auto& Item : Sign->HandledData.UsedExtraFiles)
  {
    UsedExtraArray.PushBack(rapidjson::Value(Item.c_str(),Alloc),Alloc);
  }

  ExtraObj.AddMember("required",ReqExtraArray,Alloc);
  ExtraObj.AddMember("used",UsedExtraArray,Alloc);
  Obj.AddMember("extra_files",ExtraObj,Alloc);
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addGraphDetailsForJSON(
  const openfluid::ware::SimulatorSignature* Sign,
  rapidjson::Value& Obj,rapidjson::Document::AllocatorType& Alloc) const
{
  Obj.AddMember("description",rapidjson::Value(Sign->HandledUnitsGraph.UpdatedUnitsGraph.c_str(),Alloc),Alloc);

  rapidjson::Value ClassesArray(rapidjson::kArrayType);
  for (const auto& Class : Sign->HandledUnitsGraph.UpdatedUnitsClass)
  {
    rapidjson::Value ClassObj(rapidjson::kObjectType);
    ClassObj.AddMember("class_name",rapidjson::Value(Class.UnitsClass.c_str(),Alloc),Alloc);
    ClassObj.AddMember("description",rapidjson::Value(Class.Description.c_str(),Alloc),Alloc);
    ClassesArray.PushBack(ClassObj,Alloc);
  }
  Obj.AddMember("units_classses",ClassesArray,Alloc);
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addWareDetailsForJSON(
  const openfluid::ware::SimulatorSignature* Sign, 
  rapidjson::Value& Obj, rapidjson::Document::AllocatorType& Alloc) const
{
  Obj.AddMember("domain",rapidjson::Value(Sign->Domain.c_str(),Alloc),Alloc);
  Obj.AddMember("process",rapidjson::Value(Sign->Process.c_str(),Alloc),Alloc);
  Obj.AddMember("method",rapidjson::Value(Sign->Method.c_str(),Alloc),Alloc);


  rapidjson::Value DataObj(rapidjson::kObjectType);
  addDataDetailsForJSON(Sign,DataObj,Alloc);
  Obj.AddMember("data",DataObj,Alloc);


  rapidjson::Value SchedObj(rapidjson::kObjectType);
  SchedObj.AddMember("type",rapidjson::Value(getSchedulingTypeAsString(Sign->TimeScheduling.Type).c_str(),Alloc),
                     Alloc);
  SchedObj.AddMember("min",Sign->TimeScheduling.Min,Alloc);
  SchedObj.AddMember("min",Sign->TimeScheduling.Max,Alloc);
  Obj.AddMember("scheduling",SchedObj,Alloc);


  rapidjson::Value GraphObj(rapidjson::kObjectType);
  addGraphDetailsForJSON(Sign,GraphObj,Alloc);
  Obj.AddMember("spatial_graph",GraphObj,Alloc);
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::addWareDetailsForJSON(
  const openfluid::ware::ObserverSignature* /*Sign*/, 
  rapidjson::Value& /*Obj*/, 
  rapidjson::Document::AllocatorType& /*Alloc*/) const
{
  // nothing to be done
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::writeToStreamAsJSON(std::ostream& OutStm,
                                                                                    bool WithErrors) const
{
  rapidjson::Document JSON;
  JSON.SetObject();
  rapidjson::Document::AllocatorType& JSONalloc = JSON.GetAllocator();

  rapidjson::Value Available(rapidjson::kArrayType);

  for (auto& Plug : m_SearchResults.availablePlugins())
  {
    if (Plug->Verified && Plug->Signature)
    {
      const auto Sign = Plug->Signature;

      rapidjson::Value WareObj(rapidjson::kObjectType);
      WareObj.AddMember("id",rapidjson::Value(Sign->ID.c_str(),JSONalloc),JSONalloc);
      WareObj.AddMember("file_path",rapidjson::Value(Plug->FileFullPath.c_str(),JSONalloc),JSONalloc);
      WareObj.AddMember("abi_version",rapidjson::Value(Sign->BuildInfo.SDKVersion.c_str(),JSONalloc),JSONalloc);

      rapidjson::Value BuildObj(rapidjson::kObjectType);
      BuildObj.AddMember("type",rapidjson::Value(Sign->BuildInfo.BuildType.c_str(),JSONalloc),JSONalloc);
      BuildObj.AddMember("compiler_id",rapidjson::Value(Sign->BuildInfo.CompilerID.c_str(),JSONalloc),JSONalloc);
      BuildObj.AddMember("compiler_version",
                         rapidjson::Value(Sign->BuildInfo.CompilerVersion.c_str(),JSONalloc),JSONalloc);
      BuildObj.AddMember("compiler_flags",rapidjson::Value(Sign->BuildInfo.CompilerFlags.c_str(),JSONalloc),JSONalloc);
      WareObj.AddMember("build_info",BuildObj,JSONalloc);

      WareObj.AddMember("name",rapidjson::Value(Sign->Name.c_str(),JSONalloc),JSONalloc);
      WareObj.AddMember("description",rapidjson::Value(Sign->Description.c_str(),JSONalloc),JSONalloc); 
      WareObj.AddMember("version",rapidjson::Value(Sign->Version.c_str(),JSONalloc),JSONalloc);
      WareObj.AddMember("status",
                        rapidjson::Value(openfluid::ware::WareSignature::getStatusAsString(Sign->Status).c_str(),
                        JSONalloc),JSONalloc);

      rapidjson::Value AuthsArray(rapidjson::kArrayType);
      for (auto& Author : Sign->Authors)
      {
        rapidjson::Value AuthObj(rapidjson::kObjectType);
        AuthObj.AddMember("name",rapidjson::Value(Author.first.c_str(),JSONalloc),JSONalloc);
        AuthObj.AddMember("email",rapidjson::Value(Author.second.c_str(),JSONalloc),JSONalloc);
        AuthsArray.PushBack(AuthObj,JSONalloc);
      }
      WareObj.AddMember("authors",AuthsArray,JSONalloc);

      addWareDetailsForJSON(Sign,WareObj,JSONalloc);

      Available.PushBack(WareObj,JSONalloc);
    }
  }

  JSON.AddMember("available",Available,JSONalloc);


  if (WithErrors)
  {
    addErrorsToJSONDoc(JSON,JSONalloc);
  }


  OutStm << getJSONAsString(JSON) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResultsSerializer<SignatureInstanceType>::writeToStream(std::ostream& OutStm,
                                                            const std::string& Format,
                                                            bool Detailed, bool WithErrors) const
{
  if (Format == "text")
  {
    if (Detailed)
    {
      writeToStreamAsText(OutStm,WithErrors);
    }
    else
    {
      writeListToStreamAsText(OutStm,WithErrors);
    }
  }
  else if (Format == "json")
  {
    if (Detailed)
    {
      writeToStreamAsJSON(OutStm,WithErrors);
    }
    else
    {
      writeListToStreamAsJSON(OutStm,WithErrors);
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Wrong format for available wares reporting");
  }
}


} }  // namespaces


#endif /* __OPENFLUID_MACHINE_WAREPLUGINSSEARCHRESULTSSERIALIZER_HPP__ */
