/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file PreferencesManager.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "PreferencesManager.hpp"

#include <boost/filesystem.hpp>
#include <QDir>
#include <QFileInfo>
#include <QLocale>

#include <openfluid/config.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/FrameworkException.hpp>

namespace openfluid { namespace guicommon {

PreferencesManager* PreferencesManager::mp_Instance = NULL;
QString PreferencesManager::m_FileName = "";
const int PreferencesManager::RecentProjectsLimit = 10;

// =====================================================================
// =====================================================================


PreferencesManager* PreferencesManager::getInstance()
{
  if (!mp_Instance)
    mp_Instance = new PreferencesManager();

  return mp_Instance;
}

// =====================================================================
// =====================================================================


PreferencesManager::PreferencesManager():
  mp_ConfFile(NULL)
{
  if (m_FileName.isEmpty())
  {
    m_FileName = QString(openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile().c_str());
    // TODO qt suffix to remove
    m_FileName += "qt";
  }



  QDir FileDir = QFileInfo(m_FileName).path();

  if (!FileDir.exists() && !QDir::root().mkpath(FileDir.absolutePath()))
    throw openfluid::base::FrameworkException("PreferencesManager::PreferencesManager",
                                              "Cannot create " + FileDir.path().toStdString() + " directory");

  mp_ConfFile = new QSettings(QString(m_FileName),QSettings::IniFormat);

  if (!QFileInfo(m_FileName).exists())
  {
    setDefaultValues();
    mp_ConfFile->sync();
  }
}

// =====================================================================
// =====================================================================


PreferencesManager::~PreferencesManager()
{
  mp_Instance = 0;
}

// =====================================================================
// =====================================================================

void PreferencesManager::setDefaultValues()
{
  setRecentMax(5);
  setDeltaT(300);
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
    throw openfluid::base::FrameworkException(
        "PreferencesManager::setFileName",
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
  if (!mp_ConfFile->contains("lang")) mp_ConfFile->setValue("lang",guessLang());
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
  QMFiles = QDir(QString(openfluid::base::RuntimeEnvironment::getInstance()->getTranslationsDir().c_str())).entryList(QStringList("*.qm"),QDir::Files);

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


void PreferencesManager::setRecentMax(unsigned int RecentMax)
{
  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  if (RecentMax > RecentProjectsLimit) RecentMax = RecentProjectsLimit;
  mp_ConfFile->setValue("recentmax",RecentMax);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}

// =====================================================================
// =====================================================================


unsigned int PreferencesManager::getRecentMax()
{
  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  if (!mp_ConfFile->contains("recentmax")) mp_ConfFile->setValue("recentmax",(unsigned int)(RecentProjectsLimit/2));
  unsigned int RecentMax = mp_ConfFile->value("recentmax").toUInt();
  mp_ConfFile->endGroup();
  return RecentMax;
}


// =====================================================================
// =====================================================================


bool PreferencesManager::addRecentProject(const QString& ProjectName,
                                          const QString& ProjectPath)
{
  if (ProjectPath.indexOf("=") >= 0)
    return false;

  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  QStringList Recents = mp_ConfFile->value("list").toStringList();
  mp_ConfFile->endGroup();

  QString NewRecentPair = ProjectName+"|"+ProjectPath;
  int RecentMax = getRecentMax();

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


void PreferencesManager::clearRecentProjects()
{
  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  mp_ConfFile->remove("list");
  mp_ConfFile->endGroup();

}


// =====================================================================
// =====================================================================


void PreferencesManager::adaptRecentProjects()
{
  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  QStringList Recents = mp_ConfFile->value("list").toStringList();
  mp_ConfFile->endGroup();

  int RecentMax = getRecentMax();

  while (Recents.size() > RecentMax) Recents.removeLast();

  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  mp_ConfFile->setValue("list",Recents);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


PreferencesManager::RecentProjectsList_t PreferencesManager::getRecentProjects()
{
  mp_ConfFile->beginGroup("openfluid.builder.recentprojects");
  QStringList Recents = mp_ConfFile->value("list").toStringList();
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


void PreferencesManager::setWorkdir(const QString& Workdir)
{
  mp_ConfFile->beginGroup("openfluid.builder.paths");
  mp_ConfFile->setValue("workdir",Workdir);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}

// =====================================================================
// =====================================================================


QString PreferencesManager::getWorkdir()
{
  mp_ConfFile->beginGroup("openfluid.builder.paths");
  QString Dir = mp_ConfFile->value("workdir").toString();
  mp_ConfFile->endGroup();

  if (Dir.isEmpty())
  {
    Dir = QString(openfluid::base::RuntimeEnvironment::getInstance()->getUserHomeDir().c_str())+"/OpenFLUID-Projects";
  }
  return Dir;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setExtraPaths(const QString& Key, const QStringList& Paths)
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


void PreferencesManager::addExtraPath(const QString& Key, const QString& Path)
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


void PreferencesManager::removeExtraPath(const QString& Key, const QString& Path)
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


QStringList PreferencesManager::getExtraPaths(const QString& Key)
{
  mp_ConfFile->beginGroup("openfluid.builder.paths");
  QStringList Paths = mp_ConfFile->value(Key).toStringList();
  mp_ConfFile->endGroup();

  return Paths;
}


// =====================================================================
// =====================================================================



void PreferencesManager::setExtraSimulatorsPaths(const QStringList& Paths)
{
  setExtraPaths("extrasimpaths",Paths);
}

// =====================================================================
// =====================================================================


void PreferencesManager::addExtraSimulatorsPath(const QString& Path)
{
  addExtraPath("extrasimpaths",Path);
}

// =====================================================================
// =====================================================================


void PreferencesManager::removeExtraSimulatorsPath(const QString& Path)
{
  removeExtraPath("extrasimpaths",Path);
}

// =====================================================================
// =====================================================================


QStringList PreferencesManager::getExtraSimulatorsPaths()
{
  return getExtraPaths("extrasimpaths");
}


// =====================================================================
// =====================================================================


void PreferencesManager::setExtraExtensionsPaths(const QStringList& Paths)
{
  setExtraPaths("extraextpaths",Paths);
}


// =====================================================================
// =====================================================================


void PreferencesManager::addExtraExtensionsPath(const QString& Path)
{
  addExtraPath("extraextpaths",Path);
}


// =====================================================================
// =====================================================================


void PreferencesManager::removeExtraExtensionsPath(const QString& Path)
{
  removeExtraPath("extraextpaths",Path);
}

// =====================================================================
// =====================================================================


QStringList PreferencesManager::getExtraExtensionsPaths()
{
  return getExtraPaths("extraextpaths");
}


// =====================================================================
// =====================================================================

void PreferencesManager::setExtraObserversPaths(const QStringList& Paths)
{
  setExtraPaths("extraobspaths",Paths);
}


// =====================================================================
// =====================================================================


void PreferencesManager::addExtraObserversPath(const QString& Path)
{
  addExtraPath("extraobspaths",Path);
}

// =====================================================================
// =====================================================================


void PreferencesManager::removeExtraObserversPath(const QString& Path)
{
  removeExtraPath("extraobspaths",Path);
}

// =====================================================================
// =====================================================================


QStringList PreferencesManager::getExtraObserversPaths()
{
  return getExtraPaths("extraobspaths");
}

// =====================================================================
// =====================================================================


void PreferencesManager::setDeltaT(openfluid::core::Duration_t DeltaT)
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  mp_ConfFile->setValue("deltat",DeltaT);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}

// =====================================================================
// =====================================================================


openfluid::core::Duration_t PreferencesManager::getDeltaT()
{
  openfluid::core::Duration_t DeltaT;

  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  if (!mp_ConfFile->contains("deltat")) mp_ConfFile->setValue("deltat",300);
  DeltaT = mp_ConfFile->value("deltat").toUInt();
  mp_ConfFile->endGroup();

  return DeltaT;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setBegin(const QString& Begin)
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  mp_ConfFile->setValue("begin",Begin);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getBegin()
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  QString DateStr = mp_ConfFile->value("begin").toString();
  mp_ConfFile->endGroup();

  if (DateStr.isEmpty())
  {
    std::string Now = boost::posix_time::to_iso_extended_string(boost::posix_time::microsec_clock::local_time());
    Now[10] = ' ';
    openfluid::core::DateTime DT;
    DT.setFromISOString(Now);
    setBegin(QString(DT.getAsISOString().c_str()));
    DateStr = QString(DT.getAsISOString().c_str());
  }

  return DateStr;
}


// =====================================================================
// =====================================================================

void PreferencesManager::setEnd(const QString& End)
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  mp_ConfFile->setValue("end",End);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();

}

// =====================================================================
// =====================================================================


QString PreferencesManager::getEnd()
{
  mp_ConfFile->beginGroup("openfluid.builder.runconfig");
  QString DateStr = mp_ConfFile->value("end").toString();
  mp_ConfFile->endGroup();

  if (DateStr.isEmpty())
  {
    openfluid::core::DateTime DT;
    DT.setFromISOString(getBegin().toStdString());
    DT = DT + openfluid::core::DateTime::Day();
    setEnd(QString(DT.getAsISOString().c_str()));
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


bool PreferencesManager::isExtensionValueExist(const QString& PluginName, const QString& Key)
{
  QString GroupName = "openfluid.builder.extensions:"+PluginName;

  return isValidKey(GroupName,Key);
}


// =====================================================================
// =====================================================================


QString PreferencesManager::getExtensionValue(const QString& PluginName,
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


void PreferencesManager::setExtensionValue(const QString& PluginName,
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


Qt::DockWidgetArea PreferencesManager::getDockPosition()
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


void PreferencesManager::setDockPosition(Qt::DockWidgetArea Position)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("dockpos",Position);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


Qt::ToolBarArea PreferencesManager::getToolBarPosition()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("toolbarpos")) mp_ConfFile->setValue("toolbarpos",Qt::TopToolBarArea);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  Qt::ToolBarArea Pos = Qt::ToolBarArea(mp_ConfFile->value("toolbarpos").toInt());
  mp_ConfFile->endGroup();

  return Pos;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setToolBarPosition(Qt::ToolBarArea Position)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("toolbarpos",Position);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::getItemRemovalConfirm()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("itemremovalconfirm")) mp_ConfFile->setValue("itemremovalconfirm",true);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool Confirm = mp_ConfFile->value("itemremovalconfirm").toBool();
  mp_ConfFile->endGroup();

  return Confirm;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setItemRemovalConfirm(bool Confirm)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("itemremovalconfirm",Confirm);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::getParamRemovalConfirm()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("paramremovalconfirm")) mp_ConfFile->setValue("paramremovalconfirm",true);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool Confirm = mp_ConfFile->value("paramremovalconfirm").toBool();
  mp_ConfFile->endGroup();

  return Confirm;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setParamRemovalConfirm(bool Confirm)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("paramremovalconfirm",Confirm);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}


// =====================================================================
// =====================================================================


bool PreferencesManager::getWaresWatcher()
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  if (!mp_ConfFile->contains("wareswatchers")) mp_ConfFile->setValue("wareswatchers",true);
  mp_ConfFile->endGroup();

  mp_ConfFile->beginGroup("openfluid.builder.interface");
  bool Watchers = mp_ConfFile->value("wareswatchers").toBool();
  mp_ConfFile->endGroup();

  return Watchers;
}


// =====================================================================
// =====================================================================


void PreferencesManager::setWaresWatcher(bool Active)
{
  mp_ConfFile->beginGroup("openfluid.builder.interface");
  mp_ConfFile->setValue("wareswatchers",Active);
  mp_ConfFile->endGroup();
  mp_ConfFile->sync();
}

} } //namespaces

