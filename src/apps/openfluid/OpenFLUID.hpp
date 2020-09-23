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
  @file OpenFLUID.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CMDLINEAPP_OPENFLUID_HPP__
#define __OPENFLUID_CMDLINEAPP_OPENFLUID_HPP__


#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>


namespace openfluid {
namespace base {
class RuntimeEnvironment;
}
namespace machine {
class Engine;
}
}


// =====================================================================
// =====================================================================


class OpenFLUIDApp
{
  private:

    enum class RunType { None, Simulation, InfoRequest, Buddy };

    RunType m_RunType;

    std::pair<std::string,std::string> m_BuddyToRun;

    openfluid::base::RuntimeEnvironment* mp_RunEnv;

    openfluid::machine::SimulationBlob m_SimBlob;

    std::unique_ptr<openfluid::machine::Engine> mp_Engine;


    void printlnExecMessagesStats();

    void printOpenFLUIDInfos();

    void printPaths(bool ShowTemp = true);

    void printEnvInfos();

    /**
      Runs simulation
    */
    void runSimulation();

    /**
      Runs buddy
    */
    void runBuddy();


  public:

    OpenFLUIDApp();

    ~OpenFLUIDApp();

    int stopAppReturn(const std::string& ErrorType, const std::string& Msg);

    void processOptions(int ArgC, char **ArgV);

    void run();

};


#endif /* __OPENFLUID_CMDLINEAPP_OPENFLUID_HPP__ */
