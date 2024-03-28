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
  @file Dimensions_TEST.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_dimensions


#include <boost/test/unit_test.hpp>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/Dimensions.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  {
    openfluid::core::Dimensions D;
    BOOST_CHECK(D.isScalar());
    BOOST_CHECK_EQUAL(D.getSerializedVariableSize(), "1");
  }

  {
    openfluid::core::Dimensions D(4);
    BOOST_CHECK(D.isVector());
    BOOST_CHECK_EQUAL(D.getSerializedVariableSize(), "[4]");
  }

  {
    openfluid::core::Dimensions D(4,1);
    BOOST_CHECK(D.isMatrix());
    BOOST_CHECK_EQUAL(D.getSerializedVariableSize(), "[4,1]");
  }

  {
    openfluid::core::Dimensions D("[1]");
    BOOST_CHECK(D.isVector());
    BOOST_CHECK_EQUAL(D.getSerializedVariableSize(), "[1]");
  }

  {
    openfluid::core::Dimensions D("[1,100]");
    BOOST_CHECK(D.isMatrix());
    BOOST_CHECK_EQUAL(D.getSerializedVariableSize(), "[1,100]");
  }

  {
    BOOST_CHECK_THROW(openfluid::core::Dimensions D("a"), openfluid::base::FrameworkException);
  }

  {
    BOOST_CHECK_THROW(openfluid::core::Dimensions D("[1;100]"), openfluid::base::FrameworkException);
  }
}
