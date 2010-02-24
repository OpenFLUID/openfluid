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
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningsCount(),0);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningMsgs().size(),0);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->isWarningFlag(),false);

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  openfluid::base::ExecutionMessages::getInstance()->addWarning("Sender",1,"Warning message #1");
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningsCount(),1);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->isWarningFlag(),true);
  openfluid::base::ExecutionMessages::getInstance()->resetWarningFlag();
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->isWarningFlag(),false);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningsCount(),1);

  openfluid::base::ExecutionMessages::getInstance()->addWarning("Sender","Warning message #2");
  openfluid::base::ExecutionMessages::getInstance()->addWarning("Sender",std::string("Source"),1,"Warning message #3");
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningsCount(),3);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->isWarningFlag(),true);
  openfluid::base::ExecutionMessages::getInstance()->resetWarningFlag();
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->isWarningFlag(),false);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningsCount(),3);
  openfluid::base::ExecutionMessages::getInstance()->doMemRelease();
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningsCount(),3);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningMsgs().size(),0);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->isWarningFlag(),false);

  openfluid::base::ExecutionMessages::getInstance()->addWarning("Sender","Source","Warning message #4");
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningsCount(),4);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->getWarningMsgs().size(),1);
  BOOST_REQUIRE_EQUAL(openfluid::base::ExecutionMessages::getInstance()->isWarningFlag(),true);


}
