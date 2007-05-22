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
    wxString m_AppDir;

    wxArrayString m_PlugsDirs;

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

};




#endif



