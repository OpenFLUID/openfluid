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
  @file process.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <iostream>

#include <openfluid/utils/Process.hpp>


void snippets_process()
{

{
//! [usage_run_progargs]
openfluid::utils::Process P("/path/to/program",{"--version"});
P.run();
const auto OutLines = P.stdOutLines(); // output stream lines
const auto ErrLines = P.stdErrLines(); // error stream lines
//! [usage_run_progargs]
}

{
//! [usage_run_cmd]
openfluid::utils::Process::Command Cmd;
Cmd.Program = "/path/to/program";  // program to execute
Cmd.Args = {"launch","--option=value"};  // program arguments
Cmd.setOutErrFiles("/path/to/files","program-run");  // enable redirection of output and error streams to files

openfluid::utils::Process::Environment Env;
Env.Vars["VAR1"] = "value1";  // add variable VAR1
Env.Vars["VAR2"] = "value2";  // add variable VAR2

openfluid::utils::Process P(Cmd,Env);
P.run();
std::cout << "Exit code: " << P.getExitCode() << std::endl;
// Note : output and error streams are redirected to files, output and error lines are empty
//! [usage_run_cmd]
}

{
//! [usage_execute]
openfluid::utils::Process::execute("/path/to/program");

openfluid::utils::Process::Command Cmd = {.Program = "/path/to/program", .Args = {"--version"}};
openfluid::utils::Process::execute(Cmd);
//! [usage_execute]
}

}
