/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __RUNTIMEENV_H__
#define __RUNTIMEENV_H__


#include <wx/wx.h>
#include <wx/datetime.h>
#include "openfluid-base.h"

// =====================================================================
// =====================================================================


/**
   Extra simulation info for post-simulation reporting
 */
struct ExtraSimInfos
{
  wxDateTime StartTime;
  wxTimeSpan RunTime;
  wxString SimID;
};


// =====================================================================
// =====================================================================




/**

*/
class RuntimeEnvironment
{
  private:
    wxString m_OutputDir;
    wxString m_InputDir;
    wxString m_TraceDir;
    wxString m_AppDir;
    wxString m_UserDataDir;

    wxArrayString m_PlugsDirs;

    bool m_ClearOutputDir;

    bool m_QuietRun;

    bool m_VerboseRun;

    bool m_WriteResults;

    bool m_WriteSimReport;

    bool m_EnableTrace;

    bool m_CheckVarNames;

    openfluid::base::FunctionEnvironment* mp_FuncEnv;


  public:

    /**
      Default constructor
    */
    RuntimeEnvironment();

    /**
      Constructor
    */
    RuntimeEnvironment(wxString AppDir);

    /**
      Destructor
    */
    ~RuntimeEnvironment();

    void setInputDir(const wxString InputDir) { m_InputDir = InputDir; mp_FuncEnv->setValue(wxT("dir.input"),m_InputDir); };

    wxString getInputDir() const { return m_InputDir; };

    void setOutputDir(const wxString OutputDir) { m_OutputDir = OutputDir; mp_FuncEnv->setValue(wxT("dir.output"),m_OutputDir); };

    wxString getOutputDir() const { return m_OutputDir; };

    void setDateTimeOutputDir();

    wxString getAppDir() const { return m_AppDir; };

    wxString getInputFullPath(wxString Filename) const { return m_InputDir + wxFILE_SEP_PATH + Filename; };

    wxString getOutputFullPath(wxString Filename) const { return m_OutputDir + wxFILE_SEP_PATH + Filename; };

    wxString getPluginFullPath(wxString Filename);

    wxArrayString getPluginsPaths() const { return m_PlugsDirs; };

    wxString getTraceDir() const { return m_TraceDir; };

    wxString getTraceFullPath(wxString Filename) const { return m_TraceDir + wxFILE_SEP_PATH + Filename; };

    void setTraceDir(const wxString TraceDir) { m_TraceDir = TraceDir; mp_FuncEnv->setValue(wxT("dir.trace"),m_TraceDir); };

    bool isClearOutputDir() const { return m_ClearOutputDir; };

    void setClearOutputDir(bool ClearDir) { m_ClearOutputDir = ClearDir; mp_FuncEnv->setValue(wxT("mode.cleanoutput"),m_ClearOutputDir); };

    bool isQuietRun() const { return m_QuietRun; };

    void setQuietRun(bool Quiet) { m_QuietRun = Quiet; m_VerboseRun = !Quiet; mp_FuncEnv->setValue(wxT("mode.quiet"),m_QuietRun); mp_FuncEnv->setValue(wxT("mode.verbose"),m_VerboseRun); };

    bool isVerboseRun() const { return m_VerboseRun; };

    void setVerboseRun(bool Verbose) { m_VerboseRun = Verbose; m_QuietRun = !Verbose; mp_FuncEnv->setValue(wxT("mode.quiet"),m_QuietRun); mp_FuncEnv->setValue(wxT("mode.verbose"),m_VerboseRun); };

    bool isWriteResults() const { return m_WriteResults; };

    void setWriteResults(bool WriteIt) { m_WriteResults = WriteIt; mp_FuncEnv->setValue(wxT("mode.saveresults"),m_WriteResults); };

    bool isWriteSimReport() const { return m_WriteSimReport; };

    void setWriteSimReport(bool WriteIt) { m_WriteSimReport = WriteIt; mp_FuncEnv->setValue(wxT("mode.writereport"),m_WriteSimReport); };

    bool isTraceMode() const { return m_EnableTrace; };

    void setTraceMode(bool Trace) { m_EnableTrace = Trace; mp_FuncEnv->setValue(wxT("mode.trace"),m_EnableTrace); };

    bool isCheckVarNames() const { return m_CheckVarNames; };

    void setCheckVarNames(bool Check) { m_CheckVarNames = Check; mp_FuncEnv->setValue(wxT("mode.checkvarnames"),m_CheckVarNames); };

    openfluid::base::FunctionEnvironment* getFunctionEnvironment() const { return mp_FuncEnv; };



};




#endif



