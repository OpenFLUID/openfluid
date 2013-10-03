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
  \file DebugSim.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <boost/math/special_functions/fpclassify.hpp>

#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN

// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("examples.surf.fire.prod-spread")

  DECLARE_NAME("Example simulator for production and spreading of fire over land units");
  DECLARE_DESCRIPTION("This simulator simulate the dynamics of fuel stock decreasing during fire, based on wind speed and stock type."
                      "It also performs the propagation of the head of the fire using landscapes connections");

  DECLARE_VERSION("1.0");
  DECLARE_STATUS(openfluid::ware::BETA);

  DECLARE_DOMAIN("fire");
  DECLARE_PROCESS("production, spreading");
  DECLARE_METHOD("wind coefficient, stock quantity and type");
  DECLARE_AUTHOR("Jean-Christophe Fabre","fabrejc@supagro.inra.fr");

  DECLARE_SIMULATOR_PARAM("ignitionunits","semicolon separated list of land units IDs where the fire ignites","-")

  DECLARE_REQUIRED_ATTRIBUTE("stockini","LU","Initial stock quantity available on the land unit","-");
  DECLARE_REQUIRED_ATTRIBUTE("landcover","LU","Land cover code for the land unit","-");

  DECLARE_REQUIRED_VAR("gas.atm.V.windspeed","AU","Wind speed","m/s");

  DECLARE_PRODUCED_VAR("fire.surf.Q.stocklevel[integer]","LU","Quantity of stock remaining on the land unit","-");
  DECLARE_PRODUCED_VAR("fire.surf.Q.stockratio[double]","LU","Ratio of stock remaining on the land unit","-");

  DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE

/**

*/
class FireProductionSpreadingSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    std::vector<long int> m_IgnitionUnits;

    bool m_IsIgnited;

    openfluid::core::IDBoolMap m_UnitsStatus;

    openfluid::core::IDDoubleMap m_UnitsCombustionFactor;

    openfluid::core::IDDoubleMap m_UnitsStockIni;

  public:


    FireProductionSpreadingSimulator():
      PluggableSimulator(), m_IsIgnited(false)
    {

    }


    // =====================================================================
    // =====================================================================


    ~FireProductionSpreadingSimulator()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      OPENFLUID_GetSimulatorParameter(Params,"ignitionunits",m_IgnitionUnits);
    }

    // =====================================================================
    // =====================================================================


    void prepareData()
    {
      openfluid::core::Unit* LU;

      OPENFLUID_UNITS_ORDERED_LOOP("LU",LU)
      {
        m_UnitsStatus[LU->getID()] = false;

        openfluid::core::StringValue CoverCode;
        OPENFLUID_GetAttribute(LU,"landcover",CoverCode);

        // set combustion factor according to land cover on the land unit
        // WATER 0
        // FOREST 1
        // CULTIVATED 0.6
        // URBAN 0.2
        if (CoverCode.get() == "WATER") m_UnitsCombustionFactor[LU->getID()] = 0.0;
        else if (CoverCode.get() == "URBAN") m_UnitsCombustionFactor[LU->getID()] = 15.0/3600.0;
        else if (CoverCode.get() == "CULTIVATED") m_UnitsCombustionFactor[LU->getID()] = 45.0/3600.0;
        else if (CoverCode.get() == "FOREST") m_UnitsCombustionFactor[LU->getID()] = 75.0/3600.0;
        else OPENFLUID_RaiseError("FireProductionSpreadingSimulator::prepareData()","unknown cover code");
      }

      // set fire ignition on the land units
      for (unsigned int i=0;i<m_IgnitionUnits.size();i++)
      {
        if (m_UnitsStatus.find(m_IgnitionUnits[i]) != m_UnitsStatus.end())
        {
          m_UnitsStatus[m_IgnitionUnits[i]] = true;
          m_IsIgnited = true;
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {
      if (m_IgnitionUnits.empty())
        OPENFLUID_RaiseError("FireProductionSpreadingSimulator::checkConsistency()","List of land unit to ignite is empty, or wrong land unit ID");
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      openfluid::core::Unit* LU;

      OPENFLUID_UNITS_ORDERED_LOOP("LU",LU)
      {

        double StockIni = 0.0;
        OPENFLUID_GetAttribute(LU,"stockini",StockIni);

        m_UnitsStockIni[LU->getID()] = StockIni;

        openfluid::core::IntegerValue Stock((long int)(StockIni));
        OPENFLUID_InitializeVariable(LU,"fire.surf.Q.stocklevel",Stock);
        OPENFLUID_InitializeVariable(LU,"fire.surf.Q.stockratio",1.0);
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    double getWindCoefficient(openfluid::core::Unit* U)
    {
      openfluid::core::UnitsPtrList_t* ParentAU = U->getParentUnits("AU");

      if (ParentAU != NULL && ParentAU->size()==1)
      {
        openfluid::core::IndexedValue WindSpeed;
        OPENFLUID_GetLatestVariable(ParentAU->front(),"gas.atm.V.windspeed",WindSpeed);

        // set wind coeff according to wind speed
        // 0 < 25 km/h : 1
        // 26 < 50 km/h : 1.4
        // < 75 km/h : 1.7
        // > 75 km/h : 2

        int WindCoeff = 1.0;
        if (WindSpeed.getValue()->asDoubleValue().get() > 25) WindCoeff = 1.4;
        if (WindSpeed.getValue()->asDoubleValue().get() > 50) WindCoeff = 1.7;
        if (WindSpeed.getValue()->asDoubleValue().get() > 75) WindCoeff = 2;

        return WindCoeff;
      }
      else
        return 1.0;
    }


    // =====================================================================
    // =====================================================================


    void updateStock(openfluid::core::Unit* U)
    {

      openfluid::core::IntegerValue Stock;

      OPENFLUID_GetVariable(U,"fire.surf.Q.stocklevel",
                            OPENFLUID_GetCurrentTimeIndex()-OPENFLUID_GetDefaultDeltaT(),
                            Stock);


      // compute the new stock based on time, wind coefficient and comustion factor
      if (m_UnitsStatus[U->getID()])
        Stock.set(Stock.get() - (int)(getWindCoefficient(U) * m_UnitsCombustionFactor[U->getID()] * (double(OPENFLUID_GetDefaultDeltaT()))));


      if (Stock <= 0)
      {
        Stock = 0;
        m_UnitsStatus[U->getID()] = false;
      }


      // compute the new stock ratio : current/initial
      double StockRatio;

      if (m_UnitsStockIni[U->getID()] > 0)
      {
        StockRatio = ((double)(Stock.get()))/((double)(m_UnitsStockIni[U->getID()]));
        if (boost::math::isnan(StockRatio) || boost::math::isinf(StockRatio))
          StockRatio = 0.0;
      }
      else
        StockRatio = 1.0;

      if (StockRatio > 1.0) StockRatio = 1.0;
      if (StockRatio < 0.0) StockRatio = 0.0;

      OPENFLUID_AppendVariable(U,"fire.surf.Q.stocklevel",Stock);
      OPENFLUID_AppendVariable(U,"fire.surf.Q.stockratio",StockRatio);
    }


    // =====================================================================
    // =====================================================================


    void updateStatus(openfluid::core::Unit* U)
    {
      openfluid::core::UnitsPtrList_t* FromLU = U->getFromUnits("LU");

      // compute the burning/non-burning status of each land unit
      // a cell starts burning if :
      // - its stock > 0
      // - and the from land unit is burning
      // - and the from unit stock is burnt at 75%
      if (!m_UnitsStatus[U->getID()] &&
          FromLU != NULL && FromLU->size()==1)
      {
        openfluid::core::IntegerValue Stock;
        OPENFLUID_GetVariable(U,"fire.surf.Q.stocklevel",Stock);

        if (Stock > 0 && m_UnitsStatus[FromLU->front()->getID()])
        {
          openfluid::core::IntegerValue FromStock;
          OPENFLUID_GetVariable(FromLU->front(),"fire.surf.Q.stocklevel",FromStock);

          double FromBurntRatio = double(FromStock.get())/m_UnitsStockIni[FromLU->front()->getID()];

          if (FromBurntRatio <= 0.75)
            m_UnitsStatus[U->getID()] = true;
        }
      }
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::Unit* LU;

      OPENFLUID_UNITS_ORDERED_LOOP("LU",LU)
      {
        updateStock(LU);
      }

      OPENFLUID_UNITS_ORDERED_LOOP("LU",LU)
      {
        updateStatus(LU);
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


DEFINE_SIMULATOR_CLASS(FireProductionSpreadingSimulator)

