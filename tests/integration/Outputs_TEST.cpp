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
#define BOOST_TEST_MODULE integrationtest_outputresults
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include "tests-config.h"
#include <iostream>
#include <fstream>


// =====================================================================
// =====================================================================

bool CompareTextFiles(std::string FilePathA, std::string FilePathB,
                      int FromLine = -1, int ToLine = -1)
{

  //std::cout << FilePathA << "     " << FilePathB << std::endl;

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

  //std::cout << "READ!!" << std::endl;


  if (ContentA.size() == 0 && ContentB.size() == 0) return true;

  if (FromLine < 0) FromLine = 0;
  if (ToLine < 0) ToLine = ContentA.size()-1;

  //std::cout << FL << " -> " << TL << std::endl;

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

BOOST_AUTO_TEST_CASE(check_normaloutput)
{
  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.CheckMessages/messages.out").string(),
                                 boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.OUT.CheckMessages/messages.out").string()));



  BOOST_FAIL("under construction");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_progressiveoutput)
{
  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/OPENFLUID.OUT.CheckManyMessages/messages.out").string(),
                                 boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/OPENFLUID.OUT.CheckManyMessages/messages.out").string()));


  BOOST_FAIL("under construction");
}
