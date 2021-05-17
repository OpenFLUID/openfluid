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
  @file filesystem.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/tools/Filesystem.hpp>


void snippets_filesystem()
{

{
//! [joinpath]
std::string Path;

Path = openfluid::tools::Filesystem::joinPath({"my","joined","path","myfile.txt"});
// Path = "my/joined/path/myfile.txt"
Path = openfluid::tools::Filesystem::joinPath({"/another","joined","path/anotherfile.txt"});
// Path = "/another/joined/path/anotherfile.txt"
//! [joinpath]
}


{
//! [filename]
std::string Name;

Name = openfluid::tools::Filesystem::filename("/tmp/archive.tar.gz");
// Name = "archive.tar.gz"
//! [filename]
}


{
//! [basename]
std::string Name;

Name = openfluid::tools::Filesystem::basename("/tmp/archive.tar.gz");
// Name = "archive.tar"
//! [basename]
}

{
//! [minimalbasename]
std::string Name;

Name = openfluid::tools::Filesystem::minimalBasename("/tmp/archive.tar.gz");
// Name = "archive"
//! [minimalbasename]
}


{
//! [dirname]
std::string Name;

Name = openfluid::tools::Filesystem::dirname("/tmp/archive.tar.gz");
// Name = "/tmp"
//! [dirname]
}


{
//! [extension]
std::string Ext;

Ext = openfluid::tools::Filesystem::extension("/tmp/archive.tar.gz");
// Ext = "gz"
//! [extension]
}


{
//! [completeextension]
std::string Ext;

Ext = openfluid::tools::Filesystem::completeExtension("/tmp/archive.tar.gz");
// Ext = "tar.gz"
//! [completeextension]
}


}
