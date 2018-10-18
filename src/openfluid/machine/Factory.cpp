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
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
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
  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t Items = Descriptor.items();

  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t::iterator it;

  for(it = Items.begin() ; it != Items.end() ; ++it)
  {
    openfluid::core::DatastoreItem* Item = new openfluid::core::DatastoreItem(
        (*it)->getID(),(*it)->getPrefixPath(),(*it)->getRelativePath(), (*it)->getType(), (*it)->getUnitsClass());

    Store.addItem(Item);
  }
}


// =====================================================================
// =====================================================================


void Factory::buildModelInstanceFromDescriptor(const openfluid::fluidx::CoupledModelDescriptor& ModelDesc,
                                               ModelInstance& MInstance)
{
  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t::const_iterator it;
  ModelItemInstance* IInstance = nullptr;


  if (ModelDesc.items().empty())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"No simulator in model");
  }


  for (it=ModelDesc.items().begin();it!=ModelDesc.items().end();++it)
  {
    if ((*it)->isEnabled())
    {

      if ((*it)->isType(openfluid::ware::WareType::UNDEFINED))
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "unknown model item type");
      }

      if ((*it)->isType(openfluid::ware::WareType::SIMULATOR))
      {
        openfluid::ware::WareID_t ID = ((openfluid::fluidx::SimulatorDescriptor*)(*it))->getID();

        if (!openfluid::tools::isValidWareID(ID))
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "invalid simulator ID \""+ID+"\"");
        }

        // instanciation of a plugged simulator using the plugin manager
        IInstance = SimulatorPluginsManager::instance()->loadWareSignatureOnly(ID);

        IInstance->Params = (*it)->getParameters();
        IInstance->ItemType = openfluid::ware::WareType::SIMULATOR;
      }

      if ((*it)->isType(openfluid::ware::WareType::GENERATOR))
      {
        // instanciation of a data generator
        openfluid::fluidx::GeneratorDescriptor* GenDesc = (openfluid::fluidx::GeneratorDescriptor*)(*it);

        IInstance = new ModelItemInstance();
        IInstance->Verified = true;
        IInstance->Params = (*it)->getParameters();
        IInstance->ItemType = openfluid::ware::WareType::GENERATOR;

        openfluid::ware::SimulatorSignature* Signature = new openfluid::ware::SimulatorSignature();

        std::string TypedVarName = GenDesc->getVariableName();
        GenDesc->isVectorVariable() ? TypedVarName += "[vector]" : TypedVarName += "[double]";

        Signature->ID = buildGeneratorID(GenDesc->getVariableName(),
                                         GenDesc->isVectorVariable(),GenDesc->getUnitsClass());
        Signature->HandledData.ProducedVars
        .push_back(openfluid::ware::SignatureTypedSpatialDataItem(TypedVarName,GenDesc->getUnitsClass(),"",""));

        IInstance->GeneratorInfo.reset(new GeneratorExtraInfo());
        IInstance->GeneratorInfo->VariableName = GenDesc->getVariableName();
        IInstance->GeneratorInfo->UnitsClass = GenDesc->getUnitsClass();
        IInstance->GeneratorInfo->VariableSize = GenDesc->getVariableSize();

        if (GenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Fixed)
        {
          IInstance->GeneratorInfo->GeneratorMethod = openfluid::fluidx::GeneratorDescriptor::Fixed;
        }

        if (GenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Random)
        {
          IInstance->GeneratorInfo->GeneratorMethod = openfluid::fluidx::GeneratorDescriptor::Random;
        }

        if (GenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Interp)
        {
          IInstance->GeneratorInfo->GeneratorMethod = openfluid::fluidx::GeneratorDescriptor::Interp;
          Signature->HandledData.RequiredExtraFiles.push_back(GenDesc->getParameters()["sources"]);
          Signature->HandledData.RequiredExtraFiles.push_back(GenDesc->getParameters()["distribution"]);
        }

        if (GenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Inject)
        {
          IInstance->GeneratorInfo->GeneratorMethod = openfluid::fluidx::GeneratorDescriptor::Inject;
          Signature->HandledData.RequiredExtraFiles.push_back(GenDesc->getParameters()["sources"]);
          Signature->HandledData.RequiredExtraFiles.push_back(GenDesc->getParameters()["distribution"]);
        }

        if (IInstance->GeneratorInfo->GeneratorMethod == openfluid::fluidx::GeneratorDescriptor::NoGenMethod)
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "unknown generator type");
        }

        IInstance->Body = nullptr;
        IInstance->Signature = Signature;
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
  openfluid::fluidx::MonitoringDescriptor::SetDescription_t::const_iterator it;
  ObserverInstance* OInstance;

  for (it=MonDesc.items().begin();it!=MonDesc.items().end();++it)
  {
    if ((*it)->isEnabled())
    {
      openfluid::ware::WareID_t ID = ((openfluid::fluidx::ObserverDescriptor*)(*it))->getID();

      if (!openfluid::tools::isValidWareID(ID))
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"invalid observer ID \""+ID+"\"");
      }

      // instanciation of a plugged observer using the plugin manager
      OInstance = ObserverPluginsManager::instance()->loadWareSignatureOnly(ID);
      OInstance->Params = (*it)->getParameters();
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

