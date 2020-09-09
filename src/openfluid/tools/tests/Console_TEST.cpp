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
  @file Console_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_console


#include <boost/test/unit_test.hpp>

#include <openfluid/tools/Console.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_console)
{
  openfluid::tools::Console::saveAttributes();

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::RED);
  std::cout << "red";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::GREEN);
  std::cout << "green";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::BROWN);
  std::cout << "brown";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::BLUE);
  std::cout << "blue";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::MAGENTA);
  std::cout << "magenta";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::CYAN);
  std::cout << "cyan";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::DARKGREY);
  std::cout << "darkgrey";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;


  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::GREY);
  std::cout << "grey";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::LIGHTRED);
  std::cout << "lightred";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::LIGHTGREEN);
  std::cout << "lightgreen";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::YELLOW);
  std::cout << "yellow";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::LIGHTBLUE);
  std::cout << "lightblue";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::LIGHTMAGENTA);
  std::cout << "lightmagenta";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::LIGHTCYAN);
  std::cout << "lightcyan";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setColor(openfluid::tools::Console::Colors::WHITE);
  std::cout << "white";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;


#if defined(OPENFLUID_OS_UNIX)
  std::cout << openfluid::tools::Console::getANSIColorCode(openfluid::tools::Console::Colors::RED) << "streamed red";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;
#endif


  openfluid::tools::Console::setOKColor();
  std::cout << "OK";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setWarningColor();
  std::cout << "warning";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;

  openfluid::tools::Console::setErrorColor();
  std::cout << "error";
  openfluid::tools::Console::resetAttributes();
  std::cout << std::endl;

  std::cout << "===================" << std::endl;
}

