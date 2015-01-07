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
 @file SimulatorSignatureRegistry_TEST.cpp

 @author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_SimulatorSignatureRegistry
#include <boost/test/unit_test.hpp>

#include "tests-config.hpp"
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_constructor)
{
  openfluid::machine::SimulatorSignatureRegistry* Signatures =
      openfluid::machine::SimulatorSignatureRegistry::instance();

  BOOST_CHECK_EQUAL(
      Signatures->getSimSignatures()[openfluid::fluidx::ModelItemDescriptor::PluggedSimulator].size(),
      7);
  BOOST_CHECK_EQUAL(
      Signatures->getSimSignatures()[openfluid::fluidx::ModelItemDescriptor::Generator].size(),
      4);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_getSignatureItemInstance)
{
  openfluid::machine::SimulatorSignatureRegistry* Reg =
      openfluid::machine::SimulatorSignatureRegistry::instance();

  openfluid::machine::ModelItemSignatureInstance* Sign =
      Reg->signatureItemInstance("examples.primitives.unitsA.prod");

  BOOST_CHECK_EQUAL(Sign->Signature->ID, "examples.primitives.unitsA.prod");

  openfluid::fluidx::SimulatorDescriptor ItemDesc(
      "examples.primitives.unitsB.prod");

  openfluid::machine::ModelItemSignatureInstance* Sign2 =
      Reg->signatureItemInstance(&ItemDesc);

  BOOST_CHECK_EQUAL(Sign2->Signature->ID, "examples.primitives.unitsB.prod");
}

// =====================================================================
// =====================================================================
