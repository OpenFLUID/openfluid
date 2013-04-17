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
  \file SimStatus_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_simstatus
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/base/SimulationStatus.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/core/TypeDefs.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2000,1,1,0,0,0),
                                    openfluid::core::DateTime(2001,1,1,0,0,0),
                                    60);

  BOOST_REQUIRE_EQUAL(SimStatus.getCurrentStage(),openfluid::base::SimulationStatus::PRE);
  BOOST_REQUIRE_EQUAL(SimStatus.getCurrentTimeIndex(),0);
  BOOST_REQUIRE_EQUAL(SimStatus.isFirstTimeIndex(),true);

  BOOST_REQUIRE(SimStatus.getBeginDate() == openfluid::core::DateTime(2000,1,1,0,0,0));
  BOOST_REQUIRE(SimStatus.getEndDate() == openfluid::core::DateTime(2001,1,1,0,0,0));
  BOOST_REQUIRE_EQUAL(SimStatus.getDefaultDeltaT(),60);

  BOOST_REQUIRE(SimStatus.getCurrentDate() == openfluid::core::DateTime(2000,1,1,0,0,0));

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2000,1,1,0,0,0),
                                      openfluid::core::DateTime(2000,3,1,0,0,0),
                                      147);

  openfluid::core::DateTime PrevTime;


  SimStatus.setCurrentStage(openfluid::base::SimulationStatus::INITIALIZERUN);
  BOOST_REQUIRE_EQUAL(SimStatus.getCurrentStage(),openfluid::base::SimulationStatus::INITIALIZERUN);

  SimStatus.setCurrentStage(openfluid::base::SimulationStatus::RUNSTEP);

  bool Continue = true;

  do
  {
    try
    {
      SimStatus.setCurrentTimeIndex(SimStatus.getCurrentTimeIndex()+SimStatus.getDefaultDeltaT());
      std::cout << SimStatus.getCurrentTimeIndex() << std::endl;
    }
    catch (...)
    {
      Continue = false;
    }

  } while (Continue);

  SimStatus = openfluid::base::SimulationStatus(openfluid::core::DateTime(2000,1,1,0,0,0),
                        openfluid::core::DateTime(2001,1,1,0,0,0),
                        76);

  SimStatus.setCurrentStage(openfluid::base::SimulationStatus::PREPAREDATA);

  BOOST_REQUIRE_THROW(SimStatus.setCurrentTimeIndex(SimStatus.getCurrentTimeIndex()+SimStatus.getDefaultDeltaT()),
                      openfluid::base::OFException);

  SimStatus.setCurrentStage(openfluid::base::SimulationStatus::RUNSTEP);

  BOOST_REQUIRE_NO_THROW(SimStatus.setCurrentTimeIndex(SimStatus.getCurrentTimeIndex()+SimStatus.getDefaultDeltaT()));
}

// =====================================================================
// =====================================================================
