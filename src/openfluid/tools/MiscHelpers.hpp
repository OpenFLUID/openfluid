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
  @file MiscHelpers.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_TOOLS_MISCHELPERS_HPP__
#define __OPENFLUID_TOOLS_MISCHELPERS_HPP__


#include <vector>
#include <string>
#include <cmath>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>


#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x


namespace openfluid { namespace tools {


/**
  Checks if the given string matches the given pattern, including * and ? wildcards
  @param[in] Pattern The pattern to match
  @param[in] Str The string to test
  @return true if the given string matches the given pattern
*/
bool OPENFLUID_API matchWithWildcard(const std::string& Pattern, const std::string& Str);


/**
  Replaces a string by another string if it is empty
  @param[in] SourceStr the source string to process
  @param[in] ReplaceStr the replacement string to use
  @return the processed string
*/
std::string OPENFLUID_API replaceEmptyString(std::string SourceStr,
                                             const std::string& ReplaceStr);


/**
  Compares two OpenFLUID software versions. Version number must be formed as major.minor.patch[~status]
  @param[in] VersionA the first version number
  @param[in] VersionB the second version number
  @param[in] Strict If true, the comparison include the status part of the version (it ignores it otherwise)
  @return 1 if VersionA is greater than VersionB,
          -1 if VersionB is greater than VersionA,
          0 if versions are equals,
          -2 if a version format is not well-formed
*/
int OPENFLUID_API compareVersions(const std::string& VersionA, const std::string& VersionB, bool Strict = true);


/**
  Suspends execution of current thread for milliseconds
  @param[in] MSec the milliseconds interval
*/
void OPENFLUID_API millisleep(const unsigned long MSec);


/**
  Suspend execution of current thread for microseconds
  @param[in] USec the microseconds interval
*/
void OPENFLUID_API microsleep(const unsigned long USec);


/**
  Returns the current date and time as a formatted string
  @param[in] Format an strftime()-like format string
  @return the formatted date-time
*/
std::string OPENFLUID_API getNowAsString(const std::string& Format);


/**
  Generates a pseudo-unique identifier using alphanumeric characters and Mersenne Twister random engine.
  @param[in] Length the length of the identifier to generate
  @return the generated identifier
*/
std::string OPENFLUID_API generatePseudoUniqueIdentifier(const unsigned int Length);


/**
  Returns the given regex pattern string with special chars that are escaped
  @param[in] Str the regex pattern
  @return the regex pattern with special chars that are escaped
*/
std::string OPENFLUID_API escapePattern(const std::string& Str);


/**
  Returns a pair of integer values extracted from a geometry string
  @param[in] Str the geometry string (e.g. "@Point(127 82)", "@Size(1024 768)")
  @param[in] Geominfo the expected geometry information type (e.g. "Point", "Size")
  @return the pair of values extracted from the geometry string
*/
std::pair<int,int> OPENFLUID_API fromGeometryString(const std::string& Str, const std::string& GeomInfo);


/**
  Returns a geometry string from a geometry info type and a pair of integer values
  @param[in] Geominfo the geometry information type (e.g. "Point", "Size")
  @param[in] Value1 the first value
  @param[in] Value2 the second value
  @return the geometry string
*/
std::string OPENFLUID_API toGeometryString(const std::string& GeomInfo, int Value1, int Value2);


} } //namespaces


#endif /* __OPENFLUID_TOOLS_MISCHELPERS_HPP__ */

