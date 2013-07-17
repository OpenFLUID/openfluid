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
 * Archiver.hpp
 *
 *  Created on: 15 juil. 2013
 *      Author: Manuel CHATAIGNER
*/

#ifndef __ARCHIVER_HPP__
#define __ARCHIVER_HPP__

#include <string>
#include <vector>
#include <map>
#include <openfluid/dllexport.hpp>
#include <openfluid/tools/SwissTools.hpp>

#include <sys/stat.h>
#include <fcntl.h>
#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>


namespace openfluid { namespace tools {



class DLLEXPORT Archiver
{
  private:

    /**
      @action Removes slash after dir name if the path passed as parameter contains
    */
    static std::string removeLastSlash(const std::string& DirPath);

    /**
      @action Get relative path of the file from directory path
      @param File path
      @param Path of working directory
    */
    static std::string getRelativePath(const std::string& FilePath, const std::string& DirPath);


  public:

    /**
      @action Create zip archive which contains all files of InputDir passed as parameter
    */
    static void compressDirectoryAsZip(const std::string& InputDir, const std::string& OutputFile);

    /**
      @action Read and return content of archive passed as parameter
    */
    static void uncompressArchive(const std::string& ArchivePath, const std::string& OutputDir);
};


} } //namespaces



#endif // __ARCHIVER_HPP__
