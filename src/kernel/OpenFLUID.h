/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OpenFLUID.h
  \brief header of OpenFLUID application

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/



#include <boost/date_time/posix_time/posix_time.hpp>

#include "openfluid-base.h"
#include "openfluid-core.h"
#include "Engine.h"
#include "PluginManager.h"
#include "RuntimeEnv.h"
#include "config.h"

#include "buddies/OFBuddy.h"


// =====================================================================
// =====================================================================


class OpenFLUIDApp
{
  private:

    enum RunType { None, Simulation, InfoRequest, Buddy };

    RunType m_RunType;

    std::pair<std::string,std::string> m_BuddyToRun;

    RuntimeEnvironment* m_RunEnv;
    openfluid::base::ExecutionMessages m_ExecMsgs;
    Engine* mp_Engine;

    boost::posix_time::ptime m_FullStartTime;
    boost::posix_time::ptime m_FullEndTime;
    boost::posix_time::ptime m_EffectiveStartTime;
    boost::posix_time::ptime m_EffectiveEndTime;


    void printlnExecStatus();

    void printlnExecMessagesStats();

    void printOpenFLUIDInfos();

    void printPaths(bool ShowTemp = true);

    void printEnvInfos();

    void printPluginsList();

    void printPluginsHandledDataReport(openfluid::base::SignatureHandledData HandledData, std::string Suffix, bool IsXMLFormat);

    void printPluginsHandledDataItemReport(openfluid::base::SignatureHandledDataItem HandledItem, std::string Suffix, std::string Type, bool IsXMLFormat);

    void printPluginsReport(bool IsXMLFormat, const std::string Pattern);

    /**
      Runs simulation
    */
    void runSimulation();



  public:

    OpenFLUIDApp();

    ~OpenFLUIDApp();

    int stopAppReturn(std::string Msg);

    void processOptions(int ArgC, char **ArgV);

    void run();

};





