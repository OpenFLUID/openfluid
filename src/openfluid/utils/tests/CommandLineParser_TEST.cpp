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
  @file CommandLineParser_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_commandlineparser
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/utils/CommandLineParser.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_option)
{
  openfluid::utils::CommandLineOption FB("foobar","f","a foobar option");

  BOOST_REQUIRE_EQUAL(FB.getLongName(),"foobar");
  BOOST_REQUIRE_EQUAL(FB.getShortName(),"f");
  BOOST_REQUIRE_EQUAL(FB.getHelpText(),"a foobar option");
  BOOST_REQUIRE(!FB.isActive());


  openfluid::utils::CommandLineOption BF("barfoo","","a barfoo option","true");

  BOOST_REQUIRE_EQUAL(BF.getLongName(),"barfoo");
  BOOST_REQUIRE(BF.getShortName().empty());
  BOOST_REQUIRE_EQUAL(BF.getHelpText(),"a barfoo option");
  BOOST_REQUIRE(!BF.isActive());
  BOOST_REQUIRE_EQUAL(BF.getValue(),"");

  BF.activate();
  BOOST_REQUIRE(!BF.isActive());

  BF.activate("baz");
  BOOST_REQUIRE(BF.isActive());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_command)
{

  openfluid::utils::CommandLineCommand RunCmd("run","run the process");

  RunCmd.addOption(openfluid::utils::CommandLineOption("extra-paths","p","extra paths for data",true));
  RunCmd.addOption(openfluid::utils::CommandLineOption("verbose","",""));
  RunCmd.addOption(openfluid::utils::CommandLineOption("quiet","q","quiet mode"));

  BOOST_REQUIRE_EQUAL(RunCmd.getName(),"run");
  BOOST_REQUIRE_EQUAL(RunCmd.getHelpText(),"run the process");
  BOOST_REQUIRE_EQUAL(RunCmd.options().size(),3);
  BOOST_REQUIRE_EQUAL(RunCmd.options().at("quiet").getLongName(),"quiet");
  BOOST_REQUIRE_EQUAL(RunCmd.options().at("quiet").getShortName(),"q");

  BOOST_REQUIRE(!RunCmd.isOptionActive("extra-paths"));
  BOOST_REQUIRE(!RunCmd.isOptionActive("quiet"));
  BOOST_REQUIRE(!RunCmd.isOptionActive("verbose"));


  RunCmd.activateOption("verbose");
  RunCmd.activateOption(RunCmd.getOptionNameFromShortName("p"),"/path/to/");

  BOOST_REQUIRE(RunCmd.isOptionActive("extra-paths"));
  BOOST_REQUIRE(!RunCmd.isOptionActive("quiet"));
  BOOST_REQUIRE(RunCmd.isOptionActive("verbose"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_parser)
{
  openfluid::utils::CommandLineParser Parser("myprogram","this is my program");

  std::list<std::string> ArgValues1 {"run","extra","-i","/i/path","--output-path=/o/path","end"};

  BOOST_REQUIRE(!Parser.parse(ArgValues1));


  openfluid::utils::CommandLineCommand RunCmd("run","run the process");
  RunCmd.addOption(openfluid::utils::CommandLineOption("input-path","i","input path",true));
  RunCmd.addOption(openfluid::utils::CommandLineOption("output-path","o","output path",true));
  RunCmd.addOption(openfluid::utils::CommandLineOption("verbose","","verbose mode"));
  Parser.addCommand(RunCmd);

  openfluid::utils::CommandLineCommand SearchCmd("search","search for data");
  SearchCmd.addOption(openfluid::utils::CommandLineOption("path","p","path to search",true));
  SearchCmd.addOption(openfluid::utils::CommandLineOption("extended","e","enable extended search"));
  Parser.addCommand(SearchCmd);

  BOOST_REQUIRE(Parser.parse(ArgValues1));
  Parser.printState(std::cout);

  std::cout << "=================================" << std::endl;

  std::list<std::string> ArgValues2 {"search","-p","/p/path","-e","*.*"};
  BOOST_REQUIRE(Parser.parse(ArgValues2));
  Parser.printState(std::cout);

  std::cout << "=================================" << std::endl;

  std::list<std::string> ArgValues3 {"run","-p","/p/path","-e","*.*"};
  BOOST_REQUIRE(!Parser.parse(ArgValues3));
  Parser.printState(std::cout);
  std::cout << "\n" << Parser.getParsingMessage() << std::endl;

  std::cout << "=================================" << std::endl;

  std::list<std::string> ArgValues4 {"-h"};
  BOOST_REQUIRE(Parser.parse(ArgValues4));
  Parser.printHelp(std::cout);
//  std::cout << "\n" << Parser.getParsingMessage() << std::endl;

  std::cout << "=================================" << std::endl;

  std::list<std::string> ArgValues5 {"search","--help"};
  BOOST_REQUIRE(Parser.parse(ArgValues5));
  Parser.printHelp(std::cout);
//  std::cout << "\n" << Parser.getParsingMessage() << std::endl;

  std::cout << "=================================" << std::endl;

  Parser.addOption(openfluid::utils::CommandLineOption("version","","display program version"));
  BOOST_REQUIRE(Parser.parse(ArgValues4));
  Parser.printHelp(std::cout);

  std::cout << std::endl;


}



