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
  @file IDHelpers.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#ifndef __OPENFLUID_TOOLS_IDHELPERS_HPP__
#define __OPENFLUID_TOOLS_IDHELPERS_HPP__


#include <regex>
#include <string>

#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/tools/VarHelpers.hpp>


namespace openfluid { namespace tools {


// constant regex used for validation 
//   auto stands for char[] here
//   char[] used instead of string since not compatible with constexpr up to C++ 17 (TOWATCH)


//   authorized chars: a to z, A to Z, 0 to 9, -, ., _
//   must start by an alphanumeric char
constexpr auto DatasetNameRuleString("[A-Za-z0-9]+([A-Za-z0-9_\\.\\-]*)");

constexpr auto WareIDRuleString("[A-Za-z0-9]+([A-Za-z0-9_\\.\\-]*)");
constexpr auto WareIDRuleStringAndTpl("[A-Za-z0-9]+([A-Za-z0-9_@\\.\\-]*)");

constexpr auto FragmentNameRuleString("[A-Za-z]+([A-Za-z0-9_\\.\\-]*)");


//   authorized chars: a to z, A to Z, 0 to 9, -, ., _
//   must start by an alphabetic char
constexpr auto VariableNameRuleString("[A-Za-z]+([A-Za-z0-9_\\.\\-]*)");

constexpr auto UnitsClassNameRuleString("^[^;:#]+$");


/**
  Generates string from class and ID of a spatial unit using the "Class#ID" pattern.
  @param[in] Class The units class
  @param[in] ID The unit ID
  @return the generated string
*/
std::string OPENFLUID_API classIDToString(const openfluid::core::UnitsClass_t& Class, openfluid::core::UnitID_t ID);


/**
  Checks whether an alphanumeric name is valid or not.\n
  To be valid, a name must only contain alphanumeric characters.
  @param[in] Name the name to check
  @return true if the name is valid
*/
bool OPENFLUID_API isValidAlphaNumName(const std::string& Name);


/**
  Checks whether a dataset or project name is valid or not.\n
  To be valid, a name must only contain alphanumeric, '_', '-', '.' characters,
  and must begin by an alphanumeric character.
  @param[in] Name the name to check
  @return true if the name is valid
*/
bool OPENFLUID_API isValidDatasetName(const std::string& Name);


/**
  Checks whether a ware ID is valid or not.\n
  To be valid, a ware ID must only contain alphanumeric, '_', '-', '.' characters,
  and must begin by an alphanumeric character.
  @param[in] ID the ware ID to check
  @param[in] Template True to enable a template on the ware ID
  @return true if the ID is valid
*/
bool OPENFLUID_API isValidWareID(const openfluid::ware::WareID_t& ID, bool Template=false);


/**
  Checks whether a units classe name is valid or not.\n
  To be valid, a units class name must not contain any of ';', ':' and '#' characters,
  @param[in] UnitsClass the units class name to check
  @return true if the name is valid
*/
bool OPENFLUID_API isValidUnitsClassName(const openfluid::core::UnitsClass_t& Name);


/**
  Checks whether a variable name (without type) is valid or not.\n
  To be valid, a variable name must only contain alphanumeric, '_', '-' or '.' characters,
  and must begin by an alphanumeric character.
  @param[in] Name the variable name to check
  @return true if the name is valid
*/
bool OPENFLUID_API isValidVariableName(const openfluid::core::VariableName_t& Name);


/**
  Checks whether a variable sequence (without type) is valid or not.\n
  To be valid, a sequence must contain triplets of variable separated by ';'
  @return the regex as string (for compatibility reasons)
*/
std::string OPENFLUID_API getVariableSelectionRegExp();


/**
  Checks whether a variable name (with or without type) is valid or not.\n
  To be valid, a variable name must only contain alphanumeric, '_', '-' or '.' characters,
  must begin by an alphanumeric character, and can be suffixed by a valid type into square brackets.
  @param[in] Name the variable name to check
  @return true if the name is valid
*/
bool OPENFLUID_API isValidTypedVariableName(const openfluid::core::VariableName_t& Name);


/**
  Checks whether an attribute name is valid or not.\n
  To be valid, an attribute name must only contain alphanumeric, '_', '-' or '.' characters,
  and must begin by an alphanumeric character.
  @param[in] Name the attribute name to check
  @return true if the name is valid
*/
bool OPENFLUID_API isValidAttributeName(const openfluid::core::AttributeName_t& Name);


/**
  Checks and extracts variable name and type from a full variable name (with or without type).\n
  To be valid, a variable name must only contain alphanumeric, '_', '-' or '.' characters,
  must begin by an alphanumeric character, and can be suffixed by a valid type into square brackets.
  @param[in] Name the variable name to process
  @param[out] VarName the extracted variable name
  @param[out] VarType the extracted variable type
  @return true if the check and extraction are OK
*/
bool OPENFLUID_API extractVariableNameAndType(const openfluid::core::VariableName_t& Name,
                                              openfluid::core::VariableName_t& VarName,
                                              openfluid::core::Value::Type& VarType);


/**
  Builds a generator ID
  @param[in] VarPairs Pair of unit classes/variables to create the generator ID
  @param[in] StrDimType Dimension type of generator
*/
std::string OPENFLUID_API buildGeneratorID(const openfluid::tools::UnitVarPairs_t& VarPairs,
                                           const std::string& StrDimType);


} } // namespaces


#endif /* __OPENFLUID_TOOLS_IDHELPERS_HPP__ */
