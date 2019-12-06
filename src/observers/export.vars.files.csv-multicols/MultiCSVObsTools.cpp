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
  @file MultiCSVObsTools.cpp

  @author Armel THONI <armel.thoni@inra.fr>
*/


#include <sstream>
#include <chrono>
#include <regex>

#include <openfluid/base/OtherException.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/DataHelpers.hpp>

#include "MultiCSVObsTools.hpp"


MultiCSVFormat::MultiCSVFormat(): CSVFormat(), 
                                  MissingValueString("NA")
{

}


// =====================================================================
// =====================================================================


ClassIDVar::ClassIDVar():
  UnitsClassesStr(""), UnitsIDsStr(""), VariablesStr("")
{

}


// =====================================================================
// =====================================================================


std::string ClassIDVar::GetClassIDVarString(bool WithPrecision)
{
  std::string TripletString = UnitsClassesStr + "#" + UnitsIDsStr + ":" + VariablesStr;
  if (HasPrecision && WithPrecision)
  {
    TripletString += "%" + std::to_string(Precision);
  }
  return TripletString;
}


// =====================================================================
// =====================================================================


CSVMultiSet::CSVMultiSet() :
  Selection(""), SelectionList(), FormatName("")
{

}


// =====================================================================
// =====================================================================


std::vector<ClassIDVar> stringSelectionToClassIDVarList(const std::string SelectionStr, 
                                                        const unsigned int DefaultPrecision, 
                                                        const std::string DefaultFloatFormat)
{
  std::vector<std::string> Columns = openfluid::tools::splitString(SelectionStr, ";");
  
  
  std::vector<ClassIDVar> CSVTriplets;
  for (std::string Column : Columns)
  {
    // parse and create CSVTriplet
    std::size_t HashPosition = Column.find("#");
    std::size_t ColonPosition = Column.find(":");
    std::size_t PercentPosition = Column.find("%");
    
    ClassIDVar CurrentCSVTriplet;
    
    CurrentCSVTriplet.UnitsClassesStr = Column.substr(0,HashPosition);
    CurrentCSVTriplet.UnitsIDsStr = Column.substr(HashPosition+1, ColonPosition-HashPosition-1);
    CurrentCSVTriplet.FloatFormat = DefaultFloatFormat;
    
    if (PercentPosition != std::string::npos) // if precision information
    {
      CurrentCSVTriplet.VariablesStr = Column.substr(ColonPosition+1, PercentPosition-ColonPosition-1);
      CurrentCSVTriplet.HasPrecision = true;
      
      std::size_t EndOfPrecisionPos = Column.size();
      
      if (Column.back() == 'e' || Column.back() == 'f')
      {
        EndOfPrecisionPos--;
        if (Column.back() == 'e')
        {
          CurrentCSVTriplet.FloatFormat = "scientific";
        }
        else
        {
          CurrentCSVTriplet.FloatFormat = "fixed";
        }
      }
      CurrentCSVTriplet.Precision = std::stoi(Column.substr(PercentPosition+1, EndOfPrecisionPos).c_str());
    }
    else
    {
      CurrentCSVTriplet.VariablesStr = Column.substr(ColonPosition+1);
      CurrentCSVTriplet.HasPrecision = false;
      CurrentCSVTriplet.Precision = DefaultPrecision;
    }
    
    CSVTriplets.push_back(CurrentCSVTriplet);
  }
  return CSVTriplets;
}


// =====================================================================
// =====================================================================


std::string buildMultiColsdHeader(const MultiCSVFormat& Format, const std::string& FilePath,
                        const std::string ColumnsHeaders)
{
  std::ostringstream HeaderSStr;

  if(Format.Header == MultiCSVFormat::Info || Format.Header == MultiCSVFormat::Full)
  {
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);

    HeaderSStr << Format.CommentChar << "========================================================================\n";
    HeaderSStr << Format.CommentChar << " file: " << openfluid::tools::Filesystem::filename(FilePath) << "\n";
    HeaderSStr << Format.CommentChar << " date: " << std::ctime(&t);
    HeaderSStr << Format.CommentChar << " columns: " << ColumnsHeaders << "\n";
    HeaderSStr << Format.CommentChar << "========================================================================\n";

  }

  HeaderSStr << buildTimeHeader(Format, ColumnsHeaders);

  return HeaderSStr.str();
}


// =====================================================================
// =====================================================================


std::string buildMultiColsFilename(const std::string& OutputDir, const std::string& OutFileExt,
                          const std::string& SetName)
{
  std::ostringstream oss;
  oss << OutputDir << "/" << SetName << "." << OutFileExt;

  return oss.str();
}


// =====================================================================
// =====================================================================


std::vector<std::string> parseMultiColsSetsFromParamsTree(const openfluid::ware::WareParamsTree& ParamsTree,
                                                 MultiSetMap_t& Sets)
{
  std::vector<std::string> Messages;

  Sets.clear();

  if (ParamsTree.root().hasChild("set"))
  {
    for (auto& Set : ParamsTree.root().child("set"))
    {
      std::string SetName = Set.first;

      Sets[SetName].Selection = Set.second.getChildValue("selection","");
      Sets[SetName].FormatName = Set.second.getChildValue("format","");
    }
  }

  return Messages;
}


// =====================================================================
// =====================================================================


std::vector<std::string> parseMultiColsFormatsFromParamsTree(const openfluid::ware::WareParamsTree& ParamsTree,
                                                    MultiFormatMap_t& Formats)
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
      
      Formats[FormatName].MissingValueString = Format.second.getChildValue("missingvalue","NA").get();
    }
  }
  return Messages;
}
