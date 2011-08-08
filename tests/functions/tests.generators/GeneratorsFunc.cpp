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
  \file GeneratorsFunc.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/base.hpp>
#include <openfluid/core.hpp>
#include <openfluid/tools.hpp>


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS

// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.generators"));
  DECLARE_SIGNATURE_NAME(("test function for generators"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_REQUIRED_VAR("tests.fixed","TestUnits","fixed value from generators for tests","");
  DECLARE_REQUIRED_VAR("tests.random","TestUnits","random value from generators for tests","");
  DECLARE_REQUIRED_VAR("tests.interp","TestUnits","interpolated value from generators for tests","");

  DECLARE_REQUIRED_PREVVAR("tests.fixedprev[]","TestUnits","fixed value from generators at a previous time step for tests","");

END_SIGNATURE_HOOK

/**

*/
class GeneratorsFunction : public openfluid::base::PluggableFunction
{
  private:

  public:


  GeneratorsFunction() : PluggableFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  ~GeneratorsFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  bool initParams(openfluid::core::FuncParamsMap_t /*Params*/)
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


  bool initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
  {


    return true;
  }

  // =====================================================================
  // =====================================================================


  bool runStep(const openfluid::base::SimulationStatus* SimStatus)
  {
    openfluid::core::Unit* TU;
    DECLARE_UNITS_ORDERED_LOOP(1);

    openfluid::core::ScalarValue SValue;
    openfluid::core::VectorValue VValue;


    BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

      OPENFLUID_GetVariable(TU,"tests.fixed",SimStatus->getCurrentStep(),&SValue);
      if (!openfluid::tools::IsCloseEnough(SValue,12.7))
        OPENFLUID_RaiseError("tests.generators","incorrect value for tests.fixed variable");


      OPENFLUID_GetVariable(TU,"tests.random",SimStatus->getCurrentStep(),&SValue);
      if (!(SValue >= 20.53 && SValue<= 50.0))
        OPENFLUID_RaiseError("tests.generators","incorrect value for tests.random variable");


      OPENFLUID_GetVariable(TU,"tests.interp",SimStatus->getCurrentStep(),&SValue);

      if (TU->getID() % 2 != 0)
      {
        if (!(SValue >= -15.0 && SValue <= -1.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interp variable (source1.dat)");
      }
      else
      {
        if (!(SValue >= 101.0 && SValue<= 115.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interp variable (source2.dat)");
      }


      OPENFLUID_GetVariable(TU,"tests.interpmin",SimStatus->getCurrentStep(),&SValue);

      if (TU->getID() % 2 != 0)
      {
        if (!(SValue >= 0.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interpmin variable (source1.dat)");
      }
      else
      {
        if (!(SValue >= 101.0 && SValue<= 115.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interpmin variable (source2.dat)");
      }


      OPENFLUID_GetVariable(TU,"tests.interpminmax",SimStatus->getCurrentStep(),&SValue);

      if (!(SValue >= 0.0 && SValue<= 20.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.interpminmax variable");



      OPENFLUID_GetVariable(TU,"tests.inject",SimStatus->getCurrentStep(),&SValue);

      if (TU->getID() % 2 != 0)
      {

        if (!openfluid::tools::IsCloseEnough(SValue,double(SimStatus->getCurrentStep())))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.inject variable (source3.dat)");
      }
      else
      {
        if (!openfluid::tools::IsCloseEnough(SValue,0.0))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.inject variable (source4.dat)");
      }


      if (SimStatus->getCurrentStep()>0)
      {
        OPENFLUID_GetVariable(TU,"tests.fixedprev",SimStatus->getCurrentStep()-1,&VValue);
        if (!openfluid::tools::IsCloseEnough(VValue[0],5.3))
          OPENFLUID_RaiseError("tests.generators","incorrect value for tests.fixedprev variable");
      }


    END_LOOP


    return true;
  }

  // =====================================================================
  // =====================================================================


  bool finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
  {


    return true;
  }

};

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_HOOK(GeneratorsFunction)

