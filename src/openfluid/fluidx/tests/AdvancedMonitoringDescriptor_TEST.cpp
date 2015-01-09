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
 @file AdvancedMonitoringDescriptor_TEST.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
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
      FXDesc.monitoringDescriptor());

  BOOST_CHECK_EQUAL(Monit.items().size(), 2);
}


// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_duplicates)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/duplicates");

  BOOST_CHECK_THROW(
      openfluid::fluidx::AdvancedMonitoringDescriptor(FXDesc.monitoringDescriptor()),
      openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedMonitoringDescriptor Monit(
      FXDesc.monitoringDescriptor());

  BOOST_CHECK_EQUAL(Monit.items().size(), 2);

  BOOST_CHECK_THROW(Monit.removeItem(6),
                    openfluid::base::FrameworkException);

  Monit.removeItem(0);

  BOOST_CHECK_EQUAL(Monit.items().size(), 1);
  BOOST_CHECK_THROW(Monit.itemAt(6),
                    openfluid::base::FrameworkException);

  openfluid::fluidx::ObserverDescriptor CSVObsDesc("export.vars.files.csv");
  Monit.appendItem(&CSVObsDesc);

  BOOST_CHECK_EQUAL(Monit.items().size(), 2);
  BOOST_CHECK_EQUAL(Monit.itemAt(1).getID(),"export.vars.files.csv");
  BOOST_CHECK_EQUAL(Monit.itemAt(0).getID(),"export.spatial-graph.files.dot");


  openfluid::fluidx::ObserverDescriptor VTKObsDesc("export.vars.files.vtk");
  Monit.appendItem(&VTKObsDesc);

  const std::list<openfluid::fluidx::ObserverDescriptor*>& Items =
      Monit.items();
  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator it =
      Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");

  Monit.moveItem(2,1);
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  Monit.moveItem(1,0);
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  // does nothing
  Monit.moveItem(0,0);
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  // does nothing
  Monit.moveItem(1,1);
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  Monit.moveItem(0,1);
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");

  Monit.moveItem(1,2);
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");

  //does nothing
  Monit.moveItem(2,2);
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.csv");
  BOOST_CHECK_EQUAL((*++it)->getID(), "export.vars.files.vtk");


  Monit.removeItem(0);
  Monit.removeItem(0);

  // does nothing
  Monit.moveItem(0,0);
  it = Items.begin();

  BOOST_CHECK_EQUAL((*it)->getID(), "export.vars.files.vtk");

}

// =====================================================================
// =====================================================================
