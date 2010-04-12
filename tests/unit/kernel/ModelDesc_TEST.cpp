/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


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
  BOOST_REQUIRE_EQUAL(GenDesc1.getVariableSize(),1);
  BOOST_REQUIRE_EQUAL(GenDesc1.getParameters().size(),0);

  GeneratorDescriptor GenDesc2("test.var2","test.unitclass2",GeneratorDescriptor::Interp,13);

  BOOST_REQUIRE_EQUAL(GenDesc2.getVariableName(),"test.var2");
  BOOST_REQUIRE_EQUAL(GenDesc2.getUnitClass(),"test.unitclass2");
  BOOST_REQUIRE_EQUAL(GenDesc2.getGeneratorMethod(),GeneratorDescriptor::Interp);
  BOOST_REQUIRE_EQUAL(GenDesc2.getVariableSize(),13);
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

  GeneratorDescriptor GenDesc1("test.var","test.unitclass",GeneratorDescriptor::Fixed,7);
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
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getVariableSize(),7);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->isScalarVariable(),false);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->isVectorVariable(),true);
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
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getVariableSize(),1);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->isScalarVariable(),true);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->isVectorVariable(),false);
  BOOST_REQUIRE_EQUAL(((GeneratorDescriptor*)(*it))->getParameters().size(),2);

}

// =====================================================================
// =====================================================================
