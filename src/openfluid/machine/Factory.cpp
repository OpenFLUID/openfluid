/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file
  @brief Implements ...

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/machine/Factory.hpp>

#include <openfluid/fluidx/DomainDescriptor.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/fluidx/RunDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>
#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/core/Datastore.hpp>
#include <openfluid/core/DatastoreItem.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/SimulatorPluginsManager.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/Generator.hpp>
#include <openfluid/machine/SimulationBlob.hpp>


namespace openfluid { namespace machine {



// =====================================================================
// =====================================================================


void Factory::buildDomainFromDescriptor(openfluid::fluidx::DomainDescriptor& Descriptor,
                                        openfluid::core::CoreRepository& CoreRepos)
{

  // ============== Domain definition ==============

  std::list<openfluid::fluidx::UnitDescriptor>::iterator itUnits;
  std::list<openfluid::core::UnitClassID_t>::iterator itLinkedUnits;

  openfluid::core::Unit *FromUnit, *ToUnit, *ParentUnit, *ChildUnit;

  // creating units
  for (itUnits = Descriptor.getUnits().begin();itUnits != Descriptor.getUnits().end();++itUnits)
  {
    CoreRepos.addUnit(openfluid::core::Unit((*itUnits).getUnitClass(),
                                               (*itUnits).getUnitID(),
                                               (*itUnits).getProcessOrder()));
  }

  // linking to units
  for (itUnits = Descriptor.getUnits().begin();itUnits != Descriptor.getUnits().end();++itUnits)
  {

    for (itLinkedUnits = (*itUnits).getUnitsTos().begin();itLinkedUnits != (*itUnits).getUnitsTos().end();++itLinkedUnits)
    {
      FromUnit = CoreRepos.getUnit((*itUnits).getUnitClass(),(*itUnits).getUnitID());
      ToUnit = CoreRepos.getUnit((*itLinkedUnits).first,(*itLinkedUnits).second);

      if (ToUnit != NULL)
      {
        FromUnit->addToUnit(ToUnit);
        ToUnit->addFromUnit(FromUnit);
      }
      else
      {
        std::ostringstream UnitStr;
        UnitStr << FromUnit->getClass() << "#" << FromUnit->getID();
        throw openfluid::base::OFException("OpenFLUID framework","DomainFactory::buildGraphFromDescriptor","Target -to- unit referenced by " + UnitStr.str() + " does not exist" );
      }
    }
  }


  // linking child units
  for (itUnits = Descriptor.getUnits().begin();itUnits != Descriptor.getUnits().end();++itUnits)
  {

    for (itLinkedUnits = (*itUnits).getUnitsParents().begin();itLinkedUnits != (*itUnits).getUnitsParents().end();++itLinkedUnits)
    {
      ChildUnit = CoreRepos.getUnit((*itUnits).getUnitClass(),(*itUnits).getUnitID());
      ParentUnit = CoreRepos.getUnit((*itLinkedUnits).first,(*itLinkedUnits).second);

      if (ParentUnit != NULL)
      {
        ParentUnit->addChildUnit(ChildUnit);
        ChildUnit->addParentUnit(ParentUnit);
      }
      else
      {
        std::ostringstream UnitStr;
        UnitStr << ChildUnit->getClass() << "#" << ChildUnit->getID();
        throw openfluid::base::OFException("OpenFLUID framework","DomainFactory::buildGraphFromDescriptor","Target -parent- unit referenced by " + UnitStr.str() + " does not exist" );
      }
    }
  }


  CoreRepos.sortUnitsByProcessOrder();



  // ============== Input Data ==============


  std::list<openfluid::fluidx::InputDataDescriptor>::iterator itIData;

  for (itIData = Descriptor.getInputData().begin();itIData != Descriptor.getInputData().end();++itIData)
  {

    openfluid::fluidx::InputDataDescriptor::UnitIDInputData_t Data = (*itIData).getData();
    openfluid::core::Unit* TheUnit;

    for (openfluid::fluidx::InputDataDescriptor::UnitIDInputData_t::const_iterator itUnit=Data.begin();itUnit!=Data.end();++itUnit)
    {
      TheUnit = CoreRepos.getUnit((*itIData).getUnitsClass(),itUnit->first);

      if (TheUnit != NULL)
      {
        for (openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t::const_iterator itUnitData = itUnit->second.begin();itUnitData!=itUnit->second.end();++itUnitData)
        {
          TheUnit->getInputData()->setValue(itUnitData->first,itUnitData->second);
        }
      }
    }
  }


  // ============== Events ==============


  std::list<openfluid::fluidx::EventDescriptor>::iterator itEvent;
  openfluid::core::Unit* EventUnit;

  for (itEvent = Descriptor.getEvents().begin();itEvent != Descriptor.getEvents().end();++itEvent)
  {

    EventUnit = CoreRepos.getUnit((*itEvent).getUnitClass(),(*itEvent).getUnitID());

    if (EventUnit != NULL)
    {
      EventUnit->getEvents()->addEvent((*itEvent).getEvent());
    }

  }

}



// =====================================================================
// =====================================================================


void Factory::buildDatastoreFromDescriptor(openfluid::fluidx::DatastoreDescriptor& Descriptor,
                                          openfluid::core::Datastore& Store)
{
  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t Items = Descriptor.getItems();

  openfluid::fluidx::DatastoreDescriptor::DatastoreDescription_t::iterator it;

  for(it = Items.begin() ; it != Items.end() ; ++it)
  {
    openfluid::core::DatastoreItem* Item = new openfluid::core::DatastoreItem(
        (*it)->getID(),(*it)->getPrefixPath(),(*it)->getRelativePath(), (*it)->getType(), (*it)->getUnitClass());

    Store.addItem(Item);
  }
}


// =====================================================================
// =====================================================================


void Factory::buildModelInstanceFromDescriptor(openfluid::fluidx::CoupledModelDescriptor& ModelDesc,
                                               ModelInstance& MInstance)
{

  openfluid::fluidx::CoupledModelDescriptor::SetDescription_t::const_iterator it;
  ModelItemInstance* IInstance;


  if (ModelDesc.getItems().empty())
    throw openfluid::base::OFException("OpenFLUID framework","ModelFactory::buildInstanceFromDescriptor","No simulator in model");


  for (it=ModelDesc.getItems().begin();it!=ModelDesc.getItems().end();++it)
  {
    if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::NoWareType))
      throw openfluid::base::OFException("OpenFLUID framework","ModelFactory::buildInstanceFromDescriptor","unknown model item type");

    if ((*it)->isType(openfluid::fluidx::ModelItemDescriptor::PluggedSimulator))
    {
      // instanciation of a plugged simulation function using the plugin manager
      IInstance = SimulatorPluginsManager::getInstance()->loadWareSignatureOnly(((openfluid::fluidx::SimulatorDescriptor*)(*it))->getFileID());
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

      std::string VarName = GenDesc->getVariableName();
      GenDesc->isVectorVariable() ? VarName += "[vector]" : VarName += "[double]";

      Signature->ID = buildGeneratorID(GenDesc->getVariableName(),GenDesc->isVectorVariable(),GenDesc->getUnitClass());

      Signature->HandledData.ProducedVars.push_back(openfluid::ware::SignatureHandledTypedDataItem(VarName,GenDesc->getUnitClass(),"",""));

      IInstance->GeneratorInfo = new GeneratorExtraInfo();
      IInstance->GeneratorInfo->VariableName = GenDesc->getVariableName();
      IInstance->GeneratorInfo->UnitClass = GenDesc->getUnitClass();
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
        throw openfluid::base::OFException("OpenFLUID framework","ModelFactory::buildInstanceFromDescriptor","unknown generator type");

      IInstance->Body = NULL;
      IInstance->Signature = Signature;
    }

    IInstance->OriginalPosition = MInstance.getItemsCount()+1;
    MInstance.appendItem(IInstance);

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

  for (it=MonDesc.getItems().begin();it!=MonDesc.getItems().end();++it)
  {
    // instanciation of a plugged observer using the plugin manager
    OInstance = ObserverPluginsManager::getInstance()->loadWareSignatureOnly(((openfluid::fluidx::ObserverDescriptor*)(*it))->getID());
    OInstance->Params = (*it)->getParameters();

    MonInstance.appendObserver(OInstance);
  }
}


// =====================================================================
// =====================================================================


void Factory::fillRunEnvironmentFromDescriptor(openfluid::fluidx::RunDescriptor& RunDesc)
{
  if (!RunDesc.isFilled())
    throw openfluid::base::OFException("OpenFLUID framework","Factory::fillRunEnvironmentFromDescriptor","Wrong or undefined run configuration");


  openfluid::base::RuntimeEnvironment::getInstance()->setSimulationTimeInformation(RunDesc.getBeginDate(),RunDesc.getEndDate(),
      RunDesc.getDeltaT());

  if (RunDesc.isUserValuesBufferSize())
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setValuesBufferSize(RunDesc.getValuesBufferSize());
  }
  else
  {
    openfluid::base::RuntimeEnvironment::getInstance()->unsetUserValuesBufferSize();
  }

  openfluid::base::RuntimeEnvironment::getInstance()->setFilesBufferSize(RunDesc.getFilesBufferSizeInKB()*1024);

  if (RunDesc.isSimulationID())
  {
    openfluid::base::RuntimeEnvironment::getInstance()->setSimulationID(RunDesc.getSimulationID());
  }

}


// =====================================================================
// =====================================================================


void Factory::buildSimulationBlobFromDescriptors(openfluid::fluidx::FluidXDescriptor& FluidXDesc,
    SimulationBlob& SimBlob)
{
  buildDomainFromDescriptor(FluidXDesc.getDomainDescriptor(),SimBlob.getCoreRepository());

  buildDatastoreFromDescriptor(FluidXDesc.getDatastoreDescriptor(),SimBlob.getDatastore());

  SimBlob.getSimulationStatus() = openfluid::base::SimulationStatus(FluidXDesc.getRunDescriptor().getBeginDate(),
                                                                    FluidXDesc.getRunDescriptor().getEndDate(),
                                                                    FluidXDesc.getRunDescriptor().getDeltaT());

  SimBlob.getRunDescriptor() = FluidXDesc.getRunDescriptor();

  fillRunEnvironmentFromDescriptor(FluidXDesc.getRunDescriptor());
}


} } //namespaces


