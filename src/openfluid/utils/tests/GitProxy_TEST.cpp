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
  @file GitProxy_TEST.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_process


#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/utils/Process.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_ops)
{
  std::string RepoName = "dummy_repo";
  auto RepoPath = openfluid::tools::FilesystemPath({CONFIGTESTS_TMP_DIR, RepoName});
  // cleaning repo if exists
  RepoPath.removeDirectory();
  RepoPath.makeDirectory();

  openfluid::utils::GitProxy Git;
  BOOST_REQUIRE(!Git.isPathGitRepo(RepoPath.toGeneric()));

  openfluid::utils::Process::Command Cmd{
    .Program = "git",
    .Args = {"init"},
    .WorkDir = RepoPath.toGeneric()
  };
  openfluid::utils::Process P(Cmd);
  BOOST_CHECK(P.run());
  BOOST_CHECK_EQUAL(P.getExitCode(),0);
  BOOST_REQUIRE(Git.isPathGitRepo(RepoPath.toGeneric()));

  BOOST_REQUIRE_EQUAL(Git.getCurrentBranchName(RepoPath.toGeneric()), "master");


  openfluid::utils::Process::Command CmdCheckout{
    .Program = "git",
    .Args = {"checkout", "-b", "foo"},
    .WorkDir = RepoPath.toGeneric()
  };
  openfluid::utils::Process PCheckout(CmdCheckout);
  BOOST_CHECK(PCheckout.run());
  BOOST_CHECK_EQUAL(PCheckout.getExitCode(),0);
  BOOST_REQUIRE_EQUAL(Git.getCurrentBranchName(RepoPath.toGeneric()), "foo");

  RepoPath.removeDirectory();
}

