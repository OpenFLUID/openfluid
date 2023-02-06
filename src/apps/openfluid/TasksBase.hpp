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
  @file TasksBase.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <iostream>

#include <openfluid/tools/Console.hpp>
#include <openfluid/utils/CommandLineParser.hpp>


#ifndef __OPENFLUID_CMDLINEAPP_BASE_HPP__
#define __OPENFLUID_CMDLINEAPP_BASE_HPP__


class TasksBase
{
  protected:
    
    const openfluid::utils::CommandLineCommand m_Cmd;
    
    const std::vector<std::string> m_ExtraArgs;

    const std::vector<std::string> m_ThirdPartyArgs;

    // TOIMPL should be removed once all tasks are implemented
    static int notImplemented()
    {
      openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::BLUE);
      std::cout << "[NOT IMPLEMENTED]";
      openfluid::tools::Console::resetAttributes();
      std::cout << std::endl;
      return 42;
    }


  public:

    TasksBase() = delete;

    TasksBase(const openfluid::utils::CommandLineParser& Parser) : 
      m_Cmd(Parser.command(Parser.getActiveCommand())),
      m_ExtraArgs(Parser.extraArgs()), m_ThirdPartyArgs(Parser.thirdPartyArgs())
    { }

    virtual ~TasksBase()
    { }

    inline static const int ErrorCode = -1;

    virtual int process() const = 0;

    static int unknownCommand()
    {
      return error("unknown command");
    }

    static int error(const std::string& Msg = "", const std::string& Cat = "ERROR")
    {
      std::cout << "\n";

      if (!Cat.empty())
      {
        openfluid::tools::Console::setErrorColor();
        std::cout << "[" << Cat << "]";
        openfluid::tools::Console::resetAttributes();
        std::cout << " ";
      }
      std::cout << Msg;
      std::cout << std::endl;

      return ErrorCode;
    }

};

#endif /* __OPENFLUID_CMDLINEAPP_BASE_HPP__ */
