/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_modelreader
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>

#include "tests-config.h"
#include "ModelReader.h"
#include "FunctionDescriptor.h"
#include "GeneratorDescriptor.h"
// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  ModelReader Reader;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  ModelReader Reader;
  ModelDescriptor ModelDesc;
  ModelDesc = Reader.readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/model.xml").string());

  ModelDescriptor::ModelDescription_t ModelItems;

  ModelItems = ModelDesc.getItems();

  BOOST_REQUIRE_EQUAL(ModelItems.size(),5);

  ModelDescriptor::ModelDescription_t::iterator it;



  it = ModelItems.begin();
  BOOST_REQUIRE_EQUAL((*it)->isType(ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getVariableName(),"tests.generator.interp");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getUnitClass(),"TU");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getGeneratorMethod(),GeneratorDescriptor::Interp);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters().size(),4);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["sources"],"sources.xml");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["distribution"],"distri.dat");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam1"],"100");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam2"],"0.1");

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(ModelItemDescriptor::PluggedFunction),true);
  BOOST_REQUIRE_EQUAL(((FunctionDescriptor*)(*it))->getFileID(),"tests.functionA");
  BOOST_REQUIRE_EQUAL(((FunctionDescriptor*)(*it))->getParameters().size(),2);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam1"],"100");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam2"],"0.1");

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getVariableName(),"tests.generator.fixed");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getUnitClass(),"TU");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getGeneratorMethod(),GeneratorDescriptor::Fixed);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters().size(),3);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["fixedvalue"],"20");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam1"],"100");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam2"],"0.1");

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getVariableName(),"tests.generator.random");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getUnitClass(),"TU");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getGeneratorMethod(),GeneratorDescriptor::Random);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters().size(),4);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["min"],"20.53");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["max"],"50");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam1"],"100");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam2"],"0.1");


  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(ModelItemDescriptor::PluggedFunction),true);
  BOOST_REQUIRE_EQUAL(((FunctionDescriptor*)(*it))->getFileID(),"tests.functionB");
  BOOST_REQUIRE_EQUAL(((FunctionDescriptor*)(*it))->getParameters().size(),5);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam1"],"50");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["gparam2"],"0.1");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["strparam"],"strvalue");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["doubleparam"],"1.1");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters()["longparam"],"11");

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_error_handling)
{
  ModelDescriptor ModelDesc;

  bool HasFailed;

  HasFailed = false;
  try
  {
    ModelDesc = ModelReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/modeldoesnotexist.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    ModelDesc = ModelReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongmodel1.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    ModelDesc = ModelReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongmodel3.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    ModelDesc = ModelReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongmodel3.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    ModelDesc = ModelReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongmodel4.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    ModelDesc = ModelReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongmodel5.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    ModelDesc = ModelReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongmodel6.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    ModelDesc = ModelReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongmodel7.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    ModelDesc = ModelReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongmodel8.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);

}


