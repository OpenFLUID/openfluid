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

#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_WaresDevPackage
#include <boost/test/unit_test.hpp>

#include <openfluid/waresdev/WaresDevPackage.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/config.hpp>
#include "tests-config.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QSettings>


// =====================================================================
// =====================================================================


class F
{
  private:

    openfluid::waresdev::WareSrcManager* Mgr;

    QString WorkspaceForExportPath;
    QString WorkspaceForImportPath;

    QStringList DirPathsCreated;

    QString getCleanedPath(const QString& MainDirPath, const QString& RelativeSubPath)
    {
      QDir MainDir(MainDirPath);
      QString AbsoluteSubPath = MainDir.absoluteFilePath(RelativeSubPath);

      openfluid::tools::emptyDirectoryRecursively(AbsoluteSubPath.toStdString());
      MainDir.rmdir(AbsoluteSubPath);

      return AbsoluteSubPath;
    }

  public:

    QString TestPackagePath = QDir(QString::fromStdString(CONFIGTESTS_INPUT_MISCDATA_DIR)).absoluteFilePath(
        "ofwdp/my_package.ofwdp");

    QString WaresdevForExportPath;
    QString WaresdevForImportPath;

    QString OutOfwdpPath;
    QDir OutOfwdpDir;

    QDir TempPackageExportDir;
    QDir TempPackageImportFileDir;

    QStringList ToExportWareFolders = {
                                        "simulators/sim1",
                                        "simulators/sim3",
                                        "builderexts/bext2",
    };
    QStringList ToExportWareFoldersPaths;
    QStringList NotToExportWareFolders = {
                                           "simulators/sim2",
                                           "builderexts/bext1",
                                           "builderexts/bext3",
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

    F()
    {
      Mgr = openfluid::waresdev::WareSrcManager::instance();

      QString TempOFPath = QString::fromStdString(openfluid::base::RuntimeEnvironment::instance()->getTempDir());
      QDir TempOFDir(TempOFPath);

      WorkspaceForExportPath = TempOFDir.absoluteFilePath("ofwdp_export_workspace");
      WorkspaceForImportPath = TempOFDir.absoluteFilePath("ofwdp_import_workspace");

      WaresdevForExportPath = getCleanedPath(WorkspaceForExportPath, "wares-dev");
      WaresdevForImportPath = getCleanedPath(WorkspaceForImportPath, "wares-dev");

      OutOfwdpPath = getCleanedPath(QString::fromStdString(CONFIGTESTS_OUTPUT_DATA_DIR), "ofwdp");
      OutOfwdpDir = QDir(OutOfwdpPath);

      QString TempOfwdpPath = getCleanedPath(TempOFPath, "ofwdp");
      QDir TempOfwdpDir(TempOfwdpPath);
      TempPackageExportDir = QDir(TempOfwdpDir.filePath("export/my_package"));
      TempPackageImportFileDir = QDir(TempOfwdpDir.filePath("import_file/my_package"));

      DirPathsCreated << WorkspaceForExportPath << WorkspaceForImportPath << OutOfwdpPath << TempOfwdpPath;
    }

    ~F()
    {
      QDir Temp(QDir::temp());

      for (const QString& DirPath : DirPathsCreated)
      {
        QDir Dir(DirPath);

        if (Dir.exists())
        {
          openfluid::tools::emptyDirectoryRecursively(DirPath.toStdString());
          Dir.rmdir(DirPath);
        }
      }

    }

    void createExportConfiguration()
    {
      QStringList WareFolders = ToExportWareFolders + NotToExportWareFolders;
      QStringList ContentFolders = ToExportContentFolders + NotToExportContentFolders;
      QStringList ContentFiles = ToExportContentFiles + NotToExportContentFiles;

      writeWaresdevContent(WaresdevForExportPath, WareFolders, ContentFolders, ContentFiles);

      Mgr->switchWorkspace(WorkspaceForExportPath);
    }

    void createImportConfiguration()
    {
      QStringList WareFolders = NotToExportWareFolders;
      QStringList ContentFolders = ToExportContentFolders + NotToExportContentFolders;
      QStringList ContentFiles = ToExportContentFiles + NotToExportContentFiles;

      writeWaresdevContent(WaresdevForImportPath, WareFolders, ContentFolders, ContentFiles);

      Mgr->switchWorkspace(WorkspaceForImportPath);
    }

    void writeWaresdevContent(QDir WaresDevDir, const QStringList& WareFolders, const QStringList& ContentFolders,
      const QStringList& ContentFiles)
    {
      for (const QString& WareFolder : WareFolders)
      {
        WaresDevDir.mkpath(WareFolder);

        QString WarePath = WaresDevDir.absoluteFilePath(WareFolder);

        if (ToExportWareFolders.contains(WareFolder))
          ToExportWareFoldersPaths << WarePath;

        QDir WareDir(WarePath);

        for (const QString& ContentFolder : ContentFolders)
          WareDir.mkpath(WareDir.absoluteFilePath(ContentFolder));

        for (const QString& ContentFile : ContentFiles)
          QFile(WareDir.absoluteFilePath(ContentFile)).open(QIODevice::WriteOnly);
      }
    }
};

// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(PkgExport,F)
{
  createExportConfiguration();

  openfluid::waresdev::WaresDevExportPackage PkgExport(OutOfwdpDir.absoluteFilePath("my_package.ofwdp"),
                                                       ToExportWareFoldersPaths,
                                                       "the packager",
                                                       "the description");

  BOOST_CHECK_EQUAL(PkgExport.getWaresPaths().size(), 3);

  PkgExport.exportToPackage();

  BOOST_CHECK(TempPackageExportDir.exists());
  BOOST_CHECK(TempPackageExportDir.exists("ofwdp.conf"));

  QSettings FileConfSettings(TempPackageExportDir.filePath("ofwdp.conf"), QSettings::IniFormat);
  FileConfSettings.beginGroup("OpenFLUID waresdev package");
  BOOST_CHECK_EQUAL(FileConfSettings.value("packager").toString().toStdString(), "the packager");
  BOOST_CHECK_EQUAL(FileConfSettings.value("description").toString().toStdString(), "the description");
  FileConfSettings.endGroup();

  BOOST_CHECK(OutOfwdpDir.exists("my_package.ofwdp"));

  QString Cmd = openfluid::utils::ExternalProgram::getRegisteredProgram(
      openfluid::utils::ExternalProgram::SevenZipProgram).getFullProgramPath();
  Cmd.append(
      QString(" x \"%1\" \"-o%2\" -aoa").arg(OutOfwdpDir.absoluteFilePath("my_package.ofwdp")).arg(OutOfwdpPath));
  BOOST_CHECK(!QProcess::execute(Cmd));

  QStringList FilesInPackage = OutOfwdpDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
  BOOST_CHECK(FilesInPackage.contains("ofwdp.conf"));

  for (const QString& WareFile : ToExportWareFolders)
  {
    QDir WareDir(OutOfwdpDir.absoluteFilePath(WareFile));

    BOOST_CHECK(WareDir.exists());

    for (const QString& WareContent : (ToExportContentFolders + ToExportContentFiles))
      BOOST_CHECK(WareDir.exists(WareContent));

    for (const QString& WareContent : (NotToExportContentFolders + NotToExportContentFiles))
      BOOST_CHECK(!WareDir.exists(WareContent));
  }

  for (const QString& WareFile : NotToExportWareFolders)
  {
    QDir WareDir(OutOfwdpDir.absoluteFilePath(WareFile));

    BOOST_CHECK(!WareDir.exists());
  }

}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(PkgImport_fetchInformation,F)
{
  createImportConfiguration();

  openfluid::waresdev::WaresDevImportPackage PkgImport(TestPackagePath);

  PkgImport.fetchInformation();

  BOOST_CHECK(TempPackageImportFileDir.exists());

  QStringList FilesInPackage = TempPackageImportFileDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
  BOOST_CHECK_EQUAL(FilesInPackage.size(), 1);
  BOOST_CHECK_EQUAL(FilesInPackage[0].toStdString(), "ofwdp.conf");

  for (const QString& WareFile : ToExportWareFolders)
  {
    QDir WareDir(TempPackageImportFileDir.absoluteFilePath(WareFile));

    BOOST_CHECK(WareDir.exists());

    for (const QString& WareContent : (ToExportContentFolders + ToExportContentFiles))
      BOOST_CHECK(WareDir.exists(WareContent));

    for (const QString& WareContent : (NotToExportContentFolders + NotToExportContentFiles))
      BOOST_CHECK(!WareDir.exists(WareContent));
  }

  for (const QString& WareFile : NotToExportWareFolders)
  {
    QDir WareDir(TempPackageImportFileDir.absoluteFilePath(WareFile));

    BOOST_CHECK(!WareDir.exists());
  }

  BOOST_CHECK_EQUAL(PkgImport.getPackagers().toStdString(), "the packager");
  BOOST_CHECK_EQUAL(PkgImport.getPackageDescription().toStdString(), "the description");

  QStringList WarePathsToDisplay = PkgImport.getWaresPaths();
  BOOST_CHECK_EQUAL(WarePathsToDisplay.size(), 3);
  for (const QString& WareFolder : ToExportWareFolders)
    BOOST_CHECK(WarePathsToDisplay.contains(TempPackageImportFileDir.absoluteFilePath(WareFolder)));
}


// =====================================================================
// =====================================================================


BOOST_FIXTURE_TEST_CASE(PkgImport_copyWares,F)
{
  createImportConfiguration();

  openfluid::waresdev::WaresDevImportPackage PkgImport(TestPackagePath);

  PkgImport.fetchInformation();

  QStringList SelectedWares;
  for (const QString& WareFile : ToExportWareFolders)
    SelectedWares << TempPackageImportFileDir.absoluteFilePath(WareFile);

  PkgImport.setSelectedWares(SelectedWares);

  PkgImport.copyWares();

  QDir WaresdevForImportDir(WaresdevForImportPath);

  for (const QString& WareFile : ToExportWareFolders)
  {
    QDir WareDir(WaresdevForImportDir.absoluteFilePath(WareFile));

    BOOST_CHECK(WareDir.exists());

    for (const QString& WareContent : (ToExportContentFolders + ToExportContentFiles))
      BOOST_CHECK(WareDir.exists(WareContent));

    for (const QString& WareContent : (NotToExportContentFolders + NotToExportContentFiles))
      BOOST_CHECK(!WareDir.exists(WareContent));
  }

}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
