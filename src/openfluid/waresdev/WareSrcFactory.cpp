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


#include <openfluid/waresdev/WareSrcFactory.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/config.hpp>

#include <QObject>
#include <QApplication>
#include <QRegExp>
#include <QTextStream>


namespace openfluid { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcFactory::WareSrcFactory(WareSrcManager::WareType Type) :
    m_WareId("")
{
  QString TypeSubDir;

  switch (Type)
  {
    case WareSrcManager::SIMULATOR:
      TypeSubDir = QString::fromStdString(openfluid::config::SIMULATORS_PLUGINS_SUBDIR);
      break;
    case WareSrcManager::OBSERVER:
      TypeSubDir = QString::fromStdString(openfluid::config::OBSERVERS_PLUGINS_SUBDIR);
      break;
    case WareSrcManager::BUILDEREXT:
      TypeSubDir = QString::fromStdString(openfluid::config::BUILDEREXTS_PLUGINS_SUBDIR);
      break;
    default:
      break;
  }

  m_SharedTemplatesDir = QDir(
      QString("%1/%2/templates").arg(
          QString::fromStdString(openfluid::base::RuntimeEnvironment::instance()->getInstallPrefix())).arg(
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
    case openfluid::builderext::TYPE_MODAL:
      TplFilename = "source_modal.cpp.tpl";
      break;
    case openfluid::builderext::TYPE_MODELESS:
      TplFilename = "source_modeless.cpp.tpl";
      break;
    case openfluid::builderext::TYPE_WORKSPACE:
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
    case openfluid::builderext::TYPE_MODAL:
      TplFilename = "source_modal.hpp.tpl";
      break;
    case openfluid::builderext::TYPE_MODELESS:
      TplFilename = "source_modeless.hpp.tpl";
      break;
    case openfluid::builderext::TYPE_WORKSPACE:
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


} }  // namespaces
