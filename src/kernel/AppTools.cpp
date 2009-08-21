/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


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
#include <openfluid-tools.h>
#include <iostream>
#include <sstream>

// =====================================================================
// =====================================================================


std::string GetExecutablePath()
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
  return _S(Path);
}


// =====================================================================
// =====================================================================


std::vector<std::string> GetFilesByExt(const std::string DirToExplore, const std::string Ext, bool WithPath, bool ExtIncludeDot)
{


  bool Continue;
  std::vector<std::string> FileList;

  wxString FileRoot = wxT("*.");
  if (ExtIncludeDot) FileRoot = wxT("*");

  if (wxDirExists(_U(DirToExplore.c_str())))
  {

    wxDir DirManager(_U(DirToExplore.c_str()));

    if (DirManager.IsOpened())
    {
      // listage des plugins presents

      wxString FoundFile;

      Continue = DirManager.GetFirst(&FoundFile,FileRoot+_U(Ext.c_str()));
      while (Continue)
      {
        if (WithPath) FileList.push_back(DirToExplore+_S(wxFILE_SEP_PATH)+_S(FoundFile));
        else FileList.push_back(_S(FoundFile));
        Continue = DirManager.GetNext(&FoundFile);
      }
    }
  }

  return FileList;

}


// =====================================================================
// =====================================================================

std::vector<std::string> SplitString(const std::string StrToSplit, const std::string SepString, bool ReturnsEmpty)
{
  // using wxStringTokenizer class

  std::vector<std::string> SplitParts;


  wxStringTokenizerMode TokensMode = wxTOKEN_DEFAULT;
  if (ReturnsEmpty) TokensMode = wxTOKEN_RET_EMPTY_ALL;

  wxStringTokenizer Tokenizer(_U(StrToSplit.c_str()),_U(SepString.c_str()),TokensMode);

  while (Tokenizer.HasMoreTokens())
  {
    SplitParts.push_back(_S(Tokenizer.GetNextToken()));
  }

  return SplitParts;
}


// =====================================================================
// =====================================================================

std::string GenerateSimulationID()
{

  std::string BaseStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  std::string IDStr = "";

  IDStr = _S(wxDateTime::Now().Format(wxT("%Y%m%d"))) + "-";

  srand(time(NULL));


  for (int i=0;i<6;i++)
  {
    IDStr = IDStr + BaseStr[rand() % 26];
  }


  return IDStr;

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
      if ((TmpDir = opendir(Path)))
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

std::string ReplaceEmptyString(std::string SourceStr, std::string ReplaceStr)
{
  if (SourceStr.length() == 0) SourceStr = ReplaceStr;
  return SourceStr;
}

// =====================================================================
// =====================================================================


bool IsVectorNamedVariable(std::string Name)
{
  return _U(Name.c_str()).Right(2) == wxT("[]");
}


// =====================================================================
// =====================================================================

std::string GetVectorNamedVariableName(std::string Name)
{
  wxString NameWX = _U(Name.c_str());

  if (NameWX.Right(2) == wxT("[]")) return _S(NameWX.Mid(0,NameWX.Length()-2));
  else return _S(NameWX);

}

// =====================================================================
// =====================================================================


std::string RemoveTrailingSlashes(std::string Str)
{
  wxString StrWX = _U(Str.c_str());

  while (StrWX.EndsWith(wxString(wxFILE_SEP_PATH)))
  {
    StrWX.RemoveLast();
  }

  return _S(StrWX);

}

// =====================================================================
// =====================================================================

void printSTDOUT(std::vector<std::string> Strings, std::string Sep)
{
  for (unsigned int i=0; i< Strings.size(); i++) std::cout << Strings[i] << Sep;
  std::cout << std::endl;
}

// =====================================================================
// =====================================================================


openfluid::core::DateTime GenerateDateTimeFromStep(openfluid::core::DateTime DT0, openfluid::core::TimeStep_t TS, unsigned int CurrentStep)
{
  DT0.addSeconds(TS*CurrentStep);
  return DT0;
}




