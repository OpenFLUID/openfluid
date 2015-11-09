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
  @file PreferencesManager.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
  @author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "PreferencesManager.hpp"

#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QDateTime>

#include <openfluid/config.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace base {


OPENFLUID_SINGLETON_INITIALIZATION(PreferencesManager)

QString PreferencesManager::m_FileName = "";
const int PreferencesManager::RecentProjectsLimit = 10;


// =====================================================================
// =====================================================================


PreferencesManager::PreferencesManager():
  mp_ConfFile(nullptr)
{
  openfluid::base::Environment::init();

  if (m_FileName.isEmpty())
  {
    m_FileName = QString(openfluid::base::Environment::getConfigFile().c_str());
  }


  QDir FileDir = QFileInfo(m_FileName).path();

  if (!FileDir.exists() && !QDir::root().mkpath(FileDir.absolutePath()))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Cannot create " + FileDir.path().toStdString() + " directory");

  mp_ConfFile = new QSettings(QString(m_FileName),QSettings::IniFormat);

  if (!QFileInfo(m_FileName).exists())
  {
    setDefaultValues();
    mp_ConfFile->sync();
  }

  setWaresdevTextEditorDefaults(false);
}


// =====================================================================
// =====================================================================


PreferencesManager::~PreferencesManager()
{

}


// =====================================================================
// =====================================================================


void PreferencesManager::setDefaultValues()
{
  setBuilderRecentMax(5);
  setBuilderDeltaT(300);
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getFileName()
{
  return m_FileName;
}

// =====================================================================
// =====================================================================


void PreferencesManager::setFileName(const QString& AbsoluteFileName)
{
  if (!mp_Instance)
    m_FileName = AbsoluteFileName;
  else
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "FileName can not be changed after PreferencesManager instantiation");
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isValidKey(const QString& Group, const QString& Key)
{
  return mp_ConfFile->contains(Group+"/"+Key);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setLang(const QString& Lang)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("lang",Lang);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getLang()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("lang"))
    mp_ConfFile->setValue("lang",guessLang());
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  QString Lang = mp_ConfFile->value("lang").toString();
  mp_ConfFile->endGroup();
  return Lang;
}


// =====================================================================
// =====================================================================


QStringList PreferencesManager::getAvailableLangs()
{
  QStringList QMFiles;
  QMFiles = QDir(QString(openfluid::base::Environment::getTranslationsDir().c_str()))
                 .entryList(QStringList("*.qm"),QDir::Files);

  QStringList Langs;
  for (int i=0;i<QMFiles.size();++i)
    Langs.append(QMFiles[i].right(8).left(5));

  return Langs;
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isAvailableLang(const QString& Lang)
{
  return getAvailableLangs().contains(Lang);
}


// =====================================================================
// =====================================================================


QString PreferencesManager::guessLang()
{
  QString Locale = QLocale::system().name();

  if (isAvailableLang(Locale)) return Locale;
  else return "default";
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderRecentMax(int RecentMax)
{
  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  if (RecentMax > RecentProjectsLimit)
    RecentMax = RecentProjectsLimit;
  mp_ConfFile->setValue("recentmax",RecentMax);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();

  adaptBuilderRecentProjects();
}

// =====================================================================
// =====================================================================


int PreferencesManager::getBuilderRecentMax()
{
  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");

  if (!mp_ConfFile->contains("recentmax"))
    mp_ConfFile->setValue("recentmax",(unsigned int)(RecentProjectsLimit/2));

  unsigned int RecentMax = mp_ConfFile->value("recentmax").toUInt();
  mp_ConfFile->endGroup();
  return RecentMax;
}


// =====================================================================
// =====================================================================


bool PreferencesManager::addBuilderRecentProject(const QString& ProjectName,
                                          const QString& ProjectPath)
{
  if (ProjectPath.indexOf("=") >= 0)
    return false;

  QStringList Recents;

  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  if (mp_ConfFile->contains("list"))
    Recents = mp_ConfFile->value("list").toStringList();
  mp_ConfFile->endGroup();

  QString NewRecentPair = ProjectName+"|"+ProjectPath;
  int RecentMax = getBuilderRecentMax();

  Recents.removeAll(NewRecentPair); // remove similar existing projects/paths
  Recents.prepend(NewRecentPair); // add the new project/path at the beginning
  while (Recents.size() > RecentMax) Recents.removeLast();

  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  mp_ConfFile->setValue("list",Recents);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();

  return true;
}


// =====================================================================
// =====================================================================


void PreferencesManager::clearBuilderRecentProjects()
{
  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  mp_ConfFile->remove("list");
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


void PreferencesManager::adaptBuilderRecentProjects()
{
  QStringList Recents;

  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  if (mp_ConfFile->contains("list"))
    Recents = mp_ConfFile->value("list").toStringList();
  mp_ConfFile->endGroup();

  int RecentMax = getBuilderRecentMax();

  while (Recents.size() > RecentMax) Recents.removeLast();

  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  mp_ConfFile->setValue("list",Recents);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


PreferencesManager::RecentProjectsList_t PreferencesManager::getBuilderRecentProjects()
{
  QStringList Recents;

  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  if (mp_ConfFile->contains("list"))
    Recents = mp_ConfFile->value("list").toStringList();
  mp_ConfFile->endGroup();

  RecentProjectsList_t RPL;

  for (int i = 0; i < Recents.size(); ++i)
  {
    QStringList ProjectPair = Recents[i].split('|');
    if (ProjectPair.size() == 2)
    {
      RecentProject_t RP;
      RP.Name = ProjectPair[0];
      RP.Path = ProjectPair[1];
      RPL.push_back(RP);
    }
  }

  return RPL;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderWorkspacesPaths(const QStringList& Paths)
{
  mp_ConfFile->beginGroup("openfluid.builder.paths");
  mp_ConfFile->setValue("workspace",Paths);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QStringList PreferencesManager::getBuilderWorkspacesPaths()
{
  mp_ConfFile->beginGroup("openfluid.builder.paths");
  QStringList PathsList = mp_ConfFile->value("workspace").toStringList();
  mp_ConfFile->endGroup();

  if (PathsList.isEmpty())
  {
    PathsList.append(QString(
      openfluid::base::Environment::getUserDataFullPath(openfluid::config::WORKSPACE_PATH).c_str()));
  }

  return PathsList;
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getBuilderWorkspacePath()
{
  QStringList Paths = getBuilderWorkspacesPaths();

  if (!Paths.isEmpty())
    return Paths[0];
  else
    return "";
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getBuilderProjectsPath()
{
  return getBuilderWorkspacePath()+"/"+QString::fromStdString(openfluid::config::PROJECTS_PATH);
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderExtraPaths(const QString& Key, const QStringList& Paths)
{
  mp_ConfFile->beginGroup("openfluid.builder.paths");
  if (Paths.isEmpty())
    mp_ConfFile->remove(Key);
  else
    mp_ConfFile->setValue(Key,Paths);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();

}


// =====================================================================
// =====================================================================


void PreferencesManager::addBuilderExtraPath(const QString& Key, const QString& Path)
{
  if (Path.indexOf("=") >= 0)
    return;

  mp_ConfFile->beginGroup("openfluid.builder.paths");
  QStringList Paths = mp_ConfFile->value(Key).toStringList();
  mp_ConfFile->endGroup();

  Paths.removeAll(Path); // remove similar existing paths
  Paths.append(Path); // add the new path at the beginning

  mp_ConfFile->beginGroup("openfluid.builder.paths");
  mp_ConfFile->setValue(Key,Paths);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


void PreferencesManager::removeBuilderExtraPath(const QString& Key, const QString& Path)
{
  if (Path.indexOf("=") >= 0)
    return;

  mp_ConfFile->beginGroup("openfluid.builder.paths");
  QStringList Paths = mp_ConfFile->value(Key).toStringList();
  mp_ConfFile->endGroup();

  if (!Paths.isEmpty())
  {
    Paths.removeAll(Path); // remove similar existing paths

    mp_ConfFile->beginGroup("openfluid.builder.paths");
    mp_ConfFile->setValue(Key,Paths);
    mp_ConfFile->endGroup();
    mp_ConfFile->sync();
  }
}


// =====================================================================
// =====================================================================


QStringList PreferencesManager::getBuilderExtraPaths(const QString& Key)
{
  mp_ConfFile->beginGroup("openfluid.builder.paths");
  QStringList Paths = mp_ConfFile->value(Key).toStringList();
  mp_ConfFile->endGroup();

  return Paths;
}


// =====================================================================
// =====================================================================



void PreferencesManager::setBuilderExtraSimulatorsPaths(const QStringList& Paths)
{
  setBuilderExtraPaths("extrasimpaths",Paths);
}

// =====================================================================
// =====================================================================


void PreferencesManager::addBuilderExtraSimulatorsPath(const QString& Path)
{
  addBuilderExtraPath("extrasimpaths",Path);
}

// =====================================================================
// =====================================================================


void PreferencesManager::removeBuilderExtraSimulatorsPath(const QString& Path)
{
  removeBuilderExtraPath("extrasimpaths",Path);
}

// =====================================================================
// =====================================================================


QStringList PreferencesManager::getBuilderExtraSimulatorsPaths()
{
  return getBuilderExtraPaths("extrasimpaths");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderExtraExtensionsPaths(const QStringList& Paths)
{
  setBuilderExtraPaths("extraextpaths",Paths);
}


// =====================================================================
// =====================================================================


void PreferencesManager::addBuilderExtraExtensionsPath(const QString& Path)
{
  addBuilderExtraPath("extraextpaths",Path);
}


// =====================================================================
// =====================================================================


void PreferencesManager::removeBuilderExtraExtensionsPath(const QString& Path)
{
  removeBuilderExtraPath("extraextpaths",Path);
}


// =====================================================================
// =====================================================================


QStringList PreferencesManager::getBuilderExtraExtensionsPaths()
{
  return getBuilderExtraPaths("extraextpaths");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderExtraObserversPaths(const QStringList& Paths)
{
  setBuilderExtraPaths("extraobspaths",Paths);
}


// =====================================================================
// =====================================================================


void PreferencesManager::addBuilderExtraObserversPath(const QString& Path)
{
  addBuilderExtraPath("extraobspaths",Path);
}


// =====================================================================
// =====================================================================


void PreferencesManager::removeBuilderExtraObserversPath(const QString& Path)
{
  removeBuilderExtraPath("extraobspaths",Path);
}


// =====================================================================
// =====================================================================


QStringList PreferencesManager::getBuilderExtraObserversPaths()
{
  return getBuilderExtraPaths("extraobspaths");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderDeltaT(openfluid::core::Duration_t DeltaT)
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  mp_ConfFile->setValue("deltat",DeltaT);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


openfluid::core::Duration_t PreferencesManager::getBuilderDeltaT()
{
  openfluid::core::Duration_t DeltaT;

  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  if (!mp_ConfFile->contains("deltat"))
    mp_ConfFile->setValue("deltat",300);
  DeltaT = mp_ConfFile->value("deltat").toUInt();
  mp_ConfFile->endGroup();

  return DeltaT;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderBegin(const QString& Begin)
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  mp_ConfFile->setValue("begin",Begin);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getBuilderBegin()
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  QString DateStr = mp_ConfFile->value("begin").toString();
  mp_ConfFile->endGroup();

  if (DateStr.isEmpty())
  {
    std::string Now = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
    Now[10] = ' ';
    openfluid::core::DateTime DT;
    DT.setFromISOString(Now);
    setBuilderBegin(QString(DT.getAsISOString().c_str()));
    DateStr = QString(DT.getAsISOString().c_str());
  }

  return DateStr;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderEnd(const QString& End)
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  mp_ConfFile->setValue("end",End);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getBuilderEnd()
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  QString DateStr = mp_ConfFile->value("end").toString();
  mp_ConfFile->endGroup();

  if (DateStr.isEmpty())
  {
    openfluid::core::DateTime DT;
    DT.setFromISOString(getBuilderBegin().toStdString());
    DT = DT + openfluid::core::DateTime::Day();
    setBuilderEnd(QString(DT.getAsISOString().c_str()));
    DateStr = QString(DT.getAsISOString().c_str());
  }

  return DateStr;
}


// =====================================================================
// =====================================================================


bool PreferencesManager::addMarketplace(const QString& PlaceName,
                                        const QString& PlaceUrl)
{
  if (PlaceName.indexOf('=') >= 0)
    return false;

  mp_ConfFile->beginGroup("openfluid.market.marketplaces");
  mp_ConfFile->setValue(PlaceName,PlaceUrl);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
  return true;
}


// =====================================================================
// =====================================================================


void PreferencesManager::removeMarketplace(const QString& PlaceName)
{
  mp_ConfFile->beginGroup("openfluid.market.marketplaces");
  mp_ConfFile->remove(PlaceName);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


PreferencesManager::MarketPlaces_t PreferencesManager::getMarketplaces()
{
  MarketPlaces_t Places;

  mp_ConfFile->beginGroup("openfluid.market.marketplaces");
  QStringList PlacesKeys = mp_ConfFile->childKeys();

  for (int i = 0; i < PlacesKeys.size(); ++i)
    Places[PlacesKeys[i]] = mp_ConfFile->value(PlacesKeys[i]).toString();

  mp_ConfFile->endGroup();

  return Places;
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderExtensionValueExist(const QString& PluginName, const QString& Key)
{
  QString GroupName = "openfluid.builder.extensions:"+PluginName;

  return isValidKey(GroupName,Key);
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getBuilderExtensionValue(const QString& PluginName,
                                              const QString& Key)
{
  QString GroupName = "openfluid.builder.extensions:"+PluginName;

  mp_ConfFile->beginGroup(GroupName);
  QString Value = mp_ConfFile->value(Key).toString();
  mp_ConfFile->endGroup();
  return Value;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderExtensionValue(const QString& PluginName,
                                           const QString& Key, const QString& Value)
{
  QString GroupName = "openfluid.builder.extensions:"+PluginName;

  mp_ConfFile->beginGroup(GroupName);
  mp_ConfFile->setValue(Key,Value);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


Qt::DockWidgetArea PreferencesManager::getBuilderDockPosition()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("dockpos")) mp_ConfFile->setValue("dockpos",Qt::LeftDockWidgetArea);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  Qt::DockWidgetArea Pos = Qt::DockWidgetArea(mp_ConfFile->value("dockpos").toInt());
  mp_ConfFile->endGroup();

  return Pos;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderDockPosition(Qt::DockWidgetArea Position)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("dockpos",Position);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


Qt::ToolBarArea PreferencesManager::getBuilderToolBarPosition()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("toolbarpos"))
    mp_ConfFile->setValue("toolbarpos",Qt::TopToolBarArea);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  Qt::ToolBarArea Pos = Qt::ToolBarArea(mp_ConfFile->value("toolbarpos").toInt());
  mp_ConfFile->endGroup();

  return Pos;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderToolBarPosition(Qt::ToolBarArea Position)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("toolbarpos",Position);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderItemRemovalConfirm()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("itemremovalconfirm"))
    mp_ConfFile->setValue("itemremovalconfirm",true);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool Confirm = mp_ConfFile->value("itemremovalconfirm").toBool();
  mp_ConfFile->endGroup();

  return Confirm;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderItemRemovalConfirm(bool Confirm)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("itemremovalconfirm",Confirm);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderParamRemovalConfirm()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("paramremovalconfirm"))
    mp_ConfFile->setValue("paramremovalconfirm",true);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool Confirm = mp_ConfFile->value("paramremovalconfirm").toBool();
  mp_ConfFile->endGroup();

  return Confirm;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderParamRemovalConfirm(bool Confirm)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("paramremovalconfirm",Confirm);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderWaresWatchersActive()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("wareswatchers"))
    mp_ConfFile->setValue("wareswatchers",true);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool Watchers = mp_ConfFile->value("wareswatchers").toBool();
  mp_ConfFile->endGroup();

  return Watchers;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderWaresWatchersActive(bool Active)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("wareswatchers",Active);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderSpatialUnitsRemovalConfirm()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("spatialunitsremovalconfirm"))
    mp_ConfFile->setValue("spatialunitsremovalconfirm",true);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool Confirm = mp_ConfFile->value("spatialunitsremovalconfirm").toBool();
  mp_ConfFile->endGroup();

  return Confirm;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderSpatialUnitsRemovalConfirm(bool Confirm)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("spatialunitsremovalconfirm",Confirm);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderSpatialConnsRemovalConfirm()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("spatialconnsremovalconfirm"))
    mp_ConfFile->setValue("spatialconnsremovalconfirm",true);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool Confirm = mp_ConfFile->value("spatialconnsremovalconfirm").toBool();
  mp_ConfFile->endGroup();

  return Confirm;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderSpatialConnsRemovalConfirm(bool Confirm)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("spatialconnsremovalconfirm",Confirm);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderSpatialAttrsRemovalConfirm()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("spatialattrsremovalconfirm"))
    mp_ConfFile->setValue("spatialattrsremovalconfirm",true);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool Confirm = mp_ConfFile->value("spatialattrsremovalconfirm").toBool();
  mp_ConfFile->endGroup();

  return Confirm;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderSpatialAttrsRemovalConfirm(bool Confirm)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("spatialattrsremovalconfirm",Confirm);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isBuilderAutomaticSaveBeforeRun()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("savebeforerun"))
    mp_ConfFile->setValue("savebeforerun",false);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool AutoSave = mp_ConfFile->value("savebeforerun").toBool();
  mp_ConfFile->endGroup();

  return AutoSave;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBuilderAutomaticSaveBeforeRun(bool AutoSave)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("savebeforerun",AutoSave);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevAutomaticSaveBeforeBuild()
{
  mp_ConfFile->beginGroup("openfluid.waresdev.interface");
  if (!mp_ConfFile->contains("savebeforebuild"))
    mp_ConfFile->setValue("savebeforebuild",true);
  bool AutoSave = mp_ConfFile->value("savebeforebuild").toBool();
  mp_ConfFile->endGroup();

  return AutoSave;
}


// =====================================================================
// =====================================================================



void PreferencesManager::setWaresdevAutomaticSaveBeforeBuild(bool AutoSave)
{
  mp_ConfFile->beginGroup("openfluid.waresdev.interface");
  mp_ConfFile->setValue("savebeforebuild",AutoSave);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevTextEditorDefaults(bool ForceReset)
{
  mp_ConfFile->beginGroup("openfluid.waresdev.texteditor");


  mp_ConfFile->beginGroup("syntax_highlighting");

  if(! mp_ConfFile->contains("enabled") || ForceReset)
  mp_ConfFile->setValue("enabled",true);

  QMap<QString,QString> DefaultHLRules;

  DefaultHLRules.insert("function/color","system");
  DefaultHLRules.insert("function/decoration","bold");

  DefaultHLRules.insert("keyword/color","#7F0055");
  DefaultHLRules.insert("keyword/decoration","bold");

  DefaultHLRules.insert("datatype/color","system");
  DefaultHLRules.insert("datatype/decoration","bold");

  DefaultHLRules.insert("preprocessor/color","#7F0055");
  DefaultHLRules.insert("preprocessor/decoration","bold");

  DefaultHLRules.insert("openfluid-keyword/color","#546F02");
  DefaultHLRules.insert("openfluid-keyword/decoration","bold");

  DefaultHLRules.insert("openfluid-deprecated/color","#546F02");
  DefaultHLRules.insert("openfluid-deprecated/decoration","bold,strike-through");

  DefaultHLRules.insert("deprecated/color","#7F0055");
  DefaultHLRules.insert("deprecated/decoration","bold,strike-through");

  DefaultHLRules.insert("quoted/color","#2A00FF");
  DefaultHLRules.insert("quoted/decoration","none");

  DefaultHLRules.insert("comment/color","#A7A7A7");
  DefaultHLRules.insert("comment/decoration","italic");

  for(QMap<QString,QString>::iterator it = DefaultHLRules.begin(); it != DefaultHLRules.end(); ++it)
  {
    if(! mp_ConfFile->contains(it.key()) || ForceReset)
      mp_ConfFile->setValue(it.key(),it.value().remove(" ").split(',',QString::SkipEmptyParts));
  }

  mp_ConfFile->endGroup();


  mp_ConfFile->beginGroup("currentline_highlighting");

  if(! mp_ConfFile->contains("enabled") || ForceReset)
    mp_ConfFile->setValue("enabled",true);

  if(! mp_ConfFile->contains("color") || ForceReset)
    mp_ConfFile->setValue("color","#EFF6FF");

  mp_ConfFile->endGroup();


  if(! mp_ConfFile->contains("fontname") || ForceReset)
    mp_ConfFile->setValue("fontname","DejaVu Sans Mono");

  if(! mp_ConfFile->contains("linewrapping/enabled") || ForceReset)
     mp_ConfFile->setValue("linewrapping/enabled",true);

  if(! mp_ConfFile->contains("indent/spacenumber") || ForceReset)
     mp_ConfFile->setValue("indent/spacenumber",2);

  mp_ConfFile->endGroup();

  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevSyntaxHighlightingEnabled()
{
  return mp_ConfFile->value("openfluid.waresdev.texteditor/syntax_highlighting/enabled",true).toBool();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevSyntaxHighlightingEnabled(bool Enabled)
{
  mp_ConfFile->setValue("openfluid.waresdev.texteditor/syntax_highlighting/enabled",Enabled);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


PreferencesManager::SyntaxHighlightingRules_t PreferencesManager::getWaresdevSyntaxHighlightingRules()
{
  SyntaxHighlightingRules_t Rules;

  mp_ConfFile->beginGroup("openfluid.waresdev.texteditor/syntax_highlighting");

  QStringList StyleNames = mp_ConfFile->childGroups();
  for(const QString& StyleName : StyleNames)
  {
    mp_ConfFile->beginGroup(StyleName);
    Rules.insert(StyleName, SyntaxHighlightingRule_t(mp_ConfFile->value("color","").toString(),
                                                     mp_ConfFile->value("decoration","").toStringList()));
    mp_ConfFile->endGroup();
  }

   mp_ConfFile->endGroup();

   return Rules;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevSyntaxHighlightingRules(const SyntaxHighlightingRules_t& Rules)
{
  mp_ConfFile->beginGroup("openfluid.waresdev.texteditor/syntax_highlighting");

  for(SyntaxHighlightingRules_t::const_iterator it = Rules.begin(); it != Rules.end(); ++it)
  {
    mp_ConfFile->beginGroup(it.key());

    mp_ConfFile->setValue("color",it.value().m_Color);
    mp_ConfFile->setValue("decoration",it.value().m_Decoration);

    mp_ConfFile->endGroup();
  }

  mp_ConfFile->endGroup();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevCurrentlineHighlightingEnabled()
{
  return mp_ConfFile->value("openfluid.waresdev.texteditor/currentline_highlighting/enabled",true).toBool();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevCurrentlineHighlightingEnabled(bool Enabled)
{
  mp_ConfFile->setValue("openfluid.waresdev.texteditor/currentline_highlighting/enabled",Enabled);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getWaresdevCurrentlineColor()
{
  return mp_ConfFile->value("openfluid.waresdev.texteditor/currentline_highlighting/color","").toString();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevCurrentlineColor(const QString& Color)
{
  mp_ConfFile->setValue("openfluid.waresdev.texteditor/currentline_highlighting/color",Color);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getWaresdevFontName()
{
  return mp_ConfFile->value("openfluid.waresdev.texteditor/fontname","").toString();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevFontName(const QString& FontName)
{
  mp_ConfFile->setValue("openfluid.waresdev.texteditor/fontname",FontName);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevLineWrappingEnabled()
{
  return mp_ConfFile->value("openfluid.waresdev.texteditor/linewrapping/enabled",true).toBool();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevLineWrappingEnabled(bool Enabled)
{
  mp_ConfFile->setValue("openfluid.waresdev.texteditor/linewrapping/enabled",Enabled);
  mp_ConfFile->sync();
}

// =====================================================================
// =====================================================================


int PreferencesManager::getWaresdevIndentSpaceNb()
{
  return mp_ConfFile->value("openfluid.waresdev.texteditor/indent/spacenumber",0).toInt();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevIndentSpaceNb(int SpaceNumber)
{
  mp_ConfFile->setValue("openfluid.waresdev.texteditor/indent/spacenumber",SpaceNumber);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getWaresdevConfigEnv(const QString& Name)
{
  return mp_ConfFile->value("openfluid.waresdev.commands/config/env/"+Name,"").toString();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevConfigEnv(const QString& Name,const QString& Value)
{
  mp_ConfFile->setValue("openfluid.waresdev.commands/config/env/"+Name,Value);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getWaresdevConfigOptions()
{
  return mp_ConfFile->value("openfluid.waresdev.commands/config/options","").toString();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevConfigOptions(const QString& Options)
{
  mp_ConfFile->setValue("openfluid.waresdev.commands/config/options",Options);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getWaresdevConfigGenerator()
{
  QString Generator;
  mp_ConfFile->beginGroup("openfluid.waresdev.commands");
#ifdef Q_OS_WIN32
  if (!mp_ConfFile->contains("config/generator"))
    mp_ConfFile->setValue("config/generator","MinGW Makefiles");
#endif
  Generator = mp_ConfFile->value("config/generator","").toString();
  mp_ConfFile->endGroup();

  return Generator;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevConfigGenerator(const QString& Generator)
{
  mp_ConfFile->setValue("openfluid.waresdev.commands/config/generator",Generator);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getWaresdevBuildEnv(const QString& Name)
{
  return mp_ConfFile->value("openfluid.waresdev.commands/build/env/"+Name,"").toString();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevBuildEnv(const QString& Name,const QString& Value)
{
  mp_ConfFile->setValue("openfluid.waresdev.commands/build/env/"+Name,Value);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isWaresdevShowCommandEnv(const QString& Name)
{
  bool Shown;
  mp_ConfFile->beginGroup("openfluid.waresdev.commands");
#ifdef Q_OS_WIN32
  if (!mp_ConfFile->contains("showenv/"+Name))
    mp_ConfFile->setValue("showenv/"+Name,false);
#endif
  Shown = mp_ConfFile->value("showenv/"+Name,false).toBool();
  mp_ConfFile->endGroup();

  return Shown;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresdevShowCommandEnv(const QString& Name, bool Enabled)
{
  mp_ConfFile->setValue("openfluid.waresdev.commands/showenv/"+Name,Enabled);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::isSslNoVerify()
{
  return mp_ConfFile->value("openfluid.waresdev.commands/sslNoVerify", false).toBool();
}


// =====================================================================
// =====================================================================


void PreferencesManager::setSslNoVerify(bool NoVerify)
{
  mp_ConfFile->setValue("openfluid.waresdev.commands/sslNoVerify", NoVerify);
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


} } //namespaces

