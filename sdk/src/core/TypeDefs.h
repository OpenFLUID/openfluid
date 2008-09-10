/**
  \file TypeDefs.h
  \brief header of data types definition

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <vector>
#include <wx/hashmap.h>
#include "ArrayValue.h"

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
typedef ArrayValue<ScalarValue> VectorValue;



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

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, float, wxIntegerHash, wxIntegerEqual, IDFloatMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, double, wxIntegerHash, wxIntegerEqual, IDDoubleMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::ScalarValue, wxIntegerHash, wxIntegerEqual, IDScalarValueMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, int, wxIntegerHash, wxIntegerEqual, IDIntMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, bool, wxIntegerHash, wxIntegerEqual, IDBoolMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::VectorValue, wxIntegerHash, wxIntegerEqual, IDVectorValueMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::VectorValue*, wxIntegerHash, wxIntegerEqual, IDVectorValuePtrMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::SerieOfScalarValue, wxIntegerHash, wxIntegerEqual, IDSerieOfScalarValueMap);

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::SerieOfScalarValue*, wxIntegerHash, wxIntegerEqual, IDSerieOfScalarValuePtrMap);




} } // namespaces


#endif /*TYPEDEFS_H_*/
