/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file ExternalProgram.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/tools/ExternalProgram.hpp>

#include <QFileInfo>
#include <QDir>


namespace openfluid { namespace tools {

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
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    ProgName = "cmake";
#endif

#if defined(Q_OS_WIN32)
    ProgName = "cmake.exe";
#endif
  }
  else if (Prog == ZipProgram)
  {
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    ProgName = "zip";
#endif

#if defined(Q_OS_WIN32)
    ProgName = "zip.exe";
#endif
  }
  else if (Prog == SevenZipProgram)
  {
#if defined(Q_OS_UNIX)
    ProgName = "7z";
#endif

#if defined(Q_OS_MAC)
    ProgName = "7z";
#endif

#if defined(Q_OS_WIN32)
    ProgName = "7z.exe";
#endif
  }
  else if (Prog == GnuplotProgram)
  {
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    ProgName = "gnuplot";
#endif

#if defined(Q_OS_WIN32)
    ProgName = "gnuplot.exe";
#endif
  }
  else if (Prog == GoogleEarthProgram)
  {
#if defined(Q_OS_UNIX)
    ProgName = "google-earth";
#endif

#if defined(Q_OS_MAC)
    ProgName = "Google Earth";
    ModSearchPaths.append("/Applications/Google Earth.app/Contents/MacOS/");
#endif

#if defined(Q_OS_WIN32)
    ProgName = "google-earth.exe";
#endif
  }
  else if (Prog == GccProgram)
  {
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    ProgName = "gcc";
#endif

#if defined(Q_OS_WIN32)
    ProgName = "gcc.exe";
#endif
  }
  else if (Prog == PdfLatexProgram)
  {
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    ProgName = "pdflatex";
#endif

#if defined(Q_OS_WIN32)
    ProgName = "pdflatex.exe";
#endif
  }
  else if (Prog == BibTexProgram)
  {
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    ProgName = "bibtex";
#endif

#if defined(Q_OS_WIN32)
    ProgName = "bibtex.exe";
#endif
  }
  else if (Prog == Latex2HTMLProgram)
  {
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    ProgName = "latex2html";
#endif

#if defined(Q_OS_WIN32)
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
#if defined __unix__ || defined __APPLE__
    PathsList = PATHStr.split(":",QString::SkipEmptyParts);
#endif

#ifdef WIN32
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
