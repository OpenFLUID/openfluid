/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file main.cpp
  \brief implements main

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <wx/app.h>
#include "OpenFLUID.h"

// =====================================================================
// =====================================================================


int main(int argc, char **argv)
{

  // wxWidgets libraries initialization
  if (!wxInitialize())
  {
    std::cout << std::endl << std::endl << "ERROR: wrong wx libs initialization" << std::endl;
    std::cout << std::endl;
    std::cout.flush();

    return 127;
  }


  int ReturnValue = 0;

  OpenFLUIDApp App;

  try
  {
    App.processOptions(argc,argv);
    App.run();
  }
  catch (openfluid::base::OFException& E)
  {
    ReturnValue = App.stopAppReturn("ERROR: " + std::string(E.what()));
  }
  catch (std::bad_alloc& E)
  {
    ReturnValue = App.stopAppReturn("MEMORY ALLOCATION ERROR: " + std::string(E.what()) + ". Possibly not enough memory available");
  }
  catch (std::exception& E)
  {
    ReturnValue = App.stopAppReturn("SYSTEM ERROR: " + std::string(E.what()));
  }
  catch (...)
  {
    ReturnValue = App.stopAppReturn("UNKNOWN ERROR");
  }

  // wxWidgets libraries finalization
  wxUninitialize();

  return ReturnValue;
}

// =====================================================================
// =====================================================================





