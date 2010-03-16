/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file InputData_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_inputdata
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "openfluid-core.h"
#include <vector>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::InputData<openfluid::core::ScalarValue> IDataDouble;

  openfluid::core::InputData<openfluid::core::InputDataValue> IDataStr;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::InputData<openfluid::core::ScalarValue> IData;
  openfluid::core::ScalarValue Value;
  std::vector<openfluid::core::InputDataName_t> Names;

  BOOST_REQUIRE_EQUAL(IData.setValue("idata_1",2.0),true);
  BOOST_REQUIRE_EQUAL(IData.setValue("idata_2",3.2),true);
  BOOST_REQUIRE_EQUAL(IData.setValue("idata_3",4.3),true);
  BOOST_REQUIRE_EQUAL(IData.setValue("idata_3",2.0),false);

  BOOST_REQUIRE_EQUAL(IData.getValue("idata_1",&Value),true);
  BOOST_REQUIRE_CLOSE(Value,2.0,0.001);

  BOOST_REQUIRE_EQUAL(IData.getValue("idata_2",&Value),true);
  BOOST_REQUIRE_CLOSE(Value,3.2,0.001);
  BOOST_REQUIRE_EQUAL(IData.getValue("idata_3",&Value),true);
  BOOST_REQUIRE_CLOSE(Value,4.3,0.001);

  BOOST_REQUIRE_EQUAL(IData.getValue("idata_4",&Value),false);

  Names = IData.getInputDataNames();
  BOOST_REQUIRE_EQUAL(Names.size(),3);



  openfluid::core::InputData<openfluid::core::InputDataValue> IDataStr;
  openfluid::core::InputDataValue IDValue;
  openfluid::core::ScalarValue DoubleValue;

  BOOST_REQUIRE_EQUAL(IDataStr.setValue("idata_1","CODEA"),true);
  BOOST_REQUIRE_EQUAL(IDataStr.setValue("idata_3","3.5"),true);
  BOOST_REQUIRE_EQUAL(IDataStr.setValue("idata_3","CODEC"),false);

  BOOST_REQUIRE_EQUAL(IDataStr.getValue("idata_1",&IDValue),true);
  BOOST_REQUIRE_EQUAL(IDValue,"CODEA");

  BOOST_REQUIRE_EQUAL(IDataStr.getValue("idata_2",&IDValue),false);

  BOOST_REQUIRE_EQUAL(IDataStr.getValue("idata_3",&IDValue),true);
  BOOST_REQUIRE_EQUAL(IDValue,"3.5");
  BOOST_REQUIRE_EQUAL(IDataStr.getValueAsDouble("idata_3",&DoubleValue),true);
  BOOST_REQUIRE_CLOSE(DoubleValue,3.5,0.001);

  BOOST_REQUIRE_EQUAL(IDataStr.getValue("idata_4",&IDValue),false);

  Names = IDataStr.getInputDataNames();
  BOOST_REQUIRE_EQUAL(Names.size(),2);



}
