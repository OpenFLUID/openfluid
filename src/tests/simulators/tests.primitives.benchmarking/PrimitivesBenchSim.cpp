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
  @file PrimitivesBenchSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/core.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <QString>
#include <chrono>


// =====================================================================
// =====================================================================


#define THIS_SIM_ID "tests.primitives.benchmarking"

BEGIN_SIMULATOR_SIGNATURE(THIS_SIM_ID)

  DECLARE_NAME("test simulator for primitives benchmarking");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");

  DECLARE_REQUIRED_ATTRIBUTE("indataDouble","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataString","TestUnits","","")
  DECLARE_REQUIRED_ATTRIBUTE("indataVector","TestUnits","","")

  DECLARE_REQUIRED_VARIABLE("tests.double","TestUnits","double for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.integer","TestUnits","integer for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.bool","TestUnits","boolean for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.string","TestUnits","string for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.null","TestUnits","null for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.doubleval","TestUnits","double value for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.integerval","TestUnits","integer value for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.boolval","TestUnits","boolean value for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.stringval","TestUnits","string value for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.vector","TestUnits","vector for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.matrix","TestUnits","matrix for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.map","TestUnits","map for tests","");

  DECLARE_REQUIRED_VARIABLE("tests.typed.none","TestUnits","untyped for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.double[double]","TestUnits","double for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.integer[integer]","TestUnits","integer for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.bool[boolean]","TestUnits","boolean for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.string[string]","TestUnits","string for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.vector[vector]","TestUnits","vector for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.oldvector[]","TestUnits","old style vector for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.matrix[matrix]","TestUnits","matrix for tests","");
  DECLARE_REQUIRED_VARIABLE("tests.typed.map[map]","TestUnits","map for tests","");

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class PrimitivesBenchmarkingSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    PrimitivesBenchmarkingSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~PrimitivesBenchmarkingSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void prepareData() { }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::SpatialUnit* TU;
      std::chrono::high_resolution_clock::time_point StartTime, EndTime;
      std::chrono::milliseconds Duration;
      double XVal;
      double VarDouble = 0.0;
      openfluid::core::DoubleValue VarDoubleVal;
      openfluid::core::IndexedValue IndVal;
      int Repeats = 10000;

      std::cout << std::endl;


      // =================================


      StartTime = std::chrono::high_resolution_clock::now();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          OPENFLUID_GetAttribute(TU,"indataDouble")->toDouble(VarDouble);
          XVal = VarDouble;
          OPENFLUID_GetAttribute(TU,"indataDouble")->toDouble(VarDouble);
          XVal += VarDouble;
        }
      }
      EndTime = std::chrono::high_resolution_clock::now();

      Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
      std::cout << "attribute by return: " << Duration.count() << "ms" << std::endl;


      StartTime = std::chrono::high_resolution_clock::now();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          OPENFLUID_GetAttribute(TU,"indataDouble",VarDouble);
          XVal = VarDouble;
          OPENFLUID_GetAttribute(TU,"indataDouble",VarDouble);
          XVal += VarDouble;
        }
      }
      EndTime = std::chrono::high_resolution_clock::now();

      Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
      std::cout << "attribute by reference: " << Duration.count() << "ms" << std::endl;


      // =================================


      StartTime = std::chrono::high_resolution_clock::now();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          XVal = OPENFLUID_GetVariable(TU,"tests.double")->asDoubleValue() +
                 OPENFLUID_GetVariable(TU,"tests.doubleval",OPENFLUID_GetCurrentTimeIndex())->asDoubleValue();
        }
      }
      EndTime = std::chrono::high_resolution_clock::now();

      Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
      std::cout << "current variable by return: " << Duration.count() << "ms" << std::endl;


      StartTime = std::chrono::high_resolution_clock::now();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
/*          VarDouble = 0.0;
          VarDoubleVal = openfluid::core::DoubleValue();
          IndVal = openfluid::core::IndexedValue();*/
          OPENFLUID_GetVariable(TU,"tests.double",VarDouble);
          OPENFLUID_GetVariable(TU,"tests.doubleval",VarDoubleVal);
          XVal = VarDouble + VarDoubleVal;
        }
      }
      EndTime = std::chrono::high_resolution_clock::now();

      Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
      std::cout << "current variable by reference: " << Duration.count() << "ms" << std::endl;


      // =================================


      StartTime = std::chrono::high_resolution_clock::now();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          XVal = OPENFLUID_GetLatestVariable(TU,"tests.doubleval").value()->asDoubleValue();
        }
      }
      EndTime = std::chrono::high_resolution_clock::now();

      Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
      std::cout << "latest variable by return: " << Duration.count() << "ms" << std::endl;



      StartTime = std::chrono::high_resolution_clock::now();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          OPENFLUID_GetLatestVariable(TU,"tests.doubleval",IndVal);
          XVal = IndVal.value()->asDoubleValue();
        }
      }
      EndTime = std::chrono::high_resolution_clock::now();

      Duration = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
      std::cout << "latest variable by reference: " << Duration.count() << "ms" << std::endl;



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

DEFINE_SIMULATOR_CLASS(PrimitivesBenchmarkingSimulator)

