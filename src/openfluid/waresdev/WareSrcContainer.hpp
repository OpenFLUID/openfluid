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
  @file WareSrcContainer.hpp
 
  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEV_WARESRCCONTAINER_HPP__
#define __OPENFLUID_WARESDEV_WARESRCCONTAINER_HPP__


#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/waresdev/WareSrcMsgStream.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


class OPENFLUID_API WareSrcContainer
{

  public:

    enum class ConfigMode
    {
      CONFIG_DEBUG, CONFIG_RELEASE
    };

    enum class BuildMode
    {
      BUILD_WITHINSTALL, BUILD_NOINSTALL
    };


  protected:

    std::string m_AbsolutePath;

    openfluid::ware::WareType m_Type;

    openfluid::ware::WareID_t m_ID;

    /**
      Absolute path of the CMake config file if it exists, otherwise an empty string
    */
    std::string m_AbsoluteCMakeConfigPath;

    /**
      Absolute path of the main .cpp as set in the CMake config file, if this .cpp file exists,
      otherwise an empty string
    */
    std::string m_AbsoluteMainCppPath;

    /**
      Absolute path of the ui-parameterization .cpp as set in the CMake config file, if this .cpp file exists,
      otherwise an empty string
    */
    std::string m_AbsoluteUiParamCppPath;

    /**
      Absolute path of the CMake config file if it exists, otherwise an empty string
    */
    std::string m_AbsoluteCMakeListsPath;

    /**
      Absolute path of the wareshub.json file if it exists, otherwise an empty string
    */
    std::string m_AbsoluteJsonPath;

    openfluid::waresdev::WareSrcMsgStream* mp_Stream;

    ConfigMode m_ConfigMode;

    BuildMode m_BuildMode;

    unsigned int m_BuildJobs;

    std::string m_BuildDirPath;

    std::string m_OFVersion;

    /**
      @throw openfluid::base::FrameworkException
    */
    void findCMake();


  public:

    WareSrcContainer(const std::string& AbsolutePath, openfluid::ware::WareType Type, const std::string& WareID);

    virtual ~WareSrcContainer();

    void update();

    std::string getAbsolutePath() const
    {
      return m_AbsolutePath;
    }

    openfluid::ware::WareType getType() const
    {
      return m_Type;
    }

    openfluid::ware::WareID_t getID() const
    {
      return m_ID;
    }

    /**
      Return the absolute paths of:
      - the CMake config file if it exists on disk,
      - the main .cpp file as set in the CMakeLists.txt, if this .cpp exists on disk.
      If no .cpp file was found (because it doesn't exists, it wasn't found in CMake file,
      or CMake file doesn't exist):
      - the first .cpp file found in this ware directory (not recursive).
      @return a list of existing absolute paths into this ware directory. It can be empty.
    */
    std::vector<std::string> getDefaultFilesPaths();

    std::string getBuildDirPath() const;

    /**
      Returns the absolute path of the main .cpp as set in the CMake config file, if this .cpp file exists,
      otherwise an empty string
    */
    std::string getMainCppPath() const;

    /**
      Returns the absolute path of the ui-parameterization .cpp as set in the CMake config file,
      if this .cpp file exists, otherwise an empty string
    */
    std::string getUiParamCppPath() const;

    /**
      Returns the absolute path of the CMake config file, if this file exists,
      otherwise an empty string
    */
    std::string getCMakeConfigPath() const;

    /**
      Returns the absolute path of the CMakeLists.txt file, if this file exists,
      otherwise an empty string
    */
    std::string getCMakeListsPath() const;

    /**
      Returns the absolute path of the wareshub.json file, if this file exists,
      otherwise an empty string
    */
    std::string getJsonPath() const;

    std::map<std::string,std::string> getConfigureVariables() const;

    std::string getConfigureGenerator() const;

    std::string getConfigureExtraOptions() const;

    std::string getBuildTarget() const;

    std::string getGenerateDocTarget() const;

    unsigned int getBuildJobs() const;

    void prepareBuildDirectory() const;

    void setMsgStream(openfluid::waresdev::WareSrcMsgStream& Stream);

    void setConfigMode(ConfigMode Mode);

    void setBuildMode(BuildMode Mode);

    void setBuildJobs(unsigned int Jobs);

};


} } // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCCONTAINER_HPP__ */
