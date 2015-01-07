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
  @file

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#ifndef __OPENFLUID_CORE_TYPEDEFS_HPP__
#define __OPENFLUID_CORE_TYPEDEFS_HPP__


#include <vector>
#include <list>
#include <map>
#include <openfluid/dllexport.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/StringValue.hpp>


namespace openfluid { namespace core {



/**
  Type definition for the ID of units
*/
typedef unsigned int UnitID_t;

/**
  Type definition for the process order of units
*/
typedef int PcsOrd_t;

/**
  Type definition for the class of units
*/
typedef std::string UnitClass_t;

/**
  Type definition for name of an attribute
*/
typedef std::string AttributeName_t;

/**
  Type definition for name of a variable
*/
typedef std::string VariableName_t;


typedef std::pair<UnitClass_t,UnitID_t> UnitClassID_t;

typedef std::pair<UnitClassID_t,UnitClassID_t> UnitsLink_t;


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
*/
typedef IDMap<float>::Type IDFloatMap;

/**
  Map associating a double precision value to a unit ID
*/
typedef IDMap<double>::Type IDDoubleMap;

/**
  Map associating a DoubleValue value to a unit ID
*/
typedef IDMap<DoubleValue>::Type IDDoubleValueMap;

/**
  Map associating an integer value to a unit ID
*/
typedef IDMap<int>::Type IDIntMap;

/**
  Map associating a boolean value to a unit ID
*/
typedef IDMap<bool>::Type IDBoolMap;

/**
  Map associating an openfluid::core::VectorValue value to a unit ID
*/
typedef IDMap<VectorValue>::Type IDVectorValueMap;

/**
  Map associating a pointer to an openfluid::core::VectorValue value to a unit ID
*/
typedef IDMap<VectorValue*>::Type IDVectorValuePtrMap;

typedef IDMap<SerieOfDoubleValue>::Type IDSerieOfDoubleValueMap;

typedef IDMap<SerieOfDoubleValue*>::Type IDSerieOfDoubleValuePtrMap;




} } // namespaces


#endif  /* __OPENFLUID_CORE_TYPEDEFS_HPP__ */
