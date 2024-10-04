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
  @file StringHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
 */


#ifndef __OPENFLUID_TOOLS_STRINGHELPERS_HPP__
#define __OPENFLUID_TOOLS_STRINGHELPERS_HPP__


#include <string>
#include <list>
#include <sstream>
#include <vector>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


/**
  Converts a string to lower case
  @snippet misc/strings.cpp str_lower
  @param[in] Str the string to convert
  @return the string converted to lower case
*/
std::string OPENFLUID_API toLowerCase(const std::string& Str);


/**
  Converts a string to upper case
  @snippet misc/strings.cpp str_upper
  @param[in] Str the string to convert
  @return the string converted to upper case
*/
std::string OPENFLUID_API toUpperCase(const std::string& Str);


/**
  Trims all the whitespaces character located on the left side of the string
  @snippet misc/strings.cpp str_triml
  @param[in] Str the string to trim
  @return the trimmed string
*/
std::string OPENFLUID_API trimLeft(const std::string& Str);


/**
  Trims all the whitespaces character located on the right side of the string
  @snippet misc/strings.cpp str_trimr
  @param[in] Str the string to trim
  @return the trimmed string
*/
std::string OPENFLUID_API trimRight(const std::string& Str);


/**
  Trims all the whitespaces character located on both right and left sides of the string
  @snippet misc/strings.cpp str_trim
  @param[in] Str the string to trim
  @return the trimmed string
*/
std::string OPENFLUID_API trim(const std::string& Str);


/**
  Converts a string into a numeric value.  
  Due to the poor performance of this generic method, it is preferable to use standard C++ typed methods
  such as std::stoi, std:stod and others to convert a string into numerical values
  @snippet misc/strings.cpp str_numbool
  @param[in] Str the string to convert
  @param[out] Num the converted numerical value
  @return true if conversion is successful, false otherwise
  @sa std::stoi() to convert integer values : https://en.cppreference.com/w/cpp/string/basic_string/stol
  @sa std::stof() to convert floating point values : https://en.cppreference.com/w/cpp/string/basic_string/stof
*/
template<typename T>
inline bool toNumeric(const std::string& Str, T& Num)
{
  std::istringstream iss(Str);
  char C;
  return ((iss >> (Num)) && !iss.get(C));
}


/**
  Converts a string into a numeric value.  
  Due to the poor performance of this generic method, it is preferable to use standard C++ typed methods
  such as std::stoi, std:stod and others to convert a string into numerical values
  @snippet misc/strings.cpp str_num
  @param[in] Str the string to convert
  @return the converted numerical value
  @throws openfluid::base::FrameworkException Error during conversion
  @sa std::stoi() to convert integer values : https://en.cppreference.com/w/cpp/string/basic_string/stol
  @sa std::stof() to convert floating point values : https://en.cppreference.com/w/cpp/string/basic_string/stof
*/
template<typename T>
inline T toNumeric(const std::string& Str)
{
  T Num;
  if (!toNumeric(Str,Num))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error converting '"+Str+"' to numeric value");
  }
  return Num;
}


/**
  Checks if a string starts with a given substring
  @snippet misc/strings.cpp str_starts
  @param[in] Str the string to check
  @param[in] SubStr the substring
  @return true if the string starts with the substring, false otherwise
*/
bool OPENFLUID_API startsWith(const std::string& Str,const std::string& SubStr);


/**
  Checks if a string ends with a given substring
  @snippet misc/strings.cpp str_ends
  @param[in] Str the string to check
  @param[in] SubStr the substring
  @return true if the string ends with the substring, false otherwise
*/
bool OPENFLUID_API endsWith(const std::string& Str,const std::string& SubStr);


/**
  Checks if a string contains a given substring
  @snippet misc/strings.cpp str_contains
  @param[in] Str the string to check
  @param[in] SubStr the substring
  @param[in] CaseSensitive true for case sensitive
  @return true if the string contains the substring, false otherwise
*/
bool OPENFLUID_API contains(const std::string& Str,const std::string& SubStr, bool CaseSensitive = true);


/**
  Replaces every occurrence in a string of a searched substring by a replacement substring
  @snippet misc/strings.cpp str_repl
  @param[in] Str the original string
  @param[in] SearchStr the searched substring
  @param[in] ReplaceStr the replacement substring
  @return a string with replaced parts
*/
std::string OPENFLUID_API replace(const std::string& Str,const std::string& SearchStr, const std::string& ReplaceStr);


/**
  Formats a string using a format and a variable number of arguments.  
  As it is based on the C snprintf functions, it uses the same format specification, 
  and has the same limitations in usable types, performance and error handling
  @snippet misc/strings.cpp str_format
  @param[in] Fmt The format string using POSIX format style
  @param[in] Args The variadic list of arguments to format
  @sa snprintf function specification: https://en.cppreference.com/w/cpp/io/c/fprintf
*/
template<class...A>
inline std::string format(const std::string& Fmt, A&&...Args)
{
  const int Size = std::snprintf(nullptr,0,Fmt.c_str(), std::forward<A>(Args)...); // first call to compute buffer size
  char* Buf = new char[Size+1];  // allow buffer, +1 for terminal '\0' char
  std::snprintf(Buf,Size+1,Fmt.c_str(), std::forward<A>(Args)...);  // second call for effective formatting

  std::string Str(Buf);  // convert buffer to string
  delete[] Buf;  // free buffer
  return Str;
}


/**
  Splits a string into a vector of strings using a separator character
  @snippet misc/strings.cpp str_split
  @param[in] Str the string to split
  @param[in] Sep the separator character used to split the string
  @param[in] KeepEmpty if true, empty strings are returned (disabled by default)
  @return a vector of strings
*/
std::vector<std::string> OPENFLUID_API split(const std::string& Str, const char Sep, bool KeepEmpty = false);


/**
  Splits a string into a vector of strings using any character of the the string of separators
  @snippet misc/strings.cpp str_split
  @param[in] Str the string to split
  @param[in] SepChars the string of separators used to split the string
  @param[in] KeepEmpty if true, empty strings are returned (disabled by default)
  @return a vector of strings
*/
std::vector<std::string> OPENFLUID_API split(const std::string& Str, const std::string& SepChars,
                                             bool KeepEmpty = false);


/**
  Joins a vector of strings into a string using a separator
  @snippet misc/strings.cpp str_join
  @param[in] Vect the vector of strings to join
  @param[in] Sep the string used as separator
  @return the joined string
*/
std::string OPENFLUID_API join(const std::vector<std::string>& Vect, const std::string& Sep);


} } // namespaces


// =====================================================================
// =====================================================================


/**
  Stream operator to append a plain text to a vector of strings
  @snippet misc/strings.cpp vectstr_opstr
*/
std::vector<std::string>& OPENFLUID_API operator<<(std::vector<std::string>& Vect, std::string&& Str);


/**
  Stream operator to append a string to a vector of strings
  @snippet misc/strings.cpp vectstr_opstr
*/
std::vector<std::string>& OPENFLUID_API operator<<(std::vector<std::string>& Vect, const std::string& Str);


/**
  Stream operator to append a vector of string to another vector of strings
  @snippet misc/strings.cpp vectstr_opvect
*/
std::vector<std::string>& OPENFLUID_API operator<<(std::vector<std::string>& Vect, 
                                                   const std::vector<std::string>& OtherVect);


#endif /* __OPENFLUID_TOOLS_STRINGHELPERS_HPP__ */
