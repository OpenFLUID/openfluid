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
  @file Dimensions.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <algorithm>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/Dimensions.hpp>
#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace core {


Dimensions::Dimensions() : Type(DimensionType::SCALAR), Cols(1), Rows(1)
{
}


// =====================================================================
// =====================================================================


Dimensions::Dimensions(unsigned long Size): Type(DimensionType::VECTOR), Cols(1), Rows(Size)
{
}


// =====================================================================
// =====================================================================


Dimensions::Dimensions(unsigned long ColsNbr, unsigned long RowsNbr) : 
  Type(DimensionType::MATRIX), Cols(ColsNbr),   Rows(RowsNbr)
{
}


// =====================================================================
// =====================================================================


Dimensions::Dimensions(std::string SerializedVariableSize)
{
  applyDimensions(SerializedVariableSize);
}


// =====================================================================
// =====================================================================


void Dimensions::applyDimensions(std::string SerializedVariableSize)
{
  Rows = 1;
  Cols = 1;
  
  if (SerializedVariableSize[0] != s_BeginningSymbol)
  {
    // compatibility with previous syntax S=1 <=> SCALAR, S>1 <=> VECTOR
    if (SerializedVariableSize == "" || SerializedVariableSize == "1")
    {
      Type = DimensionType::SCALAR;
    }
    else
    {
      applyDimensions(s_BeginningSymbol+SerializedVariableSize+s_EndSymbol);
    }
  }
  else 
  {
    std::string TrimmedStrDims = SerializedVariableSize.substr(1, SerializedVariableSize.size()-2);
    unsigned int SplittedGroups = std::count(TrimmedStrDims.begin(), TrimmedStrDims.end(), ',');
    if (SplittedGroups > 1)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
              "generator dimensions parsing failure");
    }
    else if (SplittedGroups == 1)
    {
      Type = DimensionType::MATRIX;
      std::size_t P = TrimmedStrDims.find(s_SeparatorSymbol);
      
      try
      {
        // Not using openfluid::tools::split since not reachable from core namespace
        Cols = std::stoi(TrimmedStrDims.substr(0, P));
        Rows = std::stoi(TrimmedStrDims.substr(P));
      }
      catch(const std::exception& e)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "generator dimensions parsing failure");
      }
    }
    else
    {
      Type = DimensionType::VECTOR;
      try
      {
        Rows = std::stoi(TrimmedStrDims);
      }
      catch(const std::exception& e)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "generator dimensions parsing failure");
      }
    }
  }
}


// =====================================================================
// =====================================================================


std::string Dimensions::strType() const
{
  switch (Type)
  {
    case DimensionType::SCALAR:
      return "scalar";
    case DimensionType::VECTOR:
      return "vector";
    case DimensionType::MATRIX:
      return "matrix";
  }
  return "";
}


// =====================================================================
// =====================================================================


bool Dimensions::isScalar() const
{
  return (Type == DimensionType::SCALAR);
}


// =====================================================================
// =====================================================================


bool Dimensions::isVector() const
{
  return (Type == DimensionType::VECTOR);
}


// =====================================================================
// =====================================================================


bool Dimensions::isMatrix() const
{
  return (Type == DimensionType::MATRIX);
}


// =====================================================================
// =====================================================================


const std::string Dimensions::getSerializedVariableSize() const
{
  switch (Type)
  {
    case DimensionType::SCALAR:
      return "1";
    case DimensionType::VECTOR:
      return s_BeginningSymbol+std::to_string(Rows)+s_EndSymbol;
    case DimensionType::MATRIX:
      return s_BeginningSymbol+std::to_string(Cols)+s_SeparatorSymbol+std::to_string(Rows)+s_EndSymbol;
  }
  return "1";
}

} } // namespaces
