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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <QCoreApplication>

#include <openfluid/base/Init.hpp>

#include "OpenFLUID.hpp"


int main(int argc, char **argv)
{
  int ReturnValue = 0;

  INIT_OPENFLUID_APPLICATION(argc,argv);

  OpenFLUIDApp App;

  try
  {
    App.processOptions(argc,argv);
    App.run();
  }
  catch (openfluid::base::Exception& E)
  {
    ReturnValue = App.stopAppReturn("OpenFLUID ERROR",
                                    E.getMessage() + " [" + E.getContext().toString() + "]");
  }
  catch (std::bad_alloc& E)
  {
    ReturnValue = App.stopAppReturn("MEMORY ALLOCATION ERROR",
                                    std::string(E.what()) +". Possibly not enough memory available");
  }
  catch (std::exception& E)
  {
    ReturnValue = App.stopAppReturn("SYSTEM ERROR",std::string(E.what()));
  }
  catch (...)
  {
    ReturnValue = App.stopAppReturn("UNKNOWN ERROR","no information");
  }

  return ReturnValue;
}


