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
  @file Filesystem.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QString>
#include <QCoreApplication>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


namespace openfluid { namespace tools {


bool removeDirectoryRecursively(const QString& Path)
{
  bool Res = true;
  QDir CurrentDir(Path);

  if (CurrentDir.exists(Path))
  {
    for (QFileInfo Info : CurrentDir.entryInfoList(QDir::NoDotAndDotDot |
                                                   QDir::System |
                                                   QDir::Hidden  |
                                                   QDir::AllDirs |
                                                   QDir::Files,
                                                   QDir::DirsFirst))
    {
      if (Info.isDir())
      {
        Res = removeDirectoryRecursively(Info.absoluteFilePath());
      }
      else
      {
        Res = QFile::remove(Info.absoluteFilePath());
      }

      if (!Res)
      {
        return Res;
      }
    }
    Res = CurrentDir.rmdir(Path);
  }

  return Res;
}


// =====================================================================
// =====================================================================


bool copyDirectoryRecursively(const QString& SrcPath,
                              const QString& DestPath,
                              const bool DontCopyDotDirs)
{
  bool Res = true;

  QString DestDirPath = DestPath+"/"+QFileInfo(SrcPath).fileName();

  if (QFileInfo(DestDirPath).isDir()) removeDirectoryRecursively(DestDirPath);

  QDir().mkpath(DestDirPath);

  QDir CurrentDir(SrcPath);

  for (QFileInfo Info : CurrentDir.entryInfoList(QDir::NoDotAndDotDot |
                                                 QDir::System |
                                                 QDir::Hidden  |
                                                 QDir::AllDirs |
                                                 QDir::Files,
                                                 QDir::DirsFirst))
  {
    if (Info.isDir())
    {

      if (!DontCopyDotDirs || (DontCopyDotDirs && !Info.fileName().startsWith(".")))
        Res = copyDirectoryRecursively(Info.absoluteFilePath(), DestDirPath, DontCopyDotDirs);
    }
    else
    {
      Res = QFile::copy(Info.absoluteFilePath(),DestDirPath+"/"+Info.fileName());
    }
  }


  return Res;
}


// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================


std::string Filesystem::filename(const std::string& Path)
{
  return QFileInfo(QString::fromStdString(Path)).fileName().toStdString();
}


// =====================================================================
// =====================================================================


std::string Filesystem::basename(const std::string& Path)
{
  return QFileInfo(QString::fromStdString(Path)).completeBaseName().toStdString();
}


// =====================================================================
// =====================================================================


std::string Filesystem::dirname(const std::string& Path)
{
  return QFileInfo(QString::fromStdString(Path)).dir().path().toStdString();
}


// =====================================================================
// =====================================================================


std::string Filesystem::extension(const std::string& Path)
{
  return QFileInfo(QString::fromStdString(Path)).suffix().toStdString();
}


// =====================================================================
// =====================================================================


std::string Filesystem::currentPath()
{
  return QDir::currentPath().toStdString();
}


// =====================================================================
// =====================================================================


bool Filesystem::isDirectory(const std::string& Path)
{
  return QFileInfo(QString::fromStdString(Path)).isDir();
}


// =====================================================================
// =====================================================================


bool Filesystem::isFile(const std::string& Path)
{
  return QFileInfo(QString::fromStdString(Path)).isFile();
}


// =====================================================================
// =====================================================================


bool Filesystem::makeDirectory(const std::string& Path)
{
  return QDir().mkpath(QString::fromStdString(Path));
}


// =====================================================================
// =====================================================================


bool Filesystem::removeDirectory(const std::string& Path)
{
  return removeDirectoryRecursively(QString::fromStdString(Path));
}


// =====================================================================
// =====================================================================


std::string Filesystem::makeUniqueSubdirectory(const std::string& Path, const std::string& SubdirName)
{
  if (SubdirName.empty())
    return std::string();

  static QString PID = QString("%1").arg(QCoreApplication::applicationPid());

  if (QDir().mkpath(QString::fromStdString(Path)))
  {

    // Process ID - Pseudo Unique Identifier of 16 chars length
    QString PIDPUI = PID + "-" +QString::fromStdString(openfluid::tools::generatePseudoUniqueIdentifier(16));

    // pattern for subdir : {SubdirName}-{PID}-{PUI}-%1
    QString FullSubdirPath = QString::fromStdString(Path)+"/"+QString::fromStdString(SubdirName)+"-"+PIDPUI+"-%1";

    // Suffix replacing the "%1" string for potential duplicates
    // (duplicates should never happen, so suffix should be 0)
    int IncSuffix = 0;

    QString CandidateFullPath = FullSubdirPath.arg(IncSuffix);

    while (QFileInfo(CandidateFullPath).isDir())
    {
      IncSuffix++;
      CandidateFullPath = FullSubdirPath.arg(IncSuffix);
    }

    if (QDir().mkpath(CandidateFullPath))
      return CandidateFullPath.toStdString();
    else
      return std::string();
  }

  return std::string();
}


// =====================================================================
// =====================================================================


bool Filesystem::removeFile(const std::string& Path)
{
  return QFile(QString::fromStdString(Path)).remove();
}


// =====================================================================
// =====================================================================


bool Filesystem::copyFile(const std::string& SrcPath, const std::string& DestPath)
{
  return QFile::copy(QString::fromStdString(SrcPath),QString::fromStdString(DestPath));
}


// =====================================================================
// =====================================================================


bool Filesystem::copyDirectory(const std::string& SrcPath, const std::string& DestPath,
                               const bool DontCopyDotDirs)
{
  return copyDirectoryRecursively(QString::fromStdString(SrcPath),QString::fromStdString(DestPath),
                                  DontCopyDotDirs);
}


} }  // namespaces
