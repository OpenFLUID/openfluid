/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <wx/filename.h>
#include <wx/dir.h>

#include "AppTools.h"

// =====================================================================
// =====================================================================


wxString GetExecutablePath()
{
  // Le code de cette fonction est issu du wiki wxWidgets (auteur: Nigel Hathaway)

  static bool found = false;
  static wxString path;

  if (!found)
  {
#ifdef __WXMSW__

    char buf[512];
    *buf = '\0';
    GetModuleFileName(NULL, buf, 511);
    path = buf;

#elif defined(__WXMAC__)

    ProcessInfoRec processinfo;
    ProcessSerialNumber procno ;
    FSSpec fsSpec;

    procno.highLongOfPSN = NULL ;
    procno.lowLongOfPSN = kCurrentProcess ;
    processinfo.processInfoLength = sizeof(ProcessInfoRec);
    processinfo.processName = NULL;
    processinfo.processAppSpec = &fsSpec;

    GetProcessInformation( &procno , &processinfo ) ;
    path = wxMacFSSpec2MacFilename(&fsSpec);
#else
    wxString argv0 = wxTheApp->argv[0];

    if (wxIsAbsolutePath(argv0))
    {
      path = argv0;
		}
    else
    {
      wxPathList pathlist;
      pathlist.AddEnvList(wxT("PATH"));
      path = pathlist.FindAbsoluteValidPath(argv0);
    }

    wxFileName filename(path);
    filename.Normalize();
    path = filename.GetFullPath();

#endif
    found = true;
  }
	return path;
}


// =====================================================================
// =====================================================================


wxArrayString GetFilesByExt(const wxString DirToExplore, const wxString Ext, bool WithPath)
{


  bool Continue;
  wxArrayString FileList;

  wxDir DirManager(DirToExplore);

  if (DirManager.IsOpened())
  {
    // listage des plugins présents

    wxString FoundFile;

    Continue = DirManager.GetFirst(&FoundFile,wxT("*.")+Ext);
    while (Continue)
    {
      if (WithPath) FileList.Add(DirToExplore+wxFILE_SEP_PATH+FoundFile);
      else FileList.Add(FoundFile);
      Continue = DirManager.GetNext(&FoundFile);
    }
  }

  return FileList;

}

