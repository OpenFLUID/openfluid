/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __APPTOOLS_H__
#define __APPTOOLS_H__

#include <string>
#include <vector>
#include <openfluid-core.h>

/**
  Get current program executable path
  \return a std::string containing the path, empty if indetermined

*/
std::string GetExecutablePath();

/**
  Get list of files with specified extension contained in the specified dir
  \param[in] DirToExplore the directory to explore
  \param[in] Ext the file extension
  \param[in] WithPath return full path with file name if true, file name only otherwise
*/
std::vector<std::string> GetFilesByExt(const std::string DirToExplore, const std::string Ext, bool WithPath = false, bool ExtIncludeDot = false);

/**
  Splits the passed string int a std::stringArray, splitted by the passed SepString
  \param[in] StrToSplit the string to split
  \param[in] SepString the string used to split
  \param[in] ReturnsEmpty if true, the empty strings are returned
  \return a wxArraySTring
*/
std::vector<std::string> SplitString(const std::string StrToSplit, const std::string SepString,  bool ReturnsEmpty = false);

std::string GenerateSimulationID();

std::string FormatExecutionMessage(std::string Message);

bool EmptyDirectoryRecursively(const char* DirPath);

std::string ReplaceEmptyString(std::string SourceStr, std::string ReplaceStr);

bool IsVectorNamedVariable(std::string Name);

std::string GetVectorNamedVariableName(std::string Name);

std::string RemoveTrailingSlashes(std::string Str);

void printSTDOUT(std::vector<std::string> Strings, std::string Sep);

openfluid::core::DateTime GenerateDateTimeFromStep(openfluid::core::DateTime DT0, openfluid::core::TimeStep_t TS, unsigned int CurrentStep);


#endif
