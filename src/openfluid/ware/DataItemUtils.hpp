/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file DataItemUtils.hpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/

#pragma once


#include <string>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace ware {

// contains minimal functions used by signature data item constructors


/**
  Transmits a given variable name to container without changing type
  @param[in] VO the string to use
  @param[in] V the string to populate
  @param[in] T the type to change (ignored)
  @return true always
*/
inline bool OPENFLUID_API identityExtractor(const std::string& VO, std::string& V, openfluid::core::Value::Type&  /*T*/)
{
  V = VO;
  return true;
}


// =====================================================================
// =====================================================================


/**
  Checks whether a string is empty
  @param[in] Str the string to check
  @return true if the string is not empty
*/
inline bool OPENFLUID_API isNonEmpty(const std::string& Str)
{
  return Str.length() > 0;
}


} } // namespaces
