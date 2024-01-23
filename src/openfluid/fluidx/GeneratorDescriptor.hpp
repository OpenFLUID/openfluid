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
  @file GeneratorDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_FLUIDX_GENERATORDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_GENERATORDESCRIPTOR_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace fluidx {


struct DataDimensions
{
  enum class DType {SCALAR, VECTOR, MATRIX};

  DType Type;
  
  unsigned long Cols, Rows;

  void applyDimensions(std::string SerializedVariableSize)
  {
    Rows = 1;
    Cols = 1;
    if (SerializedVariableSize[0] != '[') // compatibility with previous syntax S=1 <=> SCALAR, S>1 <=> VECTOR
    {
      if (SerializedVariableSize == "" || SerializedVariableSize == "1")
      {
        Type = DType::SCALAR;
      }
      else
      {
        applyDimensions("["+SerializedVariableSize+"]");
      }
    }
    else 
    {
      std::string TrimmedStrDims = SerializedVariableSize.substr(1, SerializedVariableSize.size()-2);
      if (TrimmedStrDims.find(",") != std::string::npos)
      {
        Type = DType::MATRIX;
        auto Dims = openfluid::tools::split(TrimmedStrDims, ",");
        try
        {
          Cols = std::stoi(Dims[0]);
          Rows = std::stoi(Dims[1]);
        }
        catch(const std::exception& e)
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "generator dimensions parsing failure");
        }
      }
      else
      {
        Type = DType::VECTOR;
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

  DataDimensions() : Type(DType::SCALAR), Cols(1), Rows(1)
  {
  }

  DataDimensions(unsigned long Size): Type(DType::VECTOR), Cols(1), Rows(Size)
  {
  }

  DataDimensions(unsigned long ColsNbr, unsigned long RowsNbr) : Type(DType::MATRIX), Cols(ColsNbr), Rows(RowsNbr)
  {
  }

  DataDimensions(std::string SerializedVariableSize)
  {
    applyDimensions(SerializedVariableSize);
  }

  std::string strType() const
  {
    switch (Type)
    {
      case DType::SCALAR:
        return "scalar";
      case DType::VECTOR:
        return "vector";
      case DType::MATRIX:
        return "matrix";
    }
    return "";
  }

  bool isScalar() const
  {
    return (Type == DType::SCALAR);
  }
  
  bool isVector() const
  {
    return (Type == DType::VECTOR);
  }
  
  bool isMatrix() const
  {
    return (Type == DType::MATRIX);
  }

  const std::string getSerializedVariableSize() const
  {
    switch (Type)
    {
      case DType::SCALAR:
        return "1";
      case DType::VECTOR:
        return "["+std::to_string(Rows)+"]";
      case DType::MATRIX:
        return "["+std::to_string(Cols)+","+std::to_string(Rows)+"]";
    }
    return "1";
  }
};


class OPENFLUID_API GeneratorDescriptor : public ModelItemDescriptor
{
  public:
    enum class GeneratorMethod { NONE, FIXED, RANDOM, INTERP, INJECT };


  private:

    openfluid::core::VariableName_t m_VarName;

    openfluid::core::UnitsClass_t m_UnitsClass;

    GeneratorMethod m_GenMethod;

    openfluid::core::Value::Type m_VarType;

    DataDimensions m_VarDimensions;

    std::string m_GeneratedID;


  public:

    GeneratorDescriptor() = delete;

    virtual ~GeneratorDescriptor()
    { }

    GeneratorDescriptor(openfluid::core::VariableName_t VarName, openfluid::core::UnitsClass_t UnitsClass,
                        GeneratorMethod GenMethod, 
                        openfluid::core::Value::Type VarType=openfluid::core::Value::Type::DOUBLE, 
                        DataDimensions VarDimensions=DataDimensions());

    openfluid::core::VariableName_t getVariableName() const;

    openfluid::core::UnitsClass_t getUnitsClass() const;

    GeneratorMethod getGeneratorMethod() const;

    DataDimensions getVariableDimensions() const
    {
      return m_VarDimensions;
    }

    std::size_t getVariableSize() const
    {
      return m_VarDimensions.Cols*m_VarDimensions.Rows;
    }

    openfluid::core::Value::Type getVariableType() const;
};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_GENERATORDESCRIPTOR_HPP__ */
