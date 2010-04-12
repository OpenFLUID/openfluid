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


// =====================================================================
// =====================================================================



/**
  Get list of files with specified extension contained in the specified dir
  \param[in] DirToExplore the directory to explore
  \param[in] Ext the file extension
  \param[in] WithPath return full path with file name if true, file name only otherwise
*/
std::vector<std::string> GetFilesByExt(const std::string DirToExplore, const std::string Ext, bool WithPath = false, bool ExtIncludeDot = false);

/**
  Splits the passed string into a std::string array, split using the given SepString
  \param[in] StrToSplit the string to split
  \param[in] SepString the string used to split
  \param[in] ReturnsEmpty if true, the empty strings are returned
  \return a vector of strings
*/
std::vector<std::string> SplitString(const std::string StrToSplit, const std::string Separators,  bool ReturnsEmpty = false);

bool EmptyDirectoryRecursively(const std::string DirPath);

std::string ReplaceEmptyString(std::string SourceStr, std::string ReplaceStr);

bool IsVectorNamedVariable(std::string Name);

std::string GetVectorNamedVariableName(std::string Name);

std::string RemoveTrailingSlashes(std::string Str);

void printSTDOUT(std::vector<std::string> Strings, std::string Sep);

openfluid::core::DateTime GenerateDateTimeFromStep(openfluid::core::DateTime DT0, openfluid::core::TimeStep_t TS, unsigned int CurrentStep);

void CopyDirectoryRecursively(const std::string SourcePath, const std::string IntoPath, const bool DontCopyDotDirs = false);

std::vector<std::string> GetFileLocationsUsingPATHEnvVar(const std::string Filename);

#endif
