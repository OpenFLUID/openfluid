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
  @file CSVObsTools.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <sstream>
#include <chrono>
#include <regex>

#include <openfluid/base/OtherException.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include "CSVObsTools.hpp"


CSVSet::CSVSet() :
  UnitsClass(""), UnitsIDsStr(""), isAllUnits(false), VariableName(""), isAllVars(false), FormatName("")
{

};


// =====================================================================
// =====================================================================


std::string buildHeader(const CSVFormat& Format, const std::string& FilePath,
                        const openfluid::core::UnitsClass_t& UClass, openfluid::core::UnitID_t UID,
                        const openfluid::core::VariableName_t& VarName)
{
  std::ostringstream HeaderSStr;

  if(Format.Header == CSVFormat::HeaderType::Info || Format.Header == CSVFormat::HeaderType::Full)
  {
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);

    HeaderSStr << Format.CommentChar << "========================================================================\n";
    HeaderSStr << Format.CommentChar << " file: " << openfluid::tools::FilesystemPath(FilePath).filename() << "\n";
    HeaderSStr << Format.CommentChar << " date: " << std::ctime(&t);
    HeaderSStr << Format.CommentChar << " unit: " << UClass << " #" << UID << "\n";
    HeaderSStr << Format.CommentChar << " variable: " << VarName << "\n";
    HeaderSStr << Format.CommentChar << "========================================================================\n";

  }

  HeaderSStr << buildTimeHeader(Format, VarName);

  return HeaderSStr.str();
}


// =====================================================================
// =====================================================================


std::string buildFilename(const std::string& OutputDir, const std::string& OutFileExt,
                          const std::string& SetName,
                          const openfluid::core::UnitsClass_t& UnitsClass,
                          const openfluid::core::UnitID_t& UnitID,
                          const openfluid::core::VariableName_t& Varname)
{
  std::ostringstream oss;

  oss << OutputDir << "/" << SetName << "_" << UnitsClass << UnitID << "_" << Varname << "." << OutFileExt;

  return oss.str();
}


// =====================================================================
// =====================================================================


std::vector<std::string> parseFormatsFromParamsTree(const openfluid::ware::WareParamsTree& ParamsTree,
                                                    FormatMap_t& Formats)
{
  std::vector<std::string> Messages;

  Formats.clear();

  if (ParamsTree.root().hasChild("format"))
  {
    for (auto Format : ParamsTree.root().child("format"))
    {
      std::string FormatName = Format.first;

      Formats[FormatName].ColSeparator = basicParseFormatsFromParamsTree(Format.second, "colsep");
      long Precision;
      basicParseFormatsFromParamsTree(Format.second, "precision").toInteger(Precision);
      Formats[FormatName].Precision = Precision;
      Formats[FormatName].FloatFormat = basicParseFormatsFromParamsTree(Format.second, "float-format");
      Formats[FormatName].DateFormat = basicParseFormatsFromParamsTree(Format.second, "date").get();
      Formats[FormatName].IsTimeIndexDateFormat = (Formats[FormatName].DateFormat == "timeindex");
      Formats[FormatName].CommentChar = basicParseFormatsFromParamsTree(Format.second, "commentchar");
      Formats[FormatName].Header = StrToHeaderType(basicParseFormatsFromParamsTree(Format.second, "header").get());
    }
  }

  return Messages;
}


// =====================================================================
// =====================================================================


std::vector<std::string> parseSetsFromParamsTree(const openfluid::ware::WareParamsTree& ParamsTree,
                                                 SetMap_t& Sets)
{
  std::vector<std::string> Messages;

  Sets.clear();

  if (ParamsTree.root().hasChild("set"))
  {
    for (auto& Set : ParamsTree.root().child("set"))
    {
      std::string SetName = Set.first;

      Sets[SetName].UnitsClass = Set.second.getChildValue("unitsclass","");
      if (Sets[SetName].UnitsClass == "")
      {
        // search for deprecated "unitclass" parameter
        Sets[SetName].UnitsClass = Set.second.getChildValue("unitclass","");
        if (!Sets[SetName].UnitsClass.empty())
        {
          Messages.push_back("Usage of set.<setname>.unitclass parameter is deprecated."
              "Use set.<setname>.unitsclass instead");
        }
      }

      Sets[SetName].UnitsIDsStr = Set.second.getChildValue("unitsIDs","*");
      Sets[SetName].VariableName = Set.second.getChildValue("vars","*");

      Sets[SetName].FormatName = Set.second.getChildValue("format","");
    }
  }

  return Messages;
}

