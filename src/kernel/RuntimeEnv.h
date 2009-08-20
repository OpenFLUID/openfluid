/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __RUNTIMEENV_H__
#define __RUNTIMEENV_H__


#include <wx/wx.h>
#include <wx/datetime.h>
#include "openfluid-tools.h"
#include "openfluid-base.h"

#include "AppTools.h"


// =====================================================================
// =====================================================================


/**
   Extra simulation info for post-simulation reporting
 */
struct ExtraSimInfos
{
  wxDateTime StartTime;
  wxTimeSpan RunTime;
  std::string SimID;
};


// =====================================================================
// =====================================================================




/**

*/
class RuntimeEnvironment
{
  private:
    std::string m_OutputDir;
    std::string m_InputDir;
    std::string m_TraceDir;
    std::string m_AppDir;
    std::string m_UserDataDir;

    std::vector<std::string> m_PlugsDirs;

    bool m_ClearOutputDir;

    bool m_QuietRun;

    bool m_VerboseRun;

    bool m_WriteResults;

    bool m_WriteSimReport;

    bool m_EnableTrace;

    bool m_CheckVarNames;

    bool m_ProgressiveOutput;

    unsigned int m_ProgOutPacket;

    unsigned int m_ProgOutKeep;

    openfluid::base::FunctionEnvironment* mp_FuncEnv;


  public:

    /**
      Default constructor
    */
    RuntimeEnvironment();

    /**
      Constructor
    */
    RuntimeEnvironment(std::string AppDir);

    /**
      Destructor
    */
    ~RuntimeEnvironment();

    void setInputDir(const std::string InputDir) { m_InputDir = InputDir; mp_FuncEnv->setValue("dir.input",m_InputDir); };

    std::string getInputDir() const { return m_InputDir; };

    void setOutputDir(const std::string OutputDir) { m_OutputDir = OutputDir; mp_FuncEnv->setValue("dir.output",m_OutputDir); };

    std::string getOutputDir() const { return m_OutputDir; };

    void setDateTimeOutputDir();

    std::string getAppDir() const { return m_AppDir; };

    std::string getInputFullPath(std::string Filename) const { return m_InputDir + _S(wxFILE_SEP_PATH) + Filename; };

    std::string getOutputFullPath(std::string Filename) const { return m_OutputDir + _S(wxFILE_SEP_PATH) + Filename; };

    std::string getPluginFullPath(std::string Filename);

    void addExtraPluginsPaths(std::string ColonSeparatedPaths);

    std::vector<std::string> getPluginsPaths() const { return m_PlugsDirs; };

    std::string getTraceDir() const { return m_TraceDir; };

    std::string getTraceFullPath(std::string Filename) const { return (m_TraceDir + _S(wxFILE_SEP_PATH) + Filename); };

    void setTraceDir(const std::string TraceDir) { m_TraceDir = TraceDir; mp_FuncEnv->setValue("dir.trace",m_TraceDir); };

    bool isClearOutputDir() const { return m_ClearOutputDir; };

    void setClearOutputDir(bool ClearDir) { m_ClearOutputDir = ClearDir; mp_FuncEnv->setValue("mode.cleanoutput",m_ClearOutputDir); };

    bool isQuietRun() const { return m_QuietRun; };

    void setQuietRun(bool Quiet) { m_QuietRun = Quiet; m_VerboseRun = !Quiet; mp_FuncEnv->setValue("mode.quiet",m_QuietRun); mp_FuncEnv->setValue("mode.verbose",m_VerboseRun); };

    bool isVerboseRun() const { return m_VerboseRun; };

    void setVerboseRun(bool Verbose) { m_VerboseRun = Verbose; m_QuietRun = !Verbose; mp_FuncEnv->setValue("mode.quiet",m_QuietRun); mp_FuncEnv->setValue("mode.verbose",m_VerboseRun); };

    bool isWriteResults() const { return m_WriteResults; };

    void setWriteResults(bool WriteIt) { m_WriteResults = WriteIt; mp_FuncEnv->setValue("mode.saveresults",m_WriteResults); };

    bool isWriteSimReport() const { return m_WriteSimReport; };

    void setWriteSimReport(bool WriteIt) { m_WriteSimReport = WriteIt; mp_FuncEnv->setValue("mode.writereport",m_WriteSimReport); };

    bool isTraceMode() const { return m_EnableTrace; };

    void setTraceMode(bool Trace) { m_EnableTrace = Trace; mp_FuncEnv->setValue("mode.trace",m_EnableTrace); };

    bool isCheckVarNames() const { return m_CheckVarNames; };

    void setCheckVarNames(bool Check) { m_CheckVarNames = Check; mp_FuncEnv->setValue("mode.checkvarnames",m_CheckVarNames); };

    openfluid::base::FunctionEnvironment* getFunctionEnvironment() const { return mp_FuncEnv; };

    bool isProgressiveOutput() const { return m_ProgressiveOutput; };

    void setProgressiveOutputKeep(unsigned int Keep)  { m_ProgressiveOutput = true; m_ProgOutKeep = Keep; };

    void setProgressiveOutputPacket(unsigned int Packet)  { m_ProgressiveOutput = true; m_ProgOutPacket = Packet; };

    unsigned int getProgressiveOutputKeep() const { return m_ProgOutKeep; };

    unsigned int getProgressiveOutputPacket() const  { return m_ProgOutPacket; };

};




#endif



