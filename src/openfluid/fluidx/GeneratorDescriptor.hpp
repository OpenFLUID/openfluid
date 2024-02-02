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


#include <openfluid/core/Dimensions.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/tools/VarHelpers.hpp>


namespace openfluid { namespace fluidx {


class OPENFLUID_API GeneratorDescriptor : public ModelItemDescriptor
{
  public:
    enum class GeneratorMethod { NONE, FIXED, RANDOM, INTERP, INJECT, INJECTMULTICOL };


  private:

    openfluid::tools::UnitVarTriplets_t m_VarTriplets;

    GeneratorMethod m_GenMethod;

    openfluid::core::Value::Type m_VarType;

    openfluid::core::Dimensions m_VarDimensions;

    std::string m_GeneratedID;


  public:

    GeneratorDescriptor() = delete;

    virtual ~GeneratorDescriptor()
    { }

    GeneratorDescriptor(openfluid::tools::UnitVarTriplets_t VarTriplets,
                        GeneratorMethod GenMethod, 
                        openfluid::core::Value::Type VarType=openfluid::core::Value::Type::NONE, 
                        openfluid::core::Dimensions VarDimensions=openfluid::core::Dimensions());
    
    openfluid::core::VariableName_t getVariableName() const;

    openfluid::core::UnitsClass_t getUnitsClass() const;

    const openfluid::tools::UnitVarTriplets_t& getVariableTriplets() const;

    GeneratorMethod getGeneratorMethod() const;

    openfluid::core::Dimensions getVariableDimensions() const
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
