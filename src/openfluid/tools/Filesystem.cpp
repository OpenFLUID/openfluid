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
*/


// OpenFLUID:stylecheck:!incs
// OpenFLUID:stylecheck:!inco


#include <openfluid/global.hpp>

#include <filesystem>
#include <thread>
#include <ostream>
#include <fstream>
#include <regex>

#if defined(OPENFLUID_OS_WINDOWS)
#include <userenv.h>
#endif

#include <boost/algorithm/string/join.hpp>

#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/MiscHelpers.hpp>


namespace openfluid { namespace tools {


class PathImpl
{
  private:
    
    std::filesystem::path m_Path;


  public:

    PathImpl(const std::string& PathStr)
    {
      m_Path = std::filesystem::path(PathStr,std::filesystem::path::format::generic_format);
    }

    PathImpl(const std::filesystem::path& StdPath) :
      m_Path(StdPath)
    { }

    const std::filesystem::path& stdPath() const
    {
      return m_Path;
    }

    bool isDirectory() const
    {
      return std::filesystem::is_directory(m_Path);
    }

    bool isFile() const
    {
      return (std::filesystem::is_regular_file(m_Path) || std::filesystem::is_symlink(m_Path));
    }

    bool exists() const
    {
      return std::filesystem::exists(m_Path);
    }

    bool makeDirectory() const
    {
      std::error_code TmpErr;
      return (std::filesystem::is_directory(m_Path) || std::filesystem::create_directories(m_Path,TmpErr));
    }

    bool removeDirectory() const
    {
      if (isDirectory())
      {
        std::error_code TmpErr;
        std::filesystem::remove_all(m_Path,TmpErr);  
      }      
      return true;
    }

    bool removeFile() const
    {
      if (isFile())
      {
        std::error_code TmpErr;
        return std::filesystem::remove(m_Path,TmpErr);
      }
      return true;
    }
};


// =====================================================================
// =====================================================================


std::string Filesystem::removeTrailingSeparators(const std::string& Path, char Sep) noexcept
{
  std::string TmpStr = Path;
  unsigned int MinSize = 1; // minimal size to avoid removing the unique separator if any (e.g. "/")

  if (std::regex_match(Path,std::regex("[a-zA-Z]:.*")))
  {
    MinSize = 3; // minimal size to avoid removing the windows drive if any (e.g. "c:/")
  }

  while (TmpStr.size() > MinSize  
         && TmpStr.back() == Sep) 
  {
    TmpStr.pop_back();
  }

  return TmpStr;
}


// =====================================================================
// =====================================================================


std::string Filesystem::joinPath(const std::vector<std::string>& PathParts)
{
  return boost::algorithm::join(PathParts,"/");
}


// =====================================================================
// =====================================================================


std::string Filesystem::toNativePath(const std::string& Path)
{
  return std::filesystem::path(openfluid::tools::Filesystem::cleanPath(Path)).make_preferred().string();
}


// =====================================================================
// =====================================================================


std::string Filesystem::toGenericPath(const std::string& Path)
{
  return std::filesystem::path(openfluid::tools::Filesystem::cleanPath(Path)).generic_string();
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
  return PathImpl(Path).stdPath().filename().generic_string();
}


// =====================================================================
// =====================================================================


std::string Filesystem::dirname(const std::string& Path)
{
  return PathImpl(Path).stdPath().parent_path().generic_string();
}


// =====================================================================
// =====================================================================


std::string Filesystem::basename(const std::string& Path)
{
  return PathImpl(Path).stdPath().stem().generic_string();
}


// =====================================================================
// =====================================================================


std::string Filesystem::minimalBasename(const std::string& Path)
{
  std::string TmpFilename = PathImpl(Path).stdPath().filename().generic_string();
  size_t FirstDot = TmpFilename.find_first_of(".");

  if (FirstDot != std::string::npos)
  {
    return TmpFilename.substr(0,FirstDot);
  }

  return TmpFilename;
}


// =====================================================================
// =====================================================================


std::string Filesystem::extension(const std::string& Path)
{
  std::string Ext = PathImpl(Path).stdPath().extension().generic_string();

  if (!Ext.empty() && Ext[0] == '.')
  {
    Ext.erase(0,1);
  }

  return Ext;
}


// =====================================================================
// =====================================================================


std::string Filesystem::completeExtension(const std::string& Path)
{
  std::string TmpFilename = PathImpl(Path).stdPath().filename().generic_string();
  size_t FirstDot = TmpFilename.find_first_of(".");

  if (FirstDot != std::string::npos)
  {
    return TmpFilename.substr(FirstDot+1);
  }

  return "";
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

  return removeTrailingSeparators(
           std::filesystem::absolute(PathImpl(TmpStr).stdPath()).lexically_normal().generic_string()
         );
}


// =====================================================================
// =====================================================================


std::string Filesystem::cleanPath(const std::string& Path)
{
  return removeTrailingSeparators(PathImpl(Path).stdPath().lexically_normal().generic_string());
}


// =====================================================================
// =====================================================================


bool Filesystem::isDirectory(const std::string& Path)
{
  return PathImpl(Path).isDirectory();
}


// =====================================================================
// =====================================================================


bool Filesystem::isFile(const std::string& Path)
{
  return PathImpl(Path).isFile();
}


// =====================================================================
// =====================================================================


bool Filesystem::exists(const std::string& Path)
{
  return PathImpl(Path).exists();
}


// =====================================================================
// =====================================================================


bool Filesystem::copyFile(const std::string& SrcPath, const std::string& DestPath)
{
  std::error_code TmpErr;
  return std::filesystem::copy_file(PathImpl(SrcPath).stdPath(),PathImpl(DestPath).stdPath(),TmpErr);
}


// =====================================================================
// =====================================================================


bool Filesystem::renameFile(const std::string& OriginalPath, const std::string& NewPath)
{
  std::error_code TmpErr;
  const auto OrigStdPath = PathImpl(OriginalPath);
  const auto NewStdPath = PathImpl(NewPath);

  std::filesystem::rename(OrigStdPath.stdPath(),NewStdPath.stdPath(),TmpErr);
  return (NewStdPath.isFile() && !OrigStdPath.isFile());
}


// =====================================================================
// =====================================================================


bool Filesystem::removeFile(const std::string& Path)
{
  return PathImpl(Path).removeFile();
}


// =====================================================================
// =====================================================================


bool Filesystem::makeDirectory(const std::string& Path)
{
  return PathImpl(Path).makeDirectory();
}


// =====================================================================
// =====================================================================


bool Filesystem::removeDirectory(const std::string& Path)
{
  return PathImpl(Path).removeDirectory();
}


// =====================================================================
// =====================================================================


bool Filesystem::copyDirectory(const std::string& SrcPath, const std::string& DestPath,
                               bool WithBaseDir, bool RemoveExisting)
{
  const auto SrcPathImpl = PathImpl(SrcPath);
  auto DestPathImpl = PathImpl(DestPath);


  if (WithBaseDir)
  {
    DestPathImpl = PathImpl(DestPath+"/"+PathImpl(SrcPath).stdPath().filename().generic_string());
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
  if (PathImpl(DestPathImpl.stdPath().parent_path()).makeDirectory())
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


bool Filesystem::emptyDirectory(const std::string& Path)
{
  for (const auto & Entry : std::filesystem::directory_iterator(PathImpl(Path).stdPath()))
  {
    if (Entry.is_directory())
    {
      if (!removeDirectory(Entry.path().string()))
      {
        return false;
      }
    }
    else
    {
      if (!removeFile(Entry.path().string()))
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

  if (makeDirectory(Path))
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
      return joinPath({Path,SubdirName+"-"+PIDPUI+"-"+std::to_string(IncSuffix)}); 
    };

    std::string CandidateFullPath =  generateFullPath();

    while (exists(CandidateFullPath))
    {
      IncSuffix++;
      CandidateFullPath = generateFullPath();
    }

    if (makeDirectory(CandidateFullPath))
    {
      return CandidateFullPath;
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
  if (makeDirectory(Path))
  {
    std::ostringstream SS;
    SS << std::this_thread::get_id();
    static std::string PID = SS.str(); // use thread id to improve entropy

    std::string FileRoot = minimalBasename(FileName);
    std::string FileExt = completeExtension(FileName);
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
      return joinPath({Path,FileRoot+"-"+PIDPUI+std::to_string(IncSuffix)+FileExt}); 
    };

    std::string CandidateFilePath = generateFullPath();

    while (exists(CandidateFilePath))
    {
      IncSuffix++;
      CandidateFilePath = generateFullPath();
    }

    std::ofstream EmptyFile(CandidateFilePath,std::ofstream::out);
    if (EmptyFile.is_open())
    {
      EmptyFile.close();
      return CandidateFilePath;
    }
  }

  return std::string();
}


// =====================================================================
// =====================================================================


std::vector<std::string> Filesystem::findFiles(const std::string& Path, bool WithPath, const std::string& Pattern)
{
  std::vector<std::string> FilesList;
  const auto SearchPath = PathImpl(Path);

  if (SearchPath.isDirectory())
  {
    std::regex RegExp(Pattern);

    for (const auto & Entry : std::filesystem::directory_iterator(SearchPath.stdPath()))
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
  const auto SearchPath = PathImpl(Path);

  if (SearchPath.isDirectory())
  {
    std::regex RegExp(Pattern);

    for (const auto & Entry : std::filesystem::directory_iterator(SearchPath.stdPath()))
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
