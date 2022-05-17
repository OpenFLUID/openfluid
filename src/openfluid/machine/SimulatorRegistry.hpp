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
  @file SimulatorRegistry.hpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_SIMULATORREGISTRY_HPP__
#define __OPENFLUID_MACHINE_SIMULATORREGISTRY_HPP__


#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/ware/GeneratorSignature.hpp>
#include <openfluid/machine/WareRegistry.hpp>
#include <openfluid/utils/SingletonMacros.hpp>


namespace openfluid { namespace machine {


// =====================================================================
// =====================================================================


class OPENFLUID_API GeneratorSpecs
{
  public:

    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method = 
      openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::NONE;

    openfluid::core::UnitsClass_t UnitsClass;

    openfluid::core::VariableName_t VariableName;
    
    unsigned int VariableSize = 1;
};


// =====================================================================
// =====================================================================


class OPENFLUID_API SimulatorRegistry : public WareRegistry<openfluid::ware::SimulatorSignature>
{

  OPENFLUID_SINGLETON_DEFINITION(SimulatorRegistry)


  private:

    WaresByID_t m_Generators;
    

    SimulatorRegistry();

    WareContainer<openfluid::ware::SimulatorSignature> createWareContainer() const
    {
      return WareContainer<openfluid::ware::SimulatorSignature>(openfluid::ware::WareType::SIMULATOR);
    }

    WareContainer<openfluid::ware::SimulatorSignature> createGeneratorContainer() const
    {
      return WareContainer<openfluid::ware::SimulatorSignature>(openfluid::ware::WareType::GENERATOR);
    }


  public:

    bool addWare(const openfluid::ware::WareID_t& ID);

    void discoverWares(const std::string IDPattern = "");

    void discoverWares(bool WithGhosts, const std::string IDPattern = "");

    openfluid::ware::WareID_t addGenerator(const GeneratorSpecs& Specs);

    const WareContainer<openfluid::ware::SimulatorSignature>& 
    generatorContainer(const openfluid::ware::WareID_t& ID) const;

    const WareContainer<openfluid::ware::SimulatorSignature>& 
    wareOrGeneratorContainer(const openfluid::ware::WareID_t& ID) const;

    bool hasGenerator(const openfluid::ware::WareID_t& ID) const
    {
      return (m_Generators.find(ID) != m_Generators.end());
    }

    const WaresByID_t& generators() const
    {
      return m_Generators;
    }

    void clearGenerators()
    {
      m_Generators.clear();
    }

    void clear()
    {
      clearWares();
      clearGenerators();
    }

};


} } //namespaces

#endif /* __OPENFLUID_MACHINE_SIMULATORREGISTRY_HPP__ */
