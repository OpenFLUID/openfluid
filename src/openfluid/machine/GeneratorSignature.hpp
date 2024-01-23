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
  @file GeneratorSignature.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */

#ifndef __OPENFLUID_MACHINE_GENERATORSIGNATURE_HPP__
#define __OPENFLUID_MACHINE_GENERATORSIGNATURE_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API GeneratorSignature: public openfluid::ware::SimulatorSignature
{
  private:

    void setFixedInfo();

    void setRandomInfo();

    void setInterpolationInfo();

    void setInjectionInfo(); 


  public:

    const openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method;

    const openfluid::core::UnitsClass_t UnitsClass;

    const openfluid::core::VariableName_t VariableName;

    const openfluid::core::Value::Type VariableType;

    const openfluid::fluidx::DataDimensions VariableDimensions;

    GeneratorSignature(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod M,
                       const openfluid::core::UnitsClass_t& U, 
                       const openfluid::core::VariableName_t& VN, 
                       const openfluid::core::Value::Type VT=openfluid::core::Value::DOUBLE, 
                       const openfluid::fluidx::DataDimensions& VD=openfluid::fluidx::DataDimensions());

    virtual ~GeneratorSignature() = default;

    openfluid::ware::WareType getType() const
    {
      return openfluid::ware::WareType::GENERATOR;
    }

};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_GENERATORSIGNATURE_HPP__ */
