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
  @file FileHelpers.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QDir>
#include <QFileInfo>

#include <iostream>

#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/QtHelpers.hpp>


namespace openfluid { namespace tools {


std::vector<std::string> findFilesByExtension(const std::string& Path,
                                              const std::string& Ext,
                                              bool WithPath, bool ExtIncludeDot)
{
  QStringList NameFilters;

  if (ExtIncludeDot)
  {
    NameFilters << QString::fromStdString('*'+Ext);
  }
  else
  {
    NameFilters << QString::fromStdString("*."+Ext);
  }


  QFileInfoList FoundFiles =
      QDir(QString::fromStdString(Path)).entryInfoList(NameFilters, QDir::NoDotAndDotDot | QDir::Hidden | QDir::Files);

  std::vector<std::string> FileList;

  for (int i=0;i<FoundFiles.size();i++)
  {
    if (WithPath)
    {
      FileList.push_back(FoundFiles[i].absoluteFilePath().toStdString());
    }
    else
    {
      FileList.push_back(FoundFiles[i].fileName().toStdString());
    }
  }

  return FileList;

}


// =====================================================================
// =====================================================================


std::vector<std::string> findFilesBySuffixAndExtension(const std::string& Path,
                                                       const std::string& Suffix,
                                                       const std::string& Ext,
                                                       bool WithPath,
                                                       bool ExtIncludeDot)

{
  QStringList NameFilters;

  if (ExtIncludeDot)
  {
    NameFilters << QString::fromStdString('*'+Suffix+Ext);
  }
  else
  {
    NameFilters << QString::fromStdString('*'+Suffix+'.'+Ext);
  }


  QFileInfoList FoundFiles =
      QDir(QString::fromStdString(Path)).entryInfoList(NameFilters, QDir::NoDotAndDotDot | QDir::Hidden | QDir::Files);

  std::vector<std::string> FileList;

  for (int i=0;i<FoundFiles.size();i++)
  {
    if (WithPath)
    {
      FileList.push_back(FoundFiles[i].absoluteFilePath().toStdString());
    }
    else
    {
      FileList.push_back(FoundFiles[i].fileName().toStdString());
    }
  }

  return FileList;
}


// =====================================================================
// =====================================================================


std::vector<std::string> findDirectories(const std::string& Path,
                                         bool WithPath)
{

  QFileInfoList FoundDirs =
      QDir(QString::fromStdString(Path)).entryInfoList(QDir::NoDotAndDotDot | QDir::Hidden | QDir::Dirs);

  std::vector<std::string> DirList;

  for (int i=0;i<FoundDirs.size();i++)
  {
    if (WithPath)
    {
      DirList.push_back(FoundDirs[i].absoluteFilePath().toStdString());
    }
    else
    {
      DirList.push_back(FoundDirs[i].fileName().toStdString());
    }
  }

  return DirList;
}


// =====================================================================
// =====================================================================


bool emptyDirectoryRecursively(const std::string& Path)
{
  QDir CurrentDir(QString::fromStdString(Path));

  for (QFileInfo Info : CurrentDir.entryInfoList(QDir::NoDotAndDotDot |
                                                 QDir::System |
                                                 QDir::Hidden  |
                                                 QDir::AllDirs |
                                                 QDir::Files,
                                                 QDir::DirsFirst))
  {
    if (Info.isDir())
    {
      if (!Filesystem::removeDirectory(Info.absoluteFilePath().toStdString()))
      {
        return false;
      }
    }
    else
    {
      if (!Filesystem::removeFile(Info.absoluteFilePath().toStdString()))
      {
        return false;
      }
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


void copyDirectoryContentsRecursively(const std::string& SrcPath,
                                      const std::string& DestPath,
                                      const bool DontCopyDotDirs)
{

  if (QFileInfo(QString::fromStdString(DestPath)).isDir())
  {
    Filesystem::removeDirectory(DestPath);
  }

  Filesystem::makeDirectory(DestPath);


  QFileInfoList FoundFiles =
      QDir(QString::fromStdString(SrcPath)).entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  |
                                                          QDir::AllDirs | QDir::Files, QDir::DirsFirst);

  for (int i=0;i<FoundFiles.size();i++)
  {
    if (FoundFiles[i].isDir() && !(DontCopyDotDirs && FoundFiles[i].fileName().startsWith(".")))
    {
      Filesystem::copyDirectory(FoundFiles[i].absoluteFilePath().toStdString(),
                                DestPath,
                                DontCopyDotDirs);
    }
    else
    {
      Filesystem::copyFile(FoundFiles[i].absoluteFilePath().toStdString(),
                           DestPath+"/"+FoundFiles[i].fileName().toStdString());
    }
  }
}


} }  // namespaces
