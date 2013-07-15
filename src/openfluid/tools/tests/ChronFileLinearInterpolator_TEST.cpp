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
  \file MarketClient_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_chronfilelinearinterpolator
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/progress.hpp>

#include <openfluid/tools/ChronFileLinearInterpolator.hpp>
#include <openfluid/base/FrameworkException.hpp>


#include <iostream>
#include <fstream>

#include <tests-config.hpp>

// =====================================================================
// =====================================================================

bool validateException(const openfluid::base::FrameworkException& /*E*/) { return true; }

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  boost::filesystem::create_directories(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators"));

  openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/measured_ticks.dat").string(),
                                                     boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp.dat").string(),
                                                     openfluid::core::DateTime(1997,1,1,11,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),60);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  boost::filesystem::create_directories(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators"));
  {
    boost::progress_timer t;
    std::cout << "measured_ticks_interp60.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/measured_ticks.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp60.dat").string(),
                                                       openfluid::core::DateTime(1997,1,1,11,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),60);

    CFLI.runInterpolation();
  }

  {
    boost::progress_timer t;
    std::cout << "measured_ticks_interp227.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/measured_ticks.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp227.dat").string(),
                                                       openfluid::core::DateTime(1997,1,1,11,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),227,
                                                       openfluid::tools::ChronFileInterpolator::PREPROCESS_CUMULATE);

    CFLI.runInterpolation();
  }

  // wrong end date
  {
    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/measured_ticks.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp227.dat").string(),
                                                       openfluid::core::DateTime(1997,1,1,11,0,0),openfluid::core::DateTime(1998,1,1,15,30,17),227,
                                                       openfluid::tools::ChronFileInterpolator::PREPROCESS_CUMULATE);
    BOOST_REQUIRE_EXCEPTION(CFLI.runInterpolation(),openfluid::base::FrameworkException,validateException);
  }

  // wrong begin date
  {
    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/measured_ticks.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp227.dat").string(),
                                                       openfluid::core::DateTime(1997,1,1,1,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),227,
                                                       openfluid::tools::ChronFileInterpolator::PREPROCESS_CUMULATE);

    BOOST_REQUIRE_EXCEPTION(CFLI.runInterpolation(),openfluid::base::FrameworkException,validateException);
  }


  // begin date > end date
  {
    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/measured_ticks.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/measured_ticks_interp227.dat").string(),
                                                       openfluid::core::DateTime(1998,1,1,11,0,0),openfluid::core::DateTime(1997,1,1,15,30,17),227,
                                                       openfluid::tools::ChronFileInterpolator::PREPROCESS_CUMULATE);

    BOOST_REQUIRE_EXCEPTION(CFLI.runInterpolation(),openfluid::base::FrameworkException,validateException);
  }



  // real temperature file at 60sec
  {
    boost::progress_timer t;
    std::cout << "temp_interp60.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/temp.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/temp_interp60.dat").string(),
                                                       openfluid::core::DateTime(2013,1,1,12,0,0),openfluid::core::DateTime(2013,9,30,12,30,0),60);

    CFLI.runInterpolation();
  }


  // real temperature file at 1day
  {
    boost::progress_timer t;
    std::cout << "temp_interp86400.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/temp.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/temp_interp86400.dat").string(),
                                                       openfluid::core::DateTime(2013,1,1,12,0,0),openfluid::core::DateTime(2013,9,30,12,30,0),86400);

    CFLI.runInterpolation();
  }


  // real rain file at 1hour
  {
    boost::progress_timer t;
    std::cout << "rain_interp3600.dat: " << std::endl;


    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/rain.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp3600.dat").string(),
                                                       openfluid::core::DateTime(1992,6,1,0,0,0),openfluid::core::DateTime(1996,4,1,12,30,17),3600);

    CFLI.runInterpolation();
  }


  // real rain file at 1hour (short period)
  {
    boost::progress_timer t;
    std::cout << "rain_interp3600short.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/rain.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp3600short.dat").string(),
                                                       openfluid::core::DateTime(1992,6,1,0,0,0),openfluid::core::DateTime(1993,4,1,12,30,17),3600);

    CFLI.runInterpolation();
  }


  // reinterp real rain file at 1hour (short period)
  {
    boost::progress_timer t;
    std::cout << "rain_reinterp3600short.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp3600short.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_reinterp3600short.dat").string(),
                                                       openfluid::core::DateTime(1992,6,1,0,0,0),openfluid::core::DateTime(1993,4,1,12,0,0),3600);

    CFLI.runInterpolation();
  }


  // compare inter and reinterp
  {
    boost::progress_timer t;
    std::cout << "rain_interp3600short.dat vs rain_reinterp3600short.dat: " << std::endl;

    std::ifstream ifs1(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp3600short.dat").string().c_str());
    std::ifstream ifs2(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_reinterp3600short.dat").string().c_str());

    std::istream_iterator<char> b1(ifs1), e1;
    std::istream_iterator<char> b2(ifs2), e2;

    BOOST_REQUIRE_EQUAL_COLLECTIONS(b1, e1, b2, e2);
  }

  // real rain file at 60sec (short period)
  {
    boost::progress_timer t;
    std::cout << "rain_interp60veryshort.dat: " << std::endl;

    openfluid::tools::ChronFileLinearInterpolator CFLI(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/ChronFiles/rain.dat").string(),
                                                       boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/Interpolators/rain_interp60veryshort.dat").string(),
                                                       openfluid::core::DateTime(1992,7,1,0,0,0),openfluid::core::DateTime(1992,12,31,23,59,59),60);

    CFLI.runInterpolation();
  }



}



