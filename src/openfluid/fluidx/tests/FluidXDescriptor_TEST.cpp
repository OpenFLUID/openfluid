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
  @file FluidXDescriptor_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_fluidxdescriptor


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/results_collector.hpp>
#if BOOST_WORKAROUND(  __GNUC__, < 3 )
#include <boost/test/output_test_stream.hpp>
typedef boost::test_tools::output_test_stream onullstream_type;
#else
#include <boost/test/utils/nullstream.hpp>
typedef boost::onullstream onullstream_type;
#endif

#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/WareSetDescriptor.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::IOListener* L = new openfluid::base::IOListener();

  openfluid::fluidx::FluidXDescriptor FXDesc(L);

  delete L;
}


// =====================================================================
// =====================================================================


void TestDataset(std::string DatasetPath, bool /*AggregatedAttributes*/ = false)
{
  std::cout << "Checking " << DatasetPath << std::endl;
  BOOST_TEST_CHECKPOINT("Checking " << DatasetPath);

  openfluid::base::IOListener* L = new openfluid::base::IOListener();
  openfluid::fluidx::FluidXDescriptor FXDesc(L);

  boost::onullstream ONullStream;
  FXDesc.loadFromDirectory(DatasetPath);


  BOOST_TEST_CHECKPOINT("-- Model");

  BOOST_REQUIRE_EQUAL(FXDesc.model().getGlobalParameters().size(), 2);
  BOOST_REQUIRE_EQUAL(FXDesc.model().getGlobalParameters()["gparam1"].get(),"100");
  BOOST_REQUIRE_EQUAL(FXDesc.model().getGlobalParameters()["gparam2"].get(),"0.1");

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t ModelItems;

  ModelItems = FXDesc.model().items();

  BOOST_REQUIRE_EQUAL(ModelItems.size(), 5);

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t::iterator it;

  it = ModelItems.begin();
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::ware::WareType::GENERATOR), true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),
                      "tests.generator.interp");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(), 1);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitsClass(), "TU");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod(),
                      openfluid::fluidx::GeneratorDescriptor::Interp);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),
                      2);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters()["sources"].get(),
                      "sources.xml");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters()["distribution"].get(),
                      "distri.dat");

  ++it;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::ware::WareType::SIMULATOR),
                      true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::SimulatorDescriptor*)(*it))->getID(),
                      "tests.simulatorA");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::SimulatorDescriptor*)(*it))->getParameters().size(),
                      0);

  ++it;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::ware::WareType::GENERATOR), true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),
                      "tests.generator.fixed");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(), 11);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitsClass(), "TU");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod(),
                      openfluid::fluidx::GeneratorDescriptor::Fixed);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),
                      1);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters()["fixedvalue"].get(),
                      "20");

  ++it;
  BOOST_REQUIRE_EQUAL(
      (*it)->isType(openfluid::ware::WareType::GENERATOR), true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),
                      "tests.generator.random");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(), 1);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitsClass(), "TU");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod(),
                      openfluid::fluidx::GeneratorDescriptor::Random);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),
                      2);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters()["min"].get(),
                      "20.53");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters()["max"].get(),
                      "50");

  ++it;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::ware::WareType::SIMULATOR),true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::SimulatorDescriptor*)(*it))->getID(),
                      "tests.simulatorB");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::SimulatorDescriptor*)(*it))->getParameters().size(),4);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters()["gparam1"].get(),
                      "50");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters()["strparam"].get(),
                      "strvalue");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters()["doubleparam"].get(),
                      "1.1");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters()["longparam"].get(),
                      "11");


  BOOST_TEST_CHECKPOINT("-- Run");

  BOOST_REQUIRE_EQUAL(FXDesc.runConfiguration().getDeltaT(), 4753);
  BOOST_REQUIRE(FXDesc.runConfiguration().getBeginDate() == openfluid::core::DateTime(1997,1,2,11,15,48));
  BOOST_REQUIRE(FXDesc.runConfiguration().getEndDate() == openfluid::core::DateTime(2005,11,30,6,53,7));
  BOOST_REQUIRE_EQUAL(FXDesc.runConfiguration().isUserValuesBufferSize(), true);
  BOOST_REQUIRE_EQUAL(FXDesc.runConfiguration().getValuesBufferSize(), 100);


  BOOST_TEST_CHECKPOINT("-- Domain definition");

  BOOST_REQUIRE_EQUAL(FXDesc.spatialDomain().getUnitsCount(), 14);

  BOOST_REQUIRE_EQUAL(FXDesc.spatialDomain().getUnitsCount("unitsP"), 1);
  BOOST_REQUIRE_EQUAL(FXDesc.spatialDomain().getUnitsCount("unitsA"), 8);
  BOOST_REQUIRE_EQUAL(FXDesc.spatialDomain().getUnitsCount("unitsB"), 5);
  BOOST_REQUIRE_EQUAL(FXDesc.spatialDomain().getUnitsCount("unitsZ"), 0);

  auto Unit = FXDesc.spatialDomain().spatialUnit("unitsP",1);
  BOOST_REQUIRE_EQUAL(Unit.getUnitsClass(),"unitsP");
  BOOST_REQUIRE_EQUAL(Unit.getID(), 1);
  BOOST_REQUIRE_EQUAL(Unit.getProcessOrder(), 1);
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().size(), 0);
  BOOST_REQUIRE_EQUAL(Unit.parentSpatialUnits().size(), 0);

  Unit = FXDesc.spatialDomain().spatialUnit("unitsA",3);
  BOOST_REQUIRE_EQUAL(Unit.getUnitsClass(), "unitsA");
  BOOST_REQUIRE_EQUAL(Unit.getID(), 3);
  BOOST_REQUIRE_EQUAL(Unit.getProcessOrder(), 1);
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().size(), 1);
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().front().first, "unitsB");
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().front().second, 11);
  BOOST_REQUIRE_EQUAL(Unit.parentSpatialUnits().size(), 1);
  BOOST_REQUIRE_EQUAL(Unit.parentSpatialUnits().front().first, "unitsP");
  BOOST_REQUIRE_EQUAL(Unit.parentSpatialUnits().front().second, 1);

  Unit = FXDesc.spatialDomain().spatialUnit("unitsA",1);
  BOOST_REQUIRE_EQUAL(Unit.getUnitsClass(), "unitsA");
  BOOST_REQUIRE_EQUAL(Unit.getID(), 1);
  BOOST_REQUIRE_EQUAL(Unit.getProcessOrder(), 1);
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().size(), 1);
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().front().first, "unitsB");
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().front().second, 2);
  BOOST_REQUIRE_EQUAL(Unit.parentSpatialUnits().size(), 0);

  Unit = FXDesc.spatialDomain().spatialUnit("unitsB",11);
  BOOST_REQUIRE_EQUAL(Unit.getID(), 11);
  BOOST_REQUIRE_EQUAL(Unit.getProcessOrder(), 1);
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().size(), 1);
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().front().first, "unitsB");
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().front().second, 3);
  BOOST_REQUIRE_EQUAL(Unit.parentSpatialUnits().size(), 1);
  BOOST_REQUIRE_EQUAL(Unit.parentSpatialUnits().front().first, "unitsP");
  BOOST_REQUIRE_EQUAL(Unit.parentSpatialUnits().front().second, 1);

  Unit = FXDesc.spatialDomain().spatialUnit("unitsB",7);
  BOOST_REQUIRE_EQUAL(Unit.getUnitsClass(), "unitsB");
  BOOST_REQUIRE_EQUAL(Unit.getID(), 7);
  BOOST_REQUIRE_EQUAL(Unit.getProcessOrder(), 4);
  BOOST_REQUIRE_EQUAL(Unit.toSpatialUnits().size(), 0);
  BOOST_REQUIRE_EQUAL(Unit.parentSpatialUnits().size(), 0);


  BOOST_TEST_CHECKPOINT("-- Datastore");

  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t DataItems = FXDesc.datastore().items();

  BOOST_REQUIRE_EQUAL(DataItems.size(),3);

  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t::iterator DataIt = DataItems.begin();

  BOOST_REQUIRE_EQUAL((*DataIt)->getID(), "mymap");
  BOOST_REQUIRE_EQUAL((*DataIt)->getRelativePath(), "datastore/testvect");
  BOOST_REQUIRE_EQUAL((*DataIt)->getType(),openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_REQUIRE_EQUAL((*DataIt)->getUnitsClass(), "unitsA");

  ++DataIt;

  BOOST_REQUIRE_EQUAL((*DataIt)->getID(), "mymap2");
  BOOST_REQUIRE_EQUAL((*DataIt)->getRelativePath(), "datastore/testvect.shp");
  BOOST_REQUIRE_EQUAL((*DataIt)->getType(),openfluid::core::UnstructuredValue::GeoVectorValue);
  BOOST_REQUIRE_EQUAL((*DataIt)->getUnitsClass(), "");

  ++DataIt;

  BOOST_REQUIRE_EQUAL((*DataIt)->getID(),"myrast");
  BOOST_REQUIRE_EQUAL((*DataIt)->getRelativePath(),"datastore/testrast.tif");
  BOOST_REQUIRE_EQUAL((*DataIt)->getType(),openfluid::core::UnstructuredValue::GeoRasterValue);
  BOOST_REQUIRE_EQUAL((*DataIt)->getUnitsClass(),"");


  BOOST_TEST_CHECKPOINT("-- Monitoring");

  openfluid::fluidx::MonitoringDescriptor::SetDescription_t Observers = FXDesc.monitoring().items();

   BOOST_CHECK_EQUAL(Observers.size(), 4);

   openfluid::fluidx::MonitoringDescriptor::SetDescription_t::iterator ObsIt = Observers.begin();

   // output.files.csv

   BOOST_CHECK((*ObsIt)->isType(openfluid::ware::WareType::OBSERVER));
   BOOST_CHECK_EQUAL((*ObsIt)->getID(), "output.files.csv");


   // output.files.kml

   ++ObsIt;

   BOOST_CHECK_EQUAL((*ObsIt)->getID(), "output.files.kml");


   // output.files.kml-dynamic

   ++ObsIt;

   BOOST_CHECK_EQUAL((*ObsIt)->getID(), "output.files.kml-dynamic");


   // output.files.vtk

   ++ObsIt;

   BOOST_CHECK_EQUAL((*ObsIt)->getID(), "output.files.vtk");

   delete L;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_read_operations)
{
  std::vector<std::string> DatasetPaths;
  DatasetPaths.push_back(CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptors/manyfiles1");
  DatasetPaths.push_back(CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptors/singlefile1");
  DatasetPaths.push_back(CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptors/singlefile2");

  for (const auto& Path : DatasetPaths)
  {
    TestDataset(Path);
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_error_handling_while_reading)
{
  bool HasFailed;
  boost::onullstream ONullStream;

  openfluid::base::IOListener* L = new openfluid::base::IOListener();

  HasFailed = false;
  try
  {
    openfluid::fluidx::FluidXDescriptor(L)
          .loadFromDirectory(CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptors/pathdoesnotexist");
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed, true);

  HasFailed = false;
  try
  {
    openfluid::fluidx::FluidXDescriptor(L)
          .loadFromDirectory(CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptors/wrong-nofile");
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed, true);

  HasFailed = false;
  try
  {
    openfluid::fluidx::FluidXDescriptor(L)
          .loadFromDirectory(CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptors/wrong-twomodels");
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed, true);

  HasFailed = false;
  try
  {
    openfluid::fluidx::FluidXDescriptor(L)
          .loadFromDirectory(CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.FluidXDescriptors/wrong-runs");
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed, true);

  BOOST_REQUIRE_THROW(
      openfluid::fluidx::FluidXDescriptor(L)
            .loadFromDirectory(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXDescriptors/wrong-unknowndatatype"),
      openfluid::base::FrameworkException);

  BOOST_REQUIRE_THROW(
      openfluid::fluidx::FluidXDescriptor(L)
            .loadFromDirectory(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXDescriptors/wrong-missingdataid"),
      openfluid::base::FrameworkException);


  delete L;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_write_operations)
{
  std::string InputDir = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXDescriptors/manyfiles1";
  std::string OutputDirSingle = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXDescriptorSingle";
  std::string OutputDirMany = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXDescriptorMany";

  openfluid::base::IOListener* L = new openfluid::base::IOListener();
  openfluid::fluidx::FluidXDescriptor FXDesc(L);

  FXDesc.loadFromDirectory(InputDir);

  FXDesc.writeToManyFiles(OutputDirMany);
  FXDesc.writeToSingleFile(OutputDirSingle+"/all.fluidx");

  std::vector<std::string> DatasetPaths;
  DatasetPaths.push_back(OutputDirSingle);
  DatasetPaths.push_back(OutputDirMany);

  for (const auto& Path : DatasetPaths)
    TestDataset(Path,true);

  delete L;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_write_operations_for_integration_tests)
{
  std::string InputDir = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXWriter";
  std::string OutputDirSingle = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXWriterSingle";
  std::string OutputDirMany = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXWriterMany";

  openfluid::base::IOListener* L = new openfluid::base::IOListener();
  openfluid::fluidx::FluidXDescriptor FXDesc(L);

  FXDesc.loadFromDirectory(InputDir);

  FXDesc.writeToManyFiles(OutputDirMany);
  FXDesc.writeToSingleFile(OutputDirSingle+"/all.fluidx");

  delete L;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_write_read_operations_of_xml_entities)
{
  std::string DatasetDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.FluidXWriterXMLEntities";

  std::string RefParamStr = "< ' > && \"hula hoop\"";


  openfluid::base::IOListener* L = new openfluid::base::IOListener();

  {
    openfluid::fluidx::ObserverDescriptor* ObsDesc = nullptr;
    openfluid::fluidx::FluidXDescriptor FXDesc(L);

    FXDesc.runConfiguration().setBeginDate(openfluid::core::DateTime(2014,9,4,17,0,0));
    FXDesc.runConfiguration().setEndDate(openfluid::core::DateTime(2014,9,4,18,0,0));
    FXDesc.runConfiguration().setDeltaT(60);

    FXDesc.runConfiguration().setFilled(true);

    ObsDesc = new openfluid::fluidx::ObserverDescriptor("tests.observer");
    ObsDesc->setParameter("param1",openfluid::core::StringValue(RefParamStr));

    FXDesc.monitoring().items().push_back(ObsDesc);

    FXDesc.writeToManyFiles(DatasetDir);
  }


  {
    openfluid::fluidx::FluidXDescriptor FXDesc(L);

    FXDesc.loadFromDirectory(DatasetDir);

    openfluid::ware::WareParams_t Params = FXDesc.monitoring().items().front()->getParameters();

    BOOST_REQUIRE_EQUAL(Params["param1"].toString(),RefParamStr);
  }

  delete L;

}
