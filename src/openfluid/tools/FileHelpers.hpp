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


namespace openfluid { namespace tools {


/**
  Gets the list of files with specified extension contained in the specified directory
  @param[in] Path the directory to explore
  @param[in] Ext the file extension
  @param[in] WithPath return full path with file name if true, file name only otherwise
  @param[in] ExtIncludeDot if true, the given extension through Ext parameter is suffixed by a dot
*/
std::vector<std::string> OPENFLUID_API findFilesByExtension(const std::string& Path,
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
std::vector<std::string> OPENFLUID_API findFilesBySuffixAndExtension(const std::string& Path,
                                                                     const std::string& Suffix,
                                                                     const std::string& Ext,
                                                                     bool WithPath = false,
                                                                     bool ExtIncludeDot = false);

/**
  Gets the list of directories found in the specified directory
  @param[in] Path the directory to explore
  @param[in] WithPath return full path if true, directory name only otherwise
*/
std::vector<std::string> OPENFLUID_API findDirectories(const std::string& Path,
                                                       bool WithPath = false);

/**
  Recursively removes all files and directories contained in the given directory.
  It deletes the directory and recreates it.
  @param[in] Path the directory to empty
  @return true if successful
*/
bool OPENFLUID_API emptyDirectoryRecursively(const std::string& Path);

/**
  Copies a source directory to destination path, not including the root of the source directory.
  If the destination path does not exists, it is created.
  @param[in] SrcPath the source directory to copy
  @param[in] DestPath the destination directory
  @param[in] DontCopyDotDirs flag for copying dot ('.*') directories. Default is false.
*/
void OPENFLUID_API copyDirectoryContentsRecursively(const std::string& SrcPath,
                                                    const std::string& DestPath,
                                                    const bool DontCopyDotDirs = false);


} }  // namespaces


#endif /* __OPENFLUID_TOOLS_FILEHELPERS_HPP__ */
