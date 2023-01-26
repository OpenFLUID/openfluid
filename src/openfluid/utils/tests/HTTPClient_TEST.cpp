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
  @file HTTPClient_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_httpclient


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/utils/HTTPClient.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_keyvaluelist)
{
  openfluid::utils::HTTPClient::KeyValueList KVL;

  BOOST_REQUIRE(KVL.Data.empty());

  KVL.add("k1","v1");
  KVL.add("k2","v2");
  KVL.add("k3","v3");

  BOOST_REQUIRE_EQUAL(KVL.Data.size(),3);
  BOOST_REQUIRE(KVL.exists("k1"));
  BOOST_REQUIRE(KVL.exists("k3"));
  BOOST_REQUIRE(KVL.exists("k2"));
  BOOST_REQUIRE(!KVL.exists("k17"));

  KVL.remove("k2");
  BOOST_REQUIRE(KVL.exists("k1"));
  BOOST_REQUIRE(KVL.exists("k3"));
  BOOST_REQUIRE(!KVL.exists("k2"));
  BOOST_REQUIRE(!KVL.exists("k17"));

  KVL.remove("k1");
  KVL.remove("k2");
  KVL.remove("k3");
  BOOST_REQUIRE(!KVL.exists("k1"));
  BOOST_REQUIRE(!KVL.exists("k3"));
  BOOST_REQUIRE(!KVL.exists("k2"));

  BOOST_REQUIRE(KVL.Data.empty());
}


// =====================================================================
// =====================================================================


void checkGet(const openfluid::utils::HTTPClient& Client)
{
  openfluid::utils::HTTPClient::Response Resp;

  Resp = Client.getResource({.Path="/users"});
  std::cout << "GET /users : " << Resp.StatusCode << std::endl;
  BOOST_CHECK_EQUAL(Resp.StatusCode,200);
  BOOST_CHECK(Resp.isOK());
  BOOST_CHECK(openfluid::tools::contains(Resp.Content,"\"username\": \"Bret\""));

  Resp = Client.getResource({.Path="/users/23"});
  std::cout << "GET /users/23 : " << Resp.StatusCode << std::endl;
  std::cout << "  - " << Resp.NetworkError << ": " << Resp.ErrorStr << std::endl;
  BOOST_CHECK(!Resp.isOK());
  BOOST_CHECK_EQUAL(Resp.StatusCode,404);

  Resp = Client.getResource({.Path="/users/2"});
  std::cout << "GET /users/2 : " << Resp.StatusCode << std::endl;
  BOOST_CHECK(Resp.isOK());
  BOOST_CHECK_EQUAL(Resp.StatusCode,200);
  BOOST_CHECK(openfluid::tools::contains(Resp.Content,"\"username\": \"Antonette\""));
}


// =====================================================================
// =====================================================================


void checkPost(const openfluid::utils::HTTPClient& Client)
{
  openfluid::utils::HTTPClient::Response Resp;

  Resp = Client.postResource({.Path="/auth"});
  std::cout << "POST /auth (no body): " << Resp.StatusCode << std::endl;
  BOOST_CHECK(!Resp.isOK());

  Resp = Client.postResource({.Path="/auth", .Body="{\"username\":\"Antonette\",\"password\":\"\"}"});
  std::cout << "POST /auth (bad body): " << Resp.StatusCode << std::endl;
  BOOST_CHECK(!Resp.isOK());

  Resp = Client.postResource({.Path="/auth",
                              .Body="{\"username\":\"Antonette\",\"password\":\"an70_n3773\"}"});
  std::cout << "POST /auth (correct body): " << Resp.StatusCode << std::endl;
  BOOST_CHECK(Resp.isOK());
  BOOST_CHECK_EQUAL(Resp.StatusCode,200);
  BOOST_CHECK(openfluid::tools::contains(Resp.Content,"\"token\": \"2b01d9d5"));
}


// =====================================================================
// =====================================================================


void checkPut(const openfluid::utils::HTTPClient& Client)
{
  BOOST_CHECK_THROW(Client.putResource({}), openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


void checkPatch(const openfluid::utils::HTTPClient& Client)
{
  BOOST_CHECK_THROW(Client.patchResource({}), openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


void checkDelete(const openfluid::utils::HTTPClient& Client)
{
  BOOST_CHECK_THROW(Client.deleteResource({}), openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_actions)
{
  for (const auto& U : {CONFIGTESTS_RESTSERVICE_URL_HTTP,CONFIGTESTS_RESTSERVICE_URL_HTTPS})
  {
    if (!U.empty())
    {
      std::cout << " ======== " << U << " ========" << std::endl;
      openfluid::utils::HTTPClient Client(U);

      if (Client.getResource({.Path="/users"}).isOK())
      {
        checkGet(Client);
        checkPost(Client);
        checkPut(Client);
        checkPatch(Client);
        checkDelete(Client);
      }
      else
      {
        std::cout << "not reachable" << std::endl;
      }
    }
    else
    {
      std::cout << "http URL is empty" << std::endl;
    }
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_downloads)
{
  std::string LoremIpsum = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, "
                           "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
                           "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "
                           "aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in "
                           "voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint "
                           "occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim "
                           "id est laborum.";

  std::string LoremIpsumDLoaded;

  std::cout << "Downloading from " << CONFIGTESTS_FILEDOWNLOAD_URL << std::endl;


  BOOST_CHECK(openfluid::utils::HTTPClient::downloadToString(CONFIGTESTS_FILEDOWNLOAD_URL,LoremIpsumDLoaded));
  BOOST_CHECK_EQUAL(LoremIpsum,LoremIpsumDLoaded);

  std::string LI1, LI2;
  openfluid::utils::HTTPClient::downloadToString(CONFIGTESTS_FILEDOWNLOAD_URL,LI1);
  openfluid::utils::HTTPClient::downloadToString(CONFIGTESTS_FILEDOWNLOAD_URL,LI2);
  BOOST_CHECK_EQUAL(LI1,LI2);


  auto FileDloadDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/HTTPClient";
  auto FileDloadDirFSP = openfluid::tools::FilesystemPath(FileDloadDir);

  FileDloadDirFSP.removeFile("lorem_ipsum_dload.txt");
  FileDloadDirFSP.makeDirectory();

  BOOST_CHECK(openfluid::utils::HTTPClient::downloadToFile(CONFIGTESTS_FILEDOWNLOAD_URL,
                                                           FileDloadDir+"/lorem_ipsum_dload.txt"));
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  if (!CONFIGTESTS_ALLOW_NETWORK_REQUESTS)
  {
    std::cout << "** Test not run due to disabled network requests **" << std::endl;
    return CONFIGTESTS_SKIP_CODE;
  }

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}

