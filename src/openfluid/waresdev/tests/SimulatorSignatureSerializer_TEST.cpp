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
  @file SimulatorSignatureSerializer_TEST.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_simulatorsignatureserializer


#include <boost/test/unit_test.hpp>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>

#include "SignatureCommon.hpp"
#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const auto WorkPath = openfluid::tools::Path({CONFIGTESTS_OUTPUT_DATA_DIR,"SignatureSerializer","simulator"});


openfluid::ware::SimulatorSignature getRefSignature()
{
  openfluid::ware::SimulatorSignature Sign;

  Sign.ID = "unit.test.simulator-signature";
  Sign.Name = "Simulator signature for unit testing";
  Sign.Description = "This is a simulator signature\n"
                     " that should be used for **unit testing only**";
  Sign.Version = "2022.07";
  Sign.Status = openfluid::ware::BETA;

  Sign.Authors = {
    {"Kurt Cobain","kurt@nirvana.org"},
    {"Dave Grohl","dave@nirvana.org"},
    {"Krist Novoselic","krist@nirvana.org"}
  };
  Sign.Contacts = {{"OpenFLUID contact","contact@openfluid-project.org"}};
  Sign.License = "GPL-3.0-or-later";

  Sign.Tags = {"simulator","unit testing","development","software quality"}  ;
  Sign.Links = {
    {"OpenFLUID","www.openfluid-project.org"},
    {"OpenFLUID community","community.openfluid-project.org"}
  };

  Sign.Issues.add({"Issue X","This is the issue X",{"bug","results"},"Dave",
                   openfluid::core::DateTime(1983,07,22,16,47,11),
                   openfluid::core::DateTime::now()});
  Sign.Issues.add({"Issue Y","This is\nthe issue Y",{"feature","IO"},"Kurt",
                   openfluid::core::DateTime(),
                   openfluid::core::DateTime(),
                   false});

  Sign.HandledData.UsedParams.push_back({"coeff","coefficient",""});
  Sign.HandledData.RequiredParams.push_back({"speedl","speed limit","m/s",openfluid::core::Value::DOUBLE});
  Sign.HandledData.RequiredParams.push_back({"forcefield","field of force","n/m2",openfluid::core::Value::MATRIX});

  Sign.HandledData.UsedExtraFiles = {"observed.csv","simulated.csv","randomized.csv"};
  Sign.HandledData.RequiredExtraFiles = {"forced_data1.csv","forced_data2.csv",};

  Sign.HandledData.UsedAttribute.push_back({"venue_capacity[integer]","YU","the venue capacity",""});
  Sign.HandledData.UsedAttribute.push_back({"venue_volume","YU","the venue volume","m3s"});
  Sign.HandledData.RequiredAttribute.push_back(
    {"stage_area","ZU","the stage area","m2",openfluid::core::Value::DOUBLE});
  Sign.HandledData.ProducedAttribute.push_back(
    {"area_per_musician","ZU","the area for each musician on stage","m2",openfluid::core::Value::DOUBLE});

  Sign.HandledData.ProducedVars.push_back(
    {"venue.band.music.instruments","YU","the music","db",openfluid::core::Value::VECTOR});
  Sign.HandledData.ProducedVars.push_back({"venue.stage.light.system[map]","YU","the light","lm"});
  Sign.HandledData.UpdatedVars.push_back(
    {"venue.temperature.attendance","ZU","the venue temperature","K",openfluid::core::Value::DOUBLE});
  Sign.HandledData.RequiredVars.push_back(
    {"venue.stage.power","ZU","the electric power","W",openfluid::core::Value::INTEGER});
  Sign.HandledData.UsedVars.push_back({"venue.band.mojo","ZU","the mojo",""});

  Sign.HandledData.UsedEventsOnUnits = {"ZU","YU"};

  Sign.TimeScheduling.setAsRange(300,600);

  Sign.HandledUnitsGraph.UpdatedUnitsGraph = "Spatial graph is modified";
  Sign.HandledUnitsGraph.UpdatedUnitsClass = {
    {"XU","XU units are created"},
    {"YU","YU units connections to XU units are updated"},
  };

  return Sign;
}


// =====================================================================
// =====================================================================


void compareSignatures(const openfluid::ware::SimulatorSignature& Sign1,
                       const openfluid::ware::SimulatorSignature& Sign2,
                       const std::string& Context)
{
  std::cout << Context << " : compareSignatures()" << std::endl;

  compareSignaturesBase<openfluid::ware::SimulatorSignature>(Sign1,Sign2);

  // parameters
  BOOST_CHECK(!Sign1.HandledData.UsedParams.empty());
  BOOST_CHECK(!Sign1.HandledData.RequiredParams.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.UsedParams.size(),Sign2.HandledData.UsedParams.size());
  BOOST_CHECK_EQUAL(Sign1.HandledData.RequiredParams.size(),Sign2.HandledData.RequiredParams.size());

  // extra files
  BOOST_CHECK(!Sign1.HandledData.UsedExtraFiles.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.UsedExtraFiles.size(),Sign2.HandledData.UsedExtraFiles.size());
  BOOST_CHECK_EQUAL_COLLECTIONS(Sign1.HandledData.UsedExtraFiles.begin(),Sign1.HandledData.UsedExtraFiles.end(),
                                Sign2.HandledData.UsedExtraFiles.begin(),Sign2.HandledData.UsedExtraFiles.end());
  BOOST_CHECK(!Sign1.HandledData.RequiredExtraFiles.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.RequiredExtraFiles.size(),Sign2.HandledData.RequiredExtraFiles.size());
  BOOST_CHECK_EQUAL_COLLECTIONS(
    Sign1.HandledData.RequiredExtraFiles.begin(),Sign1.HandledData.RequiredExtraFiles.end(),
    Sign2.HandledData.RequiredExtraFiles.begin(),Sign2.HandledData.RequiredExtraFiles.end()
  );

  // attributes
  BOOST_CHECK(!Sign1.HandledData.ProducedAttribute.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.ProducedAttribute.size(),Sign2.HandledData.ProducedAttribute.size());
  BOOST_CHECK(!Sign1.HandledData.RequiredAttribute.empty());  
  BOOST_CHECK_EQUAL(Sign1.HandledData.RequiredAttribute.size(),Sign2.HandledData.RequiredAttribute.size());
  BOOST_CHECK(!Sign1.HandledData.RequiredAttribute.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.RequiredAttribute.size(),Sign2.HandledData.RequiredAttribute.size());

  // variables
  BOOST_CHECK(!Sign1.HandledData.ProducedVars.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.ProducedVars.size(),Sign2.HandledData.ProducedVars.size());
  BOOST_CHECK(!Sign1.HandledData.UsedVars.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.UsedVars.size(),Sign2.HandledData.UsedVars.size());
  BOOST_CHECK(!Sign1.HandledData.RequiredVars.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.RequiredVars.size(),Sign2.HandledData.RequiredVars.size());
  BOOST_CHECK(!Sign1.HandledData.UpdatedVars.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.UpdatedVars.size(),Sign2.HandledData.UpdatedVars.size());

  // events
  BOOST_CHECK_EQUAL(Sign1.HandledData.UsedEventsOnUnits.size(),Sign2.HandledData.UsedEventsOnUnits.size());
  BOOST_CHECK_EQUAL_COLLECTIONS(Sign1.HandledData.UsedEventsOnUnits.begin(),Sign1.HandledData.UsedEventsOnUnits.end(),
                                Sign2.HandledData.UsedEventsOnUnits.begin(),Sign2.HandledData.UsedEventsOnUnits.end());

  // spatial graph
  BOOST_CHECK(!Sign1.HandledUnitsGraph.UpdatedUnitsGraph.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledUnitsGraph.UpdatedUnitsGraph,Sign2.HandledUnitsGraph.UpdatedUnitsGraph);
  BOOST_CHECK(!Sign1.HandledUnitsGraph.UpdatedUnitsClass.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledUnitsGraph.UpdatedUnitsClass.size(),Sign2.HandledUnitsGraph.UpdatedUnitsClass.size());

  // scheduling
  BOOST_CHECK(Sign1.TimeScheduling.Type == Sign2.TimeScheduling.Type);
  BOOST_CHECK_EQUAL(Sign1.TimeScheduling.Min,Sign2.TimeScheduling.Min);
  BOOST_CHECK_EQUAL(Sign1.TimeScheduling.Max,Sign2.TimeScheduling.Max);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(write_json)
{
  auto Sign = getRefSignature();

  openfluid::waresdev::SimulatorSignatureSerializer()
                       .writeToJSONFile(Sign,WorkPath.fromThis("openfluid-ware.json").toGeneric());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(write_cpp)
{
  auto Sign = getRefSignature();

  openfluid::waresdev::SimulatorSignatureSerializer()
                       .writeToWareCPPFile(Sign,WorkPath.fromThis("signature.cpp").toGeneric());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(read_json)
{
  auto SignRef = getRefSignature();
  auto Sign = 
    openfluid::waresdev::SimulatorSignatureSerializer()
                         .readFromJSONFile(WorkPath.fromThis("openfluid-ware.json").toGeneric());

  compareSignatures(Sign,SignRef,"read_json");
}


// =====================================================================
// =====================================================================


bool displayExceptionMsg(const openfluid::base::FrameworkException& E)
{
  std::cout << E.what() << std::endl;
  return true;
} 


BOOST_AUTO_TEST_CASE(read_bad_json)
{
  const auto FilesPath = openfluid::tools::Path({CONFIGTESTS_INPUT_MISCDATA_DIR,"SignatureSerializer"});

  BOOST_CHECK_NO_THROW(
    openfluid::waresdev::SimulatorSignatureSerializer().readFromJSONFile(FilesPath.toGeneric()+"/ref_sim.json")
  );


  const std::vector<std::string> WrongFiles = {
    "bad_format.json",
    "bad_sim_noentry.json",
    "bad_sim_noid.json","bad_sim_emptyid.json",
    "bad_sim_data_noname.json","bad_sim_data_emptyname.json","bad_sim_data_noclass.json"
  };

  for (const auto& F : WrongFiles)
  {
    std::cout << "read_bad_json : " << F << std::endl;
    BOOST_CHECK_EXCEPTION(
      openfluid::waresdev::SimulatorSignatureSerializer().readFromJSONFile(FilesPath.toGeneric()+"/"+F),
      openfluid::base::FrameworkException,displayExceptionMsg
    );
  }
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  WorkPath.makeDirectory();

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}

