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
  @file ExecutionTimePoint_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_executiontimepoint
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/machine/ExecutionTimePoint.hpp>
#include <openfluid/base/SimulationStatus.hpp>


// =====================================================================
// =====================================================================


class SimA : openfluid::ware::PluggableSimulator
{
  public:

    SimA() : openfluid::ware::PluggableSimulator()
    { };

    ~SimA()
    { };

    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    { }

    void prepareData()
    { }

    void checkConsistency()
    { }

    openfluid::base::SchedulingRequest initializeRun()
    { return DefaultDeltaT(); }

    openfluid::base::SchedulingRequest runStep()
    { std::cout << "sim.a" << std::endl; return DefaultDeltaT(); }

    void finalizeRun()
    { }

};


class SimB : openfluid::ware::PluggableSimulator
{
  public:

    SimB() : openfluid::ware::PluggableSimulator() {};

    ~SimB() {};

    void initParams(const openfluid::ware::WareParams_t& /*Params*/)  { }

    void prepareData() { }

    void checkConsistency() { }

    openfluid::base::SchedulingRequest initializeRun() { return DefaultDeltaT(); }

    openfluid::base::SchedulingRequest runStep() { std::cout << "sim.b" << std::endl; return DefaultDeltaT(); }

    void finalizeRun() { }

};


class SimC : openfluid::ware::PluggableSimulator
{
  public:

    SimC() : openfluid::ware::PluggableSimulator() {};

    ~SimC() {};

    void initParams(const openfluid::ware::WareParams_t& /*Params*/)  { }

    void prepareData() { }

    void checkConsistency() { }

    openfluid::base::SchedulingRequest initializeRun() { return DefaultDeltaT(); }

    openfluid::base::SchedulingRequest runStep() { std::cout << "sim.c" << std::endl; return DefaultDeltaT(); }

    void finalizeRun() { }

};


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2012,1,1,0,0,0),
                                              openfluid::core::DateTime(2012,1,15,14,46,39),60);
  openfluid::machine::ExecutionTimePoint TP(17);

  BOOST_REQUIRE_EQUAL(TP.getTimeIndex(),17);
  BOOST_REQUIRE(!TP.hasItemsToProcess());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
	openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2012,1,1,0,0,0),
	                                            openfluid::core::DateTime(2012,1,15,14,46,39),60);
	openfluid::machine::ExecutionTimePoint TP(17);

	BOOST_REQUIRE_EQUAL(TP.getTimeIndex(),17);

	openfluid::machine::ModelItemInstance* MII;

  MII = new openfluid::machine::ModelItemInstance();
  MII->Body.reset((openfluid::ware::PluggableSimulator*)(new SimA()));
  MII->Body->linkToSimulation(&SimStatus);
  MII->Signature = new openfluid::ware::SimulatorSignature();
  MII->Signature->ID = "sim.a";
  MII->OriginalPosition = 1;
  TP.appendItem(MII);

  MII = new openfluid::machine::ModelItemInstance();
  MII->Body.reset((openfluid::ware::PluggableSimulator*)(new SimC()));
  MII->Body->linkToSimulation(&SimStatus);
  MII->Signature = new openfluid::ware::SimulatorSignature();
  MII->Signature->ID = "sim.c";
  MII->OriginalPosition = 3;
  TP.appendItem(MII);

  MII = new openfluid::machine::ModelItemInstance();
  MII->Body.reset((openfluid::ware::PluggableSimulator*)(new SimB()));
  MII->Body->linkToSimulation(&SimStatus);
  MII->Signature = new openfluid::ware::SimulatorSignature();
  MII->Signature->ID = "sim.b";
  MII->OriginalPosition = 2;
  TP.appendItem(MII);



	BOOST_REQUIRE(TP.hasItemsToProcess());

	TP.sortByOriginalPosition();

	while (TP.hasItemsToProcess()) TP.processNextItem();

  BOOST_REQUIRE(!TP.hasItemsToProcess());

}

// =====================================================================
// =====================================================================
