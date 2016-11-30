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
  @file ModelInstance.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_MODELINSTANCE_HPP__
#define __OPENFLUID_MACHINE_MODELINSTANCE_HPP__

#include <list>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/machine/ExecutionTimePoint.hpp>
#include <openfluid/base/SimulationLogger.hpp>
#include <openfluid/machine/SimulationProfiler.hpp>



namespace openfluid { namespace ware {

class PluggableSimulator;

}  }


namespace openfluid { namespace machine {

class MachineListener;
class SimulationBlob;
class ModelItemInstance;


class OPENFLUID_API ModelInstance
{
  private:

    std::list<ModelItemInstance*> m_ModelItems;

    openfluid::machine::MachineListener* mp_Listener;

    openfluid::base::SimulationLogger* mp_SimLogger;

    openfluid::machine::SimulationProfiler* mp_SimProfiler;

    openfluid::machine::SimulationBlob& m_SimulationBlob;

    std::list<ExecutionTimePoint> m_TimePointList;

    openfluid::ware::WareParams_t m_GlobalParams;

    bool m_Initialized;

    void appendItemToTimePoint(openfluid::core::TimeIndex_t TimeIndex, openfluid::machine::ModelItemInstance* Item);

    void checkDeltaTMode(openfluid::base::SchedulingRequest& SReq, const openfluid::ware::WareID_t& ID);


  protected:

    openfluid::ware::WareParams_t mergeParamsWithGlobalParams(const openfluid::ware::WareParams_t& Params) const;


  public:

    ModelInstance(openfluid::machine::SimulationBlob& SimulationBlob,
                  openfluid::machine::MachineListener* Listener);

    ~ModelInstance();

    void appendItem(ModelItemInstance* ItemInstance);

    /**
      Insert a ModelItemInstance before the given postion (positions starts at index 0)
      @param[in] ItemInstance the ModelItemInstance to insert
      @param[in] Position the position
    */
    void insertItem(ModelItemInstance* ItemInstance, unsigned int Position);

    /**
      Delete the ModelItemInstance located at the given postion (positions starts at index 0),
      and remove it from the ModelInstance
      @param[in] Position the position
    */
    void deleteItem(unsigned int Position);

    void clear();

    unsigned int getItemsCount() const
    { return m_ModelItems.size(); };

    openfluid::ware::WareParams_t& globalParameters()
    { return m_GlobalParams; };

    void setGlobalParameter(const openfluid::ware::WareParamKey_t& Key, const openfluid::ware::WareParamValue_t& Value);

    void setGlobalParameters(const openfluid::ware::WareParams_t& Params)
    { m_GlobalParams = Params; };

    const std::list<ModelItemInstance*>& items() const
    { return m_ModelItems; };

    void initialize(openfluid::base::SimulationLogger* SimLogger);

    void finalize();

    void call_initParams() const;

    void call_prepareData() const;

    void call_checkConsistency() const;

    void call_initializeRun();

    inline bool hasTimePointToProcess() const
    { return !m_TimePointList.empty(); };

    void processNextTimePoint();

    inline openfluid::core::Duration_t getNextTimePointIndex() const
    {
      if (m_TimePointList.empty())
        return -2;
      return m_TimePointList.front().getTimeIndex();
    }

    void call_finalizeRun() const;

    void resetInitialized()
    { m_Initialized = false; }

};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_MODELINSTANCE_HPP__ */
