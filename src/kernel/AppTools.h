/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __APPTOOLS_H__
#define __APPTOOLS_H__

#include <string>
#include <vector>
#include <openfluid-core.h>
#include <wx/wx.h>

// =====================================================================
// =====================================================================

// unicode management macros
// inspired from Code::Blocks macros http://www.codeblocks.org/

#ifdef wxUSE_UNICODE
    #define _U(x) wxString(x,wxConvUTF8) // char* to wxString
    #define _UU(x,y) wxString(x,y)       // char* to wxString
    #define _C(x) (x).mb_str(wxConvUTF8) // wxString to char*
    #define _CC(x,y) (x).mb_str(y)       // wxString to char*
    #define _S(x) (std::string(wxString(x).mb_str(wxConvUTF8))) //wxString to std::string
#else
    #define _U(x) (x)          // char* to wxString
    #define _C(x) (x).c_str()  // wxString to char*
    #define _S(x) (std::string((x).c_str()))//wxString to std::string
#endif


// =====================================================================
// =====================================================================



/**
  Get list of files with specified extension contained in the specified dir
  \param[in] DirToExplore the directory to explore
  \param[in] Ext the file extension
  \param[in] WithPath return full path with file name if true, file name only otherwise
*/
std::vector<std::string> GetFilesByExt(const std::string DirToExplore, const std::string Ext, bool WithPath = false, bool ExtIncludeDot = false);

/**
  Splits the passed string into a std::string array, split using the given SepString
  \param[in] StrToSplit the string to split
  \param[in] SepString the string used to split
  \param[in] ReturnsEmpty if true, the empty strings are returned
  \return a vector of strings
*/
std::vector<std::string> SplitString(const std::string StrToSplit, const std::string SepString,  bool ReturnsEmpty = false);

bool EmptyDirectoryRecursively(const std::string DirPath);

std::string ReplaceEmptyString(std::string SourceStr, std::string ReplaceStr);

bool IsVectorNamedVariable(std::string Name);

std::string GetVectorNamedVariableName(std::string Name);

std::string RemoveTrailingSlashes(std::string Str);

void printSTDOUT(std::vector<std::string> Strings, std::string Sep);

openfluid::core::DateTime GenerateDateTimeFromStep(openfluid::core::DateTime DT0, openfluid::core::TimeStep_t TS, unsigned int CurrentStep);

void CopyDirectoryRecursively(const std::string SourcePath, const std::string IntoPath);

std::vector<std::string> GetFileLocationsUsingPATHEnvVar(const std::string Filename);

#endif
