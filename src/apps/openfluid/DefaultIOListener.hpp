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
  @file DefaultIOListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CMDLINEAPP_DEFAULTIOLISTENER_HPP__
#define __OPENFLUID_CMDLINEAPP_DEFAULTIOLISTENER_HPP__


#include <openfluid/base/IOListener.hpp>
#include <openfluid/tools/Console.hpp>


// =====================================================================
// =====================================================================


class DefaultIOListener : public openfluid::base::IOListener
{
  public:

    DefaultIOListener()
    { }


    ~DefaultIOListener()
    { }


    void onFileLoad(std::string Filename)
    {
      std::cout << "    file: " << Filename << " ";
    }


    void onFileLoaded(openfluid::base::Listener::Status Status)
    {
      if (Status == openfluid::base::Listener::LISTEN_OK)
      {
        openfluid::tools::Console::setOKColor();
        std::cout << "[OK]";
        openfluid::tools::Console::resetAttributes();
      }
      else if (Status == openfluid::base::Listener::LISTEN_WARNING)
      {
        openfluid::tools::Console::setWarningColor();
        std::cout << "[Warning]";
        openfluid::tools::Console::resetAttributes();
      }
      else if (Status == openfluid::base::Listener::LISTEN_ERROR)
      {
        openfluid::tools::Console::setErrorColor();
        std::cout << "[Error]";
        openfluid::tools::Console::resetAttributes();
      }

      std::cout << std::endl;
    }

};


#endif /* __OPENFLUID_CMDLINEAPP_DEFAULTIOLISTENER_HPP__ */
