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
  @file CMakeProxy.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <map>
#include <vector>

#include <openfluid/utils/ProgramProxy.hpp>
#include <openfluid/dllexport.hpp>


#ifndef __OPENFLUID_UTILS_CMAKEPROXY_HPP__
#define __OPENFLUID_UTILS_CMAKEPROXY_HPP__


namespace openfluid { namespace utils {


class OPENFLUID_API CMakeProxy : public ProgramProxy<CMakeProxy>
{
  private:

    static void findCMakeProgram();


  public:

    inline static const std::string DefaultBuildType = "Release";

    CMakeProxy();

    virtual ~CMakeProxy();

    static bool isAvailable();

    static std::string getBuildDir(const std::string& BuildType = DefaultBuildType);

    static Process::Command getConfigureCommand(const std::string& BuildDir, const std::string& SrcDir,
                                                const std::map<std::string,std::string>& Variables = {},
                                                const std::string& Generator = "", 
                                                const std::vector<std::string>& Options = {});

    /**
      Returns a build command prepared using the given arguments
      @param[in] BuildDir the directory where the build will be performed
      @param[in] Target the target to build (empty by default)
      @param[in] Jobs the number of parallel jobs to use, 0 means that parallel build is not active (0 by default)
      @param[in] CMakeOptions a string of options passed to CMake (empty by default)
      @param[in] OtherOptions a string of options passed to the build tool (empty by default)
    */
    static Process::Command getBuildCommand(const std::string& BuildDir,
                                            const std::string& Target = "",
                                            const unsigned int Jobs = 0,
                                            const std::vector<std::string>& CMakeOptions = {}, 
                                            const std::vector<std::string>& OtherOptions = {});

    static Process::Command getTarCompressCommand(const std::string& WorkDir,
                                                  const std::string& TarFilePath,
                                                  const std::vector<std::string>& RelativePathsToCompress,
                                                  const std::string& Options = "");

    static Process::Command getTarUncompressCommand(const std::string& WorkDir, const std::string& TarFilePath,
                                                    const std::string& Options = "");

};


} }  // namespaces


#endif /* __OPENFLUID_UTILS_CMAKEPROXY_HPP__ */
