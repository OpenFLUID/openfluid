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
  @file ExternalProgram_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_externalprogram


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/utilsq/ExternalProgram.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_registered)
{
  openfluid::utils::ExternalProgram ExtProg =
      openfluid::utils::ExternalProgram::getRegisteredProgram(
        openfluid::utils::ExternalProgram::RegisteredPrograms::CMake);
  std::cout << "Looking for CMake: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::utils::ExternalProgram::getRegisteredProgram(
    openfluid::utils::ExternalProgram::RegisteredPrograms::Gcc);
  std::cout << "Looking for gcc: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::utils::ExternalProgram::getRegisteredProgram(
    openfluid::utils::ExternalProgram::RegisteredPrograms::Zip);
  std::cout << "Looking for zip: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::utils::ExternalProgram::getRegisteredProgram(
    openfluid::utils::ExternalProgram::RegisteredPrograms::SevenZip);
  std::cout << "Looking for 7zip: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::utils::ExternalProgram::getRegisteredProgram(
    openfluid::utils::ExternalProgram::RegisteredPrograms::Gnuplot);
  std::cout << "Looking for gnuplot: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg =
      openfluid::utils::ExternalProgram::getRegisteredProgram(
        openfluid::utils::ExternalProgram::RegisteredPrograms::GoogleEarth);
  std::cout << "Looking for GoogleEarth: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::utils::ExternalProgram::getRegisteredProgram(
    openfluid::utils::ExternalProgram::RegisteredPrograms::PdfLatex);
  std::cout << "Looking for pdflatex: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg = openfluid::utils::ExternalProgram::getRegisteredProgram(
    openfluid::utils::ExternalProgram::RegisteredPrograms::BibTex);
  std::cout << "Looking for bibtex: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg =
      openfluid::utils::ExternalProgram::getRegisteredProgram(
        openfluid::utils::ExternalProgram::RegisteredPrograms::Latex2HTML);
  std::cout << "Looking for latex2html: " << ExtProg.getFullProgramPath().toStdString() << std::endl;

  ExtProg =
      openfluid::utils::ExternalProgram::getRegisteredProgram(
        openfluid::utils::ExternalProgram::RegisteredPrograms::GrassGis);
  std::cout << "Looking for GRASS GIS: " << ExtProg.getFullProgramPath().toStdString() << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_others)
{
#if defined(Q_OS_UNIX)
  openfluid::utils::ExternalProgram ExtProg("ls");
    std::cout << "Looking for ls: " << ExtProg.getFullProgramPath().toStdString() << std::endl;
#endif
}

