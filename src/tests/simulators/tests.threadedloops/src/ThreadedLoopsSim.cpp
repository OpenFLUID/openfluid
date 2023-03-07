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
  @file ThreadedLoopsSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <cmath>

#include <openfluid/ware/ThreadedLoopMacros.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/Timer.hpp>


class ThreadedLoopsSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    openfluid::core::PcsOrd_t m_LastOrd;

  public:


  ThreadedLoopsSimulator() : PluggableSimulator()
  {


  }


  // =====================================================================
  // =====================================================================


  ~ThreadedLoopsSimulator()
  {


  }


  // =====================================================================
  // =====================================================================


  void initParams(const openfluid::ware::WareParams_t& /*Params*/)
  {

  }


  // =====================================================================
  // =====================================================================


  void prepareData()
  {

    std::cout << std::endl << "Max threads: " << OPENFLUID_GetSimulatorMaxThreads() << std::endl;

  }


  // =====================================================================
  // =====================================================================


  void checkConsistency()
  {

  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest initializeRun()
  {
    openfluid::core::SpatialUnit* TU;

    OPENFLUID_UNITS_ORDERED_LOOP("TU",TU)
    {
      OPENFLUID_InitializeVariable(TU,"tests.data.sequence",0.0);
      OPENFLUID_InitializeVariable(TU,"tests.data.threaded",0.0);
    }

    return DefaultDeltaT();
  }


  // =====================================================================
  // =====================================================================


  void processUnit(openfluid::core::SpatialUnit* aUnit)
  {
    if (m_LastOrd > aUnit->getProcessOrder())
    {
      OPENFLUID_RaiseError("wrong process order");
    }

    m_LastOrd = aUnit->getProcessOrder();

    openfluid::tools::microsleep(100);
  }


  // =====================================================================
  // =====================================================================


  void produceDataOnTUThreaded(openfluid::core::SpatialUnit* aUnit, const openfluid::core::DoubleValue& Value)
  {
    openfluid::tools::microsleep(100*aUnit->getID());

    OPENFLUID_LogInfo("Threaded process of spatial unit " << aUnit->getClass() << "#" << aUnit->getID());

    OPENFLUID_AppendVariable(aUnit,"tests.data.threaded",double(aUnit->getID())+Value/1000.0);
  }


  // =====================================================================
  // =====================================================================


  void produceDataOnTUSequenced(openfluid::core::SpatialUnit* aUnit, const openfluid::core::DoubleValue& Value)
  {
    openfluid::tools::microsleep(100*aUnit->getID());

    OPENFLUID_AppendVariable(aUnit,"tests.data.sequence",double(aUnit->getID())+Value/1000.0);
  }


  // =====================================================================
  // =====================================================================


  void processUnitXTimes(openfluid::core::SpatialUnit* aUnit, const unsigned int& Times)
  {
#ifdef __GNUC__
    OPENFLUID_LogDebug(__PRETTY_FUNCTION__);
#endif

    for (unsigned int i=0; i<Times;i++ )
    {
      processUnit(aUnit);
    }
  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
  {
    openfluid::core::SpatialUnit* TU;
    openfluid::tools::Timer T;

    std::cout << std::endl;


    T.restart();
    m_LastOrd = 0;
    OPENFLUID_UNITS_ORDERED_LOOP("TU",TU)
    {
      processUnit(TU);
    }
    T.stop();
    std::cout << "TU Classic: " << T.elapsed() << "ms" << std::endl;

    T.restart();
    m_LastOrd = 0;
    APPLY_UNITS_ORDERED_LOOP_THREADED("TU",ThreadedLoopsSimulator::processUnit);
    T.stop();
    std::cout << "TU Threaded: " << T.elapsed() << "ms" << std::endl;


    T.restart();
    OPENFLUID_UNITS_ORDERED_LOOP("TU",TU)
    {
      produceDataOnTUSequenced(TU,double((OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT())));
    }
    T.stop();
    std::cout << "TU Production Sequenced: " << T.elapsed() << "ms"  << std::endl;

    T.restart();
    APPLY_UNITS_ORDERED_LOOP_THREADED("TU",ThreadedLoopsSimulator::produceDataOnTUThreaded,
                                      double((OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT())));
    T.stop();
    std::cout << "TU Production Threaded: " << T.elapsed() << "ms"  << std::endl;


    T.restart();
    m_LastOrd = 0;
    OPENFLUID_UNITS_ORDERED_LOOP("TU",TU)
    {
      processUnitXTimes(TU,3);
    }
    T.stop();
    std::cout << "TU Classic 3 times: " << T.elapsed() << "ms"  << std::endl;

    T.restart();
    m_LastOrd = 0;
    APPLY_UNITS_ORDERED_LOOP_THREADED("TU",ThreadedLoopsSimulator::processUnitXTimes,3);
    T.stop();
    std::cout << "TU Threaded 3 times: " << T.elapsed() << "ms"  << std::endl;


    // _-_-_-_-_-_-_-_-_-_-_-_-_
    std::cout << std::endl;

    T.restart();
    m_LastOrd = 0;
    OPENFLUID_ALLUNITS_ORDERED_LOOP(TU)
    {
      processUnit(TU);
    }
    T.stop();
    std::cout << "Full Classic: " << T.elapsed() << "ms"  << std::endl;

    T.restart();
    m_LastOrd = 0;
    APPLY_ALLUNITS_ORDERED_LOOP_THREADED(ThreadedLoopsSimulator::processUnit);
    T.stop();
    std::cout << "Full Threaded: " << T.elapsed() << "ms"  << std::endl;


    T.restart();
    m_LastOrd = 0;
    OPENFLUID_ALLUNITS_ORDERED_LOOP(TU)
    {
      processUnitXTimes(TU,4);
    }
    T.stop();
    std::cout << "Full Classic 4 times: " << T.elapsed() << "ms"  << std::endl;

    T.restart();
    m_LastOrd = 0;
    APPLY_ALLUNITS_ORDERED_LOOP_THREADED(ThreadedLoopsSimulator::processUnitXTimes,4);
    T.stop();
    std::cout << "Full Threaded 4 times: " << T.elapsed() << "ms"  << std::endl;


    return DefaultDeltaT();
  }


  // =====================================================================
  // =====================================================================


  void finalizeRun()
  {

  }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(ThreadedLoopsSimulator)

