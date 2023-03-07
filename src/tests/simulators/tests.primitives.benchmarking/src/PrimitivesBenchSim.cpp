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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/tools/Timer.hpp>


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
      double XVal;
      double VarDouble = 0.0;
      openfluid::core::DoubleValue VarDoubleVal;
      openfluid::core::IndexedValue IndVal;
      int Repeats = 10000;
      openfluid::tools::Timer T;

      std::cout << std::endl;


      // -----------------------------


      T.restart();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          VarDouble = OPENFLUID_GetAttribute(TU,"indataDouble")->asDoubleValue();
          XVal = VarDouble;
          VarDouble = OPENFLUID_GetAttribute(TU,"indataDouble")->asDoubleValue();
          XVal += VarDouble;
        }
      }
      T.stop();

      std::cout << "attribute by return: " << T.elapsed() << "ms" << std::endl;


      T.restart();
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
      T.stop();

      std::cout << "attribute by reference: " << T.elapsed() << "ms" << std::endl;


      // -----------------------------


      T.restart();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          XVal = OPENFLUID_GetVariable(TU,"tests.double")->asDoubleValue() +
                 OPENFLUID_GetVariable(TU,"tests.doubleval",OPENFLUID_GetCurrentTimeIndex())->asDoubleValue();
        }
      }
      T.stop();

      std::cout << "current variable by return: " << T.elapsed() << "ms" << std::endl;


      T.restart();
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
      T.stop();

      std::cout << "current variable by reference: " << T.elapsed() << "ms" << std::endl;


      // -----------------------------


      T.restart();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          XVal = OPENFLUID_GetLatestVariable(TU,"tests.doubleval").value()->asDoubleValue();
        }
      }
      T.stop();

      std::cout << "latest variable by return: " << T.elapsed() << "ms" << std::endl;


      T.restart();
      for (int i = 0;i<Repeats;i++)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
        {
          OPENFLUID_GetLatestVariable(TU,"tests.doubleval",IndVal);
          XVal = IndVal.value()->asDoubleValue();
        }
      }
      T.stop();

      std::cout << "latest variable by reference: " << T.elapsed() << "ms" << std::endl;


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

