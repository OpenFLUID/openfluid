#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <vector>

namespace mhydasdk { namespace core {



typedef int HOID;


/**
 Type definition for scalar values 
 */
typedef double MHYDASScalarValue;

/**
 Type definition for distributed properties values 
 */
typedef double PropertyValue;


/**
 Type definition for vector values 
 */
typedef std::vector<MHYDASScalarValue> MHYDASVectorValue;

/**
 Type definition for a time serie of scalar values 
 */
typedef std::vector<MHYDASScalarValue> SerieOfMHYDASScalarValue;


/**
 Type definition for a time serie of vectorvalues 
 */
typedef std::vector<MHYDASVectorValue> SerieOfMHYDASVectorValue;



} } // namespaces


#endif /*TYPEDEFS_H_*/
