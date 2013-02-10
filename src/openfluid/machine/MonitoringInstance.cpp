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
  \file MonitoringInstance.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/ObserverInstance.hpp>


namespace openfluid { namespace machine {


MonitoringInstance::MonitoringInstance(openfluid::machine::SimulationBlob& SimulationBlob):
    m_SimulationBlob(SimulationBlob), m_Initialized(false)
{

}


// =====================================================================
// =====================================================================


MonitoringInstance::~MonitoringInstance()
{
  if (m_Initialized)
    finalize();

  if (m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","MonitoringInstance::~MonitoringInstance()","Destroyed initialized Monitoring");
}


// =====================================================================
// =====================================================================


void MonitoringInstance::appendObserver(ObserverInstance* ObsInstance)
{
  if (m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","ObserverInstance::appendItem()","Trying to append observer after observers list initialization");

  m_Observers.push_back(ObsInstance);
}


// =====================================================================
// =====================================================================


void MonitoringInstance::initialize(openfluid::base::SimulationLogger* SimLogger)
{
  openfluid::machine::ObserverPluginsManager* OPlugsMgr = openfluid::machine::ObserverPluginsManager::getInstance();

  std::list<ObserverInstance*>::const_iterator ObsIter;
  ObserverInstance* CurrentObserver;

  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    CurrentObserver = (*ObsIter);
    OPlugsMgr->completeSignatureWithWareBody(CurrentObserver);

    CurrentObserver->Body->linkToSimulationLogger(SimLogger);
    CurrentObserver->Body->linkToSimulation(&(m_SimulationBlob.getSimulationStatus()));
    CurrentObserver->Body->linkToRunEnvironment(openfluid::base::RuntimeEnvironment::getInstance()->getWareEnvironment());
    CurrentObserver->Body->linkToCoreRepository(&(m_SimulationBlob.getCoreRepository()));
    CurrentObserver->Body->initializeWare(CurrentObserver->Signature->ID);

    ObsIter++;
  }

  m_Initialized = true;
}


// =====================================================================
// =====================================================================


void MonitoringInstance::finalize()
{
  if (!m_Initialized)
    throw openfluid::base::OFException("OpenFLUID framework","MonitoringInstance::finalize()","Trying to finalize an uninitialized observers list");

  std::list<ObserverInstance*>::const_iterator ObsIter;

  // call of finalizeWare method on each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    (*ObsIter)->Body->finalizeWare();
    ObsIter++;
  }


  // destroy of each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    delete (*ObsIter)->Body;
    ObsIter++;
  }

  m_Initialized = false;
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_initParams() const
{
  std::list<ObserverInstance*>::const_iterator ObsIter;

  // call of initParams method on each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    (*ObsIter)->Body->initParams((*ObsIter)->Params);
    ObsIter++;
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_onPrepared() const
{
  std::list<ObserverInstance*>::const_iterator ObsIter;

  // call of initParams method on each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    (*ObsIter)->Body->onPrepared();
    ObsIter++;
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_onInitializedRun() const
{
  std::list<ObserverInstance*>::const_iterator ObsIter;

  // call of initParams method on each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    (*ObsIter)->Body->onInitializedRun();
    ObsIter++;
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_onStepCompleted() const
{
  std::list<ObserverInstance*>::const_iterator ObsIter;

  // call of initParams method on each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    (*ObsIter)->Body->onStepCompleted();
    ObsIter++;
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_onFinalizedRun() const
{
  std::list<ObserverInstance*>::const_iterator ObsIter;

  // call of initParams method on each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    (*ObsIter)->Body->onFinalizedRun();
    ObsIter++;
  }
}

} }  // namespaces
