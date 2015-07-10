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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <sstream>
#include <chrono>
#include <regex>

#include <openfluid/base/OtherException.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include "CSVObsTools.hpp"


CSVFormat::CSVFormat():
  Header(Info), ColSeparator(";"), DateFormat("%Y%m%dT%H%M%S"),
  CommentChar("#"), Precision(5), IsTimeIndexDateFormat(false)
{

};


// =====================================================================
// =====================================================================


CSVSet::CSVSet() :
  UnitsClass(""), UnitsIDsStr(""), isAllUnits(false), VariablesStr(""), isAllVars(false), FormatName("")
{

};


// =====================================================================
// =====================================================================


CSVFormat::HeaderType StrToHeaderType(const std::string& HeaderStr)
{
  if (HeaderStr == "none")
    return CSVFormat::None;
  else if (HeaderStr == "colnames-as-data")
    return CSVFormat::ColnamesAsData;
  else if (HeaderStr == "colnames-as-comment")
          return CSVFormat::ColnamesAsComment;
  else if (HeaderStr == "full")
    return CSVFormat::Full;
  else
    return CSVFormat::Info;
}


// =====================================================================
// =====================================================================


std::string HeaderTypeToStr(CSVFormat::HeaderType HType)
{
  if (HType == CSVFormat::ColnamesAsComment)
    return "colnames-as-comment";
  else if (HType == CSVFormat::ColnamesAsData)
    return "colnames-as-data";
  else if (HType == CSVFormat::Full)
    return "full";
  else if (HType == CSVFormat::None)
    return "none";
  else
    return "";
}


// =====================================================================
// =====================================================================


std::string buildHeader(const CSVFormat& Format, const std::string& FilePath,
                        const openfluid::core::UnitsClass_t& UClass, openfluid::core::UnitID_t UID,
                        const openfluid::core::VariableName_t& VarName)
{
  std::ostringstream HeaderSStr;

  if(Format.Header == CSVFormat::Info || Format.Header == CSVFormat::Full)
  {
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);

    HeaderSStr << Format.CommentChar << "========================================================================\n";
    HeaderSStr << Format.CommentChar << " file: " << openfluid::tools::Filesystem::filename(FilePath) << "\n";
    HeaderSStr << Format.CommentChar << " date: " << std::ctime(&t);
    HeaderSStr << Format.CommentChar << " unit: " << UClass << " #" << UID << "\n";
    HeaderSStr << Format.CommentChar << " variable: " << VarName << "\n";
    HeaderSStr << Format.CommentChar << "========================================================================\n";

  }

  if(Format.Header == CSVFormat::ColnamesAsComment || Format.Header == CSVFormat::Full)
  {
    if (Format.IsTimeIndexDateFormat)
      HeaderSStr << Format.CommentChar << "timeindex";
    else
      HeaderSStr << Format.CommentChar << "datetime";

    HeaderSStr << Format.ColSeparator << VarName << "\n";

  }

  if(Format.Header == CSVFormat::ColnamesAsData)
  {
    if (Format.IsTimeIndexDateFormat)
      HeaderSStr << "timeindex";
    else
      HeaderSStr << "datetime";

    HeaderSStr << Format.ColSeparator << VarName << "\n";
  }


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


std::string StrToDateFormat(const std::string& FormatStr)
{
  if (FormatStr == "ISO")
    return "%Y%m%dT%H%M%S";
  else if (FormatStr == "6cols")
    return "%Y\t%m\t%d\t%H\t%M\t%S";
  else
    return FormatStr;
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

      Formats[FormatName].ColSeparator = Format.second.getChildValue("colsep","\t");
      long Precision;
      Format.second.getChildValue("precision",5).toInteger(Precision);
      Formats[FormatName].Precision = Precision;
      Formats[FormatName].DateFormat = Format.second.getChildValue("date","ISO").get();
      Formats[FormatName].IsTimeIndexDateFormat = (Formats[FormatName].DateFormat == "timeindex");
      Formats[FormatName].CommentChar = Format.second.getChildValue("commentchar","#");
      Formats[FormatName].Header = StrToHeaderType(Format.second.getChildValue("header","").get());
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
          Messages.push_back("Usage of set.<setname>.unitclass parameter is deprecated."
              "Use set.<setname>.unitsclass instead");
      }

      Sets[SetName].UnitsIDsStr = Set.second.getChildValue("unitsIDs","*");
      Sets[SetName].VariablesStr = Set.second.getChildValue("vars","*");

      Sets[SetName].FormatName = Set.second.getChildValue("format","");
    }
  }

  return Messages;
}

