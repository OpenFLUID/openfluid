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
  \file MonitoringInstance.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_MONITORINGINSTANCE_HPP__
#define __OPENFLUID_MACHINE_MONITORINGINSTANCE_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/base/SimulationLogger.hpp>

#include <list>


namespace openfluid { namespace machine {

class SimulationBlob;
class ObserverInstance;

class OPENFLUID_API MonitoringInstance
{
  private:

    std::list<ObserverInstance*> m_Observers;

    openfluid::machine::SimulationBlob& m_SimulationBlob;

    bool m_Initialized;

  public:

    MonitoringInstance(openfluid::machine::SimulationBlob& SimulationBlob);

    ~MonitoringInstance();

    void appendObserver(ObserverInstance* ObsInstance);

    const std::list<ObserverInstance*>& getObservers() const { return m_Observers; };

    void initialize(openfluid::base::SimulationLogger* mp_SimLogger);

    void finalize();

    void call_initParams() const;

    void call_onPrepared() const;

    void call_onInitializedRun() const;

    void call_onStepCompleted(const openfluid::core::TimeIndex_t& TimeIndex) const;

    void call_onFinalizedRun() const;
};


} }  // namespaces


#endif /* __OPENFLUID_MACHINE_MONITORINGINSTANCE_HPP__ */
