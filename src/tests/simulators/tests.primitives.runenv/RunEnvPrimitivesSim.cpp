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


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/core.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


// =====================================================================
// =====================================================================


#define THIS_SIM_ID "tests.primitives.runenv"

BEGIN_SIMULATOR_SIGNATURE(THIS_SIM_ID)

  DECLARE_SIGNATURE_NAME("test simulator for run env primitives");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class RunEnvPrimitivesSimulator : public openfluid::ware::PluggableSimulator
{
  private:


  public:


    RunEnvPrimitivesSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~RunEnvPrimitivesSimulator()
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
      std::string RunEnvStr;
      bool RunEnvBool;


      if (!OPENFLUID_GetRunEnvironment("dir.input",RunEnvStr))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (dir.input)");

      std::cout << "dir.input = " << RunEnvStr << std::endl;

      if (!OPENFLUID_GetRunEnvironment("dir.output",RunEnvStr))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (dir.output)");

      std::cout << "dir.output = " << RunEnvStr << std::endl;

      if (OPENFLUID_GetRunEnvironment("wrong.str",RunEnvStr))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (wrong.str)");

      RunEnvBool = false;
      if (OPENFLUID_GetRunEnvironment("mode.verbose",RunEnvBool))
       OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (mode.verbose)");

      if (RunEnvBool) OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (mode.verbose is true)");

      if (OPENFLUID_GetRunEnvironment("wrong.bool",RunEnvBool))
        OPENFLUID_RaiseError("tests.primitives.use","incorrect OPENFLUID_GetRunEnvironment (wrong.bool)");
    }


    // =====================================================================
    // =====================================================================


    void checkConsistency() { }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      return Never();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      return Never();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

// =====================================================================
// =====================================================================

DEFINE_SIMULATOR_CLASS(RunEnvPrimitivesSimulator)

