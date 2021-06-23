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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QFileInfo>
#include <QDir>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/ExternalProgram.hpp>


namespace openfluid { namespace utils {


ExternalProgram::ExternalProgram(const QStringList& ProgramNames,
                                 const QStringList& SearchPaths,
                                 bool UsePathEnv) :
  m_ProgramNames(ProgramNames), m_SearchPaths(SearchPaths), m_UsePathEnv(UsePathEnv)
{
  searchForProgram();
}


// =====================================================================
// =====================================================================


ExternalProgram::ExternalProgram(const QString& ProgramName,
                                 const QStringList& SearchPaths,
                                 bool UsePathEnv) :
  m_SearchPaths(SearchPaths), m_UsePathEnv(UsePathEnv)
{
  m_ProgramNames << ProgramName;
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
  QStringList ProgNames;

  QStringList ModSearchPaths = SearchPaths;

  if (Prog == RegisteredPrograms::CMake)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "cmake";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "cmake.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::Zip)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "zip";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "zip.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::SevenZip)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "7z";
    ProgNames << "7za";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "7z.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::Gnuplot)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "gnuplot";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "gnuplot.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::GoogleEarth)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "google-earth";
#endif

#if defined(OPENFLUID_OS_MAC)
    ProgNames << "Google Earth";
    ModSearchPaths.append("/Applications/Google Earth.app/Contents/MacOS/");
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "google-earth.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::Gcc)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "gcc";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "gcc.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::PdfLatex)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "pdflatex";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "pdflatex.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::BibTex)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "bibtex";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "bibtex.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::Latex2HTML)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "latex2html";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "latex2html.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::Git)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "git";
#endif

#if defined(OPENFLUID_OS_WINDOWS)
    ProgNames << "git.exe";
#endif
  }
  else if (Prog == RegisteredPrograms::GrassGis)
  {
#if defined(OPENFLUID_OS_UNIX)
    ProgNames << "grass";
#endif
  }

  return ExternalProgram(ProgNames,ModSearchPaths,UsePathEnv);
}


// =====================================================================
// =====================================================================


QString ExternalProgram::findUsingPATHEnvVar(const QStringList& ProgramNames)
{
  QString PATHStr(qgetenv("PATH"));

  QStringList PathsList;

  if (!PATHStr.isEmpty())
  {
    PathsList = PATHStr.split(openfluid::tools::Filesystem::pathsListSeparator(),QString::SkipEmptyParts);
  }

#if defined(OPENFLUID_OS_UNIX)
  if (!PathsList.contains("/usr/local/bin"))
  {
   PathsList.append("/usr/local/bin");
  }
#endif


  return findUsingPathsList(ProgramNames,PathsList);
}


// =====================================================================
// =====================================================================


QString ExternalProgram::findUsingPathsList(const QStringList& ProgramNames, const QStringList& PathsList)
{
  for (auto& CurrentPath : PathsList)
  {
    for (auto& CurentName : ProgramNames)
    {
      QFileInfo FileToTest(QDir(CurrentPath),CurentName);
      if (FileToTest.isFile())
      {
        return FileToTest.absoluteFilePath();
      }
    }
  }

  return "";
}


// =====================================================================
// =====================================================================


void ExternalProgram::searchForProgram()
{
  m_FullProgramPath = "";

  if (!m_SearchPaths.isEmpty())
  {
    m_FullProgramPath = findUsingPathsList(m_ProgramNames,m_SearchPaths);
  }

  if (m_FullProgramPath.isEmpty() && m_UsePathEnv)
  {
    m_FullProgramPath = findUsingPATHEnvVar(m_ProgramNames);
  }
}

} } // namespaces
