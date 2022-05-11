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
  @file WareRegistrySerializer.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#ifndef __OPENFLUID_MACHINE_WAREREGISTRYSERIALIZER_HPP__
#define __OPENFLUID_MACHINE_WAREREGISTRYSERIALIZER_HPP__


#include <sstream>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/ware/ObserverSignature.hpp>
#include <openfluid/thirdparty/JSON.hpp>


namespace openfluid { namespace machine {


/**
  Class for management of search results of pluggable wares
  @tparam SignatureType class defining the container for ware signature only
*/
template<class SignatureType>
class OPENFLUID_API WareRegistrySerializer
{
  private:

    const WareRegistry<SignatureType>* m_Registry;


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

    void addErrorsToJSONDoc(openfluid::thirdparty::json& Doc) const;

    static std::string getJSONAsString(openfluid::thirdparty::json& Doc);

    void writeListToStreamAsJSON(std::ostream& OutStm,bool WithErrors) const;

    void addDataForJSON(const openfluid::ware::SignatureDataItem& Item, openfluid::thirdparty::json& Arr) const;

    void addSpatialDataForJSON(const openfluid::ware::SignatureSpatialDataItem& Item,
                               openfluid::thirdparty::json& Arr) const;

    void addTypedSpatialDataForJSON(const openfluid::ware::SignatureTypedSpatialDataItem& Item,
                                    openfluid::thirdparty::json& Arr) const;

    void addDataDetailsForJSON(const openfluid::ware::SimulatorSignature* Sign,
                               openfluid::thirdparty::json& Obj) const;

    void addGraphDetailsForJSON(const openfluid::ware::SimulatorSignature* Sign,
                                openfluid::thirdparty::json& Obj) const;

    void addWareDetailsForJSON(const openfluid::ware::SimulatorSignature* Sign,
                               openfluid::thirdparty::json& Obj) const;

    void addWareDetailsForJSON(const openfluid::ware::ObserverSignature* Sign,
                               openfluid::thirdparty::json& Obj) const;

    void writeToStreamAsJSON(std::ostream& OutStm,bool WithErrors) const;


  public:

    WareRegistrySerializer(const WareRegistry<SignatureType>* Registry):
      m_Registry(Registry)
    {

    }

    void writeToStream(std::ostream& OutStm,const std::string& Format,bool Detailed, bool WithErrors) const;
 
};


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::writeListToStreamAsText(std::ostream& OutStm,
                                                                                        bool WithErrors) const
{
  for (auto& C : m_Registry->availableWares())
  {
    if (C.second.isValid() && C.second.hasSignature())
    {
      OutStm << C.second.signature()->ID << "\n";
    }
  }

  if (WithErrors && ! m_Registry->erroredWares().empty())
  {
    for (auto& C : m_Registry->erroredWares())
    {
      OutStm << "Error on file " << C.first <<  ": " << C.second.getMessage() << "\n";
    }
  }
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addWareMetaForText(
  const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const
{  
  OutStm << getIndentedText(2,"Domain",openfluid::tools::replaceEmptyString(Sign->Domain,"(unknown)")) << "\n";
  OutStm << getIndentedText(2,"Process",openfluid::tools::replaceEmptyString(Sign->Process,"(unknown)")) << "\n";
  OutStm << getIndentedText(2,"Method",openfluid::tools::replaceEmptyString(Sign->Method,"(unknown)")) << "\n";  
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addWareMetaForText(
  const openfluid::ware::ObserverSignature* /*Sign*/, std::ostream& /*OutStm*/) const
{
  // nothing to be done  
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addSchedulingDetailsForText(
  const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const
{
  std::ostringstream oss;

  if (Sign->TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT)
  {
    oss << "fixed to default deltaT";
  }
  else if (Sign->TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED)
  {
    oss << "fixed to " <<  Sign->TimeScheduling.Min << " seconds";
  }
  else if (Sign->TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
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


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addDataForText(
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


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addSpatialDataForText(
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


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addTypedSpatialDataForText(
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


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addDataDetailsForText(
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


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addGraphDetailsForText(
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


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addWareDynaForText(
  const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const
{
  addSchedulingDetailsForText(Sign,OutStm);
  addDataDetailsForText(Sign,OutStm);
  addGraphDetailsForText(Sign,OutStm);
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addWareDynaForText(
  const openfluid::ware::ObserverSignature* /*Sign*/, std::ostream& /*OutStm*/) const
{
  // nothing to be done  
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::writeToStreamAsText(std::ostream& OutStm,bool WithErrors) const
{
  bool First = true;

  for (auto& C : m_Registry->availableWares())
  {
    if (C.second.isValid() && C.second.hasSignature())
    {
      if (!First)
      {
        OutStm << "================================================================================\n";
      }
      First = false;

      const auto Sign = C.second.signature().get();

      OutStm << getIndentedText(1,Sign->ID) << "\n";
      OutStm << getIndentedText(2,"Name",openfluid::tools::replaceEmptyString(Sign->Name,"(none)")) << "\n";
      OutStm << getIndentedText(2,"File",C.second.getPath()) << "\n";
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
        getIndentedText(3,"Compilation flags",
                        openfluid::tools::replaceEmptyString(Sign->BuildInfo.CompilationFlags,"(none specified)")) << 
        "\n";
      OutStm << 
        getIndentedText(2,"Development status",openfluid::ware::WareSignature::getStatusAsString(Sign->Status)) << "\n";
      OutStm << getIndentedText(2,"Author(s)",getAuthorsAsString(Sign->Authors)) << "\n";
      addWareDynaForText(Sign,OutStm);
    }
  }

  if (WithErrors && !m_Registry->erroredWares().empty())
  {
    if (!First)
    {
      OutStm << "================================================================================\n";
    }
    First = false;

    for (auto& EFile : m_Registry->erroredWares())
    {
      
      OutStm << "Error on file " << EFile.first <<  ": " << EFile.second.getMessage() << "\n";
    }
  }
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::
  addErrorsToJSONDoc(openfluid::thirdparty::json& Doc) const
{
  openfluid::thirdparty::json Errors(openfluid::thirdparty::json::value_t::array);

  for (auto& EFile : m_Registry->erroredWares())
  {
    openfluid::thirdparty::json EObj(openfluid::thirdparty::json::value_t::object);
    EObj["file_path"] = EFile.first;
    EObj["message"] = EFile.second.getMessage();
    Errors.push_back(EObj);
  }
  
  Doc["errors"] = Errors;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string WareRegistrySerializer<SignatureType>::getJSONAsString(openfluid::thirdparty::json& Doc)
{
  return Doc.dump(2);
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::writeListToStreamAsJSON(std::ostream& OutStm, bool WithErrors) const
{
  openfluid::thirdparty::json JSON(openfluid::thirdparty::json::value_t::object);

  openfluid::thirdparty::json Available(openfluid::thirdparty::json::value_t::array);

  for (auto& C : m_Registry->availableWares())
  {
    if (C.second.isValid() && C.second.hasSignature())
    {
      Available.push_back(C.second.signature()->ID);
    }
  }

  JSON["available"] = Available;

  if (WithErrors)
  {
    addErrorsToJSONDoc(JSON);
  }


  OutStm << getJSONAsString(JSON) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addDataForJSON(
  const openfluid::ware::SignatureDataItem& Item, openfluid::thirdparty::json& Arr) const
{
   openfluid::thirdparty::json Obj(openfluid::thirdparty::json::value_t::object);
   Obj["name"] = Item.DataName;
   Obj["description"] = Item.Description;
   Obj["si_unit"] = Item.DataUnit;
   Arr.push_back(Obj);
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addSpatialDataForJSON(
  const openfluid::ware::SignatureSpatialDataItem& Item, openfluid::thirdparty::json& Arr) const
{
  openfluid::thirdparty::json Obj(openfluid::thirdparty::json::value_t::object);
  Obj["name"] = Item.DataName;
  Obj["units_class"] = Item.UnitsClass;
  Obj["description"] = Item.Description;
  Obj["si_unit"] = Item.DataUnit;
  Arr.push_back(Obj);
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addTypedSpatialDataForJSON(
  const openfluid::ware::SignatureTypedSpatialDataItem& Item, openfluid::thirdparty::json& Arr) const
{
  std::string TypeStr = openfluid::core::Value::getStringFromValueType(Item.DataType);

  openfluid::thirdparty::json Obj(openfluid::thirdparty::json::value_t::object);
  Obj["name"] = Item.DataName;
  Obj["type"] = TypeStr;
  Obj["units_class"] = Item.UnitsClass;
  Obj["description"] = Item.Description;
  Obj["si_unit"] = Item.DataUnit;
  Arr.push_back(Obj);
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addDataDetailsForJSON(
  const openfluid::ware::SimulatorSignature* Sign, openfluid::thirdparty::json& Obj) const
{
  // ------ Parameters

  openfluid::thirdparty::json ParamsObj(openfluid::thirdparty::json::value_t::object);

  openfluid::thirdparty::json  ReqParamsArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.RequiredParams)
  {
    addDataForJSON(Item,ReqParamsArray);
  }

  openfluid::thirdparty::json  UsedParamsArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.UsedParams)
  {
    addDataForJSON(Item,UsedParamsArray);
  }

  ParamsObj["required"] = ReqParamsArray;
  ParamsObj["used"] = UsedParamsArray;
  Obj["parameters"] = ParamsObj;


  // ------ Attributes

  openfluid::thirdparty::json AttrsObj(openfluid::thirdparty::json::value_t::object);

  openfluid::thirdparty::json  ReqAttrsArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.RequiredAttribute)
  {
    addSpatialDataForJSON(Item,ReqAttrsArray);
  } 

  openfluid::thirdparty::json  UsedAttrsArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.UsedAttribute)
  {
    addSpatialDataForJSON(Item,UsedAttrsArray);
  } 

  openfluid::thirdparty::json  ProdAttrsArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.ProducedAttribute)
  {
    addSpatialDataForJSON(Item,ProdAttrsArray);
  } 

  AttrsObj["required"] = ReqAttrsArray;
  AttrsObj["used"] = UsedAttrsArray;
  AttrsObj["produced"] = ProdAttrsArray;
  Obj["attributes"] = AttrsObj;


  // ------ Variables

  openfluid::thirdparty::json VarsObj(openfluid::thirdparty::json::value_t::object);

  openfluid::thirdparty::json  ReqVarsArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.RequiredVars)
  {
    addTypedSpatialDataForJSON(Item,ReqVarsArray);
  } 

  openfluid::thirdparty::json  UsedVarsArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.UsedVars)
  {
    addTypedSpatialDataForJSON(Item,UsedVarsArray);
  } 

  openfluid::thirdparty::json  UpVarsArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.UpdatedVars)
  {
    addTypedSpatialDataForJSON(Item,UpVarsArray);
  } 

  openfluid::thirdparty::json  ProdVarsArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.ProducedVars)
  {
    addTypedSpatialDataForJSON(Item,ProdVarsArray);
  } 

  VarsObj["required"] = ReqVarsArray;
  VarsObj["used"] = UsedVarsArray;
  VarsObj["updated"] = UpVarsArray;
  VarsObj["produced"] = ProdVarsArray;
  Obj["variables"] = VarsObj;


  // ------ Events

  openfluid::thirdparty::json  EvArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.UsedEventsOnUnits)
  {
    EvArray.push_back(Item);
  }
  Obj["events"] = EvArray;


  // ------ Extrafiles

  openfluid::thirdparty::json ExtraObj(openfluid::thirdparty::json::value_t::object);

  openfluid::thirdparty::json  ReqExtraArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.RequiredExtraFiles)
  {
    ReqExtraArray.push_back(Item);
  }

  openfluid::thirdparty::json  UsedExtraArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Item : Sign->HandledData.UsedExtraFiles)
  {
    UsedExtraArray.push_back(Item);
  }

  ExtraObj["required"] = ReqExtraArray;
  ExtraObj["used"] = UsedExtraArray;
  Obj["extra_files"] = ExtraObj;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addGraphDetailsForJSON(
  const openfluid::ware::SimulatorSignature* Sign, openfluid::thirdparty::json& Obj) const
{
  Obj["description"] = Sign->HandledUnitsGraph.UpdatedUnitsGraph;

  openfluid::thirdparty::json ClassesArray(openfluid::thirdparty::json::value_t::array);
  for (const auto& Class : Sign->HandledUnitsGraph.UpdatedUnitsClass)
  {
    openfluid::thirdparty::json ClassObj(openfluid::thirdparty::json::value_t::object);
    ClassObj["class_name"] = Class.UnitsClass;
    ClassObj["description"] = Class.Description;
    ClassesArray.push_back(ClassObj);
  }
  Obj["units_classses"] = ClassesArray;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addWareDetailsForJSON(
  const openfluid::ware::SimulatorSignature* Sign, openfluid::thirdparty::json& Obj) const
{
  Obj["domain"] = Sign->Domain;
  Obj["process"] = Sign->Process;
  Obj["method"] = Sign->Method;


  openfluid::thirdparty::json DataObj(openfluid::thirdparty::json::value_t::object);
  addDataDetailsForJSON(Sign,DataObj);
  Obj["data"] = DataObj;


  openfluid::thirdparty::json SchedObj(openfluid::thirdparty::json::value_t::object);
  SchedObj["type"] = getSchedulingTypeAsString(Sign->TimeScheduling.Type);
  SchedObj["min"] = Sign->TimeScheduling.Min;
  SchedObj["max"] = Sign->TimeScheduling.Max;
  Obj["scheduling"] = SchedObj;


  openfluid::thirdparty::json GraphObj(openfluid::thirdparty::json::value_t::object);
  addGraphDetailsForJSON(Sign,GraphObj);
  Obj["spatial_graph"] = GraphObj;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addWareDetailsForJSON(
  const openfluid::ware::ObserverSignature* /*Sign*/, openfluid::thirdparty::json& /*Obj*/) const
{
  // nothing to be done
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::writeToStreamAsJSON(std::ostream& OutStm,
                                                                        bool WithErrors) const
{
  openfluid::thirdparty::json JSON(openfluid::thirdparty::json::value_t::object);

  openfluid::thirdparty::json Available(openfluid::thirdparty::json::value_t::array);

  for (auto& C : m_Registry->availableWares())
  {
    if (C.second.isValid() && C.second.hasSignature())
    {
      const auto Sign = C.second.signature().get();

      openfluid::thirdparty::json WareObj(openfluid::thirdparty::json::value_t::object);
      WareObj["id"] = Sign->ID;
      WareObj["file_path"] = C.second.getPath();
      WareObj["abi_version"] = Sign->BuildInfo.SDKVersion;

      openfluid::thirdparty::json BuildObj(openfluid::thirdparty::json::value_t::object);
      BuildObj["type"] = Sign->BuildInfo.BuildType;
      BuildObj["compiler_id"] = Sign->BuildInfo.CompilerID;
      BuildObj["compiler_version"] = Sign->BuildInfo.CompilerVersion;
      BuildObj["compilation_flags"] = Sign->BuildInfo.CompilationFlags;
      WareObj["build_info"] = BuildObj;

      WareObj["name"] = Sign->Name;
      WareObj["description"] = Sign->Description; 
      WareObj["version"] = Sign->Version;
      WareObj["status"] = openfluid::ware::WareSignature::getStatusAsString(Sign->Status);

      openfluid::thirdparty::json AuthsArray(openfluid::thirdparty::json::value_t::array);
      for (auto& Author : Sign->Authors)
      {
        openfluid::thirdparty::json AuthObj(openfluid::thirdparty::json::value_t::object);
        AuthObj["name"] = Author.first;
        AuthObj["email"] = Author.second;
        AuthsArray.push_back(AuthObj);
      }
      WareObj["authors"] = AuthsArray;

      addWareDetailsForJSON(Sign,WareObj);

      Available.push_back(WareObj);
    }
  }

  JSON["available"] = Available;


  if (WithErrors)
  {
    addErrorsToJSONDoc(JSON);
  }


  OutStm << getJSONAsString(JSON) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::writeToStream(std::ostream& OutStm,
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


#endif /* __OPENFLUID_MACHINE_WAREREGISTRYSERIALIZER_HPP__ */
