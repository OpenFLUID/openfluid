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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_TOOLS_IDHELPERS_HPP__
#define __OPENFLUID_TOOLS_IDHELPERS_HPP__


#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {

/**
  Checks whether a ware ID is valid or not.\n
  To be valid, a ware ID must only contain alphanumeric, '_', '-', '.' characters,
  and must begin by an alphanumeric character.
  @param[in] ID the ware ID to check
  @return true if the ID is valid
*/
bool OPENFLUID_API isValidWareID(const openfluid::ware::WareID_t& ID);


/**
  Checks whether a variable name (without type) is valid or not.\n
  To be valid, a variable name must only contain alphanumeric, '_', '-' or '.' characters,
  and must begin by an alphanumeric character.
  @param[in] Name the variable name to check
  @return true if the name is valid
*/
bool OPENFLUID_API isValidVariableName(const openfluid::core::VariableName_t& Name);


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
bool OPENFLUID_API extractVarableNameAndType(const openfluid::core::VariableName_t& Name,
                                             openfluid::core::VariableName_t& VarName,
                                             openfluid::core::Value::Type& VarType);

} } // namespaces


#endif /* __OPENFLUID_TOOLS_IDHELPERS_HPP__ */
