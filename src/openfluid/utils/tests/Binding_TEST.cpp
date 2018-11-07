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
  @file Binding_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_binding


#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <openfluid/global.hpp>
#include <openfluid/utils/Binding.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


#define PRINTCHARRAY(prefix,array,count) \
  for (unsigned int ipca=0; ipca<(count); ipca++) \
  { \
    std::cout << (prefix) << std::string(array[ipca]) << std::endl; \
  }

#define PRINTNUMARRAY(prefix,array,count) \
  for (unsigned int ipca=0; ipca<(count); ipca++) \
  { \
    std::cout << (prefix) << array[ipca] << std::endl; \
  }


// =====================================================================
// =====================================================================


class ErrOut : public openfluid::utils::BindingAbstractOutErr
{
  public:

    void printfOut(const char* fmt, ... ) const
    {
      va_list args;
      va_start(args,fmt);
      vprintf(fmt,args);
      va_end(args);
    }

    void printfErr(const char* fmt, ... ) const
    {
      va_list args;
      va_start(args,fmt);
      vfprintf(stderr,fmt,args);
      va_end(args);
    }
};


OPENFLUID_BINDING_DEFINE(ErrOut);


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::utils::Binding* TB = openfluid::utils::Binding::make();
  openfluid::utils::Binding::destroy(TB);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_datasetsprojects)
{
  openfluid::utils::Binding* TB;


  // dataset

  std::string DSPath = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.PrimitivesAll";
  TB = openfluid::utils::Binding::openDataset(DSPath.c_str());
  TB->printSimulationInfo();

  auto Classes = TB->getUnitsClasses();
  unsigned int ClassesCount = TB->getUnitsClassesCount();
  BOOST_REQUIRE_EQUAL(ClassesCount,2);
  PRINTCHARRAY("unitsclass: ",Classes,ClassesCount);
  delete Classes;

  auto UnitsIDs = TB->getUnitsIDs("TestUnits");
  unsigned int UnitsIDsCount = TB->getUnitsIDsCount("TestUnits");
  BOOST_REQUIRE_EQUAL(UnitsIDsCount,12);
  PRINTNUMARRAY("unitid: ",UnitsIDs,UnitsIDsCount);
  delete UnitsIDs;

  std::cout << "simIDs: " << TB->getSimulatorsIDs() << std::endl;
  std::cout << "obsIDs: " << TB->getObserversIDs() << std::endl;

  openfluid::utils::Binding::destroy(TB);


  // project

  std::string PrjPath = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.PRJ.Primitives";
  TB = openfluid::utils::Binding::openProject(PrjPath.c_str());
  TB->printSimulationInfo();

  Classes = TB->getUnitsClasses();
  ClassesCount = TB->getUnitsClassesCount();
  BOOST_REQUIRE_EQUAL(ClassesCount,2);
  PRINTCHARRAY("unitsclass: ",Classes,ClassesCount);
  delete Classes;

  UnitsIDs = TB->getUnitsIDs("TestUnits");
  UnitsIDsCount = TB->getUnitsIDsCount("TestUnits");
  BOOST_REQUIRE_EQUAL(UnitsIDsCount,12);
  PRINTNUMARRAY("unitid: ",UnitsIDs,UnitsIDsCount);
  delete UnitsIDs;

  std::cout << "simIDs: " << TB->getSimulatorsIDs() << std::endl;
  std::cout << "obsIDs: " << TB->getObserversIDs() << std::endl;

  openfluid::utils::Binding::destroy(TB);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_warespaths)
{
#if defined(OPENFLUID_OS_UNIX)
  std::string PathSep(":");
#elif defined(OPENFLUID_OS_WINDOWS)
  std::string PathSep(";");
#endif


  // Simulators
  auto Paths = openfluid::utils::Binding::getSimulatorsPaths();
  auto PathsCount = openfluid::utils::Binding::getSimulatorsPathsCount();
  PRINTCHARRAY("simpath: ",Paths,PathsCount);
  delete Paths;

  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraSimulatorsPathsCount(),0);

  openfluid::utils::Binding::addExtraSimulatorsPaths("/path/sim/1");
  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraSimulatorsPathsCount(),1);

  openfluid::utils::Binding::addExtraSimulatorsPaths("/path/sim/2");
  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraSimulatorsPathsCount(),2);

  openfluid::utils::Binding::addExtraSimulatorsPaths(std::string("/path/sim/3"+PathSep+"/path/sim/4").c_str());
  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraSimulatorsPathsCount(),4);

  openfluid::utils::Binding::resetExtraSimulatorsPaths();
  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraSimulatorsPathsCount(),0);


  // Observers
  Paths = openfluid::utils::Binding::getObserversPaths();
  PathsCount = openfluid::utils::Binding::getObserversPathsCount();
  PRINTCHARRAY("obspath: ",Paths,PathsCount);
  delete Paths;

  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraObserversPathsCount(),0);

  openfluid::utils::Binding::addExtraObserversPaths("/path/obs/100");
  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraObserversPathsCount(),1);

  openfluid::utils::Binding::addExtraObserversPaths("/path/obs/200");
  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraObserversPathsCount(),2);

  openfluid::utils::Binding::addExtraObserversPaths(std::string("/path/obs/300"+PathSep+"/path/obs/400").c_str());
  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraObserversPathsCount(),4);

  openfluid::utils::Binding::resetExtraObserversPaths();
  BOOST_REQUIRE_EQUAL(openfluid::utils::Binding::getExtraObserversPathsCount(),0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_config)
{
  openfluid::utils::Binding* TB;

  std::string DSPath = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.PrimitivesAll";
  TB = openfluid::utils::Binding::openDataset(DSPath.c_str());

  BOOST_REQUIRE_EQUAL(TB->getDefaultDeltaT(),3600);
  TB->setDefaultDeltaT(600);
  BOOST_REQUIRE_EQUAL(TB->getDefaultDeltaT(),600);

  BOOST_REQUIRE_EQUAL(TB->getPeriodBeginDate(),"2000-01-01 00:00:00");
  BOOST_REQUIRE_EQUAL(TB->getPeriodEndDate(),"2000-01-01 06:00:00");
  TB->setPeriod("2008-01-02 11:13:00","2011-08-06 15:00:00");
  BOOST_REQUIRE_EQUAL(TB->getPeriodBeginDate(),"2008-01-02 11:13:00");
  BOOST_REQUIRE_EQUAL(TB->getPeriodEndDate(),"2011-08-06 15:00:00");

  openfluid::utils::Binding::destroy(TB);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_parameters)
{
  openfluid::utils::Binding* TB;

  std::string DSPath = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.PrimitivesAll";
  TB = openfluid::utils::Binding::openDataset(DSPath.c_str());


  // simulators

  BOOST_REQUIRE(std::string(TB->getSimulatorParamNames("tests.primitives.simparams")).find("strparam") !=
                std::string::npos);
  BOOST_REQUIRE_EQUAL(TB->getSimulatorParam("tests.primitives.simparams","strparam"),"strvalue");

  TB->setSimulatorParam("tests.primitives.simparams","bindparam","binded!");
  BOOST_REQUIRE_EQUAL(TB->getSimulatorParam("tests.primitives.simparams","bindparam"),"binded!");
  TB->removeSimulatorParam("tests.primitives.simparams","bindparam");
  BOOST_REQUIRE_EQUAL(TB->getSimulatorParam("tests.primitives.simparams","bindparam"),"");
  BOOST_REQUIRE(std::string(TB->getSimulatorParamNames("tests.primitives.simparams")).find("bindparam") ==
                std::string::npos);


  // model global

  BOOST_REQUIRE(std::string(TB->getModelGlobalParamNames()).find("bindgparam") ==
                std::string::npos);
  TB->setModelGlobalParam("bindgparam","bindedg!");
  BOOST_REQUIRE_EQUAL(TB->getModelGlobalParam("bindgparam"),"bindedg!");
  BOOST_REQUIRE(std::string(TB->getModelGlobalParamNames()).find("bindgparam") !=
                std::string::npos);
  TB->removeModelGlobalParam("bindgparam");
  BOOST_REQUIRE_EQUAL(TB->getModelGlobalParam("bindgparam"),"");
  BOOST_REQUIRE(std::string(TB->getModelGlobalParamNames()).find("bindgparam") ==
                std::string::npos);


  // observers

  BOOST_REQUIRE(std::string(TB->getObserverParamNames("export.vars.files.csv")).find("set.fullfull.unitsclass") !=
                std::string::npos);
  BOOST_REQUIRE_EQUAL(TB->getObserverParam("export.vars.files.csv","set.fullfull.unitsclass"),"TestUnits");

  TB->setObserverParam("export.vars.files.csv","bindparam","binded!");
  BOOST_REQUIRE_EQUAL(TB->getObserverParam("export.vars.files.csv","bindparam"),"binded!");
  TB->removeObserverParam("export.vars.files.csv","bindparam");
  BOOST_REQUIRE_EQUAL(TB->getObserverParam("export.vars.files.csv","bindparam"),"");
  BOOST_REQUIRE(std::string(TB->getObserverParamNames("export.vars.files.csv")).find("bindparam") ==
                std::string::npos);


  openfluid::utils::Binding::destroy(TB);


  // generators

  DSPath = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.Generators";
  TB = openfluid::utils::Binding::openDataset(DSPath.c_str());

  BOOST_REQUIRE(std::string(TB->getGeneratorParamNames("TestUnits","tests.random")).find("min") !=
                std::string::npos);
  BOOST_REQUIRE(std::string(TB->getGeneratorParamNames("TestUnits","tests.random")).find("max") !=
                  std::string::npos);
  BOOST_REQUIRE(std::string(TB->getGeneratorParamNames("TestUnits","tests.random")).find("mid") ==
                  std::string::npos);

  BOOST_REQUIRE_EQUAL(TB->getGeneratorParam("TestUnits","tests.random","min"),"20.53");

  TB->setGeneratorParam("TestUnits","tests.random","bindparam","binded");
  BOOST_REQUIRE_EQUAL(TB->getGeneratorParam("TestUnits","tests.random","bindparam"),"binded");

  openfluid::utils::Binding::destroy(TB);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_attributes)
{
  openfluid::utils::Binding* TB;

  std::string DSPath = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.PrimitivesAll";
  TB = openfluid::utils::Binding::openDataset(DSPath.c_str());

  BOOST_REQUIRE(std::string(TB->getAttributesNames("TestUnits")).find("indataDouble") != std::string::npos);
  BOOST_REQUIRE(std::string(TB->getAttributesNames("TestUnits")).find("indataVector") != std::string::npos);
  BOOST_REQUIRE(std::string(TB->getAttributesNames("TestUnits")).find("indataWrong") == std::string::npos);

  BOOST_REQUIRE_EQUAL(TB->getAttribute("TestUnits",1,"indataDouble"),"1.1");
  BOOST_REQUIRE_EQUAL(TB->getAttribute("TestUnits",6,"indataVector"),"[6.1,6.2,6.3]");
  BOOST_REQUIRE_EQUAL(TB->getAttribute("TestUnits",10,"indataString"),"CODE10");

  TB->createAttribute("TestUnits","bindStr","binded");
  BOOST_REQUIRE(std::string(TB->getAttributesNames("TestUnits")).find("bindStr") != std::string::npos);
  BOOST_REQUIRE_EQUAL(TB->getAttribute("TestUnits",8,"bindStr"),"binded");
  TB->setAttribute("TestUnits",8,"bindStr","not binded");
  BOOST_REQUIRE_EQUAL(TB->getAttribute("TestUnits",8,"bindStr"),"not binded");
  BOOST_REQUIRE_EQUAL(TB->getAttribute("TestUnits",9,"bindStr"),"binded");
  TB->removeAttribute("TestUnits","bindStr");
  BOOST_REQUIRE(std::string(TB->getAttributesNames("TestUnits")).find("bindStr") == std::string::npos);
  BOOST_REQUIRE_EQUAL(TB->getAttribute("TestUnits",8,"bindStr"),"");

  openfluid::utils::Binding::destroy(TB);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_simulations)
{
  openfluid::utils::Binding* TB = nullptr;

  std::string DSPath = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.PrimitivesAll";
  std::string OutPath = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.PrimitivesAll-Binding";

  openfluid::utils::Binding::addExtraSimulatorsPaths(CONFIGTESTS_BINARY_DIR.c_str());
  openfluid::utils::Binding::setCurrentOutputDir(OutPath.c_str());

  TB = openfluid::utils::Binding::openDataset(DSPath.c_str());

  TB->addVariablesExportAsCSV("binding","TestUnits","*","*",10);

  BOOST_CHECK_EQUAL(TB->runSimulation(),1);
  std::cout << TB->getLastError() << std::endl;

  BOOST_CHECK_EQUAL(TB->runSimulation(true),1);
  std::cout << TB->getLastError() << std::endl;

  openfluid::utils::Binding::destroy(TB);
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  openfluid::utils::Binding::init();

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
