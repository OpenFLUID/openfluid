/**
  \file main.h
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include <wx/app.h>
#include <wx/cmdline.h>

#include "sdk-base.h"
#include "sdk-core.h"


#include "Engine.h"
#include "PluginManager.h"


// =====================================================================
// =====================================================================


static const wxCmdLineEntryDesc CmdLineDesc[] =
{
  {wxCMD_LINE_OPTION, _T("i"), _T("inputdir"), _T("set dataset input directory") },
  {wxCMD_LINE_OPTION, _T("o"), _T("outputdir"), _T("set results output directory")},
  {wxCMD_LINE_SWITCH, _T("a"), _T("autooutdir"), _T("automatic results output directory")},
  {wxCMD_LINE_SWITCH, _T("p"), _T("pluginslist"), _T("list available pluggable functions (do not run the model)")},
  {wxCMD_LINE_SWITCH, _T("v"), _T("verbose"), _T("enable verbose display")},
  {wxCMD_LINE_SWITCH, _T("h"), _T("help"), _T("show this help message"),wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
  {wxCMD_LINE_NONE}
};


// =====================================================================
// =====================================================================


class MHYDASApp : public wxAppConsole
{
  private:

    bool m_OKToRun;

    mhydasdk::base::RuntimeEnvironment* mp_RunEnv;

    mhydasdk::core::CoreRepository* mp_CoreData;

    Engine *mp_Engine;

    PluginManager *mp_PlugMan;

    wxDateTime m_EffectiveStartTime;
    wxDateTime m_EffectiveEndTime;

    void printlnExecStatus(bool Status);

    void printMHYDASInfos();

    void printDataInfos();

    void printPluginsList();

    bool stopAppReturn();

  public:
    virtual bool OnInit();
    virtual int OnRun();
    virtual int OnExit();

    bool buildModel();

    bool loadData();

    bool checkConsistency();

    bool runSimulation();

    bool saveResults();

};



// =====================================================================
// =====================================================================



DECLARE_APP(MHYDASApp)
