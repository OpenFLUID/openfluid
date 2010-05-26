/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


#include <openfluid/tools/SwissTools.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>


namespace openfluid { namespace tools {


void TokenizeString(const std::string& StrToTokenize,
                    std::vector<std::string>& Tokens,
                    const std::string& Delimiters = " ")
{
  Tokens.clear();

  std::string::size_type LastPos = StrToTokenize.find_first_not_of(Delimiters, 0);
  std::string::size_type Pos = StrToTokenize.find_first_of(Delimiters, LastPos);

  while (std::string::npos != Pos || std::string::npos != LastPos)
  {
    // Found a token, add it to the vector.
    Tokens.push_back(StrToTokenize.substr(LastPos, Pos - LastPos));
    // Skip delimiters.  Note the "not_of"
    LastPos = StrToTokenize.find_first_not_of(Delimiters, Pos);
    // Find next "non-delimiter"
    Pos = StrToTokenize.find_first_of(Delimiters, LastPos);
  }
}


// =====================================================================
// =====================================================================


bool WildcardMatching(const std::string Pattern, const std::string Str)
{

  // source : http://www.codeproject.com/KB/string/wildcmp.aspx

  const char *cp = NULL;
  const char *mp = NULL;

  const char *StrToCheck = Str.c_str();
  const char *WildStr = Pattern.c_str();

  while ((*StrToCheck) && (*WildStr != '*'))
  {
    if ((*WildStr != *StrToCheck) && (*WildStr != '?'))
    {
      return false;
    }
    WildStr++;
    StrToCheck++;
  }

  while (*StrToCheck)
  {
    if (*WildStr == '*')
    {
      if (!*++WildStr)
      {
        return true;
      }
      mp = WildStr;
      cp = StrToCheck+1;
    }
    else
    {
      if ((*WildStr == *StrToCheck) || (*WildStr == '?'))
      {
        WildStr++;
        StrToCheck++;
      }
      else
      {
        WildStr = mp;
        StrToCheck = cp++;
      }
    }
  }

  while (*WildStr == '*')
  {
    WildStr++;
  }
  return !*WildStr;
}



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


} } // namespaces
