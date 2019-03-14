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
  @file HayamiSUSim.cpp
  
  @author Armel THONI <armel.thoni@inra.fr>
 */


/*
<sim2doc>
% Definition des noms de la fonction
\newcommand{\frenchname}{Transfert d'eau sur les unités de surface selon la méthode de propagation d'Hayami}
\newcommand{\englishname}{Water transfer on surface units using Hayami propagation method}

% Definition FileID
\newcommand{\FileID}{water.surf.transfer-su.hayami}

% Definition variables produites
\newcommand{\VarProdA}{water.surf.Q.downstream-su}

% Required variables
\newcommand{\VarRequiredA}{water.surf.H.runoff}
\newcommand{\VarUsedA}{water.uz-surf.Q.exfiltration}
\newcommand{\VarUsedB}{water.uz-surf.H.exfiltration}
\newcommand{\VarUsedC}{water.sz-surf.H.exfiltration}

% Definition parametres
\newcommand{\ParamA}{maxsteps}
\newcommand{\ParamB}{meancel}
\newcommand{\ParamC}{meansigma}

% Definition des proprietes distribuees
\newcommand{\PropDisA}{nmanning}
\newcommand{\PropDisB}{area}
\newcommand{\PropDisC}{slope}
\newcommand{\PropDisD}{flowdist}

% Main file for function description and utilisation


\selectlanguage{english}

\begin{abstract}
\iflanguage{english}{\input{./doc/english/HayamiSU_abstract.tex}}{\input{./doc/french/HayamiSU_resume.tex}}
\end{abstract}

%******************************
% Scientific concepts
\iflanguage{english}{\section{Scientific concepts}}{\section{Concepts scientifiques}}
\iflanguage{english}{\input{./doc/english/HayamiSU_scientific_concept.tex}}
  {\input{./doc/french/HayamiSU_concept_scientifique.tex}}


%******************************
% Functional description
\iflanguage{english}{\section{Functional description}}{\section{Notice d'utilisation}}
\iflanguage{english}{\input{./doc/english/HayamiSU_functional_description.tex}}
  {\input{./doc/french/HayamiSU_notice_utilisation.tex}}

%******************************
% References
\bibliography{./doc/common/water.surf.transfer-su.hayami.bib}

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

#include "HayamiTools.hpp"


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("water.surf.transfer-su.hayami")

  DECLARE_NAME("Water transfer on surface units using hayami propagation method");
  DECLARE_DESCRIPTION("Calculation of discharge routing through the channel network using diffusive wave equation "
                      "resolved with Hayami method");
  DECLARE_DOMAIN("hydrology");
  DECLARE_PROCESS("transfer");
  DECLARE_METHOD("Hayami");

  DECLARE_STATUS(openfluid::ware::STABLE);
  DECLARE_VERSION("13.05");

  DECLARE_AUTHOR("Roger Moussa","moussa@supagro.inra.fr");
  DECLARE_AUTHOR("Jean-Christophe Fabre","fabrejc@supagro.inra.fr");
  DECLARE_AUTHOR("Simon Lalauze","lalauze@supagro.inra.fr");
  DECLARE_AUTHOR("Xavier Louchart","louchart@supagro.inra.fr");


  // Required and used variables
  DECLARE_REQUIRED_VARIABLE("water.surf.H.runoff","SU","Runoff height on surface of SU","m");
  DECLARE_USED_VARIABLE("water.uz-surf.Q.exfiltration","SU",
                        "Water exfiltration output volume from the soil reservoir to surface of SU","m3/s");
  DECLARE_USED_VARIABLE("water.sz-surf.H.exfiltration","GU",
                        "Water exfiltration height from saturated zone of GU to surface of SU","m");
  DECLARE_USED_VARIABLE("water.uz-surf.H.exfiltration","GU",
                        "Water exfiltration height from unsaturated zone of GU to surface of SU","m");

  // Produced variable
  DECLARE_PRODUCED_VARIABLE("water.surf.Q.downstream-su","SU","Output volume at the outlet of SU","m3/s");

  // Required attributes
  DECLARE_REQUIRED_ATTRIBUTE("area","SU","Area of SU","m2");
  DECLARE_REQUIRED_ATTRIBUTE("nmanning","SU","Manning roughness coefficient of SU","s/m(-1/3)");
  DECLARE_REQUIRED_ATTRIBUTE("slope","SU","Mean slope of SU","m/m");
  DECLARE_REQUIRED_ATTRIBUTE("flowdist","SU","Flow distance between the SU and the downstream unit (SU or RS)","m");

  // Simulator parameters
  DECLARE_USED_PARAMETER("maxsteps","Maximum hayami kernel steps","-");
  DECLARE_USED_PARAMETER("meancel","Wave mean celerity on SUs","m/s");
  DECLARE_USED_PARAMETER("meansigma","Mean diffusivity on SUs","m2/s");


  // Scheduling
  DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class HayamiSUSimulator : public openfluid::ware::PluggableSimulator
{
  private:
    int m_MaxSteps;

    double m_MeanCelerity;

    double m_MeanSigma;

    double m_MeanSlope;

    double m_MeanManning;

    IDKernelMap m_SUKernel;

    openfluid::core::IDSerieOfDoubleValuePtrMap m_Input;

    openfluid::core::IDDoubleValueMap m_CurrentInputSum;

    bool m_UseExfiltrationUZFromSUVar;
    bool m_UseExfiltrationUZFromGUVar;
    bool m_UseExfiltrationSZVar;


  public:

    HayamiSUSimulator(): PluggableSimulator(),
      m_MaxSteps(100), m_MeanCelerity(0.045), m_MeanSigma(500)
    {


    }


    // =====================================================================
    // =====================================================================


    ~HayamiSUSimulator()
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

      openfluid::core::UnitID_t ID;
      openfluid::core::SpatialUnit* SU;
      openfluid::core::SpatialUnit* GU;
      openfluid::core::DoubleValue SUmanning, SUslope, SUarea;

      m_UseExfiltrationUZFromSUVar = true;
      m_UseExfiltrationUZFromGUVar = true;
      m_UseExfiltrationSZVar = true;


      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {
        ID = SU->getID();

        // Getting distributed properties
        OPENFLUID_GetAttribute(SU,("nmanning"),SUmanning);
        OPENFLUID_GetAttribute(SU,("slope"),SUslope);
        OPENFLUID_GetAttribute(SU,("area"),SUarea);

        // Checking parameters consistency
        std::string IDStr;
        openfluid::tools::convertValue(ID,&IDStr);
        if (SUmanning <= 0) 
        {
          OPENFLUID_RaiseError("The Manning roughness coefficient of SU " + IDStr + " should be positive.");
        }
        if (SUslope <= 0)
        {
          OPENFLUID_RaiseError("The mean slope of SU " + IDStr + " should be positive.");
        }
        if (SUarea <= 0)
        {
          OPENFLUID_RaiseError("The area of SU " + IDStr + " should be positive.");
        }

        m_UseExfiltrationUZFromSUVar = OPENFLUID_IsVariableExist(SU,"water.uz-surf.Q.exfiltration") && 
                                       m_UseExfiltrationUZFromSUVar;

      }


      // Checking if GU units exist
      if (OPENFLUID_IsUnitsClassExist("GU"))
      {
        // Checking if GU variables exist
        OPENFLUID_UNITS_ORDERED_LOOP("GU",GU)
        {
          m_UseExfiltrationSZVar = OPENFLUID_IsVariableExist(GU,"water.sz-surf.H.exfiltration") && 
                                   m_UseExfiltrationSZVar;
          m_UseExfiltrationUZFromGUVar = OPENFLUID_IsVariableExist(GU,"water.uz-surf.H.exfiltration") && 
                                         m_UseExfiltrationSZVar;
        }
      }


      float Cel, Sigma;

      openfluid::core::DoubleValue TmpValue;
      openfluid::core::DoubleValue Area;
      unsigned int SUCount = 0;
      unsigned int GUCount = 0;

      m_MeanSlope = 0;
      m_MeanManning = 0;

      OPENFLUID_GetUnitsCount("SU",SUCount);

      // Computing mean slope and manning coefficient on SUs
      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {

        ID = SU->getID();

        m_Input[ID] = new openfluid::core::SerieOfDoubleValue();
        m_CurrentInputSum[ID] = 0;

        OPENFLUID_GetAttribute(SU,"slope",TmpValue);
        m_MeanSlope = m_MeanSlope + TmpValue;
        OPENFLUID_GetAttribute(SU,"nmanning",TmpValue);
        m_MeanManning = m_MeanManning + TmpValue;

      }

      m_MeanSlope = m_MeanSlope / SUCount;
      m_MeanManning = m_MeanManning / SUCount;


      // Distribution of celerity and diffusivity on each SU according to slope and manning coefficient
      unsigned int DeltaT =  OPENFLUID_GetDefaultDeltaT();
      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {

        OPENFLUID_GetAttribute(SU,"nmanning",TmpValue);
        OPENFLUID_GetAttribute(SU,"slope",Area);
        Cel = m_MeanCelerity * (m_MeanManning / TmpValue) * (sqrt((Area / m_MeanSlope)));
        Sigma = m_MeanSigma * (TmpValue / m_MeanManning) * (m_MeanSlope / Area);

        // Computing Hayami kernel
        m_SUKernel[SU->getID()] = t_HayamiKernel();
        OPENFLUID_GetAttribute(SU,"flowdist",TmpValue);
        ComputeHayamiKernel(Cel, Sigma,TmpValue,m_MaxSteps,DeltaT, m_SUKernel[SU->getID()]);
      }


      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {
        OPENFLUID_InitializeVariable(SU,"water.surf.Q.downstream-su",0.0);
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
      openfluid::core::DoubleValue QOutput, CurrentExfiltration, CurrentExfiltrationSZ, CurrentExfiltrationUZ;
      openfluid::core::DoubleValue QInput;
      openfluid::core::DoubleValue CurrentRunoff, Area, AreaGU;

      openfluid::core::SpatialUnit* SU;
      openfluid::core::SpatialUnit* GU;
      std::list<openfluid::core::SpatialUnit*>::iterator GUiter;
      std::list<openfluid::core::SpatialUnit*>* GUsList;

      DeltaT = OPENFLUID_GetDefaultDeltaT();
      CurrentStep = OPENFLUID_GetCurrentTimeIndex() / OPENFLUID_GetDefaultDeltaT();


      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {
        ID = SU->getID();

        CurrentExfiltration = 0;
        CurrentExfiltrationUZ = 0;
        CurrentExfiltrationSZ = 0;

        // Getting SU properties
        OPENFLUID_GetAttribute(SU,"area",Area);

        // Getting water input
        OPENFLUID_GetVariable(SU,"water.surf.H.runoff",CurrentRunoff);


        /************************************************************************************************************
          Adding exfiltration from soil
         ************************************************************************************************************/
        // Exfiltration from SU (simple model)
        if (m_UseExfiltrationUZFromSUVar)
        {
          OPENFLUID_GetVariable(SU,"water.uz-surf.Q.exfiltration",CurrentExfiltration);
        }

        // Exfiltration from multilayer model (UZ and SZ) GU
        if (m_UseExfiltrationSZVar || m_UseExfiltrationUZFromGUVar)
        {
          GUsList = SU->toSpatialUnits("GU");
          if (GUsList != NULL)
          {
            OPENFLUID_UNITSLIST_LOOP(GUsList,GU)
            {

              OPENFLUID_GetAttribute(GU,"area",AreaGU);
              if (m_UseExfiltrationSZVar)
              {
                OPENFLUID_GetVariable(GU,"water.sz-surf.H.exfiltration",CurrentExfiltrationSZ);
              }
              if (m_UseExfiltrationUZFromGUVar)
              {
                OPENFLUID_GetVariable(GU,"water.uz-surf.H.exfiltration",CurrentExfiltrationUZ);
              }
              CurrentExfiltration = CurrentExfiltration + 
                                    (((CurrentExfiltrationSZ + CurrentExfiltrationUZ) * AreaGU) / DeltaT);
            }
          }
        }


        /************************************************************************************************************
          Computing input water volume
         ************************************************************************************************************/
        QInput = (CurrentRunoff * Area / DeltaT) + CurrentExfiltration;
        m_CurrentInputSum[ID] = m_CurrentInputSum[ID] + QInput;
        //TODO check m_Input size growth
        m_Input[ID]->push_back(QInput);


        /************************************************************************************************************
          Doing Hayami propagation
         ************************************************************************************************************/
        QOutput = 0;
        if (m_CurrentInputSum[ID] > 0)
        {
          //TODO check given CurrentStep
          QOutput = DoHayamiPropagation(m_SUKernel[ID], CurrentStep-1, m_Input[ID], m_MaxSteps, DeltaT);
        }
        OPENFLUID_AppendVariable(SU,"water.surf.Q.downstream-su",QOutput);

        // Checking produced variable consistency
        std::string IDStr;
        openfluid::tools::convertValue(ID,&IDStr);
        if (QOutput < 0)
        {
          OPENFLUID_RaiseError("The produced variable output volume at the outlet of the SU " + IDStr + 
                               " (water.surf.Q.downstream-su) is negative.");
        }

      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {
      openfluid::core::SpatialUnit* SU;
      
      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {
        delete m_Input[SU->getID()];
      }

    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(HayamiSUSimulator);

