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
  \file ExternalProgram.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __EXTERNALPROGRAM_HPP__
#define __EXTERNALPROGRAM_HPP__

#include <openfluid/dllexport.hpp>

#include <QStringList>


namespace openfluid { namespace tools {

class DLLEXPORT ExternalProgram
{
  private:

    QString m_Program;

    QString m_FullProgramPath;

    QStringList m_SearchPaths;

    bool m_UsePathEnv;


  public:

    enum RegisteredPrograms { CMakeProgram, ZipProgram, SevenZipProgram,
                              GnuplotProgram, GoogleEarthProgram, GccProgram,
                              PdfLatexProgram, BibTexProgram, Latex2HTMLProgram };

    ExternalProgram(const QString& Program,
                    const QStringList& SearchPaths = QStringList(),
                    bool UsePathEnv = true);

    ~ExternalProgram();

    static ExternalProgram getRegisteredProgram(RegisteredPrograms Prog,
                                                const QStringList& SearchPaths = QStringList(),
                                                bool UsePathEnv = true);

    static QString findUsingPATHEnvVar(const QString& Program);

    static QString findUsingPathsList(const QString& Program, const QStringList& PathsList);

    bool isFound() const { return !m_FullProgramPath.isEmpty(); };

    void searchForProgram();

    QString getFullProgramPath() const { return m_FullProgramPath; };

};


} } // namespaces

#endif /* __EXTERNALPROGRAM_HPP__ */
