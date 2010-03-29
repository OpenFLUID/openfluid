/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file Outputs_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE integrationtest_outputs
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include "tests-config.h"
#include <iostream>
#include <fstream>

#include "openfluid-tools.h"


// =====================================================================
// =====================================================================

bool CompareTextFiles(std::string FilePathA, std::string FilePathB,
                      int FromLine = -1, int ToLine = -1)
{

  std::ifstream FileReader;
  std::vector<std::string> ContentA,ContentB;
  std::string StrLine;

  //File A
  FileReader.open(FilePathA.c_str());
  if (!FileReader) return false;
  while(std::getline(FileReader,StrLine)) ContentA.push_back(StrLine);
  FileReader.close();

  //File B
  FileReader.open(FilePathB.c_str());
  if (!FileReader) return false;
  while(std::getline(FileReader,StrLine)) ContentB.push_back(StrLine);
  FileReader.close();


  if (ContentA.size() == 0 && ContentB.size() == 0) return true;

  if (FromLine < 0) FromLine = 0;
  if (ToLine < 0) ToLine = ContentA.size()-1;


  if (ToLine < FromLine) return false;
  if (ContentA.size() < FromLine || ContentB.size() < FromLine || ContentB.size() < ToLine)
    return false;

  for (unsigned int i=FromLine;i<=ToLine;i++)
  {
    if (ContentA[i] != ContentB[i]) return false;
  }

  return true;

}


// =====================================================================
// =====================================================================


bool RunSimulation(std::string InputPath, std::string OutputPath)
{

  std::string CommandToRun = boost::filesystem::path(CONFIGTESTS_DISTBIN_DIR + "/openfluid-engine -i " + InputPath + " -o " + OutputPath + " -p " + CONFIGTESTS_OUTPUT_BINARY_DIR).string();
  std::cout << CommandToRun << std::endl;

  return (system(CommandToRun.c_str()) == 0);
}


// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_comparetextfiles)
{
  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string()));

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string(),
                                   boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Variables.h").string()) == false);


  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string(),5));

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string(),18,97));

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Variables.h").string(),2,5));

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/does/not/exists").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string()) == false);

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/include/core/Unit.h").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/does/not/exists").string()) == false);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_outputsdirect)
{

  std::string InputDir = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckOutputsAtEnd";
  std::string OutputDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.CheckOutputsAtEnd";
  std::string RefDir = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.REF.CheckOutputs";

  BOOST_REQUIRE(RunSimulation(InputDir,OutputDir));


  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/messages.out").string(),
                                 boost::filesystem::path(RefDir+"/messages.out").string()));


  std::string IDStr;

  for (unsigned int i=1; i<8;i++)
  {
    openfluid::tools::ConvertValue(i,&IDStr);

    BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/UnitsA"+IDStr+"_fullA.scalars.out").string(),
                                   boost::filesystem::path(RefDir+"/UnitsA"+IDStr+"_fullA.scalars.out").string(),4));

    BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/UnitsA"+IDStr+"_fullA.vector.tests.vector.out").string(),
                                   boost::filesystem::path(RefDir+"/UnitsA"+IDStr+"_fullA.vector.tests.vector.out").string(),4));


  }

  for (unsigned int i=1; i<6;i++)
  {
    openfluid::tools::ConvertValue((i*2),&IDStr);

    BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/UnitsB"+IDStr+"_fullB.scalars.out").string(),
                                   boost::filesystem::path(RefDir+"/UnitsB"+IDStr+"_fullB.scalars.out").string(),4));

    BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/UnitsB"+IDStr+"_fullB.vector.tests.vector.out").string(),
                                   boost::filesystem::path(RefDir+"/UnitsB"+IDStr+"_fullB.vector.tests.vector.out").string(),4));


  }

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_outputsprogressive)
{

  std::string InputDir = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.IN.CheckOutputsProgressive";
  std::string OutputDir = CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.CheckOutputsProgressive";
  std::string RefDir = CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.REF.CheckOutputs";

  BOOST_REQUIRE(RunSimulation(InputDir,OutputDir));


  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/messages.out").string(),
                                 boost::filesystem::path(RefDir+"/messages.out").string()));


  std::string IDStr;

  for (unsigned int i=1; i<8;i++)
  {
    openfluid::tools::ConvertValue(i,&IDStr);

    BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/UnitsA"+IDStr+"_fullA.scalars.out").string(),
                                   boost::filesystem::path(RefDir+"/UnitsA"+IDStr+"_fullA.scalars.out").string(),4));

    BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/UnitsA"+IDStr+"_fullA.vector.tests.vector.out").string(),
                                   boost::filesystem::path(RefDir+"/UnitsA"+IDStr+"_fullA.vector.tests.vector.out").string(),4));


  }

  for (unsigned int i=1; i<6;i++)
  {
    openfluid::tools::ConvertValue((i*2),&IDStr);

    BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/UnitsB"+IDStr+"_fullB.scalars.out").string(),
                                   boost::filesystem::path(RefDir+"/UnitsB"+IDStr+"_fullB.scalars.out").string(),4));

    BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(OutputDir+"/UnitsB"+IDStr+"_fullB.vector.tests.vector.out").string(),
                                   boost::filesystem::path(RefDir+"/UnitsB"+IDStr+"_fullB.vector.tests.vector.out").string(),4));


  }

}
