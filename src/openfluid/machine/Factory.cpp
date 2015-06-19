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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/machine/Factory.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/fluidx/RunDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>
#include <openfluid/core/Datastore.hpp>
#include <openfluid/core/DatastoreItem.hpp>
#include <openfluid/core/SpatialGraph.hpp>
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



// =====================================================================
// =====================================================================


void Factory::buildDomainFromDescriptor(openfluid::fluidx::SpatialDomainDescriptor& Descriptor,
                                        openfluid::core::SpatialGraph& SGraph)
{

  // ============== Domain definition ==============

  std::list<openfluid::fluidx::SpatialUnitDescriptor>::iterator itUnits;
  std::list<openfluid::core::UnitClassID_t>::iterator itLinkedUnits;

  openfluid::core::SpatialUnit *FromUnit, *ToUnit, *ParentUnit, *ChildUnit;

  // creating units
  for (itUnits = Descriptor.spatialUnits().begin();itUnits != Descriptor.spatialUnits().end();++itUnits)
  {
    SGraph.addUnit(openfluid::core::SpatialUnit((*itUnits).getUnitsClass(),
                                               (*itUnits).getID(),
                                               (*itUnits).getProcessOrder()));
  }

  // linking to units
  for (itUnits = Descriptor.spatialUnits().begin();itUnits != Descriptor.spatialUnits().end();++itUnits)
  {

    for (itLinkedUnits = (*itUnits).toSpatialUnits().begin();
        itLinkedUnits != (*itUnits).toSpatialUnits().end();
        ++itLinkedUnits)
    {
      FromUnit = SGraph.spatialUnit((*itUnits).getUnitsClass(),(*itUnits).getID());
      ToUnit = SGraph.spatialUnit((*itLinkedUnits).first,(*itLinkedUnits).second);

      if (ToUnit != NULL)
      {
        FromUnit->addToUnit(ToUnit);
        ToUnit->addFromUnit(FromUnit);
      }
      else
      {
        std::ostringstream UnitStr;
        UnitStr << FromUnit->getClass() << "#" << FromUnit->getID();
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Target -to- unit referenced by " + UnitStr.str() +
                                                  " does not exist" );
      }
    }
  }


  // linking child units
  for (itUnits = Descriptor.spatialUnits().begin();itUnits != Descriptor.spatialUnits().end();++itUnits)
  {

    for (itLinkedUnits = (*itUnits).parentSpatialUnits().begin();
        itLinkedUnits != (*itUnits).parentSpatialUnits().end();
        ++itLinkedUnits)
    {
      ChildUnit = SGraph.spatialUnit((*itUnits).getUnitsClass(),(*itUnits).getID());
      ParentUnit = SGraph.spatialUnit((*itLinkedUnits).first,(*itLinkedUnits).second);

      if (ParentUnit != NULL)
      {
        ParentUnit->addChildUnit(ChildUnit);
        ChildUnit->addParentUnit(ParentUnit);
      }
      else
      {
        std::ostringstream UnitStr;
        UnitStr << ChildUnit->getClass() << "#" << ChildUnit->getID();
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Target -parent- unit referenced by " + UnitStr.str() +
                                                  " does not exist" );
      }
    }
  }


  SGraph.sortUnitsByProcessOrder();



  // ============== Attributes ==============


  std::list<openfluid::fluidx::AttributesDescriptor>::iterator itAttrsDesc;

  for (itAttrsDesc = Descriptor.attributes().begin();itAttrsDesc != Descriptor.attributes().end();++itAttrsDesc)
  {

    openfluid::fluidx::AttributesDescriptor::UnitIDAttribute_t UnitsAttrs = (*itAttrsDesc).attributes();
    openfluid::core::SpatialUnit* TheUnit;

    openfluid::fluidx::AttributesDescriptor::UnitIDAttribute_t::const_iterator itUnit;
    openfluid::fluidx::AttributesDescriptor::UnitIDAttribute_t::const_iterator itUnitb = UnitsAttrs.begin();
    openfluid::fluidx::AttributesDescriptor::UnitIDAttribute_t::const_iterator itUnite = UnitsAttrs.end();

    for (itUnit=itUnitb; itUnit!=itUnite; ++itUnit)
    {
      TheUnit = SGraph.spatialUnit((*itAttrsDesc).getUnitsClass(),itUnit->first);

      if (TheUnit != NULL)
      {
        openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t::const_iterator itUnitAttr;

        for (itUnitAttr = itUnit->second.begin(); itUnitAttr!=itUnit->second.end(); ++itUnitAttr)
        {
          if (!openfluid::tools::isValidAttributeName(itUnitAttr->first))
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Wrong syntax for attribute "+
                                                      itUnitAttr->first + " on units class "+
                                                      (*itAttrsDesc).getUnitsClass());

          TheUnit->attributes()->setValue(itUnitAttr->first,itUnitAttr->second);
        }
      }
    }
  }


  // ============== Events ==============


  std::list<openfluid::fluidx::EventDescriptor>::iterator itEvent;
  openfluid::core::SpatialUnit* EventUnit;

  for (itEvent = Descriptor.events().begin();itEvent != Descriptor.events().end();++itEvent)
  {

    EventUnit = SGraph.spatialUnit((*itEvent).getUnitsClass(),(*itEvent).getUnitID());

    if (EventUnit != NULL)
    {
      EventUnit->events()->addEvent((*itEvent).event());
    }

  }

}



// =====================================================================
// =====================================================================


void Factory::buildDatastoreFromDescriptor(openfluid::fluidx::DatastoreDescriptor& Descriptor,
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


void Factory::buildModelInstanceFromDescriptor(openfluid::fluidx::CoupledModelDescriptor& ModelDesc,
                                               ModelInstance& MInstance)
{

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t::const_iterator it;
  ModelItemInstance* IInstance = nullptr;


  if (ModelDesc.items().empty())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"No simulator in model");


  for (it=ModelDesc.items().begin();it!=ModelDesc.items().end();++it)
  {
    if ((*it)->isEnabled())
    {

      if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::NoWareType))
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "unknown model item type");

      if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::PluggedSimulator))
      {
        openfluid::ware::WareID_t ID = ((openfluid::fluidx::SimulatorDescriptor*)(*it))->getID();

        if (!openfluid::tools::isValidWareID(ID))
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "invalid simulator ID \""+ID+"\"");

        // instanciation of a plugged simulator using the plugin manager
        IInstance = SimulatorPluginsManager::instance()->loadWareSignatureOnly(ID);

        IInstance->Params = (*it)->getParameters();
        IInstance->ItemType = openfluid::fluidx::ModelItemDescriptor::PluggedSimulator;
      }

      if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::Generator))
      {
        // instanciation of a data generator
        openfluid::fluidx::GeneratorDescriptor* GenDesc = (openfluid::fluidx::GeneratorDescriptor*)(*it);

        IInstance = new ModelItemInstance();
        IInstance->Verified = true;
        IInstance->Params = (*it)->getParameters();
        IInstance->ItemType = openfluid::fluidx::ModelItemDescriptor::Generator;

        openfluid::ware::SimulatorSignature* Signature = new openfluid::ware::SimulatorSignature();

        std::string TypedVarName = GenDesc->getVariableName();
        GenDesc->isVectorVariable() ? TypedVarName += "[vector]" : TypedVarName += "[double]";

        Signature->ID = buildGeneratorID(GenDesc->getVariableName(),
                                         GenDesc->isVectorVariable(),GenDesc->getUnitsClass());
        Signature->HandledData.ProducedVars
        .push_back(openfluid::ware::SignatureTypedSpatialDataItem(TypedVarName,GenDesc->getUnitsClass(),"",""));

        IInstance->GeneratorInfo = new GeneratorExtraInfo();
        IInstance->GeneratorInfo->VariableName = GenDesc->getVariableName();
        IInstance->GeneratorInfo->UnitsClass = GenDesc->getUnitsClass();
        IInstance->GeneratorInfo->VariableSize = GenDesc->getVariableSize();

        if (GenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Fixed)
          IInstance->GeneratorInfo->GeneratorMethod = openfluid::fluidx::GeneratorDescriptor::Fixed;

        if (GenDesc->getGeneratorMethod() == openfluid::fluidx::GeneratorDescriptor::Random)
          IInstance->GeneratorInfo->GeneratorMethod = openfluid::fluidx::GeneratorDescriptor::Random;

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
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "unknown generator type");

        IInstance->Body = NULL;
        IInstance->Signature = Signature;
      }

      IInstance->OriginalPosition = MInstance.getItemsCount()+1;
      MInstance.appendItem(IInstance);
    }
  }

  MInstance.setGlobalParameters(ModelDesc.getGlobalParameters());

}


// =====================================================================
// =====================================================================


void Factory::buildMonitoringInstanceFromDescriptor(openfluid::fluidx::MonitoringDescriptor& MonDesc,
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
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "invalid observer ID \""+ID+"\"");

      // instanciation of a plugged observer using the plugin manager
      OInstance = ObserverPluginsManager::instance()->loadWareSignatureOnly(ID);
      OInstance->Params = (*it)->getParameters();

      MonInstance.appendObserver(OInstance);
    }
  }
}


// =====================================================================
// =====================================================================


void Factory::fillRunEnvironmentFromDescriptor(openfluid::fluidx::RunDescriptor& RunDesc)
{
  if (!RunDesc.isFilled())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Wrong or undefined run configuration");


  if (RunDesc.isUserValuesBufferSize())
  {
    openfluid::base::RuntimeEnvironment::instance()->setValuesBufferSize(RunDesc.getValuesBufferSize());
  }
  else
  {
    openfluid::base::RuntimeEnvironment::instance()->unsetUserValuesBufferSize();
  }
}


// =====================================================================
// =====================================================================


void Factory::buildSimulationBlobFromDescriptors(openfluid::fluidx::FluidXDescriptor& FluidXDesc,
    SimulationBlob& SimBlob)
{
  buildDomainFromDescriptor(FluidXDesc.spatialDomainDescriptor(),SimBlob.spatialGraph());

  buildDatastoreFromDescriptor(FluidXDesc.datastoreDescriptor(),SimBlob.datastore());

  SimBlob.simulationStatus() = openfluid::base::SimulationStatus(FluidXDesc.runDescriptor().getBeginDate(),
                                                                 FluidXDesc.runDescriptor().getEndDate(),
                                                                 FluidXDesc.runDescriptor().getDeltaT(),
                                                                 FluidXDesc.runDescriptor().getSchedulingConstraint());

  SimBlob.runDescriptor() = FluidXDesc.runDescriptor();

  fillRunEnvironmentFromDescriptor(FluidXDesc.runDescriptor());
}


} } //namespaces


