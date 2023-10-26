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
  @file IDHelpers.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <regex>

#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace tools {


std::string classIDToString(const openfluid::core::UnitsClass_t& Class, openfluid::core::UnitID_t ID)
{
  return Class + "#" + std::to_string(ID);
}


// =====================================================================
// =====================================================================


bool isValidAlphaNumName(const std::string& Name)
{
  // authorized chars: a to z, A to Z, 0 to 9
  std::regex Exp("[A-Za-z0-9]+");

  return std::regex_match(Name,Exp);
}


// =====================================================================
// =====================================================================


bool isValidDatasetName(const std::string& Name)
{
  // authorized chars: a to z, A to Z, 0 to 9, -, ., _
  // must start by an alphanumeric char

  return std::regex_match(Name,std::regex(DatasetNameRuleString));
}


// =====================================================================
// =====================================================================


bool isValidWareID(const openfluid::ware::WareID_t& ID, bool Template)
{
  // authorized chars: a to z, A to Z, 0 to 9, -, ., _
  // must start by an alphanumeric char

  if (Template)
  {
    return std::regex_match(ID,std::regex(WareIDRuleStringAndTpl));
  }
  else
  {
    return std::regex_match(ID,std::regex(WareIDRuleString));
  }
}


// =====================================================================
// =====================================================================


bool isValidVariableName(const openfluid::core::VariableName_t& Name)
{
  // authorized chars: a to z, A to Z, 0 to 9, -, ., _
  // must start by an alphanumeric char
  std::regex Exp("[A-Za-z0-9]+([A-Za-z0-9_\\.\\-]*)");

  return std::regex_match(Name,Exp);
}


// =====================================================================
// =====================================================================


bool isValidTypedVariableName(const openfluid::core::VariableName_t& Name)
{
  // authorized chars: a to z, A to Z, 0 to 9, -, ., _
  // must start by an alphanumeric char
  // can have a type between square brackets at the end
  std::regex 
    Exp("[A-Za-z0-9]+([A-Za-z0-9_\\.\\-]*)(\\[(|double|integer|boolean|string|vector|matrix|map|tree|null)\\])?");

  return std::regex_match(Name,Exp);
}


// =====================================================================
// =====================================================================


bool isValidAttributeName(const openfluid::core::AttributeName_t& Name)
{
  // authorized chars: a to z, A to Z, 0 to 9, -, ., _
  // must start by an alphanumeric char
  std::regex Exp("[A-Za-z0-9]+([A-Za-z0-9_\\.\\-]*)");

  return std::regex_match(Name,Exp);
}


// =====================================================================
// =====================================================================


bool extractVariableNameAndType(const openfluid::core::VariableName_t& Name,
                               openfluid::core::VariableName_t& VarName,
                               openfluid::core::Value::Type& VarType)
{
  if (!isValidTypedVariableName(Name))
  {
    return false;
  }

  std::size_t InPos = Name.find("[");

  // no type
  if (InPos == std::string::npos)
  {
    VarName = Name;
    VarType = openfluid::core::Value::NONE;
    return true;
  }

  //short vector type
  if (Name.substr(Name.size()-2,2) == "[]")
  {
    VarName = Name.substr(0,Name.size()-2);
    VarType = openfluid::core::Value::VECTOR;
    return true;
  }

  // other types
  VarName = Name.substr(0,InPos);
  return openfluid::core::Value::getValueTypeFromString(Name.substr(InPos+1,Name.size()-InPos-2),VarType);

}


// =====================================================================
// =====================================================================


std::string buildGeneratorID(const openfluid::core::VariableName_t& VarName,
                             bool IsVector,
                             const openfluid::core::UnitsClass_t& ClassName)
{
  // <varname>.<unitsclass>.gen<type> where <type> can be scalar or vector

  std::string GenID(VarName);
  GenID += ".";
  GenID += ClassName;
  GenID += ".gen";
  if (IsVector)
  {
    GenID += "vector";
  }
  else
  {
    GenID += "scalar";
  }

  return GenID;
}

} } // namespaces
