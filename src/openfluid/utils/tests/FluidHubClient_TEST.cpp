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
  @file FluidHubClient_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_fluidhubclient
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <QCoreApplication>

#include <openfluid/utils/FluidHubClient.hpp>

#include <tests-config.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_connection)
{
  openfluid::utils::FluidHubClient FHClient;

  BOOST_REQUIRE(FHClient.connect(CONFIGTESTS_FLUIDHUB_URL));

  BOOST_REQUIRE(FHClient.isConnected());
  BOOST_REQUIRE_EQUAL(FHClient.getHubStatus(),"testing");
  BOOST_REQUIRE_EQUAL(FHClient.getHubName(),"fluidhub for testing");
  BOOST_REQUIRE_EQUAL(FHClient.getHubURL(),CONFIGTESTS_FLUIDHUB_URL);
  BOOST_REQUIRE(!FHClient.getHubAPIVersion().empty());
  BOOST_REQUIRE_EQUAL(FHClient.getHubCapabilities().size(),2);

  std::set<std::string> Capabilities = FHClient.getHubCapabilities();
  BOOST_REQUIRE(Capabilities.find("news") != Capabilities.end());
  BOOST_REQUIRE(Capabilities.find("wareshub") != Capabilities.end());
  BOOST_REQUIRE(Capabilities.find("nonsense") == Capabilities.end());

  std::cout << "HubURL: " << FHClient.getHubURL() << std::endl;
  std::cout << "HubName: " << FHClient.getHubName() << std::endl;
  std::cout << "HubAPIVersion: " << FHClient.getHubAPIVersion() << std::endl;


  FHClient.disconnect();

  BOOST_REQUIRE(!FHClient.isConnected());
  BOOST_REQUIRE(FHClient.getHubStatus().empty());
  BOOST_REQUIRE(FHClient.getHubName().empty());
  BOOST_REQUIRE(FHClient.getHubURL().empty());
  BOOST_REQUIRE(FHClient.getHubAPIVersion().empty());
  BOOST_REQUIRE(FHClient.getHubCapabilities().empty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::utils::FluidHubClient FHClient;

  BOOST_REQUIRE(FHClient.connect(CONFIGTESTS_FLUIDHUB_URL));
  BOOST_REQUIRE(FHClient.isConnected());

  // Wares

  openfluid::utils::FluidHubClient::WaresDescByType_t WaresDesc = FHClient.getAvailableWares();

  BOOST_REQUIRE_EQUAL(WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].size(),4);
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.A") !=
                WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.B") !=
                WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.C") !=
                WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.D") !=
                WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.E") ==
                WaresDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());

  BOOST_REQUIRE_EQUAL(WaresDesc[openfluid::utils::FluidHubClient::OBSERVER].size(),2);
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::OBSERVER].find("examples.obs.J") !=
                WaresDesc[openfluid::utils::FluidHubClient::OBSERVER].end());
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::OBSERVER].find("examples.obs.K") !=
                WaresDesc[openfluid::utils::FluidHubClient::OBSERVER].end());
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::OBSERVER].find("examples.obs.L") ==
                WaresDesc[openfluid::utils::FluidHubClient::OBSERVER].end());

  BOOST_REQUIRE_EQUAL(WaresDesc[openfluid::utils::FluidHubClient::BUILDEREXT].size(),1);
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::BUILDEREXT].find("examples.bext.X") !=
                WaresDesc[openfluid::utils::FluidHubClient::BUILDEREXT].end());
  BOOST_REQUIRE(WaresDesc[openfluid::utils::FluidHubClient::BUILDEREXT].find("examples.bext.Y") ==
                WaresDesc[openfluid::utils::FluidHubClient::BUILDEREXT].end());


  // Detailed wares

  openfluid::utils::FluidHubClient::WaresDetailedDescByType_t WaresDetailedDesc =
      FHClient.getAvailableWaresWithDetails();

  // simulators
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].size(),4);

  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.A") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].ShortDescription,
                      "This is examples.sim.A...");
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].GitUrl,
                      "https://host.domain.org/foo-wareshub/git/simulators/examples.sim.A");
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].ROUsers.size(),
                      1);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].ROUsers.find("*") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].ROUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].ROUsers.find("user1")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].ROUsers.end());
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.size(),
                      3);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.find("*") ==
                WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.find("user1")
                != WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.find("user2")
                != WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.find("user3")
                != WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.find("user4")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.A"].RWUsers.end());

  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.B") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());

  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.C") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].ShortDescription,
                      "This is examples.sim.C...");
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].GitUrl,
                      "https://host.domain.org/foo-wareshub/git/simulators/examples.sim.C");
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].ROUsers.size(),
                      1);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].ROUsers.find("*") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].ROUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].ROUsers.find("user3")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].ROUsers.end());
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.size(),
                      2);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.find("*") ==
                WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.find("user1")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.find("user2")
                != WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.find("user3")
                != WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.find("user4")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].RWUsers.end());

  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.D") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());

  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].find("examples.sim.E") ==
                WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR].end());

  // observers

  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER].size(),2);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER].find("examples.obs.J") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER].end());
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].ShortDescription,
                      "This is examples.obs.J...");
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].GitUrl,
                      "https://host.domain.org/foo-wareshub/git/observers/examples.obs.J");
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].ROUsers.size(),
                      1);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].ROUsers.find("*") ==
                WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].ROUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].ROUsers.find("user1")
                != WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].ROUsers.end());
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].RWUsers.size(),
                      1);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].RWUsers.find("*") ==
                WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].RWUsers.find("user1")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].RWUsers.find("user2")
                != WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].RWUsers.find("user3")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].RWUsers.end());

  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER].find("examples.obs.K") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER].end());

  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER].find("examples.obs.L") ==
                WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER].end());

  // builderexts

  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT].size(),1);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT].find("examples.bext.X") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT].end());
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"]
                                                                                      .ShortDescription,
                      "This is examples.bext.X...");
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].GitUrl,
                      "https://host.domain.org/foo-wareshub/git/builderexts/examples.bext.X");
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].ROUsers.size(),
                      0);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].ROUsers.find("*") ==
                WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].ROUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].ROUsers.find("user1")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].ROUsers.end());
  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].RWUsers.size(),
                      1);
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].RWUsers.find("*") !=
                WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].RWUsers.find("user1")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].RWUsers.end());
  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].RWUsers.find("user2")
                == WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].RWUsers.end());

  BOOST_REQUIRE(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT].find("examples.bext.Y") ==
                WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT].end());

  // with username

  WaresDetailedDesc = FHClient.getAvailableWaresWithDetails("fakeuser");

  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::SIMULATOR]["examples.sim.C"].GitUrl,
                      "https://fakeuser@host.domain.org/foo-wareshub/git/simulators/examples.sim.C");

  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::OBSERVER]["examples.obs.J"].GitUrl,
                      "https://fakeuser@host.domain.org/foo-wareshub/git/observers/examples.obs.J");

  BOOST_REQUIRE_EQUAL(WaresDetailedDesc[openfluid::utils::FluidHubClient::BUILDEREXT]["examples.bext.X"].GitUrl,
                      "https://fakeuser@host.domain.org/foo-wareshub/git/builderexts/examples.bext.X");

  // News

  std::string NewsContent = FHClient.getNews();
  BOOST_REQUIRE(!NewsContent.empty());

  NewsContent = FHClient.getNews("fr");
  BOOST_REQUIRE(!NewsContent.empty());

  NewsContent = FHClient.getNews("groland");
  BOOST_REQUIRE(NewsContent.empty());


  FHClient.disconnect();
  BOOST_REQUIRE(!FHClient.isConnected());



  BOOST_REQUIRE(FHClient.connect(CONFIGTESTS_FLUIDHUB_URL));
  BOOST_REQUIRE(FHClient.isConnected());


  FHClient.disconnect();
  BOOST_REQUIRE(!FHClient.isConnected());
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  if (CONFIGTESTS_FLUIDHUB_URL.empty())
    return 0;

  QCoreApplication app(argc, argv);

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}
