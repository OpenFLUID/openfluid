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
  \file OutputDesc_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_outputdesc
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/base/OutputDescriptor.hpp>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::OutputDescriptor OutDesc;

  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets().size(),0);

  openfluid::base::OutputFilesDescriptor OutFilesDesc;
  BOOST_REQUIRE_EQUAL(OutFilesDesc.getSets().size(),0);
  BOOST_REQUIRE_EQUAL(OutFilesDesc.getColSeparator(),"\t");
  BOOST_REQUIRE_EQUAL(OutFilesDesc.getDateFormat(),"%Y%m%dT%H%M%S");
  BOOST_REQUIRE_EQUAL(OutFilesDesc.getCommentChar(),"%");

  openfluid::base::OutputSetDescriptor OutSetDesc;
  OutSetDesc.setName("foo");
  OutSetDesc.setUnitsClass("TU");
  BOOST_REQUIRE_EQUAL(OutSetDesc.getName(),"foo");
  BOOST_REQUIRE_EQUAL(OutSetDesc.getUnitsClass(),"TU");
  BOOST_REQUIRE_EQUAL(OutSetDesc.isAllUnits(),false);
  BOOST_REQUIRE_EQUAL(OutSetDesc.getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(OutSetDesc.isAllScalars(),false);
  BOOST_REQUIRE_EQUAL(OutSetDesc.getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(OutSetDesc.isAllVectors(),false);
  BOOST_REQUIRE_EQUAL(OutSetDesc.getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(OutSetDesc.getPrecision(),5);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::OutputDescriptor OutDesc;

  openfluid::base::OutputFilesDescriptor OutFilesDesc1,OutFilesDesc2;

  openfluid::base::OutputSetDescriptor OutSetDesc11, OutSetDesc12, OutSetDesc21;


  OutSetDesc11.setName("OSD11");
  OutSetDesc11.setUnitsClass("TU1");

  OutSetDesc11.setAllUnits(true);
  OutSetDesc11.setAllScalars(true);
  OutSetDesc11.setAllVectors(true);

  OutSetDesc12.setName("OSD12");
  OutSetDesc12.setUnitsClass("TU1");

  OutSetDesc12.getUnitsIDs().push_back(13);
  OutSetDesc12.getUnitsIDs().push_back(33);
  OutSetDesc12.getUnitsIDs().push_back(53);
  OutSetDesc12.setAllScalars(true);
  OutSetDesc12.setAllVectors(true);

  OutSetDesc21.setName("OSD21");
  OutSetDesc21.setUnitsClass("TU21");

  OutSetDesc21.setAllUnits(true);
  OutSetDesc21.getScalars().push_back("scalar1");
  OutSetDesc21.getScalars().push_back("scalar5");
  OutSetDesc21.getScalars().push_back("scalar2");
  OutSetDesc21.getVectors().push_back("vector1");

  OutFilesDesc1.setDateFormat("%Y %M %D");
  OutFilesDesc1.setColSeparator(";");
  OutFilesDesc1.setCommentChar("#");
  OutFilesDesc1.getSets().push_back(OutSetDesc11);
  OutFilesDesc1.getSets().push_back(OutSetDesc12);

  OutFilesDesc2.setCommentChar(";");
  OutFilesDesc2.getSets().push_back(OutSetDesc21);

  OutDesc.getFileSets().push_back(OutFilesDesc1);
  OutDesc.getFileSets().push_back(OutFilesDesc2);


  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets().size(),2);

  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getDateFormat(),"%Y %M %D");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getColSeparator(),";");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getCommentChar(),"#");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets().size(),2);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[0].getName(),"OSD11");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[0].getUnitsClass(),"TU1");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[0].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[0].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[0].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[0].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[0].isAllVectors(),true);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[0].getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].getName(),"OSD12");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].getUnitsClass(),"TU1");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].isAllUnits(),false);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].getUnitsIDs().size(),3);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].getUnitsIDs()[0],13);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].getUnitsIDs()[1],33);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].getUnitsIDs()[2],53);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].isAllVectors(),true);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[0].getSets()[1].getVectors().size(),0);

  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getDateFormat(),"%Y%m%dT%H%M%S");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getColSeparator(),"\t");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getCommentChar(),";");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets().size(),1);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].getName(),"OSD21");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].getUnitsClass(),"TU21");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].isAllScalars(),false);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].getScalars().size(),3);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].getScalars()[0],"scalar1");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].getScalars()[1],"scalar5");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].getScalars()[2],"scalar2");
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].isAllVectors(),false);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].getVectors().size(),1);
  BOOST_REQUIRE_EQUAL(OutDesc.getFileSets()[1].getSets()[0].getVectors()[0],"vector1");

}

// =====================================================================
// =====================================================================
