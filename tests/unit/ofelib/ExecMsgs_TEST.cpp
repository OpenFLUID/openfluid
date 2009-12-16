/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ExecMsgs_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_execmsgs
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "openfluid-base.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::ExecutionMessages Messages;

  BOOST_REQUIRE_EQUAL(Messages.getWarningsCount(),0);
  BOOST_REQUIRE_EQUAL(Messages.getWarningMsgs().size(),0);
  BOOST_REQUIRE_EQUAL(Messages.isWarningFlag(),false);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::base::ExecutionMessages Messages;

  Messages.addWarning("Sender",1,"Warning message #1");
  BOOST_REQUIRE_EQUAL(Messages.getWarningsCount(),1);
  BOOST_REQUIRE_EQUAL(Messages.isWarningFlag(),true);
  Messages.resetWarningFlag();
  BOOST_REQUIRE_EQUAL(Messages.isWarningFlag(),false);
  BOOST_REQUIRE_EQUAL(Messages.getWarningsCount(),1);

  Messages.addWarning("Sender","Warning message #2");
  Messages.addWarning("Sender",std::string("Source"),1,"Warning message #3");
  BOOST_REQUIRE_EQUAL(Messages.getWarningsCount(),3);
  BOOST_REQUIRE_EQUAL(Messages.isWarningFlag(),true);
  Messages.resetWarningFlag();
  BOOST_REQUIRE_EQUAL(Messages.isWarningFlag(),false);
  BOOST_REQUIRE_EQUAL(Messages.getWarningsCount(),3);
  Messages.doMemRelease();
  BOOST_REQUIRE_EQUAL(Messages.getWarningsCount(),3);
  BOOST_REQUIRE_EQUAL(Messages.getWarningMsgs().size(),0);
  BOOST_REQUIRE_EQUAL(Messages.isWarningFlag(),false);

  Messages.addWarning("Sender","Source","Warning message #4");
  BOOST_REQUIRE_EQUAL(Messages.getWarningsCount(),4);
  BOOST_REQUIRE_EQUAL(Messages.getWarningMsgs().size(),1);
  BOOST_REQUIRE_EQUAL(Messages.isWarningFlag(),true);


}
