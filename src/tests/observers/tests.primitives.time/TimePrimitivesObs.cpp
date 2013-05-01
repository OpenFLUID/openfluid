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
  \file TimePrimitivesObs.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableObserver.hpp>


// =====================================================================
// =====================================================================


DECLARE_OBSERVER_PLUGIN


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("tests.primitives.time")

  DECLARE_SIGNATURE_NAME("");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

END_SIMULATOR_SIGNATURE


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
        OPENFLUID_RaiseError("tests.primitives.time","onInitializedRun()","wrong stage");

      if (OPENFLUID_GetCurrentDate() != OPENFLUID_GetBeginDate())
        OPENFLUID_RaiseError("tests.primitives.time","onInitializedRun()","wrong current date");

    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::RUNSTEP)
        OPENFLUID_RaiseError("tests.primitives.time","onStepCompleted()","wrong stage");


      if (m_IsFirstRunStep)
      {
        m_IsFirstRunStep = false;

        if (OPENFLUID_GetCurrentTimeIndex() != OPENFLUID_GetDefaultDeltaT())
          OPENFLUID_RaiseError("tests.primitives.time","onStepCompleted()","wrong time index");

        if (OPENFLUID_GetPreviousRunTimeIndex() != 0)
          OPENFLUID_RaiseError("tests.primitives.time","onStepCompleted()","wrong previous run time index");
      }
      else
      {
        if (OPENFLUID_GetPreviousRunTimeIndex() != (OPENFLUID_GetCurrentTimeIndex()-47))
          OPENFLUID_RaiseError("tests.primitives.time","onStepCompleted()","wrong previous run time index");

        if ((OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) % 47 != 0 )
          OPENFLUID_RaiseError("tests.primitives.time","onStepCompleted()","wrong time index");

      }

      if (OPENFLUID_GetCurrentDate() != (OPENFLUID_GetBeginDate()+OPENFLUID_GetCurrentTimeIndex()))
        OPENFLUID_RaiseError("tests.primitives.time","onStepCompleted()","wrong current date");
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      if (OPENFLUID_GetCurrentStage() != openfluid::base::SimulationStatus::FINALIZERUN)
        OPENFLUID_RaiseError("tests.primitives.time","finalizeRun()","wrong stage");

    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(TimePrimitivesObserver)

