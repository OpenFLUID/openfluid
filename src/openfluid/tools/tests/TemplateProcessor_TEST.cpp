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
  @file TemplateProcessor_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_templateprocessor


#include <fstream>

#include <boost/test/unit_test.hpp>

#include <openfluid/tools/TemplateProcessor.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/base/FrameworkException.hpp>

#include "tests-config.hpp"


// =====================================================================
// =====================================================================


const std::string TPInputPath = CONFIGTESTS_INPUT_MISCDATA_DIR+"/TemplateProcessor";
const std::string TPOutputPath = CONFIGTESTS_OUTPUT_DATA_DIR+"/TemplateProcessor";


// =====================================================================
// =====================================================================


bool compare(const std::string& FilePath,const std::string& Str)
{
  std::ifstream IFS(FilePath);
  std::string FileContent((std::istreambuf_iterator<char>(IFS)),(std::istreambuf_iterator<char>()));
  return FileContent == Str;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::tools::TemplateProcessor TplProc("%%","%%");

  TplProc = openfluid::tools::TemplateProcessor("{{","}}");

  BOOST_CHECK_THROW(openfluid::tools::TemplateProcessor("{{",""),openfluid::base::FrameworkException);
  BOOST_CHECK_THROW(openfluid::tools::TemplateProcessor("","]]"),openfluid::base::FrameworkException);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_renderstring)
{
  const openfluid::tools::TemplateProcessor::Data Data = {
    {"key1","value #1"},
    {"key2","value #2"},
    {"","empty value"}
  };


  {
    openfluid::tools::TemplateProcessor TplProc("%%","%%");
    std::string TplStr = "This is %%key1%% and %% key2 %%. Thats all!";
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderString(TplStr,Data,Errors);
    BOOST_CHECK(Errors.empty());
    BOOST_CHECK_EQUAL(RenderedStr,"This is value #1 and value #2. Thats all!");
  }

  {
    openfluid::tools::TemplateProcessor TplProc("%%","%%");
    std::string TplStr = "This is %%key1%% and %% key2 %%";
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderString(TplStr,Data,Errors);
    BOOST_CHECK(Errors.empty());
    BOOST_CHECK_EQUAL(RenderedStr,"This is value #1 and value #2");
  }

  {
    openfluid::tools::TemplateProcessor TplProc("%%","%%");
    std::string TplStr = "This is value 1 and value 2 without replacement";
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderString(TplStr,Data,Errors);
    BOOST_CHECK(Errors.empty());
    BOOST_CHECK_EQUAL(RenderedStr,"This is value 1 and value 2 without replacement");
  }

  {
    openfluid::tools::TemplateProcessor TplProc("%%","%%");
    std::string TplStr = "This is %%key1%%, %%doesnotexist%%, %%nonexist%% and %%doesnotexist%%. Thats all!";
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderString(TplStr,Data,Errors);
    BOOST_CHECK(!Errors.empty());
    BOOST_CHECK_EQUAL(Errors.size(),2);
    BOOST_CHECK_EQUAL(RenderedStr,"This is value #1, %%doesnotexist%%, %%nonexist%% and %%doesnotexist%%. Thats all!");
  }

  {
    openfluid::tools::TemplateProcessor TplProc("{{","}}");
    std::string TplStr = "This is {{key1}} and {{ key2  }}. Thats all!";
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderString(TplStr,Data,Errors);
    BOOST_CHECK(Errors.empty());
    BOOST_CHECK_EQUAL(RenderedStr,"This is value #1 and value #2. Thats all!");
  }

  {
    openfluid::tools::TemplateProcessor TplProc("${","}");
    std::string TplStr = "This is ${key1} and ${ key2  }. Thats all!";
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderString(TplStr,Data,Errors);
    BOOST_CHECK(Errors.empty());
    BOOST_CHECK_EQUAL(RenderedStr,"This is value #1 and value #2. Thats all!");
  }

  {
    openfluid::tools::TemplateProcessor TplProc("%%","%%");
    std::string TplStr = "%%#key1%%Enabled if %%key1%% exists and is not empty %%#key3%%%%key3%%";
    openfluid::tools::TemplateProcessor::Errors Errors;
 
    TplProc.enableAutoComment("#");
    TplProc.ignoreUnknown(true);

    auto RenderedStr = TplProc.renderString(TplStr,Data,Errors);
    BOOST_CHECK(Errors.empty());
    BOOST_CHECK_EQUAL(RenderedStr,"Enabled if value #1 exists and is not empty #");
  }

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_renderfiletostring)
{
  const openfluid::tools::TemplateProcessor::Data Data = {
    {"WAREID","sim.test.id"},
    {"WAREDESCRIPTION","This is the sim.test.id simulator"},
    {"braces","no brace"},
    {"percent","%"}
  };


  {
    openfluid::tools::TemplateProcessor TplProc("%%","%%");
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderFile(TPInputPath+"/sim.src/README.md",Data,Errors);
    BOOST_CHECK(Errors.empty());
    BOOST_CHECK_EQUAL(RenderedStr,"# sim.test.id simulator\n\nThis is the sim.test.id simulator\n");
  }

  {
    openfluid::tools::TemplateProcessor TplProc("{{","}}");
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderFile(TPInputPath+"/misc/braces.txt",Data,Errors);
    BOOST_CHECK(Errors.empty());
    BOOST_CHECK_EQUAL(RenderedStr,"This is a file with no brace\nand no %\n\nThat's it!\n\n");
  }

  {
    openfluid::tools::TemplateProcessor TplProc("&",";");
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderFile(TPInputPath+"/misc/errors.txt",Data,Errors);
    BOOST_CHECK(!Errors.empty());
    BOOST_CHECK_EQUAL(Errors.size(),2);
    BOOST_CHECK_EQUAL(RenderedStr,"This is a file with &amp;, &nbsp; and no %\n");
  }

  {
    openfluid::tools::TemplateProcessor TplProc("%%","%%");
    openfluid::tools::TemplateProcessor::Errors Errors;

    auto RenderedStr = TplProc.renderFile(TPInputPath+"/sim.src/README.md",Data,Errors);
    BOOST_CHECK(Errors.empty());
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_renderfiletofile)
{
  openfluid::tools::FilesystemPath(TPOutputPath+"/sim.src").removeDirectory();

  const openfluid::tools::TemplateProcessor::Data Data = {
    {"WAREID","sim.test.id"},
    {"WAREDESCRIPTION","This is the sim.test.id simulator"}
  };


  {
    openfluid::tools::TemplateProcessor TplProc("%%","%%");
    openfluid::tools::TemplateProcessor::Errors Errors;

    TplProc.renderFile(TPInputPath+"/sim.src/README.md",TPOutputPath+"/sim.src/README.md",
                       Data,Errors);
    BOOST_CHECK(Errors.empty());
    BOOST_CHECK(openfluid::tools::Path(TPOutputPath+"/sim.src/README.md").isFile());
    BOOST_CHECK(compare(TPOutputPath+"/sim.src/README.md",
                        "# sim.test.id simulator\n\nThis is the sim.test.id simulator\n"));
  }
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_renderdir)
{
  for (const auto& ID : {"sim.test.1","sim.test.2","sim.test.x"})
  {
    openfluid::tools::FilesystemPath(TPOutputPath+"/"+ID).removeDirectory();

    const openfluid::tools::TemplateProcessor::Data Data = {
      {"WAREID",ID},
      {"WAREDESCRIPTION",std::string("This is the ")+ID+std::string(" simulator")},
      {"ROOTCPPFILENAME","WareMain.cpp"},
      {"CLASSNAME","WareMain"},
      {"SIMSCHEDRETURN","DefaultDeltaT()"}
    };


    {
      openfluid::tools::TemplateProcessor TplProc("%%","%%");
      openfluid::tools::TemplateProcessor::Errors Errors;

      TplProc.renderDirectory(TPInputPath+"/sim.src",TPOutputPath+"/"+Data.at("WAREID"),
                              Data,Errors);

      BOOST_CHECK(Errors.empty());
      BOOST_CHECK(compare(TPOutputPath+"/"+Data.at("WAREID")+"/README.md",
                          "# "+Data.at("WAREID")+" simulator\n\nThis is the "+Data.at("WAREID")+" simulator\n"));
    }
  }
}
