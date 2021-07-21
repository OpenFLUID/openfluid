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
  @file SettingsBackend_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_settingsbackend


#include <tuple>

#include <boost/test/unit_test.hpp>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/SettingsBackend.hpp>
#include <openfluid/tools/Filesystem.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const std::string SBInputPath = CONFIGTESTS_INPUT_MISCDATA_DIR+"/SettingsBackend";


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_setting_value)
{
  {
    openfluid::tools::SettingValue Val;

    BOOST_CHECK(Val.isNull());

    BOOST_CHECK(!Val.is<bool>());
    BOOST_CHECK_THROW(Val.get<bool>(),openfluid::base::FrameworkException);
    BOOST_CHECK_EQUAL(Val.get<bool>(true),true);

    BOOST_CHECK(!Val.is<int>());
    BOOST_CHECK_THROW(Val.get<int>(),openfluid::base::FrameworkException);
    BOOST_CHECK_EQUAL(Val.get<int>(79),79);

    BOOST_CHECK(!Val.is<double>());
    BOOST_CHECK_THROW(Val.get<double>(),openfluid::base::FrameworkException);
    BOOST_CHECK_CLOSE(Val.get<double>(1.23456),1.23456,0.00001);

    BOOST_CHECK(!Val.is<std::string>());
    BOOST_CHECK_THROW(Val.get<std::string>(),openfluid::base::FrameworkException);
    BOOST_CHECK_EQUAL(Val.get<std::string>("Hello"),"Hello");

    BOOST_CHECK(!Val.is<std::vector<std::string>>());
    BOOST_CHECK_THROW(Val.get<std::vector<std::string>>(),openfluid::base::FrameworkException);
    std::vector<std::string> ExpVect = {"Hello","world","!"};
    std::vector<std::string> RetVect = Val.get<std::vector<std::string>>(ExpVect);
    BOOST_CHECK_EQUAL_COLLECTIONS(ExpVect.begin(),ExpVect.end(),RetVect.begin(),RetVect.end());

/*    BOOST_CHECK(!Val.is<rapidjson::Value*>());
    auto ExpJSONVal = std::make_unique<rapidjson::Value>();
    BOOST_CHECK_THROW(Val.get<rapidjson::Value*>(),openfluid::base::FrameworkException);
    BOOST_CHECK_EQUAL(Val.get<rapidjson::Value*>(ExpJSONVal.get()),ExpJSONVal.get());*/
  }


  {
    openfluid::tools::SettingValue Val(true);

    BOOST_CHECK(Val.is<bool>());
    BOOST_CHECK_EQUAL(Val.get<bool>(),true);
    BOOST_CHECK_EQUAL(Val.get<bool>(false),true);
  }

  {
    openfluid::tools::SettingValue Val(42);

    BOOST_CHECK(Val.is<int>());
    BOOST_CHECK_EQUAL(Val.get<int>(),42);
    BOOST_CHECK_EQUAL(Val.get<int>(24),42);
  }

  {
    openfluid::tools::SettingValue Val(17.17);

    BOOST_CHECK(Val.is<double>());
    BOOST_CHECK_CLOSE(Val.get<double>(),17.17,0.01);
    BOOST_CHECK_CLOSE(Val.get<double>(24.0),17.17,0.01);
  }

  {
    openfluid::tools::SettingValue Val(true);

    BOOST_CHECK(Val.is<bool>());
    BOOST_CHECK_EQUAL(Val.get<bool>(),true);
    BOOST_CHECK_EQUAL(Val.get<bool>(false),true);
  }

  {
    openfluid::tools::SettingValue Val("a string");

    BOOST_CHECK_EQUAL(Val.get<std::string>(),"a string");
    BOOST_CHECK_EQUAL(Val.get<std::string>("Hello"),"a string");
  }

  {
    openfluid::tools::SettingValue Val({"Here","comes","the","sun"});

    BOOST_CHECK(Val.is<std::vector<std::string>>());
    std::vector<std::string> ExpVect = {"Here","comes","the","sun"};
    std::vector<std::string> RetVect = Val.get<std::vector<std::string>>();
    BOOST_CHECK_EQUAL_COLLECTIONS(ExpVect.begin(),ExpVect.end(),RetVect.begin(),RetVect.end());
    RetVect = Val.get<std::vector<std::string>>(std::vector<std::string>({"not","found"}));
    BOOST_CHECK_EQUAL_COLLECTIONS(ExpVect.begin(),ExpVect.end(),RetVect.begin(),RetVect.end());
  }

  {
    auto JV = rapidjson::Value(-123);
    openfluid::tools::SettingValue Val(&JV);
    BOOST_CHECK(Val.isJSONValue());
    BOOST_CHECK(Val.JSONValue()->IsInt());
    BOOST_CHECK_EQUAL(Val.JSONValue()->GetInt(),-123);
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::tools::SettingsBackend SB1;
  BOOST_REQUIRE(SB1.getBackendFilePath().empty());
  BOOST_REQUIRE_EQUAL(SB1.getRole(),"settings");

  SB1.writeToStream(std::cout);
  std::cout << std::endl;


  openfluid::tools::SettingsBackend SB2("test-settings.json","test-settings");
  BOOST_REQUIRE_EQUAL(SB2.getBackendFilePath(),"test-settings.json");
  BOOST_REQUIRE_EQUAL(SB2.getRole(),"test-settings");

  BOOST_REQUIRE_EQUAL(SB2.getValue("/does/not/exist").get<std::string>("hello world!"),"hello world!");
  BOOST_REQUIRE_THROW(SB2.getValue("/does/not/exist").get<std::string>(),openfluid::base::FrameworkException);

  BOOST_REQUIRE_EQUAL(SB2.getValue("/does/not/exist").get<bool>(true),true);
  BOOST_REQUIRE_THROW(SB2.getValue("/does/not/exist").get<bool>(),openfluid::base::FrameworkException);

  BOOST_REQUIRE_EQUAL(SB2.getValue("/does/not/exist").get<int>(42),42);
  BOOST_REQUIRE_THROW(SB2.getValue("/does/not/exist").get<int>(),openfluid::base::FrameworkException);

  BOOST_REQUIRE_CLOSE(SB2.getValue("/does/not/exist").get<double>(3.1415927),3.1415927,0.0000001);
  BOOST_REQUIRE_THROW(SB2.getValue("/does/not/exist").get<double>(),openfluid::base::FrameworkException);

  std::vector<std::string> ExpStrArr = {"hello","world","!"};
  auto RetStrArr = SB2.getValue("/does/not/exist").get<std::vector<std::string>>(ExpStrArr);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(RetStrArr.begin(),RetStrArr.end(),ExpStrArr.begin(),ExpStrArr.end());
  BOOST_REQUIRE_THROW(SB2.getValue("/does/not/exist").get<std::vector<std::string>>(),
                      openfluid::base::FrameworkException);

  BOOST_REQUIRE_EQUAL(SB2.getValue("/does/not/exist").isJSONValue(),false);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_read_value)
{
  openfluid::tools::SettingsBackend SB(SBInputPath+"/settings.json");

  auto Value = SB.getValue("/string");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<std::string>());
  BOOST_CHECK_EQUAL(Value.get<std::string>(),"Hello world!");

  Value = SB.getValue("/bool");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<bool>());
  BOOST_CHECK_EQUAL(Value.get<bool>(),true);

  Value = SB.getValue("/int");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<int>());
  BOOST_CHECK_EQUAL(Value.get<int>(),42);

  Value = SB.getValue("/double");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<double>());
  BOOST_CHECK_CLOSE(Value.get<double>(),3.1415927,0.000001);

  Value = SB.getValue("/stringarray");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<std::vector<std::string>>());

  std::vector<std::string> StrArrayRef = {"hello","world","!"};
  std::vector<std::string> StrArray = Value.get<std::vector<std::string>>();

  BOOST_CHECK_EQUAL_COLLECTIONS(StrArrayRef.begin(),StrArrayRef.end(),StrArray.begin(),StrArray.end());

  // ----------------------------

  Value = SB.getValue("/object");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.isJSONValue());
  BOOST_CHECK(Value.JSONValue()->IsObject());

  Value = SB.getValue("/object/string");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<std::string>());
  BOOST_CHECK_EQUAL(Value.get<std::string>(),"Goodnight");

  Value = SB.getValue("/object/bool");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<bool>());
  BOOST_CHECK_EQUAL(Value.get<bool>(),false);

  Value = SB.getValue("/object/int");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<int>());
  BOOST_CHECK_EQUAL(Value.get<int>(),47);

  Value = SB.getValue("/object/double");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<double>());
  BOOST_CHECK_CLOSE(Value.get<double>(),1.23456,0.00001);

  Value = SB.getValue("/object/stringarray");
  BOOST_CHECK(!Value.isNull());
  BOOST_CHECK(Value.is<std::vector<std::string>>());

  StrArrayRef = {"goodmorning","and","goodnight"};
  StrArray = Value.get<std::vector<std::string>>();
  BOOST_CHECK_EQUAL_COLLECTIONS(StrArrayRef.begin(),StrArrayRef.end(),StrArray.begin(),StrArray.end());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_write_value)
{
  openfluid::tools::SettingsBackend SB("");

  SB.setValue("/","string","Hello world!");
  SB.setValue("/lvl1","bool",false);
  SB.setValue("/lvl1/lvl2","int",99);
  SB.setValue("/lvl1/lvl2/lvl3","double",1.5);

  SB.setValue("/another/path/to","string","auto-create");
  SB.setValue("/another/path/to","value","a value");
  SB.setValue("/another/path/to","value",76);

  BOOST_CHECK_THROW(SB.setValue("/path/to/another","string","no-auto-create",false),
                    openfluid::base::FrameworkException);

  // ----------------------------

  {
    BOOST_CHECK_EQUAL(SB.exists("/string"),true);    
    auto Value = SB.getValue("/string");
    BOOST_CHECK(!Value.isNull());
    BOOST_CHECK(Value.is<std::string>());
    BOOST_CHECK_EQUAL(Value.get<std::string>(),"Hello world!");
    BOOST_CHECK_EQUAL(SB.exists("/nostring"),false);
  }

  {
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/lvl2/int"),true); 
    auto Value = SB.getValue("/lvl1/lvl2/int");
    BOOST_CHECK(!Value.isNull());
    BOOST_CHECK(Value.is<int>());
    BOOST_CHECK_EQUAL(Value.get<int>(),99);
    BOOST_CHECK_EQUAL(SB.exists("/int"),false);
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/int"),false);
  }

  {
    SB.remove("/lvl1/lvl2/int");
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/lvl2/int"),false);
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/lvl2/lvl3"),true);
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/lvl2/lvl3/double"),true);

    SB.remove("/lvl1/lvl2");
    BOOST_CHECK_EQUAL(SB.exists("/lvl1"),true);
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/bool"),true);    
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/lvl2"),false);
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/lvl2/int"),false);
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/lvl2/lvl3"),false);
    BOOST_CHECK_EQUAL(SB.exists("/lvl1/lvl2/lvl3/double"),false);    
  }

  {
    auto Value = SB.getValue("/another/path/to/value");
    BOOST_CHECK(!Value.isNull());
    BOOST_CHECK(Value.is<int>());
    BOOST_CHECK_EQUAL(Value.get<int>(),76);
  }

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_persistence)
{
  std::string BackendFile = CONFIGTESTS_OUTPUT_DATA_DIR+"/SettingsBackend/openfluid-settings.json";

  openfluid::tools::Filesystem::removeFile(BackendFile);
  openfluid::tools::SettingsBackend SBTo(BackendFile,"openfluid-settings");

  {
    std::string Path = "/builder/interface";
    SBTo.setValue(Path,"lang","fr_FR");
    SBTo.setValue(Path,"dockpos",1);
    SBTo.setValue(Path,"toolbarpos",4);
    SBTo.setValue(Path,"wareswatchers",true);
  }

  {
    std::string Path = "/builder/interface/confirmation";
    SBTo.setValue(Path,"wareremoval",false);
    SBTo.setValue(Path,"paramremoval",true);
    SBTo.setValue(Path,"spatialattrsremoval",true);
    SBTo.setValue(Path,"spatialconnsremoval",true);
    SBTo.setValue(Path,"spatialunitsremoval",true);    
  }

  {
    std::string Path = "/builder/paths";
    SBTo.setValue(Path,"simulators",{"/path/to/extra/simulators"});
    SBTo.setValue(Path,"observers",std::vector<std::string>({"/path/to/extra/observers",
                                                             "/another/path/to/extra/observers"}));
    SBTo.setValue(Path,"builderexts",{});
    SBTo.setValue(Path,"workspaces",std::vector<std::string>({"/path/to/workspace",
                                                              "/path/to/workspace-workgroup",
                                                              "/path/to/workspace-sandbox"}));    
  }

  {
    std::string Path = "/builder/recentprojects";
    SBTo.setValue(Path,"max",5);

    std::vector<std::pair<std::string,std::string>> Projects = 
     {
       {"Project_A","/path/to/prjA"},
       {"Project_B","/path/to/prjB"},
       {"Project_K","/path/to/prjK"},
       {"Project_X","/path/to/prjX"},
       {"Project_Y","/path/to/prjY"}
     };
    rapidjson::Value ListValue(rapidjson::kArrayType);
    for (const auto& Prj : Projects)
    {
      rapidjson::Value Item;
      Item.SetObject();
      Item.AddMember("name",rapidjson::Value(Prj.first.c_str(),SBTo.data().GetAllocator()),
                     SBTo.data().GetAllocator());
      Item.AddMember("path",rapidjson::Value(Prj.second.c_str(),SBTo.data().GetAllocator()),
                     SBTo.data().GetAllocator());
      ListValue.PushBack(Item,SBTo.data().GetAllocator());
    }
    SBTo.setValue(Path,"list",openfluid::tools::SettingValue(&ListValue));
  }

  {
    std::string Path = "/builder/runconfig";
    SBTo.setValue(Path,"begin","2018-02-06 00:00:00");
    SBTo.setValue(Path,"end","2018-02-07 00:00:00");    
    SBTo.setValue(Path,"delta",300);    
  }

  {
    std::string Path = "/waresdev/interface";
    SBTo.setValue(Path,"savebeforebuild",true);
  }

  {
    std::string Path = "/waresdev/interface/texteditor";
    SBTo.setValue(Path,"font","DejaVu Sans Mono");
  }

  {
    std::string Path = "/waresdev/interface/texteditor/highlighting/syntax";
    SBTo.setValue(Path,"enabled",true);

    std::vector<std::tuple<std::string,std::string,std::vector<std::string>>> Categs =
    {
      {"comment","#A7A7A7",{"italic"}},
      {"datatype","system",{"bold"}},
      {"deprecated","#7F0055",{"bold", "strike-through"}},
      {"function","system",{"bold"}},
      {"keyword","#7F0055",{"bold"}},
      {"openfluid-deprecated","#546F02",{"bold", "strike-through"}},
      {"openfluid-keyword","#546F02",{"bold"}},
      {"quoted","#2A00FF",{}}
    };
    rapidjson::Value ListValue(rapidjson::kArrayType);
    for (const auto& Cat : Categs)
    {
      rapidjson::Value Item;
      Item.SetObject();
      Item.AddMember("name",rapidjson::Value(std::get<0>(Cat).c_str(),SBTo.data().GetAllocator()),
                     SBTo.data().GetAllocator());
      Item.AddMember("color",rapidjson::Value(std::get<1>(Cat).c_str(),SBTo.data().GetAllocator()),
                     SBTo.data().GetAllocator());
      rapidjson::Value ItemDeco(rapidjson::kArrayType);
      for (const auto& Deco: std::get<2>(Cat))
      {
        ItemDeco.PushBack(rapidjson::Value(Deco.c_str(),SBTo.data().GetAllocator()),SBTo.data().GetAllocator());
      }
      Item.AddMember("decoration",ItemDeco,SBTo.data().GetAllocator());      
      ListValue.PushBack(Item,SBTo.data().GetAllocator());
    }
    SBTo.setValue(Path,"categories",openfluid::tools::SettingValue(&ListValue));
  }

  {
    std::string Path = "/waresdev/interface/texteditor/highlighting/currentline";
    SBTo.setValue(Path,"enabled",true);
    SBTo.setValue(Path,"color","#EFF6FF");
  }

  // ---------------------------------------

  openfluid::tools::SettingsBackend SBFrom(BackendFile,"openfluid-settings");
  
  SBFrom.writeToStream(std::cout);
  std::cout << std::endl;

  {
    BOOST_CHECK_EQUAL(SBFrom.getValue("/builder/interface/lang").get<std::string>(),"fr_FR");
    BOOST_CHECK_EQUAL(SBFrom.getValue("/builder/interface/dockpos").get<int>(),1);
    BOOST_CHECK_EQUAL(SBFrom.getValue("/builder/interface/toolbarpos").get<int>(),4);
    BOOST_CHECK_EQUAL(SBFrom.getValue("/builder/interface/wareswatchers").get<bool>(),true);
  }

  {
    BOOST_CHECK_EQUAL(SBFrom.getValue("/builder/recentprojects/max").get<int>(),5);

    const auto ListValue = SBFrom.getValue("/builder/recentprojects/list").JSONValue();

    BOOST_CHECK(ListValue->IsArray());
    BOOST_CHECK_EQUAL(ListValue->Size(),5);

    for (auto& Val : ListValue->GetArray())
    {
      BOOST_CHECK(Val.HasMember("name"));
      BOOST_CHECK(Val.HasMember("path"));
    }
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_wrong_file)
{
  openfluid::tools::SettingsBackend SB(SBInputPath+"/wrong.json");

  SB.writeToStream(std::cout);
}
