/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __RUNTIMEENV_H__
#define __RUNTIMEENV_H__


#include <wx/wx.h>
#include <wx/datetime.h>
#include "mhydasdk-base.h"

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
    
    bool m_WriteResults;    

    bool m_WriteSimReport;    

    bool m_EnableTrace;
    
    bool m_CheckVarNames;
    
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

    void setInputDir(const wxString InputDir) { m_InputDir = InputDir; };

    wxString getInputDir() const { return m_InputDir; };

    void setOutputDir(const wxString OutputDir) { m_OutputDir = OutputDir; };

    wxString getOutputDir() const { return m_OutputDir; };

    void setDateTimeOutputDir();

    wxString getAppDir() const { return m_AppDir; };

    wxString getInputFullPath(wxString Filename) const { return m_InputDir + wxFILE_SEP_PATH + Filename; };

    wxString getOutputFullPath(wxString Filename) const { return m_OutputDir + wxFILE_SEP_PATH + Filename; };
    
    wxString getPluginFullPath(wxString Filename);

    wxArrayString getPluginsPaths() const { return m_PlugsDirs; };

    wxString getTraceDir() const { return m_TraceDir; };

    wxString getTraceFullPath(wxString Filename) const { return m_TraceDir + wxFILE_SEP_PATH + Filename; };
    
    
    void setTraceDir(const wxString TraceDir) { m_TraceDir = TraceDir; };        
    
    bool isClearOutputDir() const { return m_ClearOutputDir; };
    
    void setClearOutputDir(bool ClearDir) { m_ClearOutputDir = ClearDir; };

    bool isQuietRun() const { return m_QuietRun; };
    
    void setQuietRun(bool Quiet) { m_QuietRun = Quiet; };
    
    bool isWriteResults() const { return m_WriteResults; };
    
    void setWriteResults(bool WriteIt) { m_WriteResults = WriteIt; };

    bool isWriteSimReport() const { return m_WriteSimReport; };
    
    void setWriteSimReport(bool WriteIt) { m_WriteSimReport = WriteIt; };
    
    bool isTraceMode() const { return m_EnableTrace; };
        
    void setTraceMode(bool Trace) { m_EnableTrace = Trace; };

    bool isCheckVarNames() const { return m_CheckVarNames; };
        
    void setCheckVarNames(bool Trace) { m_CheckVarNames = Trace; };
    
    
    mhydasdk::base::FunctionEnvironment createFunctionEnvironment();
    
    
    
};




#endif



