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
  @file RunEnvPrimitivesSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
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

  DECLARE_NAME("test simulator for run env primitives");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


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
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetRunEnvironment (dir.input)");

      std::cout << "dir.input = " << RunEnvStr << std::endl;

      if (!OPENFLUID_GetRunEnvironment("dir.output",RunEnvStr))
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetRunEnvironment (dir.output)");

      std::cout << "dir.output = " << RunEnvStr << std::endl;

      if (OPENFLUID_GetRunEnvironment("wrong.str",RunEnvStr))
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetRunEnvironment (wrong.str)");

      RunEnvBool = false;
      if (OPENFLUID_GetRunEnvironment("mode.verbose",RunEnvBool))
       OPENFLUID_RaiseError("incorrect OPENFLUID_GetRunEnvironment (mode.verbose)");

      if (RunEnvBool) OPENFLUID_RaiseError("incorrect OPENFLUID_GetRunEnvironment (mode.verbose is true)");

      if (OPENFLUID_GetRunEnvironment("wrong.bool",RunEnvBool))
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetRunEnvironment (wrong.bool)");
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

