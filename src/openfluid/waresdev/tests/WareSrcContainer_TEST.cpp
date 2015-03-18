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
 @file WareSrcContainer_TEST.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_WareSrcContainer
#include <boost/test/unit_test.hpp>

#include <openfluid/waresdev/WareSrcContainer.hpp>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/tools/Filesystem.cpp>
#include <openfluid/config.hpp>

#include <QDir>


// =====================================================================
// =====================================================================

struct F
{
    QString m_CMakeConfigFile;

    QString m_WorkspacePath;
    QString m_WaresdevPath;
    QString m_SimulatorsPath;

    QStringList m_RealDirs;
    QStringList m_RealFiles;

    F()
    {
      m_CMakeConfigFile = QString::fromStdString(openfluid::config::WARESDEV_CMAKE_USERFILE);

      m_WorkspacePath = openfluid::base::PreferencesManager::instance()->getWorkspacePath();

      m_WaresdevPath = QString("%1/%2").arg(m_WorkspacePath).arg(
          QString::fromStdString(openfluid::config::WARESDEV_SUBDIR));

      m_SimulatorsPath = QString("%1/%2").arg(m_WaresdevPath).arg(
          QString::fromStdString(openfluid::config::SIMULATORS_PLUGINS_SUBDIR));
    }

    void createTestFiles()
    {
      m_RealDirs.insert(0, QString("%1/ware_ok").arg(m_SimulatorsPath));
      // right CMake content
      m_RealFiles.insert(0, QString("%1/ware_ok/%2").arg(m_SimulatorsPath).arg(m_CMakeConfigFile));
      m_RealFiles.insert(1, QString("%1/ware_ok/main.cpp").arg(m_SimulatorsPath));
      m_RealFiles.insert(2, QString("%1/ware_ok/other.cpp").arg(m_SimulatorsPath));

      m_RealDirs.insert(1, QString("%1/ware_wrongcmake").arg(m_SimulatorsPath));
      // wrong CMake content
      m_RealFiles.insert(3, QString("%1/ware_wrongcmake/%2").arg(m_SimulatorsPath).arg(m_CMakeConfigFile));
      m_RealFiles.insert(4, QString("%1/ware_wrongcmake/main.cpp").arg(m_SimulatorsPath));
      m_RealFiles.insert(5, QString("%1/ware_wrongcmake/other.txt").arg(m_SimulatorsPath));

      m_RealDirs.insert(2, QString("%1/ware_nomaincpp").arg(m_SimulatorsPath));
      // right CMake content
      m_RealFiles.insert(6, QString("%1/ware_nomaincpp/%2").arg(m_SimulatorsPath).arg(m_CMakeConfigFile));
      m_RealFiles.insert(7, QString("%1/ware_nomaincpp/other.cpp").arg(m_SimulatorsPath));

      m_RealDirs.insert(3, QString("%1/ware_nocmake").arg(m_SimulatorsPath));
      m_RealFiles.insert(8, QString("%1/ware_nocmake/main.cpp").arg(m_SimulatorsPath));
      m_RealFiles.insert(9, QString("%1/ware_nocmake/other.cpp").arg(m_SimulatorsPath));

      m_RealDirs.insert(4, QString("%1/ware_nocpp").arg(m_SimulatorsPath));
      // right CMake content
      m_RealFiles.insert(10, QString("%1/ware_nocpp/%2").arg(m_SimulatorsPath).arg(m_CMakeConfigFile));

      m_RealDirs.insert(5, QString("%1/ware_empty").arg(m_SimulatorsPath));

      m_RealDirs.insert(6, QString("%1/ware_empty2").arg(m_SimulatorsPath));
      m_RealDirs.insert(7, QString("%1/ware_empty2/subdir").arg(m_SimulatorsPath));
      m_RealFiles.insert(11, QString("%1/ware_empty2/subdir/%2").arg(m_SimulatorsPath).arg(m_CMakeConfigFile));

      QDir Dir(m_WorkspacePath);

      foreach(QString D,m_RealDirs)Dir.mkpath(D);

      foreach(QString F,m_RealFiles)QFile(F).open(QIODevice::ReadWrite);

      QStringList RightCmake;
      RightCmake << m_RealFiles.at(0) << m_RealFiles.at(6) << m_RealFiles.at(10);

      foreach(QString F, RightCmake){
      QFile File(F);
      if(File.open(QIODevice::ReadWrite))
      File.write("# list of CPP files, the sim2doc tag must be contained in the first one\n"
          "# ex: SET(SIM_CPP MySim.cpp)\n"
          "SET(SIM_CPP main.cpp)\n");
    }

      QFile File(m_RealFiles.at(3));
      if (File.open(QIODevice::ReadWrite))
        File.write("# list of CPP files, the sim2doc tag must be contained in the first one\n"
                   "# ex: SET(SIM_CPP MySim.cpp)\n"
                   "SET(SIM_CPP wrongmain.cpp)\n");
    }

};


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(searchMainCppFileName)
{
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("AEIOU").toStdString(), "");

  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP file.cpp )").toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(SIM_CPP file.cpp )").toStdString(), "file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(SIM_CPP file.cpp)").toStdString(),
                    "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName(" SET (SIM_CPP file.cpp ) ").toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP file.cpp other_file.cpp)")
      .toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP file.cpp other_file.cpp) ")
      .toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(SIM_CPP file.cpp other_file.cpp)")
      .toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName(
          "# list of CPP files, the sim2doc tag must be contained in the first one\n"
          "# ex: SET(SIM_CPP MySimulator.cpp)\n"
          "SET(SIM_CPP file.cpp other_file.cpp)\n").toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName(
          "# list of CPP files, the sim2doc tag must be contained in the first one\n"
          "# ex: SET(SIM_CPP MySimulator.cpp)\n"
          "#SET(SIM_CPP file.cpp)\n").toStdString(),
      "");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP file.hpp)").toStdString(), "");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP fi-le.cpp)").toStdString(),
      "fi-le.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP fi_le.cpp)").toStdString(),
      "fi_le.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (SIM_CPP fi.le.cpp)").toStdString(),
      "fi.le.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(SIM_CPP piege.cppabc.cpp)").toStdString(),
      "piege.cppabc.cpp");

  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (OBS_CPP file.cpp )").toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(OBS_CPP file.cpp )").toStdString(), "file.cpp");
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(OBS_CPP file.cpp)").toStdString(),
                    "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName(" SET (OBS_CPP file.cpp ) ").toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (OBS_CPP file.cpp other_file.cpp)")
      .toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET (OBS_CPP file.cpp other_file.cpp) ")
      .toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchMainCppFileName("SET(OBS_CPP file.cpp other_file.cpp)")
      .toStdString(),
      "file.cpp");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(searchUiParamCppFileName)
{
  BOOST_CHECK_EQUAL(openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("AEIOU").toStdString(), "");

  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (OBS_PARAMSUI_CPP file.cpp )").toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET(OBS_PARAMSUI_CPP file.cpp )").toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET(OBS_PARAMSUI_CPP file.cpp)").toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName(" SET (OBS_PARAMSUI_CPP file.cpp ) ")
      .toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (OBS_PARAMSUI_CPP file.cpp other_file.cpp)")
          .toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (OBS_PARAMSUI_CPP file.cpp other_file.cpp) ")
          .toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET(OBS_PARAMSUI_CPP file.cpp other_file.cpp)")
          .toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName(
          "# list of CPP files for parameterization widget, if any\n"
          "# ex: SET(OBS_PARAMSUI_CPP MyWidget.cpp)\n"
          "SET(OBS_PARAMSUI_CPP file.cpp other_file.cpp)\n").toStdString(),
      "file.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName(
          "# list of CPP files for parameterization widget, if any\n"
          "# ex: SET(OBS_PARAMSUI_CPP MyWidget.cpp)\n"
          "#SET(OBS_PARAMSUI_CPP file.cpp)\n").toStdString(),
      "");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (SIM_PARAMSUI_CPP file.hpp)").toStdString(),
      "");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (SIM_PARAMSUI_CPP fi-le.cpp)").toStdString(),
      "fi-le.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (SIM_PARAMSUI_CPP fi_le.cpp)").toStdString(),
      "fi_le.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET (SIM_PARAMSUI_CPP fi.le.cpp)").toStdString(),
      "fi.le.cpp");
  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcContainer::searchUiParamCppFileName("SET(SIM_PARAMSUI_CPP piege.cppabc.cpp)")
          .toStdString(),
      "piege.cppabc.cpp");
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(getDefaultFiles,F)
{
  openfluid::tools::removeDirectoryRecursively(m_WorkspacePath);

  createTestFiles();


  QStringList List;

  List = openfluid::waresdev::WareSrcContainer(m_RealDirs.at(0), openfluid::waresdev::WareSrcManager::SIMULATOR, "")
      .getDefaultFilesPaths();
  BOOST_CHECK_EQUAL(List.count(), 2);
  BOOST_CHECK_EQUAL(QFileInfo(List.value(0)).fileName().toStdString(), m_CMakeConfigFile.toStdString());
  BOOST_CHECK_EQUAL(QFileInfo(List.value(1)).fileName().toStdString(), "main.cpp");

  List = openfluid::waresdev::WareSrcContainer(m_RealDirs.at(1), openfluid::waresdev::WareSrcManager::SIMULATOR, "")
      .getDefaultFilesPaths();
  BOOST_CHECK_EQUAL(List.count(), 2);
  BOOST_CHECK_EQUAL(QFileInfo(List.value(0)).fileName().toStdString(), m_CMakeConfigFile.toStdString());
  BOOST_CHECK_EQUAL(QFileInfo(List.value(1)).fileName().toStdString(), "main.cpp");

  List = openfluid::waresdev::WareSrcContainer(m_RealDirs.at(2), openfluid::waresdev::WareSrcManager::SIMULATOR, "")
      .getDefaultFilesPaths();
  BOOST_CHECK_EQUAL(List.count(), 2);
  BOOST_CHECK_EQUAL(QFileInfo(List.value(0)).fileName().toStdString(), m_CMakeConfigFile.toStdString());
  BOOST_CHECK_EQUAL(QFileInfo(List.value(1)).fileName().toStdString(), "other.cpp");

  List = openfluid::waresdev::WareSrcContainer(m_RealDirs.at(3), openfluid::waresdev::WareSrcManager::SIMULATOR, "")
      .getDefaultFilesPaths();
  BOOST_CHECK_EQUAL(List.count(), 1);
  BOOST_CHECK_EQUAL(QFileInfo(List.value(0)).fileName().toStdString(), "main.cpp");

  List = openfluid::waresdev::WareSrcContainer(m_RealDirs.at(4), openfluid::waresdev::WareSrcManager::SIMULATOR, "")
      .getDefaultFilesPaths();
  BOOST_CHECK_EQUAL(List.count(), 1);
  BOOST_CHECK_EQUAL(QFileInfo(List.value(0)).fileName().toStdString(), m_CMakeConfigFile.toStdString());

  List = openfluid::waresdev::WareSrcContainer(m_RealDirs.at(5), openfluid::waresdev::WareSrcManager::SIMULATOR, "")
      .getDefaultFilesPaths();
  BOOST_CHECK_EQUAL(List.count(), 0);

  List = openfluid::waresdev::WareSrcContainer(m_RealDirs.at(6), openfluid::waresdev::WareSrcManager::SIMULATOR, "")
      .getDefaultFilesPaths();
  BOOST_CHECK_EQUAL(List.count(), 0);

  openfluid::tools::removeDirectoryRecursively(m_WorkspacePath);
}


// =====================================================================
// =====================================================================


