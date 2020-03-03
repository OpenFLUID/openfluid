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
 @file WareSrcManager.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QDir>

#include <openfluid/config.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/waresdev/WareSrcManager.hpp>


namespace openfluid { namespace waresdev {


OPENFLUID_SINGLETON_INITIALIZATION(WareSrcManager)


WareSrcManager::WareSrcManager() :
    m_WaresdevDirName(QString::fromStdString(openfluid::config::WARESDEV_PATH))
{
  openfluid::base::Environment::init();

  switchWorkspace(openfluid::base::PreferencesManager::instance()->getBuilderWorkspacePath());
}


// =====================================================================
// =====================================================================


WareSrcManager::~WareSrcManager()
{

}


// =====================================================================
// =====================================================================


void WareSrcManager::switchWorkspace(const QString& NewAbsoluteWorkspacePath)
{
  m_WorkspacePath = QDir(NewAbsoluteWorkspacePath).absolutePath();

  m_WaresdevPath = QDir(m_WorkspacePath).absoluteFilePath(m_WaresdevDirName);

  QDir WaresdevDir(m_WaresdevPath);

  m_WareTypePathByWareType[openfluid::ware::WareType::SIMULATOR] = WaresdevDir.filePath(
      QString::fromStdString(openfluid::config::SIMULATORS_PATH));
  m_WareTypePathByWareType[openfluid::ware::WareType::OBSERVER] = WaresdevDir.filePath(
      QString::fromStdString(openfluid::config::OBSERVERS_PATH));
  m_WareTypePathByWareType[openfluid::ware::WareType::BUILDEREXT] = WaresdevDir.filePath(
      QString::fromStdString(openfluid::config::BUILDEREXTS_PATH));

  for(const QString& Path : m_WareTypePathByWareType)
  {
    if (!QDir(Path).mkpath(Path))
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
        QString("unable to open or create %1 directory").arg(Path).toStdString());
    }
  }
}


// =====================================================================
// =====================================================================


QString WareSrcManager::getWareTypePath(openfluid::ware::WareType WareSrcType)
{
  if (m_WareTypePathByWareType.contains(WareSrcType))
  {
    return m_WareTypePathByWareType.value(WareSrcType);
  }

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "unknown source type");
}


// =====================================================================
// =====================================================================


QString WareSrcManager::getWarePath(const QString& WareID, openfluid::ware::WareType OFWareType,
                                    QString& ErrMsg)
{
  openfluid::ware::WareType Type;

  switch (OFWareType)
  {
    case openfluid::ware::WareType::SIMULATOR:
      Type = openfluid::ware::WareType::SIMULATOR;
      break;
    case openfluid::ware::WareType::OBSERVER:
      Type = openfluid::ware::WareType::OBSERVER;
      break;
    default:
      ErrMsg = QObject::tr("Unknown ware type");
      return "";
  }

  QDir Dir(getWareTypePath(Type));

  // TODO later, add check of other workspaces than current
  if (Dir.exists(WareID))
  {
    return Dir.filePath(WareID);
  }
  
  else
  {
    // check in example dir
    QDir ExampleDir = QDir(QString::fromStdString(openfluid::base::Environment::getUserExampleSimulatorsDir()));
    
    if (ExampleDir.exists(WareID))
    {
      return ExampleDir.filePath(WareID);
    }
  }

  ErrMsg = QObject::tr("Unable to find \"%1\" in the current workspace.").arg(WareID);
  return "";
}


// =====================================================================
// =====================================================================


// TODO later, pass or use other workspace paths ?
WareSrcManager::PathInfo WareSrcManager::getPathInfo(const QString& Path)
{
  PathInfo Info;

  Info.m_AbsolutePath = QDir::cleanPath(QDir(Path).absolutePath());

  // we *are* current workspace
  if (Info.m_AbsolutePath == m_WorkspacePath)
  {
    return Info;
  }

  // we are not under current workspace
  if (Info.m_AbsolutePath.indexOf(m_WorkspacePath) != 0)
  {
    QString UserExampleSimulatorsDir = QString::fromStdString(
      openfluid::base::Environment::getUserExampleSimulatorsDir());
    QDir ExampleDir = QDir(UserExampleSimulatorsDir);
    
    // we are under example dir
    if (Info.m_AbsolutePath.indexOf(UserExampleSimulatorsDir) == 0)
    {
      Info.m_IsInExamples = true;
      // check if example simulator
      QString RelToExample = ExampleDir.relativeFilePath(Info.m_AbsolutePath);
      QString PotentialWareName = RelToExample.section('/', 0, 0, QString::SectionSkipEmpty);
      
      Info.m_isAWare = true;
      Info.m_WareName = PotentialWareName;
      Info.m_AbsolutePathOfWare = Info.m_AbsolutePath;
      Info.m_WareType = openfluid::ware::WareType::SIMULATOR;
      Info.m_RelativePathToWareDir = QDir(Info.m_AbsolutePathOfWare).relativeFilePath(Info.m_AbsolutePath);
      
      QFileInfo FileInfo(Info.m_AbsolutePath);

      Info.m_isAWareFile = FileInfo.isFile();
      if (Info.m_isAWareFile)
      {
        Info.m_isAWare = false;
        Info.m_FileName = FileInfo.fileName();
      }
    }
    return Info;
  }

  Info.m_IsInCurrentWorkspace = true;

  for (QMap<openfluid::ware::WareType,QString>::iterator it = m_WareTypePathByWareType.begin();
       it != m_WareTypePathByWareType.end(); ++it)
  {
    // we *are* this waretype directory
    if (Info.m_AbsolutePath == it.value())
    {
      return Info;
    }

    // we are not under this waretype directory
    if (Info.m_AbsolutePath.indexOf(it.value()) != 0)
    {
      continue;
    }

    QDir WareTypeDir(it.value());
    QString RelToWareType = WareTypeDir.relativeFilePath(Info.m_AbsolutePath);
    QString PotentialWareName = RelToWareType.section('/', 0, 0, QString::SectionSkipEmpty);
    QString PotentialWarePath = WareTypeDir.absoluteFilePath(PotentialWareName);

    // we are just under this waretype directory, but just a file out of any ware
    if (QFileInfo(PotentialWarePath).isFile())
    {
      return Info;
    }

    Info.m_WareName = PotentialWareName;
    Info.m_AbsolutePathOfWare = PotentialWarePath;
    Info.m_WareType = it.key();
    Info.m_RelativePathToWareDir = QDir(Info.m_AbsolutePathOfWare).relativeFilePath(Info.m_AbsolutePath);

    if (PotentialWarePath == Info.m_AbsolutePath)
    {
      Info.m_isAWare = true;
      return Info;
    }

    QFileInfo FileInfo(Info.m_AbsolutePath);

    Info.m_isAWareFile = FileInfo.isFile();
    if (Info.m_isAWareFile)
    {
      Info.m_FileName = FileInfo.fileName();
    }

    return Info;
  }

  return Info;
}


// =====================================================================
// =====================================================================


QString WareSrcManager::getWaresdevPath() const
{
  return m_WaresdevPath;
}


// =====================================================================
// =====================================================================


QString WareSrcManager::getWorkspacePath() const
{
  return m_WorkspacePath;
}


} }  // namespaces
