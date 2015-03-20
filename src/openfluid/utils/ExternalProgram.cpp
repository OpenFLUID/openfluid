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
  @file ExternalProgram.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/utils/ExternalProgram.hpp>
#include <QFileInfo>
#include <QDir>


namespace openfluid { namespace utils {

ExternalProgram::ExternalProgram(const QString& Program,
                                 const QStringList& SearchPaths,
                                 bool UsePathEnv) :
  m_Program(Program), m_SearchPaths(SearchPaths), m_UsePathEnv(UsePathEnv)
{
  searchForProgram();
}


// =====================================================================
// =====================================================================


ExternalProgram::~ExternalProgram()
{

}


// =====================================================================
// =====================================================================


ExternalProgram ExternalProgram::getRegisteredProgram(RegisteredPrograms Prog,
                                                      const QStringList& SearchPaths,
                                                      bool UsePathEnv)
{
  QString ProgName;

  QStringList ModSearchPaths = SearchPaths;

  if (Prog == CMakeProgram)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgName = "cmake";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgName = "cmake.exe";
#endif
  }
  else if (Prog == ZipProgram)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgName = "zip";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgName = "zip.exe";
#endif
  }
  else if (Prog == SevenZipProgram)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgName = "7z";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgName = "7z.exe";
#endif
  }
  else if (Prog == GnuplotProgram)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgName = "gnuplot";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgName = "gnuplot.exe";
#endif
  }
  else if (Prog == GoogleEarthProgram)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgName = "google-earth";
#endif

#if defined(OPENFLUID_OS_MAC)
    ProgName = "Google Earth";
    ModSearchPaths.append("/Applications/Google Earth.app/Contents/MacOS/");
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgName = "google-earth.exe";
#endif
  }
  else if (Prog == GccProgram)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgName = "gcc";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgName = "gcc.exe";
#endif
  }
  else if (Prog == PdfLatexProgram)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgName = "pdflatex";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgName = "pdflatex.exe";
#endif
  }
  else if (Prog == BibTexProgram)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgName = "bibtex";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgName = "bibtex.exe";
#endif
  }
  else if (Prog == Latex2HTMLProgram)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgName = "latex2html";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgName = "latex2html.exe";
#endif
  }

  return ExternalProgram(ProgName,ModSearchPaths,UsePathEnv);
}


// =====================================================================
// =====================================================================


QString ExternalProgram::findUsingPATHEnvVar(const QString& Program)
{
  QString PATHStr(qgetenv("PATH"));

  QStringList PathsList;

  if (!PATHStr.isEmpty())
  {
#if defined(OPENFLUID_OS_UNIX)
    PathsList = PATHStr.split(":",QString::SkipEmptyParts);
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    PathsList = PATHStr.split(";",QString::SkipEmptyParts);
#endif
  }

  return findUsingPathsList(Program,PathsList);
}


// =====================================================================
// =====================================================================


QString ExternalProgram::findUsingPathsList(const QString& Program, const QStringList& PathsList)
{
  for (int i=0; i< PathsList.size();i++)
  {
    QFileInfo FileToTest(QDir(PathsList[i]),Program);
    if (FileToTest.isFile()) return FileToTest.absoluteFilePath();
  }

  return "";
}


// =====================================================================
// =====================================================================


void ExternalProgram::searchForProgram()
{
  m_FullProgramPath = "";

  if (!m_SearchPaths.isEmpty())
    m_FullProgramPath = findUsingPathsList(m_Program,m_SearchPaths);

  if (m_FullProgramPath.isEmpty() && m_UsePathEnv)
    m_FullProgramPath = findUsingPATHEnvVar(m_Program);
}

} } // namespaces
