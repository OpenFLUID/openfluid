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

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


class OPENFLUID_API Filesystem
{

  public:

    Filesystem() = delete;

    /**
      Returns a joined path string from a vector of path parts
      @param[in] PathParts a vector of path parts
      @return the joined path
      @snippet misc/filesystem.cpp joinpath
    */
    static std::string joinPath(const std::vector<std::string>& PathParts);

    /**
      Returns the name of the file in the given path
      @snippet misc/filesystem.cpp filename
      @param[in] Path the given path
      @return the filename
    */
    static std::string filename(const std::string& Path);

    /**
      Returns the complete base name of the file in the given path
      @snippet misc/filesystem.cpp basename
      @param[in] Path the given path
      @return the base name of the file
    */
    static std::string basename(const std::string& Path);

    /**
      Returns the directory name (parent path) of the given path
      @snippet misc/filesystem.cpp dirname
      @param[in] Path the given path
      @return the directory name
    */
    static std::string dirname(const std::string& Path);

    /**
      Returns the extension of the file of the given path
      @snippet misc/filesystem.cpp extension
      @param[in] Path the given path
      @return the extension
    */
    static std::string extension(const std::string& Path);

    /**
      Returns the current application path
      @return the current path
    */
    static std::string currentPath();

    /**
      Returns true if the given path is a directory
      @param[in] Path the given path
      @return true or false
    */
    static bool isDirectory(const std::string& Path);

    /**
      Returns true if the given path is a file or a valid symbolic link
      @param[in] Path the given path
      @return true or false
    */
    static bool isFile(const std::string& Path);

    /**
      Creates the directory of the given path. It creates all parent directories necessary to create the directory.
      If the directory already exists, it does nothing.
      @param[in] Path the given path
      @return true if the directory was successfully created, false otherwise
    */
    static bool makeDirectory(const std::string& Path);

    /**
      Removes the directory of the given path. It recursively deletes all contents of the directory.
      @param[in] Path the given path
      @return true if the directory was successfully deleted, false otherwise
    */
    static bool removeDirectory(const std::string& Path);

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
      Removes the file of the given path.
      @param[in] Path the given path
      @return true if the file was successfully deleted, false otherwise
    */
    static bool removeFile(const std::string& Path);

    /**
      Copies a file from source to destination.
      @param[in] SrcPath the source path
      @param[in] DestPath the destination path
      @return true if the file was successfully copied, false otherwise
    */
    static bool copyFile(const std::string& SrcPath, const std::string& DestPath);

    /**
      Recursively copies a directory from source to destination.
      @param[in] SrcPath the source path
      @param[in] DestPath the destination path
      @param[in] DontCopyDotDirs if set to true, it ignores the directories beginning with a dot ('.').
                 Default is false
      @return true if the directory was successfully copied, false otherwise
    */
    static bool copyDirectory(const std::string& SrcPath, const std::string& DestPath,
                              const bool DontCopyDotDirs = false);

};


} }  // namespaces


#endif /* __OPENFLUID_TOOLS_FILESYSTEM_HPP__ */
