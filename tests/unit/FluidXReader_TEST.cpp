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
#define BOOST_TEST_MODULE unittest_fluidxreader
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
#include <openfluid/io.hpp>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::io::FluidXReader FXR(openfluid::io::IOListener());
}

// =====================================================================
// =====================================================================

void TestDataset(std::string DatasetPath)
{
  openfluid::io::FluidXReader FXR = openfluid::io::FluidXReader(new openfluid::io::IOListener());

  boost::onullstream ONullStream;
  FXR.loadFromDirectory(DatasetPath);

  // Model
  // ====================================================================

  BOOST_REQUIRE_EQUAL(FXR.getModelDescriptor().getGlobalParameters().size(),2);
  BOOST_REQUIRE_EQUAL(FXR.getModelDescriptor().getGlobalParameters()["gparam1"],"100");
  BOOST_REQUIRE_EQUAL(FXR.getModelDescriptor().getGlobalParameters()["gparam2"],"0.1");



  openfluid::base::ModelDescriptor::ModelDescription_t ModelItems;

  ModelItems = FXR.getModelDescriptor().getItems();

  BOOST_REQUIRE_EQUAL(ModelItems.size(),5);

  openfluid::base::ModelDescriptor::ModelDescription_t::iterator it;

  it = ModelItems.begin();
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::base::ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getVariableName(),"tests.generator.interp");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getVariableSize(),1);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getUnitClass(),"TU");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getGeneratorMethod(),openfluid::base::GeneratorDescriptor::Interp);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters().size(),2);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters()["sources"],"sources.xml");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters()["distribution"],"distri.dat");

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::base::ModelItemDescriptor::PluggedFunction),true);
  BOOST_REQUIRE_EQUAL(((openfluid::base::FunctionDescriptor*)(*it))->getFileID(),"tests.functionA");
  BOOST_REQUIRE_EQUAL(((openfluid::base::FunctionDescriptor*)(*it))->getParameters().size(),0);

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::base::ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getVariableName(),"tests.generator.fixed");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getVariableSize(),11);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getUnitClass(),"TU");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getGeneratorMethod(),openfluid::base::GeneratorDescriptor::Fixed);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters().size(),1);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters()["fixedvalue"],"20");

  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::base::ModelItemDescriptor::Generator),true);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getVariableName(),"tests.generator.random");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getVariableSize(),1);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getUnitClass(),"TU");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getGeneratorMethod(),openfluid::base::GeneratorDescriptor::Random);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters().size(),2);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters()["min"],"20.53");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters()["max"],"50");


  it++;
  BOOST_REQUIRE_EQUAL((*it)->isType(openfluid::base::ModelItemDescriptor::PluggedFunction),true);
  BOOST_REQUIRE_EQUAL(((openfluid::base::FunctionDescriptor*)(*it))->getFileID(),"tests.functionB");
  BOOST_REQUIRE_EQUAL(((openfluid::base::FunctionDescriptor*)(*it))->getParameters().size(),4);
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters()["gparam1"],"50");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters()["strparam"],"strvalue");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters()["doubleparam"],"1.1");
  BOOST_REQUIRE_EQUAL(((openfluid::base::GeneratorDescriptor*)(*it))->getParameters()["longparam"],"11");



  // Output
  // ====================================================================


  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets().size(),5);


  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getColSeparator()," ");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getDateFormat(),"%Y %m %d %H %M %S");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getCommentChar(),"%");

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets().size(),3);

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[0].getName(),"full");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[0].getUnitsClass(),"XU");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[0].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[0].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[0].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[0].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[0].getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[0].isAllVectors(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[0].getPrecision(),5);

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].getName(),"2vars");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].getUnitsClass(),"YU");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].getScalars().size(),1);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].getScalars()[0],"var1");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].isAllScalars(),false);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].getVectors().size(),1);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].getVectors()[0],"var2");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].isAllVectors(),false);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[1].getPrecision(),3);

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].getName(),"3units");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].getUnitsClass(),"ZU");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].getUnitsIDs().size(),3);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].getUnitsIDs()[0],5);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].getUnitsIDs()[1],197);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].getUnitsIDs()[2],73);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].isAllUnits(),false);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].isAllVectors(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[0].getSets()[2].getPrecision(),5);


  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getColSeparator(),";");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getDateFormat(),"%Y%m%dT%H%M%S");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getCommentChar(),"#");

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets().size(),3);

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[0].getName(),"full");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[0].getUnitsClass(),"KU");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[0].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[0].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[0].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[0].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[0].getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[0].isAllVectors(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[0].getPrecision(),9);

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].getName(),"3vars");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].getUnitsClass(),"LU");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].getScalars().size(),2);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].getScalars()[0],"var1");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].getScalars()[1],"var5");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].isAllScalars(),false);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].getVectors().size(),1);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].getVectors()[0],"var2");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].isAllVectors(),false);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[1].getPrecision(),5);

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].getName(),"2units");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].getUnitsClass(),"MU");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].getUnitsIDs().size(),2);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].getUnitsIDs()[0],2);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].getUnitsIDs()[1],1);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].isAllUnits(),false);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].isAllVectors(),true);
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[1].getSets()[2].getPrecision(),5);


  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[2].getColSeparator(),"\t");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[2].getDateFormat(),"%Y%m%dT%H%M%S");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[2].getCommentChar(),"%");

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[3].getColSeparator(),"\t");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[3].getDateFormat(),"%Y%m%dT%H%M%S");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[3].getCommentChar(),"%");

  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[4].getColSeparator(),"\t");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[4].getDateFormat(),"%Y\t%m\t%d\t%H\t%M\t%S");
  BOOST_REQUIRE_EQUAL(FXR.getOutputDescriptor().getFileSets()[4].getCommentChar(),"%");



  // Run
  // ====================================================================


  BOOST_REQUIRE_EQUAL(FXR.getRunDescriptor().getDeltaT(),4753);
  BOOST_REQUIRE(FXR.getRunDescriptor().getBeginDate() == openfluid::core::DateTime(1997,1,2,11,15,48));
  BOOST_REQUIRE(FXR.getRunDescriptor().getEndDate() == openfluid::core::DateTime(2005,11,30,6,53,7));
  BOOST_REQUIRE_EQUAL(FXR.getRunDescriptor().isSimulationID(),true);
  BOOST_REQUIRE_EQUAL(FXR.getRunDescriptor().getSimulationID(),"testrun");
  BOOST_REQUIRE_EQUAL(FXR.getRunDescriptor().isUserValuesBufferSize(),true);
  BOOST_REQUIRE_EQUAL(FXR.getRunDescriptor().getValuesBufferSize(),100);
  BOOST_REQUIRE_EQUAL(FXR.getRunDescriptor().getFilesBufferSizeInKB(),2);



  // Domain definition
  // ====================================================================

  std::list<openfluid::base::UnitDescriptor>::iterator UnitsIt;

  BOOST_REQUIRE_EQUAL(FXR.getDomainDescriptor().getUnits().size(),14);

  UnitsIt = FXR.getDomainDescriptor().getUnits().begin();
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(),"unitsP");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(),0);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(),0);

  UnitsIt++;
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(),"unitsA");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(),3);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().first,"unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().second,11);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().front().first,"unitsP");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().front().second,1);


  UnitsIt++;
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(),"unitsA");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().first,"unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().second,2);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(),0);

  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(),"unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(),11);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().first,"unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().front().second,3);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(),1);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().front().first,"unitsP");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().front().second,1);

  UnitsIt++;
  UnitsIt++;
  UnitsIt++;
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitClass(),"unitsB");
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitID(),7);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getProcessOrder(),4);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsTos().size(),0);
  BOOST_REQUIRE_EQUAL((*UnitsIt).getUnitsParents().size(),0);



  // Domain input data
  // ====================================================================

  std::list<openfluid::base::InputDataDescriptor>::iterator IDataIt;

  BOOST_REQUIRE_EQUAL(FXR.getDomainDescriptor().getInputData().size(),3);

  IDataIt = FXR.getDomainDescriptor().getInputData() .begin();
  BOOST_REQUIRE_EQUAL((*IDataIt).getUnitsClass(),"unitsA");
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder().size(),1);
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder()[0],"indataA");
  BOOST_REQUIRE ((*IDataIt).getDataBlob().size() > 0);

  IDataIt++;
  BOOST_REQUIRE_EQUAL((*IDataIt).getUnitsClass(),"unitsB");
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder().size(),2);
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder()[0],"indataB1");
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder()[1],"indataB3");
  BOOST_REQUIRE ((*IDataIt).getDataBlob().size() > 0);


  IDataIt++;
  BOOST_REQUIRE_EQUAL((*IDataIt).getUnitsClass(),"unitsB");
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder().size(),1);
  BOOST_REQUIRE_EQUAL((*IDataIt).getColumnsOrder()[0],"indataB2");
  BOOST_REQUIRE ((*IDataIt).getDataBlob().size() > 0);


  // Domain calendar
  // ====================================================================

  std::list<openfluid::base::EventDescriptor>::iterator EventIt;

  BOOST_REQUIRE_EQUAL(FXR.getDomainDescriptor().getEvents().size(),10);

  EventIt = FXR.getDomainDescriptor().getEvents().begin();
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitClass(),"unitsA");
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitID(),1);
  BOOST_REQUIRE((*EventIt).getEvent().getDateTime() == openfluid::core::DateTime(1999,12,31,23,59,59));
  BOOST_REQUIRE_EQUAL((*EventIt).getEvent().getInfosCount(),4);
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("when","before"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("where","1"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("numeric","1.13"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("string","EADGBE"));

  EventIt++;
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitClass(),"unitsA");
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitID(),1);
  BOOST_REQUIRE((*EventIt).getEvent().getDateTime() == openfluid::core::DateTime(1999,12,1,12,0,0));
  BOOST_REQUIRE_EQUAL((*EventIt).getEvent().getInfosCount(),4);
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("when","before"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("where","1"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("numeric","1.13"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("string","EADG"));

  EventIt++;
  EventIt++;
  EventIt++;
  EventIt++;
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitClass(),"unitsB");
  BOOST_REQUIRE_EQUAL((*EventIt).getUnitID(),7);
  BOOST_REQUIRE((*EventIt).getEvent().getDateTime() == openfluid::core::DateTime(2000,1,1,2,18,12));
  BOOST_REQUIRE_EQUAL((*EventIt).getEvent().getInfosCount(),4);
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("when","during"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("where","9"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("numeric","1.15"));
  BOOST_REQUIRE((*EventIt).getEvent().isInfoEqual("string","EADGBE"));

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  std::vector<std::string> DatasetPaths;
  DatasetPaths.push_back(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXReader/manyfiles1").string());
  DatasetPaths.push_back(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXReader/singlefile1").string());
  DatasetPaths.push_back(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXReader/singlefile2").string());


  boost::unit_test::test_suite* TestSuite = BOOST_TEST_SUITE( "" );
  TestSuite->add(BOOST_PARAM_TEST_CASE(&TestDataset,DatasetPaths.begin(),DatasetPaths.end()));


  boost::unit_test::framework::run(TestSuite);
  boost::unit_test::test_results const& TestResults =  boost::unit_test::results_collector.results(TestSuite->p_id);

  boost::unit_test::unit_test_log.set_stream(std::cerr);
  BOOST_CHECK(TestResults.p_assertions_failed == 0);
  BOOST_CHECK(!TestResults.p_aborted);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_error_handling)
{
  bool HasFailed;
  boost::onullstream ONullStream;

  HasFailed = false;
  try
  {
    openfluid::io::FluidXReader(new openfluid::io::IOListener()).loadFromDirectory(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXReader/pathdoesnotexist").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    openfluid::io::FluidXReader(new openfluid::io::IOListener()).loadFromDirectory(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXReader/wrong-nofile").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    openfluid::io::FluidXReader(new openfluid::io::IOListener()).loadFromDirectory(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXReader/wrong-twomodels").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    openfluid::io::FluidXReader(new openfluid::io::IOListener()).loadFromDirectory(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.FluidXReader/wrong-runs").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);

}
