/**
  \file main.h
  \brief header of main

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/datetime.h>

#include "mhydasdk-base.h"
#include "mhydasdk-core.h"


#include "Engine.h"
#include "PluginManager.h"


// =====================================================================
// =====================================================================


static const wxCmdLineEntryDesc CmdLineDesc[] =
{
  {wxCMD_LINE_SWITCH, _T("a"), _T("auto-outdir"), _T("generate automatic results output directory")},
  {wxCMD_LINE_SWITCH, _T("c"), _T("clean-outdir"), _T("clean results output directory by removing existing files")},  
  {wxCMD_LINE_SWITCH, _T("f"), _T("functions-list"), _T("list available functions (do not run the model)")},
  {wxCMD_LINE_SWITCH, _T("h"), _T("help"), _T("show this help message"),wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
  {wxCMD_LINE_OPTION, _T("i"), _T("inputdir"), _T("set dataset input directory") },
  {wxCMD_LINE_OPTION, _T("o"), _T("outputdir"), _T("set results output directory")}, 
  {wxCMD_LINE_SWITCH, _T("q"), _T("quiet"), _T("quiet display during simulation run")},  
  {wxCMD_LINE_SWITCH, _T("r"), _T("functions-report"), _T("print a report of available functions, with details (do not run the model)")},
  {wxCMD_LINE_SWITCH, _T("s"), _T("no-simreport"), _T("do not generate simulation report")},  
  {wxCMD_LINE_SWITCH, _T("x"), _T("xml-functions-report"), _T("print a report of available functions in xml format, with details (do not run the model)")},  
  {wxCMD_LINE_SWITCH, _T("z"), _T("no-results"), _T("do not write results files")},
  {wxCMD_LINE_NONE}
};


// =====================================================================
// =====================================================================


class MHYDASApp : public wxAppConsole
{
  private:

    bool m_OKToRun;

    RuntimeEnvironment* mp_RunEnv;
    
    mhydasdk::base::ExecutionMessages* mp_ExecMsgs;

    ExtraSimInfos m_ExSI;

    mhydasdk::core::CoreRepository* mp_CoreData;

    Engine *mp_Engine;

    PluginManager *mp_PlugMan;

    wxDateTime m_TotalStartTime;
    wxDateTime m_TotalEndTime;
    wxDateTime m_EffectiveStartTime;
    wxDateTime m_EffectiveEndTime;

    void printlnExecStatus();
    
    void printlnExecMessagesStats();
    
    void printMHYDASInfos();

    void printDataInfos();

    void printPluginsList();

    void printPluginsVarsPropsParamsReport(wxArrayString VarsPropsParams, wxString Suffix, bool IsXMLFormat);

    void printPluginsReport(bool IsXMLFormat);

    bool stopAppReturn();

  public:

    /**
      Initialization. Automatically called
    */
    virtual bool OnInit();

    /**
      Run. Automatically called
    */
    virtual int OnRun();

    /**
      Exit. Automatically called
    */
    virtual int OnExit();

    /**
      Builds the model.
    */
    bool buildModel();

    /**
      Loads input data and output config.
    */
    bool loadData();

    /**
      Checks global consistency
    */
    bool checkConsistency();

    /**
      Runs simulation
    */
    bool runSimulation();

    /**
      Saves results
    */
    bool saveResults();

    /**
      Saves simulation reports
    */
    bool saveSimulationReports();


};



// =====================================================================
// =====================================================================

DECLARE_APP(MHYDASApp)


