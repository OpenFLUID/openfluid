#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <vector>

namespace openfluid { namespace core {



typedef int UnitID;


/**
 Type definition for scalar values
 */
typedef double ScalarValue;


/**
 Type definition for vector values
 */
typedef std::vector<ScalarValue> VectorValue;

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

WX_DECLARE_HASH_MAP(openfluid::core::UnitID, openfluid::core::VectorValue*, wxIntegerHash, wxIntegerEqual, IDVectorValueMap);



} } // namespaces


#endif /*TYPEDEFS_H_*/
