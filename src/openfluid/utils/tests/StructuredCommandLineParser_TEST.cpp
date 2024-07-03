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
  @file StructuredCommandLineParser_TEST.cpp

  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_structuredcommandlineparser


#include <iostream>
#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>

#include <openfluid/utils/StructuredCommandLineParser.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_commands_with_no_section)
{
  openfluid::utils::CommandLineSection Section1("Section 1");
  openfluid::utils::CommandLineSection Section2("Section 2");

  openfluid::utils::StructuredCommandLineParser Parser("myprogram","this is my program", {&Section1, &Section2});

  openfluid::utils::CommandLineCommand RunCmd("run","run the process");
  openfluid::utils::CommandLineCommand BuildCmd("build","build project");
  openfluid::utils::CommandLineCommand VersionCmd("version","display version");

  Parser.addCommand(RunCmd, &Section1);
  Parser.addCommand(BuildCmd, &Section2);
  Parser.addCommand(VersionCmd);

  std::ostringstream oss;
  Parser.printHelp(oss);
  std::string CommandsDisplay = oss.str();
  std::string NoSectionText = "Others";
  std::string CommandNoSectionText = " version ";
  std::string AvailableOptionsText = "Available options:";                                 

  BOOST_CHECK(CommandsDisplay.find(NoSectionText) != std::string::npos);
  BOOST_CHECK(CommandsDisplay.find(CommandNoSectionText) != std::string::npos);
  BOOST_CHECK(CommandsDisplay.find(CommandNoSectionText) > CommandsDisplay.find(NoSectionText));
  BOOST_CHECK(CommandsDisplay.find(CommandNoSectionText) < CommandsDisplay.find(AvailableOptionsText));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_commands_order)
{
  openfluid::utils::CommandLineSection Section1("Section 1");

  openfluid::utils::StructuredCommandLineParser Parser("myprogram","this is my program", {&Section1});

  openfluid::utils::CommandLineCommand VersionCmd("version","display version");
  openfluid::utils::CommandLineCommand RunCmd("run","run the process");
  openfluid::utils::CommandLineCommand BuildCmd("build","build project");

  Parser.addCommand(VersionCmd, &Section1);
  Parser.addCommand(RunCmd, &Section1);
  Parser.addCommand(BuildCmd, &Section1);

  std::ostringstream oss;
  Parser.printHelp(oss);
  std::string CommandsDisplay = oss.str();                               

  BOOST_CHECK(CommandsDisplay.find(VersionCmd.getName()) != std::string::npos);
  BOOST_CHECK(CommandsDisplay.find(RunCmd.getName()) != std::string::npos);
  BOOST_CHECK(CommandsDisplay.find(BuildCmd.getName()) != std::string::npos);
  BOOST_CHECK(CommandsDisplay.find(RunCmd.getName()) > CommandsDisplay.find(VersionCmd.getName()));
  BOOST_CHECK(CommandsDisplay.find(BuildCmd.getName()) > CommandsDisplay.find(RunCmd.getName()));
}