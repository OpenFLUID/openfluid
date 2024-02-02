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
  @file MultiCSVObsTools.hpp

  @author Armel THONI <armel.thoni@inra.fr>
*/


#ifndef __MULTICSVOBSTOOLS_HPP__
#define __MULTICSVOBSTOOLS_HPP__


#include <map>
#include <vector>

#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/ware/WareParamsTree.hpp>
#include <openfluid/tools/VarHelpers.hpp>

#include "../../CsvObserverBase.hpp"


// =====================================================================
// =====================================================================


class MultiCSVFormat: public CSVFormat
{
  public:
    
    std::string MissingValueString;

    MultiCSVFormat();
    
    std::vector<std::string> generateFormatFields(const std::string& FormatName)
    {
        std::vector<std::string> Fields = CSVFormat::generateFormatFields(FormatName);
        Fields.push_back(MissingValueString);
        return Fields;
    }

};


 // =====================================================================
 // =====================================================================


class CSVMultiSet
{
  public:
      
    std::string Selection;
    
    std::vector<openfluid::tools::ClassIDVarPrecision> SelectionList;
    
    std::vector<openfluid::tools::ClassIDVarPrecision> ExpandedSelection;

    std::string FormatName;

    CSVMultiSet();
};


// =====================================================================
// =====================================================================


typedef std::map<std::string, CSVMultiSet> MultiSetMap_t;

typedef std::map<std::string, MultiCSVFormat> MultiFormatMap_t;


// =====================================================================
// =====================================================================


std::vector<openfluid::tools::ClassIDVarPrecision> stringSelectionToClassIDVarList(const std::string& SelectionStr, 
                                                        const unsigned int DefaultPrecision, 
                                                        const std::string& DefaultFloatFormat);

std::vector<std::string> parseMultiColsSetsFromParamsTree(const openfluid::ware::WareParamsTree& ParamsTree,
                                                          MultiSetMap_t& Sets);

std::string buildMultiColsFilename(const std::string& OutputDir, const std::string& OutFileExt,
                                   const std::string& SetName);

std::string buildMultiColsdHeader(const MultiCSVFormat& Format, const std::string& FilePath,
                                  const std::string& ColumnsHeaders);

std::vector<std::string> parseMultiColsFormatsFromParamsTree(const openfluid::ware::WareParamsTree& ParamsTree,
                                                             MultiFormatMap_t& Formats);

#endif /* __MULTICSVOBSTOOLS_HPP__ */
