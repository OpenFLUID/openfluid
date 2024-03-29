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
  @file ExternalProgram.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UTILS_EXTERNALPROGRAM_HPP__
#define __OPENFLUID_UTILS_EXTERNALPROGRAM_HPP__


#include <string>
#include <vector>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace utils {

/**
  Management of external programs
*/
class OPENFLUID_API ExternalProgram
{
  private:

    std::vector<std::string> m_ProgramNames;

    std::string m_FullProgramPath;

    std::vector<std::string> m_SearchPaths;

    bool m_UsePathEnv;


  public:

    /**
      List of pre-registered external programs
    */
    enum class RegisteredPrograms { CMake, Zip, SevenZip,
                                    Gnuplot, GoogleEarth, Gcc,
                                    Pandoc, PdfLatex, BibTex, Latex2HTML,
                                    Git, GrassGis, R, Rscript };

    ExternalProgram() = delete;

    /**
      Constructor for a given external programs. Automatically searches for the program at construction
      @param[in] ProgramNames The possible names of the program to find
      @param[in] SearchPaths The list of paths to search. Default value is an empty list
      @param[in] UsePathEnv Enable the use of PATH environment variable to search for the program.
      Default value is true.
    */
    ExternalProgram(const std::vector<std::string>& ProgramNames,
                    const std::vector<std::string>& SearchPaths = std::vector<std::string>(),
                    bool UsePathEnv = true);

    /**
      Constructor for a given external programs. Automatically searches for the program at construction
      @param[in] ProgramName The name of the program to find
      @param[in] SearchPaths The list of paths to search. Default value is an empty list
      @param[in] UsePathEnv Enable the use of PATH environment variable to search for the program.
      Default value is true.
    */
    ExternalProgram(const std::string& ProgramName,
                    const std::vector<std::string>& SearchPaths = std::vector<std::string>(),
                    bool UsePathEnv = true);

    ~ExternalProgram();

    /**
      Returns an ExternalProgram object initialized with the registered program to find
      @param[in] Prog The registered program to find
      @param[in] SearchPaths The list of paths to search. Default value is an empty list
      @param[in] UsePathEnv Enable the use of PATH environment variable to search for the registered program.
      Default value is true.
      @return An ExternalProgram object initialized with the registered program to find
    */
    static ExternalProgram getRegisteredProgram(RegisteredPrograms Prog,
                                               const std::vector<std::string>& SearchPaths = std::vector<std::string>(),
                                               bool UsePathEnv = true);

    /**
      Finds a program using the PATH environment variable
      @param[in] ProgramNames The list of the potential program names of the program to find
      @return The full path of the first found program
    */
    static std::string findUsingPATHEnvVar(const std::vector<std::string>& ProgramNames);

    /**
      Finds a program using a given paths list
      @param[in] ProgramNames The list of the potential program names of the program to find
      @param[in] PathsList The list of paths to search
      @return The full path of the first found program
    */
    static std::string findUsingPathsList(const std::vector<std::string>& ProgramNames, 
                                          const std::vector<std::string>& PathsList);

    /**
      Returns true if the current program has been found
      @return true if the current program has been found
    */
    bool isFound() const
    { 
      return !m_FullProgramPath.empty(); 
    };

    /**
      Searches for the program
    */
    void searchForProgram();

    /**
      Returns the full path of the program
      @return the full path of the program if it has been found, otherwise an empty string
    */
    std::string getFullProgramPath() const
    { 
      return m_FullProgramPath; 
    };

};


} } // namespaces

#endif /* __OPENFLUID_UTILS_EXTERNALPROGRAM_HPP__ */
