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
 \file BuilderClassListStore_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_BuilderClassListStore
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "BuilderClassListStore.hpp"

// =====================================================================
// =====================================================================

struct init_Store
{
    Glib::RefPtr<BuilderClassListStore> mref_ClassListStore;

    init_Store()
    {
      BuilderTestHelper::getInstance()->initGtk();
      mref_ClassListStore = BuilderClassListStore::create();
    }

    ~init_Store()
    {
    }
};

BOOST_FIXTURE_TEST_SUITE(BuilderClassListStoreTest, init_Store)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_constructor)
{
  BOOST_CHECK_EQUAL(mref_ClassListStore->isEmpty(),true);
}

BOOST_AUTO_TEST_CASE(test_setClasses)
{
  std::vector<std::string> ClassList;
  ClassList.push_back("class A");
  ClassList.push_back("class B");
  ClassList.push_back("class C");

  mref_ClassListStore->setClasses(ClassList);

  BOOST_CHECK_EQUAL(mref_ClassListStore->isEmpty(),false);
  BOOST_CHECK_EQUAL(mref_ClassListStore->children().size(),3);
}
//
//BOOST_AUTO_TEST_CASE(test_classExists)
//{
//  std::vector<std::string> ClassList;
//  ClassList.push_back("class A");
//  ClassList.push_back("class B");
//  ClassList.push_back("class C");
//
//  mref_ClassListStore->setClasses(ClassList);
//
//  BOOST_CHECK_EQUAL(mref_ClassListStore->classExists("class A"),true);
//  BOOST_CHECK_EQUAL(mref_ClassListStore->classExists("class B"),true);
//  BOOST_CHECK_EQUAL(mref_ClassListStore->classExists("class C"),true);
//  BOOST_CHECK_EQUAL(mref_ClassListStore->classExists("class D"),false);
//}

//BOOST_AUTO_TEST_CASE(test_getPath)
//{
//  std::vector<std::string> ClassList;
//  ClassList.push_back("class A");
//  ClassList.push_back("class B");
//  ClassList.push_back("class C");
//
//  mref_ClassListStore->setClasses(ClassList);
//
//  BOOST_CHECK_EQUAL(mref_ClassListStore->getPathFromClassName("class A").to_string(),"0");
//  BOOST_CHECK_EQUAL(mref_ClassListStore->getPathFromClassName("class B").to_string(),"1");
//  BOOST_CHECK_EQUAL(mref_ClassListStore->getPathFromClassName("class C").to_string(),"2");
//  BOOST_CHECK_EQUAL(mref_ClassListStore->getPathFromClassName("class D"),0);
//}

BOOST_AUTO_TEST_CASE(test_getClassNameFromIter)
{
  std::vector<std::string> ClassList;
  ClassList.push_back("class A");
  ClassList.push_back("class B");
  ClassList.push_back("class C");

  mref_ClassListStore->setClasses(ClassList);

  BOOST_CHECK_EQUAL(mref_ClassListStore->getClassNameFromIter(mref_ClassListStore->get_iter("0")),"class A");
  BOOST_CHECK_EQUAL(mref_ClassListStore->getClassNameFromIter(mref_ClassListStore->get_iter("1")),"class B");
  BOOST_CHECK_EQUAL(mref_ClassListStore->getClassNameFromIter(mref_ClassListStore->get_iter("2")),"class C");
  BOOST_CHECK_EQUAL(mref_ClassListStore->getClassNameFromIter(mref_ClassListStore->get_iter("3")),"");
}

BOOST_AUTO_TEST_CASE(test_getIterFromClassName)
{
  std::vector<std::string> ClassList;
  ClassList.push_back("class A");
  ClassList.push_back("class B");
  ClassList.push_back("class C");

  mref_ClassListStore->setClasses(ClassList);

  BOOST_CHECK_EQUAL(mref_ClassListStore->getIterFromClassName("class A"),mref_ClassListStore->get_iter("0"));
  BOOST_CHECK_EQUAL(mref_ClassListStore->getIterFromClassName("class B"),mref_ClassListStore->get_iter("1"));
  BOOST_CHECK_EQUAL(mref_ClassListStore->getIterFromClassName("class C"),mref_ClassListStore->get_iter("2"));
  BOOST_CHECK(mref_ClassListStore->getIterFromClassName("class D") == 0);
  BOOST_CHECK(mref_ClassListStore->getIterFromClassName("") == 0);
}

//BOOST_AUTO_TEST_CASE(test_getClassNameFromRowRef)
//{
//  std::vector<std::string> ClassList;
//  ClassList.push_back("class A");
//  ClassList.push_back("class B");
//  ClassList.push_back("class C");
//
//  mref_ClassListStore->setClasses(ClassList);
//
//  BOOST_CHECK_EQUAL(mref_ClassListStore->getClassNameFromRowRef(mref_ClassListStore->createRowRefFromPathString("0")),"class A");
//  BOOST_CHECK_EQUAL(mref_ClassListStore->getClassNameFromRowRef(mref_ClassListStore->createRowRefFromPathString("1")),"class B");
//  BOOST_CHECK_EQUAL(mref_ClassListStore->getClassNameFromRowRef(mref_ClassListStore->createRowRefFromPathString("2")),"class C");
//  BOOST_CHECK_EQUAL(mref_ClassListStore->getClassNameFromRowRef(mref_ClassListStore->createRowRefFromPathString("3")),"");
//}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END();
