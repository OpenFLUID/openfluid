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


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace base {


OPENFLUID_SINGLETON_INITIALIZATION(RunContextManager)


const std::string RunContextManager::m_ProjectRole = "openfluid-project";
const std::vector<std::string> RunContextManager::m_DeprecatedProjectFiles =
  {"openfluid-project.conf",".openfluid-project.conf"};


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


void RunContextManager::updateProjectFile(const std::string& ProjectFilePath)
{

#if (OPENFLUID_VERSION_NUMERIC >= OPENFLUID_VERSION_COMPUTE(2,3,0))
# warning "the project file format compatibility is deprecated and should be removed"
#endif


  if (!openfluid::tools::Filesystem::isFile(ProjectFilePath))
  {
    std::string FormerPrjFilePath = 
      openfluid::tools::Filesystem::joinPath({
        openfluid::tools::Filesystem::dirname(ProjectFilePath),
        m_DeprecatedProjectFiles.front()
      });

    if (openfluid::tools::Filesystem::isFile(FormerPrjFilePath))
    {
      boost::property_tree::ptree INI;
      boost::property_tree::ini_parser::read_ini(FormerPrjFilePath,INI);

      openfluid::tools::SettingsBackend SB(ProjectFilePath,m_ProjectRole,false);
      SB.setValue("/","name","");
      SB.setValue("/","description","");
      SB.setValue("/","authors","");
      SB.setValue("/","creation_date","");
      SB.setValue("/","lastmod_date","");
      SB.setValue("/","inc_outdir",false);


      auto cleanKeyValue = [](const std::string& Key, const std::string& Value)
      {
         std::string UnquotedVal = Value;
         if (Value.front() == '"' && Value.back() == '"')
         {
           UnquotedVal = Value.substr(1,UnquotedVal.length()-2);
         }

         return std::make_pair(
           openfluid::tools::trim(Key),
           openfluid::tools::trim(UnquotedVal)
         );
      };


      for(const auto& Section : INI)
      {
        if (Section.first == "OpenFLUID%20Project" || Section.first == "OpenFLUID Project")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());

            if (openfluid::tools::toLowerCase(Key) == "name")
            {
              SB.setValue("/","name",Value);
            }
            else if (openfluid::tools::toLowerCase(Key) == "authors")
            {
              SB.setValue("/","authors",Value);
            }
            else if (openfluid::tools::toLowerCase(Key) == "description")
            {
              SB.setValue("/","description",Value);
            }
            else if (openfluid::tools::toLowerCase(Key) == "creationdate")
            {
              SB.setValue("/","creation_date",Value);
            }
            else if (openfluid::tools::toLowerCase(Key) == "lastmoddate")
            {
              SB.setValue("/","lastmod_date",Value);
            }
            else if (openfluid::tools::toLowerCase(Key) == "incoutput")
            {
              SB.setValue("/","inc_outdir",Value=="true");
            }
          }
        }
        else if (Section.first == "builder.model.warescolors")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());

            SB.setValue("/context/builder/model/colors",Key,Value);
          }
        }
        else if (Section.first == "builder.model.graphicalview")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());

            SB.setValue("/context/builder/model/graphicalview/positions",Key,Value);
          }
        }
        else if (Section.first == "builder.model.render")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());

            if (openfluid::tools::toLowerCase(Key) == "color.mode")
            {
              SB.setValue("/context/builder/model/graphicalview","color_mode",Value);
            }
          }
        }
        else if (Section.first == "builder.spatial.unitsclasses")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());

            if (openfluid::tools::toLowerCase(Key) == "order")
            {
              std::vector<std::string> OrderArray;
              auto Classes = openfluid::tools::splitString(Value,",");

              for (const auto& C : Classes)
              {
                OrderArray.push_back(openfluid::tools::trim(C));
              }

              SB.setValue("/context/builder/spatial/unitsclasses","order",OrderArray);
            }
            else
            {
              auto SplittedKey = openfluid::tools::splitString(Key,".");
              if (SplittedKey.size() == 2)
              {
                auto ClassName = openfluid::tools::trim(SplittedKey[0]);
                auto StyleName = openfluid::tools::trim(SplittedKey[1]);

                if (StyleName == "linewidth")
                {
                  try
                  {
                    int Width = std::stoi(Value);
                    SB.setValue("/context/builder/spatial/unitsclasses/styles/"+ClassName,StyleName,Width);
                  }
                  catch (std::invalid_argument&)
                  { }
                }
                else
                {
                  SB.setValue("/context/builder/spatial/unitsclasses/styles/"+ClassName,StyleName,Value);
                }
              }
            }
          }
        }
        else if (Section.first == "builder.runconfig.options")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());

            if (openfluid::tools::toLowerCase(Key) == "clearoutdir")
            {
              SB.setValue("/context/builder/runconfig","clear_outdir",Value == "true");
            }
            else if (openfluid::tools::toLowerCase(Key) == "profiling")
            {
              SB.setValue("/context/builder/runconfig","profiling",Value == "true");
            }
            else if (openfluid::tools::toLowerCase(Key) == "maxthreads")
            {
              try
              {
                int Max = std::stoi(Value);
                SB.setValue("/context/builder/runconfig","maxthreads",Max);
              }
              catch (std::invalid_argument&)
              { }
            }
          }
        }
        else if (Section.first == "builder.simulation.options")
        {
          for(const auto& Entry : Section.second)
          {
            auto [Key,Value] = cleanKeyValue(Entry.first,Entry.second.get_value<std::string>());

            SB.setValue("/context/builder/simulation",Key,Value);
          }
        }
      }

      SB.save();
    }
  }
}


// =====================================================================
// =====================================================================


bool RunContextManager::checkProject(const std::string& ProjectPath)
{
  std::string AbsPath = openfluid::tools::Filesystem::absolutePath(ProjectPath);
  std::string PrjFilePath = getFilePathFromProjectPath(AbsPath);

  // try to convert a former openfluid-project.conf file if necessary
  updateProjectFile(PrjFilePath);

  if (!openfluid::tools::Filesystem::isFile(PrjFilePath))
  {
    return false;
  }

  if (!openfluid::tools::Filesystem::isDirectory(getInputDirFromProjectPath(AbsPath)))
  {
    return false;
  }

  openfluid::tools::SettingsBackend SB(PrjFilePath,m_ProjectRole);

  bool OK = SB.exists("/name") &&
            SB.exists("/description") &&
            SB.exists("/authors") &&
            SB.exists("/creation_date") &&
            SB.exists("/lastmod_date");

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

    std::string PrjFilePath = getFilePathFromProjectPath(m_ProjectPath);
    mp_ProjectFile = new openfluid::tools::SettingsBackend(PrjFilePath,m_ProjectRole);

    m_ProjectName = mp_ProjectFile->getValue("/name").get<std::string>("");
    m_ProjectDescription = mp_ProjectFile->getValue("/description").get<std::string>("");
    m_ProjectAuthors = mp_ProjectFile->getValue("/authors").get<std::string>("");
    m_ProjectCreationDate = mp_ProjectFile->getValue("/creation_date").get<std::string>("19700101T000000");
    m_ProjectLastModDate = mp_ProjectFile->getValue("/lastmod_date").get<std::string>("19700101T000000");
    m_ProjectIncOutputDir = mp_ProjectFile->getValue("/inc_outdir").get<bool>(false);

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

    std::string PrjFilePath = getFilePathFromProjectPath(AbsPath);
    mp_ProjectFile = new openfluid::tools::SettingsBackend(PrjFilePath,m_ProjectRole);

    m_ProjectName = Name;
    m_ProjectDescription = Description;
    m_ProjectAuthors = Authors;
    m_ProjectIncOutputDir = Inc;
    m_ProjectCreationDate = openfluid::tools::getNowAsString("%Y%m%dT%H%M%S");

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

  m_ProjectLastModDate = openfluid::tools::getNowAsString("%Y%m%dT%H%M%S");

  mp_ProjectFile->setValue("/","name",m_ProjectName);
  mp_ProjectFile->setValue("/","description",m_ProjectDescription);
  mp_ProjectFile->setValue("/","authors",m_ProjectAuthors);
  mp_ProjectFile->setValue("/","creation_date",m_ProjectCreationDate);
  mp_ProjectFile->setValue("/","lastmod_date",m_ProjectLastModDate);
  mp_ProjectFile->setValue("/","inc_outdir",m_ProjectIncOutputDir);

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

  std::string DateDir = "OPENFLUID."+openfluid::tools::getNowAsString("%Y%m%dT%H%M%S")+".OUT";
  m_OutputDir = openfluid::tools::Filesystem::joinPath({m_UserDataDir,DateDir});

  updateWaresEnvironment();
}


// =====================================================================
// =====================================================================


void RunContextManager::updateProjectOutputDir()
{
  if (m_ProjectIsOpen && m_ProjectIncOutputDir)
  {
    std::string Now = openfluid::tools::getNowAsString("%Y%m%dT%H%M%S");
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

    std::string PrjFilePath = getFilePathFromProjectPath(Path);
    openfluid::tools::SettingsBackend PrjFile(PrjFilePath,m_ProjectRole);

    Name = PrjFile.getValue("/name").get<std::string>("");
    Description = PrjFile.getValue("/description").get<std::string>("");
    Authors = PrjFile.getValue("/authors").get<std::string>("");
    CreationDate = PrjFile.getValue("/creation_date").get<std::string>("19700101T000000");
    LastModDate = PrjFile.getValue("/lastmod_date").get<std::string>("19700101T000000");

    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool RunContextManager::projectContainsDeprecatedFile(const std::string& Path)
{
  for (const auto& File : m_DeprecatedProjectFiles)
  {
    if (openfluid::tools::Filesystem::exists(openfluid::tools::Filesystem::joinPath({Path,File})))
    {
      return true;
    }
  }
  
  return false;
}


// =====================================================================
// =====================================================================


openfluid::tools::SettingValue RunContextManager::getProjectContextValue(const std::string& Pointer) const
{
  if (!m_ProjectIsOpen)
  {
    return openfluid::tools::SettingValue();
  }

  return mp_ProjectFile->getValue("/context"+Pointer);
}


// =====================================================================
// =====================================================================


openfluid::tools::SettingValue RunContextManager::getProjectContextValue(const std::string& Pointer,
                                                                         const std::string& Key) const
{
  return getProjectContextValue(Pointer+"/"+Key);
}


// =====================================================================
// =====================================================================


void RunContextManager::setProjectContextValue(const std::string& ParentPointer, const std::string& Key, 
                                               const openfluid::tools::SettingValue& Value)
{
  if (!m_ProjectIsOpen)
  {
    return;
  }

  mp_ProjectFile->setValue("/context"+ParentPointer,Key,Value);
}


// =====================================================================
// =====================================================================


void RunContextManager::removeProjectContextValue(const std::string& Pointer)
{
  if (!m_ProjectIsOpen)
  {
    return;
  }

  mp_ProjectFile->remove(Pointer);
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

