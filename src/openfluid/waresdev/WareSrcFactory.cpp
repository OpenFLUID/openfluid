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
  @file WareSrcFactory.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
 */


#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace waresdev {


openfluid::tools::Path getTemplateSkeletonPath(const std::string& Type)
{
  return openfluid::tools::Path({openfluid::base::Environment::getInstallPrefix(),
                                 openfluid::config::SHARE_WARESDEV_INSTALL_PATH,
                                 "templates",Type,"skeleton"});
}


// =====================================================================
// =====================================================================


openfluid::tools::Path getTemplateFilePath(const std::string& Type, const std::string& Filename)
{
  return openfluid::tools::Path({openfluid::base::Environment::getInstallPrefix(),
                                 openfluid::config::SHARE_WARESDEV_INSTALL_PATH,
                                 "templates",Type,"files",Filename});
}


// =====================================================================
// =====================================================================


openfluid::tools::TemplateProcessor::Data 
  WareSrcFactory::prepareCommonTemplateData(const openfluid::ware::WareSignature* Signature,
                                            const Configuration& Config)
{
  openfluid::tools::TemplateProcessor::Data Data;

  Data["WAREID"] = Signature->ID;
  Data["WARETYPE"] = openfluid::ware::stringifyWareType(Signature->getType());

  Data["WAREDESCRIPTION"] = Signature->Description;

  Data["CLASSNAME"] = Config.MainClassName;

  Data["WARELINKUID"] = openfluid::tools::generatePseudoUniqueIdentifier(16);

  Data["SIGNATUREINFOS"] = getSignatureInfos(Signature);

  Data["PARAMSUIENABLED"] = Config.WithParamsUI ? "ON" : "OFF";
  Data["PARAMSUIROOTCPPFILENAME"] = "";

  if (Config.WithParamsUI)
  {
    Data["PARAMSUICLASSNAME"] = Config.ParamsUIClassName;
    Data["PARAMSUIROOTCPPFILENAME"] = "WareUI.cpp";
  }

  return Data;
}


// =====================================================================
// =====================================================================


openfluid::tools::TemplateProcessor::Data 
WareSrcFactory::prepareTemplateData(const openfluid::ware::SimulatorSignature& Signature,
                                    const Configuration& Config)
{
  auto Data = prepareCommonTemplateData(static_cast<const openfluid::ware::WareSignature*>(&Signature),Config);

  Data["SIMSIGNATUREDATA"] = getSimulatorSignatureData(Signature);

  Data["SIMSCHEDRETURN"] = getSimulatorSchedulingReturn(Signature);
  Data["SIMINITCODE"] = getSimulatorInitCode(Signature);
  Data["SIMRUNCODE"] = getSimulatorRunCode(Signature);

  return Data;
}


// =====================================================================
// =====================================================================


openfluid::tools::TemplateProcessor::Data 
WareSrcFactory::prepareTemplateData(const openfluid::ware::ObserverSignature& Signature,const Configuration& Config)
{
  auto Data = prepareCommonTemplateData(static_cast<const openfluid::ware::WareSignature*>(&Signature),Config);

  return Data;
}


// =====================================================================
// =====================================================================


openfluid::tools::TemplateProcessor::Data 
WareSrcFactory::prepareTemplateData(const openfluid::builderext::BuilderExtensionSignature& Signature,
                                   const Configuration& Config)
{
  auto Data = prepareCommonTemplateData(static_cast<const openfluid::ware::WareSignature*>(&Signature),Config);

  Data["BUILDEREXTCATEGORY"] = "openfluid::builderext::ExtensionCategory::OTHER";
  if (Signature.Category == openfluid::builderext::ExtensionCategory::SPATIAL)
  {
    Data["BUILDEREXTCATEGORY"] = "openfluid::builderext::ExtensionCategory::SPATIAL";
  }
  if (Signature.Category == openfluid::builderext::ExtensionCategory::MODEL)
  {
    Data["BUILDEREXTCATEGORY"] = "openfluid::builderext::ExtensionCategory::MODEL";
  }
  if (Signature.Category == openfluid::builderext::ExtensionCategory::RESULTS)
  {
    Data["BUILDEREXTCATEGORY"] = "openfluid::builderext::ExtensionCategory::RESULTS";
  }

  Data["BUILDEREXTMENUTEXT"] = Signature.MenuText;

  return Data;
}


// =====================================================================
// =====================================================================


void renderParamsUIFiles(const std::string& ParentPath, const openfluid::ware::WareID_t& ID,
                         const openfluid::tools::TemplateProcessor::Data& Data)
{
  openfluid::tools::TemplateProcessor TplProc("%%","%%");
  openfluid::tools::TemplateProcessor::Errors Errors;

  for (const std::string& Ext : {".cpp",".hpp"})
  {
    auto CommonFile =  openfluid::tools::Path({openfluid::base::Environment::getInstallPrefix(),
                                                openfluid::config::SHARE_WARESDEV_INSTALL_PATH,
                                                "templates","common","files","WareUI"+Ext});

    auto WareSrcSrcFile = openfluid::tools::Path({ParentPath,ID,"src","WareUI"+Ext});

    TplProc.renderFile(CommonFile.toGeneric(),WareSrcSrcFile.toGeneric(),Data,Errors);
  }
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::createSimulator(const openfluid::ware::SimulatorSignature& Signature,
                                            const Configuration& Config,
                                            const std::string& ParentPath)
{
  openfluid::base::Environment::init();

  openfluid::tools::TemplateProcessor TplProc("%%","%%");
  openfluid::tools::TemplateProcessor::Errors Errors;

  auto Data = prepareTemplateData(Signature,Config);
  
  auto CommonSkelPath = getTemplateSkeletonPath("common");
  auto SkelPath =  getTemplateSkeletonPath("simulator");

  auto WareSrcDir = openfluid::tools::Path({ParentPath,Signature.ID});

  try 
  {
    TplProc.renderDirectory(CommonSkelPath.toGeneric(),WareSrcDir.toGeneric(),Data,Errors);
    TplProc.renderDirectory(SkelPath.toGeneric(),WareSrcDir.toGeneric(),Data,Errors);

    if (Config.WithParamsUI)
    {
     renderParamsUIFiles(ParentPath,Signature.ID,Data);
    }
  }
  catch(openfluid::base::FrameworkException& E)
  {
    return std::string();
  }

  return WareSrcDir.toGeneric(); 
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::createObserver(const openfluid::ware::ObserverSignature& Signature, 
                                           const Configuration& Config,
                                           const std::string& ParentPath)
{
  openfluid::base::Environment::init();

  openfluid::tools::TemplateProcessor TplProc("%%","%%");
  openfluid::tools::TemplateProcessor::Errors Errors;

  auto Data = prepareTemplateData(Signature,Config);
  

  auto CommonSkelPath = getTemplateSkeletonPath("common");
  auto SkelPath =  getTemplateSkeletonPath("observer");

  auto WareSrcDir = openfluid::tools::Path({ParentPath,Signature.ID});

  try 
  {
    TplProc.renderDirectory(CommonSkelPath.toGeneric(),WareSrcDir.toGeneric(),Data,Errors);
    TplProc.renderDirectory(SkelPath.toGeneric(),WareSrcDir.toGeneric(),Data,Errors);

    if (Config.WithParamsUI)
    {
     renderParamsUIFiles(ParentPath,Signature.ID,Data);
    }
  }
  catch(openfluid::base::FrameworkException& E)
  {
    return std::string();
  }

  return WareSrcDir.toGeneric();
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::createBuilderext(const openfluid::builderext::BuilderExtensionSignature& Signature, 
                                             const Configuration& Config,
                                             const std::string& ParentPath)
{
  openfluid::base::Environment::init();

  openfluid::tools::TemplateProcessor TplProc("%%","%%");
  openfluid::tools::TemplateProcessor::Errors Errors;

  auto Data = prepareTemplateData(Signature,Config);
  

  auto CommonSkelPath = getTemplateSkeletonPath("common");
  auto SkelPath =  getTemplateSkeletonPath("builderext");

  auto WareSrcDir = openfluid::tools::Path({ParentPath,Signature.ID});

  std::string ModeSuffix;
  if (Signature.Mode == openfluid::builderext::ExtensionMode::MODAL)
  {
    ModeSuffix = "modal";
  }
  else if (Signature.Mode == openfluid::builderext::ExtensionMode::MODELESS)
  {
    ModeSuffix = "modeless";
  }
  else if (Signature.Mode == openfluid::builderext::ExtensionMode::WORKSPACE)
  {
    ModeSuffix = "workspace";
  }

  try 
  {
    TplProc.renderDirectory(CommonSkelPath.toGeneric(),WareSrcDir.toGeneric(),Data,Errors);
    TplProc.renderDirectory(SkelPath.toGeneric(),WareSrcDir.toGeneric(),Data,Errors);

    for (const std::string& Ext : {".cpp",".hpp"})
    {
      auto CommonFile =  openfluid::tools::Path({openfluid::base::Environment::getInstallPrefix(),
                                                  openfluid::config::SHARE_WARESDEV_INSTALL_PATH,
                                                  "templates","builderext","files","WareMain_"+ModeSuffix+Ext});

      auto WareSrcSrcFile = openfluid::tools::Path({ParentPath,Signature.ID,"src","WareMain"+Ext});

      TplProc.renderFile(CommonFile.toGeneric(),WareSrcSrcFile.toGeneric(),Data,Errors);
    }
  }
  catch(openfluid::base::FrameworkException& E)
  {
    return std::string();
  }

  return WareSrcDir.toGeneric();
}


// =====================================================================
// =====================================================================


std::string getDeclarationStringFromVarType(openfluid::core::Value::Type VarType)
{
  if (VarType == openfluid::core::Value::NONE)
  {
    return "";
  }
  else
  {
    return "["+openfluid::core::Value::getStringFromValueType(VarType)+"]";
  }
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::getSignatureInfos(const openfluid::ware::WareSignature* Signature)
{
  std::string TmpStr;

  TmpStr += "  // Informations\n";
  TmpStr += "  DECLARE_NAME(\""+Signature->Name+"\")\n";
  TmpStr += "  DECLARE_DESCRIPTION(\""+Signature->Description+"\")\n";
  TmpStr += "  DECLARE_VERSION(\""+Signature->Version+"\")\n";

  TmpStr += "  DECLARE_STATUS(";
  if (Signature->Status == openfluid::ware::EXPERIMENTAL)
  {
    TmpStr += "openfluid::ware::EXPERIMENTAL";
  }
  else if (Signature->Status == openfluid::ware::BETA)
  {
    TmpStr += "openfluid::ware::BETA";
  }
  else
  {
    TmpStr += "openfluid::ware::STABLE";
  }
  TmpStr += ")\n";

  for (auto& Item : Signature->Authors)
  {
    TmpStr += "  DECLARE_AUTHOR(\""+Item.first+"\",\""+Item.second+"\")\n";
  }

  return TmpStr;
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::getSimulatorSignatureData(const openfluid::ware::SimulatorSignature& Signature)
{
  std::string TmpStr;

  // TODO replace indentation size with value from user's preferences

  if (Signature.HandledData.RequiredParams.size() + Signature.HandledData.UsedParams.size())
  {
    TmpStr += "\n  // Parameters\n";
  }

  // -- Parameters
  for (auto& Item : Signature.HandledData.RequiredParams)
  {
    TmpStr += "  DECLARE_REQUIRED_PARAMETER(\""+Item.DataName+"\","
                                           "\""+Item.Description+"\","
                                           "\""+Item.DataUnit+"\")\n";
  }

  for (auto& Item : Signature.HandledData.UsedParams)
  {
    TmpStr += "  DECLARE_USED_PARAMETER(\""+Item.DataName+"\","
                                       "\""+Item.Description+"\","
                                       "\""+Item.DataUnit+"\")\n";
  }

  // -- Extra files

  if (Signature.HandledData.RequiredExtraFiles.size() + Signature.HandledData.UsedExtraFiles.size())
  {
    TmpStr += "\n  // Extra files\n";
  }

  for (auto& Item : Signature.HandledData.RequiredExtraFiles)
  {
    TmpStr += "  DECLARE_REQUIRED_EXTRAFILE(\""+Item+"\")\n";
  }

  for (auto& Item : Signature.HandledData.UsedExtraFiles)
  {
    TmpStr += "  DECLARE_USED_EXTRAFILE(\""+Item+"\")\n";
  }


  // -- Attributes

  if (Signature.HandledData.RequiredAttribute.size() + Signature.HandledData.UsedAttribute.size() +
      Signature.HandledData.ProducedAttribute.size())
  {
    TmpStr += "\n  // Attributes\n";
  }

  for (auto& Item : Signature.HandledData.RequiredAttribute)
  {
    TmpStr += "  DECLARE_REQUIRED_ATTRIBUTE(\""+Item.DataName+"\","
                                           "\""+Item.UnitsClass+"\","
                                           "\""+Item.Description+"\","
                                           "\""+Item.DataUnit+"\")\n";
  }

  for (auto& Item : Signature.HandledData.UsedAttribute)
  {
    TmpStr += "  DECLARE_USED_ATTRIBUTE(\""+Item.DataName+"\","
                                       "\""+Item.UnitsClass+"\","
                                       "\""+Item.Description+"\","
                                       "\""+Item.DataUnit+"\")\n";
  }

  for (auto& Item : Signature.HandledData.ProducedAttribute)
  {
    TmpStr += "  DECLARE_PRODUCED_ATTRIBUTE(\""+Item.DataName+"\","
                                           "\""+Item.UnitsClass+"\","
                                           "\""+Item.Description+"\","
                                           "\""+Item.DataUnit+"\")\n";
  }


  // -- Events

  if (Signature.HandledData.UsedEventsOnUnits.size())
  {
    TmpStr += "\n  // Events\n";
  }

  for (auto& Item : Signature.HandledData.UsedEventsOnUnits)
  {
    TmpStr += "  DECLARE_USED_EVENTS(\""+Item+"\")\n";
  }


  // -- Variables

  if (Signature.HandledData.RequiredVars.size() + Signature.HandledData.UsedVars.size() +
      Signature.HandledData.ProducedVars.size() + Signature.HandledData.UpdatedVars.size())
  {
    TmpStr += "\n  // Variables\n";
  }


  for (auto& Item : Signature.HandledData.RequiredVars)
  {
    TmpStr += "  DECLARE_REQUIRED_VARIABLE(\""+Item.DataName+getDeclarationStringFromVarType(Item.DataType)+"\","
                                          "\""+Item.UnitsClass+"\","
                                          "\""+Item.Description+"\","
                                          "\""+Item.DataUnit+"\")\n";
  }

  for (auto& Item : Signature.HandledData.UsedVars)
  {
    TmpStr += "  DECLARE_USED_VARIABLE(\""+Item.DataName+getDeclarationStringFromVarType(Item.DataType)+"\","
                                          "\""+Item.UnitsClass+"\","
                                          "\""+Item.Description+"\","
                                          "\""+Item.DataUnit+"\")\n";
  }

  for (auto& Item : Signature.HandledData.ProducedVars)
  {
    TmpStr += "  DECLARE_PRODUCED_VARIABLE(\""+Item.DataName+getDeclarationStringFromVarType(Item.DataType)+"\","
                                          "\""+Item.UnitsClass+"\","
                                          "\""+Item.Description+"\","
                                          "\""+Item.DataUnit+"\")\n";
  }

  for (auto& Item : Signature.HandledData.UpdatedVars)
  {
    TmpStr += "  DECLARE_UPDATED_VARIABLE(\""+Item.DataName+getDeclarationStringFromVarType(Item.DataType)+"\","
                                          "\""+Item.UnitsClass+"\","
                                          "\""+Item.Description+"\","
                                          "\""+Item.DataUnit+"\")\n";
  }

  // -- Spatial graph

  if (Signature.HandledUnitsGraph.UpdatedUnitsClass.size() + Signature.HandledUnitsGraph.UpdatedUnitsGraph.size())
  {
    TmpStr += "\n  // Spatial graph\n";
  }

  if (!Signature.HandledUnitsGraph.UpdatedUnitsGraph.empty())
  {
    TmpStr += "  DECLARE_UPDATED_UNITSGRAPH(\""+Signature.HandledUnitsGraph.UpdatedUnitsGraph+"\")\n";
  }

  for (auto& Item : Signature.HandledUnitsGraph.UpdatedUnitsClass)
  {
    TmpStr += "  DECLARE_UPDATED_UNITSCLASS(\""+Item.UnitsClass+"\",\""+Item.Description+"\")\n";
  }


  // -- Scheduling

  TmpStr += "\n  // Scheduling\n";

  if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::DEFAULT)
  {
    TmpStr += "  DECLARE_SCHEDULING_DEFAULT\n";
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED)
  {
    std::string TmpValueStr = std::to_string(Signature.TimeScheduling.Min);
    TmpStr += "  DECLARE_SCHEDULING_FIXED("+TmpValueStr+")\n";
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
  {
    std::string TmpValueStr = std::to_string(Signature.TimeScheduling.Min) + "," +
                              std::to_string(Signature.TimeScheduling.Max);
    TmpStr += "  DECLARE_SCHEDULING_RANGE("+TmpValueStr+")\n";
  }
  else
  {
    TmpStr += "  DECLARE_SCHEDULING_UNDEFINED\n";
  }

  return TmpStr;
}


// =====================================================================
// =====================================================================


std::string getSimulatorCommonCode(const openfluid::ware::SimulatorSignature& Signature,
                                   const std::string& PrimitiveStr)
{
  std::string TmpStr;
  std::set<openfluid::core::UnitsClass_t> UnitsClasses;

  for (auto& Item : Signature.HandledData.ProducedVars)
  {
    UnitsClasses.insert(Item.UnitsClass);
  }

  if (!UnitsClasses.empty())
  {
    // TODO replace indentation size with value from user's preferences

    TmpStr += "      // -- beginning of automatically generated code --\n";
    TmpStr += "      openfluid::core::SpatialUnit* U;\n";

    for (auto& UClass : UnitsClasses)
    {
      TmpStr += "\n";
      TmpStr += "      OPENFLUID_UNITS_ORDERED_LOOP(\""+UClass+"\",U)\n";
      TmpStr += "      {\n";

      for (auto& Item : Signature.HandledData.ProducedVars)
      {
        if (Item.UnitsClass == UClass)
        {
          std::string TmpValueStr = "0.0";
          if (Item.DataType == openfluid::core::Value::BOOLEAN)
          {
            TmpValueStr = "openfluid::core::BooleanValue()";
          }
          else if (Item.DataType == openfluid::core::Value::INTEGER)
          {
            TmpValueStr = "openfluid::core::IntegerValue(0)";
          }
          else if (Item.DataType == openfluid::core::Value::STRING)
          {
            TmpValueStr = "openfluid::core::StringValue()";
          }
          else if (Item.DataType == openfluid::core::Value::MAP)
          {
            TmpValueStr = "openfluid::core::MapValue()";
          }
          else if (Item.DataType == openfluid::core::Value::VECTOR)
          {
            TmpValueStr = "openfluid::core::VectorValue(5,0.0)";
          }
          else if (Item.DataType == openfluid::core::Value::MATRIX)
          {
            TmpValueStr = "openfluid::core::MatrixValue(5,5,0.0)";
          }
          else if (Item.DataType == openfluid::core::Value::TREE)
          {
            TmpValueStr = "openfluid::core::TreeValue()";
          }
          else if (Item.DataType == openfluid::core::Value::NULLL)
          {
            TmpValueStr = "openfluid::core::NullValue()";
          }

          TmpStr += "        "+PrimitiveStr+"(U,\""+Item.DataName+"\","+TmpValueStr+");\n";
        }
      }
      TmpStr += "      }\n";
    }
    TmpStr += "      // -- end of automatically generated code --\n";
  }

  return TmpStr;
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::getSimulatorInitCode(const openfluid::ware::SimulatorSignature& Signature)
{
  return getSimulatorCommonCode(Signature,"OPENFLUID_InitializeVariable");
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::getSimulatorRunCode(const openfluid::ware::SimulatorSignature& Signature)
{
  return getSimulatorCommonCode(Signature,"OPENFLUID_AppendVariable");
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::getSimulatorSchedulingReturn(const openfluid::ware::SimulatorSignature& Signature)
{
  std::string TmpStr = "DefaultDeltaT()";

  if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::FIXED ||
      Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::SchedulingType::RANGE)
  {
    TmpStr = "Duration("+std::to_string(Signature.TimeScheduling.Min)+");";
  }

  return TmpStr;
}


} }  // namespaces
