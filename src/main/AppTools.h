/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __APPTOOLS_H__
#define __APPTOOLS_H__


#include <wx/wx.h>

/**
  Get current program executable path
  \return a wxString containing the path, empty if indetermined

*/
wxString GetExecutablePath();

/**
  Get list of files with specified extension contained in the specified dir
  \param[in] DirToExplore the directory to explore
  \param[in] Ext the file extension
  \param[in] WithPath return full path with file name if true, file name only otherwise
*/
wxArrayString GetFilesByExt(const wxString DirToExplore, const wxString Ext, bool WithPath = false);

/**
  Splits the passed string int a wxStringArray, splitted by the passed SepString
  \param[in] StrToSplit the string to split
  \param[in] SepString the string used to split
  \param[in] ReturnsEmpty if true, the empty strings are returned
  \return a wxArraySTring
*/
wxArrayString SplitString(const wxString StrToSplit, const wxString SepString,  bool ReturnsEmpty = false);

wxString GenerateSimulationID();

wxString FormatExecutionMessage(wxString Message);

bool EmptyDirectoryRecursively(const char* DirPath);

wxString ReplaceEmptyString(wxString SourceStr, wxString ReplaceStr);

bool IsVectorNamedVariable(wxString Name);

wxString GetVectorNamedVariableName(wxString Name);

wxString RemoveTrailingSlashes(wxString Str);



#endif
