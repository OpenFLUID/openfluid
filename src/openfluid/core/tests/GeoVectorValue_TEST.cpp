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
#include <openfluid/base/OFException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>

// =====================================================================
// =====================================================================


class GeoVectorValueSub: public openfluid::core::GeoVectorValue
{
  public:

    GeoVectorValueSub(std::string PrefixPath,std::string RelativePath) :
      openfluid::core::GeoVectorValue(PrefixPath,RelativePath)
    {

    }

    OGRDataSource* getData()
    {
      return m_Data;
    }

    std::string getAbsolutePath()
    {
      return openfluid::core::GeoVectorValue::getAbsolutePath();
    }

    void tryOpeningSource()
    {
      openfluid::core::GeoVectorValue::tryOpeningSource();
    }
};

BOOST_AUTO_TEST_CASE(check_construction)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue");

  BOOST_CHECK_EQUAL(Val->getType(),openfluid::core::UnstructuredValue::GeoVectorValue);

  BOOST_CHECK_EQUAL(Val->getAbsolutePath(),(boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR) / "GeoVectorValue").string());

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"WrongDir");

  BOOST_CHECK_THROW(Val->tryOpeningSource(),openfluid::base::OFException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue");

  Val->tryOpeningSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFile)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue/wrongfile");

  BOOST_CHECK_THROW(Val->tryOpeningSource(),openfluid::base::OFException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFile_NoExtension)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue/SU");

  BOOST_CHECK_THROW(Val->tryOpeningSource(),openfluid::base::OFException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFileExtension)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue/SU.aaa");

  BOOST_CHECK_THROW(Val->tryOpeningSource(),openfluid::base::OFException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Shp)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue/SU.shp");

  Val->tryOpeningSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Dbf)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue/SU.dbf");

  Val->tryOpeningSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Shx)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue/SU.shx");

  Val->tryOpeningSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_get_CorrectDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"GeoVectorValue");

  BOOST_CHECK(Val->get());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_get_WrongDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,"WrongDir");

  BOOST_CHECK_THROW(Val->get(),openfluid::base::OFException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}
