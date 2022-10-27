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
  @file main.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inra.fr>
*/


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char* argv[])
{
  bool IsUserRequest = false;
  if (argc >= 1 && argv[1])
  {
    std::string Request = argv[1];
    // WARNING: following condition uses the git input message, 
    // so if git uses regionalization "Username" may be translated
    // and this check will fail
    if (Request.find("Username") != std::string::npos)
    {
      IsUserRequest = true;
    }
  }

  if (IsUserRequest)
  {
    char* Usr = std::getenv(OFBUILD_GITASKUSER_ENVVAR);
    if (Usr)
    {
      std::cout <<  Usr << std::endl;
    }
    else
    {
      std::cout << "" << std::endl;
    }
  }
  else
  {
    char* Pwd = std::getenv(OFBUILD_GITASKPASS_ENVVAR);
    if (Pwd)
    {
      std::cout <<  Pwd << std::endl;
    }
    else
    {
      std::cout << "" << std::endl;
    }
  }
}
