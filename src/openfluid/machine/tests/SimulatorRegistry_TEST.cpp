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
  @file SimulatorRegistry_TEST.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_simulatorregistry


#include <boost/test/unit_test.hpp>

#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_constructor)
{
  auto Reg = openfluid::machine::SimulatorRegistry::instance();

  Reg->discoverWares();

  BOOST_CHECK_EQUAL(Reg->availableWares().size(),11);
  
  for (const auto& C : Reg->availableWares())
  {
    BOOST_CHECK_EQUAL(C.first, C.second.signature()->ID);
  }

  BOOST_CHECK(Reg->generators().empty());

  Reg->clearWares();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_signature)
{
  auto Reg = openfluid::machine::SimulatorRegistry::instance();
  Reg->discoverWares();

  const auto& Cont = Reg->wareContainer("examples.primitives.unitsA.prod");

  BOOST_CHECK_EQUAL(Cont.signature()->ID, "examples.primitives.unitsA.prod");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_ghostsimulators)
{
  openfluid::base::Environment::addExtraSimulatorsDirs(CONFIGTESTS_INPUT_MISCDATA_DIR+"/"+"GhostSimulators");

  auto Reg = openfluid::machine::SimulatorRegistry::instance();
  Reg->discoverWares();

  BOOST_CHECK_EQUAL(Reg->availableWares().size(),16);

  BOOST_CHECK(Reg->wareContainer("simA").isValid());
  BOOST_CHECK(Reg->wareContainer("simA").isGhost());
  BOOST_CHECK(Reg->wareContainer("simA").hasSignature());
  BOOST_CHECK(!Reg->wareContainer("simFake").isValid());
  BOOST_CHECK(!Reg->wareContainer("simFake").isGhost());
  BOOST_CHECK(!Reg->wareContainer("simFake").hasSignature());
}
