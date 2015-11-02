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
  @file RESTClient_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_restclient
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/cstdlib.hpp>

#include <iostream>

#include <QCoreApplication>

#include <openfluid/utils/RESTClient.hpp>
#include <openfluid/base/FrameworkException.hpp>

#include <tests-config.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_get)
{
  openfluid::utils::RESTClient Client;
  openfluid::utils::RESTClient::Reply Reply;

  Client.setBaseURL(QString::fromStdString(CONFIGTESTS_RESTSERVICE_URL));

  Reply = Client.getResource("/users");
  std::cout << "GET /users : " << Reply.getStatusCode() << std::endl;
  BOOST_REQUIRE(Reply.isOK());
  BOOST_REQUIRE_EQUAL(Reply.getStatusCode(),200);
  BOOST_REQUIRE_NE(Reply.getContent().indexOf("\"username\": \"Bret\""),-1);

  Reply = Client.getResource("/users/23");
  std::cout << "GET /users/23 : " << Reply.getStatusCode() << std::endl;
  std::cout << "  - " << Reply.getNetworkErrorCode() << ": " <<
                         Reply.getNetworkErrorString().toStdString() << std::endl;
  BOOST_REQUIRE(!Reply.isOK());
  BOOST_REQUIRE_EQUAL(Reply.getStatusCode(),404);

  Reply = Client.getResource("/users/2");
  std::cout << "GET /users/2 : " << Reply.getStatusCode() << std::endl;
  BOOST_REQUIRE(Reply.isOK());
  BOOST_REQUIRE_EQUAL(Reply.getStatusCode(),200);
  BOOST_REQUIRE_NE(Reply.getContent().indexOf("\"username\": \"Antonette\""),-1);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_post)
{
  openfluid::utils::RESTClient Client;

  BOOST_REQUIRE_THROW(Client.postResource("",""), openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_put)
{
  openfluid::utils::RESTClient Client;

  BOOST_REQUIRE_THROW(Client.putResource("",""), openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_patch)
{
  openfluid::utils::RESTClient Client;

  BOOST_REQUIRE_THROW(Client.patchResource("",""), openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_delete)
{
  openfluid::utils::RESTClient Client;

  BOOST_REQUIRE_THROW(Client.deleteResource("",""), openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  if (CONFIGTESTS_RESTSERVICE_URL.empty())
    return 0;

  QCoreApplication app(argc, argv);

  openfluid::utils::RESTClient Client;
  Client.setBaseURL(QString::fromStdString(CONFIGTESTS_RESTSERVICE_URL));

  if (!Client.getResource("/users").isOK())
  {
    std::cout << "** Test not run due to failing connection to remote service **" << std::endl;
    return 0;
  }

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}
