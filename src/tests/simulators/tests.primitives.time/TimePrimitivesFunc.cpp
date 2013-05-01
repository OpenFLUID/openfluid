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
  \file TimePrimitivesFunc.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


BEGIN_SIMULATOR_SIGNATURE("tests.primitives.time")

  DECLARE_SIGNATURE_NAME("");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

  DECLARE_SCHEDULING_DEFAULT();

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class TimePrimitivesFunction : public openfluid::ware::PluggableSimulator
{
  private:

    bool m_IsFirstRunStep;

  public:


    TimePrimitivesFunction() : PluggableSimulator(), m_IsFirstRunStep(true)
    {


    }


    // =====================================================================
    // =====================================================================


    ~TimePrimitivesFunction()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::INITPARAMS)
        OPENFLUID_RaiseError("tests.primitives.time","initParams()","wrong stage");
    }

    // =====================================================================
    // =====================================================================


    void prepareData()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::PREPAREDATA)
        OPENFLUID_RaiseError("tests.primitives.time","prepareData()","prepare data");

      if (OPENFLUID_GetBeginDate() != openfluid::core::DateTime(2000,1,1,0,0,0))
        OPENFLUID_RaiseError("tests.primitives.time","prepareData()","wrong begin date");

      if (OPENFLUID_GetEndDate() != openfluid::core::DateTime(2000,1,1,6,0,0))
        OPENFLUID_RaiseError("tests.primitives.time","prepareData()","wrong end date");

      if (OPENFLUID_GetSimulationDuration() != 21600)
        OPENFLUID_RaiseError("tests.primitives.time","prepareData()","wrong simulation duration");

      if (OPENFLUID_GetDefaultDeltaT() != 3600)
        OPENFLUID_RaiseError("tests.primitives.time","prepareData()","wrong default deltaT");

    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::CHECKCONSISTENCY)
        OPENFLUID_RaiseError("tests.primitives.time","checkConsistency()","wrong stage");
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::INITIALIZERUN)
        OPENFLUID_RaiseError("tests.primitives.time","initializeRun()","wrong stage");

      if (OPENFLUID_GetCurrentDate() != OPENFLUID_GetBeginDate())
        OPENFLUID_RaiseError("tests.primitives.time","initializeRun()","wrong current date");

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::RUNSTEP)
        OPENFLUID_RaiseError("tests.primitives.time","runStep()","wrong stage");


      if (m_IsFirstRunStep)
      {
        m_IsFirstRunStep = false;

        if (OPENFLUID_GetCurrentTimeIndex() != OPENFLUID_GetDefaultDeltaT())
          OPENFLUID_RaiseError("tests.primitives.time","runStep()","wrong time index");

        if (OPENFLUID_GetPreviousRunTimeIndex() != 0)
          OPENFLUID_RaiseError("tests.primitives.time","runStep()","wrong previous run time index");
      }
      else
      {
        if (OPENFLUID_GetPreviousRunTimeIndex() != (OPENFLUID_GetCurrentTimeIndex()-47))
          OPENFLUID_RaiseError("tests.primitives.time","runStep()","wrong previous run time index");

        if ((OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) % 47 != 0 )
          OPENFLUID_RaiseError("tests.primitives.time","runStep()","wrong time index");

      }

      if (OPENFLUID_GetCurrentDate() != (OPENFLUID_GetBeginDate()+OPENFLUID_GetCurrentTimeIndex()))
        OPENFLUID_RaiseError("tests.primitives.time","runStep()","wrong current date");


      return Duration(47);
    }

    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::FINALIZERUN)
        OPENFLUID_RaiseError("tests.primitives.time","finalizeRun()","wrong stage");
    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(TimePrimitivesFunction)

