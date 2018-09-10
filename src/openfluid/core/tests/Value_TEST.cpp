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
  @file Value_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_value


#include <chrono>
#include <list>
#include <memory>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/circular_buffer.hpp>

#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/TreeValue.hpp>
#include <openfluid/core/StringValue.hpp>


// =====================================================================
// =====================================================================


#define DECLARE_TEST_TICKER(name) \
  std::chrono::high_resolution_clock::time_point _M_Start_##name; \
  std::chrono::high_resolution_clock::time_point _M_Stop_##name; \
  std::chrono::milliseconds _M_Duration_##name;


#define START_TEST_TICKER(name) \
  _M_Start_##name = std::chrono::high_resolution_clock::now();


#define MARK_TEST_TICKER(name) \
  _M_Stop_##name = std::chrono::high_resolution_clock::now(); \
  _M_Duration_##name = std::chrono::duration_cast<std::chrono::milliseconds>(_M_Stop_##name - _M_Start_##name);


#define TEST_DURATION_AS_MS(name) \
  (_M_Duration_##name.count())


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_sequence)
{
  std::cout << "======== check_sequence ========" << std::endl;


  std::list< std::shared_ptr<openfluid::core::Value> > ValuesSeq;


  ValuesSeq.push_back(std::shared_ptr<openfluid::core::DoubleValue>(new openfluid::core::DoubleValue(3.7)));
  ValuesSeq.push_back(std::shared_ptr<openfluid::core::MapValue>(new openfluid::core::MapValue()));
  ValuesSeq.push_back(std::shared_ptr<openfluid::core::BooleanValue>(new openfluid::core::BooleanValue(true)));
  ValuesSeq.push_back(std::shared_ptr<openfluid::core::VectorValue>(new openfluid::core::VectorValue(500,17.0)));
  ValuesSeq.push_back(std::shared_ptr<openfluid::core::MatrixValue>(new openfluid::core::MatrixValue(29,47,1955.19)));
  ValuesSeq.push_back(std::shared_ptr<openfluid::core::StringValue>(
      new openfluid::core::StringValue("Welcome to the jungle!")));
  ValuesSeq.push_back(std::shared_ptr<openfluid::core::IntegerValue>(new openfluid::core::IntegerValue(1984)));
  ValuesSeq.push_back(std::shared_ptr<openfluid::core::NullValue>(new openfluid::core::NullValue()));
  ValuesSeq.push_back(std::shared_ptr<openfluid::core::TreeValue>(new openfluid::core::TreeValue()));

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
      BOOST_REQUIRE_EQUAL(MatVTmp->getColsNbr(),29);
      BOOST_REQUIRE_EQUAL(MatVTmp->getRowsNbr(),47);

      Processed = true;
    }


    if (ValuesSeq.front()->isMapValue())
    {
      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asMapValue().getSize(),0);

      Processed = true;
    }


    if (ValuesSeq.front()->isTreeValue())
    {
      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asTreeValue().size(),1);
      Processed = true;
    }

    if (ValuesSeq.front()->isStringValue())
    {
      BOOST_REQUIRE_EQUAL(ValuesSeq.front().get()->asStringValue().get(),"Welcome to the jungle!");

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


// =====================================================================
// =====================================================================


class ValueCapsule
{
  public:

    std::unique_ptr<openfluid::core::Value> m_Val;

    ValueCapsule()
    : m_Val(new openfluid::core::NullValue())
    {

    }

    void set(const openfluid::core::Value& Val)
    {
      m_Val.reset(Val.clone());
    };

    openfluid::core::Value* get()
    {
      return m_Val.get();
    }
};


BOOST_AUTO_TEST_CASE(check_capsule)
{
  std::cout << "======== check_capsule ========" << std::endl;

  ValueCapsule Capsule;

  openfluid::core::DoubleValue DblVal(0.1);

  Capsule.set(openfluid::core::DoubleValue(3.14));

  DblVal = *(Capsule.get());

  BOOST_REQUIRE_CLOSE(DblVal.get(),3.14,0.0001);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_performance)
{

  std::cout << "======== check_performance ========" << std::endl;


  DECLARE_TEST_TICKER(DBL);
  DECLARE_TEST_TICKER(DBLVAL);


  const unsigned int ElementsNbr = 10000000;
  const unsigned int BufferSize = 5;

  boost::circular_buffer<double> BufferDouble(BufferSize);
  boost::circular_buffer<std::shared_ptr<openfluid::core::Value> > BufferValue(5000);


  // ------

  START_TEST_TICKER(DBL);

  for (unsigned int i=0; i< ElementsNbr;i++)
  {
    BufferDouble.push_back(static_cast<double>(i)/ElementsNbr);
  }

  MARK_TEST_TICKER(DBL);
  std::cout << "Duration [double], populating: " << TEST_DURATION_AS_MS(DBL) << "ms" << std::endl;


  // ------

  START_TEST_TICKER(DBLVAL);

  for (unsigned int i=0; i< ElementsNbr;i++)
  {
    BufferValue.push_back(std::shared_ptr<openfluid::core::Value>(
        new openfluid::core::DoubleValue(static_cast<double>(i)/ElementsNbr)));
  }

  MARK_TEST_TICKER(DBLVAL);
  std::cout << "Duration [DoubleValue], populating: " << TEST_DURATION_AS_MS(DBLVAL) << "ms" << std::endl;


  // ------

  START_TEST_TICKER(DBL);

  for (unsigned int i=0; i< BufferSize;i++)
  {
    double Tmp = BufferDouble[i];
    std::cout << Tmp << std::endl;
  }

  MARK_TEST_TICKER(DBL);
  std::cout << "Duration [double], accessing: " << TEST_DURATION_AS_MS(DBL) << "ms" << std::endl;


  // ------

  START_TEST_TICKER(DBLVAL);

  for (unsigned int i=0; i< BufferSize;i++)
  {
    double Tmp = BufferValue[i].get()->asDoubleValue().get();
    std::cout << Tmp << std::endl;
  }

  MARK_TEST_TICKER(DBLVAL);
  std::cout << "Duration [DoubleValue], accessing: " << TEST_DURATION_AS_MS(DBLVAL) << "ms" << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_stringtotype)
{
  openfluid::core::Value::Type ValueType;

  BOOST_REQUIRE(openfluid::core::Value::getValueTypeFromString("double",ValueType));
  BOOST_REQUIRE_EQUAL(ValueType,openfluid::core::Value::DOUBLE);

  BOOST_REQUIRE(openfluid::core::Value::getValueTypeFromString("vector",ValueType));
  BOOST_REQUIRE_EQUAL(ValueType,openfluid::core::Value::VECTOR);

  BOOST_REQUIRE(openfluid::core::Value::getValueTypeFromString("null",ValueType));
  BOOST_REQUIRE_EQUAL(ValueType,openfluid::core::Value::NULLL);

  BOOST_REQUIRE(openfluid::core::Value::getValueTypeFromString("tree",ValueType));
  BOOST_REQUIRE_EQUAL(ValueType,openfluid::core::Value::TREE);


  BOOST_REQUIRE(!openfluid::core::Value::getValueTypeFromString("foo",ValueType));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_typetostring)
{
  BOOST_REQUIRE_EQUAL(openfluid::core::Value::getStringFromValueType(openfluid::core::Value::DOUBLE),"double");

  BOOST_REQUIRE_EQUAL(openfluid::core::Value::getStringFromValueType(openfluid::core::Value::VECTOR),"vector");

  BOOST_REQUIRE_EQUAL(openfluid::core::Value::getStringFromValueType(openfluid::core::Value::NULLL),"null");

  BOOST_REQUIRE_EQUAL(openfluid::core::Value::getStringFromValueType(openfluid::core::Value::TREE),"tree");
}

