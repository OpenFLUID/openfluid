/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file OutputsFunc.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "openfluid-base.h"
#include "openfluid-core.h"
#include "openfluid-tools.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS

// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID("tests.outputs");
  DECLARE_SIGNATURE_NAME("test function for outputs");
  DECLARE_SIGNATURE_SDKVERSION;

  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_PRODUCED_VAR("tests.vector[]","UnitsA","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.scalar","UnitsA","scalar for tests","");
  DECLARE_PRODUCED_VAR("tests.vector[]","UnitsB","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.scalar","UnitsB","scalar for tests","");

END_SIGNATURE_HOOK


class OutputsFunction : public openfluid::base::PluggableFunction
{
  private:

  public:


  OutputsFunction() : PluggableFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  ~OutputsFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  bool initParams(openfluid::core::FuncParamsMap_t Params)
  {


    return true;
  }

  // =====================================================================
  // =====================================================================


  bool prepareData()
  {


    return true;
  }


  // =====================================================================
  // =====================================================================


  bool checkConsistency()
  {


    return true;
  }


  // =====================================================================
  // =====================================================================


  bool initializeRun(const openfluid::base::SimulationInfo* SimInfo)
  {


    return true;
  }

  // =====================================================================
  // =====================================================================


  bool runStep(const openfluid::base::SimulationStatus* SimStatus)
  {
    openfluid::core::Unit *TUA, *TUB;
    long VectorSize = SimStatus->getCurrentStep() % 40;
    openfluid::core::VectorValue TheVector;
    std::string IDStr, TSStr;

    openfluid::tools::ConvertValue(SimStatus->getCurrentStep(),&TSStr);

    DECLARE_UNITS_ORDERED_LOOP(1);
    DECLARE_UNITS_ORDERED_LOOP(5);

    BEGIN_UNITS_ORDERED_LOOP(1,"UnitsA",TUA)

      TheVector = openfluid::core::VectorValue(VectorSize,double(TUA->getID()));
      OPENFLUID_AppendVariable(TUA,"tests.vector",TheVector);
      OPENFLUID_AppendVariable(TUA,"tests.scalar",double(TUA->getID()));

      openfluid::tools::ConvertValue(TUA->getID(),&IDStr);
      OPENFLUID_RaiseWarning("tests.messages",SimStatus->getCurrentStep(),"["+TUA->getClass()+"|"+IDStr+"|"+TSStr+"] Message from tests.outputs function");

    END_LOOP

    BEGIN_UNITS_ORDERED_LOOP(5,"UnitsB",TUB)

      TheVector = openfluid::core::VectorValue(VectorSize,double(TUB->getID()));
      OPENFLUID_AppendVariable(TUB,"tests.vector",TheVector);
      OPENFLUID_AppendVariable(TUB,"tests.scalar",double(TUB->getID()));

      openfluid::tools::ConvertValue(TUB->getID(),&IDStr);
      OPENFLUID_RaiseWarning("tests.messages",SimStatus->getCurrentStep(),"["+TUB->getClass()+"|"+IDStr+"|"+TSStr+"] Message from tests.outputs function");

    END_LOOP


    return true;
  }

  // =====================================================================
  // =====================================================================


  bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
  {


    return true;
  }

};

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_HOOK(OutputsFunction)

