/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file AdvancedMonitoringDescriptor_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_AdvancedMonitoringDescriptor
#include <boost/test/unit_test.hpp>

#include <openfluid/fluidx/AdvancedMonitoringDescriptor.hpp>
#include "tests-config.hpp"
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/ObserverSignatureRegistry.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedMonitoringDescriptor Monit(
      FXDesc.getMonitoringDescriptor());

  BOOST_CHECK_EQUAL(Monit.getItems().size(), 2);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_duplicates)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/duplicates");

  BOOST_CHECK_THROW(
      openfluid::fluidx::AdvancedMonitoringDescriptor(FXDesc.getMonitoringDescriptor()),
      openfluid::base::OFException);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedMonitoringDescriptor Monit(
      FXDesc.getMonitoringDescriptor());

  BOOST_CHECK_EQUAL(Monit.getItems().size(), 2);
  BOOST_CHECK_EQUAL(Monit.getDescriptor("export.vars.files.csv").getID(),
                    "export.vars.files.csv");
  BOOST_CHECK_EQUAL(
      Monit.getDescriptor("export.spatial-graph.files.dot").getID(),
      "export.spatial-graph.files.dot");

  BOOST_CHECK_THROW(Monit.removeFromObserverList("dummy"),
                    openfluid::base::OFException);

  Monit.removeFromObserverList("export.vars.files.csv");

  BOOST_CHECK_EQUAL(Monit.getItems().size(), 1);
  BOOST_CHECK_THROW(Monit.getDescriptor("export.vars.files.csv"),
                    openfluid::base::OFException);
  BOOST_CHECK_EQUAL(
      Monit.getDescriptor("export.spatial-graph.files.dot").getID(),
      "export.spatial-graph.files.dot");

  Monit.addToObserverList("export.vars.files.csv");
  BOOST_CHECK_EQUAL(Monit.getItems().size(), 2);
  BOOST_CHECK_EQUAL(Monit.getDescriptor("export.vars.files.csv").getID(),
                    "export.vars.files.csv");
  BOOST_CHECK_EQUAL(
      Monit.getDescriptor("export.spatial-graph.files.dot").getID(),
      "export.spatial-graph.files.dot");

  Monit.addToObserverList("export.vars.files.vtk");

  const std::list<openfluid::fluidx::ObserverDescriptor*>& Items =
      Monit.getItems();
  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator it =
      Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");

  Monit.moveItemTowardsTheBeginning("export.vars.files.vtk");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  Monit.moveItemTowardsTheBeginning("export.vars.files.vtk");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  // does nothing
  Monit.moveItemTowardsTheBeginning("export.vars.files.vtk");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  // does nothing
  Monit.moveItemTowardsTheBeginning("wrong.ID");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  Monit.moveItemTowardsTheEnd("export.vars.files.vtk");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  Monit.moveItemTowardsTheEnd("export.vars.files.vtk");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");

  //does nothing
  Monit.moveItemTowardsTheEnd("export.vars.files.vtk");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");

  //does nothing
  Monit.moveItemTowardsTheEnd("wrong.ID");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");

  Monit.removeFromObserverList("export.spatial-graph.files.dot");
  Monit.removeFromObserverList("export.vars.files.csv");

  // does nothing
  Monit.moveItemTowardsTheBeginning("export.vars.files.vtk");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.vars.files.vtk");

  // does nothing
  Monit.moveItemTowardsTheEnd("export.vars.files.vtk");
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.vars.files.vtk");
}

// =====================================================================
// =====================================================================
