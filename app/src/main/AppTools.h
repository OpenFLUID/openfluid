/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __APPTOOLS_H__
#define __APPTOOLS_H__


#include <wx/wx.h>

wxString GetExecutablePath();

wxArrayString GetFilesByExt(const wxString DirToExplore, const wxString Ext, bool WithPath = false);

wxArrayString SplitString(const wxString StrToSplit, const wxString SepString);




#endif
