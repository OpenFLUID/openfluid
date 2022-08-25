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
  @file Factory.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/machine/Factory.hpp>
#include <openfluid/core/Datastore.hpp>
#include <openfluid/core/DatastoreItem.hpp>
#include <openfluid/core/SpatialGraph.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>
#include <openfluid/fluidx/RunConfigurationDescriptor.hpp>
#include <openfluid/fluidx/SpatialDomainDescriptor.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/ObserverRegistry.hpp>
#include <openfluid/machine/Generator.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/tools/IDHelpers.hpp>


namespace openfluid { namespace machine {


void Factory::buildDomainFromDescriptor(const openfluid::fluidx::SpatialDomainDescriptor& Descriptor,
                                        openfluid::core::SpatialGraph& SGraph)
{
  openfluid::core::SpatialUnit *FromUnit, *ToUnit, *ParentUnit, *ChildUnit;
  const auto& SpatialUnitsByClass = Descriptor.spatialUnits();

  for (const auto& UnitsClass : SpatialUnitsByClass)
  {
    for (const auto& Unit : UnitsClass.second)
    {
      // Creation of the spatial unit

      openfluid::core::SpatialUnit* TheUnit =
          SGraph.addUnit(openfluid::core::SpatialUnit(Unit.second.getUnitsClass(),Unit.second.getID(),
                                                      Unit.second.getProcessOrder()));

      if (TheUnit != nullptr)
      {
        // Add of attributes

        for (const auto& Attribute : Unit.second.attributes())
        {
          if (!openfluid::tools::isValidAttributeName(Attribute.first))
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Wrong syntax for attribute "+ Attribute.first +
                                                      " on units class "+ UnitsClass.first);
          }
          TheUnit->attributes()->setValueFromRawString(Attribute.first,Attribute.second);
        }

        // Add of events

        for (const auto& Event : Unit.second.events())
        {
          TheUnit->events()->addEvent(Event.event());
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "unit " + openfluid::tools::classIDToString(TheUnit->getClass(),
                                                                                              TheUnit->getID()) +
                                                                                              " not created correctly");
      }

    }
  }


  // units linking
  for (const auto& UnitsClass : SpatialUnitsByClass)
  {
    for (const auto& Unit : UnitsClass.second)
    {
      // From-To links

      for (const auto& LinkedUnit : Unit.second.toSpatialUnits())
      {
        FromUnit = SGraph.spatialUnit(Unit.second.getUnitsClass(),Unit.second.getID());
        ToUnit = SGraph.spatialUnit(LinkedUnit.first,LinkedUnit.second);

        if (ToUnit != nullptr)
        {
          FromUnit->addToUnit(ToUnit);
          ToUnit->addFromUnit(FromUnit);
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "to unit referenced by " +
                                                    openfluid::tools::classIDToString(FromUnit->getClass(),
                                                                                      FromUnit->getID()) +
                                                    " does not exist" );
        }
      }

      // Parent-Child links

      for (const auto& LinkedUnit : Unit.second.parentSpatialUnits())
      {
        ChildUnit = SGraph.spatialUnit(Unit.second.getUnitsClass(),Unit.second.getID());
        ParentUnit = SGraph.spatialUnit(LinkedUnit.first,LinkedUnit.second);

        if (ParentUnit != nullptr)
        {
          ParentUnit->addChildUnit(ChildUnit);
          ChildUnit->addParentUnit(ParentUnit);
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "child unit referenced by " +
                                                    openfluid::tools::classIDToString(ChildUnit->getClass(),
                                                                                      ChildUnit->getID()) +
                                                    " does not exist");
        }
      }
    }
  }


  SGraph.sortUnitsByProcessOrder();
}


// =====================================================================
// =====================================================================


void Factory::buildDatastoreFromDescriptor(const openfluid::fluidx::DatastoreDescriptor& Descriptor,
                                           openfluid::core::Datastore& Store)
{
  for(const auto& Desc : Descriptor.items())
  {
    Store.addItem(openfluid::core::DatastoreItem(Desc->getID(),Desc->getPrefixPath(),Desc->getRelativePath(),
                                                 Desc->getType(), Desc->getUnitsClass()));
  }
}


// =====================================================================
// =====================================================================


void Factory::buildModelInstanceFromDescriptor(const openfluid::fluidx::CoupledModelDescriptor& ModelDesc,
                                               ModelInstance& MInstance)
{
  if (ModelDesc.items().empty())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"No simulator in model");
  }

  for (const auto* Desc : ModelDesc.items())
  {
    if (Desc->isEnabled())
    {
      ModelItemInstance* IInstance = nullptr;

      if (Desc->isType(openfluid::ware::WareType::UNDEFINED))
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "unknown model item type");
      }
      else if (Desc->isType(openfluid::ware::WareType::SIMULATOR))
      {
        openfluid::ware::WareID_t SimID = dynamic_cast<const openfluid::fluidx::SimulatorDescriptor*>(Desc)->getID();

        if (!openfluid::tools::isValidWareID(SimID))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"invalid simulator ID \""+SimID+"\"");
        }
        
        if (!SimulatorRegistry::instance()->addWare(SimID))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"simulator \""+SimID+"\" is not available");
        }

        // TODO manage invalid container (should not occur)
        IInstance = new ModelItemInstance(SimulatorRegistry::instance()->wareContainer(SimID));
        IInstance->Params = Desc->getParameters();
      }
      else if (Desc->isType(openfluid::ware::WareType::GENERATOR))
      {
        auto GenDesc = dynamic_cast<const openfluid::fluidx::GeneratorDescriptor*>(Desc);

        auto GenID = SimulatorRegistry::instance()->addGenerator({
                       GenDesc->getGeneratorMethod(),
                       GenDesc->getUnitsClass(),
                       GenDesc->getVariableName(),
                       GenDesc->getVariableSize()
                     });

        if (GenID.empty())
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"invalid generator definition");
        }
 
        // TODO manage invalid container (should not occur)
        IInstance = new ModelItemInstance(SimulatorRegistry::instance()->generatorContainer(GenID));
        IInstance->Params = GenDesc->getParameters();
      }

      openfluid::base::RunContextManager::instance()->processWareParams(IInstance->Params);

      IInstance->OriginalPosition = MInstance.getItemsCount()+1;
      MInstance.appendItem(IInstance);
    }
  }

  MInstance.setGlobalParameters(ModelDesc.getGlobalParameters());

}


// =====================================================================
// =====================================================================


void Factory::buildMonitoringInstanceFromDescriptor(const openfluid::fluidx::MonitoringDescriptor& MonDesc,
                                                    MonitoringInstance& MonInstance)
{
  for (const auto* Desc : MonDesc.items())
  {
    if (Desc->isEnabled())
    {
      ObserverInstance* OInstance = nullptr;
      openfluid::ware::WareID_t ID = Desc->getID();

      if (!openfluid::tools::isValidWareID(ID))
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"invalid observer ID \""+ID+"\"");
      }

      if (!ObserverRegistry::instance()->addWare(ID))
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"observer \""+ID+"\" is not available");
      }

      // TODO manage invalid container (should not occur)
      OInstance = new ObserverInstance(ObserverRegistry::instance()->wareContainer(ID));
      OInstance->Params = Desc->getParameters();
      openfluid::base::RunContextManager::instance()->processWareParams(OInstance->Params);

      MonInstance.appendObserver(OInstance);
    }
  }
}


// =====================================================================
// =====================================================================


void Factory::fillRunContextFromDescriptor(const openfluid::fluidx::RunConfigurationDescriptor& RunDesc)
{
  if (!RunDesc.isFilled())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Wrong or undefined run configuration");
  }

  if (RunDesc.isUserValuesBufferSize())
  {
    openfluid::base::RunContextManager::instance()->setValuesBufferUserSize(RunDesc.getValuesBufferSize());
  }
  else
  {
    openfluid::base::RunContextManager::instance()->unsetValuesBufferUserSize();
  }
}


// =====================================================================
// =====================================================================


void Factory::buildSimulationBlobFromDescriptors(const openfluid::fluidx::FluidXDescriptor& FluidXDesc,
                                                 SimulationBlob& SimBlob)
{
  buildDomainFromDescriptor(FluidXDesc.spatialDomain(),SimBlob.spatialGraph());

  buildDatastoreFromDescriptor(FluidXDesc.datastore(),SimBlob.datastore());

  SimBlob.simulationStatus() =
      openfluid::base::SimulationStatus(FluidXDesc.runConfiguration().getBeginDate(),
                                        FluidXDesc.runConfiguration().getEndDate(),
                                        FluidXDesc.runConfiguration().getDeltaT(),
                                        FluidXDesc.runConfiguration().getSchedulingConstraint());

  SimBlob.runConfiguration() = FluidXDesc.runConfiguration();

  fillRunContextFromDescriptor(FluidXDesc.runConfiguration());
}


} } //namespaces

