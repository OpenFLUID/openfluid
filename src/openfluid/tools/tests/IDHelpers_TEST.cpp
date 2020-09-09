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
  @file IDHelpers_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_idhelpers


#include <boost/test/unit_test.hpp>

#include <openfluid/tools/IDHelpers.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_names_operations)
{
  BOOST_REQUIRE(openfluid::tools::isValidAlphaNumName("a"));
  BOOST_REQUIRE(openfluid::tools::isValidAlphaNumName("abc"));
  BOOST_REQUIRE(openfluid::tools::isValidAlphaNumName("ABC"));
  BOOST_REQUIRE(openfluid::tools::isValidAlphaNumName("abc9def"));
  BOOST_REQUIRE(openfluid::tools::isValidAlphaNumName("abc8def8ghi"));
  BOOST_REQUIRE(openfluid::tools::isValidAlphaNumName("5abcDEFghi"));

  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName(""));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("_abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("-abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName(".abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abc def.ghi"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abcdefghi()"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abcdefghi{}"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abc789ghi]["));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abcdefghi!"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abcdefghi?"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abcdefghi*"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abc356ghi,"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abc22ghi;"));
  BOOST_REQUIRE(!openfluid::tools::isValidAlphaNumName("abcdefghi "));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_wares_operations)
{
  BOOST_REQUIRE(openfluid::tools::isValidWareID("a"));
  BOOST_REQUIRE(openfluid::tools::isValidWareID("abc"));
  BOOST_REQUIRE(openfluid::tools::isValidWareID("ABC"));
  BOOST_REQUIRE(openfluid::tools::isValidWareID("abc.def"));
  BOOST_REQUIRE(openfluid::tools::isValidWareID("abc.def.ghi"));
  BOOST_REQUIRE(openfluid::tools::isValidWareID("abc.def.ghi-xyz"));
  BOOST_REQUIRE(openfluid::tools::isValidWareID("abc_def.ghi-xyz"));
  BOOST_REQUIRE(openfluid::tools::isValidWareID("abc.d3f.gh1"));
  BOOST_REQUIRE(openfluid::tools::isValidWareID("abc.dEf.gh1"));

  BOOST_REQUIRE(!openfluid::tools::isValidWareID(""));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("_abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("-abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID(".abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc def.ghi"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi()"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi{}"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi]["));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi!"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi?"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi*"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi,"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi;"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi+"));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.def.ghi "));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("abc.dèf.ghi "));
  BOOST_REQUIRE(!openfluid::tools::isValidWareID("àbc.def.ghi "));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_attributes_operations)
{
  BOOST_REQUIRE(openfluid::tools::isValidAttributeName("a"));
  BOOST_REQUIRE(openfluid::tools::isValidAttributeName("abc"));
  BOOST_REQUIRE(openfluid::tools::isValidAttributeName("ABC"));
  BOOST_REQUIRE(openfluid::tools::isValidAttributeName("abc.def"));
  BOOST_REQUIRE(openfluid::tools::isValidAttributeName("abc.def.ghi"));
  BOOST_REQUIRE(openfluid::tools::isValidAttributeName("abc.def.ghi-xyz"));
  BOOST_REQUIRE(openfluid::tools::isValidAttributeName("abc_def"));
  BOOST_REQUIRE(openfluid::tools::isValidAttributeName("AbCdEf76"));
  BOOST_REQUIRE(openfluid::tools::isValidAttributeName("78541"));

  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName(""));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("_abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("-abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName(".abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("abc d"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("abcdé"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("abc?d"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("abc*d"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("abc+d"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("abc(d"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("abc}d"));
  BOOST_REQUIRE(!openfluid::tools::isValidAttributeName("abc[d"));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_variables_operations)
{

  // not typed variables
  BOOST_REQUIRE(openfluid::tools::isValidVariableName("a"));
  BOOST_REQUIRE(openfluid::tools::isValidVariableName("abc"));
  BOOST_REQUIRE(openfluid::tools::isValidVariableName("ABC"));
  BOOST_REQUIRE(openfluid::tools::isValidVariableName("abc.def"));
  BOOST_REQUIRE(openfluid::tools::isValidVariableName("abc.def.ghi"));
  BOOST_REQUIRE(openfluid::tools::isValidVariableName("abc.def.ghi-xyz"));
  BOOST_REQUIRE(openfluid::tools::isValidVariableName("abc_def.ghi-xyz"));
  BOOST_REQUIRE(openfluid::tools::isValidVariableName("abc.d3f.gh1"));
  BOOST_REQUIRE(openfluid::tools::isValidVariableName("abc.dEf.gh1"));

  BOOST_REQUIRE(!openfluid::tools::isValidVariableName(""));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("_abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("-abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName(".abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc def.ghi"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc.def.ghi()"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc.def.ghi{}"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc.def.ghi]["));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc.def.ghi!"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc.def.ghi?"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc.def.ghi*"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc.def.ghi+"));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc.def.ghi "));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("abc.dèf.ghi "));
  BOOST_REQUIRE(!openfluid::tools::isValidVariableName("àbc.def.ghi "));


  // typed variables
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("ABC"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi-xyz"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc_def.ghi-xyz"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.d3f.gh1"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.dEf.gh1"));

  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi[]"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi[double]"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi[integer]"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi[boolean]"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi[string]"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi[vector]"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi[map]"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi[matrix]"));
  BOOST_REQUIRE(openfluid::tools::isValidTypedVariableName("abc.def.ghi[null]"));

  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName(""));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("_abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("-abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName(".abc"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc def.ghi"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi()"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi{}"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi]["));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi!"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi?"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi*"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi+"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi "));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.dèf.ghi "));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("àbc.def.ghi "));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi[foo]"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi[matrix"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi[matrix][double]"));
  BOOST_REQUIRE(!openfluid::tools::isValidTypedVariableName("abc.def.ghi[matrix][matrix]"));


  openfluid::core::VariableName_t VarName;
  openfluid::core::Value::Type VarType;

  BOOST_REQUIRE(openfluid::tools::extractVariableNameAndType("abc.def.ghi",VarName,VarType));
  BOOST_REQUIRE_EQUAL(VarName,"abc.def.ghi");
  BOOST_REQUIRE_EQUAL(VarType,openfluid::core::Value::NONE);

  BOOST_REQUIRE(openfluid::tools::extractVariableNameAndType("abc.def.ghi[double]",VarName,VarType));
  BOOST_REQUIRE_EQUAL(VarName,"abc.def.ghi");
  BOOST_REQUIRE_EQUAL(VarType,openfluid::core::Value::DOUBLE);

  BOOST_REQUIRE(openfluid::tools::extractVariableNameAndType("abc.def.ghi[integer]",VarName,VarType));
  BOOST_REQUIRE_EQUAL(VarName,"abc.def.ghi");
  BOOST_REQUIRE_EQUAL(VarType,openfluid::core::Value::INTEGER);

  BOOST_REQUIRE(openfluid::tools::extractVariableNameAndType("abc.def.ghi[boolean]",VarName,VarType));
  BOOST_REQUIRE_EQUAL(VarName,"abc.def.ghi");
  BOOST_REQUIRE_EQUAL(VarType,openfluid::core::Value::BOOLEAN);

  BOOST_REQUIRE(openfluid::tools::extractVariableNameAndType("abc.def.ghi[]",VarName,VarType));
  BOOST_REQUIRE_EQUAL(VarName,"abc.def.ghi");
  BOOST_REQUIRE_EQUAL(VarType,openfluid::core::Value::VECTOR);

  BOOST_REQUIRE(openfluid::tools::extractVariableNameAndType("abc.def.ghi[vector]",VarName,VarType));
  BOOST_REQUIRE_EQUAL(VarName,"abc.def.ghi");
  BOOST_REQUIRE_EQUAL(VarType,openfluid::core::Value::VECTOR);

  BOOST_REQUIRE(openfluid::tools::extractVariableNameAndType("abc.def.ghi[matrix]",VarName,VarType));
  BOOST_REQUIRE_EQUAL(VarName,"abc.def.ghi");
  BOOST_REQUIRE_EQUAL(VarType,openfluid::core::Value::MATRIX);

  BOOST_REQUIRE(openfluid::tools::extractVariableNameAndType("abc.def.ghi[map]",VarName,VarType));
  BOOST_REQUIRE_EQUAL(VarName,"abc.def.ghi");
  BOOST_REQUIRE_EQUAL(VarType,openfluid::core::Value::MAP);

  BOOST_REQUIRE(openfluid::tools::extractVariableNameAndType("abc.def.ghi[null]",VarName,VarType));
  BOOST_REQUIRE_EQUAL(VarName,"abc.def.ghi");
  BOOST_REQUIRE_EQUAL(VarType,openfluid::core::Value::NULLL);
}
