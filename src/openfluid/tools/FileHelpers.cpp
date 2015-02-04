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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <openfluid/tools/FileHelpers.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>


namespace openfluid { namespace tools {


bool removeDirectoryRecursively(const QString& DirPath)
{
  bool Res = true;
  QDir CurrentDir(DirPath);

  if (CurrentDir.exists(DirPath))
  {
    Q_FOREACH(QFileInfo Info,
              CurrentDir.entryInfoList(QDir::NoDotAndDotDot |
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
    Res = CurrentDir.rmdir(DirPath);
  }

  return Res;
}


// =====================================================================
// =====================================================================


std::vector<std::string> findFilesByExtension(const std::string DirToExplore, const std::string Ext,
                                              bool WithPath, bool ExtIncludeDot)
{

  std::vector<std::string> FileList;


  std::string FileExt = Ext;
  if (!ExtIncludeDot) FileExt = "."+Ext;

  boost::filesystem::path PathToExplore(DirToExplore);

  if (boost::filesystem::is_directory(PathToExplore))
  {

    boost::filesystem::directory_iterator it;

    std::string FoundFile;

    for (it = boost::filesystem::directory_iterator(PathToExplore);it != boost::filesystem::directory_iterator(); ++it)
    {

      // lists files with specified extension

      if (boost::filesystem::is_regular(it->status()) && boost::ends_with(it->path().string(),FileExt))
      {
        if (WithPath) FileList.push_back(it->path().string());
        else FileList.push_back(it->path().filename().string());
      }
    }
  }

  return FileList;

}


// =====================================================================
// =====================================================================


std::vector<std::string> findFilesBySuffixAndExtension(const std::string& DirToExplore,
                                                       const std::string& Suffix,
                                                       const std::string& Ext,
                                                       bool WithPath,
                                                       bool ExtIncludeDot)

{

  std::vector<std::string> FileList;


  std::string FileEnd = Suffix+Ext;
  if (!ExtIncludeDot) FileEnd = Suffix+"."+Ext;

  boost::filesystem::path PathToExplore(DirToExplore);

  if (boost::filesystem::is_directory(PathToExplore))
  {

    boost::filesystem::directory_iterator it;

    std::string FoundFile;

    for (it = boost::filesystem::directory_iterator(PathToExplore);it != boost::filesystem::directory_iterator(); ++it)
    {

      // lists files with specified extension

      if (boost::filesystem::is_regular(it->status()) && boost::ends_with(it->path().string(),FileEnd))
      {
        if (WithPath) FileList.push_back(it->path().string());
        else FileList.push_back(it->path().filename().string());
      }
    }
  }

  return FileList;
}


// =====================================================================
// =====================================================================


// TODO to be fixed: should not delete the given path, may not work on win32
bool emptyDirectoryRecursively(const std::string& DirPath)
{

  boost::filesystem::path PathToEmpty(DirPath);

  boost::filesystem::remove_all(PathToEmpty);

  return boost::filesystem::create_directory(PathToEmpty);

}


// =====================================================================
// =====================================================================


std::vector<std::string> findFilesRecursively(const std::string& DirPath)
{
  std::vector<std::string> FileNames;

  boost::filesystem::directory_iterator It;

  for (It = boost::filesystem::directory_iterator(DirPath); It != boost::filesystem::directory_iterator(); ++It)
  {
    // Regular file
    if (boost::filesystem::is_regular(It->status()))
      FileNames.push_back(DirPath + "/" + It->path().filename().string());

    // Directory
    if (boost::filesystem::is_directory(It->status()))
    {
      std::vector<std::string> SubDirFiles = findFilesRecursively(It->path().string());
      std::vector<std::string>::const_iterator It;

      for (It = SubDirFiles.begin(); It != SubDirFiles.end(); ++It)
        FileNames.push_back(*It);
    }
  }

  return FileNames;
}


// =====================================================================
// =====================================================================


void copyDirectoryRecursively(const std::string& SourceDir,
                          const std::string& IntoDir,
                          const bool DontCopyDotDirs)
{

  boost::filesystem::path SourceDirPath(SourceDir);
  boost::filesystem::path IntoDirPath(IntoDir);
  boost::filesystem::path DestDirPath(IntoDir+"/"+SourceDirPath.filename().string());


  if (boost::filesystem::is_directory(DestDirPath)) boost::filesystem::remove_all(DestDirPath);

  boost::filesystem::create_directory(DestDirPath);

  boost::filesystem::directory_iterator it;

  for (it = boost::filesystem::directory_iterator(SourceDirPath);it != boost::filesystem::directory_iterator(); ++it)
  {

    if (boost::filesystem::is_regular(it->status()))
    {
      boost::filesystem::copy_file(it->path(),
                                   boost::filesystem::path(DestDirPath.string()+"/"+it->path().filename().string()));
    }

    if (boost::filesystem::is_directory(it->status()))
    {
      if (!DontCopyDotDirs || (DontCopyDotDirs && !boost::starts_with(it->path().filename().string(),".")))
      {
        copyDirectoryRecursively(it->path().string(),DestDirPath.string(), DontCopyDotDirs);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void copyDirectoryContentsRecursively(const std::string& SourceDir,
                                      const std::string& IntoDir,
                                      const bool DontCopyDotDirs)
{

  boost::filesystem::path SourceDirPath(SourceDir);
  boost::filesystem::path IntoDirPath(IntoDir);

  if (boost::filesystem::is_directory(IntoDirPath)) boost::filesystem::remove_all(IntoDirPath);

  boost::filesystem::create_directory(IntoDirPath);

  boost::filesystem::directory_iterator it;

  for (it = boost::filesystem::directory_iterator(SourceDirPath);it != boost::filesystem::directory_iterator(); ++it)
  {

    if (boost::filesystem::is_regular(it->status()))
    {
      boost::filesystem::copy_file(it->path(),
                                   boost::filesystem::path(IntoDirPath.string()+"/"+it->path().filename().string()));
    }

    if (boost::filesystem::is_directory(it->status()))
    {
      if (!DontCopyDotDirs || (DontCopyDotDirs && !boost::starts_with(it->path().filename().string(),".")))
      {
        copyDirectoryRecursively(it->path().string(),IntoDirPath.string(), DontCopyDotDirs);
      }
    }
  }
}


} }  // namespaces
