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
  @file Filesystem.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_TOOLS_FILESYSTEM_HPP__
#define __OPENFLUID_TOOLS_FILESYSTEM_HPP__


#include <string>
#include <vector>

#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


// TOIMPL Refactor for consistent usage of FilesystemPath and std::string


class OPENFLUID_API Filesystem
{

  public:

    Filesystem() = delete;

    /**
      @deprecated Since version 2.2.0. Use openfluid::tools::FilesystemPath::listSeparator() instead
    */
    [[deprecated]] constexpr static inline char pathsListSeparator() noexcept
    {
#if defined(OPENFLUID_OS_UNIX)
      return ':';
#elif defined(OPENFLUID_OS_WINDOWS)
      return ';';
#endif
    } 

    /**
      @deprecated Since version 2.2.0. Use openfluid::tools::FilesystemPath::separator() instead
    */
    [[deprecated]] constexpr static inline char pathSeparator() noexcept
    {
#if defined(OPENFLUID_OS_UNIX)
      return '/';
#elif defined(OPENFLUID_OS_WINDOWS)
      return '\\';
#endif
    }

    /**
      Returns a joined path string from a vector of path parts, using the '/' separator
      @param[in] PathParts a vector of path parts
      @return the joined path
      @snippet misc/filesystem.cpp joinpath
    */
    static std::string joinPath(const std::vector<std::string>& PathParts);

    /**
      Returns the user home path (e.g. '/home/username' on Linux, 'C:/Users/username' on Windows, ...)
      @return the user home path
    */
    static std::string homePath();

    /**
      Returns the system path for temporary files (e.g. '/tmp' on Linux, ...)
      @return the path for temporary files
    */
    static std::string tempPath();

    /**
      @deprecated Since version 2.2.0. Use openfluid::tools::FilesystemPath::filename() instead
    */
    [[deprecated]] static std::string filename(const std::string& Path);

    /**
      @deprecated Since version 2.2.0. Use openfluid::tools::FilesystemPath::dirname() instead
    */
    [[deprecated]] static std::string dirname(const std::string& Path);

    /**
      @deprecated Since version 2.2.0. Use openfluid::tools::FilesystemPath::basename() instead
    */
    [[deprecated]] static std::string basename(const std::string& Path);

    /**
      @deprecated Since version 2.2.0. Use openfluid::tools::FilesystemPath::minimalBasename() instead
    */
    [[deprecated]] static std::string minimalBasename(const std::string& Path);

    /**
      @deprecated Since version 2.2.0. Use openfluid::tools::FilesystemPath::extension() instead
    */
    [[deprecated]] static std::string extension(const std::string& Path);

    /**
      @deprecated Since version 2.2.0. Use openfluid::tools::FilesystemPath::completeExtension() instead
    */
    [[deprecated]] static std::string completeExtension(const std::string& Path);

    /**
      Returns the current path
      @return the current path
    */
    static std::string currentPath();

    /**
      Returns the absolute path of the given path.
      If the given path is already an absolute path, it is returned as-is.
      @param[in] Path the path to make absolute  
      @return the absolute path
    */
    static std::string absolutePath(const std::string& Path);

    /**
      Creates a unique subdirectory in the given path, using the given subdirectory name as a prefix.
      If the subdirectory already exists, it adds an incremental suffix to the subdirectory name.
      It creates all parent directories necessary to create the subdirectory.
      @param[in] Path the given path
      @param[in] SubdirName the given path
      @return the full path of the created unique subdirectory
    */
    static std::string makeUniqueSubdirectory(const std::string& Path, const std::string& SubdirName);

    /**
      Creates a unique file in the given path, using the given File name and extension as prefix and suffix.
      If the file already exists, it adds an incremental part to the file name.
      If the Path does not exists, it creates all needed parent directories.
      @param[in] Path the given path for the file
      @param[in] FileName the file name used to make a unique one
      @return the full path of the created unique file
    */
    static std::string makeUniqueFile(const std::string& Path, const std::string& FileName);

    /**
      Reads the content of a file
      @param[in] FileObj the given path for the file
      @return the content of the file
    */
    static std::string readFile(const openfluid::tools::FilesystemPath& FileObj);

    /**
      Writes content into a file
      @param[in] Content the content to write
      @param[in] FileObj the given path for the file
    */
    static void writeFile(const std::string& Content, const openfluid::tools::FilesystemPath& FileObj);

    /**
      Copies a file from source to destination.
      @param[in] SrcPath the source path
      @param[in] DestPath the destination path
      @return true if the file was successfully copied, false otherwise
    */
    static bool copyFile(const std::string& SrcPath, const std::string& DestPath);

    /**
      Rename a file.
      @param[in] OriginalPath the original file path
      @param[in] NewPath the new file path
      @return true if the file was successfully renamed, false otherwise
    */
    static bool renameFile(const std::string& OriginalPath, const std::string& NewPath);

    /**
      Recursively copies a directory from source to destination.
      @param[in] SrcPath the source path
      @param[in] DestPath the destination path
      @param[in] WithBaseDir if true, the source directory is created in the destination dir 
                             as an intermdiate directory (e.g. if the content of /my/path/first is copied 
                             into /my/path/second, the content will actually be copied into /my/path/second/first) 
      @param[in] RemoveExisting if true, the destination directory is deleted if it already exists
      @return true if the directory was successfully copied, false otherwise
    */
    static bool copyDirectory(const std::string& SrcPath, const std::string& DestPath,
                              bool WithBaseDir = false, bool RemoveExisting = false);

    
    /**
      Recursively copies content of a directory from source to destination, handling windows bug with regular dir copy.
      @param[in] SrcPath the source path
      @param[in] DestPath the destination path
      @return true if the directory was successfully copied, false otherwise
    */
    static bool copyDirectoryContent(const std::filesystem::path& SrcPath, const std::filesystem::path& DestPath);

    /**
      Recursively removes all files and directories contained in the given directory.
      It deletes the directory and recreates it.
      @param[in] Path the directory to empty
      @param[in] PathsToExlude Paths that are not deleted.
      @return true if successful
    */
    static bool emptyDirectory(const std::string& Path, const std::vector<std::string>& PathsToExlude = {});

    /**
      Gets the list of files found in the specified directory
      @param[in] Path the directory to explore
      @param[in] WithPath return full path if true, directory name only otherwise
      @param[in] Pattern an optional pattern to filter the files to find
    */
    static std::vector<std::string> findFiles(const std::string& Path, 
                                              bool WithPath = false, const std::string& Pattern = "");

    /**
      Gets the list of directories found in the specified directory
      @param[in] Path the directory to explore
      @param[in] WithPath return full path if true, directory name only otherwise
      @param[in] Pattern an optional pattern to filter the directories to find
    */
    static std::vector<std::string> findDirectories(const std::string& Path,
                                                    bool WithPath = false, const std::string& Pattern = "");

    /**
      Gets the list of files with specified extension contained in the specified directory
      @param[in] Path the directory to explore
      @param[in] Ext the file extension
      @param[in] WithPath return full path with file name if true, file name only otherwise
      @param[in] ExtIncludeDot if true, the given extension through Ext parameter is suffixed by a dot
    */
    static std::vector<std::string> findFilesByExtension(const std::string& Path,
                                                         const std::string& Ext,
                                                         bool WithPath = false,
                                                         bool ExtIncludeDot = false);

    /**
      Get list of files with specified extension contained in the specified dir
      @param[in] Path the directory to explore
      @param[in] Ext the file extension
      @param[in] Suffix the file suffix
      @param[in] WithPath return full path with file name if true, file name only otherwise
      @param[in] ExtIncludeDot if true, the given extension through Ext parameter is suffixed by a dot
    */
    static std::vector<std::string> findFilesBySuffixAndExtension(const std::string& Path,
                                                                  const std::string& Suffix,
                                                                  const std::string& Ext,
                                                                  bool WithPath = false,
                                                                  bool ExtIncludeDot = false);

};

} }  // namespaces


#endif /* __OPENFLUID_TOOLS_FILESYSTEM_HPP__ */
