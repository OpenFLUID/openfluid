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
  @file DefaultDocalyzeListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CMDLINEAPP_DEFAULTDOCALYZELISTENER_HPP__
#define __OPENFLUID_CMDLINEAPP_DEFAULTDOCALYZELISTENER_HPP__


#include <iostream>
#include <iomanip>

#include <openfluid/waresdev/WareSrcDocalyzerListener.hpp>
#include <openfluid/tools/Console.hpp>

#include "DefaultWareSrcOpsListener.hpp"


class DefaultDocalyzeListener : public openfluid::waresdev::WareSrcDocalyzerListener, public DefaultWareSrcOpsListener
{

  public:

    CPPCLASS_DEFAULT_FIVE(DefaultDocalyzeListener)


    void onDetectInputStart()
    { 
      displayStartMsg("Detecting input");
    }

    void onDetectInputEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
    }

    void onPrepareStart()
    { 
      displayStartMsg("Preparing for build");
    }

    void onPrepareEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
    }

    void onBuildStart()
    { 
      displayStartMsg("Building documentation");
    }

    void onBuildEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
    }

    void onReleaseOutputStart()
    { 
      displayStartMsg("Releasing produced documentation");
    }

    void onReleaseOutputEnd(openfluid::base::Listener::Status Status)
    { 
      displayStatus(Status);
    }

};


#endif /* __OPENFLUID_CMDLINEAPP_DEFAULTDOCALYZELISTENER_HPP__ */
