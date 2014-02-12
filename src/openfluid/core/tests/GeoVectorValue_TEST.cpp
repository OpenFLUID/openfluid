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
 \file GeoVectorValue_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_geovectorvalue
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>

// =====================================================================
// =====================================================================

class GeoVectorValueSub: public openfluid::core::GeoVectorValue
{
  public:

    GeoVectorValueSub(std::string PrefixPath, std::string RelativePath) :
        openfluid::core::GeoVectorValue(PrefixPath, RelativePath)
    {

    }

    OGRDataSource* getData()
    {
      return mp_Data;
    }

    std::string getAbsolutePath()
    {
      return computeAbsolutePath(m_FilePath, m_FileName);
    }

    void tryToOpenSource()
    {
      openfluid::core::GeoVectorValue::tryToOpenSource();
    }
};

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoVectorValue");

  BOOST_CHECK_EQUAL(Val->getType(),
                    openfluid::core::UnstructuredValue::GeoVectorValue);

  BOOST_CHECK_EQUAL(
      Val->getAbsolutePath(),
      boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/GeoVectorValue").string());

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "WrongDir");

  BOOST_CHECK_THROW(Val->tryToOpenSource(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoVectorValue");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFile)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoVectorValue/wrongfile");

  BOOST_CHECK_THROW(Val->tryToOpenSource(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFile_NoExtension)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoVectorValue/SU");

  BOOST_CHECK_THROW(Val->tryToOpenSource(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFileExtension)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoVectorValue/SU.aaa");

  BOOST_CHECK_THROW(Val->tryToOpenSource(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Shp)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoVectorValue/SU.shp");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Dbf)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoVectorValue/SU.dbf");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Shx)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoVectorValue/SU.shx");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_get_CorrectDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoVectorValue");

  BOOST_CHECK(Val->get());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_get_WrongDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "WrongDir");

  BOOST_CHECK_THROW(Val->get(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_Properties)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(
      CONFIGTESTS_INPUT_DATASETS_DIR, "GeoVectorValue/SU.shp");

  BOOST_CHECK(!Val->isLineType());
  BOOST_CHECK(Val->isPolygonType());

  BOOST_CHECK(Val->containsField("OFLD_ID"));
  BOOST_CHECK(!Val->containsField("wrongField"));

  BOOST_CHECK_EQUAL(Val->getFieldIndex("OFLD_ID"), 9);

  delete Val;
}

// =====================================================================
// =====================================================================
