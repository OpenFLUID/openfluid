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
  @file main.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


// OpenFLUID:stylecheck:!incs


//! [cmdlineparser]
#include <openfluid/utils/CommandLineParser.hpp>

int main(int argc, char** argv)
{
  // ------- Definition of the parser for "myprogram"

  openfluid::utils::CommandLineParser Parser("myprogram","this is my program"); 

  // add of the "run" command with associated options
  openfluid::utils::CommandLineCommand RunCmd("run","run the process");
  RunCmd.addOption(openfluid::utils::CommandLineOption("input-path","i","input path",true));
  RunCmd.addOption(openfluid::utils::CommandLineOption("output-path","o","output path",true));
  RunCmd.addOption(openfluid::utils::CommandLineOption("verbose","","verbose mode"));
  Parser.addCommand(RunCmd);

  // add of the "search" command with associated options
  openfluid::utils::CommandLineCommand SearchCmd("search","search for data");
  SearchCmd.addOption(openfluid::utils::CommandLineOption("path","p","path to search",true));
  SearchCmd.addOption(openfluid::utils::CommandLineOption("extended","e","enable extended search"));
  Parser.addCommand(SearchCmd);


  // ------- Parsing of the given arguments

  Parser.parse(argc,argv);
  

  // ------- Processing of commands and options

  std::string ActiveCommand = Parser.getActiveCommand();
  
  if (ActiveCommand.empty())
  {
    // .. code here when no command
  }
  else if (ActiveCommand == "run")
  {
    // .. code here for "run" command

    if (Parser.command(ActiveCommand).isOptionActive("input-path"))
    {
      // .. code here for "input-path" option
    }
    if (Parser.command(ActiveCommand).isOptionActive("output-path"))
    {
      // .. code here for "output-path" option
    }
    
    // ...
  }
  else if (ActiveCommand == "search")
  {
    // .. code here for "search" command
  }
}
//! [cmdlineparser]