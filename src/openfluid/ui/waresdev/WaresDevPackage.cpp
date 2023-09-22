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

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/

#ifndef QT_VERSION_MAJOR
#pragma message "Qt version not found in source"
#else
#pragma message "Qt version found in source"
#endif


#include <QSettings>
#include <QCoreApplication>

#include <openfluid/ui/waresdev/WaresDevPackage.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ui/QtHelpers.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WaresDevPackage::WaresDevPackage(const QString& PackageFilePath, const QString& TempSubFolderName) :
    m_PackageFilePath(PackageFilePath)
{
  m_PackageName = QFileInfo(m_PackageFilePath).completeBaseName();

  QString TempOfwdpSubDirPath = QDir(QString::fromStdString(openfluid::base::Environment::getTempDir()))
      .absoluteFilePath("ofwdp");

#if (QT_VERSION_MAJOR < 6)
  m_PackageTempDir = QDir(QString("%1/%2/%3").arg(TempOfwdpSubDirPath).arg(TempSubFolderName).arg(m_PackageName));
#else
  m_PackageTempDir.setPath(QString("%1/%2/%3").arg(TempOfwdpSubDirPath).arg(TempSubFolderName).arg(m_PackageName));
#endif
  
  m_PackageTempPath = m_PackageTempDir.absolutePath();

  if (m_PackageTempDir.exists())
  {
    openfluid::tools::Filesystem::emptyDirectory(m_PackageTempPath.toStdString());
  }
  else
  {
    (m_PackageTempDir.mkpath(m_PackageTempPath));
  }

  m_ConfFilePath = m_PackageTempDir.absoluteFilePath(
      QString::fromStdString(openfluid::config::WARESDEV_PACKAGE_CONFFILE));
}


// =====================================================================
// =====================================================================


WaresDevPackage::~WaresDevPackage()
{
  if (!mp_Process)
  {
    return;
  }

  if (mp_Process->state() != QProcess::NotRunning)
  {
    mp_Process->close();
  }

  delete mp_Process;
}


// =====================================================================
// =====================================================================


QStringList WaresDevPackage::getWaresPaths()
{
  return m_WaresPaths;
}


// =====================================================================
// =====================================================================


void WaresDevPackage::createAndLauchProcess(const openfluid::utils::Process::Command& Cmd)
{
  if (mp_Process)
  {
    mp_Process->close();
    delete mp_Process;
  }

  mp_Process = new QProcess(this);

  connect(mp_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStandardOutput()));
  connect(mp_Process, SIGNAL(readyReadStandardError()), this, SLOT(processErrorOutput()));

  mp_Process->start(QString::fromStdString(Cmd.Program),openfluid::ui::toQStringList(Cmd.Args));

  mp_Process->waitForFinished(-1);
  mp_Process->waitForReadyRead(-1);
}


// =====================================================================
// =====================================================================


void WaresDevPackage::processStandardOutput()
{
  mp_Process->setReadChannel(QProcess::StandardOutput);

  while (mp_Process->canReadLine())
  {
    emit info(QString::fromUtf8(mp_Process->readLine()));
  }
}


// =====================================================================
// =====================================================================


void WaresDevPackage::processErrorOutput()
{
  mp_Process->setReadChannel(QProcess::StandardError);

  while (mp_Process->canReadLine())
  {
    emit error(QString::fromUtf8(mp_Process->readLine()));
  }
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
  {
    PackageFile.remove();
  }

  compress();

  emit progressed(100);

  if (mp_Process->exitCode())
  {
    emit finished(false, tr("Export failed"));
  }
  else
  {
    emit finished(true, tr("Export completed"));
  }
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
  QString WaresdevPath = QString::fromStdString(openfluid::base::WorkspaceManager::instance()->getWaresPath());

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
      {
        RelativePathsToExport << WaresdevDir.relativeFilePath(ContentFileInfo.absoluteFilePath());
      }
    }
  }

  // With cmake tar, the paths are stored only relatively to the current dir: a file out of the current dir will
  // produce a path starting with "../", that cmake untar won't be able to uncompress.
  // So we copy the config file into the current dir before the compression and remove it after that.
  QString ConFileName = QString::fromStdString(openfluid::config::WARESDEV_PACKAGE_CONFFILE);
  QString ConfFileInWareDirAbsPath = WaresdevDir.filePath(ConFileName);
  if (QFile(m_ConfFilePath).copy(ConfFileInWareDirAbsPath))
  {
    RelativePathsToExport << ConFileName;
  }
  else
  {
    emit error(tr("Unable to write configuration file in \"%1\".\nPackage may contain errors.").arg(WaresdevPath));
  }


  openfluid::utils::Process::Command Cmd = 
    openfluid::utils::CMakeProxy::getTarCompressCommand(WaresdevPath.toStdString(),
                                                        m_PackageFilePath.toStdString(),
                                                        openfluid::ui::toStdStringVector(RelativePathsToExport),
                                                        "vz");

  QDir::setCurrent(WaresdevPath);

  createAndLauchProcess(Cmd);

  if (!QFile(ConfFileInWareDirAbsPath).remove())
  {
    emit error(tr("Unable to delete configuration file \"%1\".").arg(ConfFileInWareDirAbsPath));
  }
}


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
    {
      m_WaresPaths << m_PackageTempDir.absoluteFilePath("%1/%2").arg(WareTypeFolder).arg(WareFolder);
    }
  }

  if (mp_Process->exitCode())
  {
    emit finished(false, tr("Fetching information failed"));
  }
  else
  {
    emit finished(true, tr("Fetching information completed"));
  }

  if (qApp && qApp->thread() != thread())
  {
    moveToThread(qApp->thread());
  }
}


// =====================================================================
// =====================================================================


void WaresDevImportPackage::uncompress()
{
  openfluid::utils::Process::Command Cmd = 
    openfluid::utils::CMakeProxy::getTarUncompressCommand(m_PackageTempPath.toStdString(),
                                                          m_PackageFilePath.toStdString(),"vz");

  createAndLauchProcess(Cmd);
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
  QDir WaresDevDir(QString::fromStdString(openfluid::base::WorkspaceManager::instance()->getWaresPath()));

  bool Ok = true;

  double ProgressRatio = 100;
  if (int SelectedWarePathsNb = m_SelectedWarePaths.size())
  {
    ProgressRatio /= SelectedWarePathsNb;
  }

  int Progress = 0;

  for (const QString& WarePath : m_SelectedWarePaths)
  {
    QString DestinationPath = WaresDevDir.filePath(m_PackageTempDir.relativeFilePath(WarePath));

    QString DestinationUpDir = QFileInfo(DestinationPath).dir().absolutePath();

    if (!QDir(DestinationPath).exists())
    {
      emit info(tr("Importing \"%1\"").arg(DestinationPath));

      if (!openfluid::tools::Filesystem::copyDirectory(WarePath.toStdString(),DestinationUpDir.toStdString(),true,true))
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
  {
    emit finished(false, "Import completed with errors");
  }
  else
  {
    emit finished(true, "Import completed");
  }

  if (qApp && qApp->thread() != thread())
  {
    moveToThread(qApp->thread());
  }
}


} } } // namespaces
