/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __RUNTIMEENV_H__
#define __RUNTIMEENV_H__



#include "openfluid-tools.h"
#include "openfluid-base.h"

#include "AppTools.h"

#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


// =====================================================================
// =====================================================================


/**

*/
class RuntimeEnvironment
{
  private:
    std::string m_OutputDir;
    std::string m_InputDir;
    std::string m_UserDataDir;
    std::string m_TempDir;

    std::vector<std::string> m_PlugsDirs;

    std::string m_UserID;

    std::string m_HostName;

    bool m_ClearOutputDir;

    bool m_QuietRun;

    bool m_VerboseRun;

    bool m_WriteResults;

    bool m_WriteSimReport;

    bool m_CheckVarNames;

    bool m_ProgressiveOutput;

    unsigned int m_ProgOutPacket;

    unsigned int m_ProgOutKeep;

    openfluid::base::FunctionEnvironment* mp_FuncEnv;

    boost::posix_time::ptime m_IgnitionDateTime;

    std::string m_SimulationID;

    boost::posix_time::time_duration m_EffectiveSimulationDuration;

  public:

    /**
      Default constructor
    */
    RuntimeEnvironment();

    /**
      Destructor
    */
    ~RuntimeEnvironment();

    void setInputDir(const std::string InputDir) { m_InputDir = InputDir; mp_FuncEnv->setValue("dir.input",m_InputDir); };

    std::string getInputDir() const { return m_InputDir; };

    void setOutputDir(const std::string OutputDir) { m_OutputDir = OutputDir; mp_FuncEnv->setValue("dir.output",m_OutputDir); };

    std::string getOutputDir() const { return m_OutputDir; };

    void setDateTimeOutputDir();

    std::string getTempDir() const { return m_TempDir; mp_FuncEnv->setValue("dir.temp",m_OutputDir); };

    std::string getInputFullPath(std::string Filename) const { return boost::filesystem::path(m_InputDir + "/" + Filename).string(); };

    std::string getOutputFullPath(std::string Filename) const { return boost::filesystem::path(m_OutputDir + "/" + Filename).string(); };

    std::string getPluginFullPath(std::string Filename);

    void addExtraPluginsPaths(std::string SemicolonSeparatedPaths);

    std::vector<std::string> getPluginsPaths() const { return m_PlugsDirs; };

    bool isClearOutputDir() const { return m_ClearOutputDir; };

    void setClearOutputDir(bool ClearDir) { m_ClearOutputDir = ClearDir; mp_FuncEnv->setValue("mode.clearoutputdir",m_ClearOutputDir); };

    bool isQuietRun() const { return m_QuietRun; };

    void setQuietRun(bool Quiet);

    bool isVerboseRun() const { return m_VerboseRun; };

    void setVerboseRun(bool Verbose);

    bool isWriteResults() const { return m_WriteResults; };

    void setWriteResults(bool WriteIt) { m_WriteResults = WriteIt; mp_FuncEnv->setValue("mode.saveresults",m_WriteResults); };

    bool isWriteSimReport() const { return m_WriteSimReport; };

    void setWriteSimReport(bool WriteIt) { m_WriteSimReport = WriteIt; mp_FuncEnv->setValue("mode.writereport",m_WriteSimReport); };

    bool isCheckVarNames() const { return m_CheckVarNames; };

    void setCheckVarNames(bool Check) { m_CheckVarNames = Check; mp_FuncEnv->setValue("mode.checkvarnames",m_CheckVarNames); };

    openfluid::base::FunctionEnvironment* getFunctionEnvironment() const { return mp_FuncEnv; };

    bool isProgressiveOutput() const { return m_ProgressiveOutput; };

    void setProgressiveOutputKeep(unsigned int Keep)  { m_ProgressiveOutput = true; m_ProgOutKeep = Keep; };

    void setProgressiveOutputPacket(unsigned int Packet)  { m_ProgressiveOutput = true; m_ProgOutPacket = Packet; };

    unsigned int getProgressiveOutputKeep() const { return m_ProgOutKeep; };

    unsigned int getProgressiveOutputPacket() const  { return m_ProgOutPacket; };

    boost::posix_time::ptime getIgnitionDateTime() const { return m_IgnitionDateTime; };

    std::string getSimulationID() const {return m_SimulationID; };

    void setSimulationID(const std::string SimID) { m_SimulationID = SimID; };

    boost::posix_time::time_duration getEffectiveSimulationDuration() const { return m_EffectiveSimulationDuration; };

    void setEffectiveSimulationDuration(const boost::posix_time::time_duration& TimeDuration) { m_EffectiveSimulationDuration = TimeDuration; };

    std::string getHostName() const {return m_HostName; };

    std::string getUserID() const {return m_UserID; };
};




#endif



