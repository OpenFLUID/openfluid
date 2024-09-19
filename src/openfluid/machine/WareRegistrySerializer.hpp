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
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/waresdev/ObserverSignatureSerializer.hpp>
#include <openfluid/thirdparty/JSON.hpp>


namespace openfluid { namespace machine {


/**
  Class for serialization fo wares registries
  @tparam SignatureType class defining the container for ware signature only
*/
template<class SignatureType>
class OPENFLUID_API WareRegistrySerializer
{
  private:

    const WareRegistry<SignatureType>* m_Registry;


    static std::string getAuthorsAsString(const openfluid::ware::WareSignature::PeopleList_t& Authors)
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

    void addTypedSpatialDataForText(const openfluid::ware::SignatureSpatialDataItem& Item, 
                                    const std::string& CatStr, std::ostream& OutStm) const;

    void addDataDetailsForText(const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const;
    
    void addGraphDetailsForText(const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const;

    void addWareDynaForText(const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const;

    void addWareDynaForText(const openfluid::ware::ObserverSignature* Sign, std::ostream& OutStm) const;

    void writeToStreamAsText(std::ostream& OutStm,bool WithErrors) const;

    void addErrorsToJSONDoc(openfluid::thirdparty::json& Doc) const;

    static std::string getJSONAsString(openfluid::thirdparty::json& Doc);

    void writeListToStreamAsJSON(std::ostream& OutStm,bool WithErrors) const;

    openfluid::thirdparty::json initWareObjectForJSON(const openfluid::ware::SimulatorSignature* Sign) const;

    openfluid::thirdparty::json initWareObjectForJSON(const openfluid::ware::ObserverSignature* Sign) const;

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
  if (!Item.SIUnit.empty())
  {
    UnitStr = " ("+Item.SIUnit+")";
  }

  oss << CatStr << ". " << Item.Description << UnitStr;
  OutStm << getIndentedText(3,Item.Name,oss.str()) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addSpatialDataForText(
  const openfluid::ware::SignatureSpatialDataItem& Item, const std::string& CatStr, std::ostream& OutStm) const
{
  std::ostringstream oss;

  std::string UnitStr;
  if (!Item.SIUnit.empty())
  {
    UnitStr = " ("+Item.SIUnit+")";
  }

  oss << CatStr << ". " << Item.Description << UnitStr;
  OutStm << getIndentedText(3,Item.Name+" {"+Item.UnitsClass+"}",oss.str()) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addTypedSpatialDataForText(
  const openfluid::ware::SignatureSpatialDataItem& Item, const std::string& CatStr, std::ostream& OutStm) const
{
  std::ostringstream oss;

  std::string UnitStr;
  if (!Item.SIUnit.empty())
  {
    UnitStr = " ("+Item.SIUnit+")";
  }

  std::string TypeStr;
  if (Item.DataType != openfluid::core::Value::NONE)
  {
    TypeStr = " ["+openfluid::core::Value::getStringFromValueType(Item.DataType)+"]";
  }

  oss << CatStr << ". " << Item.Description << UnitStr;
  OutStm << getIndentedText(3,Item.Name+" {"+Item.UnitsClass+"}"+TypeStr,oss.str()) << "\n";
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void WareRegistrySerializer<SignatureType>::addDataDetailsForText(
  const openfluid::ware::SimulatorSignature* Sign, std::ostream& OutStm) const
{
  const auto& Data = Sign->HandledData;
  const auto& SimData = Sign->SimulatorHandledData;

  if (!Data.UsedParams.empty() || !Data.RequiredParams.empty() ||
      !SimData.ProducedVars.empty() || !SimData.UpdatedVars.empty() || 
      !SimData.RequiredVars.empty() || !SimData.UsedVars.empty() ||
      !SimData.ProducedAttribute.empty() || !SimData.RequiredAttribute.empty() || !SimData.UsedAttribute.empty() ||
      !SimData.RequiredExtraFiles.empty() || !SimData.UsedExtraFiles.empty() ||
      !SimData.UsedEventsOnUnits.empty())
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

    for (const auto& Item : SimData.RequiredAttribute)
    {
      addSpatialDataForText(Item,"required attribute",OutStm);
    } 

    for (const auto& Item : SimData.UsedAttribute)
    {
      addSpatialDataForText(Item,"used attribute",OutStm);
    } 

    for (const auto& Item : SimData.ProducedAttribute)
    {
      addSpatialDataForText(Item,"produced attribute",OutStm);
    } 


    // ------ Variables

    for (const auto& Item : SimData.RequiredVars)
    {
      addTypedSpatialDataForText(Item,"required variable",OutStm);
    } 

    for (const auto& Item : SimData.UsedVars)
    {
      addTypedSpatialDataForText(Item,"used variable",OutStm);
    } 

    for (const auto& Item : SimData.UpdatedVars)
    {
      addTypedSpatialDataForText(Item,"updated variable",OutStm);
    } 

    for (const auto& Item : SimData.ProducedVars)
    {
      addTypedSpatialDataForText(Item,"produced variable",OutStm);
    } 


    // ------ Events
    if (!SimData.UsedEventsOnUnits.empty())
    {
      bool First = true;
      std::string EventsStr;

      for (const auto& Item : SimData.UsedEventsOnUnits)
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

    for (const auto& Item : SimData.RequiredExtraFiles)
    {
      OutStm << getIndentedText(3,"Required extra file",Item) << "\n";
    }

    for (const auto& Item : SimData.UsedExtraFiles)
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
        getIndentedText(2,"Development status",Sign->getStatusAsString()) << "\n";
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
openfluid::thirdparty::json WareRegistrySerializer<SignatureType>::initWareObjectForJSON(
  const openfluid::ware::SimulatorSignature* Sign) const
{
  return openfluid::waresdev::SimulatorSignatureSerializer().toJSON(*Sign);
}


// =====================================================================
// =====================================================================


template<class SignatureType>
openfluid::thirdparty::json WareRegistrySerializer<SignatureType>::initWareObjectForJSON(
  const openfluid::ware::ObserverSignature* Sign) const
{
  return openfluid::waresdev::ObserverSignatureSerializer().toJSON(*Sign);
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

      openfluid::thirdparty::json WareObj = initWareObjectForJSON(Sign);
      WareObj["file_path"] = C.second.getPath();
      WareObj["sdk_version"] = Sign->BuildInfo.SDKVersion;

      openfluid::thirdparty::json BuildObj(openfluid::thirdparty::json::value_t::object);
      BuildObj["type"] = Sign->BuildInfo.BuildType;
      BuildObj["compiler_id"] = Sign->BuildInfo.CompilerID;
      BuildObj["compiler_version"] = Sign->BuildInfo.CompilerVersion;
      BuildObj["compilation_flags"] = Sign->BuildInfo.CompilationFlags;
      WareObj["build_info"] = BuildObj;

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
