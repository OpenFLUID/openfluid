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
  \file Value_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_value
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/ArrayValue.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/StringValue.hpp>

#include <list>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_null)
{
  openfluid::core::NullValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isNullValue());

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),false);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),false);

  openfluid::core::NullValue Val2 = Val1;
  BOOST_REQUIRE(Val2.isNullValue());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_interegr)
{
  openfluid::core::IntegerValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isIntegerValue());
  BOOST_REQUIRE_EQUAL( Val1.get(), 0);

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),true);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),false);

  Val1.set(17);
  std::cout << Val1 << std::endl;
  BOOST_REQUIRE_EQUAL( Val1.get(), 17);

  openfluid::core::IntegerValue Val2 = Val1;
  BOOST_REQUIRE_EQUAL( Val1.get(), Val2.get());

  Val2.set(17*7/45);
  std::cout << Val2 << " " << Val2.get() << " " << Val2.toString() << std::endl;


}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_double)
{
  openfluid::core::DoubleValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isDoubleValue());
  BOOST_REQUIRE_CLOSE( Val1.get(), 0.0, 0.0001 );

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),true);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),false);

  Val1.set(17.3);
  std::cout << Val1 << std::endl;
  BOOST_REQUIRE_CLOSE( Val1.get(), 17.3, 0.0001 );

  openfluid::core::DoubleValue Val2 = Val1;
  BOOST_REQUIRE_CLOSE( Val1.get(), Val2.get(), 0.0001 );

  Val2.set(17.3*7.19/45.67);
  std::cout << Val2 << " " << Val2.get() << " " << Val2.toString() << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_boolean)
{
  openfluid::core::BooleanValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isBooleanValue());
  BOOST_REQUIRE_EQUAL( Val1.get(), false );

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),true);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),false);

  Val1.set(true);
  std::cout << Val1 << std::endl;
  BOOST_REQUIRE_EQUAL( Val1.get(),true);

  openfluid::core::BooleanValue Val2 = Val1;
  BOOST_REQUIRE_EQUAL( Val1.get(), Val2.get());

  Val2.set(true && (false || true));
  std::cout << Val2 << " " << Val2.get() << " " << Val2.toString() << std::endl;
}


// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_vector)
{
  openfluid::core::VectorValue Val1;


  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isVectorValue());
  BOOST_REQUIRE_EQUAL( Val1.getSize(), 0 );

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),false);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),true);

  Val1 = openfluid::core::VectorValue(30,1.30);

  BOOST_REQUIRE_EQUAL( Val1.getSize(), 30 );
  BOOST_REQUIRE_CLOSE( Val1.at(16), 1.3,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1[29], 1.3,0.000001 );

  std::cout << Val1 << std::endl;


  openfluid::core::VectorValue Val2 = Val1;

  BOOST_REQUIRE_EQUAL( Val1.getSize(), Val2.getSize());
  BOOST_REQUIRE_CLOSE( Val2.at(16), 1.3,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1.at(16), Val2.at(16),0.000001 );
  BOOST_REQUIRE_CLOSE( Val2[29], 1.3,0.000001 );

  Val2[28] = 28.28;
  BOOST_REQUIRE_CLOSE( Val2[28], 28.28,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1[28], 1.3,0.000001 );


  std::cout << Val2 << std::endl;

  std::cout << Val2.toString() << std::endl;

  Val2.clear();
  BOOST_REQUIRE_EQUAL(Val2.getSize(),0);
  BOOST_REQUIRE_EQUAL(Val1.getSize(),30);

}

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_matrix)
{
  openfluid::core::MatrixValue Val1;

  std::cout << Val1 << std::endl;
  BOOST_REQUIRE(Val1.isMatrixValue());
  BOOST_REQUIRE_EQUAL( Val1.getSize1(), 0 );
  BOOST_REQUIRE_EQUAL( Val1.getSize2(), 0 );

  BOOST_REQUIRE_EQUAL(Val1.isSimple(),false);
  BOOST_REQUIRE_EQUAL(Val1.isCompound(),true);


  Val1 = openfluid::core::MatrixValue(20,30,1.35);

  BOOST_REQUIRE_EQUAL( Val1.getSize1(), 20 );
  BOOST_REQUIRE_EQUAL( Val1.getSize2(), 30 );
  BOOST_REQUIRE_CLOSE( Val1.at(16,17), 1.35,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1.get(19,29), 1.35,0.000001 );

  BOOST_REQUIRE_THROW(Val1.get(45,5),openfluid::base::OFException);

  std::cout << Val1 << std::endl;



  openfluid::core::MatrixValue Val2 = Val1;

  BOOST_REQUIRE_EQUAL( Val1.getSize(), Val2.getSize());
  BOOST_REQUIRE_CLOSE( Val2.at(9,9), 1.35,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1.at(9,9), Val2.at(9,9),0.000001 );
  BOOST_REQUIRE_CLOSE( Val2.get(9,9), 1.35,0.000001 );

  Val2.set(9,9,28.28);
  BOOST_REQUIRE_CLOSE( Val2.get(9,9), 28.28,0.000001 );
  BOOST_REQUIRE_CLOSE( Val1.get(9,9), 1.35,0.000001 );


  std::cout << Val2 << std::endl;

  std::cout << Val2.toString() << std::endl;

  Val2.clear();
  BOOST_REQUIRE_EQUAL(Val2.getSize(),0);
  BOOST_REQUIRE_EQUAL(Val2.getSize1(),0);
  BOOST_REQUIRE_EQUAL(Val2.getSize2(),0);
  BOOST_REQUIRE_EQUAL(Val1.getSize(),600);

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_array)
{
  openfluid::core::ArrayValue Val1;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_map)
{
  openfluid::core::MapValue Val1;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_string)
{
  openfluid::core::StringValue Val1;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_sequence)
{
  std::cout << "======== check_sequence ========" << std::endl;


  std::list< boost::shared_ptr<openfluid::core::Value> > ValuesSeq;


  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::DoubleValue>(new openfluid::core::DoubleValue(3.7)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::BooleanValue>(new openfluid::core::BooleanValue(true)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::VectorValue>(new openfluid::core::VectorValue(500,17.0)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::MatrixValue>(new openfluid::core::MatrixValue(29,47,1955.19)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::IntegerValue>(new openfluid::core::IntegerValue(1984)));
  ValuesSeq.push_back(boost::shared_ptr<openfluid::core::NullValue>(new openfluid::core::NullValue()));

  while(!ValuesSeq.empty())
  {
    bool Processed = false;

    if (ValuesSeq.front()->isBooleanValue())
    {
      Processed = true;

      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asBooleanValue().get(), true);

      openfluid::core::BooleanValue* BoolVTmp = static_cast<openfluid::core::BooleanValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_EQUAL(BoolVTmp->get(), true);

    }


    if (ValuesSeq.front()->isDoubleValue())
    {
      Processed = true;

      BOOST_REQUIRE_CLOSE(ValuesSeq.front().get()->asDoubleValue().get(), 3.7, 0.0001 );

      openfluid::core::DoubleValue* DblVTmp = static_cast<openfluid::core::DoubleValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_CLOSE(DblVTmp->get(), 3.7, 0.0001 );

    }


    if (ValuesSeq.front()->isIntegerValue())
    {

      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asIntegerValue().get(),1984);

      openfluid::core::IntegerValue* IntVTmp = static_cast<openfluid::core::IntegerValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_EQUAL(IntVTmp->get(),1984);

      Processed = true;
    }

    if (ValuesSeq.front()->isVectorValue())
    {
      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asVectorValue().getSize(),500);

      openfluid::core::VectorValue* VectVTmp = static_cast<openfluid::core::VectorValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_EQUAL(VectVTmp->getSize(),500);

      Processed = true;
    }


    if (ValuesSeq.front()->isMatrixValue())
    {
      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asMatrixValue().getSize(),(29*47));

      openfluid::core::MatrixValue* MatVTmp = static_cast<openfluid::core::MatrixValue*>(ValuesSeq.front().get());
      BOOST_REQUIRE_EQUAL(MatVTmp->getSize1(),29);
      BOOST_REQUIRE_EQUAL(MatVTmp->getSize2(),47);

      Processed = true;
    }



    if (ValuesSeq.front()->isNullValue())
    {
      Processed = true;
    }

    //std::cout << (*(ValuesSeq.front())) << std::endl;
    ValuesSeq.front()->writeToStream(std::cout);
    std::cout << std::endl;

    ValuesSeq.pop_front();

    BOOST_REQUIRE(Processed);
  }
}
