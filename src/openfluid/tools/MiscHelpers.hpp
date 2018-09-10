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


namespace openfluid { namespace core {
  class DateTime;
} }


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
  Removes trailing slashes if any, useful for cleaning paths
  @param[in] Str the string to process
  @return the processed string
*/
std::string OPENFLUID_API removeTrailingSlashes(const std::string& Str);


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


/*
  Suspends execution of current thread for milliseconds
  @param[in] MSec the milliseconds interval
*/
void OPENFLUID_API millisleep(const unsigned long MSec);


/*
  Suspend execution of current thread for microseconds
  @param[in] USec the microseconds interval
*/
void OPENFLUID_API microsleep(const unsigned long USec);


/**
  Generates a pseudo-unique identifier using alphanumeric characters and Mersenne Twister random engine.
  @param[in] Length the length of the identifier to generate
  @return the generated identifier
*/
std::string OPENFLUID_API generatePseudoUniqueIdentifier(const unsigned int Length);


/**
  splits the given duration in milliseconds into days, hours, minutes, seconds and milliseconds.
  @param[in] MSecsDuration the duration in milliseconds
  @param[out] Days the number of days
  @param[out] Hours the number of hours
  @param[out] Minutes the number of minutes
  @param[out] Seconds the number of seconds
  @param[out] MSecs the number of milliseconds
*/
void OPENFLUID_API splitDuration(long int MSecsDuration, int& Days, int& Hours, int& Minutes, int& Seconds, int& MSecs);


/**
  returns the given duration as a pretty string representing days, hours, minutes and decimal seconds.
  @code
  openfluid::tools::getDurationAsPrettyString(123456789); // returns "1d 10h 17m 36.789s"
  openfluid::tools::getDurationAsPrettyString(12345678);  // returns "3h 25m 45.678s"
  openfluid::tools::getDurationAsPrettyString(123456);    // returns "2m 3.456s"
  openfluid::tools::getDurationAsPrettyString(1234);      // returns "1.234s"
  openfluid::tools::getDurationAsPrettyString(12);        // returns "0.012s"
  @endcode
  @param[in] MSecsDuration the duration in milliseconds
  @return the converted duration as a string
*/
std::string OPENFLUID_API getDurationAsPrettyString(long int MSecsDuration);


} } //namespaces


#endif // __OPENFLUID_TOOLS_MISCHELPERS_HPP__

