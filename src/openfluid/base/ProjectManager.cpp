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
  @file ProjectManager.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <QFile>
#include <QDir>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/tools/QtHelpers.hpp>

#include <openfluid/config.hpp>


namespace openfluid { namespace base {


ProjectManager* ProjectManager::mp_Singleton = NULL;
QString ProjectManager::m_GroupName = "OpenFLUID Project";


// =====================================================================
// =====================================================================


ProjectManager::ProjectManager() :
  mp_PrjFile(NULL),
  m_IsIncOutputDir(false), m_IsOpened(false)
{

}


// =====================================================================
// =====================================================================


ProjectManager::~ProjectManager()
{
  close();
}


// =====================================================================
// =====================================================================


ProjectManager* ProjectManager::instance()
{
  if (mp_Singleton == NULL)
    mp_Singleton = new ProjectManager();
  return mp_Singleton;
}


// =====================================================================
// =====================================================================


std::string ProjectManager::getNow()
{
  return QDateTime::currentDateTime().toString("yyyyMMdd'T'hhmmss").toStdString();
}


// =====================================================================
// =====================================================================


std::string ProjectManager::getFilePathFromProjectPath(std::string ProjectPath)
{
  return ProjectPath.append("/").append(openfluid::config::PROJECT_FILE);
}


// =====================================================================
// =====================================================================


std::string ProjectManager::getInputDirFromProjectPath(std::string ProjectPath)
{
  return ProjectPath.append("/").append(openfluid::config::PROJECT_INPUTDIR);
}


// =====================================================================
// =====================================================================


std::string ProjectManager::getOuputDirFromProjectPath(std::string ProjectPath)
{
  return ProjectPath.append("/").append(openfluid::config::PROJECT_OUTPUTDIRPREFIX);
}


// =====================================================================
// =====================================================================


bool ProjectManager::checkProject(const std::string& ProjectPath)
{
  QString PrjFilePath = QString::fromStdString(getFilePathFromProjectPath(ProjectPath));

  // TODO to be removed in a later version (after mid-2015)
  // try to convert a former .openfluidprj project
  if (!QFile::exists(PrjFilePath))
  {
    QString FormerPrjFilePath = QString::fromStdString(ProjectPath)+"/.openfluidprj";

    // Rename the former .openfluidprj file into the new name, only if exists
    if (QFile::exists(FormerPrjFilePath))

      QFile(FormerPrjFilePath).rename(PrjFilePath);
  }


  if (!QFile::exists(PrjFilePath))
    return false;

  if (!QDir().exists(QString::fromStdString(getInputDirFromProjectPath(ProjectPath))))
    return false;

  QSettings PrjFile(PrjFilePath,QSettings::IniFormat);

  PrjFile.beginGroup(m_GroupName);
  bool OK = PrjFile.contains("Name") &&
            PrjFile.contains("Description") &&
            PrjFile.contains("Authors") &&
            PrjFile.contains("CreationDate") &&
            PrjFile.contains("LastModDate");

  PrjFile.endGroup();

  return OK;
}



// =====================================================================
// =====================================================================


bool ProjectManager::open(const std::string& Path)
{
  m_Path = Path;
  m_InputDir = getInputDirFromProjectPath(Path);
  m_OutputDir = getOuputDirFromProjectPath(Path);

  if (!m_IsOpened && checkProject(Path))
  {
    QString PrjFilePath = QString::fromStdString(getFilePathFromProjectPath(Path));
    mp_PrjFile = new QSettings(PrjFilePath,QSettings::IniFormat);


    mp_PrjFile->beginGroup(m_GroupName);
    m_Name = openfluid::tools::fromIniCompatible(mp_PrjFile->value("Name"));
    m_Description = openfluid::tools::fromIniCompatible(mp_PrjFile->value("Description"));
    m_Authors = openfluid::tools::fromIniCompatible(mp_PrjFile->value("Authors"));
    m_CreationDate = mp_PrjFile->value("CreationDate").toString().toStdString();
    m_LastModDate = mp_PrjFile->value("LastModDate").toString().toStdString();
    m_IsIncOutputDir = mp_PrjFile->value("IncOutput").toBool();
    mp_PrjFile->endGroup();

    m_IsOpened = true;

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool ProjectManager::create(const std::string& Path,
    const std::string& Name, const std::string& Description,
    const std::string& Authors, const bool Inc)
{
  if (m_IsOpened) return false;

  if (QDir().mkpath(QString::fromStdString(getInputDirFromProjectPath(Path))))
  {
    m_Path = Path;
    m_InputDir = getInputDirFromProjectPath(Path);
    m_OutputDir = getOuputDirFromProjectPath(Path);

    QString PrjFilePath = QString::fromStdString(getFilePathFromProjectPath(Path));
    mp_PrjFile = new QSettings(PrjFilePath,QSettings::IniFormat);

    m_Name = Name;
    m_Description = Description;
    m_Authors = Authors;
    m_IsIncOutputDir = Inc;
    m_CreationDate = getNow();

    m_IsOpened = true;

    return save();
  }

  return false;
}


// =====================================================================
// =====================================================================


bool ProjectManager::save()
{
  if (!m_IsOpened) return false;

  m_LastModDate = getNow();

  mp_PrjFile->beginGroup(m_GroupName);
  mp_PrjFile->setValue("Name",openfluid::tools::toIniCompatible(m_Name));
  mp_PrjFile->setValue("Description",openfluid::tools::toIniCompatible(m_Description));
  mp_PrjFile->setValue("Authors",openfluid::tools::toIniCompatible(m_Authors));
  mp_PrjFile->setValue("CreationDate",QString::fromStdString(m_CreationDate));
  mp_PrjFile->setValue("LastModDate",QString::fromStdString(m_LastModDate));
  mp_PrjFile->setValue("IncOutput",m_IsIncOutputDir);
  mp_PrjFile->endGroup();
  mp_PrjFile->sync();

  return true;
}

// =====================================================================
// =====================================================================


void ProjectManager::close()
{
  if (m_IsOpened)
  {
    m_Path = m_InputDir = m_OutputDir = m_Name = m_Description = m_Authors
        = m_CreationDate = m_LastModDate = "";

    m_IsIncOutputDir = m_IsOpened = false;

    mp_PrjFile->sync();

    delete mp_PrjFile;
    mp_PrjFile = NULL;
  }
}


// =====================================================================
// =====================================================================


void ProjectManager::updateOutputDir()
{
  if (m_IsOpened && m_IsIncOutputDir)
  {
    std::string Now = getNow();
    Now.replace(8,1,"-");

    m_OutputDir = getOuputDirFromProjectPath(m_Path).append("_").append(Now);
  }
  else
    m_OutputDir = getOuputDirFromProjectPath(m_Path);
}


// =====================================================================
// =====================================================================


bool ProjectManager::isProject(const std::string& Path)
{
  return checkProject(Path);
}


// =====================================================================
// =====================================================================


bool ProjectManager::getProjectInfos(const std::string& Path,
                                     std::string& Name, std::string& Description, std::string& Authors,
                                     std::string& CreationDate, std::string& LastModDate)
{
  Name.clear();
  Description.clear();
  Authors.clear();
  CreationDate.clear();
  LastModDate.clear();

  if (checkProject(Path))
  {
    QString PrjFilePath = QString::fromStdString(getFilePathFromProjectPath(Path));
    QSettings PrjFile(PrjFilePath,QSettings::IniFormat);

    PrjFile.beginGroup(m_GroupName);
    Name = openfluid::tools::fromIniCompatible(PrjFile.value("Name"));
    Description = openfluid::tools::fromIniCompatible(PrjFile.value("Description"));
    Authors = openfluid::tools::fromIniCompatible(PrjFile.value("Authors"));
    CreationDate = PrjFile.value("CreationDate").toString().toStdString();
    LastModDate = PrjFile.value("LastModDate").toString().toStdString();
    PrjFile.endGroup();

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


QVariant ProjectManager::getConfigValue(const QString& Group, const QString& Key) const
{
  if (!m_IsOpened)
    return QVariant();

  return mp_PrjFile->value(Group+"/"+Key);
}


// =====================================================================
// =====================================================================


void ProjectManager::setConfigValue(const QString& Group, const QString& Key, const QVariant& Value)
{
  if (!m_IsOpened)
    return;

  mp_PrjFile->setValue(Group+"/"+Key,Value);
}



} } //namespaces


