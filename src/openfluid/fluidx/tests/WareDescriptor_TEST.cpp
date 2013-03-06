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
 \file WareDescriptor_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_modeldesc
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/fluidx/FunctionDescriptor.hpp>

#include <openfluid/fluidx/FluidXDescriptor.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_parameters)
{
  openfluid::fluidx::FunctionDescriptor FuncDesc("test1.id");
  FuncDesc.setParameter("param1.level2A.level3A", "var12A3A");
  FuncDesc.setParameter("param2", "var2");
  FuncDesc.setParameter("param3.level2A", "var32A");
  FuncDesc.setParameter("param3.level2B", "var32B");
  FuncDesc.setParameter("param1.level2B.level3", "var12B3");
  FuncDesc.setParameter("param1.level2A.level3B", "var12A3B");

  BOOST_CHECK_EQUAL(FuncDesc.getParameters().size(), 3);

  std::map<std::string, std::string> Params =
      openfluid::fluidx::WareDescriptor::getParamsAsMap(
          FuncDesc.getParameters());

  BOOST_CHECK_EQUAL(Params.size(), 6);
  BOOST_CHECK_EQUAL(Params["param1.level2A.level3A"], "var12A3A");
  BOOST_CHECK_EQUAL(Params["param2"], "var2");
  BOOST_CHECK_EQUAL(Params["param3.level2A"], "var32A");
  BOOST_CHECK_EQUAL(Params["param3.level2B"], "var32B");
  BOOST_CHECK_EQUAL(Params["param1.level2B.level3"], "var12B3");
  BOOST_CHECK_EQUAL(Params["param1.level2A.level3B"], "var12A3B");

  FuncDesc.eraseParameter("param3.level2B");
  Params = openfluid::fluidx::WareDescriptor::getParamsAsMap(
      FuncDesc.getParameters());
  BOOST_CHECK_EQUAL(Params.size(), 5);
  BOOST_CHECK_EQUAL(Params["param1.level2A.level3A"], "var12A3A");
  BOOST_CHECK_EQUAL(Params["param2"], "var2");
  BOOST_CHECK_EQUAL(Params["param3.level2A"], "var32A");
  BOOST_CHECK(!Params.count("param3.level2B"));
  BOOST_CHECK_EQUAL(Params["param1.level2B.level3"], "var12B3");
  BOOST_CHECK_EQUAL(Params["param1.level2A.level3B"], "var12A3B");

  FuncDesc.eraseParameter("param2");
  Params = openfluid::fluidx::WareDescriptor::getParamsAsMap(
      FuncDesc.getParameters());
  BOOST_CHECK_EQUAL(Params.size(), 4);
  BOOST_CHECK_EQUAL(Params["param1.level2A.level3A"], "var12A3A");
  BOOST_CHECK(!Params.count("param2"));
  BOOST_CHECK_EQUAL(Params["param3.level2A"], "var32A");
  BOOST_CHECK(!Params.count("param3.level2B"));
  BOOST_CHECK_EQUAL(Params["param1.level2B.level3"], "var12B3");
  BOOST_CHECK_EQUAL(Params["param1.level2A.level3B"], "var12A3B");

  FuncDesc.eraseParameter("param1.level2B.level3");
  Params = openfluid::fluidx::WareDescriptor::getParamsAsMap(
      FuncDesc.getParameters());

  BOOST_CHECK_EQUAL(Params.size(), 4);
  BOOST_CHECK_EQUAL(Params["param1.level2A.level3A"], "var12A3A");
  BOOST_CHECK(!Params.count("param2"));
  BOOST_CHECK_EQUAL(Params["param3.level2A"], "var32A");
  BOOST_CHECK(!Params.count("param3.level2B"));
  BOOST_CHECK(!Params.count("param1.level2B.level3"));
  BOOST_CHECK_EQUAL(Params["param1.level2B"], "");
  BOOST_CHECK_EQUAL(Params["param1.level2A.level3B"], "var12A3B");

  FuncDesc.eraseParameter("param1");
  Params = openfluid::fluidx::WareDescriptor::getParamsAsMap(
      FuncDesc.getParameters());

  BOOST_CHECK_EQUAL(Params.size(), 1);
  BOOST_CHECK(!Params.count("param1.level2A.level3A"));
  BOOST_CHECK(!Params.count("param2"));
  BOOST_CHECK_EQUAL(Params["param3.level2A"], "var32A");
  BOOST_CHECK(!Params.count("param3.level2B"));
  BOOST_CHECK(!Params.count("param1.level2B.level3"));
  BOOST_CHECK(!Params.count("param1.level2B"));
  BOOST_CHECK(!Params.count("param1.level2A.level3B"));
}
