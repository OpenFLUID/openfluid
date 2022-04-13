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
  @file FileDownloader_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_filedownloader


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <QCoreApplication>

#include <openfluid/utils/FileDownloader.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/FilesystemPath.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  std::string LoremIpsum = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, "
                           "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
                           "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "
                           "aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in "
                           "voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint "
                           "occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim "
                           "id est laborum.";

  std::string LoremIpsumDLoaded;


  BOOST_CHECK(openfluid::utils::FileDownloader::downloadToString(CONFIGTESTS_FILEDOWNLOAD_URL,LoremIpsumDLoaded));

  BOOST_CHECK_EQUAL(LoremIpsum,LoremIpsumDLoaded);

  auto FileDloadDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/FileDownloader";
  auto FileDloadDirFSP = openfluid::tools::FilesystemPath(FileDloadDir);

  FileDloadDirFSP.removeFile("lorem_ipsum_dload.txt");
  FileDloadDirFSP.makeDirectory();


  BOOST_CHECK(
      openfluid::utils::FileDownloader::downloadToFile(CONFIGTESTS_FILEDOWNLOAD_URL,
                                                       FileDloadDir+"/lorem_ipsum_dload.txt"));

  std::string LI1, LI2;

  openfluid::utils::FileDownloader::downloadToString(CONFIGTESTS_FILEDOWNLOAD_URL,
                                                     LI1);
  openfluid::utils::FileDownloader::downloadToString(CONFIGTESTS_FILEDOWNLOAD_URL,
                                                     LI2);

  BOOST_CHECK_EQUAL(LI1,LI2);
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  if (!CONFIGTESTS_ALLOW_NETWORK_REQUESTS)
  {
    std::cout << "** Test not run due to disabled network requests **" << std::endl;
    return CONFIGTESTS_SKIP_CODE;
  }

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}

