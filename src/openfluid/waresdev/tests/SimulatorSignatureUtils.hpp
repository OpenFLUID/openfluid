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
  @file SimulatorSignatureUtils.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel Thöni <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEVTESTS_SIMULATORSIGNATUREUTILS_HPP__
#define __OPENFLUID_WARESDEVTESTS_SIMULATORSIGNATUREUTILS_HPP__


#include <openfluid/waresdev/SimulatorSignatureSerializer.hpp>
#include "SignatureCommon.hpp"


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

  Sign.Issues.add({101, "Issue X","This is the issue X",{"bug","results"},"Dave",
                   openfluid::core::DateTime(1983,07,22,16,47,11),
                   openfluid::core::DateTime(2024,02,29,12,16,52)});
  Sign.Issues.add({102, "Issue Y","This is\nthe issue Y",{"feature","IO"},"Kurt",
                   openfluid::core::DateTime(),
                   openfluid::core::DateTime(),
                   false});

  Sign.Dependencies["otherLib"] = ">=1.0";

  Sign.HandledData.UsedParams.push_back({"coeff","coefficient",""});
  Sign.HandledData.RequiredParams.push_back({"speedl","speed limit","m/s",openfluid::core::Value::DOUBLE});
  Sign.HandledData.RequiredParams.push_back({"forcefield","field of force","n/m2",openfluid::core::Value::MATRIX});

  Sign.HandledData.UsedExtraFiles = {"observed.csv","simulated.csv","randomized.csv"};
  Sign.HandledData.RequiredExtraFiles = {"forced_data1.csv","forced_data2.csv",};

  Sign.HandledData.UsedAttribute.push_back({"venue_capacity[integer]","YU","the venue capacity",""});
  Sign.HandledData.UsedAttribute.push_back({"venue_volume","YU","the venue volume","m3s"});
  Sign.HandledData.RequiredAttribute.push_back(
    {"stage_area","ZU","the stage area","m2",openfluid::core::Value::DOUBLE});
  Sign.SimulatorHandledData.ProducedAttribute.push_back(
    {"area_per_musician","ZU","the area for each musician on stage","m2",openfluid::core::Value::DOUBLE});

  Sign.SimulatorHandledData.ProducedVars.push_back(
    {"venue.band.music.instruments","YU","the music","db",openfluid::core::Value::VECTOR});
  Sign.SimulatorHandledData.ProducedVars.push_back({"venue.stage.light.system[map]","YU","the light","lm"});
  Sign.SimulatorHandledData.UpdatedVars.push_back(
    {"venue.temperature.attendance","ZU","the venue temperature","K",openfluid::core::Value::DOUBLE});
  Sign.HandledData.RequiredVars.push_back(
    {"venue.stage.power","ZU","the electric power","W",openfluid::core::Value::INTEGER});
  Sign.HandledData.UsedVars.push_back({"venue.band.mojo","ZU","the mojo",""});

  Sign.SimulatorHandledData.UsedEventsOnUnits = {"ZU","YU"};

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
  BOOST_CHECK_EQUAL_COLLECTIONS(Sign1.HandledData.UsedExtraFiles.begin(),
                                Sign1.HandledData.UsedExtraFiles.end(),
                                Sign2.HandledData.UsedExtraFiles.begin(),
                                Sign2.HandledData.UsedExtraFiles.end());
  BOOST_CHECK(!Sign1.HandledData.RequiredExtraFiles.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.RequiredExtraFiles.size(),
                    Sign2.HandledData.RequiredExtraFiles.size());
  BOOST_CHECK_EQUAL_COLLECTIONS(
    Sign1.HandledData.RequiredExtraFiles.begin(),Sign1.HandledData.RequiredExtraFiles.end(),
    Sign2.HandledData.RequiredExtraFiles.begin(),Sign2.HandledData.RequiredExtraFiles.end()
  );

  // attributes
  BOOST_CHECK(!Sign1.SimulatorHandledData.ProducedAttribute.empty());
  BOOST_CHECK_EQUAL(Sign1.SimulatorHandledData.ProducedAttribute.size(),
                    Sign2.SimulatorHandledData.ProducedAttribute.size());
  BOOST_CHECK(!Sign1.HandledData.RequiredAttribute.empty());  
  BOOST_CHECK_EQUAL(Sign1.HandledData.RequiredAttribute.size(),
                    Sign2.HandledData.RequiredAttribute.size());
  BOOST_CHECK(!Sign1.HandledData.RequiredAttribute.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.RequiredAttribute.size(),
                    Sign2.HandledData.RequiredAttribute.size());

  // variables
  BOOST_CHECK(!Sign1.SimulatorHandledData.ProducedVars.empty());
  BOOST_CHECK_EQUAL(Sign1.SimulatorHandledData.ProducedVars.size(),Sign2.SimulatorHandledData.ProducedVars.size());
  BOOST_CHECK(!Sign1.HandledData.UsedVars.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.UsedVars.size(),Sign2.HandledData.UsedVars.size());
  BOOST_CHECK(!Sign1.HandledData.RequiredVars.empty());
  BOOST_CHECK_EQUAL(Sign1.HandledData.RequiredVars.size(),Sign2.HandledData.RequiredVars.size());
  BOOST_CHECK(!Sign1.SimulatorHandledData.UpdatedVars.empty());
  BOOST_CHECK_EQUAL(Sign1.SimulatorHandledData.UpdatedVars.size(),Sign2.SimulatorHandledData.UpdatedVars.size());

  // events
  BOOST_CHECK_EQUAL(Sign1.SimulatorHandledData.UsedEventsOnUnits.size(),
                    Sign2.SimulatorHandledData.UsedEventsOnUnits.size());
  BOOST_CHECK_EQUAL_COLLECTIONS(Sign1.SimulatorHandledData.UsedEventsOnUnits.begin(),
                                Sign1.SimulatorHandledData.UsedEventsOnUnits.end(),
                                Sign2.SimulatorHandledData.UsedEventsOnUnits.begin(),
                                Sign2.SimulatorHandledData.UsedEventsOnUnits.end());

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


void compareDataItemsJSON(const openfluid::thirdparty::json& DataItem1,
                          const openfluid::thirdparty::json& DataItem2)
{
  BOOST_CHECK_EQUAL(DataItem1.value("name", ""), DataItem2.value("name", ""));
  BOOST_CHECK_EQUAL(DataItem1.value("description", ""), DataItem2.value("description", ""));
  BOOST_CHECK_EQUAL(DataItem1.value("siunit", ""), DataItem2.value("siunit", ""));
  BOOST_CHECK_EQUAL(DataItem1.value("type", ""), DataItem2.value("type", ""));
  BOOST_CHECK_EQUAL(DataItem1.value("unitsclass", ""), DataItem2.value("unitsclass", ""));
}


// =====================================================================
// =====================================================================


void compareIOConditionJSON(const openfluid::thirdparty::json& Object1,
                            const openfluid::thirdparty::json& Object2,
                            const std::string& ConditionKey)
{
  openfluid::thirdparty::json ConditionObject1 = Object1.value(ConditionKey, openfluid::thirdparty::json::array());
  openfluid::thirdparty::json ConditionObject2 = Object2.value(ConditionKey, openfluid::thirdparty::json::array());
  checkArraysJSON    (ConditionObject1, ConditionObject2);
  for (unsigned int i=0; i < ConditionObject1.size(); i++)
  {
    compareDataItemsJSON(ConditionObject1[i], ConditionObject2[i]);
  }
}


// =====================================================================
// =====================================================================


void compareJSON(const openfluid::thirdparty::json& Json1,
                 const openfluid::thirdparty::json& Json2,
                 const std::string& Context) 
{
  std::cout << Context << " : compareJSON()" << std::endl;

  compareJSONBase(Json1, Json2);

  openfluid::thirdparty::json Simulator1 = Json1.value("simulator",openfluid::thirdparty::json::object());
  openfluid::thirdparty::json Simulator2 = Json2.value("simulator",openfluid::thirdparty::json::object());
  BOOST_CHECK(Simulator1.is_object());
  BOOST_CHECK(Simulator2.is_object());

  openfluid::thirdparty::json Data1 = Simulator1.value("data",openfluid::thirdparty::json::object());
  openfluid::thirdparty::json Data2 = Simulator2.value("data",openfluid::thirdparty::json::object());

  // Parameters
  openfluid::thirdparty::json Params1 = Data1.value("parameters",openfluid::thirdparty::json::object());
  openfluid::thirdparty::json Params2 = Data2.value("parameters",openfluid::thirdparty::json::object());
  compareIOConditionJSON(Params1, Params2, "required");
  compareIOConditionJSON(Params1, Params2, "used");

  // Attributes
  openfluid::thirdparty::json Attributes1 = Data1.value("attributes",openfluid::thirdparty::json::object());
  openfluid::thirdparty::json Attributes2 = Data2.value("attributes",openfluid::thirdparty::json::object());
  compareIOConditionJSON(Attributes1, Attributes2, "required");
  compareIOConditionJSON(Attributes1, Attributes2, "used");
  compareIOConditionJSON(Attributes1, Attributes2, "produced");

  // Variables
  openfluid::thirdparty::json Variables1 = Data1.value("variables",openfluid::thirdparty::json::object());
  openfluid::thirdparty::json Variables2 = Data2.value("variables",openfluid::thirdparty::json::object());
  compareIOConditionJSON(Variables1, Variables2, "required");
  compareIOConditionJSON(Variables1, Variables2, "used");
  compareIOConditionJSON(Variables1, Variables2, "produced");
  compareIOConditionJSON(Variables1, Variables2, "updated");

  // Events
  openfluid::thirdparty::json Events1 = Data1.value("events",openfluid::thirdparty::json::array());
  openfluid::thirdparty::json Events2 = Data2.value("events",openfluid::thirdparty::json::array());
  checkArraysJSON    (Events1, Events2);
  for (unsigned int i=0; i < Events1.size(); i++)
  {
    BOOST_CHECK_EQUAL(Events1[i], Events2[i]);
  }

  // Extra files
  openfluid::thirdparty::json ExtraFiles1 = Data1.value("extrafiles",openfluid::thirdparty::json::object());
  openfluid::thirdparty::json ExtraFiles2 = Data2.value("extrafiles",openfluid::thirdparty::json::object());
  openfluid::thirdparty::json RequiredEF1 = ExtraFiles1.value("required",openfluid::thirdparty::json::array());
  openfluid::thirdparty::json RequiredEF2 = ExtraFiles2.value("required",openfluid::thirdparty::json::array());
  checkArraysJSON    (RequiredEF1, RequiredEF2);
  for (unsigned int i=0; i < RequiredEF1.size(); i++)
  {
    BOOST_CHECK_EQUAL(RequiredEF1[i], RequiredEF2[i]);
  }
  openfluid::thirdparty::json UsedEF1 = ExtraFiles1.value("used",openfluid::thirdparty::json::array());
  openfluid::thirdparty::json UsedEF2 = ExtraFiles2.value("used",openfluid::thirdparty::json::array());
  checkArraysJSON    (UsedEF1, UsedEF2);
  for (unsigned int i=0; i < UsedEF1.size(); i++)
  {
    BOOST_CHECK_EQUAL(UsedEF1[i], UsedEF2[i]);
  }

  // Spatial graph
  openfluid::thirdparty::json SpatialGraph1 = Simulator1.value("spatial_graph",openfluid::thirdparty::json::object());
  openfluid::thirdparty::json SpatialGraph2 = Simulator2.value("spatial_graph",openfluid::thirdparty::json::object());
  BOOST_CHECK_EQUAL(SpatialGraph1.value("description", ""), SpatialGraph2.value("description", ""));
  openfluid::thirdparty::json DetailsSG1 = SpatialGraph1.value("details", openfluid::thirdparty::json::array());
  openfluid::thirdparty::json DetailsSG2 = SpatialGraph2.value("details", openfluid::thirdparty::json::array());
  checkArraysJSON    (DetailsSG1, DetailsSG2);
  for (unsigned int i=0; i < DetailsSG1.size(); i++)
  {
    BOOST_CHECK_EQUAL(DetailsSG1[i].value("unitsclass", ""), DetailsSG2[i].value("unitsclass", ""));
    BOOST_CHECK_EQUAL(DetailsSG1[i].value("description", ""), DetailsSG2[i].value("description", ""));
  }

  // Scheduling
  openfluid::thirdparty::json Scheduling1 = Simulator1.value("scheduling",openfluid::thirdparty::json::object());
  openfluid::thirdparty::json Scheduling2 = Simulator2.value("scheduling",openfluid::thirdparty::json::object());
  BOOST_CHECK_EQUAL(Scheduling1.value("type", ""), Scheduling2.value("type", ""));
  BOOST_CHECK_EQUAL(Scheduling1.value("min", 0), Scheduling2.value("min", 0));
  BOOST_CHECK_EQUAL(Scheduling1.value("max", 0), Scheduling2.value("max", 0));
}

#endif /* __OPENFLUID_WARESDEVTESTS_SIMULATORSIGNATUREUTILS_HPP__ */
