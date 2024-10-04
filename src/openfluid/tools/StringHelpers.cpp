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
  @file StringHelpers.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <regex>
#include <utility>

#include <boost/algorithm/string.hpp>

#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace tools {


std::string toLowerCase(const std::string& Str)
{
  return boost::to_lower_copy(Str);
}


// =====================================================================
// =====================================================================


std::string toUpperCase(const std::string& Str)
{
  return boost::to_upper_copy(Str);
}


// =====================================================================
// =====================================================================


std::string trimRight(const std::string& Str)
{
  return boost::trim_right_copy(Str);
}


// =====================================================================
// =====================================================================


std::string trimLeft(const std::string& Str)
{
  return boost::trim_left_copy(Str);
}


// =====================================================================
// =====================================================================


std::string trim(const std::string& Str)
{
  return boost::trim_copy(Str);
}


// =====================================================================
// =====================================================================


bool startsWith(const std::string& Str,const std::string& SubStr)
{
  return boost::algorithm::starts_with(Str,SubStr);
}


// =====================================================================
// =====================================================================


bool endsWith(const std::string& Str,const std::string& SubStr)
{
  return boost::algorithm::ends_with(Str,SubStr);
}


// =====================================================================
// =====================================================================


bool contains(const std::string& Str,const std::string& SubStr, bool CaseSensitive)
{
  if(CaseSensitive)
  {
    return boost::algorithm::contains(Str,SubStr);
  }
  else
  {
    return boost::algorithm::icontains(Str,SubStr);
  }
}


// =====================================================================
// =====================================================================


std::string replace(const std::string& Str,const std::string& SearchStr, const std::string& ReplaceStr)
{
  return boost::replace_all_copy(Str,SearchStr,ReplaceStr);
}


// =====================================================================
// =====================================================================


std::vector<std::string> split(const std::string& Str, const char Sep, bool KeepEmpty)
{
  return split(Str,std::string(1,Sep),KeepEmpty);
}


// =====================================================================
// =====================================================================


std::vector<std::string> split(const std::string& Str, const std::string& SepChars, bool KeepEmpty)
{
  std::vector<std::string> SplitParts;

  boost::algorithm::token_compress_mode_type TokCompress = boost::token_compress_on;
  if (KeepEmpty)
  {
    TokCompress = boost::token_compress_off;
  }

  boost::split(SplitParts,Str, boost::is_any_of(SepChars),TokCompress);

  return SplitParts;
}


// =====================================================================
// =====================================================================


std::string join(const std::vector<std::string>& Vect, const std::string& Sep)
{
  return boost::algorithm::join(Vect,Sep);
}


} } // namespaces


// =====================================================================
// =====================================================================


std::vector<std::string>& operator<<(std::vector<std::string>& Vect, std::string&& Str)
{
  Vect.emplace_back(std::forward<std::string>(Str));
  return Vect;
}


// =====================================================================
// =====================================================================


std::vector<std::string>& operator<<(std::vector<std::string>& Vect, const std::string& Str)
{
  Vect.push_back(Str);
  return Vect;
}


// =====================================================================
// =====================================================================


std::vector<std::string>& operator<<(std::vector<std::string>& Vect, const std::vector<std::string>& OtherVect)
{
  for (const auto& O : OtherVect)
  {
    Vect.push_back(O);
  }
  return Vect;
}

