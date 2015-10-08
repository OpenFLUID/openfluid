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
 @file WaresDevPackage_TEST.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_WaresDevPackage
#include <boost/test/unit_test.hpp>

#include <openfluid/waresdev/WaresDevPackage.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include "tests-config.hpp"

#include <QDir>
#include <QSettings>
#include <QApplication>


// =====================================================================
// =====================================================================


class F
{
  public:

    QStringList ToExportWareFolders = {
                                        "simulators/sim1",
                                        "simulators/sim3",
                                        "builderexts/bext2",
    };
    QStringList NotToExportWareFolders = {
                                           "simulators/sim2",
                                           "builderexts/bext1",
                                           "builderexts/bext3",
    };
    QStringList ToImportWareFolders = {
                                        "simulators/sim3",
                                        "builderexts/bext2"
    };

    QStringList ToExportContentFolders = {
                                           ".git",
                                           ".git/gsubdir"
    };
    QStringList NotToExportContentFolders = {
                                              "_build",
                                              "_build/bsubdir",
                                              "_build_debug"
    };

    QStringList ToExportContentFiles = {
                                         "file.cpp",
                                         "file.hpp",
                                         ".gitignore",

                                         ".git/gfile1",
                                         ".git/gfile2",
                                         ".git/gsubdir/gsubfile"
    };
    QStringList NotToExportContentFiles = {
                                            ".cproject",

                                            "_build/bfile1",
                                            "_build/bfile2",
                                            "_build/bsubdir/bsubfile",

                                            "_build_debug/bdfile1",
                                            "_build_debug/bdfile2",
    };


    QDir WaresDevDir;
    QDir TempOfwdpDir;
    QDir OutOfwdpDir;

    QStringList ToExportWareFoldersPaths;

    F()
    {
      WaresDevDir = QDir(openfluid::waresdev::WareSrcManager::instance()->getWaresdevPath());
      TempOfwdpDir = QDir(getOfwdpPath(openfluid::base::RuntimeEnvironment::instance()->getTempDir(), true));
      OutOfwdpDir = QDir(getOfwdpPath(CONFIGTESTS_OUTPUT_DATA_DIR, true));

      createDummyWaresSources();
    }

    void createDummyWaresSources()
    {
      openfluid::tools::emptyDirectoryRecursively(WaresDevDir.absolutePath().toStdString());

      QStringList WareFolders = ToExportWareFolders + NotToExportWareFolders;
      QStringList ContentFolders = ToExportContentFolders + NotToExportContentFolders;
      QStringList ContentFiles = ToExportContentFiles + NotToExportContentFiles;

      for (const QString& WareFile : WareFolders)
      {
        WaresDevDir.mkpath(WareFile);

        QString WarePath = WaresDevDir.absoluteFilePath(WareFile);

        if (ToExportWareFolders.contains(WareFile))
          ToExportWareFoldersPaths << WarePath;

        QDir WareDir(WarePath);

        for (const QString& WareFolder : ContentFolders)
          WareDir.mkpath(WareDir.absoluteFilePath(WareFolder));

        for (const QString& WareContentFile : ContentFiles)
          QFile(WareDir.absoluteFilePath(WareContentFile)).open(QIODevice::WriteOnly);
      }
    }

    QString getOfwdpPath(std::string MainDirPath, bool RemovedItBefore)
    {
      QDir MainDir(QString::fromStdString(MainDirPath));
      QString OfwdpPath = MainDir.absoluteFilePath("ofwdp");

      if (RemovedItBefore)
      {
        openfluid::tools::emptyDirectoryRecursively(OfwdpPath.toStdString());
        MainDir.rmdir(OfwdpPath);
      }

      return OfwdpPath;
    }
};

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(constructor)
{
  F f;

  openfluid::waresdev::WaresDevExportPackage PkgExport(f.OutOfwdpDir.absoluteFilePath("my_package.ofwdp"),
                                                       f.ToExportWareFoldersPaths,
                                                       "the packager",
                                                       "the description");

  BOOST_CHECK_EQUAL(PkgExport.getWaresPaths().size(), 3);

  PkgExport.exportToPackage();

  BOOST_CHECK(f.TempOfwdpDir.exists("export/my_package"));
  BOOST_CHECK(f.TempOfwdpDir.exists("export/my_package/ofwdp.conf"));

  QSettings FileConfSettings(f.TempOfwdpDir.filePath("export/my_package/ofwdp.conf"), QSettings::IniFormat);
  FileConfSettings.beginGroup("OpenFLUID waresdev package");
  BOOST_CHECK_EQUAL(FileConfSettings.value("packager").toString().toStdString(), "the packager");
  BOOST_CHECK_EQUAL(FileConfSettings.value("description").toString().toStdString(), "the description");
  FileConfSettings.endGroup();

  BOOST_CHECK(f.OutOfwdpDir.exists("my_package.ofwdp"));


  // -> importPkg

  openfluid::waresdev::WaresDevImportPackage PkgImport(f.OutOfwdpDir.absoluteFilePath("my_package.ofwdp"));

  PkgImport.fetchInformation();

  QDir TempPackageDir(f.TempOfwdpDir.filePath("import_file/my_package"));

  BOOST_CHECK(TempPackageDir.exists());

  QStringList FilesInPackage = TempPackageDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
  BOOST_CHECK_EQUAL(FilesInPackage.size(), 1);
  BOOST_CHECK_EQUAL(FilesInPackage[0].toStdString(), "ofwdp.conf");

  for (const QString& WareFile : f.ToExportWareFolders)
  {
    QDir WareDir(TempPackageDir.absoluteFilePath(WareFile));

    BOOST_CHECK(WareDir.exists());

    for (const QString& WareContent : (f.ToExportContentFolders + f.ToExportContentFiles))
      BOOST_CHECK(WareDir.exists(WareContent));

    for (const QString& WareContent : (f.NotToExportContentFolders + f.NotToExportContentFiles))
      BOOST_CHECK(!WareDir.exists(WareContent));
  }

  for (const QString& WareFile : f.NotToExportWareFolders)
  {
    QDir WareDir(TempPackageDir.absoluteFilePath(WareFile));

    BOOST_CHECK(!WareDir.exists());
  }

  BOOST_CHECK_EQUAL(PkgImport.getPackagers().toStdString(), "the packager");
  BOOST_CHECK_EQUAL(PkgImport.getPackageDescription().toStdString(), "the description");

  QStringList WarePathsToDisplay = PkgImport.getWaresPaths();
  BOOST_CHECK_EQUAL(WarePathsToDisplay.size(), 3);
  for (const QString& WareFolder : f.ToExportWareFolders)
    BOOST_CHECK(WarePathsToDisplay.contains(TempPackageDir.absoluteFilePath(WareFolder)));

  // delete some wares to test copy

  for (const QString& ToImportFolder : f.ToImportWareFolders)
    openfluid::tools::Filesystem::removeDirectory(f.WaresDevDir.filePath(ToImportFolder).toStdString());

  QStringList SelectedWares;
  for (const QString& WareFile : f.ToExportWareFolders)
    SelectedWares << TempPackageDir.absoluteFilePath(WareFile);

  PkgImport.setSelectedWares(SelectedWares);
  PkgImport.copyWares();

  for (const QString& WareFile : f.ToImportWareFolders)
  {
    QDir WareDir(f.WaresDevDir.absoluteFilePath(WareFile));

    BOOST_CHECK(WareDir.exists());

    for (const QString& WareContent : (f.ToExportContentFolders + f.ToExportContentFiles))
      BOOST_CHECK(WareDir.exists(WareContent));

    for (const QString& WareContent : (f.NotToExportContentFolders + f.NotToExportContentFiles))
      BOOST_CHECK(!WareDir.exists(WareContent));
  }
}

