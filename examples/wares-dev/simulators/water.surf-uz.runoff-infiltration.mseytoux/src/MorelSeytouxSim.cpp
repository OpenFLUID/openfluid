/*

  This file is part of MHYDAS simulators for OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  This part of MHYDAS is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This part of MHYDAS is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this part of MHYDAS. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of MHYDAS that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of MHYDAS may use this file in accordance
  with the terms contained in the written agreement between You and INRA.

*/


/**
  @file MorelSeytouxSim.cpp
  
 */


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>


/**

 */
class MorelSeytouxSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    /**
      Resolution step
     */
    double m_ResStep;
    double m_CoeffKs;
    double m_CoeffMultiThetaIni;
    bool m_UseCoeffThetaIni;

    /**
      Theta star on each SU
     */
    openfluid::core::IDDoubleMap m_SUThetaStar;

    /**
      Sf on each SU
     */
    openfluid::core::IDDoubleMap m_SUSf;

    /**
      Saturation state on each SU
     */
    openfluid::core::IDIntMap m_SUSatState;

    /**
      Use of upstream output as input on each SU
     */
    openfluid::core::IDBoolMap m_UseUpstreamOutput;

    /**
      Current upstream input value for each SU
     */
    openfluid::core::IDDoubleMap m_CurrentUpstreamInput;

    /**
      Rain sum for each SU
     */
    openfluid::core::IDFloatMap m_RainSum;

    /**
      Previous DeltaW for each SU
     */
    openfluid::core::IDFloatMap m_PreviousDeltaW;

    openfluid::core::IDFloatMap m_PondingTime;

    openfluid::core::IDFloatMap m_PondingSum;

    openfluid::core::IDIntMap m_PondingStep;

    openfluid::core::IDFloatMap m_PondingRainIntensity;

    bool m_UseKsVar;

    openfluid::core::IDFloatMap m_DeltaW1;
    openfluid::core::IDFloatMap m_DeltaW2;
    openfluid::core::IDFloatMap m_DeltaT1;
    openfluid::core::IDFloatMap m_DeltaT2;


  public:

    MorelSeytouxSimulator(): PluggableSimulator(),
      m_ResStep(0.000005), m_CoeffKs(1), m_CoeffMultiThetaIni(1), m_UseKsVar(false)
    {


    }


    // =====================================================================
    // =====================================================================


    ~MorelSeytouxSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {

      OPENFLUID_GetWareParameter(Params,"resstep",m_ResStep);
      if (m_ResStep <= 0)
      {
        OPENFLUID_RaiseError("The numerical resolution step for cumulative infiltration height (resstep) should be "
                             "positive.");
      }

      OPENFLUID_GetWareParameter(Params,"CoeffMultiKs",m_CoeffKs);
      if (m_CoeffKs <= 0)
      {
        OPENFLUID_RaiseError("The multiplying coefficient for ks (coeffks) should be strictly positive.");
      }

      if (OPENFLUID_GetWareParameter(Params,"CoeffMultiThetaIni",m_CoeffMultiThetaIni))
      {
        m_UseCoeffThetaIni = true;
      }

      if (m_CoeffMultiThetaIni < 0)
      {
        OPENFLUID_RaiseError("The multiplication coefficient of soil moisture initial condition should be positive.");
      }

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

      openfluid::core::SpatialUnit* SU;
      openfluid::core::DoubleValue ThetaR, ThetaS, ThetaI, Hc, Area, Beta, Ksat;
      openfluid::core::UnitID_t ID;

      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {

        ID = SU->getID();

        // Getting spatial attributes
        OPENFLUID_GetAttribute(SU,"thetares",ThetaR);
        OPENFLUID_GetAttribute(SU,"thetasat",ThetaS);
        OPENFLUID_GetAttribute(SU,"Hc",Hc);
        OPENFLUID_GetAttribute(SU,"betaMS",Beta);
        OPENFLUID_GetAttribute(SU,"area",Area);

        // Checking parameters consistency
        std::string IDStr = std::to_string(ID);
        if (ThetaR < 0 || ThetaR > 1)
        {
          OPENFLUID_RaiseError("The residual soil water content of the SU " + IDStr + 
                               " should be ranging between 0 and 1.");
        }

        if (ThetaS < 0 || ThetaS > 1)
        {
          OPENFLUID_RaiseError("The saturated soil water content of the SU " + IDStr + 
                               " should be ranging between 0 and 1.");
        }

        if ((ThetaR - ThetaS) > 0)
        {
          OPENFLUID_RaiseError("thetasat of the SU " + IDStr + " should be higher or equal to thetares");
        }

        if (Hc < 0)
        {
          OPENFLUID_RaiseError("The capillary suction height (hc) of the SU " + IDStr + " is negative.");
        }

        if (Beta <= 0)
        {
          OPENFLUID_RaiseError("The viscous correction parameter (betaMS) of the SU " + IDStr + 
                               " is negative or null.");
        }

        if (Area <= 0)
        {
          OPENFLUID_RaiseError("The area of the SU " + IDStr + " is negative or null.");
        }

      }

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      openfluid::core::UnitID_t ID;

      openfluid::core::DoubleValue ThetaR, ThetaS, ThetaI, Hc, ThetaStar, Ksat;
      openfluid::core::SpatialUnit* SU;

      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {

        ID = SU->getID();

        // Getting spatial attributes
        OPENFLUID_GetAttribute(SU,"thetares",ThetaR);
        OPENFLUID_GetAttribute(SU,"thetasat",ThetaS);
        OPENFLUID_GetAttribute(SU,"thetaini",ThetaI);
        OPENFLUID_GetAttribute(SU,"Hc",Hc);

        ThetaI = ThetaI * m_CoeffMultiThetaIni; // default value of Coeff = 1

        // Checking parameters consistency
        std::string IDStr = std::to_string(ID);
        if ((ThetaI - ThetaR) < 0)
        {
          if (m_UseCoeffThetaIni)
          {
            ThetaI = ThetaR;
            OPENFLUID_LogWarning("The initial soil water content in soil reservoir (thetaini * CoeffMultiThetaIni) of "
                                 "the SU " + IDStr + " is lower than the residual soil water content (thetares). "
                                 "Thetaini is set equal to Thetares");
          }
          else
          {
            OPENFLUID_RaiseError("thetaini of the SU " + IDStr + " should be than thetares");
          }
        }
        if ((ThetaI - ThetaS) > 0)
        {
          if (m_UseCoeffThetaIni)
          {
            ThetaI = ThetaS;
            OPENFLUID_LogWarning("The initial soil water content in soil reservoir (thetaini * CoeffMultiThetaIni) of "
                                 "the SU " << IDStr << " is greater than the saturated soil water content (thetasat). "
                                 "Thetaini is set equal to Thetasat");
          }
          else
          {
            OPENFLUID_RaiseError("thetaini of the SU " + IDStr + " should be smaller than thetasat");
          }
        }

        // Getting the Ks property produced by if available
        if (!OPENFLUID_IsAttributeExist(SU,"Ks") && 
            !OPENFLUID_IsVariableExist(SU,"soil.surf.hydraulic-conductivity-Ks"))
        {
          OPENFLUID_RaiseError("Ks should be either given as attribute or previously produced as variable "
                               "(soil.surf.hydraulic-conductivity-Ks)");
        }
        else
        {
          if (OPENFLUID_IsVariableExist(SU,"soil.surf.hydraulic-conductivity-Ks"))
          {
            OPENFLUID_GetAttribute(SU,"Ks",Ksat);
            m_UseKsVar = true;
            if (Ksat < 0)
            {
              OPENFLUID_RaiseError("The saturated hydraulic conductivity of the SU " + IDStr + " is negative.");
            }
          }
        }


        // Computing ThetaStar
        ThetaStar = (ThetaI - ThetaR) / (ThetaS - ThetaR);
        m_SUThetaStar[SU->getID()] = ThetaStar;

        // Computing Sf
        m_SUSf[SU->getID()] = Hc * (1 - (1 * pow(ThetaStar,6))) * (ThetaS - ThetaI);

        // Initializing saturation state
        m_SUSatState[SU->getID()] = 0;

        // Sets whether the upstream output should be used or not
        m_UseUpstreamOutput[SU->getID()] = OPENFLUID_IsVariableExist(SU,"water.surf.Q.downstream-su");

        m_CurrentUpstreamInput[SU->getID()] = 0;
        m_RainSum[SU->getID()] = 0;
        m_PondingSum[SU->getID()] = 0;
        m_PreviousDeltaW[SU->getID()] = 0;
        m_DeltaW1[SU->getID()] = 0;
        m_DeltaW2[SU->getID()] = 0;
        m_DeltaT1[SU->getID()] = 0;
        m_DeltaT2[SU->getID()] = 0;

        OPENFLUID_InitializeVariable(SU,"water.surf.H.runoff",0.0);
        OPENFLUID_InitializeVariable(SU,"water.surf.H.infiltration",0.0);
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      float OutputsSum;
      float RainIntensity;
      float EfficientRainIntensity;
      float CurrentPondingSum;
      float EfficientPondingRainIntensity;
      float CurrentRunoff;
      float CurrentInfiltration;

      int ID;
      openfluid::core::DoubleValue CurrentRain;
      unsigned int CurrentStep;
      unsigned int TimeStep;
      openfluid::core::DoubleValue Ks;

      openfluid::core::DoubleValue Beta;
      double DeltaWi;
      bool Criteria;
      float ExtraTime;
      double InfiltrationCapacity;
      double Area;

      openfluid::core::DoubleValue TmpValue;
      openfluid::core::SpatialUnit* SU;
      openfluid::core::SpatialUnit* UpSU;
      std::list<openfluid::core::SpatialUnit*>::iterator UpSUiter;
      std::list<openfluid::core::SpatialUnit*>* UpSUsList;

      TimeStep = OPENFLUID_GetDefaultDeltaT();
      openfluid::core::TimeIndex_t CurrentTimeIndex = OPENFLUID_GetCurrentTimeIndex();
      openfluid::core::TimeIndex_t PreviousTimeIndex = OPENFLUID_GetPreviousRunTimeIndex();
      CurrentStep = CurrentTimeIndex / TimeStep;

      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {

        ID = SU->getID();

        // Getting distributed properties
        OPENFLUID_GetAttribute(SU,"betaMS",Beta);
        OPENFLUID_GetAttribute(SU,"area",Area);

        // Get the correct value of Ks
        if (m_UseKsVar)
        {
          OPENFLUID_GetVariable(SU,"soil.surf.hydraulic-conductivity-Ks",CurrentTimeIndex,Ks);
        }
        else
        {
          OPENFLUID_GetAttribute(SU,"Ks",Ks);
        }

        // Multiplying the value of Ks by the coeffks (for calibration)
        Ks = m_CoeffKs * Ks;

        CurrentRunoff = 0;
        CurrentInfiltration = 0;
        OutputsSum = 0;

        // Adding upstream SU output (step n-1) to rain
        if (m_UseUpstreamOutput[ID] && CurrentStep > 0)
        {
          UpSUsList = SU->fromSpatialUnits("SU");
          if (UpSUsList != NULL)
          {
            OPENFLUID_UNITSLIST_LOOP(UpSUsList,UpSU)
            {
              if (OPENFLUID_IsVariableExist(UpSU,"water.surf.Q.downstream-su",PreviousTimeIndex),
                                            openfluid::core::Value::DOUBLE)
              {
                OPENFLUID_GetVariable(UpSU,"water.surf.Q.downstream-su",PreviousTimeIndex,TmpValue);
                OutputsSum = OutputsSum + TmpValue * TimeStep / Area;
              }
            }
          }
        }
        m_CurrentUpstreamInput[ID] = OutputsSum;

        // Convert rain from m/s to m/time step
        OPENFLUID_GetVariable(SU,"water.atm-surf.H.rain",CurrentRain);
        CurrentRain = CurrentRain + m_CurrentUpstreamInput[ID];

        // Computing rain intensity
        RainIntensity = (CurrentRain / TimeStep);
        if (openfluid::scientific::isCloseEnough(Ks,0,0.00001))
        {
          // If ks == 0 then runoff = rain
          CurrentRunoff = CurrentRain;
          CurrentInfiltration = 0;
        }
        else
        {
          // If ks > 0
          // Compute of efficient rain intensity
          EfficientRainIntensity =  RainIntensity / Ks;

          if (m_SUSatState[ID] == 0)
          {
            // Compute saturation time
            m_RainSum[ID] = m_RainSum[ID] + CurrentRain;
            if (EfficientRainIntensity > 1)
            {
              m_PondingTime[ID] = (CurrentStep-1) * TimeStep + (1/(RainIntensity)) * 
                                  ((m_SUSf[ID] / (EfficientRainIntensity -1)-m_RainSum[ID]));
              if (m_PondingTime[ID] <= (CurrentStep * TimeStep))
              {
                if (m_PondingTime[ID] > ((CurrentStep-1) * TimeStep))
                {
                  m_PondingRainIntensity[ID] = RainIntensity;
                  m_PondingStep[ID] = CurrentStep;
                  m_PondingSum[ID] = m_RainSum[ID] + (m_PondingTime[ID]-((CurrentStep-1)*TimeStep))*RainIntensity;
                  m_SUSatState[ID] = 2;
                }
                else
                {
                  m_PondingTime[ID] = (CurrentStep-1) * TimeStep;
                  m_PondingStep[ID] = CurrentStep - 1;
                  m_PondingRainIntensity[ID] = (1+(m_SUSf[ID]/m_RainSum[ID])) * Ks;
                  m_PondingSum[ID] = m_RainSum[ID];
                  m_SUSatState[ID] = 3;
                }
              }
            }
          }

          if (m_SUSatState[ID] > 0)
          {
            EfficientPondingRainIntensity = m_PondingRainIntensity[ID] / Ks;
            ExtraTime = (CurrentStep * TimeStep) - m_PondingTime[ID];
            Criteria = true;
            DeltaWi = 0;

            // Computing values for optimization
            CurrentPondingSum = m_PondingSum[ID];

            float PSBEKs = CurrentPondingSum * (Beta * EfficientPondingRainIntensity -1) / Ks;
            float BetaOnKs = Beta / Ks;
            float EPRIPS = EfficientPondingRainIntensity * CurrentPondingSum;

            m_DeltaT1[ID] = (m_DeltaW1[ID] * BetaOnKs) - (PSBEKs) * log((EPRIPS + m_DeltaW1[ID]) / (EPRIPS));
            m_DeltaW2[ID] = m_DeltaW1[ID] + m_ResStep;


            // Determine ponding time
            while (Criteria)
            {
              m_DeltaT2[ID] = (m_DeltaW2[ID] * BetaOnKs) - (PSBEKs) * log((EPRIPS + m_DeltaW2[ID]) / (EPRIPS));
              if (ExtraTime <= m_DeltaT2[ID] && ExtraTime > m_DeltaT1[ID])
              {
                DeltaWi = m_DeltaW2[ID] - 0.5 * m_ResStep;
                m_DeltaW1[ID] = m_DeltaW2[ID] - 2 * m_ResStep;
                Criteria = false;
              }
              m_DeltaW2[ID] = m_DeltaW2[ID] + m_ResStep;
              m_DeltaT1[ID] = m_DeltaT2[ID];
            }


            // Computing infiltration capacity
            InfiltrationCapacity = (DeltaWi - m_PreviousDeltaW[ID]) / TimeStep;
            m_PreviousDeltaW[ID] = DeltaWi;

            if (RainIntensity > InfiltrationCapacity)
            {
              CurrentRunoff = (RainIntensity - InfiltrationCapacity) * TimeStep;
            }
          }
          CurrentInfiltration = CurrentRain - CurrentRunoff;
        }

        OPENFLUID_AppendVariable(SU,"water.surf.H.runoff",CurrentRunoff);
        OPENFLUID_AppendVariable(SU,"water.surf.H.infiltration", CurrentInfiltration);

        // Checking produced variables consistency
        std::string IDStr = std::to_string(ID);

        if (CurrentRunoff < 0)
        {
          OPENFLUID_LogWarning("The produced variable water runoff height (water.surf.H.runoff) on surface of SU " +
                               IDStr + " is negative.");
        }

        if (CurrentInfiltration < 0)
        {
          OPENFLUID_LogWarning("The produced variable water infiltration height (water.surf.H.infiltration) through the"
                               " surface of SU " + IDStr + " is negative.");
        }

        if (openfluid::scientific::isCloseEnough((CurrentInfiltration + CurrentRunoff), CurrentRain, 0.00001) == false)
        {
          OPENFLUID_LogWarning("The sum of water infiltration height and runoff height of SU " + IDStr + 
                               " is different to water rainfall height.");
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


DEFINE_SIMULATOR_CLASS(MorelSeytouxSimulator);

