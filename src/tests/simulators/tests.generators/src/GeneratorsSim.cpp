/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file GeneratorsSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>


class GeneratorsSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


  GeneratorsSimulator() : PluggableSimulator()
  {


  }


  // =====================================================================
  // =====================================================================


  ~GeneratorsSimulator()
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
    return DefaultDeltaT();
  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
  {
    openfluid::core::SpatialUnit* TU;

    openfluid::core::DoubleValue SValue;
    openfluid::core::VectorValue VValue;
    openfluid::core::MatrixValue MValue;

    OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
    {
      // FIXED
      OPENFLUID_GetVariable(TU,"tests.fixed",OPENFLUID_GetCurrentTimeIndex(),SValue);
      if (!openfluid::scientific::isCloseEnough<double>(SValue,12.7))
      {
        OPENFLUID_RaiseError("incorrect value for tests.fixed variable");
      }

      //    VECTOR
      OPENFLUID_GetVariable(TU,"tests.fixed-vector-deprecated",OPENFLUID_GetCurrentTimeIndex(),VValue);
      for (const auto& V : VValue)
      {
        if (!openfluid::scientific::isCloseEnough<double>(V,12.7))
        {
          OPENFLUID_RaiseError("incorrect value for tests.fixed-vector-deprecated variable");
        }
      }

      OPENFLUID_GetVariable(TU,"tests.fixed-vector-full-explicit",OPENFLUID_GetCurrentTimeIndex(),VValue);
      if (!openfluid::scientific::isCloseEnough<double>(VValue[0],1))
      {
        OPENFLUID_RaiseError("incorrect value for tests.fixed-vector-full-explicit variable");
      }

      OPENFLUID_GetVariable(TU,"tests.fixed-vector-full-explicit",OPENFLUID_GetCurrentTimeIndex(),VValue);
      if (!openfluid::scientific::isCloseEnough<double>(VValue[2],5))
      {
        OPENFLUID_RaiseError("incorrect value for tests.fixed-vector-full-explicit variable");
      }


      // RANDOM
      OPENFLUID_GetVariable(TU,"tests.random",OPENFLUID_GetCurrentTimeIndex(),SValue);
      if (!(SValue >= 20.53 && SValue<= 50.0))
      {
        OPENFLUID_RaiseError("incorrect value for tests.random variable");
      }

      //    VECTOR
      OPENFLUID_GetVariable(TU,"tests.random-vector-deprecated",OPENFLUID_GetCurrentTimeIndex(),VValue);
      for (const auto& V : VValue)
      {
        if (!(V > 20.53 && V< 50.0))
        {
          OPENFLUID_RaiseError("incorrect value for tests.random-vector-deprecated variable");
        }
      }
      if (VValue[0] != VValue[1])
      {
        OPENFLUID_RaiseError("different random values in tests.random-vector when supposed to be identical");
      }

      OPENFLUID_GetVariable(TU,"tests.random-vector",OPENFLUID_GetCurrentTimeIndex(),VValue);
      for (const auto& V : VValue)
      {
        if (!(V > 20.53 && V< 50.0))
        {
          OPENFLUID_RaiseError("incorrect value for tests.random-vector variable");
        }
      }
      if (VValue[0] == VValue[1])
      {
        OPENFLUID_RaiseError("identical random values in tests.random-vector when supposed to be different");
      }

      //    MATRIX
      OPENFLUID_GetVariable(TU,"tests.random-matrix",OPENFLUID_GetCurrentTimeIndex(),MValue);
      if (!(MValue.get(1,5) > 20.53 && MValue.get(1,5)< 50.0))
      {
        OPENFLUID_RaiseError("incorrect value for tests.random-matrix variable");
      }


      // INTERP
      OPENFLUID_GetVariable(TU,"tests.interp",OPENFLUID_GetCurrentTimeIndex(),SValue);

      if (TU->getID() % 2 != 0)
      {
        if (!(SValue >= -15.0 && SValue <= -1.0))
        {
          OPENFLUID_RaiseError("incorrect value for tests.interp variable (source1.dat)");
        }
      }
      else
      {
        if (!(SValue >= 101.0 && SValue<= 115.0))
        {
          OPENFLUID_RaiseError("incorrect value for tests.interp variable (source2.dat)");
        }
      }


      OPENFLUID_GetVariable(TU,"tests.interpmin",OPENFLUID_GetCurrentTimeIndex(),SValue);

      if (TU->getID() % 2 != 0)
      {
        if (!(SValue >= 0.0))
        {
          OPENFLUID_RaiseError("incorrect value for tests.interpmin variable (source1.dat)");
        }
      }
      else
      {
        if (!(SValue >= 101.0 && SValue<= 115.0))
        {
          OPENFLUID_RaiseError("incorrect value for tests.interpmin variable (source2.dat)");
        }
      }


      OPENFLUID_GetVariable(TU,"tests.interpminmax",OPENFLUID_GetCurrentTimeIndex(),SValue);

      if (!(SValue >= 0.0 && SValue<= 20.0))
      {
        OPENFLUID_RaiseError("incorrect value for tests.interpminmax variable");
      }


      // INJECT
      OPENFLUID_GetVariable(TU,"tests.inject",OPENFLUID_GetCurrentTimeIndex(),SValue);

      if (TU->getID() % 2 != 0)
      {

        if (!openfluid::scientific::isCloseEnough<double>(SValue,
                                                          double(OPENFLUID_GetCurrentTimeIndex())/
                                                          double(OPENFLUID_GetDefaultDeltaT())))
        {
          OPENFLUID_RaiseError("incorrect value for tests.inject variable (source3.dat)");
        }
      }
      else
      {
        if (!openfluid::scientific::isCloseEnough<double>(SValue,0.0))
        {
          OPENFLUID_RaiseError("incorrect value for tests.inject variable (source4.dat)");
        }
      }


      if (OPENFLUID_GetCurrentTimeIndex() > 0 && (OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT()) != 0)
      {
        OPENFLUID_GetVariable(TU,"tests.fixedprev-vector",
                              OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),VValue);
        if (!openfluid::scientific::isCloseEnough(VValue[0],5.3))
        {
          OPENFLUID_RaiseError("incorrect value for tests.fixedprev-vector variable");
        }
      }

    }


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


DEFINE_SIMULATOR_CLASS(GeneratorsSimulator)

