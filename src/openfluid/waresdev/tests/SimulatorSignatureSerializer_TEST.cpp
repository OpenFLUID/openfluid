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
#include "SimulatorSignatureUtils.hpp"
#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const auto WorkPath = openfluid::tools::Path({CONFIGTESTS_OUTPUT_DATA_DIR,"SignatureSerializer","simulator"});


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

