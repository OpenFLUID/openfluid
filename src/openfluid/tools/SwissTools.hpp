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
  @file

  @author JC.Fabre <fabrejc@supagro.inra.fr>
*/


#ifndef __OPENFLUID_TOOLS_SWISSTOOLS_HPP__
#define __OPENFLUID_TOOLS_SWISSTOOLS_HPP__


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
  Converts a string to another type
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


/**
  Converts a value to a string
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


/**
  Function for tokenizing string into a vector of tokens
  @param[in] StrToTokenize the string to tokenize
  @param[out] Tokens the resulting tokens
  @param[in] Delimiters the string delimiter
*/
void OPENFLUID_API TokenizeString(const std::string& StrToTokenize,
                              std::vector<std::string>& Tokens,
                              const std::string& Delimiters);


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


/**
  Checks if the given string matches the given pattern, including * and ? wildcards
  @param[in] Pattern The pattern to match
  @param[in] Str The string to test
  @return true if the given string matches the given pattern
*/
bool OPENFLUID_API WildcardMatching(const std::string& Pattern, const std::string& Str);


/**
  Gets the list of files with specified extension contained in the specified directory
  @param[in] DirToExplore the directory to explore
  @param[in] Ext the file extension
  @param[in] WithPath return full path with file name if true, file name only otherwise
  @param[in] ExtIncludeDot if true, the given extension through Ext parameter is suffixed by a dot
*/
std::vector<std::string> OPENFLUID_API GetFilesByExt(const std::string DirToExplore,
                                                 const std::string Ext,
                                                 bool WithPath = false,
                                                 bool ExtIncludeDot = false);


/**
  Get list of files with specified extension contained in the specified dir
  @param[in] DirToExplore the directory to explore
  @param[in] Ext the file extension
  @param[in] Suffix the file suffix
  @param[in] WithPath return full path with file name if true, file name only otherwise
  @param[in] ExtIncludeDot if true, the given extension through Ext parameter is suffixed by a dot
*/
std::vector<std::string> OPENFLUID_API GetFilesBySuffixAndExt(const std::string& DirToExplore,
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
std::vector<std::string> OPENFLUID_API SplitString(const std::string& StrToSplit,
                                               const std::string& Separators,
                                               bool ReturnsEmpty = false);


/**
  Recursively removes all files and directories contained in the given directory.
  It deletes the directory and recreates it.
  @param[in] DirPath the directory to empty
  @return true if successful
*/
bool OPENFLUID_API EmptyDirectoryRecursively(const std::string& DirPath);


/**
  Recursively finds all the paths of the files that exist exist
  in the given directory and subdirectories
  @param[in] DirPath the directory to explore
  @return a vector containing all files paths
*/
std::vector<std::string> OPENFLUID_API GetFilesRecursively(const std::string& DirPath);


/**
  Replaces a string by another string if it is empty
  @param[in] SourceStr the source string to process
  @param[in] ReplaceStr the replacement string to use
  @return the processed string
*/
std::string OPENFLUID_API ReplaceEmptyString(std::string SourceStr,
		                                 const std::string& ReplaceStr);


/**
  Removes trailing slashes if any, useful for cleaning paths
  @param[in] Str the string to process
  @return the processed string
*/
std::string OPENFLUID_API RemoveTrailingSlashes(std::string Str);


// TODO check if it has to be removed
void OPENFLUID_API printSTDOUT(std::vector<std::string> Strings, std::string Sep);


/**
  Copies a source directory to destination path, including the root of the source directory.
  If the destination path does not exists, it is created.
  @param[in] SourcePath the source directory to copy
  @param[in] IntoPath the destination directory
  @param[in] DontCopyDotDirs flag for copying dot ('.*') directories. Default is false.
*/
void OPENFLUID_API CopyDirectoryRecursively(const std::string& SourcePath,
		                                const std::string& IntoPath,
		                                const bool DontCopyDotDirs = false);


/**
  Copies a source directory to destination path, not including the root of the source directory.
  If the destination path does not exists, it is created.
  @param[in] SourcePath the source directory to copy
  @param[in] IntoPath the destination directory
  @param[in] DontCopyDotDirs flag for copying dot ('.*') directories. Default is false.
*/
void OPENFLUID_API CopyDirectoryContentsRecursively(const std::string& SourcePath,
		                                        const std::string& IntoPath,
		                                        const bool DontCopyDotDirs = false);


/**
  Compares two OpenFLUID software versions. Version number must be formed as major.minor.patch[~status]
  @param[in] VersionA the first version number
  @param[in] VersionB the second version number
  @param[in] Strict If true, the comparison include the status part of the version (it ignores it otherwise)
  @return 1 if VersionA is greater than VersionB, -1 if VersionB is greater than VersionA, 0 if versions are equals, -2 if a version format is not well-formed
*/
int OPENFLUID_API CompareVersions(const std::string& VersionA, const std::string& VersionB, bool Strict = true);


/*
  Suspend execution for microseconds
  @param[in] MSec the microseconds interval
*/
void OPENFLUID_API Sleep(const unsigned int MSec);


} } //namespaces


#endif // __OPENFLUID_TOOLS_SWISSTOOLS_HPP__

