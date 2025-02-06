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
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include <openfluid/waresdev/ObserverSignatureSerializer.hpp>
#include <openfluid/waresdev/BuilderextSignatureSerializer.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/utils/InternalLogger.hpp>
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

  Data["CPPFILES"] = openfluid::config::WARESDEV_SRC_MAINFILE;
  Data["CLASSNAME"] = Config.MainClassName;

  Data["PUIENABLED"] = Config.WithParamsUI ? "ON" : "OFF";
  Data["PUICPPFILES"] = Config.WithParamsUI ? openfluid::config::WARESDEV_SRC_PARAMSUIFILE : "";

  if (Config.WithParamsUI)
  {
    Data["PARAMSUICLASSNAME"] = Config.ParamsUIClassName;
  }

  return Data;
}


// =====================================================================
// =====================================================================


void appendExtraData(openfluid::tools::TemplateProcessor::Data& Data, const WareSrcFactory::Configuration& Config)
{
  for (const auto& D : Config.ExtraTemplateData)
  {
    Data[D.first] = D.second;
  }
}


// =====================================================================
// =====================================================================


openfluid::tools::TemplateProcessor::Data 
WareSrcFactory::prepareTemplateData(const openfluid::ware::SimulatorSignature& Signature,
                                    const Configuration& Config)
{
  auto Data = prepareCommonTemplateData(static_cast<const openfluid::ware::WareSignature*>(&Signature),Config);

  Data["SIMSCHEDRETURN"] = getSimulatorSchedulingReturn(Signature);
  Data["SIMINITCODE"] = getSimulatorInitCode(Signature);
  Data["SIMRUNCODE"] = getSimulatorRunCode(Signature);

  appendExtraData(Data,Config);

  return Data;
}


// =====================================================================
// =====================================================================


openfluid::tools::TemplateProcessor::Data 
WareSrcFactory::prepareTemplateData(const openfluid::ware::ObserverSignature& Signature,const Configuration& Config)
{
  auto Data = prepareCommonTemplateData(static_cast<const openfluid::ware::WareSignature*>(&Signature),Config);

  appendExtraData(Data,Config);

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

  appendExtraData(Data,Config);

  return Data;
}


// =====================================================================
// =====================================================================


openfluid::tools::Path getSrcPathObject(const openfluid::ware::WareID_t ID, 
                                        const std::string& ParentPath, bool WithIDSubDir)
{
  if (WithIDSubDir)
  {
    return openfluid::tools::Path({ParentPath,ID});
  }

  return openfluid::tools::Path(ParentPath);
}


// =====================================================================
// =====================================================================


void renderParamsUIFiles(const openfluid::tools::FilesystemPath& PathObj,
                         const openfluid::tools::TemplateProcessor::Data& Data)
{
  openfluid::tools::TemplateProcessor TplProc("%%","%%");
  openfluid::tools::TemplateProcessor::Errors Errors;

  for (const std::string& Ext : {std::string(".cpp"), std::string(".hpp")})
  {
    auto CommonFile =  openfluid::tools::Path({openfluid::base::Environment::getInstallPrefix(),
                                                openfluid::config::SHARE_WARESDEV_INSTALL_PATH,
                                                "templates","common","files","WareUI"+Ext});

    auto WareSrcSrcFile = PathObj.fromThis({"src","WareUI"+Ext});

    TplProc.renderFile(CommonFile.toGeneric(),WareSrcSrcFile.toGeneric(),Data,Errors);
  }
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::createSimulator(const openfluid::ware::SimulatorSignature& Signature,
                                            const Configuration& Config,
                                            const std::string& ParentPath,
                                            bool WithIDSubDir)
{
  openfluid::base::Environment::init();

  openfluid::tools::TemplateProcessor TplProc("%%","%%");  
  openfluid::tools::TemplateProcessor::Errors Errors;

  TplProc.enableAutoComment("#");
  TplProc.ignoreUnknown(true);

  auto Data = prepareTemplateData(Signature,Config);
  
  auto CommonSkelPath = getTemplateSkeletonPath("common");
  auto SkelPath =  getTemplateSkeletonPath("simulator");

  auto WareSrcPathObj = getSrcPathObject(Signature.ID,ParentPath,WithIDSubDir);
  auto WareInfoFileObj = WareSrcPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE);

  try 
  {
    TplProc.renderDirectory(CommonSkelPath.toGeneric(),WareSrcPathObj.toGeneric(),Data,Errors);
    TplProc.renderDirectory(SkelPath.toGeneric(),WareSrcPathObj.toGeneric(),Data,Errors);

    SimulatorSignatureSerializer().writeToJSONFile(Signature,WareInfoFileObj.toGeneric());

    if (Config.WithParamsUI)
    {
     renderParamsUIFiles(WareSrcPathObj,Data);
    }
  }
  catch(const openfluid::base::FrameworkException&)
  {
    throw;
  }

  return WareSrcPathObj.toGeneric(); 
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::createObserver(const openfluid::ware::ObserverSignature& Signature, 
                                           const Configuration& Config,
                                           const std::string& ParentPath,
                                           bool WithIDSubDir)
{
  openfluid::base::Environment::init();

  openfluid::tools::TemplateProcessor TplProc("%%","%%");
  openfluid::tools::TemplateProcessor::Errors Errors;

  TplProc.enableAutoComment("#");
  TplProc.ignoreUnknown(true);

  auto Data = prepareTemplateData(Signature,Config);

  auto CommonSkelPath = getTemplateSkeletonPath("common");
  auto SkelPath =  getTemplateSkeletonPath("observer");

  auto WareSrcPathObj = getSrcPathObject(Signature.ID,ParentPath,WithIDSubDir);
  auto WareInfoFileObj = WareSrcPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE);

  try 
  {
    TplProc.renderDirectory(CommonSkelPath.toGeneric(),WareSrcPathObj.toGeneric(),Data,Errors);
    TplProc.renderDirectory(SkelPath.toGeneric(),WareSrcPathObj.toGeneric(),Data,Errors);

    ObserverSignatureSerializer().writeToJSONFile(Signature,WareInfoFileObj.toGeneric());

    if (Config.WithParamsUI)
    {
     renderParamsUIFiles(WareSrcPathObj,Data);
    }
  }
  catch(const openfluid::base::FrameworkException&)
  {
    throw;
  }

  return WareSrcPathObj.toGeneric();
}


// =====================================================================
// =====================================================================


std::string WareSrcFactory::createBuilderext(const openfluid::builderext::BuilderExtensionSignature& Signature, 
                                             const Configuration& Config,
                                             const std::string& ParentPath,
                                             bool WithIDSubDir)
{
  openfluid::base::Environment::init();

  openfluid::tools::TemplateProcessor TplProc("%%","%%");
  openfluid::tools::TemplateProcessor::Errors Errors;

  TplProc.enableAutoComment("#");
  TplProc.ignoreUnknown(true);

  auto Data = prepareTemplateData(Signature,Config);

  auto CommonSkelPath = getTemplateSkeletonPath("common");

  auto WareSrcPathObj = getSrcPathObject(Signature.ID,ParentPath,WithIDSubDir);
  auto WareInfoFileObj = WareSrcPathObj.fromThis(openfluid::config::WARESDEV_WAREMETA_FILE);

  auto ModeSuffix = Signature.getModeAsString();

  try 
  {
    TplProc.renderDirectory(CommonSkelPath.toGeneric(),WareSrcPathObj.toGeneric(),Data,Errors);

    BuilderextSignatureSerializer().writeToJSONFile(Signature,WareInfoFileObj.toGeneric());

    for (const std::string& Ext : {std::string(".cpp"), std::string(".hpp")})
    {
      auto CommonFile =  openfluid::tools::Path({openfluid::base::Environment::getInstallPrefix(),
                                                  openfluid::config::SHARE_WARESDEV_INSTALL_PATH,
                                                  "templates","builderext","files","WareMain_"+ModeSuffix+Ext});

      auto WareSrcSrcFile = WareSrcPathObj.fromThis({openfluid::config::WARESDEV_SRC_DIR,"WareMain"+Ext});

      TplProc.renderFile(CommonFile.toGeneric(),WareSrcSrcFile.toGeneric(),Data,Errors);
    }
  }
  catch(const openfluid::base::FrameworkException& E)
  {
    openfluid::utils::log::error("BuilderExt", E.what());
    throw;
  }

  return WareSrcPathObj.toGeneric();
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


std::string getSimulatorCommonCode(const openfluid::ware::SimulatorSignature& Signature,
                                   const std::string& PrimitiveStr)
{
  std::string TmpStr;
  std::set<openfluid::core::UnitsClass_t> UnitsClasses;

  for (auto& Item : Signature.SimulatorHandledData.ProducedVars)
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

      for (auto& Item : Signature.SimulatorHandledData.ProducedVars)
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

          TmpStr += "        "+PrimitiveStr+"(U,\""+Item.Name+"\","+TmpValueStr+");\n";
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
