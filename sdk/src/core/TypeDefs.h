/**
  \file TypeDefs.h
  \brief header of data types definition

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <vector>
#include <wx/hashmap.h>
#include "Vector.h"

namespace openfluid { namespace core {



/**
  Type definition for the ID of units
*/
typedef int UnitID;


/**
 Type definition for scalar values
 */
typedef double ScalarValue;

/**
 Type definition for vector values
 */
typedef Vector<ScalarValue> VectorValue;



/**
 Type definition for a time serie of scalar values
 */
typedef std::vector<ScalarValue> SerieOfScalarValue;


/**
 Type definition for a time serie of vectorvalues
 */
typedef std::vector<VectorValue> SerieOfVectorValue;


// =====================================================================
// =====================================================================
/**
  \anchor IDFloatMap
  Map associating a float value to a unit, indexed by its ID
*/
WX_DECLARE_HASH_MAP(openfluid::core::UnitID, float, wxIntegerHash, wxIntegerEqual, IDFloatMap);

/**
  \anchor IDDoubleMap
  Map associating a double precision value to a unit, indexed by its ID
*/
WX_DECLARE_HASH_MAP(openfluid::core::UnitID, double, wxIntegerHash, wxIntegerEqual, IDDoubleMap);

/**
  \anchor IDScalarValueMap
  Map associating an openfluid::core::ScalarValue value to a unit, indexed by its ID
*/
WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::ScalarValue, wxIntegerHash, wxIntegerEqual, IDScalarValueMap);

/**
  \anchor IDIntMap
  Map associating an integer value to a unit, indexed by its ID
*/
WX_DECLARE_HASH_MAP(openfluid::core::UnitID, int, wxIntegerHash, wxIntegerEqual, IDIntMap);

/**
  \anchor IDBoolMap
  Map associating a boolean value to a unit, indexed by its ID
*/
WX_DECLARE_HASH_MAP(openfluid::core::UnitID, bool, wxIntegerHash, wxIntegerEqual, IDBoolMap);

/**
  \anchor IDVectorValueMap
  Map associating an openfluid::core::VectorValue value to a unit, indexed by its ID
*/
WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::VectorValue, wxIntegerHash, wxIntegerEqual, IDVectorValueMap);

/**
  \anchor IDVectorValuePtrMap
  Map associating a pointer to an openfluid::core::VectorValue value to a unit, indexed by its ID
*/
WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::VectorValue*, wxIntegerHash, wxIntegerEqual, IDVectorValuePtrMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::SerieOfScalarValue, wxIntegerHash, wxIntegerEqual, IDSerieOfScalarValueMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::SerieOfScalarValue*, wxIntegerHash, wxIntegerEqual, IDSerieOfScalarValuePtrMap);




} } // namespaces


#endif /*TYPEDEFS_H_*/
