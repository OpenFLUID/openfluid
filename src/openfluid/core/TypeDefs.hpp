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
  @file core/TypeDefs.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_CORE_TYPEDEFS_HPP__
#define __OPENFLUID_CORE_TYPEDEFS_HPP__


#include <vector>
#include <list>
#include <map>
#include <cstdint>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/StringValue.hpp>


namespace openfluid { namespace core {


/**
  Type definition for the ID of units

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Spatial domain"],
    "title" : "Unit ID",
    "text" : "openfluid::core::UnitID_t %%SEL_START%%UnitID%%SEL_END%%"
  }
  @endcond
*/
typedef unsigned int UnitID_t;

/**
  Type definition for the process order of units

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Spatial domain"],
    "title" : "Process order",
    "text" : "openfluid::core::PcsOrd_t %%SEL_START%%PcsOrd%%SEL_END%%"
  }
  @endcond
*/
typedef int PcsOrd_t;

/**
  Type definition for the class of units

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Spatial domain"],
    "title" : "Units class name",
    "text" : "openfluid::core::UnitsClass_t %%SEL_START%%ClassName%%SEL_END%%"
  }
  @endcond
*/
typedef std::string UnitsClass_t;

/**
  @deprecated Since version 2.1.0. Use openfluid::core::UnitsClass_t instead
*/
[[deprecated]] typedef UnitsClass_t UnitClass_t;

/**
  Type definition for name of an attribute

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Attributes"],
    "title" : "Attribute name",
    "text" : "openfluid::core::AttributeName_t %%SEL_START%%AttrName%%SEL_END%%"
  }
  @endcond
*/
typedef std::string AttributeName_t;

/**
  Type definition for name of a variable

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Variables"],
    "title" : "Variable name",
    "text" : "openfluid::core::VariableName_t %%SEL_START%%VarName%%SEL_END%%"
  }
  @endcond
*/
typedef std::string VariableName_t;

/**
  Type definition for a pair containing the unit class and the unit ID
*/
typedef std::pair<UnitsClass_t,UnitID_t> UnitClassID_t;

/**
  Type definition for a link between two units defined by their class and ID
*/
typedef std::pair<UnitClassID_t,UnitClassID_t> UnitsLink_t;


// =====================================================================
// =====================================================================


/**
 Type definition for a serie of DoubleValue
 */
typedef std::vector<DoubleValue> SerieOfDoubleValue;


/**
 Type definition for a serie of VectorValue
 */
typedef std::vector<VectorValue> SerieOfVectorValue;


// =====================================================================
// =====================================================================


/**
  Generic map container associating a unit ID to data
*/
template<class T>
struct IDMap
{
  typedef std::map<UnitID_t,T> Type;
};


/**
  Map associating a float value to a unit ID

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "ID..Map"],
    "title" : "ID to single precision (signed)",
    "text" : "openfluid::core::IDFloatMap %%SEL_START%%ValMap%%SEL_END%%"
  }
  @endcond
*/
typedef IDMap<float>::Type IDFloatMap;

/**
  Map associating a double precision value to a unit ID

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "ID..Map"],
    "title" : "ID to double precision (signed)",
    "text" : "openfluid::core::IDDoubleMap %%SEL_START%%ValMap%%SEL_END%%"
  }
  @endcond
*/
typedef IDMap<double>::Type IDDoubleMap;

/**
  Map associating a DoubleValue value to a unit ID

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "ID..Map"],
    "title" : "ID to DoubleValue",
    "text" : "openfluid::core::IDDoubleValueMap %%SEL_START%%ValMap%%SEL_END%%"
  }
  @endcond
*/
typedef IDMap<DoubleValue>::Type IDDoubleValueMap;

/**
  Map associating an integer value to a unit ID

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "ID..Map"],
    "title" : "ID to integer (signed)",
    "text" : "openfluid::core::IDIntMap %%SEL_START%%ValMap%%SEL_END%%"
  }
  @endcond
*/
typedef IDMap<int>::Type IDIntMap;

/**
  Map associating a boolean value to a unit ID

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "ID..Map"],
    "title" : "ID to boolean",
    "text" : "openfluid::core::IDBoolMap %%SEL_START%%ValMap%%SEL_END%%"
  }
  @endcond
*/
typedef IDMap<bool>::Type IDBoolMap;

/**
  Map associating an openfluid::core::VectorValue value to a unit ID

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "ID..Map"],
    "title" : "ID to VectorValue",
    "text" : "openfluid::core::IDVectorValueMap %%SEL_START%%ValMap%%SEL_END%%"
  }
  @endcond
*/
typedef IDMap<VectorValue>::Type IDVectorValueMap;

/**
  Map associating a pointer to an openfluid::core::VectorValue value to a unit ID

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "ID..Map"],
    "title" : "ID to pointer to VectorValue",
    "text" : "openfluid::core::IDVectorValuePtrMap %%SEL_START%%ValMap%%SEL_END%%"
  }
  @endcond
*/
typedef IDMap<VectorValue*>::Type IDVectorValuePtrMap;

typedef IDMap<SerieOfDoubleValue>::Type IDSerieOfDoubleValueMap;

typedef IDMap<SerieOfDoubleValue*>::Type IDSerieOfDoubleValuePtrMap;


// =====================================================================
// =====================================================================


/**
  Type for raw time (seconds since 4713BC)
*/
typedef std::uint64_t RawTime_t;

/**
  Type for time indexes (in seconds)

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Time"],
    "title" : "Time index",
    "text" : "openfluid::core::TimeIndex_t %%SEL_START%%TimeIndex%%SEL_END%%"
  }
  @endcond
*/
typedef std::uint64_t TimeIndex_t;

/**
  Type for durations (in seconds)

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Types", "Time"],
    "title" : "Duration",
    "text" : "openfluid::core::Duration_t %%SEL_START%%Duration%%SEL_END%%"
  }
  @endcond
*/
typedef std::uint64_t Duration_t;


} } // namespaces


#endif  /* __OPENFLUID_CORE_TYPEDEFS_HPP__ */
