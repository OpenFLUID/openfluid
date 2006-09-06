/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <wx/wx.h>

wxString GetExecutablePath();

wxArrayString GetFilesByExt(const wxString DirToExplore, const wxString Ext, bool WithPath = false);
