/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file

  @author JC.Fabre <fabrejc@supagro.inra.fr>
*/


#ifndef __SWISSTOOLS_HPP__
#define __SWISSTOOLS_HPP__

#include <vector>
#include <string>
#include <sstream>
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
  Template function for string to other type conversion
  @param[in] StrToConvert the string to convert
  @param[out] Converted the result of the conversion
  @return true if the conversion is correct
*/
template<typename T>
inline bool ConvertString(const std::string& StrToConvert, T* Converted)
{
  std::istringstream iss(StrToConvert);
  char c;
  return ((iss >> (*Converted)) && !iss.get(c));
}

// =====================================================================
// =====================================================================

/**
  Template function for value to string conversion
  @param[in] ValueToConvert the value to convert
  @param[out] StrConverted the result of the conversion
  @return true if the conversion is correct
*/
template<typename T>
inline bool ConvertValue(const T ValueToConvert, std::string * StrConverted)
{
  std::ostringstream oss;
  bool IsOK = !(oss << ValueToConvert).fail();

  if (IsOK) *StrConverted = oss.str();

  return IsOK;
}


// =====================================================================
// =====================================================================

/**
  Function for tokenizing string into a vector of tokens
  @param[in] StrToTokenize the string to tokenize
  @param[out] Tokens the resulting tokens
  @param[in] Delimiters the string delimiter
*/
void DLLEXPORT TokenizeString(const std::string& StrToTokenize,
                              std::vector<std::string>& Tokens,
                              const std::string& Delimiters);

// =====================================================================
// =====================================================================


/**
  Function for testing equality between two double precision floats,
  using the "close enough" method.
  @param[in] A the firts term of the equality
  @param[in] B the firts term of the equality
  @param[in] Epsilon the comparison tolerance factor

  @see http://www.parashift.com/c++-faq-lite/floating-point-arith.html
  @see http://www.boost.org/doc/libs/1_38_0/libs/test/doc/html/utf/testing-tools/floating_point_comparison.html
*/
inline bool IsCloseEnough(double A, double B, double Epsilon = 0.00001)
{
  // see Knuth section 4.2.2 pages 217-218
  return ((std::fabs(A - B)) <= (Epsilon * std::fabs(A)));
}


// =====================================================================
// =====================================================================


/**
  Function for testing equality between two double precision floats,
  using the "very close" method.
  @param[in] A the firts term of the equality
  @param[in] B the firts term of the equality
  @param[in] Epsilon the comparison tolerance factor

  @see http://www.parashift.com/c++-faq-lite/floating-point-arith.html
  @see http://www.boost.org/doc/libs/1_38_0/libs/test/doc/html/utf/testing-tools/floating_point_comparison.html
*/
inline bool IsVeryClose(double A, double B, double Epsilon = 0.00001)
{
  // see Knuth section 4.2.2 pages 217-218
  return (((std::fabs(A - B)) <= (Epsilon * std::fabs(A))) && ((std::fabs(A - B)) <= (Epsilon * std::fabs(B))));
}


// =====================================================================
// =====================================================================


bool DLLEXPORT WildcardMatching(const std::string Pattern, const std::string Str);



// =====================================================================
// =====================================================================



/**
  Get list of files with specified extension contained in the specified dir
  @param[in] DirToExplore the directory to explore
  @param[in] Ext the file extension
  @param[in] WithPath return full path with file name if true, file name only otherwise
  @param[in] ExtIncludeDot if true, the given extension through Ext parameter is suffixed by a dot
*/
std::vector<std::string> DLLEXPORT GetFilesByExt(const std::string DirToExplore,
                                                 const std::string Ext,
                                                 bool WithPath = false,
                                                 bool ExtIncludeDot = false);

/**
  Get list of files with specified extension contained in the specified dir
  @param[in] DirToExplore the directory to explore
  @param[in] Ext the file extension
  @param[in] WithPath return full path with file name if true, file name only otherwise
  @param[in] ExtIncludeDot if true, the given extension through Ext parameter is suffixed by a dot
*/
std::vector<std::string> DLLEXPORT GetFilesBySuffixAndExt(const std::string& DirToExplore,
                                                          const std::string& Suffix,
                                                          const std::string& Ext,
                                                          bool WithPath = false,
                                                          bool ExtIncludeDot = false);

/**
  Splits the passed string into a std::string array, split using the given SepString
  @param[in] StrToSplit the string to split
  @param[in] Separators the string of separators used to split the string
  @param[in] ReturnsEmpty if true, the empty strings are returned
  @return a vector of strings
*/
std::vector<std::string> DLLEXPORT SplitString(const std::string& StrToSplit,
                                               const std::string& Separators,
                                               bool ReturnsEmpty = false);

bool DLLEXPORT EmptyDirectoryRecursively(const std::string DirPath);

/**
  @action Store all the paths of the files existing in dir passed as parameter
*/
std::vector<std::string> DLLEXPORT getFilesRecursively(const std::string& DirPath);

std::string DLLEXPORT ReplaceEmptyString(std::string SourceStr, const std::string& ReplaceStr);

std::string DLLEXPORT RemoveTrailingSlashes(std::string Str);

void DLLEXPORT printSTDOUT(std::vector<std::string> Strings, std::string Sep);

void DLLEXPORT CopyDirectoryRecursively(const std::string SourcePath, const std::string IntoPath, const bool DontCopyDotDirs = false);

void DLLEXPORT CopyDirectoryContentsRecursively(const std::string SourcePath, const std::string IntoPath, const bool DontCopyDotDirs = false);


/**
  Compare two OpenFLUID software versions. Version number must be formed as major.minor.patch[~status]
  @param[in] VersionA the first version number
  @param[in] VersionB the second version number
  @param[in] Strict If true, the comparison include the status part of the version (it ignores it otherwise)
  @return 1 if VersionA is greater than VersionB, -1 if VersionB is greater than VersionA, 0 if versions are equals, -2 if a version format is not well-formed
*/
int DLLEXPORT CompareVersions(const std::string& VersionA, const std::string& VersionB, bool Strict = true);

/*
  Suspend execution for microseconds
  @param[in] MSec the microseconds interval
*/
void DLLEXPORT Sleep(const unsigned int MSec);


} } //namespaces


#endif // __SWISSTOOLS_H__

