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
  @file RunContextManager.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace base {


OPENFLUID_SINGLETON_INITIALIZATION(RunContextManager)

QString RunContextManager::m_ProjectFileGroupName = "OpenFLUID Project";


// =====================================================================
// =====================================================================


RunContextManager::RunContextManager() :
  Environment(),
  m_IsClearOutputDir(false), m_IsProfiling(false),
  m_ValuesBufferSize(0),
  mp_ProjectFile(nullptr),
  m_ProjectIncOutputDir(false), m_ProjectIsOpen(false)
{
  openfluid::base::Environment::init();

  m_InputDir = openfluid::tools::Filesystem::joinPath({m_UserDataDir,openfluid::config::DEFAULT_INPUT_PATH});
  m_OutputDir = openfluid::tools::Filesystem::joinPath({m_UserDataDir,openfluid::config::DEFAULT_OUTPUT_PATH});

  m_WaresMaxNumThreads = m_IdealThreadCount;

  m_WaresSharedEnvironment.setString("dir.temp", m_TempDir);
  m_WaresSharedEnvironment.setBoolean("mode.clearoutput",m_IsClearOutputDir);

  updateWaresEnvironment();
}


// =====================================================================
// =====================================================================


RunContextManager::~RunContextManager()
{
  closeProject();
}


// =====================================================================
// =====================================================================


std::string RunContextManager::getNow()
{
  return QDateTime::currentDateTime().toString("yyyyMMdd'T'hhmmss").toStdString();
}


// =====================================================================
// =====================================================================


std::string RunContextManager::getFilePathFromProjectPath(const std::string& ProjectPath)
{
  return openfluid::tools::Filesystem::joinPath({ProjectPath,openfluid::config::PROJECT_FILE});
}


// =====================================================================
// =====================================================================


std::string RunContextManager::getInputDirFromProjectPath(const std::string& ProjectPath)
{
  return openfluid::tools::Filesystem::joinPath({ProjectPath,openfluid::config::PROJECT_INPUTDIR});
}


// =====================================================================
// =====================================================================


std::string RunContextManager::getOuputDirFromProjectPath(const std::string& ProjectPath)
{
  return openfluid::tools::Filesystem::joinPath({ProjectPath,openfluid::config::PROJECT_OUTPUTDIRPREFIX});
}


// =====================================================================
// =====================================================================


bool RunContextManager::checkProject(const std::string& ProjectPath)
{
  std::string AbsPath = openfluid::tools::Filesystem::absolutePath(ProjectPath);

  std::string PrjFilePath = getFilePathFromProjectPath(AbsPath);

  // TODO to be removed in a later version (after mid-2016)
  // try to convert a former .openfluidprj project
  if (!openfluid::tools::Filesystem::isFile(PrjFilePath))
  {
    std::string FormerPrjFilePath = openfluid::tools::Filesystem::joinPath({AbsPath,".openfluidprj"});

    // Rename the former .openfluidprj file into the new name, only if exists
    if (openfluid::tools::Filesystem::isFile(FormerPrjFilePath))
    {
      openfluid::tools::Filesystem::renameFile(FormerPrjFilePath,PrjFilePath);
    }
  }


  if (!openfluid::tools::Filesystem::isFile(PrjFilePath))
  {
    return false;
  }

  if (!openfluid::tools::Filesystem::isDirectory(getInputDirFromProjectPath(AbsPath)))
  {
    return false;
  }

  QSettings PrjFile(QString::fromStdString(PrjFilePath),QSettings::IniFormat);

  PrjFile.beginGroup(m_ProjectFileGroupName);
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


void RunContextManager::updateWaresEnvironment()
{
  m_WaresSharedEnvironment.setString("dir.input", m_InputDir);
  m_WaresSharedEnvironment.setString("dir.output", m_OutputDir);
}


// =====================================================================
// =====================================================================


std::string RunContextManager::getInputFullPath(const std::string& Filename) const
{
  return openfluid::tools::Filesystem::joinPath({m_InputDir,Filename});
}


// =====================================================================
// =====================================================================


std::string RunContextManager::getOutputFullPath(const std::string& Filename) const
{
  return openfluid::tools::Filesystem::joinPath({m_OutputDir,Filename});
}


// =====================================================================
// =====================================================================


void RunContextManager::resetWaresMaxNumThreads()
{
  m_WaresMaxNumThreads = m_IdealThreadCount;
}


// =====================================================================
// =====================================================================


bool RunContextManager::openProject(const std::string& Path)
{
  if (!m_ProjectIsOpen && checkProject(Path))
  {
    m_ProjectPath = openfluid::tools::Filesystem::absolutePath(Path);
    m_InputDir = getInputDirFromProjectPath(m_ProjectPath);
    m_OutputDir = getOuputDirFromProjectPath(m_ProjectPath);

    QString PrjFilePath = QString::fromStdString(getFilePathFromProjectPath(m_ProjectPath));
    mp_ProjectFile = new QSettings(PrjFilePath,QSettings::IniFormat);

    mp_ProjectFile->beginGroup(m_ProjectFileGroupName);
    m_ProjectName = openfluid::tools::fromIniCompatible(mp_ProjectFile->value("Name"));
    m_ProjectDescription = openfluid::tools::fromIniCompatible(mp_ProjectFile->value("Description"));
    m_ProjectAuthors = openfluid::tools::fromIniCompatible(mp_ProjectFile->value("Authors"));
    m_ProjectCreationDate = mp_ProjectFile->value("CreationDate").toString().toStdString();
    m_ProjectLastModDate = mp_ProjectFile->value("LastModDate").toString().toStdString();
    m_ProjectIncOutputDir = mp_ProjectFile->value("IncOutput").toBool();
    mp_ProjectFile->endGroup();

    updateWaresEnvironment();

    m_ProjectIsOpen = true;

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool RunContextManager::createProject(const std::string& Path,
                                      const std::string& Name, const std::string& Description,
                                      const std::string& Authors, const bool Inc)
{
  if (m_ProjectIsOpen)
  {
    return false;
  }

  std::string AbsPath = openfluid::tools::Filesystem::absolutePath(Path);

  if (openfluid::tools::Filesystem::makeDirectory(getInputDirFromProjectPath(AbsPath)))
  {
    m_ProjectPath = AbsPath;
    m_InputDir = getInputDirFromProjectPath(AbsPath);
    m_OutputDir = getOuputDirFromProjectPath(AbsPath);

    QString PrjFilePath = QString::fromStdString(getFilePathFromProjectPath(AbsPath));
    mp_ProjectFile = new QSettings(PrjFilePath,QSettings::IniFormat);

    m_ProjectName = Name;
    m_ProjectDescription = Description;
    m_ProjectAuthors = Authors;
    m_ProjectIncOutputDir = Inc;
    m_ProjectCreationDate = getNow();

    updateWaresEnvironment();

    m_ProjectIsOpen = true;

    return saveProject();
  }

  return false;
}


// =====================================================================
// =====================================================================


bool RunContextManager::saveProject()
{
  if (!m_ProjectIsOpen)
  {
    return false;
  }

  m_ProjectLastModDate = getNow();

  mp_ProjectFile->beginGroup(m_ProjectFileGroupName);
  mp_ProjectFile->setValue("Name",openfluid::tools::toIniCompatible(m_ProjectName));
  mp_ProjectFile->setValue("Description",openfluid::tools::toIniCompatible(m_ProjectDescription));
  mp_ProjectFile->setValue("Authors",openfluid::tools::toIniCompatible(m_ProjectAuthors));
  mp_ProjectFile->setValue("CreationDate",QString::fromStdString(m_ProjectCreationDate));
  mp_ProjectFile->setValue("LastModDate",QString::fromStdString(m_ProjectLastModDate));
  mp_ProjectFile->setValue("IncOutput",m_ProjectIncOutputDir);
  mp_ProjectFile->endGroup();
  mp_ProjectFile->sync();

  return true;
}


// =====================================================================
// =====================================================================


void RunContextManager::closeProject()
{
  if (m_ProjectIsOpen)
  {
    m_ProjectPath = m_InputDir = m_OutputDir =
                    m_ProjectName = m_ProjectDescription = m_ProjectAuthors =
                    m_ProjectCreationDate = m_ProjectLastModDate = "";

    m_ProjectIncOutputDir = m_ProjectIsOpen = false;

    mp_ProjectFile->sync();

    delete mp_ProjectFile;
    mp_ProjectFile = nullptr;
  }
}


// =====================================================================
// =====================================================================


void RunContextManager::setInputDir(const std::string& InputDir)
{
  closeProject();
  m_InputDir = openfluid::tools::Filesystem::absolutePath(InputDir);
  updateWaresEnvironment();
}


// =====================================================================
// =====================================================================


void RunContextManager::setOutputDir(const std::string& OutputDir)
{
  closeProject();
  m_OutputDir = openfluid::tools::Filesystem::absolutePath(OutputDir);
  updateWaresEnvironment();
}


// =====================================================================
// =====================================================================


void RunContextManager::setDateTimeOutputDir()
{
  closeProject();

  std::string DateDir = "OPENFLUID."+QDateTime::currentDateTime().toString("yyyyMMdd'T'hhmmss").toStdString()+".OUT";
  m_OutputDir = openfluid::tools::Filesystem::joinPath({m_UserDataDir,DateDir});

  updateWaresEnvironment();
}


// =====================================================================
// =====================================================================


void RunContextManager::updateProjectOutputDir()
{
  if (m_ProjectIsOpen && m_ProjectIncOutputDir)
  {
    std::string Now = getNow();
    Now.replace(8,1,"-");

    m_OutputDir = getOuputDirFromProjectPath(m_ProjectPath).append("_").append(Now);
  }
  else
  {
    m_OutputDir = getOuputDirFromProjectPath(m_ProjectPath);
  }
}


// =====================================================================
// =====================================================================


bool RunContextManager::isProject(const std::string& Path)
{
  return checkProject(openfluid::tools::Filesystem::absolutePath(Path));
}


// =====================================================================
// =====================================================================


bool RunContextManager::getProjectInfos(const std::string& Path,
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

    PrjFile.beginGroup(m_ProjectFileGroupName);
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


QVariant RunContextManager::getProjectConfigValue(const QString& Group, const QString& Key) const
{
  if (!m_ProjectIsOpen)
  {
    return QVariant();
  }

  return mp_ProjectFile->value(Group+"/"+Key);
}


// =====================================================================
// =====================================================================


void RunContextManager::setProjectConfigValue(const QString& Group, const QString& Key, const QVariant& Value)
{
  if (!m_ProjectIsOpen)
  {
    return;
  }

  mp_ProjectFile->setValue(Group+"/"+Key,Value);
}


// =====================================================================
// =====================================================================


void RunContextManager::removeProjectConfigValue(const QString& Group, const QString& Key)
{
  if (!m_ProjectIsOpen)
  {
    return;
  }

  mp_ProjectFile->remove(Group+"/"+Key);
}


// =====================================================================
// =====================================================================


void RunContextManager::processWareParams(openfluid::ware::WareParams_t& Params) const
{
  std::map<std::string,std::string> Replacements =
    {{"${dir.input}",m_InputDir},
     {"${dir.output}",m_OutputDir},
     {"${dir.temp}",m_TempDir}};

  for (auto& Param : Params)
  {
    for (const auto& R : Replacements)
    {
      Param.second.replaceAll(R.first,R.second);
    }
  }
}


} } //namespaces

