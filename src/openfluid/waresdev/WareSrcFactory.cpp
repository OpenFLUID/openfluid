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
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QObject>
#include <QApplication>
#include <QRegExp>
#include <QTextStream>


#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace waresdev {


WareSrcFactory::WareSrcFactory(openfluid::ware::WareType Type) :
    m_WareId("")
{
  openfluid::base::Environment::init();

  QString TypeSubDir;

  switch (Type)
  {
    case openfluid::ware::WareType::SIMULATOR:
      TypeSubDir = QString::fromStdString(openfluid::config::SIMULATORS_PATH);
      break;
    case openfluid::ware::WareType::OBSERVER:
      TypeSubDir = QString::fromStdString(openfluid::config::OBSERVERS_PATH);
      break;
    case openfluid::ware::WareType::BUILDEREXT:
      TypeSubDir = QString::fromStdString(openfluid::config::BUILDEREXTS_PATH);
      break;
    default:
      break;
  }

  m_SharedTemplatesDir = QDir(
      QString("%1/%2/templates").arg(
          QString::fromStdString(openfluid::base::Environment::getInstallPrefix())).arg(
          QString::fromStdString(openfluid::config::SHARE_WARESDEV_INSTALL_PATH)));
  m_TypedTemplatesDir = QDir(m_SharedTemplatesDir.absoluteFilePath(TypeSubDir));

  m_WareTypeDir = QDir(WareSrcManager::instance()->getWareTypePath(Type));
}


// =====================================================================
// =====================================================================


WareSrcFactory::~WareSrcFactory()
{

}


// =====================================================================
// =====================================================================


void WareSrcFactory::setWareId(const QString& Id)
{
  m_WareId = Id;
}


// =====================================================================
// =====================================================================


bool WareSrcFactory::createCMakeListsFile(QString& NewFilePath, QString& ErrMsg)
{
  return copyTemplateToNewFile(m_TypedTemplatesDir.filePath("CMakeLists.txt.tpl"), "CMakeLists.txt", NewFilePath,
                               ErrMsg);
}


// =====================================================================
// =====================================================================


bool WareSrcFactory::createJsonFile(QString& NewFilePath, QString& ErrMsg)
{
  return copyTemplateToNewFile(m_SharedTemplatesDir.filePath("wareshub.json.tpl"), "wareshub.json",
                               NewFilePath, ErrMsg);
}


// =====================================================================
// =====================================================================


bool WareSrcFactory::createCppFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg)
{
  QString TplFilename;
  switch (R.getBuilderExtType())
  {
    case openfluid::builderext::MODE_MODAL:
      TplFilename = "source_modal.cpp.tpl";
      break;
    case openfluid::builderext::MODE_MODELESS:
      TplFilename = "source_modeless.cpp.tpl";
      break;
    case openfluid::builderext::MODE_WORKSPACE:
      TplFilename = "source_workspace.cpp.tpl";
      break;
    default:
      TplFilename = "source.cpp.tpl";
      break;
  }

  return (copyTemplateToNewFile(m_TypedTemplatesDir.filePath(TplFilename), R.RootCppFilename, NewFilePath, ErrMsg)
      && replaceInFile(R, NewFilePath, ErrMsg));
}


// =====================================================================
// =====================================================================


bool WareSrcFactory::createHppFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg)
{
  QString TplFilename;
  switch (R.getBuilderExtType())
  {
    case openfluid::builderext::MODE_MODAL:
      TplFilename = "source_modal.hpp.tpl";
      break;
    case openfluid::builderext::MODE_MODELESS:
      TplFilename = "source_modeless.hpp.tpl";
      break;
    case openfluid::builderext::MODE_WORKSPACE:
      TplFilename = "source_workspace.hpp.tpl";
      break;
    default:
      ErrMsg = QObject::tr("The \".hpp\" template file does not exist");
      return false;
      break;
  }

  return (copyTemplateToNewFile(m_TypedTemplatesDir.filePath(TplFilename), R.RootHppFilename, NewFilePath, ErrMsg)
      && replaceInFile(R, NewFilePath, ErrMsg));
}


// =====================================================================
// =====================================================================


bool WareSrcFactory::createParamUiCppFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg)
{
  return (copyTemplateToNewFile(m_TypedTemplatesDir.filePath("paramsui_source.cpp.tpl"), R.ParamsUiRootCppFilename,
                                NewFilePath, ErrMsg)
          && replaceInFile(R, NewFilePath, ErrMsg));
}


// =====================================================================
// =====================================================================


bool WareSrcFactory::createParamUiHppFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg)
{
  return (copyTemplateToNewFile(m_TypedTemplatesDir.filePath("paramsui_source.hpp.tpl"), R.ParamsUiRootHppFilename,
                                NewFilePath, ErrMsg)
          && replaceInFile(R, NewFilePath, ErrMsg));
}


// =====================================================================
// =====================================================================


bool WareSrcFactory::createCmakeConfigFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg)
{
  return (copyTemplateToNewFile(m_TypedTemplatesDir.filePath("CMake.in.config.tpl"), "CMake.in.config", NewFilePath,
                                ErrMsg)
          && replaceInFile(R, NewFilePath, ErrMsg));
}


// =====================================================================
// =====================================================================


bool WareSrcFactory::copyTemplateToNewFile(const QString& TemplatePath, const QString& NewFileName,
                                           QString& NewFilePath, QString& ErrMsg)
{
  if (m_WareId.isEmpty())
  {
    ErrMsg = QObject::tr("Ware ID is empty");
    return false;
  }

  QDir WareDir = QDir(m_WareTypeDir.filePath(m_WareId));
  if (!WareDir.exists())
  {
    ErrMsg = QObject::tr("Ware directory does not exist");
    return false;
  }

  QFile TemplateFile(TemplatePath);
  if (!TemplateFile.exists())
  {
    ErrMsg = QObject::tr("The \"%1\" template file does not exist").arg(QFileInfo(TemplatePath).fileName());
    return false;
  }

  NewFilePath = WareDir.absoluteFilePath(NewFileName);

  if (!TemplateFile.copy(NewFilePath))
  {
    ErrMsg = QObject::tr("Unable to copy the \"%1\" template file").arg(QFileInfo(TemplatePath).fileName());
    return false;
  }

  return true;
}


// =====================================================================
// =====================================================================


bool WareSrcFactory::replaceInFile(const Replacements& R, const QString& NewFilePath, QString& ErrMsg)
{
  QFile File(NewFilePath);

  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    ErrMsg = QObject::tr("Unable to open the created file in read mode");
    return false;
  }

  QString Str = QTextStream(&File).readAll();
  File.close();

  Str.replace("%%WAREID%%", m_WareId);
  Str.replace("%%WARELINKUID%%",R.LinkUID);
  Str.replace("%%CLASSNAME%%", R.ClassName);
  Str.replace("%%ROOTCPPFILENAME%%", R.RootCppFilename);
  Str.replace("%%ROOTHPPFILENAME%%", R.RootHppFilename);
  Str.replace("%%HPPHEADERGUARD%%", R.HppHeaderGuard);
  Str.replace("%%PARAMSUIENABLED%%", R.getParamsUiEnabled());
  Str.replace("%%PARAMSUICLASSNAME%%", R.ParamsUiClassname);
  Str.replace("%%PARAMSUIROOTCPPFILENAME%%", R.ParamsUiRootCppFilename);
  Str.replace("%%PARAMSUIROOTHPPFILENAME%%", R.ParamsUiRootHppFilename);
  Str.replace("%%PARAMSUIHPPHEADERGUARD%%", R.ParamsUiHeaderGuard);
  Str.replace("%%PARAMSUICOMMENT%%", R.ParamsUiComment);
  Str.replace("%%SIM2DOCINSTALLDISABLED%%", R.getSim2docInstall());
  Str.replace("%%SIM2DOCMODE%%", R.getSim2docMode());
  Str.replace("%%SIGNATUREINFOS%%",R.SignatureInfos);
  Str.replace("%%SIMSIGNATUREDATA%%",R.SimulatorSignatureData);
  Str.replace("%%SIMINITCODE%%",R.SimulatorInitCode);
  Str.replace("%%SIMRUNCODE%%",R.SimulatorRunCode);
  Str.replace("%%SIMSCHEDRETURN%%",R.SimulatorSchedulingReturn);
  Str.replace("%%BUILDEREXTCATEGORY%%", R.getBuilderExtCategory());
  Str.replace("%%BUILDEREXTMENUTEXT%%", R.BuilderExtMenuText);

  if (!File.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    ErrMsg = QObject::tr("Unable to open the created file in write mode");
    return false;
  }

  QTextStream(&File) << Str;
  File.close();

  return true;
}


// =====================================================================
// =====================================================================


QRegExp WareSrcFactory::getCppFilenameRegExp(QString& Tooltip, bool IsHpp)
{
  Tooltip = QObject::tr("Accepts only letters, digits, dashes ('-'), underscores ('_') and dots ('.').");
  return IsHpp ? QRegExp("[a-zA-Z0-9._-]+\\.hpp") : QRegExp("[a-zA-Z0-9._-]+\\.cpp");
}


// =====================================================================
// =====================================================================


QRegExp WareSrcFactory::getClassnameRegExp(QString& Tooltip)
{
  Tooltip = QObject::tr("Accepts only letters, digits, underscores ('_'), and must begin with a letter.");
  return QRegExp("[a-zA-Z]+[a-zA-Z0-9_]*");
}


// =====================================================================
// =====================================================================


QRegExp WareSrcFactory::getWareIdRegExp(QString& Tooltip)
{
  Tooltip = QObject::tr("Accepts only letters, digits, dashes ('-'), underscores ('_') and dots ('.').");
  return QRegExp("[a-zA-Z0-9._-]+");
}


// =====================================================================
// =====================================================================


QString WareSrcFactory::getHeaderGuard(const QString& HppFilename)
{
  return HppFilename.toUpper().replace(".", "_").append("__").prepend("__");
}


// =====================================================================
// =====================================================================


QString WareSrcFactory::getHppFilename(const QString& CppFilename)
{
  QString Hpp(CppFilename);
  return Hpp.replace(QRegExp("\\.cpp$"), ".hpp");
}


// =====================================================================
// =====================================================================


std::string getDeclarationStringFromVarType(openfluid::core::Value::Type VarType)
{
  if (VarType == openfluid::core::Value::NONE)
    return "";
  else
    return "["+openfluid::core::Value::getStringFromValueType(VarType)+"]";
}


// =====================================================================
// =====================================================================


QString WareSrcFactory::getSimulatorSignatureInfos(const openfluid::ware::SimulatorSignature& Signature)
{
  std::string TmpStr;

  TmpStr += "  // Informations\n";
  TmpStr += "  DECLARE_NAME(\""+Signature.Name+"\")\n";
  TmpStr += "  DECLARE_DESCRIPTION(\""+Signature.Description+"\")\n";
  TmpStr += "  DECLARE_VERSION(\""+Signature.Version+"\")\n";

  TmpStr += "  DECLARE_STATUS(";
  if (Signature.Status == openfluid::ware::EXPERIMENTAL)
    TmpStr += "openfluid::ware::EXPERIMENTAL";
  else if (Signature.Status == openfluid::ware::BETA)
    TmpStr += "openfluid::ware::BETA";
  else
    TmpStr += "openfluid::ware::STABLE";
  TmpStr += ")\n";

  for (auto& Item : Signature.Authors)
    TmpStr += "  DECLARE_AUTHOR(\""+Item.first+"\",\""+Item.second+"\")\n";

  return QString::fromStdString(TmpStr);
}


// =====================================================================
// =====================================================================


QString WareSrcFactory::getSimulatorSignatureData(const openfluid::ware::SimulatorSignature& Signature)
{
  std::string TmpStr/* = "\n"*/;

  // TODO replace indentation size with value from user's preferences

  if (Signature.HandledData.RequiredParams.size() + Signature.HandledData.UsedParams.size())
    TmpStr += "\n  // Parameters\n";

  // -- Parameters
  for (auto& Item : Signature.HandledData.RequiredParams)
    TmpStr += "  DECLARE_REQUIRED_PARAMETER(\""+Item.DataName+"\","
                                           "\""+Item.Description+"\","
                                           "\""+Item.DataUnit+"\")\n";

  for (auto& Item : Signature.HandledData.UsedParams)
    TmpStr += "  DECLARE_USED_PARAMETER(\""+Item.DataName+"\","
                                       "\""+Item.Description+"\","
                                       "\""+Item.DataUnit+"\")\n";


  // -- Extra files

  if (Signature.HandledData.RequiredExtraFiles.size() + Signature.HandledData.UsedExtraFiles.size())
    TmpStr += "\n  // Extra files\n";

  for (auto& Item : Signature.HandledData.RequiredExtraFiles)
    TmpStr += "  DECLARE_REQUIRED_EXTRAFILE(\""+Item+"\")\n";

  for (auto& Item : Signature.HandledData.UsedExtraFiles)
    TmpStr += "  DECLARE_USED_EXTRAFILE(\""+Item+"\")\n";


  // -- Attributes

  if (Signature.HandledData.RequiredAttribute.size() + Signature.HandledData.UsedAttribute.size() +
      Signature.HandledData.ProducedAttribute.size())
    TmpStr += "\n  // Attributes\n";

  for (auto& Item : Signature.HandledData.RequiredAttribute)
    TmpStr += "  DECLARE_REQUIRED_ATTRIBUTE(\""+Item.DataName+"\","
                                           "\""+Item.UnitsClass+"\","
                                           "\""+Item.Description+"\","
                                           "\""+Item.DataUnit+"\")\n";

  for (auto& Item : Signature.HandledData.UsedAttribute)
    TmpStr += "  DECLARE_USED_ATTRIBUTE(\""+Item.DataName+"\","
                                       "\""+Item.UnitsClass+"\","
                                       "\""+Item.Description+"\","
                                       "\""+Item.DataUnit+"\")\n";

  for (auto& Item : Signature.HandledData.ProducedAttribute)
    TmpStr += "  DECLARE_PRODUCED_ATTRIBUTE(\""+Item.DataName+"\","
                                           "\""+Item.UnitsClass+"\","
                                           "\""+Item.Description+"\","
                                           "\""+Item.DataUnit+"\")\n";


  // -- Events

  if (Signature.HandledData.UsedEventsOnUnits.size())
     TmpStr += "\n  // Events\n";

  for (auto& Item : Signature.HandledData.UsedEventsOnUnits)
    TmpStr += "  DECLARE_USED_EVENTS(\""+Item+"\")\n";


  // -- Variables

  if (Signature.HandledData.RequiredVars.size() + Signature.HandledData.UsedVars.size() +
      Signature.HandledData.ProducedVars.size() + Signature.HandledData.UpdatedVars.size())
    TmpStr += "\n  // Variables\n";


  for (auto& Item : Signature.HandledData.RequiredVars)
    TmpStr += "  DECLARE_REQUIRED_VARIABLE(\""+Item.DataName+getDeclarationStringFromVarType(Item.DataType)+"\","
                                          "\""+Item.UnitsClass+"\","
                                          "\""+Item.Description+"\","
                                          "\""+Item.DataUnit+"\")\n";

  for (auto& Item : Signature.HandledData.UsedVars)
    TmpStr += "  DECLARE_USED_VARIABLE(\""+Item.DataName+getDeclarationStringFromVarType(Item.DataType)+"\","
                                          "\""+Item.UnitsClass+"\","
                                          "\""+Item.Description+"\","
                                          "\""+Item.DataUnit+"\")\n";

  for (auto& Item : Signature.HandledData.ProducedVars)
    TmpStr += "  DECLARE_PRODUCED_VARIABLE(\""+Item.DataName+getDeclarationStringFromVarType(Item.DataType)+"\","
                                          "\""+Item.UnitsClass+"\","
                                          "\""+Item.Description+"\","
                                          "\""+Item.DataUnit+"\")\n";

  for (auto& Item : Signature.HandledData.UpdatedVars)
    TmpStr += "  DECLARE_UPDATED_VARIABLE(\""+Item.DataName+getDeclarationStringFromVarType(Item.DataType)+"\","
                                          "\""+Item.UnitsClass+"\","
                                          "\""+Item.Description+"\","
                                          "\""+Item.DataUnit+"\")\n";


  // -- Spatial graph

  if (Signature.HandledUnitsGraph.UpdatedUnitsClass.size() + Signature.HandledUnitsGraph.UpdatedUnitsGraph.size())
     TmpStr += "\n  // Spatial graph\n";

  if (!Signature.HandledUnitsGraph.UpdatedUnitsGraph.empty())
    TmpStr += "  DECLARE_UPDATED_UNITSGRAPH(\""+Signature.HandledUnitsGraph.UpdatedUnitsGraph+"\")\n";

  for (auto& Item : Signature.HandledUnitsGraph.UpdatedUnitsClass)
    TmpStr += "  DECLARE_UPDATED_UNITSCLASS(\""+Item.UnitsClass+"\",\""+Item.Description+"\")\n";


  // -- Scheduling

  TmpStr += "\n  // Scheduling\n";

  if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::DEFAULT)
    TmpStr += "  DECLARE_SCHEDULING_DEFAULT\n";
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::FIXED)
  {
    std::string TmpValueStr = (QString::fromStdString("%1").arg(Signature.TimeScheduling.Min)).toStdString();
    TmpStr += "  DECLARE_SCHEDULING_FIXED("+TmpValueStr+")\n";
  }
  else if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::RANGE)
  {
    std::string TmpValueStr = (QString::fromStdString("%1,%2")
                               .arg(Signature.TimeScheduling.Min)
                               .arg(Signature.TimeScheduling.Max)).toStdString();
    TmpStr += "  DECLARE_SCHEDULING_RANGE("+TmpValueStr+")\n";
  }
  else
    TmpStr += "  DECLARE_SCHEDULING_UNDEFINED\n";

  return QString::fromStdString(TmpStr);
}


// =====================================================================
// =====================================================================


QString getSimulatorInitRunCode(const openfluid::ware::SimulatorSignature& Signature,
                                const std::string& PrimitiveStr)
{
  std::string TmpStr;
  QStringList UnitsClasses;

  for (auto& Item : Signature.HandledData.ProducedVars)
    UnitsClasses << QString::fromStdString(Item.UnitsClass);

  UnitsClasses.removeDuplicates();
  UnitsClasses.sort();

  if (!UnitsClasses.isEmpty())
  {
    // TODO replace indentation size with value from user's preferences

    TmpStr += "      // -- beginning of automatically generated code --\n";
    TmpStr += "      openfluid::core::SpatialUnit* U;\n";

    for (auto& UClass : UnitsClasses)
    {
      TmpStr += "\n";
      TmpStr += "      OPENFLUID_UNITS_ORDERED_LOOP(\""+UClass.toStdString()+"\",U)\n";
      TmpStr += "      {\n";

      for (auto& Item : Signature.HandledData.ProducedVars)
      {
        if (Item.UnitsClass == UClass.toStdString())
        {
          std::string TmpValueStr = "0.0";
          if (Item.DataType == openfluid::core::Value::BOOLEAN)
            TmpValueStr = "openfluid::core::BooleanValue()";
          else if (Item.DataType == openfluid::core::Value::INTEGER)
            TmpValueStr = "openfluid::core::IntegerValue(0)";
          else if (Item.DataType == openfluid::core::Value::STRING)
            TmpValueStr = "openfluid::core::StringValue()";
          else if (Item.DataType == openfluid::core::Value::MAP)
            TmpValueStr = "openfluid::core::MapValue()";
          else if (Item.DataType == openfluid::core::Value::VECTOR)
            TmpValueStr = "openfluid::core::VectorValue(5,0.0)";
          else if (Item.DataType == openfluid::core::Value::MATRIX)
            TmpValueStr = "openfluid::core::MatrixValue(5,5,0.0)";
          else if (Item.DataType == openfluid::core::Value::TREE)
            TmpValueStr = "openfluid::core::TreeValue()";
          else if (Item.DataType == openfluid::core::Value::NULLL)
            TmpValueStr = "openfluid::core::NullValue()";

          TmpStr += "        "+PrimitiveStr+"(U,\""+Item.DataName+"\","+TmpValueStr+");\n";
        }
      }
      TmpStr += "      }\n";
    }
    TmpStr += "      // -- end of automatically generated code --\n";
  }

  return QString::fromStdString(TmpStr);
}


// =====================================================================
// =====================================================================


QString WareSrcFactory::getSimulatorInitCode(const openfluid::ware::SimulatorSignature& Signature)
{
  return getSimulatorInitRunCode(Signature,"OPENFLUID_InitializeVariable");
}


// =====================================================================
// =====================================================================


QString WareSrcFactory::getSimulatorRunCode(const openfluid::ware::SimulatorSignature& Signature)
{
  return getSimulatorInitRunCode(Signature,"OPENFLUID_AppendVariable");
}


// =====================================================================
// =====================================================================


QString WareSrcFactory::getSimulatorSchedulingReturn(const openfluid::ware::SimulatorSignature& Signature)
{
  QString TmpStr = "DefaultDeltaT()";

  if (Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::FIXED ||
      Signature.TimeScheduling.Type == openfluid::ware::SignatureTimeScheduling::RANGE)
    TmpStr = QString("Duration(%1)").arg(Signature.TimeScheduling.Min);

  return TmpStr;
}


} }  // namespaces
