/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/tokenzr.h>
#include <wx/datetime.h>
#include <wx/stdpaths.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>

#include "AppTools.h"

#include <iostream>

// =====================================================================
// =====================================================================


wxString GetExecutablePath()
{
  // Le code de cette fonction est issu du wiki wxWidgets (auteur: Nigel Hathaway)
/*
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
	return path;*/

  wxString Path;

  wxFileName::SplitPath(wxStandardPaths::Get().GetExecutablePath(),NULL,&Path,NULL,NULL);
  return Path;
}


// =====================================================================
// =====================================================================


wxArrayString GetFilesByExt(const wxString DirToExplore, const wxString Ext, bool WithPath, bool ExtIncludeDot)
{


  bool Continue;
  wxArrayString FileList;

  wxString FileRoot = wxT("*.");
  if (ExtIncludeDot) FileRoot = wxT("*");

  if (wxDirExists(DirToExplore))
  {

    wxDir DirManager(DirToExplore);

    if (DirManager.IsOpened())
    {
      // listage des plugins presents

      wxString FoundFile;

      Continue = DirManager.GetFirst(&FoundFile,FileRoot+Ext);
      while (Continue)
      {
        if (WithPath) FileList.Add(DirToExplore+wxFILE_SEP_PATH+FoundFile);
        else FileList.Add(FoundFile);
        Continue = DirManager.GetNext(&FoundFile);
      }
    }
  }

  return FileList;

}


// =====================================================================
// =====================================================================

wxArrayString SplitString(const wxString StrToSplit, const wxString SepString, bool ReturnsEmpty)
{
  // using wxStringTokenizer class

  wxArrayString SplitParts;


  wxStringTokenizerMode TokensMode = wxTOKEN_DEFAULT;
  if (ReturnsEmpty) TokensMode = wxTOKEN_RET_EMPTY_ALL;

  wxStringTokenizer Tokenizer(StrToSplit,SepString,TokensMode);

  while (Tokenizer.HasMoreTokens())
  {
    SplitParts.Add(Tokenizer.GetNextToken());
  }

  return SplitParts;
}


// =====================================================================
// =====================================================================

wxString GenerateSimulationID()
{
  wxString BaseStr = wxT("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

  wxString IDStr = wxT("");

  IDStr = wxDateTime::Now().Format(wxT("%Y%m%d")) + wxT("-");

  srand(time(NULL));


  for (int i=0;i<6;i++)
  {
    IDStr << BaseStr[rand() % 26];
  }



  return IDStr;

}


// =====================================================================
// =====================================================================

wxString FormatExecutionMessage(wxString Message)
{
  wxString Formatted = wxT("");

  wxArrayString Parts;

  Parts = SplitString(Message,wxT(";"));

  Formatted = Parts[2] + wxT(" (message sent by ") + Parts[0];

  if (Parts[1] != wxT("-1"))
  {
    Formatted = Formatted + wxT(", at step ") + Parts[1];
  }

  Formatted = Formatted + wxT(")");

  return Formatted;

}


// =====================================================================
// =====================================================================

bool EmptyDirectoryRecursively(const char* DirPath)
{
  DIR *Directory, *TmpDir;
  struct dirent *Entry;
  char Path[PATH_MAX];


  // check if directory can be open

  Directory = opendir(DirPath);
  if (Directory == NULL)
  {
    return false;
  }

  // empty directory recursively
  while ((Entry = readdir(Directory)) != NULL)
  {
    if (strcmp(Entry->d_name, ".") && strcmp(Entry->d_name, ".."))
    {
      snprintf(Path, (size_t) PATH_MAX, "%s/%s", DirPath, Entry->d_name);
/*      if (Entry->d_type == DT_DIR)
      {
        EmptyDirectoryRecursively(Path);
      }
*/
      if (TmpDir = opendir(Path))
      {
        EmptyDirectoryRecursively(Path);
        closedir(TmpDir);
        rmdir(Path);
      }
      else
      {
        remove(Path);
      }

    }

  }
  closedir(Directory);

  return true;
}

// =====================================================================
// =====================================================================

wxString ReplaceEmptyString(wxString SourceStr, wxString ReplaceStr)
{
  if (SourceStr.Length() == 0) SourceStr = ReplaceStr;
  return SourceStr;
}

// =====================================================================
// =====================================================================


bool IsVectorNamedVariable(wxString Name)
{
  return Name.Right(2) == wxT("[]");
}


// =====================================================================
// =====================================================================

wxString GetVectorNamedVariableName(wxString Name)
{

  if (Name.Right(2) == wxT("[]")) return Name.Mid(0,Name.Length()-2);
  else return Name;

}

// =====================================================================
// =====================================================================


wxString RemoveTrailingSlashes(wxString Str)
{
  while (Str.EndsWith(wxString(wxFILE_SEP_PATH)))
  {
    Str.RemoveLast();
  }

  return Str;

}

// =====================================================================
// =====================================================================




