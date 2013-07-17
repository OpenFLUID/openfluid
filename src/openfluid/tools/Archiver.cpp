/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/

/**
 * Archiver.cpp
 *
 *  Created on: 15 juil. 2013
 *      Author: Manuel CHATAIGNER
*/


#include <openfluid/tools/Archiver.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace tools {


std::string Archiver::removeLastSlash(const std::string& DirPath)
{
  std::string PathToReturn = DirPath;
  if (*PathToReturn.rbegin() == '/')
    PathToReturn.erase(PathToReturn.length()-1,1);

  return PathToReturn;
}


// =====================================================================
// =====================================================================


std::string Archiver::getRelativePath(const std::string& FilePath, const std::string& DirPath)
{
  std::string RelativePath = FilePath;

  size_t Found = RelativePath.find(DirPath);
  if (Found != std::string::npos)
    RelativePath.erase(0, DirPath.length() + 1);

  return RelativePath;
}


// =====================================================================
// =====================================================================


void Archiver::compressDirectoryAsZip(const std::string& InputDir, const std::string& OutputFile)
{
  struct archive* Archive = 0;
  Archive = archive_write_new();
  if (Archive == 0)
    throw openfluid::base::FrameworkException("Archiver::compressDirectoryAsZip","unable to create archive file");

  // Creation of zip archive
  if ((archive_write_set_format_zip(Archive) != ARCHIVE_OK)
   || (archive_write_open_filename(Archive, OutputFile.c_str()) != ARCHIVE_OK))
  {
    throw openfluid::base::FrameworkException("Archiver::compressDirectoryAsZip","error writing archive file");
  }

  // Remove of last slash in dir path if exists
  std::string InputDirPath = removeLastSlash(InputDir);

  // Storage of all file names contained in InputDir
  std::vector<std::string> FileNames = openfluid::tools::getFilesRecursively(InputDirPath);

  std::vector<std::string>::iterator It;

  // For each file found
  for (It = FileNames.begin(); It != FileNames.end(); ++It)
  {
    // Get relative file path from InputDir
    *It = getRelativePath(*It, InputDirPath);

    struct stat FileStat;
    struct archive_entry* Entry = 0;
    if ((Entry = archive_entry_new()) == 0)
      throw openfluid::base::FrameworkException("Archiver::compressDirectoryAsZip","unable to create archive entry");


    std::string FullFilePath = InputDirPath + "/" + *It;
    int Length, FileDescriptor;
    char Buffer[800] = "";

    // Recovery of file informations
    if (stat(FullFilePath.c_str(), &FileStat) == -1)
      throw openfluid::base::FrameworkException("Archiver::compressDirectoryAsZip","unable to get file informations");

    archive_entry_copy_stat(Entry, &FileStat);

    // Set pathname into created archive
    archive_entry_set_pathname(Entry, It->c_str());


    int Ret = archive_write_header(Archive, Entry);
    archive_entry_free(Entry);
    Entry = 0;
    if (Ret != ARCHIVE_OK)
      throw openfluid::base::FrameworkException("Archiver::compressDirectoryAsZip","error writing file header into archive");

    // Reading file
    FileDescriptor = open(FullFilePath.c_str(), O_RDONLY);

    Length = read(FileDescriptor, Buffer, sizeof(Buffer));
    while (Length > 0)
    {
      // Writing data into archive
      if (archive_write_data(Archive, Buffer, Length) != Length)
        throw openfluid::base::FrameworkException("Archiver::compressDirectoryAsZip","error writing file content into archive");

      Length = read(FileDescriptor, Buffer, sizeof(Buffer));
    }

    close(FileDescriptor);
  }

  archive_write_close(Archive);
  archive_write_free(Archive);
}


// =====================================================================
// =====================================================================


void Archiver::uncompressArchive(const std::string& ArchivePath, const std::string& OutputDir)
{
  struct archive* Archive, *ExtDir;
  struct archive_entry* Entry;
  int Length;
  char Buffer[800];

  // Creation of archive reader
  Archive = archive_read_new();
  archive_read_support_filter_all(Archive);
  archive_read_support_format_all(Archive);

  // Creation of disk writer
  ExtDir = archive_write_disk_new();
  archive_write_disk_set_options(ExtDir, ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM
            | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS);


  // Remove of last slash in dir path if exists
  std::string OutpurDirPath = removeLastSlash(OutputDir);


  // Reading archive
  if (archive_read_open_filename(Archive, ArchivePath.c_str(), 10240) != ARCHIVE_OK)
    throw openfluid::base::FrameworkException("Archiver::uncompressArchive","unable to read archive "+ArchivePath);


  // For each file found
  while (archive_read_next_header(Archive, &Entry) == ARCHIVE_OK)
  {
    // Build full file path
    std::string FileName = OutpurDirPath + "/" + archive_entry_pathname(Entry);
    archive_entry_set_pathname(Entry, FileName.c_str());

    // Writing file on the disk
    if (archive_write_header(ExtDir, Entry) != ARCHIVE_OK)
      throw openfluid::base::FrameworkException("Archiver::uncompressArchive","unable to write file header on the disk");

    // Writing data
    while ((Length = archive_read_data(Archive, Buffer, sizeof(Buffer))) > 0)
    {
      if (archive_write_data(ExtDir, Buffer, Length) != Length)
        throw openfluid::base::FrameworkException("Archiver::uncompressArchive","error writing file content on the disk");
    }

    if (archive_write_finish_entry(ExtDir) != ARCHIVE_OK)
      throw openfluid::base::FrameworkException("Archiver::uncompressArchive","error closing out the entry written");
  }

  archive_read_close(Archive);
  archive_read_free(Archive);
  archive_write_close(ExtDir);
  archive_write_free(ExtDir);

}


} } // namespaces
