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
 * Archiver_TEST.cpp
 *
 *  Created on: 16 juil. 2013
 *      Author: Manuel CHATAIGNER
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_archiver
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/tools/ExternalProgram.hpp>


// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_registered)
{
  openfluid::tools::ExternalProgram ExtProg =
      openfluid::tools::ExternalProgram::getRegisteredProgram(openfluid::tools::ExternalProgram::CMakeProgram);
  std::cout << "Looking for CMake: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::tools::ExternalProgram::getRegisteredProgram(openfluid::tools::ExternalProgram::GccProgram);
  std::cout << "Looking for gcc: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::tools::ExternalProgram::getRegisteredProgram(openfluid::tools::ExternalProgram::ZipProgram);
  std::cout << "Looking for zip: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::tools::ExternalProgram::getRegisteredProgram(openfluid::tools::ExternalProgram::SevenZipProgram);
  std::cout << "Looking for 7zip: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::tools::ExternalProgram::getRegisteredProgram(openfluid::tools::ExternalProgram::GnuplotProgram);
  std::cout << "Looking for gnuplot: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::tools::ExternalProgram::getRegisteredProgram(openfluid::tools::ExternalProgram::GoogleEarthProgram);
  std::cout << "Looking for GoogleEarth: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::tools::ExternalProgram::getRegisteredProgram(openfluid::tools::ExternalProgram::PdfLatexProgram);
  std::cout << "Looking for pdflatex: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::tools::ExternalProgram::getRegisteredProgram(openfluid::tools::ExternalProgram::BibTexProgram);
  std::cout << "Looking for bibtex: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::tools::ExternalProgram::getRegisteredProgram(openfluid::tools::ExternalProgram::Latex2HTMLProgram);
  std::cout << "Looking for latex2html: " << ExtProg.getFullProgramPath().toStdString() << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_others)
{
#if defined(Q_OS_UNIX)
  openfluid::tools::ExternalProgram ExtProg("ls");
    std::cout << "Looking for ls: " << ExtProg.getFullProgramPath().toStdString() << std::endl;
#endif
}



