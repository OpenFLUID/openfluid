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
  @file DataHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>

  @deprecated Since version 2.2.0. Include openfluid/tools/StringHelpers.hpp file instead
 */


#ifndef __OPENFLUID_TOOLS_DATAHELPERS_HPP__
#define __OPENFLUID_TOOLS_DATAHELPERS_HPP__


#include <string>
#include <list>
#include <sstream>
#include <vector>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


/**
  Converts a string to another type
  @param[in] StrToConvert the string to convert
  @param[out] Converted the result of the conversion
  @return true if the conversion is correct
  @deprecated Since version 2.2.0. Use openfluid::tools::toNumeric() instead
*/
template<typename T>
[[deprecated]] inline bool convertString(const std::string& StrToConvert, T* Converted)
{
  std::istringstream iss(StrToConvert);
  char c;
  return ((iss >> (*Converted)) && !iss.get(c));
}


// =====================================================================
// =====================================================================


/**
  Converts a string to another type
  @param[in] StrToConvert the string to convert
  @return true the string converted
  @throws openfluid::base::FrameworkException Error in string conversion
  @deprecated Since version 2.2.0. Use openfluid::tools::toNumeric() instead
*/
template<typename T>
[[deprecated]] inline T convertString(const std::string& StrToConvert)
{
  T Converted;

  if (convertString(StrToConvert,&Converted))
  {
    return Converted;
  }

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error in string conversion");
}


// =====================================================================
// =====================================================================


/**
  Converts a value to a string
  @param[in] ValueToConvert the value to convert
  @param[out] StrConverted the result of the conversion
  @return true if the conversion is correct
  @deprecated Since version 2.2.0. Use std::to_string() instead
*/
template<typename T>
[[deprecated]] inline bool convertValue(const T& ValueToConvert, std::string* StrConverted)
{
  std::ostringstream oss;
  bool IsOK = !(oss << ValueToConvert).fail();

  if (IsOK)
  {
    (*StrConverted) = oss.str();
  }

  return IsOK;
}


// =====================================================================
// =====================================================================


/**
  Converts a value to a string
  @param[in] ValueToConvert the value to convert
  @return the converted value
  @throws openfluid::base::FrameworkException Error in value conversion
  @deprecated Since version 2.2.0. Use std::to_string() instead
*/
template<typename T>
[[deprecated]] inline std::string convertValue(const T& ValueToConvert)
{
  std::string Converted;

  if (convertValue(ValueToConvert,&Converted))
  {
    return Converted;
  }

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error in value conversion");
}


// =====================================================================
// =====================================================================


/**
  Function for tokenizing string into a vector of tokens
  @param[in] StrToTokenize the string to tokenize
  @param[out] Tokens the resulting tokens
  @param[in] Delimiters the string delimiter
  @deprecated Since version 2.2.0. Use openfluid::tools::split() instead
*/
[[deprecated]]void OPENFLUID_API tokenizeString(const std::string& StrToTokenize,
                                                std::vector<std::string>& Tokens,
                                                const std::string& Delimiters);


/**
  Splits the given string into a std::string array, split using the given SepString
  @param[in] StrToSplit the string to split
  @param[in] Separators the string of separators used to split the string
  @param[in] ReturnsEmpty if true, the empty strings are returned
  @return a vector of strings
  @deprecated Since version 2.2.0. Use openfluid::tools::split() instead
*/
[[deprecated]] std::vector<std::string> OPENFLUID_API splitString(const std::string& StrToSplit,
                                                                  const std::string& Separators,
                                                                  bool ReturnsEmpty = false);


/**
  Splits the given string into a std::string array, split using the given SepString
  @param[in] StrToSplit the string to split
  @param[in] Separator the separator character used to split the string
  @param[in] ReturnsEmpty if true, the empty strings are returned
  @return a vector of strings
  @deprecated Since version 2.2.0. Use openfluid::tools::split() instead
*/
[[deprecated]] std::vector<std::string> OPENFLUID_API splitString(const std::string& StrToSplit,
                                                                  char Separator,
                                                                  bool ReturnsEmpty = false);


/**
  Replace every occurrence of a given substring (target) in a string by wanted replacement substring
  @param[in] Data the string to work on
  @param[in] ToSearch the target substring
  @param[in] ReplaceStr the replacement substring
  @deprecated Since version 2.2.0. Use openfluid::tools::replace() instead
*/
[[deprecated]] void OPENFLUID_API stringReplace(std::string& Data, 
                                                const std::string& ToSearch, const std::string& ReplaceStr);


} } // namespaces


#endif /* __OPENFLUID_TOOLS_DATAHELPERS_HPP__ */
