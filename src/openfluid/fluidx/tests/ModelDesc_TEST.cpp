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

#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::fluidx::SimulatorDescriptor SimDesc("test.id");

  BOOST_REQUIRE_EQUAL(SimDesc.getFileID(),"test.id");
  BOOST_REQUIRE_EQUAL(SimDesc.isType(openfluid::fluidx::ModelItemDescriptor::PluggedSimulator),true);
  BOOST_REQUIRE_EQUAL(SimDesc.getParameters().size(),0);

  openfluid::fluidx::GeneratorDescriptor GenDesc1("test.var","test.unitclass",openfluid::fluidx::GeneratorDescriptor::Fixed);

  BOOST_REQUIRE_EQUAL(GenDesc1.getVariableName(),"test.var");
  BOOST_REQUIRE_EQUAL(GenDesc1.getUnitClass(),"test.unitclass");
  BOOST_REQUIRE_EQUAL(GenDesc1.getGeneratorMethod(),openfluid::fluidx::GeneratorDescriptor::Fixed);
  BOOST_REQUIRE_EQUAL(GenDesc1.getVariableSize(),1);
  BOOST_REQUIRE_EQUAL(GenDesc1.getParameters().size(),0);

  openfluid::fluidx::GeneratorDescriptor GenDesc2("test.var2","test.unitclass2",openfluid::fluidx::GeneratorDescriptor::Interp,13);

  BOOST_REQUIRE_EQUAL(GenDesc2.getVariableName(),"test.var2");
  BOOST_REQUIRE_EQUAL(GenDesc2.getUnitClass(),"test.unitclass2");
  BOOST_REQUIRE_EQUAL(GenDesc2.getGeneratorMethod(),openfluid::fluidx::GeneratorDescriptor::Interp);
  BOOST_REQUIRE_EQUAL(GenDesc2.getVariableSize(),13);
  BOOST_REQUIRE_EQUAL(GenDesc2.getParameters().size(),0);

  openfluid::fluidx::CoupledModelDescriptor ModelDesc;

  BOOST_REQUIRE_EQUAL(ModelDesc.getItems().size(),0);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::fluidx::SimulatorDescriptor SimDesc("test.id");
  SimDesc.setParameter("param1",std::string("var1"));
  SimDesc.setParameter("param2",std::string("var2"));
  SimDesc.setParameter("param3",std::string("var3"));
  SimDesc.setParameter("param2",std::string("var22"));

  openfluid::fluidx::GeneratorDescriptor GenDesc1("test.var","test.unitclass",openfluid::fluidx::GeneratorDescriptor::Fixed,7);
  GenDesc1.setParameter("fixedvalue",std::string("20.5"));

  openfluid::fluidx::GeneratorDescriptor GenDesc2("test.var2","test.unitclass2",openfluid::fluidx::GeneratorDescriptor::Interp);
  GenDesc2.setParameter("sources",std::string("datasources.xml"));
  GenDesc2.setParameter("distribution",std::string("distribution.dat"));

  openfluid::fluidx::CoupledModelDescriptor ModelDesc;

  ModelDesc.appendItem(&GenDesc1);
  ModelDesc.appendItem(&SimDesc);
  ModelDesc.appendItem(&GenDesc2);

  BOOST_REQUIRE_EQUAL(ModelDesc.getItems().size(),3);

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t ModelItems;

  ModelItems = ModelDesc.getItems();

  BOOST_REQUIRE_EQUAL(ModelItems.size(),3);

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t::iterator it;



  it = ModelItems.begin();
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),"test.var");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitClass(),"test.unitclass");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod(),openfluid::fluidx::GeneratorDescriptor::Fixed);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(),7);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->isScalarVariable(),false);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->isVectorVariable(),true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),1);

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::fluidx::ModelItemDescriptor::PluggedSimulator),true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::SimulatorDescriptor*)(*it))->getFileID(),"test.id");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::SimulatorDescriptor*)(*it))->getParameters().size(),3);

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableName(),"test.var2");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getUnitClass(),"test.unitclass2");
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getGeneratorMethod(),openfluid::fluidx::GeneratorDescriptor::Interp);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getVariableSize(),1);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->isScalarVariable(),true);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->isVectorVariable(),false);
  BOOST_REQUIRE_EQUAL(((openfluid::fluidx::GeneratorDescriptor*)(*it))->getParameters().size(),2);

}

// =====================================================================
// =====================================================================
