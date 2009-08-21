/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file TypeDefs.h
  \brief header of data types definition

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <vector>
#include <list>
#include <map>
#include "Vector.h"


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
  Type definition for name of input data
*/
typedef std::string InputDataName_t;

/**
  Type definition for name of a variable
*/
typedef std::string VariableName_t;

/**
  Type definition for a time step value
*/
typedef unsigned int TimeStep_t;


typedef std::pair<UnitClass_t,UnitID_t> UnitClassID_t;

typedef std::pair<UnitClassID_t,UnitClassID_t> UnitsLink_t;


/**
 Type definition for scalar values
 */
typedef double ScalarValue;

/**
 Type definition for vector values. This type is based on the openfluid::core::Vector template.
 \sa openfluid::core::Vector

 <i>usage examples:</i>
 \code
  // create an empty vector
  openfluid::core::VectorValue TmpValue;

  // give a size of 10 to this vector
  TmpValue = openfluid::core::VectorValue(10);

  // give a size of 10 to this vector and fill it with a value of2
  TmpValue = openfluid::core::VectorValue(10,2);

  // read the value at index 6 in put it in a variable MyVar
  MyVar = TmpValue[6];
  // or
  MyVar = TmpValue.at(6);

  // set or modify the value 15 at index 3
  TmpValue.setElement(3,15);
 \endcode

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


typedef std::string FuncParamKey_t;
typedef std::string FuncParamValue_t;

/**
  Hash table for parameters (distributed properties, distributed initial conditions, ...)
*/
typedef std::map<FuncParamKey_t,FuncParamValue_t> FuncParamsMap_t;


// =====================================================================
// =====================================================================
/**
  Map associating a float value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,float> IDFloatMap;

/**
  Map associating a double precision value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,double> IDDoubleMap;

/**
  Map associating an openfluid::core::ScalarValue value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,ScalarValue> IDScalarValueMap;

/**
  Map associating an integer value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,int> IDIntMap;

/**
  Map associating a boolean value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,bool> IDBoolMap;

/**
  Map associating an openfluid::core::VectorValue value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,VectorValue> IDVectorValueMap;

/**
  Map associating a pointer to an openfluid::core::VectorValue value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,VectorValue*> IDVectorValuePtrMap;

typedef std::map<UnitID_t,SerieOfScalarValue> IDSerieOfScalarValueMap;

typedef std::map<UnitID_t,SerieOfScalarValue*> IDSerieOfScalarValuePtrMap;




} } // namespaces


#endif /*TYPEDEFS_H_*/
