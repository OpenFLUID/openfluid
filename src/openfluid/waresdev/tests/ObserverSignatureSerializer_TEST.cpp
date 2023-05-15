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
  @file ObserverSignatureSerializer_TEST.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_observersignatureserializer


#include <boost/test/unit_test.hpp>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/waresdev/ObserverSignatureSerializer.hpp>

#include "SignatureCommon.hpp"

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const auto WorkPath = openfluid::tools::Path({CONFIGTESTS_OUTPUT_DATA_DIR,"SignatureSerializer","observer"});


openfluid::ware::ObserverSignature getRefSignature()
{
  openfluid::ware::ObserverSignature Sign;

  Sign.ID = "unit.test.observer-signature";
  Sign.Name = "Observer signature for unit testing";
  Sign.Description = "This is an observer signature\n"
                     " that should be used for **unit testing only**";
  Sign.Version = "2022.07";

  Sign.Authors = {
    {"Tom Morello","tom@ratm.org"},
    {"Zach de la Rocha","zach@ratm.org"},
    {"Tim Commerford","tim@ratm.org"},
    {"Brad Wilk","brad@ratm.org"}
  };
  Sign.Contacts = {{"OpenFLUID contact","contact@openfluid-project.org"}};
  Sign.License = "MIT";

  Sign.Tags = {"observer","unit testing","development","software quality"}  ;
  Sign.Links = {
    {"OpenFLUID","www.openfluid-project.org"},
    {"OpenFLUID community","community.openfluid-project.org"}
  };

  return Sign;
}


// =====================================================================
// =====================================================================


void compareSignatures(const openfluid::ware::ObserverSignature& Sign1,
                       const openfluid::ware::ObserverSignature& Sign2,
                       const std::string& Context)
{
  std::cout << "compareSignatures() : " << Context << std::endl;

  compareSignaturesBase<openfluid::ware::ObserverSignature>(Sign1,Sign2);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(write_json)
{
  auto Sign = getRefSignature();

  openfluid::waresdev::ObserverSignatureSerializer()
                       .writeToJSONFile(Sign,WorkPath.fromThis("openfluid-ware.json").toGeneric());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(write_cpp)
{
  auto Sign = getRefSignature();

  openfluid::waresdev::ObserverSignatureSerializer()
                       .writeToWareCPPFile(Sign,WorkPath.fromThis("signature.cpp").toGeneric());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(read_json)
{
  auto SignRef = getRefSignature();
  auto Sign = 
    openfluid::waresdev::ObserverSignatureSerializer()
                         .readFromJSONFile(WorkPath.fromThis("openfluid-ware.json").toGeneric());

  compareSignatures(Sign,SignRef,"read_json");
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  WorkPath.makeDirectory();

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}

