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
  \file DebugSim.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <boost/random.hpp>
#include <ctime>

#include <openfluid/ware/PluggableSimulator.hpp>

// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN

// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("examples.surf.wind-fire.connections")

  DECLARE_NAME("Example simulator for computing dynamic connections between land units according to wind direction");
  DECLARE_DESCRIPTION("This simulator computes the landscape units connections "
                       "according to the corrected wind direction given by the parent atmospheric units."
                      "The corrected wind direction is computed by applying a random value to the main wind direction");

  DECLARE_VERSION("1.0");
  DECLARE_STATUS(openfluid::ware::BETA);

  DECLARE_DOMAIN("fire");
  DECLARE_PROCESS("spatial connections");
  DECLARE_METHOD("wind direction");
  DECLARE_AUTHOR("Jean-Christophe Fabre","fabrejc@supagro.inra.fr");

  DECLARE_REQUIRED_ATTRIBUTE("N","LU","North cell ID, -1 if none","-")
  DECLARE_REQUIRED_ATTRIBUTE("NE","LU","Northeast cell ID, -1 if none","-")
  DECLARE_REQUIRED_ATTRIBUTE("E","LU","East cell ID, -1 if none","-")
  DECLARE_REQUIRED_ATTRIBUTE("SE","LU","Southeast cell ID, -1 if none","-")
  DECLARE_REQUIRED_ATTRIBUTE("S","LU","South cell ID, -1 if none","-")
  DECLARE_REQUIRED_ATTRIBUTE("SW","LU","Southwest cell ID, -1 if none","-")
  DECLARE_REQUIRED_ATTRIBUTE("W","LU","West cell ID, -1 if none","-")
  DECLARE_REQUIRED_ATTRIBUTE("NW","LU","Northwest cell ID, -1 if none","-")

  DECLARE_REQUIRED_VAR("gas.atm.degree.mainwinddir","AU",
                       "Main wind direction in degrees in the athmospheric unit","degree")

  DECLARE_PRODUCED_VAR("gas.atm.degree.winddir","AU",
                       "Corrected wind direction in degrees in the athmospheric unit, computed using main wind direction and a random variation","degree")

  DECLARE_UPDATED_UNITSGRAPH("The spatial graph of LU is modified according to wind directions at each time step");
  DECLARE_UPDATED_UNITSCLASS("LU","The LU connections are modified according to wind directions, "
                                   "using the neighbours cells attributes N, NE, E, SE, S, SW, W, NW.");

  DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE


/**

*/
class WindFireConnectSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    typedef openfluid::core::IDMap<std::vector<openfluid::core::Unit*> >::Type Connections_t;

    boost::mt19937 m_RandomEngine;

    Connections_t m_PotentialConnections;

  public:


    WindFireConnectSimulator() : PluggableSimulator()
    {
      m_RandomEngine.seed(std::time(0));
    }


    // =====================================================================
    // =====================================================================


    ~WindFireConnectSimulator()
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

      openfluid::core::Unit* LU;

      // preparation of the lookup table for connections

      OPENFLUID_UNITS_ORDERED_LOOP("LU",LU)
      {
        m_PotentialConnections[LU->getID()].assign(8,(openfluid::core::Unit*)NULL);

        long int TargetID;

        OPENFLUID_GetAttribute(LU,"N",TargetID);
        if (TargetID != -1 ) m_PotentialConnections[LU->getID()][0] = OPENFLUID_GetUnit("LU",TargetID);

        OPENFLUID_GetAttribute(LU,"NE",TargetID);
        if (TargetID != -1 ) m_PotentialConnections[LU->getID()][1] = OPENFLUID_GetUnit("LU",TargetID);

        OPENFLUID_GetAttribute(LU,"E",TargetID);
        if (TargetID != -1 ) m_PotentialConnections[LU->getID()][2] = OPENFLUID_GetUnit("LU",TargetID);

        OPENFLUID_GetAttribute(LU,"SE",TargetID);
        if (TargetID != -1 ) m_PotentialConnections[LU->getID()][3] = OPENFLUID_GetUnit("LU",TargetID);

        OPENFLUID_GetAttribute(LU,"S",TargetID);
        if (TargetID != -1 ) m_PotentialConnections[LU->getID()][4] = OPENFLUID_GetUnit("LU",TargetID);

        OPENFLUID_GetAttribute(LU,"SW",TargetID);
        if (TargetID != -1 ) m_PotentialConnections[LU->getID()][5] = OPENFLUID_GetUnit("LU",TargetID);

        OPENFLUID_GetAttribute(LU,"W",TargetID);
        if (TargetID != -1 ) m_PotentialConnections[LU->getID()][6] = OPENFLUID_GetUnit("LU",TargetID);

        OPENFLUID_GetAttribute(LU,"NW",TargetID);
        if (TargetID != -1 ) m_PotentialConnections[LU->getID()][7] = OPENFLUID_GetUnit("LU",TargetID);
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
      boost::uniform_int<> Distribution(-45, 45);
      boost::variate_generator<boost::mt19937&, boost::uniform_int<> > Random (m_RandomEngine, Distribution);

      // add a random variation to main wind direction
      openfluid::core::IntegerValue CorrectedDir(MainWindDir.get()+Random());

      if (CorrectedDir.get()>=360) CorrectedDir.set(CorrectedDir.get()-360);
      if (CorrectedDir.get()<0) CorrectedDir.set(CorrectedDir.get()+360);

      return CorrectedDir;
    }


    // =====================================================================
    // =====================================================================


    void updateLandConnections(openfluid::core::Unit* U, const openfluid::core::IntegerValue& WindDir)
    {

      // computation of the target land unit for fire, according to wind direction
      int FireTarget = WindDir.get() + 180;
      if (FireTarget >= 360) FireTarget = FireTarget-360;
      int FireTargetIndex = FireTarget / 45;


      openfluid::core::UnitsPtrList_t* ChildrenLUs = U->getChildrenUnits("LU");
      openfluid::core::Unit* LU;

      OPENFLUID_UNITSLIST_LOOP(ChildrenLUs,LU)
      {

        // remove existing connection
        openfluid::core::UnitsPtrList_t* ToLU = LU->getToUnits("LU");

        if (ToLU != NULL && ToLU->size()==1)
        {
          OPENFLUID_RemoveFromToConnection(LU,ToLU->front());
        }


        // set new connection
        if (m_PotentialConnections[LU->getID()][FireTargetIndex] != NULL)
          OPENFLUID_AddFromToConnection(LU,m_PotentialConnections[LU->getID()][FireTargetIndex]);
      }
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      openfluid::core::Unit* AU;

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

      openfluid::core::Unit* AU;
      openfluid::core::IndexedValue LatestMainWindDir;

      OPENFLUID_UNITS_ORDERED_LOOP("AU",AU)
      {
        OPENFLUID_GetLatestVariable(AU,"gas.atm.degree.mainwinddir",LatestMainWindDir);

        // compute corrected wind direction
        openfluid::core::IntegerValue CorrectedDir =
            getCorrectedWindDir((int)(LatestMainWindDir.getValue()->asDoubleValue().get()));

        OPENFLUID_AppendVariable(AU,"gas.atm.degree.winddir",getCorrectedWindDir(CorrectedDir));

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

