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
 \file DomainReader.cpp
 \brief Implements ...

 \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_fluidxdescriptor
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/results_collector.hpp>
#if BOOST_WORKAROUND(  __GNUC__, < 3 )
#include <boost/test/output_test_stream.hpp>
typedef boost::test_tools::output_test_stream onullstream_type;
#else
#include <boost/test/utils/nullstream.hpp>
typedef boost::onullstream onullstream_type;
#endif

#include <tests-config.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/fluidx/FunctionDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/WareSetDescriptor.hpp>
#include <boost/foreach.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(new openfluid::base::IOListener());
}

// =====================================================================
// =====================================================================

void TestDataset(std::string DatasetPath)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(new openfluid::base::IOListener());

  boost::onullstream ONullStream;
  FXDesc.loadFromDirectory(DatasetPath);

  // Model
  // ====================================================================

  BOOST_REQUIRE_EQUAL(FXDesc.getModelDescriptor().getGlobalParameters().size(), 2);
  BOOST_REQUIRE_EQUAL(
      FXDesc.getModelDescriptor().getGlobalParameters().get<std::string>("gparam1"),
      "100");
  BOOST_REQUIRE_EQUAL(
      FXDesc.getModelDescriptor().getGlobalParameters().get<std::string>("gparam2"),
      "0.1");

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t ModelItems;

  ModelItems = FXDesc.getModelDescriptor().getItems();

  BOOST_REQUIRE_EQUAL(ModelItems.size(), 5);

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t::iterator it;

  it = ModelItems.begin();
  BOOST_REQUIRE_EQUAL(
      (*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator), true);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),
      "tests.generator.interp");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(), 1);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitClass(), "TU");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Interp);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),
      2);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().get<std::string>("sources"),
      "sources.xml");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().get<std::string>("distribution"),
      "distri.dat");

  it++;
  BOOST_REQUIRE_EQUAL(
      (*it)->isType(openfluid::fluidx::ModelItemDescriptor::PluggedFunction),
      true);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::FunctionDescriptor*)(*it))->getFileID(),
      "tests.functionA");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::FunctionDescriptor*)(*it))->getParameters().size(),
      0);

  it++;
  BOOST_REQUIRE_EQUAL(
      (*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator), true);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),
      "tests.generator.fixed");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(), 11);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitClass(), "TU");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Fixed);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),
      1);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().get<std::string>("fixedvalue"),
      "20");

  it++;
  BOOST_REQUIRE_EQUAL(
      (*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator), true);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),
      "tests.generator.random");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(), 1);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitClass(), "TU");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod(),
      openfluid::fluidx::GeneratorDescriptor::Random);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),
      2);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().get<std::string>("min"),
      "20.53");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().get<std::string>("max"),
      "50");

  it++;
  BOOST_REQUIRE_EQUAL(
      (*it)->isType(openfluid::fluidx::ModelItemDescriptor::PluggedFunction),
      true);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::FunctionDescriptor*)(*it))->getFileID(),
      "tests.functionB");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::FunctionDescriptor*)(*it))->getParameters().size(),
      4);
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().get<std::string>("gparam1"),
      "50");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().get<std::string>("strparam"),
      "strvalue");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().get<std::string>("doubleparam"),
      "1.1");
  BOOST_REQUIRE_EQUAL(
      ((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().get<std::string>("longparam"),
      "11");


  // Run
  // ====================================================================

  BOOST_REQUIRE_EQUAL(FXDesc.getRunDescriptor().getDeltaT(), 4753);
  BOOST_REQUIRE(
      FXDesc.getRunDescriptor().getBeginDate() == openfluid::core::DateTime(1997,1,2,11,15,48));
  BOOST_REQUIRE(
      FXDesc.getRunDescriptor().getEndDate() == openfluid::core::DateTime(2005,11,30,6,53,7));
  BOOST_REQUIRE_EQUAL(FXDesc.getRunDescriptor().isSimulationID(), true);
  BOOST_REQUIRE_EQUAL(FXDesc.getRunDescriptor().getSimulationID(), "testrun");
  BOOST_REQUIRE_EQUAL(FXDesc.getRunDescriptor().isUserValuesBufferSize(), true);
  BOOST_REQUIRE_EQUAL(FXDesc.getRunDescriptor().getValuesBufferSize(), 100);
  BOOST_REQUIRE_EQUAL(FXDesc.getRunDescriptor().getFilesBufferSizeInKB(), 2);


  // Domain definition
  // ====================================================================

  std::list<openfluid::fluidx::UnitDescriptor>::iterator UnitsIt;

  BOOST_REQUIRE_EQUAL(FXDesc.getDomainDescriptor().getUnits().size(), 14);

  UnitsIt = FXDesc.getDomainDescriptor().getUnits().begin();
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(), "unitsP");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(), 0);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(), 0);

  UnitsIt++;
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(), "unitsA");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(), 3);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().first, "unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().second, 11);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().front().first, "unitsP");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().front().second, 1);

  UnitsIt++;
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(), "unitsA");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().first, "unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().second, 2);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(), 0);

  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(), "unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(), 11);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().first, "unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().second, 3);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(), 1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().front().first, "unitsP");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().front().second, 1);

  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(), "unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(), 7);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(), 4);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(), 0);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(), 0);


  // Domain input data
  // ====================================================================

  std::list<openfluid::fluidx::InputDataDescriptor>::iterator IDataIt;

  BOOST_REQUIRE_EQUAL(FXDesc.getDomainDescriptor().getInputData().size(), 3);

  IDataIt = FXDesc.getDomainDescriptor().getInputData().begin();
  BOOST_REQUIRE_EQUAL((*IDataIt).getUnitsClass(), "unitsA");
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder().size(), 1);
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder()[0], "indataA");
  BOOST_REQUIRE((*IDataIt).getData().size() > 0);
  BOOST_REQUIRE_EQUAL((*IDataIt).getData().at(8).at("indataA"), "1.1");

  IDataIt++;
  BOOST_REQUIRE_EQUAL((*IDataIt).getUnitsClass(), "unitsB");
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder().size(), 2);
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder()[0], "indataB1");
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder()[1], "indataB3");
  BOOST_REQUIRE((*IDataIt).getData().size() > 0);
  BOOST_REQUIRE_EQUAL((*IDataIt).getData().at(7).at("indataB1"), "7.1");
  BOOST_REQUIRE_EQUAL((*IDataIt).getData().at(7).at("indataB3"), "7.3");
  BOOST_REQUIRE_EQUAL((*IDataIt).getData().at(11).at("indataB1"), "11.1");

  IDataIt++;
  BOOST_REQUIRE_EQUAL((*IDataIt).getUnitsClass(), "unitsB");
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder().size(), 1);
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder()[0], "indataB2");
  BOOST_REQUIRE((*IDataIt).getData().size() > 0);
  BOOST_REQUIRE_EQUAL((*IDataIt).getData().at(11).at("indataB2"), "codeA");
  BOOST_REQUIRE_EQUAL((*IDataIt).getData().at(7).at("indataB2"), "codeE");
  BOOST_REQUIRE_EQUAL((*IDataIt).getData().at(2).at("indataB2"), "codeC");
  BOOST_REQUIRE_EQUAL((*IDataIt).getData().at(1).at("indataB2"), "codeD");


  // Domain calendar
  // ====================================================================

  std::list<openfluid::fluidx::EventDescriptor>::iterator EventIt;

  BOOST_REQUIRE_EQUAL(FXDesc.getDomainDescriptor().getEvents().size(), 10);

  EventIt = FXDesc.getDomainDescriptor().getEvents().begin();
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitClass(), "unitsA");
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitID(), 1);
  BOOST_REQUIRE(
      (*EventIt).getEvent().getDateTime() == openfluid::core::DateTime(1999,12,31,23,59,59));
  BOOST_REQUIRE_EQUAL((*EventIt).getEvent().getInfosCount(), 4);
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("when","before"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("where","1"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("numeric","1.13"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("string","EADGBE"));

  EventIt++;
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitClass(), "unitsA");
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitID(), 1);
  BOOST_REQUIRE(
      (*EventIt).getEvent().getDateTime() == openfluid::core::DateTime(1999,12,1,12,0,0));
  BOOST_REQUIRE_EQUAL((*EventIt).getEvent().getInfosCount(), 4);
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("when","before"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("where","1"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("numeric","1.13"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("string","EADG"));

  EventIt++;
  EventIt++;
  EventIt++;
  EventIt++;
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitClass(), "unitsB");
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitID(), 7);
  BOOST_REQUIRE(
      (*EventIt).getEvent().getDateTime() == openfluid::core::DateTime(2000,1,1,2,18,12));
  BOOST_REQUIRE_EQUAL((*EventIt).getEvent().getInfosCount(), 4);
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("when","during"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("where","9"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("numeric","1.15"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("string","EADGBE"));


  // Datastore
  // ====================================================================

  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t DataItems =
      FXDesc.getDatastoreDescriptor().getItems();

  // Temporarily disabled GeoRasterValue
  //BOOST_REQUIRE_EQUAL(DataItems.size(),3);
  BOOST_REQUIRE_EQUAL(DataItems.size(), 2);

  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t::iterator DataIt =
      DataItems.begin();

  BOOST_REQUIRE_EQUAL((*DataIt)->getID(), "mymap");
  BOOST_REQUIRE_EQUAL((*DataIt)->getRelativePath(), "datastore/testvect");
  BOOST_REQUIRE_EQUAL((*DataIt)->getType(),
                      openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_REQUIRE_EQUAL((*DataIt)->getUnitClass(), "unitsA");

  DataIt++;

  BOOST_REQUIRE_EQUAL((*DataIt)->getID(), "mymap2");
  BOOST_REQUIRE_EQUAL((*DataIt)->getRelativePath(), "datastore/testvect.shp");
  BOOST_REQUIRE_EQUAL((*DataIt)->getType(),
                      openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_REQUIRE_EQUAL((*DataIt)->getUnitClass(), "");

  // Temporarily disabled GeoRasterValue
  /*    DataIt++;

   BOOST_REQUIRE_EQUAL((*DataIt)->getID(),"myrast");
   BOOST_REQUIRE_EQUAL((*DataIt)->getRelativePath(),"datastore/testrast.tif");
   BOOST_REQUIRE_EQUAL((*DataIt)->getType(),openfluid::core::UnstructuredValue::GeoRasterValue);
   BOOST_REQUIRE_EQUAL((*DataIt)->getUnitClass(),"");*/


  // Monitoring
  // ====================================================================
  openfluid::fluidx::MonitoringDescriptor::SetDescription_t Observers =
       FXDesc.getMonitoringDescriptor().getItems();

   BOOST_CHECK_EQUAL(Observers.size(), 4);

   openfluid::fluidx::MonitoringDescriptor::SetDescription_t::iterator ObsIt =
       Observers.begin();

   // output.files.csv

   BOOST_CHECK(
       (*ObsIt)->isType(openfluid::fluidx::WareDescriptor::PluggedObserver));

   BOOST_CHECK_EQUAL((*ObsIt)->getID(), "output.files.csv");

   openfluid::ware::WareParams_t Params = (*ObsIt)->getParameters();

   BOOST_CHECK_EQUAL(Params.size(), 2);

   BOOST_CHECK_EQUAL(Params.get_child("format").size(), 4);

   std::vector<std::string> FormatNames;
   BOOST_FOREACH(boost::property_tree::ptree::value_type &v,Params.get_child("format"))FormatNames.push_back(v.first);

   BOOST_CHECK_EQUAL(FormatNames[0], "ft1");
   BOOST_CHECK_EQUAL(FormatNames[1], "ft2");
   BOOST_CHECK_EQUAL(FormatNames[2], "ft4");
   BOOST_CHECK_EQUAL(FormatNames[3], "ft5");

   BOOST_CHECK_EQUAL(Params.get<std::string>("format.ft1.colsep"), " ");
   BOOST_CHECK_EQUAL(Params.get<std::string>("format.ft1.dtformat"),
                     "%Y %m %d %H %M %S");
   BOOST_CHECK_EQUAL(Params.get<std::string>("format.ft1.commentchar"), "%");

   BOOST_CHECK_EQUAL(Params.get<std::string>("format.ft2.colsep"), ";");
   BOOST_CHECK_EQUAL(Params.get<std::string>("format.ft2.dtformat"),
                     "%Y%m%dT%H%M%S");
   BOOST_CHECK_EQUAL(Params.get<std::string>("format.ft2.commentchar"), "#");

   BOOST_CHECK_EQUAL(Params.get<std::string>("format.ft4.dtformat"), "iso");

   BOOST_CHECK_EQUAL(Params.get<std::string>("format.ft5.dtformat"), "6cols");

   BOOST_CHECK_EQUAL(Params.get_child("set").size(), 9);

   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full.format"), "ft1");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full.unitsclass"), "XU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full.unitsIDs"), "*");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full.vars"), "*");
   BOOST_CHECK(Params.get_optional<std::string>("set.full.precision") == NULL);

   BOOST_CHECK_EQUAL(Params.get<std::string>("set.2vars.format"), "ft1");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.2vars.unitsclass"), "YU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.2vars.unitsIDs"), "*");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.2vars.vars"), "var1;var2[]");
   BOOST_CHECK_EQUAL(Params.get_optional<int>("set.2vars.precision"), 3);

   BOOST_CHECK_EQUAL(Params.get<std::string>("set.3units.format"), "ft1");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.3units.unitsclass"), "ZU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.3units.unitsIDs"), "5;197;73");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.3units.vars"), "*");
   BOOST_CHECK_EQUAL(Params.get_optional<int>("set.3units.precision"), 5);

   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full2.format"), "ft2");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full2.unitsclass"), "KU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full2.unitsIDs"), "*");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full2.vars"), "*");
   BOOST_CHECK_EQUAL(Params.get_optional<int>("set.full2.precision"), 9);

   BOOST_CHECK_EQUAL(Params.get<std::string>("set.3vars.format"), "ft2");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.3vars.unitsclass"), "LU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.3vars.unitsIDs"), "*");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.3vars.vars"),
                     "var1;var2[];var5");
   BOOST_CHECK(Params.get_optional<int>("set.3vars.precision") == NULL);

   BOOST_CHECK_EQUAL(Params.get<std::string>("set.2units.format"), "ft2");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.2units.unitsclass"), "MU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.2units.unitsIDs"), "2;1");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.2units.vars"), "*");
   BOOST_CHECK(Params.get_optional<int>("set.2units.precision") == NULL);

   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full3.format"), "ft3");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full3.unitsclass"), "UU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full3.unitsIDs"), "*");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full3.vars"), "*");
   BOOST_CHECK(Params.get_optional<int>("set.full3.precision") == NULL);

   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full4.format"), "ft4");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full4.unitsclass"), "UU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full4.unitsIDs"), "*");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full4.vars"), "*");
   BOOST_CHECK(Params.get_optional<int>("set.full4.precision") == NULL);

   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full5.format"), "ft5");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full5.unitsclass"), "UU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full5.unitsIDs"), "*");
   BOOST_CHECK_EQUAL(Params.get<std::string>("set.full5.vars"), "*");
   BOOST_CHECK(Params.get_optional<int>("set.full5.precision") == NULL);

   // output.files.kml

   ObsIt++;

   BOOST_CHECK_EQUAL((*ObsIt)->getID(), "output.files.kml");

   Params = (*ObsIt)->getParameters();

   BOOST_CHECK_EQUAL(Params.size(), 0);

   // output.files.kml-dynamic

   ObsIt++;

   BOOST_CHECK_EQUAL((*ObsIt)->getID(), "output.files.kml-dynamic");

   Params = (*ObsIt)->getParameters();

   BOOST_CHECK_EQUAL(Params.size(), 1);

   BOOST_CHECK_EQUAL(Params.get<std::string>("configfile"), "kmloutput.conf");

   // output.files.vtk

   ObsIt++;

   BOOST_CHECK_EQUAL((*ObsIt)->getID(), "output.files.vtk");

   Params = (*ObsIt)->getParameters();

   BOOST_CHECK_EQUAL(Params.size(), 3);

   BOOST_CHECK_EQUAL(Params.get<std::string>("DEMfile"), "DEMs/virtualdem.tif");
   BOOST_CHECK_EQUAL(Params.get<bool>("visitfile.create"), true);

   BOOST_CHECK_EQUAL(Params.get_child("serie").size(), 2);

   BOOST_CHECK_EQUAL(Params.get<std::string>("serie.vtk1.unitclass"), "SU");
   BOOST_CHECK_EQUAL(Params.get<std::string>("serie.vtk1.var"), "tests.var1");
   BOOST_CHECK_EQUAL(Params.get<std::string>("serie.vtk1.shapefile"),
                     "shapefiles/SU.shp");
   BOOST_CHECK_EQUAL(Params.get<int>("serie.vtk1.step"), 1);

   BOOST_CHECK_EQUAL(Params.get<std::string>("serie.vtk7.unitclass"), "RS");
   BOOST_CHECK_EQUAL(Params.get<std::string>("serie.vtk7.var"), "tests.var3");
   BOOST_CHECK_EQUAL(Params.get<std::string>("serie.vtk7.shapefile"),
                     "shapefiles/RS.shp");
   BOOST_CHECK_EQUAL(Params.get<int>("serie.vtk7.step"), 10);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_read_operations)
{
  std::vector<std::string> DatasetPaths;
  DatasetPaths.push_back(
      boost::filesystem::path(
          CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptor/manyfiles1").string());
  DatasetPaths.push_back(
      boost::filesystem::path(
          CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptor/singlefile1").string());
  DatasetPaths.push_back(
      boost::filesystem::path(
          CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptor/singlefile2").string());

  boost::unit_test::test_suite* TestSuite = BOOST_TEST_SUITE( "" );
  TestSuite->add(
      BOOST_PARAM_TEST_CASE(&TestDataset,DatasetPaths.begin(),DatasetPaths.end()));

  boost::unit_test::framework::run(TestSuite);
  boost::unit_test::test_results const& TestResults =
      boost::unit_test::results_collector.results(TestSuite->p_id);

  boost::unit_test::unit_test_log.set_stream(std::cerr);
  BOOST_CHECK(TestResults.p_assertions_failed == 0);
  BOOST_CHECK(!TestResults.p_aborted);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_error_handling_while_reading)
{
  bool HasFailed;
  boost::onullstream ONullStream;

  HasFailed = false;
  try
  {
    openfluid::fluidx::FluidXDescriptor(new openfluid::base::IOListener()).loadFromDirectory(
        boost::filesystem::path(
            CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptor/pathdoesnotexist").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed, true);

  HasFailed = false;
  try
  {
    openfluid::fluidx::FluidXDescriptor(new openfluid::base::IOListener()).loadFromDirectory(
        boost::filesystem::path(
            CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptor/wrong-nofile").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed, true);

  HasFailed = false;
  try
  {
    openfluid::fluidx::FluidXDescriptor(new openfluid::base::IOListener()).loadFromDirectory(
        boost::filesystem::path(
            CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptor/wrong-twomodels").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed, true);

  HasFailed = false;
  try
  {
    openfluid::fluidx::FluidXDescriptor(new openfluid::base::IOListener()).loadFromDirectory(
        boost::filesystem::path(
            CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptor/wrong-runs").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed, true);

  BOOST_REQUIRE_THROW(
      openfluid::fluidx::FluidXDescriptor(new openfluid::base::IOListener()).loadFromDirectory( boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+ "/OPENFLUID.IN.FluidXDescriptor/wrong-unknowndatatype").string()),
      openfluid::base::OFException);

  BOOST_REQUIRE_THROW(
      openfluid::fluidx::FluidXDescriptor(new openfluid::base::IOListener()).loadFromDirectory( boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+ "/OPENFLUID.IN.FluidXDescriptor/wrong-missingdataid").string()),
      openfluid::base::OFException);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_write_operations)
{
  std::string InputDir = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXDescriptor/manyfiles1";
  std::string OutputDirSingle = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXDescriptorSingle";
  std::string OutputDirMany = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXDescriptorMany";

  openfluid::fluidx::FluidXDescriptor FXDesc(new openfluid::base::IOListener());

  FXDesc.loadFromDirectory(InputDir);

  FXDesc.writeToManyFiles(OutputDirMany);
  FXDesc.writeToSingleFile(OutputDirSingle+"/all.fluidx");

  std::vector<std::string> DatasetPaths;
  DatasetPaths.push_back(OutputDirSingle);
  DatasetPaths.push_back(OutputDirMany);

  boost::unit_test::test_suite* TestSuite = BOOST_TEST_SUITE( "" );
  TestSuite->add(
      BOOST_PARAM_TEST_CASE(&TestDataset,DatasetPaths.begin(),DatasetPaths.end()));

  boost::unit_test::framework::run(TestSuite);
  boost::unit_test::test_results const& TestResults =
      boost::unit_test::results_collector.results(TestSuite->p_id);

  boost::unit_test::unit_test_log.set_stream(std::cerr);
  BOOST_CHECK(TestResults.p_assertions_failed == 0);
  BOOST_CHECK(!TestResults.p_aborted);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_write_operations_for_integration_tests)
{
  std::string InputDir = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXWriter";
  std::string OutputDirSingle = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXWriterSingle";
  std::string OutputDirMany = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXWriterMany";

  openfluid::fluidx::FluidXDescriptor FXDesc(new openfluid::base::IOListener());

  FXDesc.loadFromDirectory(InputDir);

  FXDesc.writeToManyFiles(OutputDirMany);
  FXDesc.writeToSingleFile(OutputDirSingle+"/all.fluidx");
}
