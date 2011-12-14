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
  \file FuncSignature_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_funcsignature
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/base/FuncSignature.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::FunctionSignature Signature;
  openfluid::base::SignatureHandledData SignatureData;
  openfluid::base::SignatureHandledDataItem SignatureDataItem;

}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::base::FunctionSignature* Signature;

  Signature = new openfluid::base::FunctionSignature();

  DECLARE_SIGNATURE_ID("test.id");
  DECLARE_SIGNATURE_NAME("name of the test");
  DECLARE_SIGNATURE_AUTHORNAME("John Doe");
  DECLARE_SIGNATURE_AUTHOREMAIL("john.doe@foo.bar.org");
  DECLARE_SIGNATURE_DESCRIPTION("this is the description");
  DECLARE_SIGNATURE_METHOD("method");
  DECLARE_SIGNATURE_DOMAIN("domain");
  DECLARE_SIGNATURE_VERSION("4.7");
  DECLARE_SIGNATURE_STATUS(openfluid::base::BETA);

  DECLARE_FUNCTION_PARAM("param1","this is param1","m/s");
  DECLARE_FUNCTION_PARAM("param2","this is param2","m3");

  DECLARE_REQUIRED_INPUTDATA("idata1","UnitClassA","this is idata1","goals/period");
  DECLARE_REQUIRED_INPUTDATA("idata2","UnitClassB","this is idata2","");

  DECLARE_USED_INPUTDATA("idata3","UnitClassA","this is idata3","?");

  DECLARE_PRODUCED_VAR("pvar1","UnitClassA","this is pvar1","");
  DECLARE_PRODUCED_VAR("pvar2","UnitClassA","this is pvar2","m");
  DECLARE_PRODUCED_VAR("pvar1","UnitClassB","this is pvar1","?");

  DECLARE_UPDATED_VAR("pvar1","UnitClassA","this is pvar1 updated","");

  DECLARE_REQUIRED_VAR("pvar1","UnitClassA","this is rvar1","");
  DECLARE_REQUIRED_VAR("pvar2","UnitClassA","this is rvar2","m");
  DECLARE_REQUIRED_VAR("pvar1","UnitClassB","this is rvar3","?");

  DECLARE_USED_VAR("uvar1","UnitClassA","this is uvar1","s");
  DECLARE_USED_VAR("uvar2","UnitClassA","this is uvar2","s-1");

  DECLARE_REQUIRED_PREVVAR("rvar1prev","UnitClassA","this is rvar1prev","mm/h");

  DECLARE_USED_PREVVAR("uvar1prev","UnitClassA","this is uvar1prev","mm/h");

  DECLARE_USED_EVENTS("UnitClassA");
  DECLARE_USED_EVENTS("UnitClassB");

  DECLARE_REQUIRED_EXTRAFILE("reqfile.dat");
  DECLARE_USED_EXTRAFILE("usedfile.dat");

  // ----------------------------------------------------------

  BOOST_REQUIRE_EQUAL(Signature->ID,"test.id");
  BOOST_REQUIRE_EQUAL(Signature->Name,"name of the test");
  BOOST_REQUIRE_EQUAL(Signature->AuthorEmail,"john.doe@foo.bar.org");
  BOOST_REQUIRE_EQUAL(Signature->Author,"John Doe");
  BOOST_REQUIRE_EQUAL(Signature->Domain,"domain");
  BOOST_REQUIRE_EQUAL(Signature->Method,"method");
  BOOST_REQUIRE_EQUAL(Signature->Description,"this is the description");
  BOOST_REQUIRE_EQUAL(Signature->Version,"4.7");
  BOOST_REQUIRE_EQUAL(Signature->Status,openfluid::base::BETA);

  BOOST_REQUIRE_EQUAL(Signature->HandledData.FunctionParams[1].DataName,"param2");
  BOOST_REQUIRE_EQUAL(Signature->HandledData.FunctionParams[1].Description,"this is param2");
  BOOST_REQUIRE_EQUAL(Signature->HandledData.FunctionParams[1].DataUnit,"m3");

  BOOST_REQUIRE_EQUAL(Signature->HandledData.RequiredInput.size(),2);

  BOOST_REQUIRE_EQUAL(Signature->HandledData.UsedInput.size(),1);

  BOOST_REQUIRE_EQUAL(Signature->HandledData.ProducedVars.size(),3);
  BOOST_REQUIRE_EQUAL(Signature->HandledData.ProducedVars[1].DataName,"pvar2");
  BOOST_REQUIRE_EQUAL(Signature->HandledData.ProducedVars[1].UnitClass,"UnitClassA");
  BOOST_REQUIRE_EQUAL(Signature->HandledData.ProducedVars[2].UnitClass,"UnitClassB");

  BOOST_REQUIRE_EQUAL(Signature->HandledData.RequiredVars.size(),3);
  BOOST_REQUIRE_EQUAL(Signature->HandledData.RequiredVars[1].DataName,"pvar2");
  BOOST_REQUIRE_EQUAL(Signature->HandledData.RequiredVars[1].UnitClass,"UnitClassA");
  BOOST_REQUIRE_EQUAL(Signature->HandledData.RequiredVars[2].UnitClass,"UnitClassB");

  BOOST_REQUIRE_EQUAL(Signature->HandledData.UpdatedVars.size(),1);

  BOOST_REQUIRE_EQUAL(Signature->HandledData.UsedVars.size(),2);

  BOOST_REQUIRE_EQUAL(Signature->HandledData.RequiredPrevVars.size(),1);

  BOOST_REQUIRE_EQUAL(Signature->HandledData.UsedPrevVars.size(),1);

  BOOST_REQUIRE_EQUAL(Signature->HandledData.UsedEventsOnUnits.size(),2);

  BOOST_REQUIRE_EQUAL(Signature->HandledData.RequiredExtraFiles.size(),1);

  BOOST_REQUIRE_EQUAL(Signature->HandledData.UsedExtraFiles.size(),1);
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_GetVariableNameAndType)
{
  std::string ParamName;
  openfluid::core::Value::Type ParamType;

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("noneparam",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"noneparam");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::NONE);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("none.param",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"none.param");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::NONE);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("none_param",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"none_param");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::NONE);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("none-param",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"none-param");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::NONE);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("oldvectparam[]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"oldvectparam");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::VECTOR);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("oldvect.param[]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"oldvect.param");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::VECTOR);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("oldvect_param[]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"oldvect_param");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::VECTOR);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("oldvect-param[]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"oldvect-param");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::VECTOR);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("vectparam[vector]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"vectparam");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::VECTOR);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("vect.param[vector]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"vect.param");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::VECTOR);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("vect_param[vector]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"vect_param");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::VECTOR);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("vect-param[vector]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"vect-param");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::VECTOR);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("doubleparam[double]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"doubleparam");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::DOUBLE);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("intparam[integer]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"intparam");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::INTEGER);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("boolparam[boolean]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"boolparam");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::BOOLEAN);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("matrixparam[matrix]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"matrixparam");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::MATRIX);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("mapparam[map]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"mapparam");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::MAP);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("stringparam[string]",ParamName,ParamType),true);
  BOOST_CHECK_EQUAL(ParamName,"stringparam");
  BOOST_CHECK_EQUAL(ParamType,openfluid::core::Value::STRING);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("badparam[STRING]",ParamName,ParamType),false);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("badparam[badtype]",ParamName,ParamType),false);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("badparam[",ParamName,ParamType),false);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("badparam]",ParamName,ParamType),false);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("badparam[[]",ParamName,ParamType),false);

  BOOST_CHECK_EQUAL(openfluid::base::SignatureHandledTypedDataItem::getVariableNameAndType("badparam[string",ParamName,ParamType),false);
}
