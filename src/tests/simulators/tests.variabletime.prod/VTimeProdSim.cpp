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
  @file VTimeProdSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */



#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN

// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.variabletime.prod")

  DECLARE_NAME("test simulator for variable time production");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


  DECLARE_PRODUCED_VAR("tests.double","TestUnits","double for tests","");
  DECLARE_PRODUCED_VAR("tests.string","TestUnits","string for tests","");
  DECLARE_PRODUCED_VAR("tests.null","TestUnits","null for tests","");
  DECLARE_PRODUCED_VAR("tests.vector","TestUnits","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.matrix","TestUnits","matrix for tests","");

END_SIMULATOR_SIGNATURE

/**

*/
class VariableTimeProdSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    bool m_DefaultDT;

  public:


  VariableTimeProdSimulator() : PluggableSimulator(), m_DefaultDT(false)
  {


  }


  // =====================================================================
  // =====================================================================


  ~VariableTimeProdSimulator()
  {


  }


  // =====================================================================
  // =====================================================================


  void initParams(const openfluid::ware::WareParams_t& Params)
  {
    long TmpLong = 0;
    OPENFLUID_GetSimulatorParameter(Params,"usedefaultdeltat",TmpLong);

    m_DefaultDT = TmpLong > 0;

  }

  // =====================================================================
  // =====================================================================


  void prepareData() { }


  // =====================================================================
  // =====================================================================


  void checkConsistency() { }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest initializeRun()
  {
    openfluid::core::SpatialUnit* TU;

    openfluid::core::NullValue TheNull;
    long VectorSize = 40;
    openfluid::core::VectorValue TheVector;
    long MatrixColsNb = 4;
    long MatrixRowsNb = 3;
    openfluid::core::MatrixValue TheMatrix;

    OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
    {
      OPENFLUID_InitializeVariable(TU,"tests.double",0.0);
      OPENFLUID_InitializeVariable(TU,"tests.string","empty");

      OPENFLUID_InitializeVariable(TU,"tests.null",TheNull);

      TheVector = openfluid::core::VectorValue(VectorSize,0.0);
      OPENFLUID_InitializeVariable(TU,"tests.vector",TheVector);
      TheMatrix = openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,0.0);
      OPENFLUID_InitializeVariable(TU,"tests.matrix",TheMatrix);
    }
    return DefaultDeltaT();
  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest computeDeltaT() const
  {
    return openfluid::base::SchedulingRequest((std::rand() % OPENFLUID_GetDefaultDeltaT()) + 1);
  }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
  {
    openfluid::core::SpatialUnit* TU;

    openfluid::core::NullValue TheNull;
    long VectorSize = 40;
    openfluid::core::VectorValue TheVector;
    long MatrixColsNb = 4;
    long MatrixRowsNb = 3;
    openfluid::core::MatrixValue TheMatrix;

    OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
    {
      OPENFLUID_AppendVariable(TU,"tests.double",double(OPENFLUID_GetCurrentTimeIndex()));
      OPENFLUID_AppendVariable(TU,"tests.string",openfluid::core::IntegerValue(OPENFLUID_GetCurrentTimeIndex()).toString());

      if (OPENFLUID_GetCurrentTimeIndex() % 3 == 0) OPENFLUID_AppendVariable(TU,"tests.null",TheNull);

      OPENFLUID_AppendVariable(TU,"tests.vector",openfluid::core::VectorValue(VectorSize,double(OPENFLUID_GetCurrentTimeIndex())));
      if (OPENFLUID_GetCurrentTimeIndex() % 7 == 0) OPENFLUID_AppendVariable(TU,"tests.matrix",openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,double(OPENFLUID_GetCurrentTimeIndex())));
    }

    if (m_DefaultDT)
      return DefaultDeltaT();
    else
      return computeDeltaT();
  }

  // =====================================================================
  // =====================================================================


  void finalizeRun()
  {


  }

};

// =====================================================================
// =====================================================================

DEFINE_SIMULATOR_CLASS(VariableTimeProdSimulator)

