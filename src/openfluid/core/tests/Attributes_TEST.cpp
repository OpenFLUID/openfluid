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
  \file Attributes_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_attributes
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <openfluid/core/Attributes.hpp>
#include <vector>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/NullValue.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::Attributes Attrs;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::Attributes Attrs;
  double TheDouble;
  long TheLong;
  std::string TheString;
  openfluid::core::StringValue TheStringVal;
  openfluid::core::VectorValue TheVect;
  std::vector<openfluid::core::AttributeName_t> Names;


  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_dbl",openfluid::core::DoubleValue(2.0)),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_dbl_str","2.0"),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_int",openfluid::core::IntegerValue(3)),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_int_str","3"),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_str1",openfluid::core::StringValue("4.3")),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_strA","CODEA"),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_bool",openfluid::core::BooleanValue(true)),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_bool_str","true"),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_bool_1","1"),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_bool_str1",openfluid::core::StringValue("1")),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_null",openfluid::core::NullValue()),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_null_str","null"),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_vect",openfluid::core::VectorValue(3,1.1)),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_vect_str","1.1;1.1;1.1"),true);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_dbl",openfluid::core::DoubleValue(2.0)),false);
  BOOST_REQUIRE_EQUAL(Attrs.setValue("attr_dbl","2.0"),false);


  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_dbl",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"2");
  TheString = "";
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_dbl_str",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"2.0");

  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_dbl",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"2");
  TheStringVal.set("");
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_dbl_str",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"2.0");

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_dbl",TheDouble),true);
  BOOST_REQUIRE_CLOSE(TheDouble,2.0,0.001);
  TheDouble = 0.0;
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_dbl_str",TheDouble),true);
  BOOST_REQUIRE_CLOSE(TheDouble,2.0,0.001);

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_dbl",TheLong),true);
  BOOST_REQUIRE_EQUAL(TheLong,2);
  TheLong = 0;
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_dbl_str",TheLong),false);



  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_int",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"3");
  TheString = "";
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_int_str",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"3");

  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_int",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"3");
  TheStringVal.set("");
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_int_str",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"3");

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_int",TheDouble),true);
  BOOST_REQUIRE_CLOSE(TheDouble,3.0,0.001);
  TheDouble = 0.0;
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_int_str",TheDouble),true);
  BOOST_REQUIRE_CLOSE(TheDouble,3.0,0.001);

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_int",TheLong),true);
  BOOST_REQUIRE_EQUAL(TheLong,3);
  TheLong = 0;
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_int_str",TheLong),true);
  BOOST_REQUIRE_EQUAL(TheLong,3);



  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_str1",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"4.3");
  TheString = "";
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_strA",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"CODEA");

  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_str1",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"4.3");
  TheStringVal.set("");
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_strA",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"CODEA");

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_str1",TheDouble),true);
  BOOST_REQUIRE_CLOSE(TheDouble,4.3,0.001);
  TheDouble = 0.0;
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_strA",TheDouble),false);

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_str1",TheLong),false);
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_strA",TheLong),false);



  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_bool",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"1");
  TheString = "";
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_bool_str",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"true");
  TheString = "";
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_bool_1",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"1");
  TheString = "";
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_bool_str1",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"1");

  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_bool",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"1");
  TheStringVal.set("");
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_bool_str",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"true");
  TheStringVal.set("");
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_bool_1",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"1");
  TheStringVal.set("");
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_bool_str1",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"1");

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_bool",TheDouble),true);
  BOOST_REQUIRE_CLOSE(TheDouble,1.0,0.001);
  TheDouble = 0.0;
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_bool_str",TheDouble),false);
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_bool_1",TheDouble),true);
  BOOST_REQUIRE_CLOSE(TheDouble,1.0,0.001);
  TheDouble = 0.0;
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_bool_str1",TheDouble),true);
  BOOST_REQUIRE_CLOSE(TheDouble,1.0,0.001);

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_bool",TheLong),true);
  BOOST_REQUIRE_EQUAL(TheLong,1);
  TheLong = 0;
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_bool_str",TheLong),false);
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_bool_1",TheLong),true);
  BOOST_REQUIRE_EQUAL(TheLong,1);
  TheLong = 0;
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_bool_str1",TheLong),true);
  BOOST_REQUIRE_EQUAL(TheLong,1);


  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_null",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"null");
  TheString = "";
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_null_str",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"null");

  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_null",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"null");
  TheStringVal.set("");
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_null_str",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"null");

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_null",TheDouble),false);
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_null_str",TheDouble),false);

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_null",TheLong),false);
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_null_str",TheLong),false);



  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_vect",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"1.1;1.1;1.1");
  TheString = "";
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_vect_str",TheString),true);
  BOOST_REQUIRE_EQUAL(TheString,"1.1;1.1;1.1");

  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_vect",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"1.1;1.1;1.1");
  BOOST_REQUIRE_EQUAL(TheStringVal.toVectorValue(";",TheVect), true);
  BOOST_REQUIRE_EQUAL(TheVect.size(),3);
  BOOST_REQUIRE_EQUAL(TheVect[0],1.1);
  BOOST_REQUIRE_EQUAL(TheVect[2],1.1);
  TheVect.clear();
  TheStringVal.set("");
  BOOST_REQUIRE_EQUAL(Attrs.getValue("attr_vect_str",TheStringVal),true);
  BOOST_REQUIRE_EQUAL(TheStringVal.get(),"1.1;1.1;1.1");
  BOOST_REQUIRE_EQUAL(TheStringVal.toVectorValue(";",TheVect), true);
  BOOST_REQUIRE_EQUAL(TheVect.size(),3);
  BOOST_REQUIRE_EQUAL(TheVect[0],1.1);
  BOOST_REQUIRE_EQUAL(TheVect[2],1.1);

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_vect",TheDouble),false);
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsDouble("attr_vect_str",TheDouble),false);

  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_vect",TheLong),false);
  BOOST_REQUIRE_EQUAL(Attrs.getValueAsLong("attr_vect_str",TheLong),false);



  Names = Attrs.getAttributesNames();
  BOOST_REQUIRE_EQUAL(Names.size(),14);
}
