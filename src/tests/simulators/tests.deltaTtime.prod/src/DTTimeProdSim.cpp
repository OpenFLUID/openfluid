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
  @file DTTimeProdSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/ware/PluggableSimulator.hpp>


class DeltaTTimeProdSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


  DeltaTTimeProdSimulator() : PluggableSimulator()
  {


  }


  // =====================================================================
  // =====================================================================


  ~DeltaTTimeProdSimulator()
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
    openfluid::core::SpatialUnit* TU;

    openfluid::core::NullValue TheNull;
    long VectorSize = 5;
    openfluid::core::VectorValue TheVector;
    long MatrixColsNb = 4;
    long MatrixRowsNb = 3;
    openfluid::core::MatrixValue TheMatrix;

    OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
    {
      OPENFLUID_InitializeVariable(TU,"tests.double.dt",0.0);
      OPENFLUID_InitializeVariable(TU,"tests.string.dt",std::string("emptydt"));

      OPENFLUID_InitializeVariable(TU,"tests.null.dt",TheNull);

      TheVector = openfluid::core::VectorValue(VectorSize,0.0);
      OPENFLUID_InitializeVariable(TU,"tests.vector.dt",TheVector);
      TheMatrix = openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,0.0);
      OPENFLUID_InitializeVariable(TU,"tests.matrix.dt",TheMatrix);
    }
    return DefaultDeltaT();
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
      OPENFLUID_AppendVariable(TU,"tests.double.dt",double(OPENFLUID_GetCurrentTimeIndex()));
      OPENFLUID_AppendVariable(TU,"tests.string.dt",
                               openfluid::core::IntegerValue(OPENFLUID_GetCurrentTimeIndex()).toString());

      OPENFLUID_AppendVariable(TU,"tests.null.dt",TheNull);

      OPENFLUID_AppendVariable(TU,"tests.vector.dt",
                               openfluid::core::VectorValue(VectorSize,double(OPENFLUID_GetCurrentTimeIndex())));
      OPENFLUID_AppendVariable(TU,"tests.matrix.dt",
                               openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,
                                                            double(OPENFLUID_GetCurrentTimeIndex())));
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


DEFINE_SIMULATOR_CLASS(DeltaTTimeProdSimulator)

