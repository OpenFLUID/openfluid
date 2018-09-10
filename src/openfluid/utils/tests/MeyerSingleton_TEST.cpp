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
  @file MeyerSingleton_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_singleton


#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/utils/MeyerSingleton.hpp>


// =====================================================================
// =====================================================================


class MyClass : public openfluid::utils::MeyerSingleton<MyClass>
{
  friend class openfluid::utils::MeyerSingleton<MyClass>;

  public:

      void incAndPrint()
      {
        m_Counter++;
        std::cout << m_Counter << std::endl;
      }

      unsigned int getCounter()
      {
        return m_Counter;
      }

    private:

      unsigned int m_Counter;;

      MyClass()
      {
        m_Counter = 0;
        std::cout << __PRETTY_FUNCTION__ << std::endl;
      }

      ~MyClass()
      {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
      }

};


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_meyer_singleton)
{
  MyClass::instance();

  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),0);

  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),0);
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),0);
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),0);
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),0);
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),0);

  MyClass::instance()->incAndPrint();
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),1);

  MyClass::instance()->incAndPrint();
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),2);

  MyClass::instance()->incAndPrint();
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),3);

  MyClass* MyObj = MyClass::instance();
  BOOST_REQUIRE_EQUAL(MyObj->getCounter(),3);

  MyObj->incAndPrint();
  BOOST_REQUIRE_EQUAL(MyObj->getCounter(),4);
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),4);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_meyer_singleton_more)
{
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),4);

  MyClass::instance()->incAndPrint();
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),5);

  MyClass* MyObj = MyClass::instance();
  BOOST_REQUIRE_EQUAL(MyObj->getCounter(),5);

  MyObj->incAndPrint();
  BOOST_REQUIRE_EQUAL(MyObj->getCounter(),6);
  BOOST_REQUIRE_EQUAL(MyClass::instance()->getCounter(),6);
}

