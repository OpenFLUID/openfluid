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
  @file FileHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/



#ifndef __OPENFLUID_TOOLS_FILEHELPERS_HPP__
#define __OPENFLUID_TOOLS_FILEHELPERS_HPP__

#include <openfluid/dllexport.hpp>

#include <vector>

#include <QDir>
#include <QFileInfo>



namespace openfluid { namespace tools {


/**
  Gets the list of files with specified extension contained in the specified directory
  @param[in] DirToExplore the directory to explore
  @param[in] Ext the file extension
  @param[in] WithPath return full path with file name if true, file name only otherwise
  @param[in] ExtIncludeDot if true, the given extension through Ext parameter is suffixed by a dot
*/
std::vector<std::string> OPENFLUID_API findFilesByExtension(const std::string DirToExplore,
                                                            const std::string Ext,
                                                            bool WithPath = false,
                                                            bool ExtIncludeDot = false);


/**
  Get list of files with specified extension contained in the specified dir
  @param[in] DirToExplore the directory to explore
  @param[in] Ext the file extension
  @param[in] Suffix the file suffix
  @param[in] WithPath return full path with file name if true, file name only otherwise
  @param[in] ExtIncludeDot if true, the given extension through Ext parameter is suffixed by a dot
*/
std::vector<std::string> OPENFLUID_API findFilesBySuffixAndExtension(const std::string& DirToExplore,
                                                                     const std::string& Suffix,
                                                                     const std::string& Ext,
                                                                     bool WithPath = false,
                                                                     bool ExtIncludeDot = false);


/**
  Recursively removes all files and directories contained in the given directory.
  It deletes the directory and recreates it.
  @param[in] DirPath the directory to empty
  @return true if successful
*/
bool OPENFLUID_API emptyDirectoryRecursively(const std::string& DirPath);


bool OPENFLUID_API removeDirectoryRecursively(const QString& DirPath);


/**
  Recursively finds all the paths of the files that exist exist
  in the given directory and subdirectories
  @param[in] DirPath the directory to explore
  @return a vector containing all files paths
*/
std::vector<std::string> OPENFLUID_API findFilesRecursively(const std::string& DirPath);


/**
  Copies a source directory to destination path, including the root of the source directory.
  If the destination path does not exists, it is created.
  @param[in] SourcePath the source directory to copy
  @param[in] IntoPath the destination directory
  @param[in] DontCopyDotDirs flag for copying dot ('.*') directories. Default is false.
*/
void OPENFLUID_API copyDirectoryRecursively(const std::string& SourcePath,
                                            const std::string& IntoPath,
                                            const bool DontCopyDotDirs = false);


/**
  Copies a source directory to destination path, not including the root of the source directory.
  If the destination path does not exists, it is created.
  @param[in] SourcePath the source directory to copy
  @param[in] IntoPath the destination directory
  @param[in] DontCopyDotDirs flag for copying dot ('.*') directories. Default is false.
*/
void OPENFLUID_API copyDirectoryContentsRecursively(const std::string& SourcePath,
                                                    const std::string& IntoPath,
                                                    const bool DontCopyDotDirs = false);


} }  // namespaces


#endif /* __OPENFLUID_TOOLS_FILEHELPERS_HPP__ */
