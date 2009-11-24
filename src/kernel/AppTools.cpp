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

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "AppTools.h"
#include <openfluid-tools.h>
#include <iostream>
#include <sstream>

// =====================================================================
// =====================================================================


std::vector<std::string> GetFilesByExt(const std::string DirToExplore, const std::string Ext, bool WithPath, bool ExtIncludeDot)
{

  std::vector<std::string> FileList;


  std::string FileExt = Ext;
  if (!ExtIncludeDot) FileExt = "."+Ext;

  boost::filesystem::path PathToExplore(DirToExplore);

  if (boost::filesystem::is_directory(PathToExplore))
  {

    boost::filesystem::directory_iterator it;

    std::string FoundFile;

    for (it = boost::filesystem::directory_iterator(PathToExplore);it != boost::filesystem::directory_iterator(); ++it)
    {

      // lists files with specified extension

      if (boost::filesystem::is_regular(it->status()) && boost::ends_with(it->path().string(),FileExt))
      {
        if (WithPath) FileList.push_back(it->string());
        else FileList.push_back(it->path().leaf());
      }
    }
  }

  return FileList;

}


// =====================================================================
// =====================================================================

std::vector<std::string> SplitString(const std::string StrToSplit, const std::string Separators, bool ReturnsEmpty)
{
  std::vector<std::string> SplitParts;

  boost::algorithm::token_compress_mode_type TokCompress = boost::token_compress_on;
  if (ReturnsEmpty) TokCompress = boost::token_compress_off;

  boost::split(SplitParts, StrToSplit, boost::is_any_of(Separators));

  return SplitParts;
}



// =====================================================================
// =====================================================================

bool EmptyDirectoryRecursively(const std::string DirPath)
{

  boost::filesystem::path PathToEmpty(DirPath);

  boost::filesystem::remove_all(PathToEmpty);

  return boost::filesystem::create_directory(PathToEmpty);

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
  return boost::ends_with(Name,"[]");
}


// =====================================================================
// =====================================================================

std::string GetVectorNamedVariableName(std::string Name)
{
  boost::erase_last(Name,"[]");
  return Name;
}

// =====================================================================
// =====================================================================


std::string RemoveTrailingSlashes(std::string Str)
{
  std::string PathSep = "/";

#ifdef BOOST_WINDOWS_PATH
  PathSep = "\\";
#endif


  while (boost::ends_with(Str,PathSep))
  {
    boost::erase_last(Str,PathSep);
  }

  return Str;
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

// =====================================================================
// =====================================================================


void CopyDirectoryRecursively(const std::string SourceDir, const std::string IntoDir, const bool DontCopyDotDirs)
{

  boost::filesystem::path SourceDirPath(SourceDir);
  boost::filesystem::path IntoDirPath(IntoDir);
  boost::filesystem::path DestDirPath(IntoDir+"/"+SourceDirPath.leaf());


  if (boost::filesystem::is_directory(DestDirPath)) boost::filesystem::remove_all(DestDirPath);

  boost::filesystem::create_directory(DestDirPath);

  boost::filesystem::directory_iterator it;

  for (it = boost::filesystem::directory_iterator(SourceDirPath);it != boost::filesystem::directory_iterator(); ++it)
  {

    if (boost::filesystem::is_regular(it->status()))
    {
      boost::filesystem::copy_file(it->path(),boost::filesystem::path(DestDirPath.string()+"/"+it->path().leaf()));
    }

    if (boost::filesystem::is_directory(it->status()))
    {
      if (!DontCopyDotDirs || (DontCopyDotDirs && !boost::starts_with(it->path().leaf(),".")))
      {
        CopyDirectoryRecursively(it->path().string(),DestDirPath.string(), DontCopyDotDirs);
      }
    }
  }
}

// =====================================================================
// =====================================================================

std::vector<std::string> GetFileLocationsUsingPATHEnvVar(const std::string Filename)
{

  std::vector<std::string> FileLocations;
  char *PATHEnvVar;

  PATHEnvVar = std::getenv("PATH");

  if (PATHEnvVar != NULL)
  {
    std::vector<std::string> PATHItems;

#if defined __unix__ || defined __APPLE__
    PATHItems = SplitString(std::string(PATHEnvVar), ":", false);
#endif

#ifdef WIN32
    PATHItems = SplitString(std::string(PATHEnvVar), ";", false);
#endif

    for (unsigned int i=0;i<PATHItems.size();i++)
    {
      boost::filesystem::path PathToTest(PATHItems[i]+"/"+Filename);
      if (boost::filesystem::exists(PathToTest)) FileLocations.push_back(PathToTest.string());
    }

  }

  return FileLocations;

}
