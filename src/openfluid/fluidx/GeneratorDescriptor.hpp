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
 */


#ifndef __OPENFLUID_FLUIDX_GENERATORDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_GENERATORDESCRIPTOR_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>


namespace openfluid { namespace fluidx {


class OPENFLUID_API GeneratorDescriptor : public ModelItemDescriptor
{
  public:
    enum GeneratorMethod { NoGenMethod, Fixed, Random, Interp, Inject };


  private:

    openfluid::core::VariableName_t m_VarName;

    openfluid::core::UnitsClass_t m_UnitsClass;

    GeneratorMethod m_GenMethod;

    unsigned int m_VarSize;

    std::string m_GeneratedID;


  public:

    GeneratorDescriptor();

    GeneratorDescriptor(openfluid::core::VariableName_t VarName, openfluid::core::UnitsClass_t UnitsClass,
                        GeneratorMethod GenMethod, unsigned int VarSize=1);

    openfluid::core::VariableName_t getVariableName() const;

    openfluid::core::UnitsClass_t getUnitsClass() const;

    /**
      @deprecated Since version 2.1.0. Use openfluid::fluidx::GeneratorDescriptor::getUnitsClass() const instead
    */
    openfluid::core::UnitsClass_t getUnitClass() const OPENFLUID_DEPRECATED
    { return getUnitsClass(); };

    GeneratorMethod getGeneratorMethod() const;

    inline bool isVectorVariable() const { return (m_VarSize > 1); };

    inline bool isScalarVariable() const { return (!isVectorVariable()); };

    inline unsigned int getVariableSize() const { return m_VarSize; };

    inline  openfluid::core::Value::Type getVariableType() const
    {
      if (isVectorVariable())
        return openfluid::core::Value::VECTOR;
      else
        return openfluid::core::Value::DOUBLE;
    };

    inline std::string getGeneratedID() const { return m_GeneratedID; };

    static std::string getGeneratorName(GeneratorDescriptor::GeneratorMethod GenMethod);
};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_GENERATORDESCRIPTOR_HPP__ */
