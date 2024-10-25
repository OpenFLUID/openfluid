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
  @file FilesystemPath.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_TOOLS_FILESYSTEMPATH_HPP__
#define __OPENFLUID_TOOLS_FILESYSTEMPATH_HPP__


#include <string>
#include <vector>
#include <filesystem>


#include <openfluid/utils/CppLangHelpers.hpp>
#include <openfluid/global.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


static std::error_code DefaultErrorCode = std::error_code();


class OPENFLUID_API FilesystemPath
{
  private:
    
    std::filesystem::path m_Path;


  public:

    /**
      Returns the separator used in paths lists according to current operating system
      @return the list separator character
    */
    constexpr static inline char listSeparator() noexcept
    {
#if defined(OPENFLUID_OS_UNIX)
      return ':';
#elif defined(OPENFLUID_OS_WINDOWS)
      return ';';
#endif
    } 

    /**
      Returns the path separator according to current operating system
      @return the path separator character
    */
    constexpr static inline char separator() noexcept
    {
#if defined(OPENFLUID_OS_UNIX)
      return '/';
#elif defined(OPENFLUID_OS_WINDOWS)
      return '\\';
#endif
    }

    CPPCLASS_DEFAULT_FIVE(FilesystemPath)

    /**
      Constructs a FilesystemPath object using a string
      @param[in] PathStr the path as a string
    */
    FilesystemPath(const std::string& PathStr);

    /**
      Constructs a FilesystemPath object using a C-style string
      @param[in] PathStr the path as a char*
    */
    FilesystemPath(const char* PathStr);

    /**
      Constructs a FilesystemPath object using a vector of strings that will be joined using the path separator
      @param[in] PathParts the path as a string vector
    */
    FilesystemPath(const std::vector<std::string>& PathParts);

    /**
      Creates a FilesystemPath object from a std::filesystem::path object
      @param[in] StdPath the std::filesystem::path object
      @return The newly created FilesystemPath object
    */
    static FilesystemPath fromStdPath(const std::filesystem::path& StdPath);

    /**
      Returns a const reference to the internally stored path (std::filesystem::path)
    */
    const std::filesystem::path& stdPath() const;

    /**
      Creates a new FilesystemPath object from the current FilesystemPath object.
      @param[in] PathStr a relative path added to the to the current stored path (optional, none by default)
      @return The newly created FilesystemPath object
    */
    FilesystemPath fromThis(const std::string& PathStr = "") const;
    
    /**
      Creates a new FilesystemPath object from the current FilesystemPath object.
      @param[in] PathParts a relative joined path added to the to the current stored path (optional, none by default)
      @return The newly created FilesystemPath object
    */
    FilesystemPath fromThis(const std::vector<std::string>& PathParts) const;

    /**
      Returns true if the current strored path is empty
      @return true if the current strored path is empty, false otherwise
    */
    bool empty() const;

    /**
      Removes trailing separators in the given path, if any.
      @param[in] Path the path
      @param[in] Sep the separator character (default is '/')
      @return the path without any trailing separator
    */
    static std::string removeTrailingSeparators(const std::string& Path, char Sep = '/') noexcept;

    /**
      Returns the path as a path with native paths separators
      (e.g. '/' on Unix systems, '\' on Windows systems)
      @return the cleaned path with native separators
    */
    std::string toNative() const;

    /**
      Returns the path as a path with generic paths separators (always '/')
      @return the cleaned path with generic separators
    */
    std::string toGeneric() const;

    /**
      Returns the path as a cleaned path with generic paths separators (always '/')
      @return the cleaned path with generic separators
    */
    std::string toGenericClean() const;

    /**
      Returns parts of the path, splitted using the path separator 
      @return the path parts
    */
    std::vector<std::string> split() const;

    /**
      Returns the filename of the path
      @snippet misc/filesystempath.cpp filename
      @return the filename
    */
    std::string filename() const;

    /**
      Returns the directory name (parent path)
      @snippet misc/filesystempath.cpp dirname
      @return the directory name
    */
    std::string dirname() const;


    /**
      Returns the complete base name of the path
      (without latest extension part, without trailing dot if any)
      @snippet misc/filesystempath.cpp basename
      @return the base name of the file
    */
    std::string basename() const;

    /**
      Returns the minimal base name of the path
      (e.g. without any extension part, without trailing dot if any)
      @snippet misc/filesystempath.cpp minimalbasename
      @return the minimal base name of the file
    */
    std::string minimalBasename() const;

    /**
      Returns the extension (without dot character) of the path
      @snippet misc/filesystempath.cpp extension
      @return the extension
    */
    std::string extension() const;

   /**
      Returns the complete extension (without first dot character) of the path
      @snippet misc/filesystempath.cpp completeextension
      @return the complete extension
    */
    std::string completeExtension() const;

    /**
      Returns true if a the path contains a child path (e.g. /path/to contains /path/to/child/path)
      @param[in] Path the child path
      @return true or false
    */ 
    bool contains(const std::string& Path) const;

    /**
      Returns the resulting path relative to a given path
      (e.g with path /path/to/this/place relatively to given path /path/to, resulting path is this/place)
      @param[in] Path the given path  
      @return the resulting relative path
    */
    std::string relativeTo(const std::string& Path) const;

    /**
      Returns true if the path is a directory
      @param[in] Path a relative path added to the to the base path (optional, none by default)
      @return true or false
    */
    bool isDirectory(const std::string& Path = "") const;

    /**
      Returns true if the path is a file or a valid symbolic link
      @param[in] Path a relative path added to the to the base path (optional, none by default)
      @return true or false
    */
    bool isFile(const std::string& Path = "") const;

    /**
      Returns true if the path exists (a file, a directory or a valid symbolic link)
      @param[in] Path a relative path added to the to the base path (optional, none by default)
      @return true or false
    */
    bool exists(const std::string& Path = "") const;

    /**
      Creates the directory of the path. It creates all parent directories necessary to create the directory.
      If the directory already exists, it does nothing.
      @param[in] Path a relative path added to the to the base path (optional, none by default)
      @return true if the directory has been successfully created or if it already exists, false otherwise
    */
    bool makeDirectory(const std::string& Path = "", std::error_code& ErrorCode=DefaultErrorCode) const;

    /**
      Removes the directory of the path. It recursively deletes all contents of the directory.
      @param[in] Path a relative path added to the to the base path (optional, none by default)
      @return true if the directory has been successfully deleted, false otherwise
    */
    bool removeDirectory(const std::string& Path = "", std::error_code& ErrorCode=DefaultErrorCode) const;
    
    /**
      Creates an empty file at the path. It creates all parent directories necessary to create the file.
      If the file already exists, it does nothing.
      @param[in] Path a relative path added to the to the base path (optional, none by default)
      @return true if the file has been successfully created or if it already exists, false otherwise
    */
    bool makeFile(const std::string& Path = "") const;

    /**
      Removes the file of the path.
      @param[in] Path a relative path added to the to the base path (optional, none by default)
      @return true if the file was successfully deleted, false otherwise
    */
    bool removeFile(const std::string& Path = "", std::error_code& ErrorCode=DefaultErrorCode) const;

    /**
      Removes the file or directory of the path.
      @param[in] Path a relative path added to the to the base path (optional, none by default)
      @return true if the file was successfully deleted, false otherwise
    */
    bool remove(const std::string& Path = "", std::error_code& ErrorCode=DefaultErrorCode) const;
};

/**
  Alias for the openfluid::tools::FilesystemPath class
  @see openfluid::tools::FilesystemPath
*/
using Path = FilesystemPath;


} }  // namespaces


#endif /* __OPENFLUID_TOOLS_FILESYSTEMPATH_HPP__ */
