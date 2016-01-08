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
 @file WareSrcManager_TEST.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_WareSrcManager
#include <boost/test/unit_test.hpp>

#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/config.hpp>

#include <QDir>


// =====================================================================
// =====================================================================

struct F
{
    QString m_WorkspacePath;
    QString m_WaresdevPath;
    QString m_SimulatorsPath;

    QStringList m_RealDirs;
    QStringList m_RealFiles;

    F()
    {
      m_WorkspacePath = openfluid::base::PreferencesManager::instance()->getBuilderWorkspacePath();

      m_WaresdevPath = QString("%1/%2").arg(m_WorkspacePath).arg(
          QString::fromStdString(openfluid::config::WARESDEV_PATH));

      m_SimulatorsPath = QString("%1/%2").arg(m_WaresdevPath).arg(
          QString::fromStdString(openfluid::config::SIMULATORS_PATH));

      openfluid::tools::Filesystem::removeDirectory(m_WorkspacePath.toStdString());
    }

    ~F()
    {
      openfluid::tools::Filesystem::removeDirectory(m_WorkspacePath.toStdString());
    }

    void createTestFiles()
    {
      m_RealDirs.insert(0, QString("%1/nowaresdevdir").arg(m_WorkspacePath));
      m_RealDirs.insert(1, QString("%1/nowaretype").arg(m_WaresdevPath));
      m_RealDirs.insert(2, QString("%1/nowaretype/ware1").arg(m_WaresdevPath));
      m_RealDirs.insert(3, QString("%1/ware1").arg(m_SimulatorsPath));
      m_RealDirs.insert(4, QString("%1/ware1/subdir1").arg(m_SimulatorsPath));
      m_RealDirs.insert(5, QString("%1/ware1/subdir1/subdir2").arg(m_SimulatorsPath));
      m_RealDirs.insert(6, QString("%1/ware2").arg(m_SimulatorsPath));
      m_RealDirs.insert(7, QString("%1/ware2/subdir1").arg(m_SimulatorsPath));

      m_RealFiles.insert(0, QString("%1/file0.txt").arg(m_WorkspacePath));
      m_RealFiles.insert(1, QString("%1/nowaresdevdir/file1.txt").arg(m_WorkspacePath));
      m_RealFiles.insert(2, QString("%1/file2.txt").arg(m_WaresdevPath));
      m_RealFiles.insert(3, QString("%1/nowaretype/file3.txt").arg(m_WaresdevPath));
      m_RealFiles.insert(4, QString("%1/nowaretype/ware1/file4.txt").arg(m_WaresdevPath));
      m_RealFiles.insert(5, QString("%1/file5.txt").arg(m_SimulatorsPath));
      m_RealFiles.insert(6, QString("%1/ware1/file6.txt").arg(m_SimulatorsPath));
      m_RealFiles.insert(7, QString("%1/ware1/subdir1/file7.txt").arg(m_SimulatorsPath));
      m_RealFiles.insert(8, QString("%1/ware1/subdir1/subdir2/file8.txt").arg(m_SimulatorsPath));
      m_RealFiles.insert(9, QString("%1/ware2/file9.txt").arg(m_SimulatorsPath));
      m_RealFiles.insert(10, QString("%1/ware2/subdir1/file10.txt").arg(m_SimulatorsPath));

      QDir Dir(m_WorkspacePath);

      for(const QString& D : m_RealDirs)
        Dir.mkpath(D);

      for(const QString& F : m_RealFiles)
        QFile(F).open(QIODevice::ReadWrite);
    }

  };


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(constructor,F)
{
  openfluid::base::Environment::init();

  BOOST_CHECK(!QDir(m_WaresdevPath).exists());
  BOOST_CHECK(!QDir(m_SimulatorsPath).exists());

  openfluid::waresdev::WareSrcManager::instance();

  BOOST_CHECK(QDir(m_WaresdevPath).exists());
  BOOST_CHECK(QDir(m_SimulatorsPath).exists());
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(getWareTypePath,F)
{
  openfluid::base::Environment::init();

  BOOST_CHECK_EQUAL(
      openfluid::waresdev::WareSrcManager::instance()->getWareTypePath(openfluid::ware::WareType::SIMULATOR)
          .toStdString(),
      m_SimulatorsPath.toStdString());
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(getPathInfo,F)
{
  openfluid::base::Environment::init();

  createTestFiles();


  openfluid::waresdev::WareSrcManager* Manager = openfluid::waresdev::WareSrcManager::instance();

  openfluid::waresdev::WareSrcManager::PathInfo Info;


  Info = Manager->getPathInfo("toto/warename");
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo("toto");
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealDirs.at(0));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealDirs.at(1));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealDirs.at(2));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealDirs.at(3));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, true);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);
  BOOST_CHECK_EQUAL(Info.m_AbsolutePath.toStdString(), QDir(m_RealDirs.at(3)).absolutePath().toStdString());
  BOOST_CHECK(Info.m_WareType == openfluid::ware::WareType::SIMULATOR);
  BOOST_CHECK_EQUAL(Info.m_WareName.toStdString(), "ware1");
  BOOST_CHECK_EQUAL(Info.m_AbsolutePathOfWare.toStdString(), QString("%1/ware1").arg(m_SimulatorsPath).toStdString());

  Info = Manager->getPathInfo(m_RealDirs.at(4));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealDirs.at(5));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealDirs.at(6));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, true);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);
  BOOST_CHECK_EQUAL(Info.m_AbsolutePathOfWare.toStdString(), QString("%1/ware2").arg(m_SimulatorsPath).toStdString());

  Info = Manager->getPathInfo(m_RealDirs.at(7));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);


  Info = Manager->getPathInfo("/toto/warename/file.txt");
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo("file.txt");
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo("file.txt");
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, false);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealFiles.at(0));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealFiles.at(1));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealFiles.at(2));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealFiles.at(3));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealFiles.at(4));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealFiles.at(5));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, false);

  Info = Manager->getPathInfo(m_RealFiles.at(6));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, true);
  BOOST_CHECK_EQUAL(Info.m_AbsolutePath.toStdString(), QDir(m_RealFiles.at(6)).absolutePath().toStdString());
  BOOST_CHECK(Info.m_WareType == openfluid::ware::WareType::SIMULATOR);
  BOOST_CHECK_EQUAL(Info.m_WareName.toStdString(), "ware1");
  BOOST_CHECK_EQUAL(Info.m_RelativePathToWareDir.toStdString(), "file6.txt");
  BOOST_CHECK_EQUAL(Info.m_AbsolutePathOfWare.toStdString(), QString("%1/ware1").arg(m_SimulatorsPath).toStdString());
  BOOST_CHECK_EQUAL(Info.m_FileName.toStdString(), "file6.txt");

  Info = Manager->getPathInfo(m_RealFiles.at(7));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, true);
  BOOST_CHECK_EQUAL(Info.m_AbsolutePath.toStdString(), QDir(m_RealFiles.at(7)).absolutePath().toStdString());
  BOOST_CHECK(Info.m_WareType == openfluid::ware::WareType::SIMULATOR);
  BOOST_CHECK_EQUAL(Info.m_WareName.toStdString(), "ware1");
  BOOST_CHECK_EQUAL(Info.m_RelativePathToWareDir.toStdString(), "subdir1/file7.txt");
  BOOST_CHECK_EQUAL(Info.m_AbsolutePathOfWare.toStdString(), QString("%1/ware1").arg(m_SimulatorsPath).toStdString());
  BOOST_CHECK_EQUAL(Info.m_FileName.toStdString(), "file7.txt");

  Info = Manager->getPathInfo(m_RealFiles.at(8));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, true);
  BOOST_CHECK_EQUAL(Info.m_AbsolutePath.toStdString(), QDir(m_RealFiles.at(8)).absolutePath().toStdString());
  BOOST_CHECK(Info.m_WareType == openfluid::ware::WareType::SIMULATOR);
  BOOST_CHECK_EQUAL(Info.m_WareName.toStdString(), "ware1");
  BOOST_CHECK_EQUAL(Info.m_RelativePathToWareDir.toStdString(), "subdir1/subdir2/file8.txt");
  BOOST_CHECK_EQUAL(Info.m_AbsolutePathOfWare.toStdString(), QString("%1/ware1").arg(m_SimulatorsPath).toStdString());
  BOOST_CHECK_EQUAL(Info.m_FileName.toStdString(), "file8.txt");

  Info = Manager->getPathInfo(m_RealFiles.at(9));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, true);
  BOOST_CHECK_EQUAL(Info.m_AbsolutePath.toStdString(), QDir(m_RealFiles.at(9)).absolutePath().toStdString());
  BOOST_CHECK(Info.m_WareType == openfluid::ware::WareType::SIMULATOR);
  BOOST_CHECK_EQUAL(Info.m_WareName.toStdString(), "ware2");
  BOOST_CHECK_EQUAL(Info.m_RelativePathToWareDir.toStdString(), "file9.txt");
  BOOST_CHECK_EQUAL(Info.m_AbsolutePathOfWare.toStdString(), QString("%1/ware2").arg(m_SimulatorsPath).toStdString());
  BOOST_CHECK_EQUAL(Info.m_FileName.toStdString(), "file9.txt");

  Info = Manager->getPathInfo(m_RealFiles.at(10));
  BOOST_CHECK_EQUAL(Info.m_IsInCurrentWorkspace, true);
  BOOST_CHECK_EQUAL(Info.m_isAWare, false);
  BOOST_CHECK_EQUAL(Info.m_isAWareFile, true);
  BOOST_CHECK_EQUAL(Info.m_AbsolutePath.toStdString(), QDir(m_RealFiles.at(10)).absolutePath().toStdString());
  BOOST_CHECK(Info.m_WareType == openfluid::ware::WareType::SIMULATOR);
  BOOST_CHECK_EQUAL(Info.m_WareName.toStdString(), "ware2");
  BOOST_CHECK_EQUAL(Info.m_RelativePathToWareDir.toStdString(), "subdir1/file10.txt");
  BOOST_CHECK_EQUAL(Info.m_AbsolutePathOfWare.toStdString(), QString("%1/ware2").arg(m_SimulatorsPath).toStdString());
  BOOST_CHECK_EQUAL(Info.m_FileName.toStdString(), "file10.txt");
}


// =====================================================================
// =====================================================================


