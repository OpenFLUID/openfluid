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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/



#include <boost/date_time/posix_time/posix_time.hpp>

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

    enum RunType { None, Simulation, InfoRequest, Buddy };

    RunType m_RunType;

    std::pair<std::string,std::string> m_BuddyToRun;

    openfluid::base::RuntimeEnvironment* m_RunEnv;
    openfluid::machine::SimulationBlob m_SimBlob;
    openfluid::machine::Engine* mp_Engine;

    boost::posix_time::ptime m_FullStartTime;
    boost::posix_time::ptime m_FullEndTime;
    boost::posix_time::ptime m_EffectiveStartTime;
    boost::posix_time::ptime m_EffectiveEndTime;


    void printlnExecMessagesStats();

    void printOpenFLUIDInfos();

    void printPaths(bool ShowTemp = true);

    void printEnvInfos();

    void printSimulatorsList();

    void printMonitoring();

    void printSimulatorsHandledUnitsGraphReport(openfluid::ware::SignatureHandledUnitsGraph HandledUnitsGraph, std::string Suffix);

    void printSimulatorsHandledDataReport(openfluid::ware::SignatureHandledData HandledData, std::string Suffix);

    void printSimulatorsHandledDataItemReport(openfluid::ware::SignatureHandledDataItem HandledItem, std::string Suffix, std::string Type);

    void printSimulatorsTimeSchedulingReport(openfluid::ware::SignatureTimeScheduling TScheduling);

    void printWareInfosReport(const openfluid::ware::WareSignature* Signature, const std::string& Filename);

    void printSimulatorsReport(const std::string Pattern);

    void printObserversReport(const std::string Pattern);

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

    int stopAppReturn(std::string Msg);

    void processOptions(int ArgC, char **ArgV);

    void run();

};





