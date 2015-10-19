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
  @file FluidHubAPIClient_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_fluidhubapiclient
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <QCoreApplication>

#include <openfluid/utils/FluidHubAPIClient.hpp>

#include <tests-config.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_connection)
{
  openfluid::utils::FluidHubAPIClient FHClient;

  BOOST_REQUIRE(FHClient.connect(QString::fromStdString(CONFIGTESTS_FLUIDHUB_URL)));

  BOOST_REQUIRE(FHClient.isConnected());
  BOOST_REQUIRE_EQUAL(FHClient.getHubStatus().toStdString(),"testing");
  BOOST_REQUIRE_EQUAL(FHClient.getHubName().toStdString(),"fluidhub for testing");
  BOOST_REQUIRE_EQUAL(FHClient.getHubURL().toStdString(),CONFIGTESTS_FLUIDHUB_URL);
  BOOST_REQUIRE(!FHClient.getHubAPIVersion().isEmpty());
  BOOST_REQUIRE_EQUAL(FHClient.getHubCapabilities().size(),2);

  std::set<QString> Capabilities = FHClient.getHubCapabilities();
  BOOST_REQUIRE(Capabilities.find("news") != Capabilities.end());
  BOOST_REQUIRE(Capabilities.find("wareshub") != Capabilities.end());
  BOOST_REQUIRE(Capabilities.find("nonsense") == Capabilities.end());

  std::cout << "HubURL: " << FHClient.getHubURL().toStdString() << std::endl;
  std::cout << "HubName: " << FHClient.getHubName().toStdString() << std::endl;
  std::cout << "HubAPIVersion: " << FHClient.getHubAPIVersion().toStdString() << std::endl;


  FHClient.disconnect();

  BOOST_REQUIRE(!FHClient.isConnected());
  BOOST_REQUIRE(FHClient.getHubStatus().isEmpty());
  BOOST_REQUIRE(FHClient.getHubName().isEmpty());
  BOOST_REQUIRE(FHClient.getHubURL().isEmpty());
  BOOST_REQUIRE(FHClient.getHubAPIVersion().isEmpty());
  BOOST_REQUIRE(FHClient.getHubCapabilities().empty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::utils::FluidHubAPIClient FHClient;

  BOOST_REQUIRE(FHClient.connect(QString::fromStdString(CONFIGTESTS_FLUIDHUB_URL)));
  BOOST_REQUIRE(FHClient.isConnected());

  {
    // Wares

    openfluid::utils::FluidHubAPIClient::WaresListByType_t WaresDesc = FHClient.getAllAvailableWares();

    BOOST_REQUIRE_EQUAL(WaresDesc[openfluid::ware::WareType::SIMULATOR].size(),4);
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::SIMULATOR].find("examples.sim.A") !=
        WaresDesc[openfluid::ware::WareType::SIMULATOR].end());
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::SIMULATOR].find("examples.sim.B") !=
        WaresDesc[openfluid::ware::WareType::SIMULATOR].end());
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::SIMULATOR].find("examples.sim.C") !=
        WaresDesc[openfluid::ware::WareType::SIMULATOR].end());
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::SIMULATOR].find("examples.sim.D") !=
        WaresDesc[openfluid::ware::WareType::SIMULATOR].end());
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::SIMULATOR].find("examples.sim.E") ==
        WaresDesc[openfluid::ware::WareType::SIMULATOR].end());

    BOOST_REQUIRE_EQUAL(WaresDesc[openfluid::ware::WareType::OBSERVER].size(),2);
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::OBSERVER].find("examples.obs.J") !=
        WaresDesc[openfluid::ware::WareType::OBSERVER].end());
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::OBSERVER].find("examples.obs.K") !=
        WaresDesc[openfluid::ware::WareType::OBSERVER].end());
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::OBSERVER].find("examples.obs.L") ==
        WaresDesc[openfluid::ware::WareType::OBSERVER].end());

    BOOST_REQUIRE_EQUAL(WaresDesc[openfluid::ware::WareType::BUILDEREXT].size(),1);
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::BUILDEREXT].find("examples.bext.X") !=
        WaresDesc[openfluid::ware::WareType::BUILDEREXT].end());
    BOOST_REQUIRE(WaresDesc[openfluid::ware::WareType::BUILDEREXT].find("examples.bext.Y") ==
        WaresDesc[openfluid::ware::WareType::BUILDEREXT].end());

  }


  // Detailed wares

  {

    // simulators

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t SimsDetailedDesc =
        FHClient.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR);


    BOOST_REQUIRE_EQUAL(SimsDetailedDesc.size(),4);

    BOOST_REQUIRE(SimsDetailedDesc.find("examples.sim.A") !=
        SimsDetailedDesc.end());
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].ShortDescription,
                        "This is examples.sim.A...");
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].GitUrl,
                        "https://host.domain.org/foo-wareshub/git/simulators/examples.sim.A");
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].IssuesCounters["bugs"],1);
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].IssuesCounters["features"],0);
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].IssuesCounters["reviews"],2);
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].GitBranches.size(),2);
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].GitBranches[0],"openfluid-2.0");
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].GitBranches[1],"openfluid-2.1");
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].ROUsers.size(),1);
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.A"].ROUsers.find("*") !=
        SimsDetailedDesc["examples.sim.A"].ROUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.A"].ROUsers.find("user1")
                  == SimsDetailedDesc["examples.sim.A"].ROUsers.end());
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.A"].RWUsers.size(),3);
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.A"].RWUsers.find("*") ==
        SimsDetailedDesc["examples.sim.A"].RWUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.A"].RWUsers.find("user1")
                  != SimsDetailedDesc["examples.sim.A"].RWUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.A"].RWUsers.find("user2")
                  != SimsDetailedDesc["examples.sim.A"].RWUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.A"].RWUsers.find("user3")
                  != SimsDetailedDesc["examples.sim.A"].RWUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.A"].RWUsers.find("user4")
                  == SimsDetailedDesc["examples.sim.A"].RWUsers.end());

    BOOST_REQUIRE(SimsDetailedDesc.find("examples.sim.B") != SimsDetailedDesc.end());
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.B"].GitBranches.size(),3);

    BOOST_REQUIRE(SimsDetailedDesc.find("examples.sim.C") !=
        SimsDetailedDesc.end());
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.C"].ShortDescription,"This is examples.sim.C...");
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.C"].GitUrl,
                        "https://host.domain.org/foo-wareshub/git/simulators/examples.sim.C");
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.C"].GitBranches.size(),3);
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.C"].GitBranches[0],"openfluid-2.0");
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.C"].GitBranches[1],"openfluid-2.1");
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.C"].GitBranches[2],"openfluid-2.2-preview");

    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.C"].ROUsers.size(),1);
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.C"].ROUsers.find("*") !=
        SimsDetailedDesc["examples.sim.C"].ROUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.C"].ROUsers.find("user3")
                  == SimsDetailedDesc["examples.sim.C"].ROUsers.end());
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.C"].RWUsers.size(),
                        2);
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.C"].RWUsers.find("*") ==
        SimsDetailedDesc["examples.sim.C"].RWUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.C"].RWUsers.find("user1")
                  == SimsDetailedDesc["examples.sim.C"].RWUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.C"].RWUsers.find("user2")
                  != SimsDetailedDesc["examples.sim.C"].RWUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.C"].RWUsers.find("user3")
                  != SimsDetailedDesc["examples.sim.C"].RWUsers.end());
    BOOST_REQUIRE(SimsDetailedDesc["examples.sim.C"].RWUsers.find("user4")
                  == SimsDetailedDesc["examples.sim.C"].RWUsers.end());

    BOOST_REQUIRE(SimsDetailedDesc.find("examples.sim.D") !=
        SimsDetailedDesc.end());
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.D"].IssuesCounters["bugs"],1);
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.D"].IssuesCounters["features"],1);
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.D"].IssuesCounters["reviews"],1);
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.D"].GitBranches.size(),1);
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.D"].GitBranches[0],"master");

    BOOST_REQUIRE(SimsDetailedDesc.find("examples.sim.E") ==
        SimsDetailedDesc.end());
  }

  {
    // observers

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t ObssDetailedDesc =
        FHClient.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER);


    BOOST_REQUIRE_EQUAL(ObssDetailedDesc.size(),2);
    BOOST_REQUIRE(ObssDetailedDesc.find("examples.obs.J") !=
        ObssDetailedDesc.end());
    BOOST_REQUIRE_EQUAL(ObssDetailedDesc["examples.obs.J"].ShortDescription,
                        "This is examples.obs.J...");
    BOOST_REQUIRE_EQUAL(ObssDetailedDesc["examples.obs.J"].GitUrl,
                        "https://host.domain.org/foo-wareshub/git/observers/examples.obs.J");
    BOOST_REQUIRE_EQUAL(ObssDetailedDesc["examples.obs.J"].ROUsers.size(),1);
    BOOST_REQUIRE(ObssDetailedDesc["examples.obs.J"].ROUsers.find("*") ==
        ObssDetailedDesc["examples.obs.J"].ROUsers.end());
    BOOST_REQUIRE(ObssDetailedDesc["examples.obs.J"].ROUsers.find("user1")
                  != ObssDetailedDesc["examples.obs.J"].ROUsers.end());
    BOOST_REQUIRE_EQUAL(ObssDetailedDesc["examples.obs.J"].RWUsers.size(),
                        1);
    BOOST_REQUIRE(ObssDetailedDesc["examples.obs.J"].RWUsers.find("*") ==
        ObssDetailedDesc["examples.obs.J"].RWUsers.end());
    BOOST_REQUIRE(ObssDetailedDesc["examples.obs.J"].RWUsers.find("user1")
                  == ObssDetailedDesc["examples.obs.J"].RWUsers.end());
    BOOST_REQUIRE(ObssDetailedDesc["examples.obs.J"].RWUsers.find("user2")
                  != ObssDetailedDesc["examples.obs.J"].RWUsers.end());
    BOOST_REQUIRE(ObssDetailedDesc["examples.obs.J"].RWUsers.find("user3")
                  == ObssDetailedDesc["examples.obs.J"].RWUsers.end());

    BOOST_REQUIRE(ObssDetailedDesc.find("examples.obs.K") !=
        ObssDetailedDesc.end());
    BOOST_REQUIRE_EQUAL(ObssDetailedDesc["examples.obs.K"].IssuesCounters["bugs"],0);
    BOOST_REQUIRE_EQUAL(ObssDetailedDesc["examples.obs.K"].IssuesCounters["features"],0);
    BOOST_REQUIRE_EQUAL(ObssDetailedDesc["examples.obs.K"].IssuesCounters["reviews"],0);
    BOOST_REQUIRE_EQUAL(ObssDetailedDesc["examples.obs.K"].GitBranches.size(),0);

    BOOST_REQUIRE(ObssDetailedDesc.find("examples.obs.L") ==
        ObssDetailedDesc.end());
  }


  {
    // builderexts

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t BextsDetailedDesc =
        FHClient.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT);

    BOOST_REQUIRE_EQUAL(BextsDetailedDesc.size(),1);
    BOOST_REQUIRE(BextsDetailedDesc.find("examples.bext.X") != BextsDetailedDesc.end());
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].ShortDescription,"This is examples.bext.X...");
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].GitUrl,
                        "https://host.domain.org/foo-wareshub/git/builderexts/examples.bext.X");
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].GitBranches.size(),2);
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].GitBranches[0],"openfluid-2.0");
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].GitBranches[1],"openfluid-2.1");
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].IssuesCounters["bugs"],1);
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].IssuesCounters["features"],0);
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].IssuesCounters["reviews"],2);
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].ROUsers.size(),0);
    BOOST_REQUIRE(BextsDetailedDesc["examples.bext.X"].ROUsers.find("*") ==
        BextsDetailedDesc["examples.bext.X"].ROUsers.end());
    BOOST_REQUIRE(BextsDetailedDesc["examples.bext.X"].ROUsers.find("user1")
                  == BextsDetailedDesc["examples.bext.X"].ROUsers.end());
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].RWUsers.size(),1);
    BOOST_REQUIRE(BextsDetailedDesc["examples.bext.X"].RWUsers.find("*") !=
        BextsDetailedDesc["examples.bext.X"].RWUsers.end());
    BOOST_REQUIRE(BextsDetailedDesc["examples.bext.X"].RWUsers.find("user1")
                  == BextsDetailedDesc["examples.bext.X"].RWUsers.end());
    BOOST_REQUIRE(BextsDetailedDesc["examples.bext.X"].RWUsers.find("user2")
                  == BextsDetailedDesc["examples.bext.X"].RWUsers.end());

    BOOST_REQUIRE(BextsDetailedDesc.find("examples.bext.Y") ==
        BextsDetailedDesc.end());
  }



  {
    // with username

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t SimsDetailedDesc =
        FHClient.getAvailableWaresWithDetails(openfluid::ware::WareType::SIMULATOR,"fakeuser");
    BOOST_REQUIRE_EQUAL(SimsDetailedDesc["examples.sim.C"].GitUrl,
                        "https://fakeuser@host.domain.org/foo-wareshub/git/simulators/examples.sim.C");

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t ObssDetailedDesc =
        FHClient.getAvailableWaresWithDetails(openfluid::ware::WareType::OBSERVER,"fakeuser");
    BOOST_REQUIRE_EQUAL(ObssDetailedDesc["examples.obs.J"].GitUrl,
                        "https://fakeuser@host.domain.org/foo-wareshub/git/observers/examples.obs.J");

    openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t BextsDetailedDesc =
        FHClient.getAvailableWaresWithDetails(openfluid::ware::WareType::BUILDEREXT,"fakeuser");
    BOOST_REQUIRE_EQUAL(BextsDetailedDesc["examples.bext.X"].GitUrl,
                        "https://fakeuser@host.domain.org/foo-wareshub/git/builderexts/examples.bext.X");

  }


  {
    // News

    QString NewsContent = FHClient.getNews();
    BOOST_REQUIRE(!NewsContent.isEmpty());

    NewsContent = FHClient.getNews("fr");
    BOOST_REQUIRE(!NewsContent.isEmpty());

    NewsContent = FHClient.getNews("grd");
    BOOST_REQUIRE(NewsContent.isEmpty());

  }


  FHClient.disconnect();
  BOOST_REQUIRE(!FHClient.isConnected());



  BOOST_REQUIRE(FHClient.connect(QString::fromStdString(CONFIGTESTS_FLUIDHUB_URL)));
  BOOST_REQUIRE(FHClient.isConnected());


  FHClient.disconnect();
  BOOST_REQUIRE(!FHClient.isConnected());
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  if (CONFIGTESTS_FLUIDHUB_URL.empty())
    return CONFIGTESTS_NOTRUN_EXITCODE;

  QCoreApplication app(argc, argv);

  openfluid::utils::RESTClient Client;
  Client.setBaseURL(QString::fromStdString(CONFIGTESTS_FLUIDHUB_URL));

  if (!Client.getResource("/").isOK())
  {
    std::cout << "** Test not run due to failing connection to remote service **" << std::endl;
    return CONFIGTESTS_NOTRUN_EXITCODE;
  }


  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}

