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
  @file BuilderextSignatureSerializer_TEST.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_builderextsignatureserializer


#include <boost/test/unit_test.hpp>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/waresdev/BuilderextSignatureSerializer.hpp>

#include "SignatureCommon.hpp"

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const auto WorkPath = openfluid::tools::Path({CONFIGTESTS_OUTPUT_DATA_DIR,"SignatureSerializer","builderext"});


openfluid::builderext::BuilderExtensionSignature getRefSignature()
{
  openfluid::builderext::BuilderExtensionSignature Sign;

  Sign.ID = "unit.test.builderext-signature";
  Sign.Name = "Builderext signature for unit testing";
  Sign.Description = "This is a Builder-Extension signature\n"
                     " that should be used for **unit testing only**";
  Sign.Version = "2022.07";
  Sign.Status = openfluid::ware::BETA;

  Sign.Authors = {
    {"Meg White","meg@whitestripes.org"},
    {"Jack White","jack@whitestripes.org"}
  };
  Sign.Contacts = {{"OpenFLUID contact","contact@openfluid-project.org"}};
  Sign.License = "WTFPL";

  Sign.Tags = {"builderext","unit testing","development","software quality"}  ;
  Sign.Links = {
    {"OpenFLUID","www.openfluid-project.org"},
    {"OpenFLUID community","community.openfluid-project.org"}
  };

  Sign.Issues.add({"Issue Z","This is the issue Z",{"bug","UI"},"Jack",
                   openfluid::core::DateTime(1985,10,21,19,47,11),
                   openfluid::core::DateTime::now(),true});

  Sign.MenuText = "Music band";

  Sign.Category = openfluid::builderext::ExtensionCategory::SPATIAL;
  Sign.Mode = openfluid::builderext::ExtensionMode::WORKSPACE;

  return Sign;
}


// =====================================================================
// =====================================================================


void compareSignatures(const openfluid::builderext::BuilderExtensionSignature& Sign1,
                       const openfluid::builderext::BuilderExtensionSignature& Sign2,
                       const std::string& Context)
{
  std::cout << "compareSignatures() : " << Context << std::endl;

  compareSignaturesBase<openfluid::builderext::BuilderExtensionSignature>(Sign1,Sign2);

  BOOST_CHECK(Sign1.Category == Sign2.Category);
  BOOST_CHECK(Sign1.Mode == Sign2.Mode);
  BOOST_CHECK(Sign1.Role == Sign2.Role);

  BOOST_CHECK_EQUAL(Sign1.MenuText,Sign2.MenuText);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(write_json)
{
  auto Sign = getRefSignature();

  openfluid::waresdev::BuilderextSignatureSerializer().writeToJSONFile(Sign,WorkPath.toGeneric()+"/wareinfo.json");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(write_cpp)
{
  auto Sign = getRefSignature();

  openfluid::waresdev::BuilderextSignatureSerializer().writeToWareCPPFile(Sign,WorkPath.toGeneric()+"/signature.cpp");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(read_json)
{
  auto SignRef = getRefSignature();
  auto Sign = 
    openfluid::waresdev::BuilderextSignatureSerializer().readFromJSONFile(WorkPath.toGeneric()+"/wareinfo.json");

  compareSignatures(Sign,SignRef,"read_json");
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  WorkPath.makeDirectory();

  return ::boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}

