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
 @file WareSrcFactory_TEST.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_WareSrcManager


#include <boost/test/unit_test.hpp>

#include <QDir>
#include <QTextStream>

#include <openfluid/waresdev/WareSrcFactory.hpp>
#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/config.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(create_files_sim)
{
  openfluid::base::Environment::init();

  QDir CommonTemplatesDir = QDir(
      QString("%1/%2/templates").arg(
          QString::fromStdString(openfluid::base::Environment::getInstallPrefix())).arg(
          QString::fromStdString(openfluid::config::SHARE_WARESDEV_INSTALL_PATH)));
  QDir TypedTemplatesDir = QDir(CommonTemplatesDir.absoluteFilePath("simulators"));

  QString WareTypePath = openfluid::waresdev::WareSrcManager::instance()->getWareTypePath(
      openfluid::ware::WareType::SIMULATOR);
  QDir WareTypeDir = QDir(WareTypePath);

  openfluid::tools::emptyDirectoryRecursively(WareTypePath.toStdString());

  openfluid::waresdev::WareSrcFactory Factory(openfluid::ware::WareType::SIMULATOR);

  QString NewFilePath, ErrMsg;
  openfluid::waresdev::WareSrcFactory::Replacements R;
  R.ClassName = "MySim";
  R.RootCppFilename = "MySim.cpp";
  R.Sim2docModeIndex = 1;  //"AUTO"
  R.Sim2docInstall = true;

  BOOST_CHECK_EQUAL(Factory.createCMakeListsFile(NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createJsonFile(NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createCppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createHppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createParamUiCppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createParamUiHppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createCmakeConfigFile(R, NewFilePath, ErrMsg), false);

  Factory.setWareId("wrongid");
  BOOST_CHECK_EQUAL(Factory.createCMakeListsFile(NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createJsonFile(NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createCppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createHppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createParamUiCppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createParamUiHppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createCmakeConfigFile(R, NewFilePath, ErrMsg), false);

  WareTypeDir.mkdir("sim.id");
  QDir SimDir(WareTypeDir.filePath("sim.id"));
  Factory.setWareId("sim.id");

  BOOST_CHECK_EQUAL(Factory.createCMakeListsFile(NewFilePath, ErrMsg), true);
  BOOST_CHECK(SimDir.exists("CMakeLists.txt"));
  QFile CMakeTpl(TypedTemplatesDir.filePath("CMakeLists.txt.tpl"));
  QFile CMakeResult(SimDir.filePath("CMakeLists.txt"));
  CMakeTpl.open(QIODevice::ReadOnly | QIODevice::Text);
  CMakeResult.open(QIODevice::ReadOnly | QIODevice::Text);
  BOOST_CHECK_EQUAL(QTextStream(&CMakeTpl).readAll().toStdString(), QTextStream(&CMakeResult).readAll().toStdString());
  CMakeTpl.close();
  CMakeResult.close();

  BOOST_CHECK_EQUAL(Factory.createJsonFile(NewFilePath, ErrMsg), true);
  BOOST_CHECK(SimDir.exists("wareshub.json"));
  QFile JsonTpl(CommonTemplatesDir.filePath("wareshub.json.tpl"));
  QFile JsonResult(SimDir.filePath("wareshub.json"));
  JsonTpl.open(QIODevice::ReadOnly | QIODevice::Text);
  JsonResult.open(QIODevice::ReadOnly | QIODevice::Text);
  BOOST_CHECK_EQUAL(QTextStream(&JsonTpl).readAll().toStdString(), QTextStream(&JsonResult).readAll().toStdString());
  JsonTpl.close();
  JsonResult.close();

  BOOST_CHECK_EQUAL(Factory.createCmakeConfigFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(SimDir.exists("CMake.in.config"));
  QFile CMakeConfigResult(SimDir.filePath("CMake.in.config"));
  CMakeConfigResult.open(QIODevice::ReadOnly | QIODevice::Text);
  QString CMakeConfigFileContent = QTextStream(&CMakeConfigResult).readAll();
  BOOST_CHECK(!CMakeConfigFileContent.contains("%%"));
  BOOST_CHECK(CMakeConfigFileContent.contains("SET(SIM_ID \"sim.id\")"));
  BOOST_CHECK(CMakeConfigFileContent.contains("SET(SIM_CPP MySim.cpp)"));
  BOOST_CHECK(CMakeConfigFileContent.contains("SET(SIM_PARAMSUI_ENABLED OFF)"));
  BOOST_CHECK(CMakeConfigFileContent.contains("SET(SIM_PARAMSUI_CPP )"));
  CMakeConfigResult.close();

  BOOST_CHECK_EQUAL(Factory.createCppFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(SimDir.exists("MySim.cpp"));
  QFile CppResult(SimDir.filePath("MySim.cpp"));
  CppResult.open(QIODevice::ReadOnly | QIODevice::Text);
  QString CppFileContent = QTextStream(&CppResult).readAll();
  BOOST_CHECK(!CppFileContent.contains("%%"));
  BOOST_CHECK(CppFileContent.contains("file MySim.cpp"));
  BOOST_CHECK(CppFileContent.contains("BEGIN_SIMULATOR_SIGNATURE(\"sim.id\")"));
  BOOST_CHECK(CppFileContent.contains("class MySim : public openfluid::ware::PluggableSimulator"));
  CppResult.close();

  R.ParamsUiClassname = "MyWidget";
  R.ParamsUiRootCppFilename = "MyWidget.cpp";
  R.ParamsUiRootHppFilename = openfluid::waresdev::WareSrcFactory::getHppFilename(R.ParamsUiRootCppFilename);
  R.ParamsUiHeaderGuard = openfluid::waresdev::WareSrcFactory::getHeaderGuard(R.ParamsUiRootHppFilename);

  BOOST_CHECK_EQUAL(Factory.createParamUiCppFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(SimDir.exists("MyWidget.cpp"));
  QFile CppUiResult(SimDir.filePath("MyWidget.cpp"));
  CppUiResult.open(QIODevice::ReadOnly | QIODevice::Text);
  QString CppUiFileContent = QTextStream(&CppUiResult).readAll();
  BOOST_CHECK(!CppUiFileContent.contains("%%"));
  BOOST_CHECK(CppUiFileContent.contains("file MyWidget.cpp"));
  BOOST_CHECK(CppUiFileContent.contains("#include \"MyWidget.hpp\""));
  BOOST_CHECK(CppUiFileContent
                .contains("MyWidget::MyWidget() : openfluid::builderext::PluggableParameterizationExtension()"));
  CppUiResult.close();

  BOOST_CHECK_EQUAL(Factory.createParamUiHppFile(R, NewFilePath, ErrMsg), true);
  QFile HppUiResult(SimDir.filePath("MyWidget.hpp"));
  HppUiResult.open(QIODevice::ReadOnly | QIODevice::Text);
  QString HppUiFileContent = QTextStream(&HppUiResult).readAll();
  BOOST_CHECK(!HppUiFileContent.contains("%%"));
  BOOST_CHECK(HppUiFileContent.contains("file MyWidget.hpp"));
  BOOST_CHECK(HppUiFileContent.contains("#ifndef __MYWIDGET_HPP__"));
  BOOST_CHECK(HppUiFileContent
                .contains("class MyWidget: public openfluid::builderext::PluggableParameterizationExtension"));
  HppUiResult.close();

  openfluid::tools::Filesystem::removeDirectory(WareTypePath.toStdString());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(create_files_bext)
{
  openfluid::base::Environment::init();

  QDir CommonTemplatesDir = QDir(
      QString("%1/%2/templates").arg(
          QString::fromStdString(openfluid::base::Environment::getInstallPrefix())).arg(
          QString::fromStdString(openfluid::config::SHARE_WARESDEV_INSTALL_PATH)));
  QDir TypedTemplatesDir = QDir(CommonTemplatesDir.absoluteFilePath("builderexts"));

  QString WareTypePath = openfluid::waresdev::WareSrcManager::instance()->getWareTypePath(
      openfluid::ware::WareType::BUILDEREXT);
  QDir WareTypeDir = QDir(WareTypePath);

  openfluid::tools::emptyDirectoryRecursively(WareTypePath.toStdString());

  openfluid::waresdev::WareSrcFactory Factory(openfluid::ware::WareType::BUILDEREXT);

  QString NewFilePath, ErrMsg;
  openfluid::waresdev::WareSrcFactory::Replacements R;
  R.ClassName = "MyBext";
  R.RootCppFilename = "MyBext.cpp";
  R.RootHppFilename = openfluid::waresdev::WareSrcFactory::getHppFilename(R.RootCppFilename);
  R.HppHeaderGuard = openfluid::waresdev::WareSrcFactory::getHeaderGuard(R.RootHppFilename);
  R.BuilderExtCategoryIndex = 0;  //"openfluid::builderext::CAT_SPATIAL"
  R.BuilderExtMenuText = "bla bla";

  BOOST_CHECK_EQUAL(Factory.createCMakeListsFile(NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createJsonFile(NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createCppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createHppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createParamUiCppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createParamUiHppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createCmakeConfigFile(R, NewFilePath, ErrMsg), false);

  Factory.setWareId("wrongid");
  BOOST_CHECK_EQUAL(Factory.createCMakeListsFile(NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createJsonFile(NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createCppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createHppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createParamUiCppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createParamUiHppFile(R, NewFilePath, ErrMsg), false);
  BOOST_CHECK_EQUAL(Factory.createCmakeConfigFile(R, NewFilePath, ErrMsg), false);

  WareTypeDir.mkdir("bext.id");
  QDir BextDir(WareTypeDir.filePath("bext.id"));
  Factory.setWareId("bext.id");

  BOOST_CHECK_EQUAL(Factory.createCMakeListsFile(NewFilePath, ErrMsg), true);
  BOOST_CHECK(BextDir.exists("CMakeLists.txt"));
  QFile CMakeTpl(TypedTemplatesDir.filePath("CMakeLists.txt.tpl"));
  QFile CMakeResult(BextDir.filePath("CMakeLists.txt"));
  CMakeTpl.open(QIODevice::ReadOnly | QIODevice::Text);
  CMakeResult.open(QIODevice::ReadOnly | QIODevice::Text);
  BOOST_CHECK_EQUAL(QTextStream(&CMakeTpl).readAll().toStdString(), QTextStream(&CMakeResult).readAll().toStdString());
  CMakeTpl.close();
  CMakeResult.close();

  BOOST_CHECK_EQUAL(Factory.createJsonFile(NewFilePath, ErrMsg), true);
  BOOST_CHECK(BextDir.exists("wareshub.json"));
  QFile JsonTpl(CommonTemplatesDir.filePath("wareshub.json.tpl"));
  QFile JsonResult(BextDir.filePath("wareshub.json"));
  JsonTpl.open(QIODevice::ReadOnly | QIODevice::Text);
  JsonResult.open(QIODevice::ReadOnly | QIODevice::Text);
  BOOST_CHECK_EQUAL(QTextStream(&JsonTpl).readAll().toStdString(), QTextStream(&JsonResult).readAll().toStdString());
  JsonTpl.close();
  JsonResult.close();

  BOOST_CHECK_EQUAL(Factory.createCmakeConfigFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(BextDir.exists("CMake.in.config"));
  QFile CMakeConfigResult(BextDir.filePath("CMake.in.config"));
  CMakeConfigResult.open(QIODevice::ReadOnly | QIODevice::Text);
  QString CMakeConfigFileContent = QTextStream(&CMakeConfigResult).readAll();
  BOOST_CHECK(!CMakeConfigFileContent.contains("%%"));
  BOOST_CHECK(CMakeConfigFileContent.contains("SET(BEXT_ID \"bext.id\")"));
  BOOST_CHECK(CMakeConfigFileContent.contains("SET(BEXT_CPP MyBext.cpp)"));
  CMakeConfigResult.close();

  R.BuilderExtModeIndex = 0;  //openfluid::builderext::TYPE_MODAL

  BOOST_CHECK_EQUAL(Factory.createCppFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(BextDir.exists("MyBext.cpp"));
  QFile CppResult(BextDir.filePath("MyBext.cpp"));
  CppResult.open(QIODevice::ReadOnly | QIODevice::Text);
  QString CppFileContent = QTextStream(&CppResult).readAll();
  BOOST_CHECK(!CppFileContent.contains("%%"));
  BOOST_CHECK(CppFileContent.contains("file MyBext.cpp"));
  BOOST_CHECK(CppFileContent.contains("#include \"MyBext.hpp\""));
  BOOST_CHECK(CppFileContent.contains("BEGIN_BUILDEREXT_SIGNATURE(\"bext.id\", openfluid::builderext::MODE_MODAL)"));
  BOOST_CHECK(CppFileContent.contains("DECLARE_CATEGORY(openfluid::builderext::CAT_SPATIAL)"));
  BOOST_CHECK(CppFileContent.contains("DECLARE_MENUTEXT(\"bla bla\")"));
  BOOST_CHECK(CppFileContent.contains("MyBext::MyBext()"));
  CppResult.remove();

  BOOST_CHECK_EQUAL(Factory.createHppFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(BextDir.exists("MyBext.hpp"));
  QFile HppResult(BextDir.filePath("MyBext.hpp"));
  HppResult.open(QIODevice::ReadOnly | QIODevice::Text);
  QString HppFileContent = QTextStream(&HppResult).readAll();
  BOOST_CHECK(!HppFileContent.contains("%%"));
  BOOST_CHECK(HppFileContent.contains("file MyBext.hpp"));
  BOOST_CHECK(HppFileContent.contains("#ifndef __MYBEXT_HPP__"));
  BOOST_CHECK(HppFileContent.contains("class MyBext : public openfluid::builderext::PluggableModalExtension"));
  HppResult.remove();


  R.BuilderExtModeIndex = 1;  //openfluid::builderext::TYPE_MODELESS

  BOOST_CHECK_EQUAL(Factory.createCppFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(BextDir.exists("MyBext.cpp"));
  CppResult.open(QIODevice::ReadOnly | QIODevice::Text);
  CppFileContent = QTextStream(&CppResult).readAll();
  BOOST_CHECK(!CppFileContent.contains("%%"));
  BOOST_CHECK(CppFileContent.contains("file MyBext.cpp"));
  BOOST_CHECK(CppFileContent.contains("#include \"MyBext.hpp\""));
  BOOST_CHECK(CppFileContent.contains("BEGIN_BUILDEREXT_SIGNATURE(\"bext.id\", openfluid::builderext::MODE_MODELESS)"));
  BOOST_CHECK(CppFileContent.contains("DECLARE_CATEGORY(openfluid::builderext::CAT_SPATIAL)"));
  BOOST_CHECK(CppFileContent.contains("DECLARE_MENUTEXT(\"bla bla\")"));
  BOOST_CHECK(CppFileContent.contains("MyBext::MyBext()"));
  CppResult.remove();

  BOOST_CHECK_EQUAL(Factory.createHppFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(BextDir.exists("MyBext.hpp"));
  HppResult.open(QIODevice::ReadOnly | QIODevice::Text);
  HppFileContent = QTextStream(&HppResult).readAll();
  BOOST_CHECK(!HppFileContent.contains("%%"));
  BOOST_CHECK(HppFileContent.contains("file MyBext.hpp"));
  BOOST_CHECK(HppFileContent.contains("#ifndef __MYBEXT_HPP__"));
  BOOST_CHECK(HppFileContent.contains("class MyBext : public openfluid::builderext::PluggableModelessExtension"));
  HppResult.remove();


  R.BuilderExtModeIndex = 2;  //openfluid::builderext::TYPE_WORKSPACE

  BOOST_CHECK_EQUAL(Factory.createCppFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(BextDir.exists("MyBext.cpp"));
  CppResult.open(QIODevice::ReadOnly | QIODevice::Text);
  CppFileContent = QTextStream(&CppResult).readAll();
  BOOST_CHECK(!CppFileContent.contains("%%"));
  BOOST_CHECK(CppFileContent.contains("file MyBext.cpp"));
  BOOST_CHECK(CppFileContent.contains("#include \"MyBext.hpp\""));
  BOOST_CHECK(
      CppFileContent.contains("BEGIN_BUILDEREXT_SIGNATURE(\"bext.id\", openfluid::builderext::MODE_WORKSPACE)"));
  BOOST_CHECK(CppFileContent.contains("DECLARE_CATEGORY(openfluid::builderext::CAT_SPATIAL)"));
  BOOST_CHECK(CppFileContent.contains("DECLARE_MENUTEXT(\"bla bla\")"));
  BOOST_CHECK(CppFileContent.contains("MyBext::MyBext()"));
  CppResult.close();

  BOOST_CHECK_EQUAL(Factory.createHppFile(R, NewFilePath, ErrMsg), true);
  BOOST_CHECK(BextDir.exists("MyBext.hpp"));
  HppResult.open(QIODevice::ReadOnly | QIODevice::Text);
  HppFileContent = QTextStream(&HppResult).readAll();
  BOOST_CHECK(!HppFileContent.contains("%%"));
  BOOST_CHECK(HppFileContent.contains("file MyBext.hpp"));
  BOOST_CHECK(HppFileContent.contains("#ifndef __MYBEXT_HPP__"));
  BOOST_CHECK(HppFileContent.contains("class MyBext : public openfluid::builderext::PluggableWorkspaceExtension"));
  HppResult.close();

  openfluid::tools::Filesystem::removeDirectory(WareTypePath.toStdString());
}

