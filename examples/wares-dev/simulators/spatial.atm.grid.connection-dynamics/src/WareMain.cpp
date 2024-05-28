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
  @file WareMain.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <ctime>
#include <random>

#include <openfluid/ware/PluggableSimulator.hpp>


/**

*/
class WindFireConnectSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    typedef openfluid::core::IDMap<std::vector<openfluid::core::SpatialUnit*> >::Type Connections_t;

    std::mt19937 m_RandomEngine;
    long m_RngSeed;

    Connections_t m_PotentialConnections;


  public:

    WindFireConnectSimulator() : PluggableSimulator()
    {

    }


    // =====================================================================
    // =====================================================================


    ~WindFireConnectSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      bool FoundRngSeed = OPENFLUID_GetSimulatorParameter(Params,"rngseed",m_RngSeed);
      if (!FoundRngSeed || m_RngSeed < 0)
      {
        m_RngSeed = std::time(0);
      }
      m_RandomEngine.seed(m_RngSeed);
    }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {

      openfluid::core::SpatialUnit* LU;

      // preparation of the lookup table for connections

      OPENFLUID_UNITS_ORDERED_LOOP("LU",LU)
      {
        m_PotentialConnections[LU->getID()].assign(8,nullptr);

        long int TargetID;

        OPENFLUID_GetAttribute(LU,"N",TargetID);
        if (TargetID != -1 )
        {
          m_PotentialConnections[LU->getID()][0] = OPENFLUID_GetUnit("LU",TargetID);
        }

        OPENFLUID_GetAttribute(LU,"NE",TargetID);
        if (TargetID != -1 )
        {
          m_PotentialConnections[LU->getID()][1] = OPENFLUID_GetUnit("LU",TargetID);
        }

        OPENFLUID_GetAttribute(LU,"E",TargetID);
        if (TargetID != -1 )
        {
          m_PotentialConnections[LU->getID()][2] = OPENFLUID_GetUnit("LU",TargetID);
        }

        OPENFLUID_GetAttribute(LU,"SE",TargetID);
        if (TargetID != -1 )
        {
          m_PotentialConnections[LU->getID()][3] = OPENFLUID_GetUnit("LU",TargetID);
        }

        OPENFLUID_GetAttribute(LU,"S",TargetID);
        if (TargetID != -1 )
        {
          m_PotentialConnections[LU->getID()][4] = OPENFLUID_GetUnit("LU",TargetID);
        }

        OPENFLUID_GetAttribute(LU,"SW",TargetID);
        if (TargetID != -1 )
        {
          m_PotentialConnections[LU->getID()][5] = OPENFLUID_GetUnit("LU",TargetID);
        }

        OPENFLUID_GetAttribute(LU,"W",TargetID);
        if (TargetID != -1 )
        {
          m_PotentialConnections[LU->getID()][6] = OPENFLUID_GetUnit("LU",TargetID);
        }

        OPENFLUID_GetAttribute(LU,"NW",TargetID);
        if (TargetID != -1 )
        {
          m_PotentialConnections[LU->getID()][7] = OPENFLUID_GetUnit("LU",TargetID);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {

    }


    // =====================================================================
    // =====================================================================


    openfluid::core::IntegerValue getCorrectedWindDir(const openfluid::core::IntegerValue& MainWindDir)
    {
      std::uniform_int_distribution<int> Distribution(-45, 45);

      // add a random variation to main wind direction
      openfluid::core::IntegerValue CorrectedDir(MainWindDir.get()+Distribution(m_RandomEngine));

      if (CorrectedDir.get()>=360)
      {
        CorrectedDir.set(CorrectedDir.get()-360);
      }
      if (CorrectedDir.get()<0)
      {
        CorrectedDir.set(CorrectedDir.get()+360);
      }

      return CorrectedDir;
    }


    // =====================================================================
    // =====================================================================


    void updateLandConnections(openfluid::core::SpatialUnit* U, const openfluid::core::IntegerValue& WindDir)
    {

      // computation of the target land unit for fire, according to wind direction
      int FireTarget = WindDir.get() + 180;
      if (FireTarget >= 360)
      {
        FireTarget = FireTarget-360;
      }
      int FireTargetIndex = FireTarget / 45;


      openfluid::core::UnitsPtrList_t* ChildrenLUs = U->childSpatialUnits("LU");
      openfluid::core::SpatialUnit* LU;

      OPENFLUID_UNITSLIST_LOOP(ChildrenLUs,LU)
      {

        // remove existing connection
        openfluid::core::UnitsPtrList_t* ToLU = LU->toSpatialUnits("LU");

        if (ToLU != nullptr && ToLU->size()==1)
        {
          OPENFLUID_RemoveFromToConnection(LU,ToLU->front());
        }


        // set new connection
        if (m_PotentialConnections[LU->getID()][FireTargetIndex] != nullptr)
        {
          OPENFLUID_AddFromToConnection(LU,m_PotentialConnections[LU->getID()][FireTargetIndex]);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      openfluid::core::SpatialUnit* AU;

      OPENFLUID_UNITS_ORDERED_LOOP("AU",AU)
      {
        OPENFLUID_InitializeVariable(AU,"gas.atm.degree.winddir",openfluid::core::IntegerValue(0));
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      openfluid::core::SpatialUnit* AU;
      openfluid::core::IndexedValue LatestMainWindDir;

      OPENFLUID_UNITS_ORDERED_LOOP("AU",AU)
      {
        OPENFLUID_GetLatestVariable(AU,"gas.atm.degree.mainwinddir",LatestMainWindDir);

        // compute corrected wind direction
        openfluid::core::IntegerValue CorrectedDir =
            getCorrectedWindDir((int)(LatestMainWindDir.value()->asDoubleValue().get()));

        OPENFLUID_AppendVariable(AU,"gas.atm.degree.winddir",CorrectedDir);

        // update land connections according to corrected wind direction
        updateLandConnections(AU,CorrectedDir);
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


DEFINE_SIMULATOR_CLASS(WindFireConnectSimulator)
