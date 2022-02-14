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
  @file ArchForCMake.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <iostream>

// global.hpp.in is used here because the final file global.hpp is configured later in the CMake process
// Moreover, the configured part is not used here
#include <openfluid/global.hpp.in>


int main(int argc, char **argv)
{
  std::string Proc = OPENFLUID_PROCESSOR_STRLABEL;
  std::string OS = OPENFLUID_OS_STRLABEL;

  if (Proc.empty() || OS.empty())
  {
    return 127;
  }

  std::cout << OS << "-" << Proc;

  return 0;
}
