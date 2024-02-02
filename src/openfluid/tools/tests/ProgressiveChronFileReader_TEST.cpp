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
  @file ProgressiveChronFileReader_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_progressivechronfilereader


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/tools/ProgressiveChronFileReader.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::tools::ProgressiveColumnFileReader PColumnFR(CONFIGTESTS_INPUT_MISCDATA_DIR+
                                                          "/ChronFiles/measured_ticks.dat");
  openfluid::tools::ProgressiveChronFileReader<double> PChronFR(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/temp.dat");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::tools::ProgressiveColumnFileReader PColumnFR(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/temp.dat");

  std::string TmpStr;
  std::vector<std::string> TmpVectStr;

  while (PColumnFR.getNextLine(TmpStr))
  {
    std::cout << TmpStr << std::endl;
  }

  PColumnFR.reset();

  while (PColumnFR.getNextLine(TmpVectStr))
  {
    std::cout << "[";
    for (unsigned int i =0; i< TmpVectStr.size();i++)
    {
      std::cout << TmpVectStr[i] << "|";
    }
    std::cout << std::endl;
  }


  // ---------------------------------------------------------------------


  openfluid::tools::ChronItem_t<double> CI;

  openfluid::tools::ProgressiveChronFileReader<double> PChronFR(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/temp.dat");

  while (PChronFR.getNextValue(CI))
  {
    std::cout << CI.second << " at " << CI.first.getAsISOString() << std::endl;
  }
}

