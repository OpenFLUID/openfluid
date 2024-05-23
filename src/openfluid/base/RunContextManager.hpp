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
  @file RunContextManager.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BASE_RUNCONTEXTMANAGER_HPP__
#define __OPENFLUID_BASE_RUNCONTEXTMANAGER_HPP__


#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/utils/SingletonMacros.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/SettingsBackend.hpp>


namespace openfluid { namespace base {


class OPENFLUID_API RunContextManager : public Environment
{

  OPENFLUID_SINGLETON_DEFINITION(RunContextManager)


  private:

    std::string m_OutputDir;

    std::string m_InputDir;

    bool m_IsClearOutputDir;

    bool m_IsProfiling;

    unsigned int m_ValuesBufferSize;

    unsigned int m_WaresMaxNumThreads;

    openfluid::core::MapValue m_WaresSharedEnvironment;

    openfluid::core::MapValue m_ExtraProperties;

    openfluid::tools::SettingsBackend* mp_ProjectFile;

    std::string m_ProjectPath;

    std::string m_ProjectName;

    std::string m_ProjectDescription;

    std::string m_ProjectAuthors;

    std::string m_ProjectCreationDate;

    std::string m_ProjectLastModDate;

    bool m_ProjectIncOutputDir;

    bool m_ProjectIsOpen;

    static const std::vector<std::string> m_DeprecatedProjectFiles;

    RunContextManager();

    ~RunContextManager();

    void updateWaresEnvironment();

    static std::string getFilePathFromProjectPath(const std::string& ProjectPath);

    static std::string getInputDirFromProjectPath(const std::string& ProjectPath);

    static std::string getOuputDirFromProjectPath(const std::string& ProjectPath);

    static void updateProjectFile(const std::string& ProjectPath);

    static bool checkProject(const std::string& ProjectPath);


  public:

    /**
      Returns the full path of the simulation input directory
      @return the full path of the directory
    */
    std::string getInputDir() const
    {
      return m_InputDir;
    }

    /**
      Returns the full path of a file or directory relative to the simulation input directory
      @param[in] Filename the path relative to the simulation input directory
      @return the full path to the file or directory
    */
    std::string getInputFullPath(const std::string& Filename) const;

    /**
      Sets the full path of the simulation input directory
      @param[in] InputDir the full path of the directory
    */
    void setInputDir(const std::string& InputDir);

    /**
      Returns the full path of the simulation output directory
      @return the full path of the directory
    */
    std::string getOutputDir() const
    {
      return m_OutputDir;
    }

    /**
      Returns the full path of a file or directory relative to the simulation output directory
      @param[in] Filename the path relative to the simulation output directory
      @return the full path to the file or directory
    */
    std::string getOutputFullPath(const std::string& Filename) const;

    /**
      Sets the full path of the simulation output directory
      @param[in] OutputDir the full path of the directory
    */
    void setOutputDir(const std::string& OutputDir);

    void setDateTimeOutputDir();

    /**
      Sets the full path of the simulation input and output directory
      @param[in] InputDir the full path of the input directory
      @param[in] OutputDir the full path of the output directory
    */
    void setIODir(const std::string& InputDir, const std::string& OutputDir);

    /**
      Returns the status of automatic cleaning of output directory before simulation run
      @return true if enabled, false if disabled
    */
    bool isClearOutputDir() const
    {
      return m_IsClearOutputDir;
    }

    /**
      Sets the status of automatic cleaning of output directory before simulation run
      @param Enabled set to true to enable
    */
    void setClearOutputDir(bool Enabled)
    {
      m_IsClearOutputDir = Enabled;
    }

    /**
      Returns the status of simulation profiling
      @return true if enabled, false if disabled
    */
    bool isProfiling() const
    {
      return m_IsProfiling;
    }

    /**
      Sets the status of simulation profiling
      @param Enabled set to true to enable
    */
    void setProfiling(bool Enabled)
    {
      m_IsProfiling = Enabled;
    }

    /**
      Returns the size of the buffer set by the user for simulation variables values
      @return the size of the buffer
    */
    unsigned int getValuesBufferUserSize() const
    {
      return m_ValuesBufferSize;
    }

    /**
      Sets the size of the buffer set by the user for simulation variables values
      @param[in] Size the size of the buffer
    */
    void setValuesBufferUserSize(unsigned int Size)
    {
      m_ValuesBufferSize = Size;
    }

    /**
      Unsets the size of the buffer set by the user for simulation variables values
    */
    void unsetValuesBufferUserSize()
    { 
      m_ValuesBufferSize = 0; 
    }

    /**
      Returns true if the size of the buffer for simulation variables values has been set by the user
      @return true if set by user (size > 0), false otherwise
    */
    bool isValuesBufferUserSize() const
    {
      return (m_ValuesBufferSize > 0);
    }

    /**
      Returns the value for maximum threads count to be used in OpenFLUID wares (simulators, observers, ...)
      @return the maximum threads count
    */
    unsigned int getWaresMaxNumThreads() const
    {
      return m_WaresMaxNumThreads;
    }

    /**
      Sets the value for maximum threads count to be used in OpenFLUID wares (simulators, observers, ...)
      @param[in] Num the maximum threads count
    */
    void setWaresMaxNumThreads(unsigned int Num)
    {
      m_WaresMaxNumThreads = Num;
    }

    /**
      Resets the value for maximum threads count to be used in OpenFLUID wares (simulators, observers, ...).
      The value is reset to the ideal thread count given by the OpenFLUID environment
    */
    void resetWaresMaxNumThreads();

    openfluid::core::MapValue& extraProperties()
    {
      return m_ExtraProperties;
    }

    const openfluid::core::MapValue& getWaresEnvironment() const
    {
      return m_WaresSharedEnvironment;
    }

    void processWareParams(openfluid::ware::WareParams_t& Params) const;

    std::string getProjectPath() const
    {
      return m_ProjectPath;
    }

    std::string getProjectName() const
    {
      return m_ProjectName;
    }

    void setProjectName(const std::string& Name)
    {
      m_ProjectName = Name;
    }

    std::string getProjectDescription() const
    {
      return m_ProjectDescription;
    }

    void setProjectDescription(const std::string& Description)
    {
      m_ProjectDescription = Description;
    }

    std::string getProjectAuthors() const
    {
      return m_ProjectAuthors;
    }

    void setProjectAuthors(const std::string& Authors)
    {
      m_ProjectAuthors = Authors;
    }

    std::string getProjectCreationDate() const
    {
      return m_ProjectCreationDate;
    }

    void setProjectCreationDate(const std::string& CreationDate)
    {
      m_ProjectCreationDate = CreationDate;
    }

    void setProjectCreationDateAsNow()
    {
      m_ProjectCreationDate = openfluid::tools::getNowAsString("%Y%m%dT%H%M%S");
    }

    std::string getProjectLastModDate() const
    {
      return m_ProjectLastModDate;
    }

    void setProjectLastModDate(const std::string& LastModDate)
    {
      m_ProjectLastModDate = LastModDate;
    }

    bool isProjectIncrementalOutputDir() const
    {
      return m_ProjectIncOutputDir;
    }

    void setProjectIncrementalOutputDir(const bool Inc)
    {
      m_ProjectIncOutputDir = Inc;
    }

    bool openProject(const std::string& Path);

    bool createProject(const std::string& Path, const std::string& Name,
                       const std::string& Description, const std::string& Authors,
                       const bool Inc);

    bool isProjectOpen() const
    {
      return m_ProjectIsOpen;
    }

    bool saveProject();

    void closeProject();

    static bool isProject(const std::string& Path);

    static bool getProjectInfos(const std::string& Path,
                                std::string& Name, std::string& Description, std::string& Authors,
                                std::string& CreationDate, std::string& LastModDate);

    static bool projectContainsDeprecatedFile(const std::string& Path);

    void updateProjectOutputDir();

    openfluid::tools::SettingValue getProjectContextValue(const std::string& Pointer) const;

    openfluid::tools::SettingValue getProjectContextValue(const std::string& ParentPointer,
                                                          const std::string& Key) const;

    void removeProjectContextValue(const std::string& Pointer);

    void setProjectContextValue(const std::string& ParentPointer, const std::string& Key, 
                                const openfluid::tools::SettingValue& Value);
    
};


} } //namespaces


#endif /* __OPENFLUID_BASE_RUNCONTEXTMANAGER_HPP__ */
