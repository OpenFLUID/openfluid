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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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

    openfluid::core::UnitClass_t m_UnitClass;

    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod m_GenMethod;

    unsigned int m_VarSize;


  public:

    Generator();

    ~Generator();


    void setInfos(openfluid::core::VariableName_t VarName, openfluid::core::UnitClass_t UnitClass,
                  openfluid::fluidx::GeneratorDescriptor::GeneratorMethod GenMethod, unsigned int VarSize=1)
      { m_VarName = VarName; m_UnitClass = UnitClass; m_GenMethod = GenMethod; m_VarSize = VarSize; };

    openfluid::core::VariableName_t getVariableName() const { return m_VarName; };

    openfluid::core::UnitClass_t getUnitClass() const { return m_UnitClass; };

    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod getGeneratorMethod() const { return m_GenMethod; };

    inline bool isVectorVariable() const { return (m_VarSize > 1); };

    inline bool isScalarVariable() const { return (!isVectorVariable()); };

    inline unsigned int getVariableSize() const { return m_VarSize; };


    virtual void prepareData() {};

};

} } //namespaces



#endif /* __OPENFLUID_MACHINE_GENERATOR_HPP__ */
