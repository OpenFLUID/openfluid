/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputReader.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_outputreader
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>

#include "tests-config.h"
#include "OutputReader.h"

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  OutputReader Reader;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  OutputReader Reader;
  OutputDescriptor OutputDesc;

  OutputDesc = OutputReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/output.xml").string());



  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets().size(),2);



  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getColSeparator()," ");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getDateFormat(),"%Y %m %d %H %M %S");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getCommentChar(),"%");

  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets().size(),3);

  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[0].getName(),"full");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[0].getUnitsClass(),"XU");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[0].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[0].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[0].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[0].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[0].getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[0].isAllVectors(),true);

  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].getName(),"2vars");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].getUnitsClass(),"YU");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].getScalars().size(),1);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].getScalars()[0],"var1");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].isAllScalars(),false);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].getVectors().size(),1);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].getVectors()[0],"var2");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[1].isAllVectors(),false);

  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].getName(),"3units");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].getUnitsClass(),"ZU");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].getUnitsIDs().size(),3);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].getUnitsIDs()[0],5);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].getUnitsIDs()[1],197);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].getUnitsIDs()[2],73);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].isAllUnits(),false);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[0].getSets()[2].isAllVectors(),true);



  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getColSeparator(),";");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getDateFormat(),"%Y%m%dT%H%M%S");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getCommentChar(),"#");

  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets().size(),3);

  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[0].getName(),"full");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[0].getUnitsClass(),"KU");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[0].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[0].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[0].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[0].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[0].getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[0].isAllVectors(),true);

  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].getName(),"3vars");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].getUnitsClass(),"LU");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].getUnitsIDs().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].isAllUnits(),true);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].getScalars().size(),2);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].getScalars()[0],"var1");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].getScalars()[0],"var5");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].isAllScalars(),false);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].getVectors().size(),1);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].getVectors()[0],"var2");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[1].isAllVectors(),false);

  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].getName(),"2units");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].getUnitsClass(),"MU");
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].getUnitsIDs().size(),2);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].getUnitsIDs()[0],2);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].getUnitsIDs()[1],1);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].isAllUnits(),false);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].getScalars().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].isAllScalars(),true);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].getVectors().size(),0);
  BOOST_REQUIRE_EQUAL(OutputDesc.getFileSets()[1].getSets()[2].isAllVectors(),true);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_error_handling)
{
  OutputDescriptor OutputDesc;
  bool HasFailed;

  HasFailed = false;
  try
  {
    OutputDesc = OutputReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/outputdoesnotexist.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    OutputDesc = OutputReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongoutput1.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    OutputDesc = OutputReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongoutput2.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    OutputDesc = OutputReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongoutput3.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    OutputDesc = OutputReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongoutput4.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    OutputDesc = OutputReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongoutput5.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    OutputDesc = OutputReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongoutput6.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);


  HasFailed = false;
  try
  {
    OutputDesc = OutputReader().readFromFile(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckReaders/wrongoutput7.xml").string());
  }
  catch (...)
  {
    HasFailed = true;
  }
  BOOST_REQUIRE_EQUAL(HasFailed,true);

}

