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
  @file GeneratorSignature_TEST.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_GeneratorSignature


#include <boost/test/unit_test.hpp>

#include <openfluid/machine/GeneratorSignature.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_FixedGeneratorConstructor)
{
  openfluid::machine::GeneratorSignature Sign(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::FIXED,
                                              "TU","var");

  BOOST_CHECK(!Sign.ID.empty());
  BOOST_CHECK_EQUAL(Sign.HandledData.RequiredParams.size(),1);
  BOOST_CHECK_EQUAL(Sign.HandledData.UsedParams.size(),1);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_RandomGeneratorConstructor)
{
  openfluid::machine::GeneratorSignature Sign(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::RANDOM,
                                               "TU","var",openfluid::core::Value::DOUBLE, 
                                               openfluid::fluidx::DataDimensions(5));

  BOOST_CHECK(!Sign.ID.empty());
  BOOST_CHECK_EQUAL(Sign.HandledData.RequiredParams.size(),2);
  BOOST_CHECK_EQUAL(Sign.HandledData.UsedParams.size(),2);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_InterpGeneratorConstructor)
{
  openfluid::machine::GeneratorSignature Sign(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INTERP,
                                              "TU","var");

  BOOST_CHECK(!Sign.ID.empty());
  BOOST_CHECK_EQUAL(Sign.HandledData.RequiredParams.size(),2);
  BOOST_CHECK_EQUAL(Sign.HandledData.UsedParams.size(),3);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_InjectGeneratorConstructor)
{
  openfluid::machine::GeneratorSignature Sign(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECT,
                                              "TU","var");

  BOOST_CHECK(!Sign.ID.empty());
  BOOST_CHECK_EQUAL(Sign.HandledData.RequiredParams.size(),2);
  BOOST_CHECK_EQUAL(Sign.HandledData.UsedParams.size(),2);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_MultiInjectGeneratorConstructor)
{
  openfluid::machine::GeneratorSignature Sign(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::INJECTMULTICOL,
                                              "multi","TU#*:v;TU#*:w");

  BOOST_CHECK(!Sign.ID.empty());
  BOOST_CHECK_EQUAL(Sign.HandledData.RequiredParams.size(),1);
  BOOST_CHECK_EQUAL(Sign.HandledData.UsedParams.size(),0);
}
