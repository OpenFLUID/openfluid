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
*/


#include <openfluid/ware/GeneratorSignature.hpp>


namespace openfluid { namespace ware {


GeneratorSignature::GeneratorSignature(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GeneratorMethod):
    SimulatorSignature()
{
  m_GeneratorMethod = GeneratorMethod;
  ID = openfluid::fluidx::GeneratorDescriptor::getGeneratorName(
      m_GeneratorMethod);

  switch (m_GeneratorMethod)
  {
    case openfluid::fluidx::GeneratorDescriptor::Fixed:
      setFixedInfo();
      break;
    case openfluid::fluidx::GeneratorDescriptor::Random:
      setRandomInfo();
      break;
    case openfluid::fluidx::GeneratorDescriptor::Interp:
      setInterpInfo();
      break;
    case openfluid::fluidx::GeneratorDescriptor::Inject:
      setInjectInfo();
      break;
    default:
      std::cerr
          << "GeneratorSignature::GeneratorSignature : unknown Generator method"
          << std::endl;
      break;
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

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("min","Lower bound of the random range for the value to produce","-"));

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("max","Upper bound of the random range for the value to produce","-"));

  HandledData.UsedParams.push_back(
      openfluid::ware::SignatureDataItem("deltat","DeltaT to use instead of the default DeltaT","s"));
}


// =====================================================================
// =====================================================================


void GeneratorSignature::setInterpInfo()
{
  Name = "Values from file interpolation";
  Description = "Generates a time-interpolated value from given data series";


  HandledData.UsedParams.push_back(
      openfluid::ware::SignatureDataItem("thresholdmin","Threshold min value", "-"));

  HandledData.UsedParams.push_back(
      openfluid::ware::SignatureDataItem("thresholdmax","Threshold max value", "-"));

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("sources","Data sources filename for the value to produce","-"));

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("distribution","Distribution filename for the value to produce","-"));


  HandledData.RequiredExtraFiles.push_back("Data sources for the value to produce, from \"sources\" parameter");

  HandledData.RequiredExtraFiles.push_back("Distribution for the value to produce, from \"distribution\" parameter");
}


// =====================================================================
// =====================================================================


void GeneratorSignature::setInjectInfo()
{
  Name = "Values from file injection";
  Description = "Generates an injected value -no time interpolation- from given data series";


  HandledData.UsedParams.push_back(openfluid::ware::SignatureDataItem("thresholdmin","Threshold min value", "-"));

  HandledData.UsedParams.push_back(openfluid::ware::SignatureDataItem("thresholdmax","Threshold max value", "-"));

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("sources","Data sources filename for the value to produce","-"));

  HandledData.RequiredParams.push_back(
      openfluid::ware::SignatureDataItem("distribution","Distribution filename for the value to produce","-"));


  HandledData.RequiredExtraFiles.push_back("Data sources for the value to produce, from \"sources\" parameter");

  HandledData.RequiredExtraFiles.push_back("Distribution for the value to produce, from \"distribution\" parameter");
}


} } //namespaces
