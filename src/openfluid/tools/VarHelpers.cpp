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


std::string ClassIDVar::getClassIDVarString() const
{
  std::string TripletString = UnitsClass + "#" + UnitsIDsStr + ":" + VariableName;
  return TripletString;
}


// =====================================================================
// =====================================================================


std::string ClassIDVarPrecision::getClassIDVarString(bool WithPrecision) const
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


UnitVarTriplets_t deserializeVarTriplets(const std::string& Selection)
{
  UnitVarTriplets_t ParsedSelection;
  for (const auto& SubS : openfluid::tools::split(Selection, ";"))
  {
    const auto PosHash = SubS.find("#");
    std::string UnitsClass = SubS.substr(0, PosHash);
    std::string ID = SubS.substr(PosHash+1,SubS.find(":")-(PosHash+1));
    std::string VarName = SubS.substr(SubS.find(":")+1);
    if (VarName.empty() || UnitsClass.empty())
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, "Error in variable parsing");
    }
    ClassIDVar VarTriplet(UnitsClass, ID, VarName);
    if (std::count(ParsedSelection.begin(), ParsedSelection.end(), VarTriplet) == 0)  // ensures unicity of Var/Class/ID
    {
      ParsedSelection.push_back(VarTriplet);
    }
  }
  return ParsedSelection;
}


// =====================================================================
// =====================================================================


UnitVarPairs_t OPENFLUID_API deduceVarPairs(const UnitVarTriplets_t& Triplets)
{
  UnitVarPairs_t Pairs;

  for (const auto& VarTriplet : Triplets)
  {
    ClassIDVar NewPair{VarTriplet.UnitsClass, VarTriplet.VariableName};
    if (std::find(Pairs.begin(), Pairs.end(), NewPair) == Pairs.end())
    {
      Pairs.push_back({VarTriplet.UnitsClass, VarTriplet.VariableName});
    }
  }
  return Pairs;
}


// =====================================================================
// =====================================================================


std::string OPENFLUID_API serializeVarTriplets(const UnitVarTriplets_t& Triplets)
{
  std::vector<std::string> TripletTexts;
  for (const auto& T : Triplets)
  {
    TripletTexts.push_back(T.getClassIDVarString());
  }
  return openfluid::tools::join(TripletTexts, ";");
}


} }
