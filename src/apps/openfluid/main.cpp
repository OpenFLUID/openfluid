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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <iostream>

#include <openfluid/global.hpp>
#include <openfluid/base/Init.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/utils/CommandLineParser.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/IDHelpers.hpp>

#include "InfoTasks.hpp"
#include "RunTasks.hpp"
#include "ReportTasks.hpp"
#include "DataTasks.hpp"
#include "WareTasks.hpp"
#include "WorkspaceTasks.hpp"


// =====================================================================
// =====================================================================


int main(int argc, char **argv)
{
  INIT_OPENFLUID_APPLICATION();

#if defined OPENFLUID_OS_WINDOWS
  std::string PathSepText = "semicolon";
#else
  std::string PathSepText = "colon";
#endif

  std::string DefaultMaxThreadsStr = 
    std::to_string(openfluid::base::RunContextManager::instance()->getWaresMaxNumThreads());

  std::vector<openfluid::utils::CommandLineOption> SearchOptions =
  {
    openfluid::utils::CommandLineOption("observers-paths","n",
                                        "add extra observers search paths ("+PathSepText+" separated)",true),
    openfluid::utils::CommandLineOption("simulators-paths","p",
                                        "add extra simulators search paths ("+PathSepText+" separated)",true),
  };


  openfluid::utils::CommandLineParser Parser("openfluid","OpenFLUID command-line tool");

  // ---

  auto InfoCmd = openfluid::utils::CommandLineCommand("info","display information about OpenFLUID");
  InfoCmd.addOptions({{"format","","output format, argument can be text (default) or json",true}});
  Parser.addCommand(InfoCmd);

  // ---

  auto VersionCmd = openfluid::utils::CommandLineCommand("version","display OpenFLUID version");
  VersionCmd.addOptions({{"no-status","s","do not display status part of the version number (if any)"},
                         {"numeric","n","display version number as numeric"}});
  Parser.addCommand(VersionCmd);
  
  // ---


  auto CreateDataCmd = openfluid::utils::CommandLineCommand("create-data","create project or dataset");
  CreateDataCmd.addOptions({{"type","t","type of the data to create (project or dataset)",true},
                            {"name","n","name of the project or dataset to create",true},
                            {"with-sample-data","s","generate sample data"},
                            {"parent-path","p","Parent path where to create the dataset or project",true}});
  Parser.addCommand(CreateDataCmd);  

  // ---

  auto InstallExamplesCmd = openfluid::utils::CommandLineCommand("install-examples",
                                                                 "install or reinstall examples in the user directory");
  InstallExamplesCmd.addOptions({{"projects-only","p",
                                  "install selected projects only (all projects if argument is * or empty)"},
                                 {"simulators-only","s",
                                  "install selected simulators only (all simulators if argument is * or empty)"},
                                 {"resources-path","r",
                                  "path where references examples are located (default is standard path)",true},
                                 {"install-path","i",
                                  "path where examples are installed for the user (default is user examples path)",
                                  true},
                                 {"force","f","force installation even if already installed"}});
  Parser.addCommand(InstallExamplesCmd);

  // ---

  auto RunCmd = openfluid::utils::CommandLineCommand("run","run a simulation");
  RunCmd.addOptions({{"quiet","q","quiet display during simulation"},
                     {"verbose","v","verbose display during simulation"},
                     {"profiling","k","enable simulation profiling"},
                     {"clean-output-dir","c","clean output directory before simulation"},
                     {"auto-output-dir","a","create automatic output directory"},
                     {"max-threads","t","set maximum number of threads for threaded spatial loops"
                                        " (default is "+DefaultMaxThreadsStr+")",true}});

  for (auto& Opt : SearchOptions)
  {
    RunCmd.addOption(Opt);
  } 

  Parser.addCommand(RunCmd);

  // ---

  auto ReportCmd = openfluid::utils::CommandLineCommand("report","display report about available wares");
  ReportCmd.addOptions({{"format","","output format, argument can be text (default) or json",true},
                        {"list","l","display as simple list of wares IDs"},
                        {"with-errors","e","report errors if any"}});

  for (auto& Opt : SearchOptions)
  {
    ReportCmd.addOption(Opt);
  } 

  Parser.addCommand(ReportCmd);


  // ---

  auto PrepWksCmd = openfluid::utils::CommandLineCommand("prepare-workspace","prepare an OpenFLUID workspace");
  PrepWksCmd.addOptions({{"path","p","Path to the worspace root",true}}); // TOIMPL
  Parser.addCommand(PrepWksCmd);

  // ---

  auto CreateWareCmd = openfluid::utils::CommandLineCommand("create-ware","create ware sources");
  CreateWareCmd.addOptions({{"type","t","type of the ware sources to create",true},
                            {"id","i","ID of the ware sources to create",true},
                            {"main-class","m","name to use for the main C++ class",true},
                            {"with-paramsui","w","generate the C++ class of the parameterization UI"
                                                 "(simularors and observers only)"},
                            {"paramsui-class","u","name to use for the C++ class of the parameterization UI"
                                                  "(simularors and observers only)",true},
                            {"bext-menutext","","menu text to launch the Builder-Extension",true},
                            {"bext-category","","category the Builder-Extension"
                                                 "(spatial|model|results|other, default is other)",true},
                            {"bext-mode","","mode of Builder-Extension (modal|modeless|workspace, default is modal)",
                             true},
                            {"parent-path","p","Parent path where to create the ware sources",true}});
  Parser.addCommand(CreateWareCmd);

  // ---

  auto CheckCmd = openfluid::utils::CommandLineCommand("check",""); // TOIMPL
  CheckCmd.addOptions({{"path","p","Path to the ware sources",true}});
  Parser.addCommand(CheckCmd);

  // ---

  auto MigrateWareCmd = openfluid::utils::CommandLineCommand("migrate-ware","migrate ware sources to current version "
                                                                            "("+openfluid::config::VERSION_FULL+")"); 
  MigrateWareCmd.addOptions({{"path","p","Path to the ware sources",true}}); // TOIMPL
  Parser.addCommand(MigrateWareCmd);

  // ---

  auto ConfigureCmd = openfluid::utils::CommandLineCommand("configure","run configure on ware sources"); // TOIMPL
  ConfigureCmd.addOptions({{"path","p","Path to the ware sources",true}});
  Parser.addCommand(ConfigureCmd);

  // ---

  auto DocCmd = openfluid::utils::CommandLineCommand("docalyze","build documentation of a ware"); // TOIMPL
  DocCmd.addOptions({{"path","p","Path to the ware sources",true}});
  Parser.addCommand(DocCmd);

  // ---

  auto BuildCmd = openfluid::utils::CommandLineCommand("build","run build on ware sources"); // TOIMPL
  BuildCmd.addOptions({{"path","p","Path to the ware sources",true}});
  Parser.addCommand(BuildCmd);

  // ---

  auto BuildInstallCmd = openfluid::utils::CommandLineCommand("buildinstall","run build and install on ware sources"); 
  BuildInstallCmd.addOptions({{"path","p","Path to the ware sources",true}}); // TOIMPL
  Parser.addCommand(BuildInstallCmd);

  // ---

  auto PurgeCmd = openfluid::utils::CommandLineCommand("purge","purge build outputs on ware sources"); // TOIMPL
  PurgeCmd.addOptions({{"path","p","Path to the ware sources",true}});
  Parser.addCommand(PurgeCmd);

  // ---

  auto Info2BuildCmd = openfluid::utils::CommandLineCommand("info2build","generate build files frow ware information");
  Info2BuildCmd.addOptions({{"path","p","Path to the ware sources",true}}); // TOIMPL and find another command name ? 
  Parser.addCommand(Info2BuildCmd);

  // ---

  auto MigrateGhostCmd = openfluid::utils::CommandLineCommand("migrate-ghostsim",
                                                              "migrate ghost simulator to current version "
                                                              "("+openfluid::config::VERSION_FULL+")"); 
  MigrateGhostCmd.addOptions({{"parent-path","p","Parent path to the ghost simulator to migrate",true},
                              {"id","i","ID of the ghost simulator to migrate",true}});
  Parser.addCommand(MigrateGhostCmd);

  // ---


  Parser.addOption(openfluid::utils::CommandLineOption("version","","display OpenFLUID version"));

  // ---


  if (!Parser.parse(argc,argv))
  {
    return TasksBase::error(Parser.getParsingMessage());
  }

  auto ActiveCmdStr = Parser.getActiveCommand();

  if (Parser.isHelpAsked())
  {
    Parser.printHelp(std::cout);
    return 0;
  }
  else if (ActiveCmdStr.empty())
  {
    if (Parser.command("").isOptionActive("version"))
    {
      std::cout << openfluid::config::VERSION_FULL << std::endl;
      return 0;
    }
    else
    {
      Parser.printHelp(std::cout);
      return TasksBase::ErrorCode;
    }
  }
  else if (ActiveCmdStr == "version" || ActiveCmdStr == "info")  
  {
    return InfoTasks(Parser).process();
  }
  else if (ActiveCmdStr == "prepare-workspace")
  {
    return WorkspaceTasks(Parser).process();
  }
  else if (ActiveCmdStr == "create-data" || ActiveCmdStr == "install-examples")  
  {
    return DataTasks(Parser).process();
  }
  else if (ActiveCmdStr == "run")  
  {
    return RunTasks(Parser).process();
  }
  else if (ActiveCmdStr == "report")  
  {
    return ReportTasks(Parser).process();
  }
  else if (ActiveCmdStr == "create-ware" ||
           ActiveCmdStr == "check" ||
           ActiveCmdStr == "migrate-ware" ||
           ActiveCmdStr == "docalyze" ||
           ActiveCmdStr == "configure" ||
           ActiveCmdStr == "build" ||
           ActiveCmdStr == "buildinstall" ||
           ActiveCmdStr == "purge"  ||
           ActiveCmdStr == "info2build" ||
           ActiveCmdStr == "migrate-ghostsim")  
  {
    return WareTasks(Parser).process();
  }

  return TasksBase::unknownCommand();
}

