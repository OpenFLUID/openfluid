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
  @file DefaultWareSrcOpsListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CMDLINEAPP_DEFAULTWARESRCOPSLISTENER_HPP__
#define __OPENFLUID_CMDLINEAPP_DEFAULTWARESRCOPSLISTENER_HPP__


#include <iostream>
#include <iomanip>

#include <openfluid/waresdev/WareSrcOpsListener.hpp>
#include <openfluid/tools/Console.hpp>


class DefaultWareSrcOpsListener : virtual public openfluid::waresdev::WareSrcOpsListener
{
  protected:

    bool m_Verbose = false;

    bool m_IsFirstStageMsg = true;


    static void displayStatus(const openfluid::base::Listener::Status& Status)
    {
      std::cout << " ";   

      switch (Status)
      {
        case openfluid::base::Listener::Status::ERROR_STATUS :
          openfluid::tools::Console::setErrorColor();
          std::cout << "[Error]";
          openfluid::tools::Console::resetAttributes();
          break;
        case openfluid::base::Listener::Status::OK_STATUS :
          openfluid::tools::Console::setOKColor();
          std::cout << "[OK]";
          openfluid::tools::Console::resetAttributes();
          break;
        case openfluid::base::Listener::Status::WARNING_STATUS :
          openfluid::tools::Console::setWarningColor();
          std::cout << "[Warning]";
          openfluid::tools::Console::resetAttributes();
          break;
      }

      std::cout << std::endl;
      std::cout.flush();

    };

    void displayStartMsg(const std::string& Msg)
    {    
      std::cout << "-- " << Msg;
      if (m_Verbose)
      {
        std::cout << " :";
      }
      std::cout.flush();

      m_IsFirstStageMsg = true;
    }


  public:

    CPPCLASS_DEFAULT_FIVE(DefaultWareSrcOpsListener)

    void setVerbose(bool Enabled)
    {
      m_Verbose = Enabled;
    }

    void stageMessage(const std::string& Msg)
    {
      if (m_Verbose)
      {
        if (!m_IsFirstStageMsg)
        {
          std::cout << ",";
        }
        std::cout << " " << Msg;
        std::cout.flush();
      }
      m_IsFirstStageMsg = false;
    }

};


#endif /* __OPENFLUID_CMDLINEAPP_DEFAULTDOCALYZELISTENER_HPP__ */
