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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


// OpenFLUID:stylecheck:!incs
// OpenFLUID:stylecheck:!inco


#include <openfluid/global.hpp>

#include <filesystem>
#include <fstream>
#include <thread>
#include <regex>

#if defined(OPENFLUID_OS_WINDOWS)
#include <userenv.h>
#endif

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


namespace openfluid { namespace tools {


std::string Filesystem::joinPath(const std::vector<std::string>& PathParts)
{
  return boost::algorithm::join(PathParts,"/");
}


// =====================================================================
// =====================================================================


std::string Filesystem::homePath()
{
  std::string HomeDir;

#ifdef OPENFLUID_OS_UNIX
  HomeDir = std::string(std::getenv("HOME"));
#endif

#ifdef OPENFLUID_OS_WINDOWS
  HANDLE Token = 0;
  DWORD PathLen = MAX_PATH;
  char Buf[MAX_PATH] = {'\0'};
  if (OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&Token)) 
  {
    GetUserProfileDirectory(Token,Buf,&PathLen);
    CloseHandle(Token);
    HomeDir = std::string(Buf);
  }
#endif

  return std::filesystem::path(HomeDir).generic_string();
}


// =====================================================================
// =====================================================================


std::string Filesystem::tempPath()
{
  return std::filesystem::temp_directory_path().generic_string();
}


// =====================================================================
// =====================================================================


std::string Filesystem::filename(const std::string& Path)
{
  return FilesystemPath(Path).filename();
}


// =====================================================================
// =====================================================================


std::string Filesystem::dirname(const std::string& Path)
{
  return FilesystemPath(Path).dirname();
}


// =====================================================================
// =====================================================================


std::string Filesystem::basename(const std::string& Path)
{
  return FilesystemPath(Path).basename();
}


// =====================================================================
// =====================================================================


std::string Filesystem::minimalBasename(const std::string& Path)
{
  return FilesystemPath(Path).minimalBasename();
}


// =====================================================================
// =====================================================================


std::string Filesystem::extension(const std::string& Path)
{
  return FilesystemPath(Path).extension();
}


// =====================================================================
// =====================================================================


std::string Filesystem::completeExtension(const std::string& Path)
{
  return FilesystemPath(Path).completeExtension();
}


// =====================================================================
// =====================================================================


std::string Filesystem::currentPath()
{
  return std::filesystem::current_path().generic_string();
}


// =====================================================================
// =====================================================================


std::string Filesystem::absolutePath(const std::string& Path)
{
  std::string TmpStr = Path;
  
  if (TmpStr.empty())
  {
    return "";
  } 
  else if (TmpStr == "/")
  {
    return "/";
  }

  return FilesystemPath::removeTrailingSeparators(
           std::filesystem::absolute(FilesystemPath(TmpStr).stdPath()).lexically_normal().generic_string()
         );
}


// =====================================================================
// =====================================================================


bool Filesystem::copyFile(const std::string& SrcPath, const std::string& DestPath)
{
  std::error_code TmpErr;
  return std::filesystem::copy_file(FilesystemPath(SrcPath).stdPath(),FilesystemPath(DestPath).stdPath(),TmpErr);
}


// =====================================================================
// =====================================================================


bool Filesystem::renameFile(const std::string& OriginalPath, const std::string& NewPath)
{
  std::error_code TmpErr;
  const auto OrigStdPath = FilesystemPath(OriginalPath);
  const auto NewStdPath = FilesystemPath(NewPath);

  std::filesystem::rename(OrigStdPath.stdPath(),NewStdPath.stdPath(),TmpErr);
  return (NewStdPath.isFile() && !OrigStdPath.isFile());
}


// =====================================================================
// =====================================================================


bool Filesystem::copyDirectory(const std::string& SrcPath, const std::string& DestPath,
                               bool WithBaseDir, bool RemoveExisting)
{
  const auto SrcPathImpl = FilesystemPath(SrcPath);
  auto DestPathImpl = FilesystemPath(DestPath);


  if (WithBaseDir)
  {
    DestPathImpl = FilesystemPath(DestPath+"/"+FilesystemPath(SrcPath).stdPath().filename().generic_string());
  }

  if (RemoveExisting)
  {
    if (DestPathImpl.isFile())
    {
      DestPathImpl.removeFile();
    }
    else if (DestPathImpl.isDirectory())
    {
      DestPathImpl.removeDirectory();
    }
  }

  if (DestPathImpl.exists())
  {
    return false;
  }


  // Create parent path of destination directory before copy
  if (FilesystemPath::fromStdPath(DestPathImpl.stdPath().parent_path()).makeDirectory())
  {
    try 
    {
      std::filesystem::copy(SrcPathImpl.stdPath(),DestPathImpl.stdPath(),std::filesystem::copy_options::recursive);
      return true;
    }
    catch (const std::filesystem::filesystem_error&)
    {
      return false;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool Filesystem::copyDirectoryContent(const std::filesystem::path& Source, const std::filesystem::path& Destination)
{
  // was a filesystem::copy but not working when destination directory already exists under Windows, 
  // even with copy_options::update_existing
  const auto CopyOptions = std::filesystem::copy_options::update_existing | 
                           std::filesystem::copy_options::recursive;
  for (const auto& E : std::filesystem::recursive_directory_iterator(Source))
  {
    const auto SourceItem = E.path();
    const auto DestinationItem = Destination / std::filesystem::relative(SourceItem, Source);

    try
    {
      if (std::filesystem::is_directory(SourceItem))
      {
        std::filesystem::create_directories(DestinationItem);
      }
      else
      {
        std::filesystem::copy_file(SourceItem, DestinationItem, CopyOptions);
      }
    } 
    catch (const std::filesystem::filesystem_error& e)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                             std::string("Directory content copy failed: ")+e.what());
      //return false;  TOIMPL double check this part with windows

    }
  }
  return true;
}


// =====================================================================
// =====================================================================


bool Filesystem::emptyDirectory(const std::string& Path)
{
  auto PathFSP = FilesystemPath(Path);

  if (!PathFSP.isDirectory())
  {
    return false;
  }

  for (const auto & Entry : std::filesystem::directory_iterator(PathFSP.stdPath()))
  {
    auto EntryFSP = FilesystemPath(Entry.path().string());
    if (Entry.is_directory())
    {
      if (!EntryFSP.removeDirectory())
      {
        return false;
      }
    }
    else
    {
      if (!EntryFSP.removeFile())
      {
        return false;
      }
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


std::string Filesystem::makeUniqueSubdirectory(const std::string& Path, const std::string& SubdirName)
{
  if (SubdirName.empty())
  {
    return std::string();
  }

  std::ostringstream SS;
  SS << std::this_thread::get_id();
  static std::string PID = SS.str(); // use thread id to improve entropy

  auto PathFSP = FilesystemPath(Path);

  if (PathFSP.makeDirectory())
  {
    // Thread ID - Pseudo Unique Identifier of 16 chars length
    const std::string PIDPUI = PID + "-" + openfluid::tools::generatePseudoUniqueIdentifier(16);

    // Suffix to avoid for potential duplicates
    // (duplicates should never happen, so suffix should be 0)
    unsigned int IncSuffix = 0;

    // lambda expression to generate a formatted full path
    // pattern for subdir : {SubdirName}-{PID}-{PUI}-{Inc}
    auto generateFullPath = [&]()
    { 
      return FilesystemPath({Path,SubdirName+"-"+PIDPUI+"-"+std::to_string(IncSuffix)}); 
    };

    auto CandidateFSP =  generateFullPath();

    while (CandidateFSP.exists())
    {
      IncSuffix++;
      CandidateFSP = generateFullPath();
    }

    if (CandidateFSP.makeDirectory())
    {
      return CandidateFSP.toGenericClean();
    }
    else
    {
      return std::string();
    }
  }

  return std::string();
}


// =====================================================================
// =====================================================================


std::string Filesystem::makeUniqueFile(const std::string& Path, const std::string& FileName)
{
  auto PathFSP = FilesystemPath(Path);
  auto FileNameFSP = FilesystemPath(FileName);

  if (PathFSP.makeDirectory())
  {
    std::ostringstream SS;
    SS << std::this_thread::get_id();
    static std::string PID = SS.str(); // use thread id to improve entropy

    std::string FileRoot = FileNameFSP.minimalBasename();
    std::string FileExt = FileNameFSP.completeExtension();
    if (!FileExt.empty())
    {
      FileExt = "."+FileExt;
    }

    // Process ID - Pseudo Unique Identifier of 16 chars length
    std::string PIDPUI = PID + "-" + openfluid::tools::generatePseudoUniqueIdentifier(16);

    // Suffix to avoid for potential duplicates
    // (duplicates should never happen, so suffix should be 0)
    int IncSuffix = 0;

    // lambda expression to generate a formatted full path
    // pattern for file : {Path}/{Filename root}-{PID}-{PUI}-{Inc}.{Filename ext}
    auto generateFullPath = [&]()
    { 
      return FilesystemPath({Path,FileRoot+"-"+PIDPUI+std::to_string(IncSuffix)+FileExt}); 
    };

    auto CandidateFSP = generateFullPath();

    while (CandidateFSP.exists())
    {
      IncSuffix++;
      CandidateFSP = generateFullPath();
    }

    if (CandidateFSP.makeFile())
    {
      return CandidateFSP.toGenericClean();
    }
  }

  return std::string();
}


// =====================================================================
// =====================================================================


std::string Filesystem::readFile(const openfluid::tools::Path& FileObj)
{
  std::string Content;
  std::ifstream File(FileObj.toGeneric(),std::ios::in);

  if (File.is_open())
  {
    std::copy(std::istream_iterator<char>{File >> std::noskipws},{},std::back_inserter(Content));
    File.close();
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                             "File opening failed: "+FileObj.toGeneric());
  }
  return Content;
}


// =====================================================================
// =====================================================================


void Filesystem::writeFile(const std::string& Content, const openfluid::tools::Path& FileObj)
{
  std::ofstream File(FileObj.toGeneric(),std::ios::out);

  if (File.is_open())
  {
    File << Content;
    File.close();
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                             "File opening failed: "+FileObj.toGeneric());
  }
}


// =====================================================================
// =====================================================================


std::vector<std::string> Filesystem::findFiles(const std::string& Path, bool WithPath, const std::string& Pattern)
{
  std::vector<std::string> FilesList;
  const auto SearchPathFSP = FilesystemPath(Path);

  if (SearchPathFSP.isDirectory())
  {
    std::regex RegExp(Pattern);

    for (const auto & Entry : std::filesystem::directory_iterator(SearchPathFSP.stdPath()))
    {
      if ((Entry.is_regular_file() || Entry.is_symlink()) && 
          (Pattern.empty() || std::regex_match(Entry.path().filename().generic_string(),RegExp)))
      {
        if (WithPath)
        {
          FilesList.push_back(Entry.path().generic_string());
        }
        else
        {
          FilesList.push_back(Entry.path().filename().generic_string());
        }
      }
    }
  }
  return FilesList;
}


// =====================================================================
// =====================================================================


std::vector<std::string> Filesystem::findDirectories(const std::string& Path, bool WithPath, const std::string& Pattern)
{
  std::vector<std::string> DirsList;
  const auto SearchPathFSP = FilesystemPath(Path);

  if (SearchPathFSP.isDirectory())
  {
    std::regex RegExp(Pattern);

    for (const auto & Entry : std::filesystem::directory_iterator(SearchPathFSP.stdPath()))
    {
      if (Entry.is_directory() && 
          (Pattern.empty() || std::regex_match(Entry.path().filename().generic_string(),RegExp)))
      {
        if (WithPath)
        {
          DirsList.push_back(Entry.path().generic_string());
        }
        else
        {
          DirsList.push_back(Entry.path().filename().generic_string());
        }
      }
    }
  }
  return DirsList;
}


// =====================================================================
// =====================================================================


std::vector<std::string> Filesystem::findFilesByExtension(const std::string& Path,
                                                          const std::string& Ext,
                                                          bool WithPath, bool ExtIncludeDot)
{
  return findFilesBySuffixAndExtension(Path,"",Ext,WithPath,ExtIncludeDot);
}


// =====================================================================
// =====================================================================


std::vector<std::string> Filesystem::findFilesBySuffixAndExtension(const std::string& Path,
                                                                   const std::string& Suffix,
                                                                   const std::string& Ext,
                                                                   bool WithPath,
                                                                   bool ExtIncludeDot)

{
  std::string SuffixExt = Suffix+"."+Ext;

  if (ExtIncludeDot)
  {
    SuffixExt = Suffix+Ext;
  }

  std::string Pattern = ".*"+openfluid::tools::escapePattern(SuffixExt);

  return findFiles(Path,WithPath,Pattern);
}


} }  // namespaces
