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
 \file WareSrcManager.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/waresdev/WareSrcManager.hpp>

#include <QDir>

#include <openfluid/config.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace waresdev {


WareSrcManager* WareSrcManager::mp_Instance = 0;


// =====================================================================
// =====================================================================


WareSrcManager::WareSrcManager() :
    m_WaresdevDirName(
        QString::fromStdString(openfluid::config::WARESDEV_SUBDIR))
{
  switchWorkspace(
      openfluid::base::PreferencesManager::getInstance()->getWorkspacePath());
}


// =====================================================================
// =====================================================================


WareSrcManager::~WareSrcManager()
{

}


// =====================================================================
// =====================================================================


WareSrcManager* WareSrcManager::getInstance()
{
  if (!mp_Instance)
    mp_Instance = new WareSrcManager();

  return mp_Instance;
}


// =====================================================================
// =====================================================================


void WareSrcManager::switchWorkspace(const QString& NewAbsoluteWorkspacePath)
{
  m_WorkspacePath = QDir(NewAbsoluteWorkspacePath).absolutePath();

  m_WareTypePathByWareType[SIMULATOR] = QString("%1/%2/%3").arg(m_WorkspacePath)
      .arg(m_WaresdevDirName).arg(
      QString::fromStdString(openfluid::config::SIMULATORS_PLUGINS_SUBDIR));

  m_WareTypePathByWareType[OBSERVER] = QString("%1/%2/%3").arg(m_WorkspacePath)
      .arg(m_WaresdevDirName).arg(
      QString::fromStdString(openfluid::config::OBSERVERS_PLUGINS_SUBDIR));

  m_WareTypePathByWareType[BUILDEREXT] = QString("%1/%2/builderexts").arg(
      m_WorkspacePath).arg(m_WaresdevDirName);

  foreach(QString Path,m_WareTypePathByWareType){
  if (!QDir(Path).mkpath(Path))
  throw openfluid::base::FrameworkException(
      "waresdev::WareSrcManager::switchWorkspace",
      QString("unable to open or create %1 directory").arg(Path).toStdString());
}
}


// =====================================================================
// =====================================================================


QString WareSrcManager::getWareTypePath(WareType WareSrcType)
{
  if (m_WareTypePathByWareType.contains(WareSrcType))
    return m_WareTypePathByWareType.value(WareSrcType);

  throw openfluid::base::FrameworkException(
      "waresdev::WareSrcManager::getWareTypePath", "unknown source type");
}


// =====================================================================
// =====================================================================

// TODO later, pass or use other workspace paths ?
WareSrcManager::PathInfo WareSrcManager::getPathInfo(const QString& Path)
{
  PathInfo Info;

  Info.m_AbsolutePath = QDir(Path).absolutePath();

  QString RelToWorkspace = QDir(m_WorkspacePath).relativeFilePath(
      Info.m_AbsolutePath);

  Info.m_IsInCurrentWorkspace = !RelToWorkspace.startsWith("..");


  for (QMap<WareType, QString>::iterator it = m_WareTypePathByWareType.begin();
      it != m_WareTypePathByWareType.end(); ++it)
  {
    QString RelToWareType = QDir(it.value()).relativeFilePath(
        Info.m_AbsolutePath);

    if (RelToWareType.startsWith(".."))
      continue;

    Info.m_WareType = it.key();
    Info.m_WareName = RelToWareType.split("/").at(0);

    Info.m_AbsolutePathOfWare = QDir(it.value()).filePath(Info.m_WareName);

    if (!QFileInfo(Info.m_AbsolutePathOfWare).isDir())
      break;

    if (Info.m_AbsolutePathOfWare == Info.m_AbsolutePath)
    {
      Info.m_isAWare = true;
    }
    else if (!QFileInfo(Info.m_AbsolutePath).isDir())
    {
      Info.m_isAWareFile = true;
      Info.m_RelativePathToWareDir = QDir(Info.m_AbsolutePathOfWare)
          .relativeFilePath(Info.m_AbsolutePath);
      Info.m_FileName = QFileInfo(Info.m_AbsolutePath).fileName();
    }
  }

  return Info;
}


// =====================================================================
// =====================================================================


} }  // namespaces
