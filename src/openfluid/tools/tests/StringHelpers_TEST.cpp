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
  @file StringHelpers_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_stringhelpers


#include <boost/test/unit_test.hpp>

#include <openfluid/tools/StringHelpers.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_string_functions)
{
  {  
    // --- trim string

    BOOST_REQUIRE_EQUAL(openfluid::tools::trimLeft("  This is good  \n"),"This is good  \n");
    BOOST_REQUIRE_EQUAL(openfluid::tools::trimRight("  This is good  \n"),"  This is good");
    BOOST_REQUIRE_EQUAL(openfluid::tools::trim("  This is good  \n"),"This is good");
  }


  {
    // --- string case

    BOOST_REQUIRE_EQUAL(openfluid::tools::toLowerCase("CaMeL_CaSe"),"camel_case");
    BOOST_REQUIRE_EQUAL(openfluid::tools::toUpperCase("CaMeL_CaSe"),"CAMEL_CASE");
  }


  {
    // --- string content

    BOOST_REQUIRE(openfluid::tools::startsWith("Hello World","Hello"));
    BOOST_REQUIRE(openfluid::tools::startsWith("Hello World","H"));
    BOOST_REQUIRE(openfluid::tools::startsWith("Hello World","Hello World"));
    BOOST_REQUIRE(!openfluid::tools::startsWith("Hello World","W"));

    BOOST_REQUIRE(openfluid::tools::endsWith("Hello World","World"));
    BOOST_REQUIRE(openfluid::tools::endsWith("Hello World","d"));
    BOOST_REQUIRE(openfluid::tools::endsWith("Hello World","Hello World"));
    BOOST_REQUIRE(!openfluid::tools::endsWith("Hello World","Hello"));

    BOOST_REQUIRE(openfluid::tools::contains("Hello World","World"));
    BOOST_REQUIRE(openfluid::tools::contains("Hello World","d"));
    BOOST_REQUIRE(openfluid::tools::contains("Hello World","Hello World"));
    BOOST_REQUIRE(openfluid::tools::contains("Hello World","Hello"));
    BOOST_REQUIRE(openfluid::tools::contains("Hello World","lo "));
    BOOST_REQUIRE(!openfluid::tools::contains("Hello World","Bonjour"));
  }


  {
    // --- string conversion to numeric

    BOOST_REQUIRE_EQUAL(openfluid::tools::toNumeric<int>("1"),true);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toNumeric<int>("0"),false);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toNumeric<int>("1"),1);
    BOOST_REQUIRE_CLOSE(openfluid::tools::toNumeric<double>("1"),1.0,0.00001);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toNumeric<std::string>("1"),"1");

    BOOST_REQUIRE_THROW(openfluid::tools::toNumeric<bool>("1234"),openfluid::base::FrameworkException);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toNumeric<int>("1234"),1234);
    BOOST_REQUIRE_CLOSE(openfluid::tools::toNumeric<double>("1234"),1234.0,0.00001);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toNumeric<std::string>("1234"),"1234");

    BOOST_REQUIRE_THROW(openfluid::tools::toNumeric<bool>("12.34"),openfluid::base::FrameworkException);
    BOOST_REQUIRE_THROW(openfluid::tools::toNumeric<int>("12.34"),openfluid::base::FrameworkException);
    BOOST_REQUIRE_CLOSE(openfluid::tools::toNumeric<double>("12.34"),12.34,0.01);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toNumeric<std::string>("12.34"),"12.34");

    BOOST_REQUIRE_THROW(openfluid::tools::toNumeric<bool>("abcdef"),openfluid::base::FrameworkException);
    BOOST_REQUIRE_THROW(openfluid::tools::toNumeric<int>("abcdef"),openfluid::base::FrameworkException);
    BOOST_REQUIRE_THROW(openfluid::tools::toNumeric<double>("abcdef"),openfluid::base::FrameworkException);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toNumeric<std::string>("abcdef"),"abcdef");


    double Value;
    BOOST_REQUIRE(openfluid::tools::toNumeric("34.12",Value));
    BOOST_REQUIRE_CLOSE(Value,34.12,0.01);

    BOOST_REQUIRE(!openfluid::tools::toNumeric("34b.a12",Value));
  }


  {
    // --- string conversion to bool

    BOOST_REQUIRE_EQUAL(openfluid::tools::toBoolean("1"),true);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toBoolean("true"),true);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toBoolean("0"),false);
    BOOST_REQUIRE_EQUAL(openfluid::tools::toBoolean("false"),false);
    BOOST_REQUIRE_THROW(openfluid::tools::toBoolean(""),openfluid::base::FrameworkException);
    BOOST_REQUIRE_THROW(openfluid::tools::toBoolean("a"),openfluid::base::FrameworkException);
  }


  {
    // --- replace string parts

    std::string Str = "World is pink!";

    BOOST_REQUIRE_EQUAL(openfluid::tools::replace(Str," ","_"),"World_is_pink!");
    BOOST_REQUIRE_EQUAL(openfluid::tools::replace(Str," "," "),"World is pink!");
    BOOST_REQUIRE_EQUAL(openfluid::tools::replace(Str,"!","?"),"World is pink?");
    BOOST_REQUIRE_EQUAL(openfluid::tools::replace(Str,"is","is not"),"World is not pink!");
  }


  {
    // --- format string

    BOOST_REQUIRE_EQUAL(openfluid::tools::format("tea for %i and %s for tea",2,"two"),
                        "tea for 2 and two for tea");
    BOOST_REQUIRE_EQUAL(openfluid::tools::format("Pi is %.2f",3.1415927),"Pi is 3.14");

    auto Str = openfluid::tools::format("%s with %d and %.5f or %.3e to 0x%X","word",42,17.19782,17.19782,1976);
    std::cout << "Formatted string: '" << Str << "'" << std::endl;
  }


  {
    // --- join strings
    std::string Str = "While my  guitar  gently wheeps";

    BOOST_REQUIRE_EQUAL(openfluid::tools::split(Str," ").size(),5);
    BOOST_REQUIRE_EQUAL(openfluid::tools::split(Str," t").size(),7);
    BOOST_REQUIRE_EQUAL(openfluid::tools::split(Str," ",true).size(),7);

    auto SplittedStr = openfluid::tools::split(Str," ");
    auto ExpectedVect = {"While","my","guitar","gently","wheeps"};
    BOOST_REQUIRE_EQUAL_COLLECTIONS(SplittedStr.begin(),SplittedStr.end(),ExpectedVect.begin(),ExpectedVect.end());

    // --- join strings
    std::vector<std::string> VectStr = { "While","my","guitar","gently","wheeps"};

    BOOST_REQUIRE_EQUAL(openfluid::tools::join(VectStr," "),"While my guitar gently wheeps");
    BOOST_REQUIRE_EQUAL(openfluid::tools::join(VectStr,"-o-"),"While-o-my-o-guitar-o-gently-o-wheeps");
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_stringvector_operators)
{
  {
    // << operator on vectors

    std::vector<std::string> VectStr1, VectStr2;
    const std::string Str = "!";
    
    VectStr1 << "Hello" << "World";
    BOOST_REQUIRE_EQUAL(VectStr1.size(),2);
    BOOST_REQUIRE_EQUAL(VectStr1[0],"Hello");
    BOOST_REQUIRE_EQUAL(VectStr1[1],"World");
    
    VectStr1 << Str;
    BOOST_REQUIRE_EQUAL(VectStr1.size(),3);
    BOOST_REQUIRE_EQUAL(VectStr1[0],"Hello");
    BOOST_REQUIRE_EQUAL(VectStr1[1],"World");
    BOOST_REQUIRE_EQUAL(VectStr1[2],"!");
    

    VectStr2 = {"Sun","is","shining"};
    VectStr1 << VectStr2;
    BOOST_REQUIRE_EQUAL(VectStr1.size(),6);
    BOOST_REQUIRE_EQUAL(VectStr1[0],"Hello");
    BOOST_REQUIRE_EQUAL(VectStr1[1],"World");
    BOOST_REQUIRE_EQUAL(VectStr1[2],"!");
    BOOST_REQUIRE_EQUAL(VectStr1[3],"Sun");
    BOOST_REQUIRE_EQUAL(VectStr1[4],"is");
    BOOST_REQUIRE_EQUAL(VectStr1[5],"shining");

    VectStr1.clear();
    VectStr1 << "Hello" << "World" << Str << VectStr2 << Str;
    BOOST_REQUIRE_EQUAL(VectStr1.size(),7);
    BOOST_REQUIRE_EQUAL(VectStr1[0],"Hello");
    BOOST_REQUIRE_EQUAL(VectStr1[1],"World");
    BOOST_REQUIRE_EQUAL(VectStr1[2],"!");
    BOOST_REQUIRE_EQUAL(VectStr1[3],"Sun");
    BOOST_REQUIRE_EQUAL(VectStr1[4],"is");
    BOOST_REQUIRE_EQUAL(VectStr1[5],"shining");
    BOOST_REQUIRE_EQUAL(VectStr1[6],"!");

    VectStr1.clear();
    VectStr1 << VectStr2;
    BOOST_REQUIRE_EQUAL(VectStr1.size(),3);    
    VectStr1 << VectStr1;
    BOOST_REQUIRE_EQUAL(VectStr1.size(),6);
    BOOST_REQUIRE_EQUAL(VectStr1[0],"Sun");
    BOOST_REQUIRE_EQUAL(VectStr1[1],"is");
    BOOST_REQUIRE_EQUAL(VectStr1[2],"shining");
    BOOST_REQUIRE_EQUAL(VectStr1[3],"Sun");
    BOOST_REQUIRE_EQUAL(VectStr1[4],"is");
    BOOST_REQUIRE_EQUAL(VectStr1[5],"shining");
  }
}

