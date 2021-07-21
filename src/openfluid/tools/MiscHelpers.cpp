/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file MiscHelpers.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <thread>
#include <chrono>
#include <ctime>
#include <random>
#include <sstream>
#include <regex>
#include <iomanip> 

#include <boost/algorithm/string.hpp>

#include <openfluid/core/DateTime.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/tools/DataHelpers.hpp>


namespace openfluid { namespace tools {


bool matchWithWildcard(const std::string& Pattern, const std::string& Str)
{

  // source : http://www.codeproject.com/KB/string/wildcmp.aspx

  const char* cp = nullptr;
  const char* mp = nullptr;

  const char* StrToCheck = Str.c_str();
  const char* WildStr = Pattern.c_str();

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


std::string replaceEmptyString(std::string SourceStr,
                               const std::string& ReplaceStr)
{
  if (SourceStr.empty())
  {
    SourceStr = ReplaceStr;
  }
  return SourceStr;
}


// =====================================================================
// =====================================================================


int compareVersions(const std::string& VersionA, const std::string& VersionB, bool Strict)
{
  std::string LowCaseA = boost::to_lower_copy(VersionA);
  std::string LowCaseB = boost::to_lower_copy(VersionB);

  if (LowCaseA == LowCaseB)
  {
    return 0;
  }

  std::vector<std::string> SplittedA, SplittedB;

  SplittedA = splitString(LowCaseA,".~",false);

  SplittedB = splitString(LowCaseB,".~",false);

  if ( !(SplittedA.size()==3 || SplittedA.size()==4) || !(SplittedB.size()==3 || SplittedB.size()==4))
  {
    return -2;
  }


  unsigned int MajorA, MinorA, PatchA, MajorB, MinorB, PatchB;
  std::string StatusA(""), StatusB("");

  if (SplittedA.size()==4)
  {
    StatusA = SplittedA[3];
  }
  if (SplittedB.size()==4)
  {
    StatusB = SplittedB[3];
  }

  if (convertString(SplittedA[0],&MajorA) && convertString(SplittedA[1],&MinorA) &&
      convertString(SplittedA[2],&PatchA) &&
      convertString(SplittedB[0],&MajorB) && convertString(SplittedB[1],&MinorB) &&
      convertString(SplittedB[2],&PatchB))
  {
    if (MajorA > MajorB)
    {
      return 1;
    }
    if (MajorA < MajorB)
    {
      return -1;
    }
    if (MajorA == MajorB)
    {
      if (MinorA > MinorB)
      {
        return 1;
      }
      if (MinorA < MinorB)
      {
        return -1;
      }
      if (MinorA == MinorB)
      {
        if (PatchA > PatchB)
        {
          return 1;
        }
        if (PatchA < PatchB)
        {
          return -1;
        }
        if (PatchA == PatchB)
        {
          if (!Strict)
          {
            return 0;
          }
          else
          {

            if (StatusA.empty() && StatusB.empty())
            {
              return 0;
            }

            if (StatusA == StatusB)
            {
              return 0;
            }

            if (StatusA.empty() && !StatusB.empty())
            {
              return 1;
            }
            if (!StatusA.empty() && StatusB.empty())
            {
              return -1;
            }


            if (StatusA > StatusB)
            {
              return 1;
            }
            if (StatusA < StatusB)
            {
              return -1;
            }
          }
        }
      }
    }
  }

  return -2;
}


// =====================================================================
// =====================================================================


void millisleep(const unsigned long MSec)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(MSec));
}


// =====================================================================
// =====================================================================


void microsleep(const unsigned long USec)
{
  std::this_thread::sleep_for(std::chrono::microseconds(USec));
}


// =====================================================================
// =====================================================================


std::string getNowAsString(const std::string& Format)
{
  auto Now = std::chrono::system_clock::now();
  auto RawTime = std::chrono::system_clock::to_time_t(Now);
 
   std::stringstream ss;
  ss << std::put_time(std::localtime(&RawTime),Format.c_str());
  return ss.str();
}


// =====================================================================
// =====================================================================


std::string generatePseudoUniqueIdentifier(const unsigned int Length)
{
  static const std::string PUICharSet("abcdefghijklmnopqrstuvwxyz"
                                      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "0123456789");
  static std::random_device PUIRandomDevice;
  static std::mt19937 PUIRandomEngine(PUIRandomDevice());
  static std::uniform_int_distribution<std::size_t> PUIUniformDist(0u,PUICharSet.size()-1);


  std::string PUI(Length,' ');

  for (unsigned int i = 0; i < Length; i++)
  {
    PUI[i] = PUICharSet[PUIUniformDist(PUIRandomEngine)];
  }

  return PUI;
}


// =====================================================================
// =====================================================================


std::string escapePattern(const std::string& Str)
{
  std::regex SpecialChars { R"([-[\]{}()*+?.,\^$|#\s])" };

  return std::regex_replace(Str,SpecialChars,R"(\$&)");
}


} } // namespaces
