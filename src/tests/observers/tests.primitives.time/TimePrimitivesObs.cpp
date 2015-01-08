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
  @file TimePrimitivesObs.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableObserver.hpp>


// =====================================================================
// =====================================================================


DECLARE_OBSERVER_PLUGIN


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("tests.primitives.time")

  DECLARE_NAME("");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

END_OBSERVER_SIGNATURE


// =====================================================================
// =====================================================================


class TimePrimitivesObserver : public openfluid::ware::PluggableObserver
{

  private:

    bool m_IsFirstRunStep;


  public:

    TimePrimitivesObserver() : PluggableObserver(), m_IsFirstRunStep(true)
    {

    }

    // =====================================================================
    // =====================================================================


    ~TimePrimitivesObserver()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {

    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::INITIALIZERUN)
        OPENFLUID_RaiseError("wrong stage");

      if (OPENFLUID_GetCurrentDate() != OPENFLUID_GetBeginDate())
        OPENFLUID_RaiseError("wrong current date");

    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::RUNSTEP)
        OPENFLUID_RaiseError("wrong stage");


      if (m_IsFirstRunStep)
      {
        m_IsFirstRunStep = false;

        if (OPENFLUID_GetCurrentTimeIndex() != OPENFLUID_GetDefaultDeltaT())
          OPENFLUID_RaiseError("wrong time index");

        if (OPENFLUID_GetPreviousRunTimeIndex() != 0)
          OPENFLUID_RaiseError("wrong previous run time index");
      }
      else
      {
        if (OPENFLUID_GetPreviousRunTimeIndex() != (OPENFLUID_GetCurrentTimeIndex()-47))
          OPENFLUID_RaiseError("wrong previous run time index");

        if ((OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) % 47 != 0 )
          OPENFLUID_RaiseError("wrong time index");

      }

      if (OPENFLUID_GetCurrentDate() != (OPENFLUID_GetBeginDate()+OPENFLUID_GetCurrentTimeIndex()))
        OPENFLUID_RaiseError("wrong current date");
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::FINALIZERUN)
        OPENFLUID_RaiseError("wrong stage");

    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(TimePrimitivesObserver)

