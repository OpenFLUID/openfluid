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
 @file WaresDevPackage.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QSettings>
#include <QApplication>

#include <openfluid/waresdev/WaresDevPackage.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace waresdev {

QString WaresDevPackage::m_CMakeCmd = "";


// =====================================================================
// =====================================================================


WaresDevPackage::WaresDevPackage(const QString& PackageFilePath, const QString& TempSubFolderName) :
    m_PackageFilePath(PackageFilePath)
{
  checkCMakeProgram();

  m_PackageName = QFileInfo(m_PackageFilePath).completeBaseName();

  QString TempOfwdpSubDirPath = QDir(QString::fromStdString(openfluid::base::Environment::getTempDir()))
      .absoluteFilePath("ofwdp");

  m_PackageTempDir = QDir(QString("%1/%2/%3").arg(TempOfwdpSubDirPath).arg(TempSubFolderName).arg(m_PackageName));
  m_PackageTempPath = m_PackageTempDir.absolutePath();

  if (m_PackageTempDir.exists())
    openfluid::tools::emptyDirectoryRecursively(m_PackageTempPath.toStdString());
  else
    (m_PackageTempDir.mkpath(m_PackageTempPath));

  m_ConfFilePath = m_PackageTempDir.absoluteFilePath(
      QString::fromStdString(openfluid::config::WARESDEV_PACKAGE_CONFFILE));
}


// =====================================================================
// =====================================================================


WaresDevPackage::~WaresDevPackage()
{
  if (!mp_Process)
    return;

  if (mp_Process->state() != QProcess::NotRunning)
    mp_Process->close();

  delete mp_Process;
}


// =====================================================================
// =====================================================================


bool WaresDevPackage::checkCMakeProgram()
{
  if (m_CMakeCmd.isEmpty())
    m_CMakeCmd = openfluid::utils::ExternalProgram::getRegisteredProgram(
        openfluid::utils::ExternalProgram::CMakeProgram).getFullProgramPath();

  return (!m_CMakeCmd.isEmpty());
}


// =====================================================================
// =====================================================================


QStringList WaresDevPackage::getWaresPaths()
{
  return m_WaresPaths;
}


// =====================================================================
// =====================================================================


void WaresDevPackage::createAndLauchProcess(const QString& Command)
{
  if (mp_Process)
  {
    mp_Process->close();
    delete mp_Process;
  }

  mp_Process = new QProcess(this);

  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStandardOutput()));
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processErrorOutput()));

  mp_Process->start(Command);

  mp_Process->waitForFinished(-1);
  mp_Process->waitForReadyRead(-1);
}


// =====================================================================
// =====================================================================


void WaresDevPackage::processStandardOutput()
{
  mp_Process->setReadChannel(QProcess::StandardOutput);

  while (mp_Process->canReadLine())
    emit info(QString::fromUtf8(mp_Process->readLine()));
}


// =====================================================================
// =====================================================================


void WaresDevPackage::processErrorOutput()
{
  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
    emit error(QString::fromUtf8(mp_Process->readLine()));
}


// =====================================================================
// =====================================================================


QString WaresDevPackage::getPackagers()
{
  return m_Packagers;
}


// =====================================================================
// =====================================================================


QString WaresDevPackage::getPackageDescription()
{
  return m_PackageDescription;
}


// =====================================================================
// =====================================================================


QDir WaresDevPackage::getPackageTempDir()
{
  return m_PackageTempDir;
}


// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


WaresDevExportPackage::WaresDevExportPackage(const QString& PackageFilePath, const QStringList& WaresPaths,
  const QString& Packagers, const QString& PackageDescription) :
    WaresDevPackage(PackageFilePath, "export")
{
  m_WaresPaths = WaresPaths;
  m_Packagers = Packagers;
  m_PackageDescription = PackageDescription;
}


// =====================================================================
// =====================================================================


void WaresDevExportPackage::exportToPackage()
{
  writeConfFile();

  QFile PackageFile(m_PackageFilePath);
  if (PackageFile.exists())
    PackageFile.remove();

  compress();

  emit progressed(100);

  if (mp_Process->exitCode())
    emit finished(false, tr("Export failed"));
  else
    emit finished(true, tr("Export completed"));
}


// =====================================================================
// =====================================================================


void WaresDevExportPackage::writeConfFile()
{
  QSettings ConfFileSettings(m_ConfFilePath, QSettings::IniFormat);

  ConfFileSettings.beginGroup("OpenFLUID waresdev package");
  ConfFileSettings.setValue("packager", m_Packagers);
  ConfFileSettings.setValue("description", m_PackageDescription);
  ConfFileSettings.endGroup();

  ConfFileSettings.sync();
}


// =====================================================================
// =====================================================================


void WaresDevExportPackage::compress()
{
  QString WaresdevPath = openfluid::waresdev::WareSrcManager::instance()->getWaresdevPath();

  QDir WaresdevDir(WaresdevPath);

  QStringList RelativePathsToExport;

  for (const QString& WarePath : m_WaresPaths)
  {
    QDir WareDir(WarePath);

    for (const QFileInfo& ContentFileInfo : WareDir.entryInfoList(
        QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot))
    {
      QString Filename = ContentFileInfo.fileName();

      if (Filename.startsWith(".git") || !(Filename.startsWith(".") || Filename.startsWith("_build")))
        RelativePathsToExport << WaresdevDir.relativeFilePath(ContentFileInfo.absoluteFilePath());
    }
  }

  // With cmake tar, the paths are stored only relatively to the current dir: a file out of the current dir will
  // produce a path starting with "../", that cmake untar won't be able to uncompress.
  // So we copy the config file into the current dir before the compression and remove it after that.
  QString ConFileName = QString::fromStdString(openfluid::config::WARESDEV_PACKAGE_CONFFILE);
  QString ConfFileInWareDirAbsPath = WaresdevDir.filePath(ConFileName);
  if (QFile(m_ConfFilePath).copy(ConfFileInWareDirAbsPath))
    RelativePathsToExport << ConFileName;
  else
    emit error(tr("Unable to write configuration file in \"%1\".\nPackage may contain errors.").arg(WaresdevPath));

  QString Command = QString("\"%1\" -E chdir \"%2\" "
                            "\"%1\" -E tar cfzv \"%3\" \"%4\"").arg(m_CMakeCmd).arg(WaresdevPath).arg(
      m_PackageFilePath).arg(RelativePathsToExport.join("\" \""));

  QDir::setCurrent(WaresdevPath);

  createAndLauchProcess(Command);

  if (!QFile(ConfFileInWareDirAbsPath).remove())
    emit error(tr("Unable to delete configuration file \"%1\".").arg(ConfFileInWareDirAbsPath));
}


// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


WaresDevImportPackage::WaresDevImportPackage(const QString& PackageFilePath) :
    WaresDevPackage(PackageFilePath, "import_file")
{

}


// =====================================================================
// =====================================================================


void WaresDevImportPackage::fetchInformation()
{
  uncompress();

  readConfFile();

  for (const QString& WareTypeFolder : m_PackageTempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
  {
    for (const QString& WareFolder : QDir(m_PackageTempDir.absoluteFilePath(WareTypeFolder)).entryList(
        QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name))
      m_WaresPaths << m_PackageTempDir.absoluteFilePath("%1/%2").arg(WareTypeFolder).arg(WareFolder);
  }

  if (mp_Process->exitCode())
    emit finished(false, tr("Fetching information failed"));
  else
    emit finished(true, tr("Fetching information completed"));

  if (qApp && qApp->thread() != thread())
    moveToThread(qApp->thread());
}


// =====================================================================
// =====================================================================


void WaresDevImportPackage::uncompress()
{
  QString Command = QString("\"%1\" -E chdir \"%2\" \"%1\" -E tar xfzv \"%3\"").arg(m_CMakeCmd).arg(m_PackageTempPath)
      .arg(m_PackageFilePath);

  createAndLauchProcess(Command);
}


// =====================================================================
// =====================================================================


void WaresDevImportPackage::readConfFile()
{
  QSettings ConfFileSettings(m_ConfFilePath, QSettings::IniFormat);

  ConfFileSettings.beginGroup("OpenFLUID waresdev package");
  m_Packagers = ConfFileSettings.value("packager", m_Packagers).toString();
  m_PackageDescription = ConfFileSettings.value("description", m_PackageDescription).toString();
  ConfFileSettings.endGroup();
}

// =====================================================================
// =====================================================================


void WaresDevImportPackage::setSelectedWares(const QStringList& SelectedWarePaths)
{
  m_SelectedWarePaths = SelectedWarePaths;
}


// =====================================================================
// =====================================================================


void WaresDevImportPackage::copyWares()
{
  QDir WaresDevDir(openfluid::waresdev::WareSrcManager::instance()->getWaresdevPath());

  bool Ok = true;

  double ProgressRatio = 100;
  if (int SelectedWarePathsNb = m_SelectedWarePaths.size())
    ProgressRatio /= SelectedWarePathsNb;

  int Progress = 0;

  for (const QString& WarePath : m_SelectedWarePaths)
  {
    QString DestinationPath = WaresDevDir.filePath(m_PackageTempDir.relativeFilePath(WarePath));

    QString DestinationUpDir = QFileInfo(DestinationPath).dir().absolutePath();

    if (!QDir(DestinationPath).exists())
    {
      emit info(tr("Importing \"%1\"").arg(DestinationPath));

      if (!openfluid::tools::Filesystem::copyDirectory(WarePath.toStdString(), DestinationUpDir.toStdString()))
      {
        emit error(tr("Unable to copy \"%1\" to \"%2\"").arg(WarePath).arg(DestinationUpDir));
        Ok = false;
      }
    }
    else
    {
      emit info(tr("\"%1\" already exists (ignored)").arg(DestinationPath));
    }

    Progress += ProgressRatio;
    emit progressed(Progress);
  }

  emit progressed(100);

  if (!Ok)
    emit finished(false, "Import completed with errors");
  else
    emit finished(true, "Import completed");

  if (qApp && qApp->thread() != thread())
    moveToThread(qApp->thread());
}


} } // namespaces
