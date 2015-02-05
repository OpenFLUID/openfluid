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
  @file PluggableWare_TEST.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_pluggableware
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/ware/PluggableWare.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_isWellFormated)
{
  BOOST_CHECK(openfluid::ware::PluggableWare::isWellFormated("param1"));
  BOOST_CHECK(openfluid::ware::PluggableWare::isWellFormated("param1.level2"));
  BOOST_CHECK(openfluid::ware::PluggableWare::isWellFormated("param1.level2.level3"));

  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("."));

  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("param1."));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("param1.level2."));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("param1.level2.level3."));

  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated(".param1"));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated(".param1.level2"));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated(".param1.level2.level3"));

  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated(".."));

  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated(".param1."));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated(".param1.level2."));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated(".param1.level2.level3."));

  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("..."));

  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("..param1"));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("..param1.level2"));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("..param1.level2.level3"));

  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("param1.."));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("param1.level2.."));
  BOOST_CHECK(!openfluid::ware::PluggableWare::isWellFormated("param1.level2.level3.."));
}

// =====================================================================
// =====================================================================

