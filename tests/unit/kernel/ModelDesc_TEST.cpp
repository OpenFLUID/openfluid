/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelItemDesc_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_modeldesc
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "model/GeneratorDescriptor.h"
#include "model/FunctionDescriptor.h"
#include "model/ModelDescriptor.h"
#include "openfluid-base.h"

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  FunctionDescriptor FuncDesc("test.id");

  BOOST_REQUIRE_EQUAL(FuncDesc.getFileID(),"test.id");
  BOOST_REQUIRE_EQUAL(FuncDesc.isType(ModelItemDescriptor::PluggedFunction),true);
  BOOST_REQUIRE_EQUAL(FuncDesc.getParameters().size(),0);

  GeneratorDescriptor GenDesc1("test.var","test.unitclass",GeneratorDescriptor::Fixed);

  BOOST_REQUIRE_EQUAL(GenDesc1.getVariableName(),"test.var");
  BOOST_REQUIRE_EQUAL(GenDesc1.getUnitClass(),"test.unitclass");
  BOOST_REQUIRE_EQUAL(GenDesc1.getGeneratorMethod(),GeneratorDescriptor::Fixed);
  BOOST_REQUIRE_EQUAL(GenDesc1.getParameters().size(),0);

  GeneratorDescriptor GenDesc2("test.var2","test.unitclass2",GeneratorDescriptor::Interp);

  BOOST_REQUIRE_EQUAL(GenDesc2.getVariableName(),"test.var2");
  BOOST_REQUIRE_EQUAL(GenDesc2.getUnitClass(),"test.unitclass2");
  BOOST_REQUIRE_EQUAL(GenDesc2.getGeneratorMethod(),GeneratorDescriptor::Interp);
  BOOST_REQUIRE_EQUAL(GenDesc2.getParameters().size(),0);

  ModelDescriptor ModelDesc;

  BOOST_REQUIRE_EQUAL(ModelDesc.getItems().size(),0);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  FunctionDescriptor FuncDesc("test.id");
  FuncDesc.setParameter("param1","var1");
  FuncDesc.setParameter("param2","var2");
  FuncDesc.setParameter("param3","var3");
  FuncDesc.setParameter("param2","var22");

  GeneratorDescriptor GenDesc1("test.var","test.unitclass",GeneratorDescriptor::Fixed);
  GenDesc1.setParameter("fixedvalue","20.5");

  GeneratorDescriptor GenDesc2("test.var2","test.unitclass2",GeneratorDescriptor::Interp);
  GenDesc2.setParameter("sources","datasources.xml");
  GenDesc2.setParameter("distribution","distribution.dat");

  ModelDescriptor ModelDesc;

  ModelDesc.appendItem(&GenDesc1);
  ModelDesc.appendItem(&FuncDesc);
  ModelDesc.appendItem(&GenDesc2);

  BOOST_REQUIRE_EQUAL(ModelDesc.getItems().size(),3);

  ModelDescriptor::ModelDescription_t ModelItems;

  ModelItems = ModelDesc.getItems();

  BOOST_REQUIRE_EQUAL(ModelItems.size(),3);

  ModelDescriptor::ModelDescription_t::iterator it;



  it = ModelItems.begin();
  BOOST_REQUIRE_EQUAL((*it)->isType(ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getVariableName(),"test.var");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getUnitClass(),"test.unitclass");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getGeneratorMethod(),GeneratorDescriptor::Fixed);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters().size(),1);

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(ModelItemDescriptor::PluggedFunction),true);
  BOOST_REQUIRE_EQUAL(((FunctionDescriptor*)(*it))->getFileID(),"test.id");
  BOOST_REQUIRE_EQUAL(((FunctionDescriptor*)(*it))->getParameters().size(),3);

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getVariableName(),"test.var2");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getUnitClass(),"test.unitclass2");
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getGeneratorMethod(),GeneratorDescriptor::Interp);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters().size(),2);

}

// =====================================================================
// =====================================================================
