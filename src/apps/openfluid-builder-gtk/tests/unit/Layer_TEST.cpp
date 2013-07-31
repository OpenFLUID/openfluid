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
 \file Layer_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __LAYER_TEST_CPP__
#define __LAYER_TEST_CPP__

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_Layer
#include <boost/test/unit_test.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include "BuilderTestHelper.hpp"
#include "Layer.hpp"

// =====================================================================
// =====================================================================

struct init_Layer
{
    init_Layer()
    {
      BuilderTestHelper::getInstance()->initGtk();
      openfluid::base::RuntimeEnvironment::getInstance()->setInputDir(
          CONFIGTESTS_INPUT_DATASETS_DIR);
    }
};

BOOST_FIXTURE_TEST_SUITE(LayerTest, init_Layer)

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_constructor_GeoVectorValue_correct)
{
  openfluid::core::UnstructuredValue* Val =
  new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue/SU.shp");

  Layer* TheLayer = new Layer(LayerType::LAYER_BASE,Val,"mymap","SU");

  delete TheLayer;
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_constructor_GeoVectorValue_wrong_file)
{
  openfluid::core::UnstructuredValue* Val =
  new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue/wrong.shp");

  BOOST_CHECK_THROW(new Layer(LayerType::LAYER_BASE,Val,"mymap","SU"),openfluid::base::Exception);
}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_constructor_GeoVectorValue_wrong_noSelfID)
{
  openfluid::core::UnstructuredValue* Val =
  new openfluid::core::GeoVectorValue(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue/SU_wrong_noSelfID.shp");

  BOOST_CHECK_THROW(new Layer(LayerType::LAYER_BASE,Val,"mymap","SU"),openfluid::base::Exception);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END()

#endif /* __LAYER_TEST_CPP__ */
