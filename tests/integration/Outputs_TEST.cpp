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
#include <tests-config.hpp>
#include <iostream>
#include <fstream>

#include <openfluid/tools.hpp>


// =====================================================================
// =====================================================================

bool CompareTextFiles(std::string FilePathA, std::string FilePathB,
                      int FromLine = -1, int ToLine = -1)
{

  std::ifstream FileReader;
  std::vector<std::string> ContentA,ContentB;
  std::string StrLine;

  std::cerr << "comparing " << FilePathA << " and "<< FilePathB << std::endl; std::cerr.flush();

  //File A
  FileReader.open(FilePathA.c_str());
  if (!FileReader)
  {
    std::cerr << "error : cannot read file " << FilePathA << std::endl; std::cerr.flush();
    return false;
  }
  while(std::getline(FileReader,StrLine)) ContentA.push_back(StrLine);
  FileReader.close();

  //File B
  FileReader.open(FilePathB.c_str());
  if (!FileReader)
  {
    std::cerr << "error : cannot read file " << FilePathA << std::endl; std::cerr.flush();
    return false;
  }
  while(std::getline(FileReader,StrLine)) ContentB.push_back(StrLine);
  FileReader.close();


  if (ContentA.empty() && ContentB.empty()) return true;

  if (FromLine < 0) FromLine = 0;
  if (ToLine < 0) ToLine = ContentA.size()-1;


  if (ToLine < FromLine)
  {
    std::cerr << "error : begin line is greater than end line" << std::endl; std::cerr.flush();
    return false;
  }
  if (ContentA.size() < FromLine || ContentB.size() < FromLine || ContentB.size() < ToLine)
  {
    std::cerr << "error : line counts are different" << std::endl; std::cerr.flush();
    return false;
  }

  for (unsigned int i=FromLine;i<=ToLine;i++)
  {
    if (ContentA[i] != ContentB[i])
    {
      std::cerr << "error : there are differences at line #" << (i+1) << std::endl; std::cerr.flush();
      return false;
    }
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
  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string()));

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string(),
                                   boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Variables.hp").string()) == false);


  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string(),5));

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string(),18,97));

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Variables.hpp").string(),2,5));

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/does/not/exists").string(),
                                 boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string()) == false);

  BOOST_REQUIRE(CompareTextFiles(boost::filesystem::path(CONFIGTESTS_SRC_DIR+"/src/openfluid/core/Unit.hpp").string(),
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
