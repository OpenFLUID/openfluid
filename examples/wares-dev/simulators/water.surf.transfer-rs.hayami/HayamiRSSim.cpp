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
  @file HayamiRSSim.cpp
  
  @author Armel THONI <armel.thoni@inra.fr>
 */


/*
<sim2doc>

% Definition des noms de la fonction
\newcommand{\frenchname}{Transfert d'eau dans le réseau hydrographique selon la méthode de propagation d'Hayami}
\newcommand{\englishname}{Water transfer on ditch network using hayami propagation method}

% Definition FileID
\newcommand{\FileID}{water.surf.transfer-rs.hayami}

% Definition variables produites
\newcommand{\VarProdA}{water.surf.Q.downstream-rs}
\newcommand{\VarProdB}{water.surf.H.level-rs}

% Used variables (if necessary)
\newcommand{\VarUsedA}{water.surf.Q.downstream-su}
\newcommand{\VarUsedB}{water.sz-surf.Q.baseflow}
\newcommand{\VarUsedC}{water.uz.Q.interflow}

% Definition parametres
\newcommand{\ParamA}{maxsteps}
\newcommand{\ParamB}{meancel}
\newcommand{\ParamC}{meansigma}
\newcommand{\ParamD}{calibstep}
\newcommand{\ParamE}{rsbuffer}

%Definition des proprietes distribuees
\newcommand{\PropDisA}{nmanning}
\newcommand{\PropDisB}{length}
\newcommand{\PropDisC}{width}
\newcommand{\PropDisD}{height}
\newcommand{\PropDisE}{slope}

% Main file for function description and utilisation


\selectlanguage{english}

\begin{abstract}
\input{./doc/english/HayamiRS_abstract.tex}
\end{abstract}

%******************************
% Scientific concepts
\section{Scientific concepts}
\input{./doc/english/HayamiRS_scientific_concept.tex}


%******************************
% Functional description
\section{Functional description}
\input{./doc/english/HayamiRS_functional_description.tex}

%******************************
% References
\bibliography{./doc/common/water.surf.transfer-rs.hayami.bib}

%******************************
% Appendices
\end{multicols}

%\clearpage

\begin{multicols}{2}


</sim2doc>
 */


#include <cmath>

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>

#include "HayamiTools.hpp"


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("water.surf.transfer-rs.hayami")


  DECLARE_NAME("Water transfer on reach segments using hayami propagation method");
  DECLARE_DESCRIPTION("Calculation of discharge routing through the ditch network using diffusive wave equation "
                      "resolved with Hayami method and discharge convertion to water height");
  DECLARE_DOMAIN("hydrology");
  DECLARE_PROCESS("transfer");
  DECLARE_METHOD("Hayami");

  DECLARE_STATUS(openfluid::ware::STABLE);
  DECLARE_VERSION("19.06");


  DECLARE_AUTHOR("Moussa R.","moussa@supagro.inra.fr");
  DECLARE_AUTHOR("Fabre J.-C.","fabrejc@supagro.inra.fr");
  DECLARE_AUTHOR("Louchart X.","louchart@supagro.inra.fr");
  DECLARE_AUTHOR("Lalauze S.","lalauze@supagro.inra.fr");


  // Used variables
  DECLARE_USED_VARIABLE("water.surf.Q.downstream-su","SU","output volume at the outlet of the SU","m3/s");
  DECLARE_USED_VARIABLE("water.sz-surf.Q.baseflow","GU","drainage output volume from GU to connected RS","m3/s");
  DECLARE_USED_VARIABLE("water.uz.Q.interflow","SU","interflow output volume at the outlet of the SU soil reservoir",
                        "m3/s");

  // Produced variables
  DECLARE_PRODUCED_VARIABLE("water.surf.Q.downstream-rs","RS","output volume at the outlet of the RS","m3/s");
  DECLARE_PRODUCED_VARIABLE("water.surf.H.level-rs","RS","water height at the outlet of the RS","m");

  // Required attributes
  DECLARE_REQUIRED_ATTRIBUTE("nmanning","RS","Manning roughness coefficient of the RS","s/m(-1/3)");
  DECLARE_REQUIRED_ATTRIBUTE("length","RS","lenght of the RS","m");
  DECLARE_REQUIRED_ATTRIBUTE("width","RS","width of the RS","m");
  DECLARE_REQUIRED_ATTRIBUTE("height","RS","height of the RS","m");
  DECLARE_REQUIRED_ATTRIBUTE("slope","RS","mean slope of the RS","m/m");

  // Function parameters
  DECLARE_USED_PARAMETER("maxsteps","maximum hayami kernel steps","-");
  DECLARE_USED_PARAMETER("meancel","wave mean celerity on RSs","m/s");
  DECLARE_USED_PARAMETER("meansigma","mean diffusivity on RSs","m2/s");
  DECLARE_USED_PARAMETER("calibstep","calibration step for height-discharge relation","m");
  DECLARE_USED_PARAMETER("rsbuffer","buffer upon reach for water height over reach height","m");


  // Scheduling
  DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

 */
class HayamiRSSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    int m_MaxSteps;

    float m_MeanCelerity;

    float m_MeanSigma;

    float m_MeanSlope;

    float m_MeanManning;


    bool m_UseInterflowVar;
    bool m_UseBaseflowVar;
    bool m_UseGU;

    /**
          Calibration step for height-discharge relation (meters)
     */
    float m_CalibrationStep;
    
    float m_RSBuffer;

    IDKernelMap m_RSKernel;

    bool m_UseUpSUOutput;

    openfluid::core::IDSerieOfDoubleValuePtrMap m_Input;

    openfluid::core::IDDoubleValueMap m_CurrentInputSum;

    openfluid::core::IDSerieOfDoubleValuePtrMap m_HeightDischarge;

    bool computeWaterHeightFromDischarge(openfluid::core::UnitID_t ID,
                                         openfluid::core::DoubleValue Discharge, 
                                         openfluid::core::DoubleValue& Height)
    {

      if (std::isnan(Discharge.get()))
      {
        return false;
      }

      if (Discharge < 0)
      {
        return false;
      }

      if (openfluid::scientific::isCloseEnough(Discharge,0,0.000001))
      {
        Height = 0.0;
      }
      else
      {
        unsigned int i;
        float Q1, Q2, H1, H2;

        openfluid::core::SerieOfDoubleValue* HeightDischarge = m_HeightDischarge[ID];

        // Search first index in HeightDischarge vector for which Q(Hi) < Discharge
        i = 0;
        while ((i < HeightDischarge->size()) && (HeightDischarge->at(i) < Discharge))
        {
          i++;
        }

        if (i == HeightDischarge->size())
        {
          Height = (i-1) * m_CalibrationStep;

          return false;
        }
        else
        {

          Q1 = HeightDischarge->at(i-1);
          Q2 = HeightDischarge->at(i);

          H1 = (i-1) * m_CalibrationStep;
          H2 = i * m_CalibrationStep;

          if (openfluid::scientific::isCloseEnough(Q2-Q1,0,0.000001))
          {
            Height = 0.5 * (H1+H2);
          }
          else
          {
            Height = H1 + ((Discharge-Q1) * (H2-H1) / (Q2-Q1));
          }
        }
      }

      return true;

    }


  public:


    HayamiRSSimulator(): PluggableSimulator(),
      m_MaxSteps(100),
      m_MeanCelerity(0.49),
      m_MeanSigma(500),
      m_CalibrationStep(0.01),
      m_RSBuffer(0)
    {


    }


    // =====================================================================
    // =====================================================================


    ~HayamiRSSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      // Getting simulator parameters
      OPENFLUID_GetSimulatorParameter(Params,"maxsteps",m_MaxSteps);
      OPENFLUID_GetSimulatorParameter(Params,"meancel",m_MeanCelerity);
      OPENFLUID_GetSimulatorParameter(Params,"meansigma",m_MeanSigma);
      OPENFLUID_GetSimulatorParameter(Params,"calibstep",m_CalibrationStep);
      OPENFLUID_GetSimulatorParameter(Params,"rsbuffer",m_RSBuffer);

      // Checking parameters consistency
      if (m_MaxSteps <= 0)
      {
        OPENFLUID_RaiseError("The maximum hayami kernel steps (maxsteps) should be positive.");
      }
      if (m_MeanCelerity <= 0)
      {
        OPENFLUID_RaiseError("The wave mean celerity on RSs (meancel) should be positive.");
      }
      if (m_MeanSigma <= 0)
      {
        OPENFLUID_RaiseError("The mean diffusivity on RSs (meansigma) should be positive.");
      }
      if (m_CalibrationStep <= 0)
      {
        OPENFLUID_RaiseError("The calibration step for height-discharge relation (calibstep) should be positive.");
      }
      if (m_RSBuffer < 0)
      {
        OPENFLUID_RaiseError("The buffer upon reach for water heigh over reach height (rsbuffer) should be positive or "
                             "null.");
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
      int ID;
      openfluid::core::SpatialUnit* RS;
      openfluid::core::DoubleValue RSmanning, RSslope, RSlength, RSheight, RSwidth;

      m_UseInterflowVar = true;
      m_UseBaseflowVar = true;

      m_UseUpSUOutput = false;
      m_UseGU = false;


      OPENFLUID_UNITS_ORDERED_LOOP("RS",RS)
      {

        ID = RS->getID();

        // Getting distributed properties
        OPENFLUID_GetAttribute(RS,"nmanning",RSmanning);
        OPENFLUID_GetAttribute(RS,"height",RSheight);
        OPENFLUID_GetAttribute(RS,"slope",RSslope);
        OPENFLUID_GetAttribute(RS,"length",RSlength);
        OPENFLUID_GetAttribute(RS,"width",RSwidth);

        // Checking parameters consistency
        std::string IDStr;
        openfluid::tools::convertValue(ID,&IDStr);
        if (RSmanning <= 0)
        {
          OPENFLUID_RaiseError("The Manning roughness coefficient of RS " + IDStr + " should be positive.");
        }
        if (RSheight < 0)
        {
          OPENFLUID_RaiseError("The height of RS " + IDStr + " should be positive.");
        }
        if (RSslope <= 0)
        {
          OPENFLUID_RaiseError("The mean slope of RS " + IDStr + " should be positive.");
        }
        if (RSlength <= 0)
        {
          OPENFLUID_RaiseError("The length of RS " + IDStr + " should be positive.");
        }
        if (RSwidth <= 0)
        {
          OPENFLUID_RaiseError("The width of RS " + IDStr + " should be positive.");
        }
      }


      // Checking if variables and units exist
      if (OPENFLUID_IsUnitsClassExist("SU"))
      {
        m_UseUpSUOutput = true;
      }
      if (OPENFLUID_IsUnitsClassExist("GU"))
      {
        m_UseGU = true;
      }
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      openfluid::core::SpatialUnit* SU;
      openfluid::core::SpatialUnit* GU;

      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {
        m_UseInterflowVar = OPENFLUID_IsVariableExist(SU,"water.uz.Q.interflow") && m_UseInterflowVar;
      }

      if (m_UseGU)
      {
        OPENFLUID_UNITS_ORDERED_LOOP("GU",GU)
        {
          m_UseBaseflowVar = OPENFLUID_IsVariableExist(GU,"water.sz-surf.Q.baseflow") && m_UseBaseflowVar;
        }
      }


      openfluid::core::SpatialUnit* RS;
      float Cel, Sigma;
      openfluid::core::UnitID_t ID;
      openfluid::core::DoubleValue RSmanning,RSslope,RSlength,RSheight,RSwidth;

      m_MeanSlope = 0;
      m_MeanManning = 0;


      OPENFLUID_UNITS_ORDERED_LOOP("RS",RS)
      {
        ID = RS->getID();

        m_Input[ID] = new openfluid::core::SerieOfDoubleValue();
        m_HeightDischarge[ID] = new openfluid::core::SerieOfDoubleValue();
        m_CurrentInputSum[ID] = 0;

        OPENFLUID_GetAttribute(RS,"nmanning",RSmanning);
        OPENFLUID_GetAttribute(RS,"slope",RSslope);
        m_MeanSlope = m_MeanSlope + RSslope;
        m_MeanManning = m_MeanManning + RSmanning;

      }

      unsigned int UnitsCount = OPENFLUID_GetUnitsCount("RS");
      m_MeanSlope = m_MeanSlope / double(UnitsCount);
      
      UnitsCount = OPENFLUID_GetUnitsCount("RS");
      m_MeanManning = m_MeanManning / double(UnitsCount);


      OPENFLUID_UNITS_ORDERED_LOOP("RS",RS)
      {

        OPENFLUID_GetAttribute(RS,"nmanning",RSmanning);
        OPENFLUID_GetAttribute(RS,"slope",RSslope);
        OPENFLUID_GetAttribute(RS,"length",RSlength);
        Cel = m_MeanCelerity * (m_MeanManning / RSmanning) * (sqrt((RSslope / m_MeanSlope)));
        Sigma = m_MeanSigma * (RSmanning/ m_MeanManning) * (m_MeanSlope / RSslope);
        m_RSKernel[RS->getID()] = t_HayamiKernel();
        ComputeHayamiKernel(Cel, Sigma,RSlength,m_MaxSteps,OPENFLUID_GetDefaultDeltaT(), m_RSKernel[RS->getID()]);

      }


      // Compute the downstream-height relation
      double HydrauRadius, Speed, Q, Section, CurrentHeight;
      int i, StepsNbr;

      OPENFLUID_UNITS_ORDERED_LOOP("RS",RS)
      {

        OPENFLUID_GetAttribute(RS,"nmanning",RSmanning);
        OPENFLUID_GetAttribute(RS,"height",RSheight);
        OPENFLUID_GetAttribute(RS,"slope",RSslope);
        OPENFLUID_GetAttribute(RS,"length",RSlength);
        OPENFLUID_GetAttribute(RS,"width",RSwidth);

        StepsNbr = int(ceil((RSheight + m_RSBuffer) / m_CalibrationStep));
        for (i=0;i<StepsNbr;i++)
        {
          CurrentHeight = i * m_CalibrationStep;

          Section = RSwidth * CurrentHeight;
          HydrauRadius = Section / ((2 * CurrentHeight) + RSwidth);
          Speed = (1/RSmanning) * pow(HydrauRadius,0.6666) * pow(RSslope,0.5);
          Q = Speed * Section;

          m_HeightDischarge[RS->getID()]->push_back(Q);
        }

      }

      OPENFLUID_UNITS_ORDERED_LOOP("RS",RS)
      {
        OPENFLUID_InitializeVariable(RS,"water.surf.Q.downstream-rs",0.0);
        OPENFLUID_InitializeVariable(RS,"water.surf.H.level-rs",0.0);
      }


      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      int ID;
      unsigned int CurrentStep;
      unsigned int DeltaT;
      float UpSrcSUsOutputsSum;
      float UpLatSUsOutputsSum;
      float UpRSsOutputsSum;
      float UpGUsOutputsSum;
      openfluid::core::DoubleValue QOutput;
      openfluid::core::DoubleValue QInput;
      openfluid::core::DoubleValue TmpValue, TmpInterflow;

      openfluid::core::SpatialUnit* RS;
      openfluid::core::SpatialUnit* UpRS;
      openfluid::core::SpatialUnit* UpSU;
      openfluid::core::SpatialUnit* DownGU;

      std::list<openfluid::core::SpatialUnit*>::iterator UpSUiter;
      std::list<openfluid::core::SpatialUnit*>* UpSUsList;
      std::list<openfluid::core::SpatialUnit*>::iterator DownGUiter;
      std::list<openfluid::core::SpatialUnit*>* DownGUsList;
      std::list<openfluid::core::SpatialUnit*>::iterator UpRSiter;
      std::list<openfluid::core::SpatialUnit*>* UpRSsList;


      DeltaT = OPENFLUID_GetDefaultDeltaT();
      CurrentStep = OPENFLUID_GetCurrentTimeIndex() / OPENFLUID_GetDefaultDeltaT();

      OPENFLUID_UNITS_ORDERED_LOOP("RS",RS)
      {

        ID = RS->getID();
        QOutput = 0;

        // 1.a Computation of stream coming from source SU units
        UpSrcSUsOutputsSum = 0;
        if (m_UseUpSUOutput)
        {
          UpSUsList = RS->fromSpatialUnits("SU");
          OPENFLUID_UNITSLIST_LOOP(UpSUsList,UpSU)
          {
            OPENFLUID_GetVariable(UpSU,"water.surf.Q.downstream-su",TmpValue);
            TmpInterflow = 0;
            if (m_UseInterflowVar)
            {
              OPENFLUID_GetVariable(UpSU,"water.uz.Q.interflow",TmpInterflow);
            }
            UpSrcSUsOutputsSum = UpSrcSUsOutputsSum + TmpValue + TmpInterflow;

            // Checking used variable consistency
            std::string UpSUStr;
            openfluid::tools::convertValue(UpSU,&UpSUStr);
            if (TmpValue < 0)
            {
              OPENFLUID_RaiseError("The used variable output volume at the outlet of the upstream SU " + UpSUStr + 
                                   " (water.surf.Q.downstream-su) is negative.");
            }
          }
        }


        // 1.b Computation of stream coming from side SU units
        //TODO check if the lateral contributions must be computed here
        UpLatSUsOutputsSum = 0;
        /*
          if (m_UseUpSUOutput)
          {
            UpSUsList = RS->getLatUpstreamSUs();

            for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
            {
              UpSU = *UpSUiter;

              OPENFLUID_GetVariable(UpSU,("water.surf.Q.downstream-su"),CurrentStep,&TmpValue);
              UpLatSUsOutputsSum = UpLatSUsOutputsSum + TmpValue;// / UpSU->getUsrArea();

            }
          }*/


        // 2.a Computation of stream coming from up RS units
        UpRSsOutputsSum = 0;
        UpRSsList = RS->fromSpatialUnits("RS");

        OPENFLUID_UNITSLIST_LOOP(UpRSsList,UpRS)
        {

          OPENFLUID_GetVariable(UpRS,"water.surf.Q.downstream-rs",TmpValue);
          UpRSsOutputsSum = UpRSsOutputsSum + TmpValue;

          // Checking used variable consistency
          std::string UpRSStr;
          openfluid::tools::convertValue(UpRS,&UpRSStr);
          if (TmpValue < 0)
          {
            OPENFLUID_RaiseError("The used variable output volume at the outlet of the upstream RS " + UpRSStr + 
                                 " (water.surf.Q.downstream-rs) is negative.");
          }
        }

        // Added by Simon Lalauze, 08/10/09
        // 2.b Computation of stream coming from the GU units
        UpGUsOutputsSum = 0;
        if (m_UseGU)
        {
          DownGUsList = RS->toSpatialUnits("GU");
          if (m_UseBaseflowVar)
          {

            OPENFLUID_UNITSLIST_LOOP(DownGUsList,DownGU)
            {
              OPENFLUID_GetVariable(DownGU,"water.sz-surf.Q.baseflow",TmpValue);
              UpGUsOutputsSum = UpGUsOutputsSum + TmpValue;
            }
          }
        }


        // 2.c Propagation using Hayami kernel
        QInput = UpRSsOutputsSum + UpSrcSUsOutputsSum + UpGUsOutputsSum;
        m_CurrentInputSum[ID] = m_CurrentInputSum[ID] + QInput;
        m_Input[ID]->push_back(QInput);

        QOutput = 0;
        if (m_CurrentInputSum[ID] > 0)
        {
          QOutput = DoHayamiPropagation(m_RSKernel[ID], CurrentStep-1, m_Input[ID], m_MaxSteps, DeltaT);
        }

        QOutput = QOutput + UpLatSUsOutputsSum;
        OPENFLUID_AppendVariable(RS,"water.surf.Q.downstream-rs",QOutput);

        if (!computeWaterHeightFromDischarge(ID,QOutput,TmpValue))
        {
          std::string IDStr;
          openfluid::tools::convertValue(ID,&IDStr);
          OPENFLUID_RaiseWarning("cannot compute water height on RS " + IDStr);
        }

        OPENFLUID_AppendVariable(RS,"water.surf.H.level-rs",TmpValue);

        // Checking produced variables consistency
        std::string IDStr;
        openfluid::tools::convertValue(ID,&IDStr);
        if (TmpValue < 0)
        {
          OPENFLUID_RaiseError("The produced variable water height at the outlet of the RS " + IDStr + 
                               " (water.surf.H.level-rs) is negative.");
        }
        if (QOutput < 0)
        {
          OPENFLUID_RaiseError("The produced variable output volume at the outlet of the RS " + IDStr + 
                               " (water.surf.Q.downstream-rs) is negative.");
        }
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

      openfluid::core::SpatialUnit* RS;

      OPENFLUID_UNITS_ORDERED_LOOP("RS",RS)
      {
         delete m_Input[RS->getID()];
         delete m_HeightDischarge[RS->getID()];
      }
    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(HayamiRSSimulator);

