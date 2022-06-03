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

    /**
      The method used by the generator to produce the variable
    */
    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method = 
      openfluid::fluidx::GeneratorDescriptor::GeneratorMethod::NONE;

    /**
      The units class on which the genrators produces the variable
    */
    openfluid::core::UnitsClass_t UnitsClass;

    /**
      The name of the produced variable
    */
    openfluid::core::VariableName_t VariableName;
    
    /**
      The size of the produced variable. if > 1, the variabble is a vector of double
    */
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

    /**
      Loads a simulator ware and adds it in the registry if everything went fine
      @param[in] ID the ID of the ware to load
      @return true if the wares is successfully added
    */
    bool addWare(const openfluid::ware::WareID_t& ID);

    /**
      Automatically discovers simulators wares (plugins and ghosts) and adds it in the registry, 
      optionally filtered on IDs using a given pattern
      @param[in] IDPattern the pattern to filter the wares on ID. Default is empty (no filtering)
    */
    void discoverWares(const std::string IDPattern = "");

    /**
      Automatically discovers simulators wares (plugins and optionnaly ghosts) and adds it in the registry, 
      optionally filtered on IDs using a given pattern
      @param[in] WithGhosts enable/disable the search for ghosts simulators
      @param[in] IDPattern the pattern to filter the wares on ID. Default is empty (no filtering)
    */
    void discoverWares(bool WithGhosts, const std::string IDPattern = "");

    /**
      Creates a generator and adds it in the registry if everything went fine
      @param[in] Specs the specifications of the generator to create
      @return the ID of the generator if successfully added, an empty string otherwise
    */
    openfluid::ware::WareID_t addGenerator(const GeneratorSpecs& Specs);

    /**
      Returns the container of the generator given by its ID
      @param[in] ID The ID of the ware
      @return The container of the requested generator, an invalid container if not found
    */
    const WareContainer<openfluid::ware::SimulatorSignature>& 
    generatorContainer(const openfluid::ware::WareID_t& ID) const;

   /**
      Returns the container of the simulator ware or the generator given by its ID.
      It searches first in the available simulators then in the created generators.
      @param[in] ID The ID of the simulator or generator
      @return The container of the requested ware, an invalid container if not found
    */
    const WareContainer<openfluid::ware::SimulatorSignature>& 
    wareOrGeneratorContainer(const openfluid::ware::WareID_t& ID) const;

    /**
      Returns true if a generator already exists with the given ID
      @param[in] ID The ID of the generator
      @return true if the generator exists
    */
    bool hasGenerator(const openfluid::ware::WareID_t& ID) const
    {
      return (m_Generators.find(ID) != m_Generators.end());
    }

    /**
      Returns the created generators, indexed by ID
      @return an ID-container map of generators
    */
    const WaresByID_t& generators() const
    {
      return m_Generators;
    }

    /**
      Clears the generators stored in the registry
    */
    void clearGenerators()
    {
      m_Generators.clear();
    }

    /**
      Clears the registry (simulators and generators)
    */
    void clear()
    {
      clearWares();
      clearGenerators();
    }

};


} } //namespaces

#endif /* __OPENFLUID_MACHINE_SIMULATORREGISTRY_HPP__ */
