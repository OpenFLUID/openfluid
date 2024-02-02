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
  @file VarHelpers.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <algorithm>

#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/tools/VarHelpers.hpp>


namespace openfluid { namespace tools {


// =====================================================================
// =====================================================================


std::string ClassIDVar::getClassIDVarString()
{
  std::string TripletString = UnitsClassesStr + "#" + UnitsIDsStr + ":" + VariablesStr;
  return TripletString;
}


// =====================================================================
// =====================================================================


std::string ClassIDVarPrecision::getClassIDVarString(bool WithPrecision)
{
  std::string TripletString = ClassIDVar::getClassIDVarString();
  if (HasPrecision && WithPrecision)
  {
    TripletString += "%" + std::to_string(Precision);
  }
  return TripletString;
}


// =====================================================================
// =====================================================================


std::vector<std::pair<std::string, std::string>> parseVars(const std::string& Selection)
{
  std::vector<std::pair<std::string, std::string>> ParsedSelection;
  for (const auto& SubS : openfluid::tools::split(Selection, ";"))
  {
    std::string UnitsClass = SubS.substr(0,SubS.find("#"));
    std::string VarName = SubS.substr(SubS.find(":")+1);
    std::pair<std::string, std::string> VarPair(VarName, UnitsClass);
    if (std::count(ParsedSelection.begin(), ParsedSelection.end(), VarPair) == 0)  // ensures unicity of Var/SU
    { 
      ParsedSelection.push_back(VarPair);
    }
  }
  return ParsedSelection;
}


} }
