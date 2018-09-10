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
  @file Console.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


// OpenFLUID:stylecheck:!inco
// OpenFLUID:stylecheck:!incs


#ifndef __OPENFLUID_TOOLS_CONSOLE_HPP__
#define __OPENFLUID_TOOLS_CONSOLE_HPP__


#include <string>
#include <iostream>

#include <openfluid/global.hpp>

#if defined(OPENFLUID_OS_WINDOWS)
#include <windows.h>
#endif


namespace openfluid { namespace tools {


class Console
{
  public:

    enum class Colors : char {
      BLACK = 0,
      BLUE = 1,
      GREEN = 2,
      CYAN = 3,
      RED = 4,
      MAGENTA = 5,
      BROWN = 6,
      GREY = 7,
      DARKGREY = 8,
      LIGHTBLUE = 9,
      LIGHTGREEN = 10,
      LIGHTCYAN = 11,
      LIGHTRED = 12,
      LIGHTMAGENTA = 13,
      YELLOW = 14,
      WHITE = 15
    };


    /**
      Returns corresponding ANSI code for given color
      @param[in] Color the given color
      @return the color code as string
    */
    static std::string getANSIColorCode(const Colors& Color)
    {
      switch (Color)
      {
        case Colors::BLACK : return "\033[22;30m";
        case Colors::RED : return "\033[22;31m";
        case Colors::GREEN : return "\033[22;32m";
        case Colors::BROWN : return "\033[22;33m";
        case Colors::BLUE : return "\033[22;34m";
        case Colors::MAGENTA : return "\033[22;35m";
        case Colors::CYAN : return "\033[22;36m";
        case Colors::GREY : return "\033[22;37m";
        case Colors::DARKGREY : return "\033[01;30m";
        case Colors::LIGHTRED : return "\033[01;31m";
        case Colors::LIGHTGREEN : return "\033[01;32m";
        case Colors::YELLOW : return "\033[01;33m";
        case Colors::LIGHTBLUE : return "\033[01;34m";
        case Colors::LIGHTMAGENTA: return "\033[01;35m";
        case Colors::LIGHTCYAN : return "\033[01;36m";
        case Colors::WHITE : return "\033[01;37m";
        default : return "";
      }
    }


    /**
      Saves font attributes (color and weight)
      @return the color code (-1 on non-Windows systems)
    */
    static int saveAttributes()
    {
#if defined(OPENFLUID_OS_WINDOWS)
      static bool Initialized = false;
      static WORD Attributes;

        if (!Initialized)
        {
          CONSOLE_SCREEN_BUFFER_INFO Info;
          GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
          Attributes = Info.wAttributes;
          Initialized = 1;
        }
        return (int)Attributes;
#else
      return -1;
#endif
    }

    /**
      Resets font attributes (color and weight)
    */
    static void resetAttributes()
    {
#if defined(OPENFLUID_OS_WINDOWS)
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)saveAttributes());
#else
      std::cout << "\033[0m";
#endif
    }

    /**
      Sets the given color for console text
      @param[in] Color the given color
    */
    static void setColor(Colors Color)
    {
#if defined(OPENFLUID_OS_WINDOWS)
      HANDLE HStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO Info;

      GetConsoleScreenBufferInfo(HStdOut,&Info);

      SetConsoleTextAttribute(HStdOut,(Info.wAttributes & 0xFFF0) |(WORD)Color);
#else
      std::cout << getANSIColorCode(Color);
#endif
    }

    /**
      Sets the predefined OK color for console text
    */
    static void setOKColor()
    {
#if defined(OPENFLUID_OS_WINDOWS)
      setColor(Colors::LIGHTGREEN);
#else
      std::cout << getANSIColorCode(Colors::LIGHTGREEN);
#endif
    }

    /**
      Sets the predefined warning color for console text
    */
    static void setWarningColor()
    {
#if defined(OPENFLUID_OS_WINDOWS)
      setColor(Colors::YELLOW);
#else
      std::cout << getANSIColorCode(Colors::YELLOW);
#endif
    }

    /**
      Sets the predefined error color for console text
    */
    static void setErrorColor()
    {
#if defined(OPENFLUID_OS_WINDOWS)
      setColor(Colors::LIGHTRED);
#else
      std::cout << getANSIColorCode(Colors::LIGHTRED);
#endif
    }
};


} }  // namespaces


#endif /* __OPENFLUID_TOOLS_CONSOLE_HPP__ */
