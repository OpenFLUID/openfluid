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
  @file GeneratorSignature.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/machine/GeneratorSignature.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace machine {


GeneratorSignature::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod M,
                                       const openfluid::tools::UnitVarTriplets_t& V,
                                       const openfluid::core::Value::Type VT,
                                       const openfluid::core::Dimensions& VD):
    SimulatorSignature(), Method(M), VariableTriplets(V), VariableType(VT), VariableDimensions(VD)
{
  const auto VarPairs = openfluid::tools::deduceVarPairs(VariableTriplets);
  ID = openfluid::tools::buildGeneratorID(VarPairs, VariableDimensions.strType());

  switch (Method)
  {
    case openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED:
      setFixedInfo();
      break;
    case openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM:
      setRandomInfo();
      break;
    case openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP:
      setInterpolationInfo();
      break;
    case openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT:
      setInjectionInfo();
      break;
    case openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECTMULTICOL:
      setInjectionMulticolInfo();
      break;
    default:
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Invalid generator type");
      break;
  }

  for (const auto& VarPair : VarPairs)
  {
    std::string TypedVarName = VarPair.VariableName;
    std::string VarDimType = VariableDimensions.strType();
    if (VarDimType == "scalar")
    {
      TypedVarName += "["+openfluid::core::Value::getStringFromValueType(VariableType)+"]";
    }
    else
    {
      TypedVarName += "["+VarDimType+"]";
    } 
    SimulatorHandledData.ProducedVars.push_back(openfluid::ware::SignatureSpatialDataItem(TypedVarName,
                                                                                 VarPair.UnitsClass,"",""));
  }
}


// =====================================================================
// =====================================================================


void GeneratorSignature::setFixedInfo()
{
  Name = "Fixed values";
  Description = "Generates a constant value";

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("fixedvalue","Value to produce","-"));

  HandledData.UsedParams.push_back(
      openfluid::ware::SignatureDataItem("deltat","DeltaT to use instead of the default DeltaT","s"));
}


// =====================================================================
// =====================================================================


void GeneratorSignature::setRandomInfo()
{
  Name = "Random values";
  Description = "Generates a random value in a range";

  if (VariableType != openfluid::core::Value::Type::BOOLEAN)
  {
    HandledData.RequiredParams.push_back(
        openfluid::ware::SignatureDataItem("min","Lower bound of the random range for the value to produce","-"));

    HandledData.RequiredParams.push_back(
        openfluid::ware::SignatureDataItem("max","Upper bound of the random range for the value to produce","-"));
  }


  HandledData.UsedParams.push_back(
    openfluid::ware::SignatureDataItem("seed", "Random seed as integer. if negative, seed will change each time","-"));

  if (!VariableDimensions.isScalar())
  {
    HandledData.UsedParams.push_back(
        openfluid::ware::SignatureDataItem("identicalcells","Vector cells contain identical values","bool"));
  }

  HandledData.UsedParams.push_back(
      openfluid::ware::SignatureDataItem("deltat","DeltaT to use instead of the default DeltaT","s"));
}


// =====================================================================
// =====================================================================


void GeneratorSignature::setInterpolationInfo()
{
  Name = "Values from file interpolation";
  Description = "Generates a time-interpolated value from given data series";


  HandledData.UsedParams.push_back(
      openfluid::ware::SignatureDataItem("thresholdmin","Threshold min value", "-"));

  HandledData.UsedParams.push_back(
      openfluid::ware::SignatureDataItem("thresholdmax","Threshold max value", "-"));

  HandledData.UsedParams.push_back(
      openfluid::ware::SignatureDataItem("keeptmpdir","Keep temporary directory if true (default is false)","-"));

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("sources","Data sources filename for the value to produce","-"));

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("distribution","Distribution filename for the value to produce","-"));
}


// =====================================================================
// =====================================================================


void GeneratorSignature::setInjectionInfo()
{
  Name = "Values from file injection";
  Description = "Generates an injected value -no time interpolation- from given data series";


  HandledData.UsedParams.push_back(openfluid::ware::SignatureDataItem("thresholdmin","Threshold min value", "-"));

  HandledData.UsedParams.push_back(openfluid::ware::SignatureDataItem("thresholdmax","Threshold max value", "-"));

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("sources","Data sources filename for the value to produce","-"));

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("distribution","Distribution filename for the value to produce","-"));
}


// =====================================================================
// =====================================================================


void GeneratorSignature::setInjectionMulticolInfo()
{
  Name = "Values from file injection for several values";
  Description = "Generates injected values -no time interpolation- from given data series with selected columns:\n";

  // Add custom information about selection for this generator
  for (const auto& VT : VariableTriplets)
  {
    Description += "- "+VT.getClassIDVarString() +"\n";
  }

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("datafile","Data file name for the values to produce","-"));
}


} } //namespaces
