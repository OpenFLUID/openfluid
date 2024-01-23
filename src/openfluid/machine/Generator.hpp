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
  @file Generator.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_MACHINE_GENERATOR_HPP__
#define __OPENFLUID_MACHINE_GENERATOR_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API Generator : public openfluid::ware::PluggableSimulator
{
  protected:

    openfluid::core::VariableName_t m_VarName;

    openfluid::core::UnitsClass_t m_UnitsClass;

    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod m_GenMethod;

    openfluid::fluidx::DataDimensions m_VarDimensions;


  public:

    Generator();

    virtual ~Generator()
    { }


    void setInfos(openfluid::core::VariableName_t VarName, openfluid::core::UnitsClass_t UnitsClass,
                  openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GenMethod, 
                  openfluid::fluidx::DataDimensions VarDimensions=openfluid::fluidx::DataDimensions())
    {
      m_VarName = VarName;
      m_UnitsClass = UnitsClass;
      m_GenMethod = GenMethod;
      m_VarDimensions = VarDimensions;
    }

    openfluid::core::VariableName_t getVariableName() const
    {
      return m_VarName;
    }

    openfluid::core::UnitsClass_t getUnitsClass() const
    {
      return m_UnitsClass;
    }

    /**
      @deprecated Since version 2.1.0. Use openfluid::machine::Generator::getUnitsClass() const instead
    */
    [[deprecated]] openfluid::core::UnitsClass_t getUnitClass() const
    {
      return m_UnitsClass;
    }

    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod getGeneratorMethod() const
    {
      return m_GenMethod;
    }


    virtual void prepareData()
    {

    }
};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_GENERATOR_HPP__ */
