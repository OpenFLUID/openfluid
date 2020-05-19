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
  @file ChronFileLinearInterpolator_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_chronfilelinearinterpolator


#include <iostream>
#include <fstream>
#include <iterator>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/progress.hpp>

#include <openfluid/tools/ChronFileLinearInterpolator.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/base/FrameworkException.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


bool validateException(const openfluid::base::FrameworkException& /*E*/)
{ return true; }


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::tools::Filesystem::makeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators");

  openfluid::tools::ChronFileLinearInterpolator
    CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/measured_ticks.dat",
         CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp.dat",
         openfluid::core::DateTime(1997,1,1,11,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),60);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::tools::Filesystem::makeDirectory(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators");

  {
    boost::progress_timer t;
    std::cout << "measured_ticks_interp60.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/measured_ticks.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp60.dat",
           openfluid::core::DateTime(1997,1,1,11,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),60);

    CFLI.runInterpolation();
  }

  {
    boost::progress_timer t;
    std::cout << "measured_ticks_interp227.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/measured_ticks.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp227.dat",
           openfluid::core::DateTime(1997,1,1,11,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),227,
           openfluid::tools::ChronFileInterpolator::PREPROCESS_CUMULATE);

    CFLI.runInterpolation();
  }

  // wrong end date
  {
    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/measured_ticks.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp227.dat",
           openfluid::core::DateTime(1997,1,1,11,0,0),openfluid::core::DateTime(1998,1,1,15,30,17),227,
           openfluid::tools::ChronFileInterpolator::PREPROCESS_CUMULATE);
    BOOST_REQUIRE_EXCEPTION(CFLI.runInterpolation(),openfluid::base::FrameworkException,validateException);
  }

  // wrong begin date
  {
    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/measured_ticks.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp227.dat",
           openfluid::core::DateTime(1997,1,1,1,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),227,
           openfluid::tools::ChronFileInterpolator::PREPROCESS_CUMULATE);

    BOOST_REQUIRE_EXCEPTION(CFLI.runInterpolation(),openfluid::base::FrameworkException,validateException);
  }


  // begin date > end date
  {
    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/measured_ticks.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp227.dat",
           openfluid::core::DateTime(1998,1,1,11,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),227,
           openfluid::tools::ChronFileInterpolator::PREPROCESS_CUMULATE);

    BOOST_REQUIRE_EXCEPTION(CFLI.runInterpolation(),openfluid::base::FrameworkException,validateException);
  }


  // real temperature file at 60sec
  {
    boost::progress_timer t;
    std::cout << "temp_interp60.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/temp.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/temp_interp60.dat",
           openfluid::core::DateTime(2013,1,1,12,0,0),openfluid::core::DateTime(2013,9,30,12,30,0),60);

    CFLI.runInterpolation();
  }


  // real temperature file at 1day
  {
    boost::progress_timer t;
    std::cout << "temp_interp86400.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/temp.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/temp_interp86400.dat",
           openfluid::core::DateTime(2013,1,1,12,0,0),openfluid::core::DateTime(2013,9,30,12,30,0),86400);

    CFLI.runInterpolation();
  }


  // real rain file at 1hour
  {
    boost::progress_timer t;
    std::cout << "rain_interp3600.dat: " << std::endl;


    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/rain.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp3600.dat",
           openfluid::core::DateTime(1992,6,1,0,0,0),openfluid::core::DateTime(1996,4,1,12,30,17),3600);

    CFLI.runInterpolation();
  }


  // real rain file at 1hour (short period)
  {
    boost::progress_timer t;
    std::cout << "rain_interp3600short.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/rain.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp3600short.dat",
           openfluid::core::DateTime(1992,6,1,0,0,0),openfluid::core::DateTime(1993,4,1,12,30,17),3600);

    CFLI.runInterpolation();
  }


  // reinterp real rain file at 1hour (short period)
  {
    boost::progress_timer t;
    std::cout << "rain_reinterp3600short.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp3600short.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_reinterp3600short.dat",
           openfluid::core::DateTime(1992,6,1,0,0,0),openfluid::core::DateTime(1993,4,1,12,0,0),3600);

    CFLI.runInterpolation();
  }


  // compare inter and reinterp
  {
    boost::progress_timer t;
    std::cout << "rain_interp3600short.dat vs rain_reinterp3600short.dat: " << std::endl;

    std::ifstream ifs1(std::string(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp3600short.dat").c_str());
    std::ifstream ifs2(std::string(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_reinterp3600short.dat").c_str());

    std::istream_iterator<char> b1(ifs1), e1;
    std::istream_iterator<char> b2(ifs2), e2;

    BOOST_REQUIRE_EQUAL_COLLECTIONS(b1, e1, b2, e2);
  }

  // real rain file at 60sec (short period)
  {
    boost::progress_timer t;
    std::cout << "rain_interp60veryshort.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator
      CFLI(CONFIGTESTS_INPUT_MISCDATA_DIR+"/ChronFiles/rain.dat",
           CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp60veryshort.dat",
           openfluid::core::DateTime(1992,7,1,0,0,0),openfluid::core::DateTime(1992,12,31,23,59,59),60);

    CFLI.runInterpolation();
  }

}

