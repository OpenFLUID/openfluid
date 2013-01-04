/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file GeneratorSignature.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "GeneratorSignature.hpp"

// =====================================================================
// =====================================================================


GeneratorSignature::GeneratorSignature(
    openfluid::base::GeneratorDescriptor::GeneratorMethod GeneratorMethod)
{
  m_GeneratorMethod = GeneratorMethod;

  switch (m_GeneratorMethod)
  {
    case openfluid::base::GeneratorDescriptor::Fixed:
      setFixedInfo();
      break;
    case openfluid::base::GeneratorDescriptor::Random:
      setRandomInfo();
      break;
    case openfluid::base::GeneratorDescriptor::Interp:
      setInterpInfo();
      break;
    case openfluid::base::GeneratorDescriptor::Inject:
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
  ID = "Fixed Generator";
  Name = "Fixed Generator";
  Description = "Generates a constant value";

  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("fixedvalue", "",
          "Value to produce", "-"));
}

// =====================================================================
// =====================================================================


void GeneratorSignature::setRandomInfo()
{
  ID = "Random Generator";
  Name = "Random Generator";
  Description = "Generates a random value in a range";

  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("min", "",
          "Lower bound of the random range for the value to produce", "-"));
  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("max", "",
          "Upper bound of the random range for the value to produce", "-"));
}

// =====================================================================
// =====================================================================


void GeneratorSignature::setInterpInfo()
{
  ID = "Interp Generator";
  Name = "Interpolation Generator";
  Description = "Generates a time-interpolated value from given data series";

  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("thresholdmin", "",
          "Threshold min value", "-"));
  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("thresholdmax", "",
          "Threshold max value", "-"));
  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("sources", "",
          "Data sources filename for the value to produce", "-"));
  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("distribution", "",
          "Distribution filename for the value to produce", "-"));
}

// =====================================================================
// =====================================================================


void GeneratorSignature::setInjectInfo()
{
  ID = "Inject Generator";
  Name = "Injection Generator";
  Description = "Generates an injected value -no time interpolation- from given data series";

  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("thresholdmin", "",
          "Threshold min value", "-"));
  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("thresholdmax", "",
          "Threshold max value", "-"));
  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("sources", "",
          "Data sources filename for the value to produce", "-"));
  HandledData.FunctionParams.push_back(
      openfluid::ware::SignatureHandledDataItem("distribution", "",
          "Distribution filename for the value to produce", "-"));
}
