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
  @file FilesystemPath.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <fstream> 
#include <regex>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include <openfluid/tools/FilesystemPath.hpp>


// =====================================================================
// =====================================================================


std::filesystem::path composeWithSubPath(const std::filesystem::path& Base, const std::string& Sub)
{
  auto Result = Base;

  if (!Sub.empty())
  {
    Result /= Sub;
  }

  return Result;
}


// =====================================================================
// =====================================================================


namespace openfluid { namespace tools {


FilesystemPath::FilesystemPath(const std::string& PathStr) :
  m_Path(std::filesystem::path(PathStr,std::filesystem::path::format::generic_format))
{
  // NOTE automatically clean path?
}


// =====================================================================
// =====================================================================


FilesystemPath::FilesystemPath(const char* PathStr) : 
  FilesystemPath(std::string(PathStr))
{

}


// =====================================================================
// =====================================================================


FilesystemPath::FilesystemPath(const std::vector<std::string>& PathParts) : 
  FilesystemPath(std::string(boost::algorithm::join(PathParts,"/")))
{

}


// =====================================================================
// =====================================================================


FilesystemPath FilesystemPath::fromStdPath(const std::filesystem::path& StdPath)
{
  FilesystemPath P;
  P.m_Path = StdPath;
  return P;
}


// =====================================================================
// =====================================================================


const std::filesystem::path& FilesystemPath::stdPath() const
{
  return m_Path;
}


// =====================================================================
// =====================================================================


FilesystemPath FilesystemPath::fromThis(const std::string& PathStr) const
{
  return FilesystemPath::fromStdPath(composeWithSubPath(stdPath(),PathStr));
}


// =====================================================================
// =====================================================================


FilesystemPath FilesystemPath::fromThis(const std::vector<std::string>& PathParts) const
{
  return FilesystemPath::fromStdPath(composeWithSubPath(stdPath(),boost::algorithm::join(PathParts,"/")));
}


// =====================================================================
// =====================================================================


bool FilesystemPath::empty() const
{
  return m_Path.empty();
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::removeTrailingSeparators(const std::string& Path, char Sep) noexcept
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


std::string FilesystemPath::toNative() const
{
  auto TmpPath = m_Path; 
  return TmpPath.make_preferred().string();
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::toGeneric() const
{
  return m_Path.generic_string();
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::toGenericClean() const
{
  return removeTrailingSeparators(m_Path.lexically_normal().generic_string(),'/');
}


// =====================================================================
// =====================================================================


std::vector<std::string> FilesystemPath::split() const
{
  std::vector<std::string> Parts;
  
  boost::algorithm::split(Parts,toGeneric(),boost::is_any_of("/"));

  // remove empty parts as boost::token_compress_on is not sufficient for leading and trailing empty parts (if any)
  Parts.erase(
    std::remove_if(Parts.begin(),Parts.end(), [](const std::string& S){ return S.empty(); }),
    Parts.end()
  );
  
  return Parts;
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::filename() const
{
  return m_Path.filename().generic_string();
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::dirname() const
{
  return m_Path.parent_path().generic_string();
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::basename() const
{
  return m_Path.stem().generic_string();
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::minimalBasename() const
{
  std::string TmpFilename = m_Path.filename().generic_string();
  size_t FirstDot = TmpFilename.find_first_of(".");

  if (FirstDot != std::string::npos)
  {
    return TmpFilename.substr(0,FirstDot);
  }

  return TmpFilename;
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::extension() const
{
  std::string Ext = m_Path.extension().generic_string();

  if (!Ext.empty() && Ext[0] == '.')
  {
    Ext.erase(0,1);
  }

  return Ext;
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::completeExtension() const
{
  std::string TmpFilename = m_Path.filename().generic_string();
  size_t FirstDot = TmpFilename.find_first_of(".");

  if (FirstDot != std::string::npos)
  {
    return TmpFilename.substr(FirstDot+1);
  }

  return "";
}


// =====================================================================
// =====================================================================


bool FilesystemPath::contains(const std::string& Path) const
{
  std::string P(toGenericClean());
  std::string CP(FilesystemPath(Path).toGenericClean());
  return (CP.rfind(P, 0) == 0);
}


// =====================================================================
// =====================================================================


std::string FilesystemPath::relativeTo(const std::string& Path) const
{
  if (empty())
  {
    return "";  // 
  }

  return removeTrailingSeparators(
           std::filesystem::relative(m_Path,FilesystemPath(Path).stdPath()).lexically_normal().generic_string()
         );
}


// =====================================================================
// =====================================================================


bool FilesystemPath::isDirectory(const std::string& Path) const
{
  return std::filesystem::is_directory(composeWithSubPath(m_Path,Path));
}


// =====================================================================
// =====================================================================


bool FilesystemPath::isFile(const std::string& Path) const
{
  const auto CompPath = composeWithSubPath(m_Path,Path);
  return (std::filesystem::is_regular_file(CompPath) || std::filesystem::is_symlink(CompPath));
}


// =====================================================================
// =====================================================================


bool FilesystemPath::exists(const std::string& Path) const
{
  return std::filesystem::exists(composeWithSubPath(m_Path,Path));
}


// =====================================================================
// =====================================================================


bool FilesystemPath::makeDirectory(const std::string& Path) const
{
  if (m_Path.empty() && Path.empty())
  {
    return false;
  }

  const auto CompPath = composeWithSubPath(m_Path,Path);
  std::error_code TmpErr;
  return (std::filesystem::is_directory(CompPath) || std::filesystem::create_directories(CompPath,TmpErr));
}


// =====================================================================
// =====================================================================


bool FilesystemPath::removeDirectory(const std::string& Path) const
{
  if (m_Path.empty() && Path.empty())
  {
    return false;
  }

  if (isDirectory(Path))
  {  
    const auto CompPath = composeWithSubPath(m_Path,Path);
    std::error_code TmpErr;
    std::filesystem::remove_all(CompPath,TmpErr);  
  }
  return !exists(Path);
}


// =====================================================================
// =====================================================================


bool FilesystemPath::makeFile(const std::string& Path) const
{
  if (m_Path.empty() && Path.empty())
  {
    return false;
  }

  const auto CompPath = composeWithSubPath(m_Path,Path);

  if (!isFile(Path))
  {
    std::ofstream MadeFile(fromStdPath(CompPath).toGeneric(),std::ofstream::out);
    if (MadeFile.is_open())
    {
      MadeFile.close();
    }
  }

  std::error_code TmpErr;  
  return (std::filesystem::is_regular_file(CompPath) || std::filesystem::is_symlink(CompPath));
}


// =====================================================================
// =====================================================================


bool FilesystemPath::removeFile(const std::string& Path) const
{
  if (m_Path.empty() && Path.empty())
  {
    return false;
  }

  if (isFile(Path))
  {
    const auto CompPath = composeWithSubPath(m_Path,Path);
    std::error_code TmpErr;
    return std::filesystem::remove(CompPath,TmpErr);
  }
  return !exists(Path); 
}


// =====================================================================
// =====================================================================


bool FilesystemPath::remove(const std::string& Path) const
{
  return (removeFile(Path) || removeDirectory(Path));
}


} }  // namespaces
