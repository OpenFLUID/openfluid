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
  @file Process_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_process


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/utils/Process.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_cmd)
{
  {
    openfluid::utils::Process::Command Cmd{
      .Program = CONFIGTESTS_CMAKE_PROGRAM,
      .Args = {"--option=value","-x","/path/to/work"},
    };

    BOOST_REQUIRE_EQUAL(Cmd.joined(),CONFIGTESTS_CMAKE_PROGRAM+" --option=value -x /path/to/work");
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_object)
{
  {
    openfluid::utils::Process P(CONFIGTESTS_CMAKE_PROGRAM);
    BOOST_CHECK(P.run());
    BOOST_CHECK_EQUAL(P.getExitCode(),0);
    BOOST_CHECK(!P.stdOutLines().empty());
  }

  {
    openfluid::utils::Process P(CONFIGTESTS_CMAKE_PROGRAM,{"--version"});
    BOOST_CHECK(P.run());
    BOOST_CHECK_EQUAL(P.getExitCode(),0);
    BOOST_CHECK(!P.stdOutLines().empty());
  }

  {
    openfluid::utils::Process P(CONFIGTESTS_CMAKE_PROGRAM,{"--option-that-does-not-exist"});
    BOOST_CHECK(P.run());
    BOOST_CHECK_NE(P.getExitCode(),0);
    BOOST_CHECK(!P.stdErrLines().empty());
  }

  {
    openfluid::utils::Process::Command Cmd{
      .Program = CONFIGTESTS_CMAKE_PROGRAM,
      .Args = {"--help"},
    };
    Cmd.setOutFile(CONFIGTESTS_OUTPUT_DATA_DIR+"/Process","run-cmakehelp");
    openfluid::utils::Process P(Cmd);
    BOOST_CHECK(P.run());
    BOOST_CHECK_EQUAL(P.getExitCode(),0);
    BOOST_CHECK(P.stdOutLines().empty());
  }

  {
    openfluid::utils::Process::Command Cmd{
      .Program = CONFIGTESTS_CMAKE_PROGRAM,
      .Args = {"--option-that-does-not-exist"}
    };
    Cmd.setOutErrFiles(CONFIGTESTS_OUTPUT_DATA_DIR+"/Process","run-cmakewrong");
    openfluid::utils::Process P(Cmd);
    BOOST_CHECK(P.run());
    BOOST_CHECK_NE(P.getExitCode(),0);
    BOOST_CHECK(P.stdOutLines().empty());
    BOOST_CHECK(P.stdErrLines().empty());
  }

#if defined(OPENFLUID_OS_UNIX)
  {
    openfluid::utils::Process::Environment Env;
    Env.Vars = {{"OFTESTV1","variable 1"},{"OFTESTV2","variable 2"}};
    openfluid::utils::Process P("/bin/sh",{"-c","'env'"},Env);
    BOOST_CHECK(P.run());
    BOOST_CHECK_EQUAL(P.getExitCode(),0);
    for (const auto& L : P.stdOutLines())
    {
      std::cout << L << std::endl;
    }
  }

  std::cout << "=====================" << std::endl;

  {
    openfluid::utils::Process::Environment Env;
    Env.Inherits = false;
    Env.Vars = {{"OFTESTV1","variable 1"},{"OFTESTV2","variable 2"}};
    openfluid::utils::Process P("/bin/sh",{"-c","'env'"},Env);
    BOOST_CHECK(P.run());
    BOOST_CHECK_EQUAL(P.getExitCode(),0);
    for (const auto& L : P.stdOutLines())
    {
      std::cout << L << std::endl;
    }
  }
#endif
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_static)
{
  BOOST_CHECK_EQUAL(openfluid::utils::Process::execute(CONFIGTESTS_CMAKE_PROGRAM),0);

  BOOST_CHECK_EQUAL(openfluid::utils::Process::execute(CONFIGTESTS_CMAKE_PROGRAM,{"--help"}),0);

  BOOST_CHECK_NE(openfluid::utils::Process::execute(CONFIGTESTS_CMAKE_PROGRAM,{"--option-that-does-not-exist"}),0);

  {
    openfluid::utils::Process::Command Cmd{
      .Program = CONFIGTESTS_CMAKE_PROGRAM,
      .Args = {"--option-that-does-not-exist"},
      .OutFile = CONFIGTESTS_OUTPUT_DATA_DIR+"/Process/execute-cmakewrong_out.log",
      .ErrFile = CONFIGTESTS_OUTPUT_DATA_DIR+"/Process/execute-cmakewrong_err.log",
    };

    BOOST_CHECK_NE(openfluid::utils::Process::execute(Cmd),0);
  }

#if defined(OPENFLUID_OS_UNIX)
  {
    openfluid::utils::Process::Environment Env;
    Env.Vars = {{"OFTESTV1","variable 1"},{"OFTESTV2","variable 2"}};
    openfluid::utils::Process::Command Cmd{
      .Program = "/bin/sh",
      .Args = {"-c","'env'"}
    };
    Cmd.setOutErrFiles(CONFIGTESTS_OUTPUT_DATA_DIR+"/Process","execute-env");
    BOOST_CHECK_EQUAL(openfluid::utils::Process::execute(Cmd,Env),0);
  }

  {
    openfluid::utils::Process::Environment Env;
    Env.Vars = {{"OFTESTV1","variable 1"},{"OFTESTV2","variable 2"}};
    Env.Inherits = false;
    openfluid::utils::Process::Command Cmd{
      .Program = "/bin/sh",
      .Args = {"-c","'env'"},
      .OutFile = CONFIGTESTS_OUTPUT_DATA_DIR+"/Process/execute-envnoinh_out.log",
      .ErrFile = CONFIGTESTS_OUTPUT_DATA_DIR+"/Process/execute-envnoinh_err.log",
    };
    BOOST_CHECK_EQUAL(openfluid::utils::Process::execute(Cmd,Env),0);
  }

#endif

  BOOST_CHECK_EQUAL(openfluid::utils::Process::system(CONFIGTESTS_CMAKE_PROGRAM,{"--version"}),0);

  BOOST_CHECK_NE(openfluid::utils::Process::system(CONFIGTESTS_CMAKE_PROGRAM,{"--option-that-does-not-exist"}),0);
}

