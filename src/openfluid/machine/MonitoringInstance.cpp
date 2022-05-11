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
  @file MonitoringInstance.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/base/RunContextManager.hpp>
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
  {
    finalize();
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::appendObserver(ObserverInstance* ObsInstance)
{
  if (m_Initialized)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Trying to append observer after observers list initialization");
  }

  m_Observers.push_back(ObsInstance);
}


// =====================================================================
// =====================================================================


void MonitoringInstance::initialize(openfluid::base::SimulationLogger* SimLogger)
{
  openfluid::machine::ObserverPluginsManager* OPlugsMgr = openfluid::machine::ObserverPluginsManager::instance();

  std::list<ObserverInstance*>::const_iterator ObsIter;
  ObserverInstance* CurrentObserver;

  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    CurrentObserver = (*ObsIter);

    // TODO throw execption if body is already set?
    if (!CurrentObserver->Body)
    {
      CurrentObserver->Body.reset(OPlugsMgr->getWareBody(CurrentObserver->Container));
    }

    CurrentObserver->Body->linkToSimulationLogger(SimLogger);
    CurrentObserver->Body->linkToSimulation(&(m_SimulationBlob.simulationStatus()));
    CurrentObserver->Body->linkToRunEnvironment(&openfluid::base::RunContextManager::instance()->getWaresEnvironment());
    CurrentObserver->Body->linkToSpatialGraph(&(m_SimulationBlob.spatialGraph()));
    CurrentObserver->Body->linkToDatastore(&(m_SimulationBlob.datastore()));
    CurrentObserver->Body->initializeWare(CurrentObserver->Container.signature()->ID);

    ++ObsIter;
  }

  m_Initialized = true;
}


// =====================================================================
// =====================================================================


void MonitoringInstance::finalize()
{
  if (!m_Initialized)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Trying to finalize an uninitialized observers list");
  }

  std::list<ObserverInstance*>::const_iterator ObsIter;

  // call of finalizeWare method on each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    (*ObsIter)->Body->finalizeWare();
    ++ObsIter;
  }


  // destroy of each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    (*ObsIter)->Body.reset();
    // TODO reset Signature?
    ++ObsIter;
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
    ++ObsIter;
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
    ++ObsIter;
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
    ++ObsIter;
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_onStepCompleted(const openfluid::core::TimeIndex_t& TimeIndex) const
{
  std::list<ObserverInstance*>::const_iterator ObsIter;

  // call of initParams method on each observer
  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    (*ObsIter)->Body->onStepCompleted();
    (*ObsIter)->Body->setPreviousTimeIndex(TimeIndex);
    ++ObsIter;
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
    ++ObsIter;
  }
}


} }  // namespaces
