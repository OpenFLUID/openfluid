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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_UTILS_EXTERNALPROGRAM_HPP__
#define __OPENFLUID_UTILS_EXTERNALPROGRAM_HPP__

#include <openfluid/dllexport.hpp>

#include <QStringList>


namespace openfluid { namespace utils {

/**
  Management of external programs
*/
class OPENFLUID_API ExternalProgram
{
  private:

    QStringList m_ProgramNames;

    QString m_FullProgramPath;

    QStringList m_SearchPaths;

    bool m_UsePathEnv;


  public:

    /**
      List of pre-registered external programs
    */
    enum RegisteredPrograms { CMakeProgram, ZipProgram, SevenZipProgram,
                              GnuplotProgram, GoogleEarthProgram, GccProgram,
                              PdfLatexProgram, BibTexProgram, Latex2HTMLProgram,
                              GitProgram, GrassGisProgram };

    /**
      Constructor for a given external programs. Automatically searches for the program at construction
      @param[in] ProgramNames The possible names of the program to find
      @param[in] SearchPaths The list of paths to search. Default value is an empty list
      @param[in] UsePathEnv Enable the use of PATH environment variable to search for the program.
      Default value is true.
    */
    ExternalProgram(const QStringList& ProgramNames,
                    const QStringList& SearchPaths = QStringList(),
                    bool UsePathEnv = true);

    /**
      Constructor for a given external programs. Automatically searches for the program at construction
      @param[in] ProgramName The name of the program to find
      @param[in] SearchPaths The list of paths to search. Default value is an empty list
      @param[in] UsePathEnv Enable the use of PATH environment variable to search for the program.
      Default value is true.
    */
    ExternalProgram(const QString& ProgramName,
                    const QStringList& SearchPaths = QStringList(),
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
                                                const QStringList& SearchPaths = QStringList(),
                                                bool UsePathEnv = true);

    /**
      Finds a program using the PATH environment variable
      @param[in] ProgramNames The list of the potential program names of the program to find
      @return The full path of the first found program
    */
    static QString findUsingPATHEnvVar(const QStringList& ProgramNames);

    /**
      Finds a program using a given paths list
      @param[in] ProgramNames The list of the potential program names of the program to find
      @param[in] PathsList The list of paths to search
      @return The full path of the first found program
    */
    static QString findUsingPathsList(const QStringList& ProgramNames, const QStringList& PathsList);

    /**
      Returns true if the current program has been found
      @return true if the current program has been found
    */
    bool isFound() const
    { return !m_FullProgramPath.isEmpty(); };

    /**
      Searches for the program
    */
    void searchForProgram();

    /**
      Returns the full path of the program
      @return the full path of the program if it has been found, otherwise an empty string
    */
    QString getFullProgramPath() const
    { return m_FullProgramPath; };

};


} } // namespaces

#endif /* __OPENFLUID_UTILS_EXTERNALPROGRAM_HPP__ */
