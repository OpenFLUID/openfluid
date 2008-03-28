/**
  \file HayamiRSFunc.h
  \brief header of HayamiRSFunction

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
 */


#include "HayamiRSGUFunc.h"
#include <math.h>

#include <iostream>

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_HOOK(HayamiRSFunction);


// =====================================================================
// =====================================================================

BEGIN_SIGNATURE_HOOK;
  DECLARE_SIGNATURE_ID(wxT("water.surf.transfer-rs-exchange-gu.hayami-tank"));
  DECLARE_SIGNATURE_NAME(wxT("water transfer on reach segments using hayami propagation method; surface water/groundwater exchanges + exchanges betwen groundwater units"));
  DECLARE_SIGNATURE_DESCRIPTION(wxT("WARNING: UNIQUE BUFFER WITHOUT TAKING INTO ACCOUNT THE HEIGHT OF THE REACHES"));
  DECLARE_SIGNATURE_DOMAIN(wxT("hydrology"));
  
  DECLARE_SIGNATURE_STATUS(mhydasdk::base::BETA);  
  
  DECLARE_SIGNATURE_SDKVERSION;
  
  DECLARE_SIGNATURE_AUTHORNAME(wxT("Moussa. R, Dages C., Louchart X., Fabre J.-C."));
  DECLARE_SIGNATURE_AUTHOREMAIL(wxT("moussa@supagro.inra.fr, dages@supagro.inra.fr, louchart@supagro.inra.fr, fabrejc@supagro.inra.fr"));  
  
  
  
  // Produced variables
  DECLARE_RS_PRODUCED_VAR("water.surf.Q.downstream-rs",wxT("Output volume at the outlet of the ditch"),wxT("m3/s"));  
  DECLARE_RS_PRODUCED_VAR("water.surf.H.level-rs",wxT("Water height at the outlet of the ditch"),wxT("m"));  
  DECLARE_RS_PRODUCED_VAR("water.surf-sz.Q.exchange-rs-gu",wxT("water flux exchanged betwwen RS and GU unit"),wxT("m3/s"));
  
  DECLARE_GU_PRODUCED_VAR("water.sz.Q.output",wxT("water flux output of the GU"),wxT("m3/s"));
  DECLARE_GU_PRODUCED_VAR("water.sz-surf.Q.exfiltration",wxT("exfiltration from GU to RS"),wxT("m3/s/m2"));
  DECLARE_GU_PRODUCED_VAR("water.sz.H.watertable",wxT("watertable depth, positive value from soil surface"),wxT("m"));
  
  // Required variables
  DECLARE_SU_REQUIRED_VAR("water.surf.H.infiltration",wxT(""),wxT("m"));
  DECLARE_SU_USED_VAR("water.surf.Q.downstream-su",wxT("output volume at the outlet of the SUs"),wxT("m3/s"));    
  
  
  // Required initial conditions
  DECLARE_SU_REQUIRED_INICOND("thetains",wxT(""),wxT("m3/m3"));
  DECLARE_GU_REQUIRED_INICOND("iniwatertable",wxT("initial watertable depth, positive value from soil surface"),wxT("m"));
  
  // Required properties
  DECLARE_SU_REQUIRED_PROPERTY("thetasat",wxT(""),wxT("m3/m3"));
  DECLARE_RS_REQUIRED_PROPERTY("nmanning",wxT("Manning roughness coefficient"),wxT(""));
  
  
  // Function parameters
  DECLARE_FUNCTION_PARAM("maxsteps",wxT("maximum hayami kernel steps"),wxT(""));
  DECLARE_FUNCTION_PARAM("meancel",wxT("wave mean celerity on RSs"),wxT("m/s"));  
  DECLARE_FUNCTION_PARAM("meansigma",wxT("mean diffusivity on RSs"),wxT("m2/s"));  
  DECLARE_FUNCTION_PARAM("calibstep",wxT("calibration step for height-discharge relation"),wxT("m"));    
  DECLARE_FUNCTION_PARAM("rsbuffer",wxT("buffer upon reach for water heigh over reach height"),wxT("m"));  
  
  DECLARE_FUNCTION_PARAM("coeffinfiltration",wxT("multiplicative exchange coefficient when watertable level is under surface level"),wxT(""));
  DECLARE_FUNCTION_PARAM("coeffdrainage",wxT("multiplicative exchange coefficient when watertable level is above surface level"),wxT(""));
  DECLARE_FUNCTION_PARAM("coeffgw",wxT("multiplicative exchange coefficient between GUs"),wxT(""));

END_SIGNATURE_HOOK;


HayamiRSFunction::HayamiRSFunction()
: PluggableFunction()
{



  // default values
  m_MaxSteps = 100;    
  m_MeanCelerity = 0.49;    
  m_MeanSigma = 500;
  m_MeanSlope = 0;    
  m_MeanManning = 0;        
  m_CalibrationStep = 0.01;
  m_RSBuffer = 0;

  m_CoeffInfiltration = 1;
  m_CoeffDrainage = 1;
  m_CoeffGW = 1;

}


// =====================================================================
// =====================================================================


HayamiRSFunction::~HayamiRSFunction()
{

}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::initParams(mhydasdk::core::ParamsMap Params)
{
  MHYDAS_GetFunctionParam(Params,wxT("maxsteps"),&m_MaxSteps);  
  MHYDAS_GetFunctionParam(Params,wxT("meancel"),&m_MeanCelerity);
  MHYDAS_GetFunctionParam(Params,wxT("meansigma"),&m_MeanSigma);
  MHYDAS_GetFunctionParam(Params,wxT("calibstep"),&m_CalibrationStep);
  MHYDAS_GetFunctionParam(Params,wxT("rsbuffer"),&m_RSBuffer);
  MHYDAS_GetFunctionParam(Params,wxT("coeffinfiltration"),&m_CoeffInfiltration);
  MHYDAS_GetFunctionParam(Params,wxT("coeffdrainage"),&m_CoeffDrainage);
  MHYDAS_GetFunctionParam(Params,wxT("coeffgw"),&m_CoeffGW);
  return true;
}

// =====================================================================
// =====================================================================


bool HayamiRSFunction::prepareData()
{
  // On verifie s'il existe des SU pour recuperer leur debit

  m_UseUpSUOutput = false;   
  if (mp_CoreData->getSpatialData()->getSUsCollection()->size() > 0)
  {
    m_UseUpSUOutput = true;
  } 

  return true;
}

// =====================================================================
// =====================================================================


bool HayamiRSFunction::checkConsistency()
{

  return true;
}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)
{
  std::list<mhydasdk::core::SurfaceUnit*>* SUList;
  std::list<mhydasdk::core::SurfaceUnit*>::iterator Iter;
  mhydasdk::core::GroundwaterUnit *GU;
  mhydasdk::core::SurfaceUnit *SU;
  mhydasdk::core::ReachSegment *RS;

  float Cel, Sigma;
  mhydasdk::core::PropertyValue TmpValue;
  int ID;
  mhydasdk::core::PropertyValue SUThetaIni, SUThetaSat;

  DECLARE_GU_ORDERED_LOOP;

  DECLARE_RS_ORDERED_LOOP;


  BEGIN_RS_ORDERED_LOOP(RS)
  ID = RS->getID();  

  m_Input[ID] = new mhydasdk::core::MHYDASVectorValue();
  m_HeightDischarge[ID] = new mhydasdk::core::MHYDASVectorValue();
  m_CurrentInputSum[ID] = 0;

  m_MeanSlope = m_MeanSlope + RS->getUsrSlope();
  MHYDAS_GetDistributedProperty(RS,wxT("nmanning"),&TmpValue);
  m_MeanManning = m_MeanManning + TmpValue;
  //m_MeanManning = m_MeanManning + RS->getProperties()->find(wxT("nmanning"))->second;    
  END_LOOP

  m_MeanSlope = m_MeanSlope / mp_CoreData->getSpatialData()->getRSsCollection()->size();
  m_MeanManning = m_MeanManning / mp_CoreData->getSpatialData()->getRSsCollection()->size(); 

  BEGIN_RS_ORDERED_LOOP(RS)
  MHYDAS_GetDistributedProperty(RS,wxT("nmanning"),&TmpValue);
  //    Cel = m_MeanCelerity * (m_MeanManning / RS->getProperties()->find(wxT("nmanning"))->second) * (sqrt((RS->getUsrSlope() / m_MeanSlope)));
  //    Sigma = m_MeanSigma * (RS->getProperties()->find(wxT("nmanning"))->second / m_MeanManning) * (m_MeanSlope / RS->getUsrSlope());      
  Cel = m_MeanCelerity * (m_MeanManning / TmpValue) * (sqrt((RS->getUsrSlope() / m_MeanSlope)));
  Sigma = m_MeanSigma * (TmpValue / m_MeanManning) * (m_MeanSlope / RS->getUsrSlope());      
  ComputeHayamiKernel(Cel, Sigma,RS->getUsrLength(),m_MaxSteps,SimInfo->getTimeStep(),&m_RSKernel[RS->getID()]);        
  END_LOOP


  // calcul de la relation hauteur débit

  double HydrauRadius, Speed, Q, Section, CurrentHeight;
  int i, StepsNbr;

  BEGIN_RS_ORDERED_LOOP(RS)

  MHYDAS_GetDistributedProperty(RS,wxT("nmanning"),&TmpValue);    

  // StepsNbr = int(ceil((RS->getUsrHeight() + m_RSBuffer) / m_CalibrationStep));
  StepsNbr = int(ceil((m_RSBuffer) / m_CalibrationStep));
  
  //std::cerr << " ATTENTION VERSION AVEC BUFFER UNIQUE SANS TENIR COMPTE HAUTEUR FOSSE " <<std::endl;
  
  // TODO ATTENTION VERSION AVEC BUFFER UNIQUE SANS TENIR COMPTE HAUTEUR FOSSE 
  
  
  
  for (i=0;i<StepsNbr;i++)
  {
    CurrentHeight = i * m_CalibrationStep;

    Section = RS->getUsrWidth() * CurrentHeight;
    HydrauRadius = Section / ((2 * CurrentHeight) + RS->getUsrWidth());
    Speed = (1/TmpValue) * pow(HydrauRadius,0.6666) * pow(RS->getUsrSlope(),0.5);   
    Q = Speed * Section;

    m_HeightDischarge[RS->getID()]->push_back(Q);
    
  }
  

  END_LOOP

  BEGIN_GU_ORDERED_LOOP(GU)
  /* computes ThetaSat and ThetaIni for each GU from ThetaSat and ThetaIniNS of all SUs that exchange with GU
   * ThetaSat and ThetaIni are the average of ThetaSat and ThetaIniNS of SUs weighted by the area of each SU
   */

  SUList = GU->getSUsExchange();

  m_ThetaIni[GU->getID()] = 0;
  m_ThetaSat[GU->getID()] = 0;

  for (Iter=SUList->begin(); Iter != SUList->end(); Iter++)
  {
    SU = *Iter;
    MHYDAS_GetDistributedIniCondition(SU,wxT("thetains"),&SUThetaIni);
    MHYDAS_GetDistributedProperty(SU,wxT("thetasat"),&SUThetaSat);
    m_ThetaIni[GU->getID()] = m_ThetaIni[GU->getID()] + SUThetaIni*SU->getUsrArea();
    m_ThetaSat[GU->getID()] = m_ThetaSat[GU->getID()] + SUThetaSat*SU->getUsrArea();  
  }
  // the final sum is divided by the area of the GU
  m_ThetaIni[GU->getID()] = m_ThetaIni[GU->getID()] / GU->getUsrArea();
  m_ThetaSat[GU->getID()] = m_ThetaSat[GU->getID()] / GU->getUsrArea();       

  END_LOOP


  return true;
}



// =====================================================================
// =====================================================================


bool HayamiRSFunction::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{
  int ID;
  int CurrentStep;
  int TimeStep;
  int i;
  float UpSrcSUsOutputsSum;  
  float UpLatSUsOutputsSum;  
  float UpRSsOutputsSum;  
  mhydasdk::core::MHYDASScalarValue QOutput;
  float QInput;
  mhydasdk::core::MHYDASScalarValue TmpValue;
  float TmpGUValue;
  bool m_UseUpGUExchangersgu;

  mhydasdk::core::MHYDASScalarValue WaterTable, WaterTableDown,QguOutput;
  float  InputVol,ExchangeSurface,  OutputVol;
  mhydasdk::core::MHYDASScalarValue TmpExfiltration, Exfiltration;
  mhydasdk::core::MHYDASScalarValue  SUInfiltration; // for SU
  mhydasdk::core::MHYDASScalarValue TmpQExchange;
  float RSHeight,WaterHeight ; // for RS


  mhydasdk::core::ReachSegment* RS;
  mhydasdk::core::ReachSegment* UpRS;
  mhydasdk::core::SurfaceUnit* UpSU;
  mhydasdk::core::GroundwaterUnit *GU, *GUex, *GUdown,*GURS;
  mhydasdk::core::SurfaceUnit *SU;

  std::list<mhydasdk::core::GroundwaterUnit*>* GUList;
  std::list<mhydasdk::core::GroundwaterUnit*>::iterator IterGW;
  std::list<mhydasdk::core::SurfaceUnit*>* SUList;
  std::list<mhydasdk::core::SurfaceUnit*>::iterator IterSU;
  list<mhydasdk::core::SurfaceUnit*>::iterator UpSUiter;
  list<mhydasdk::core::SurfaceUnit*>* UpSUsList;
  list<mhydasdk::core::ReachSegment*>::iterator UpRSiter;
  list<mhydasdk::core::ReachSegment*>* UpRSsList;  
  std::list<mhydasdk::core::ReachSegment*>* RSList;
  std::list<mhydasdk::core::ReachSegment*>::iterator IterRS;






  TimeStep = SimStatus->getTimeStep();
  CurrentStep = SimStatus->getCurrentStep();

  DECLARE_RS_ORDERED_LOOP;
  DECLARE_GU_ORDERED_LOOP;

  if (CurrentStep == 0)
  {
    //exchange between GUs       
    BEGIN_GU_ORDERED_LOOP(GU)
    // water level initializing
    ID = GU->getID();

    MHYDAS_GetDistributedIniCondition(GU,wxT("iniwatertable"),&WaterTable);

    MHYDAS_AppendDistributedVarValue(GU,wxT("water.sz.Q.output"),0);
    MHYDAS_AppendDistributedVarValue(GU,wxT("water.sz-surf.Q.exfiltration"),0);
    
    // Computes recharge from SUs
    InputVol = 0;

    SUList = GU->getSUsExchange();

    for (IterSU=SUList->begin(); IterSU != SUList->end(); IterSU++)
    {
      SU = *IterSU;
      MHYDAS_GetDistributedVarValue(SU,wxT("water.surf.H.infiltration"),SimStatus->getCurrentStep(),&SUInfiltration);
      InputVol = InputVol + SUInfiltration * SU->getUsrArea();
    }
    // Updates WaterTable according to the recharge
    WaterTable = WaterTable - (InputVol / (GU->getUsrArea()*(m_ThetaSat[GU->getID()] - m_ThetaIni[GU->getID()])));
    MHYDAS_AppendDistributedVarValue(GU,wxT("water.sz.H.watertable"),WaterTable);

    END_LOOP


    BEGIN_RS_ORDERED_LOOP(RS)

    ID = RS->getID();
    TmpQExchange = 0;
    MHYDAS_AppendDistributedVarValue(RS,wxT("water.surf-sz.Q.exchange-rs-gu"),TmpQExchange);

    END_LOOP   

  }




  BEGIN_RS_ORDERED_LOOP(RS)

  ID = RS->getID();

  QOutput = 0;

  // 1.a calcul du debit provenant des SU sources qui se jettent dans les noeuds sources


  UpSrcSUsOutputsSum = 0;
  if (m_UseUpSUOutput)
  {      
    UpSUsList = RS->getSrcUpstreamSUs();

    for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
    {                
      UpSU = *UpSUiter;
      MHYDAS_GetDistributedVarValue(UpSU,wxT("water.surf.Q.downstream-su"),CurrentStep,&TmpValue);
      UpSrcSUsOutputsSum = UpSrcSUsOutputsSum + TmpValue; // / UpSU->getUsrArea();

      if (CurrentStep != 0)
      {        
        GUex = mp_CoreData->getSpatialData()->getGUByID(UpSU->getGUExchangeID());
        MHYDAS_GetDistributedVarValue(GUex,wxT("water.sz-surf.Q.exfiltration"),CurrentStep-1,&TmpExfiltration);
        UpSrcSUsOutputsSum = UpSrcSUsOutputsSum + TmpExfiltration* UpSU->getUsrArea();      
      }
    }  
  }



  // 1.b calcul du debit provenant des SU laterales

  UpLatSUsOutputsSum = 0;
  if (m_UseUpSUOutput)
  {      
    UpSUsList = RS->getLatUpstreamSUs();

    for(UpSUiter=UpSUsList->begin(); UpSUiter != UpSUsList->end(); UpSUiter++)
    {                
      UpSU = *UpSUiter;

      MHYDAS_GetDistributedVarValue(UpSU,wxT("water.surf.Q.downstream-su"),CurrentStep,&TmpValue);        
      UpLatSUsOutputsSum = UpLatSUsOutputsSum + TmpValue;// / UpSU->getUsrArea();  

      if (CurrentStep != 0)
      {        
        GUex = mp_CoreData->getSpatialData()->getGUByID(UpSU->getGUExchangeID());        
        MHYDAS_GetDistributedVarValue(GUex,wxT("water.sz-surf.Q.exfiltration"),CurrentStep-1,&TmpExfiltration);
        UpLatSUsOutputsSum = UpLatSUsOutputsSum + TmpExfiltration* UpSU->getUsrArea();      
      }

    }  
  }


  // 2.a calcul des debits provenant des RS amont


  UpRSsOutputsSum = 0;

  UpRSsList = RS->getUpstreamReaches();

  for(UpRSiter=UpRSsList->begin(); UpRSiter != UpRSsList->end(); UpRSiter++) \
  {                
    UpRS = *UpRSiter;
    MHYDAS_GetDistributedVarValue(UpRS,wxT("water.surf.Q.downstream-rs"),CurrentStep,&TmpValue);
    UpRSsOutputsSum = UpRSsOutputsSum + TmpValue;                
  }    



  // 2.b propagation via Hayami


  QInput = UpRSsOutputsSum + UpSrcSUsOutputsSum; 
  m_CurrentInputSum[ID] = m_CurrentInputSum[ID] + QInput;
  m_Input[ID]->push_back(QInput);

  QOutput = 0;
  if (m_CurrentInputSum[ID] > 0)
  {    
    QOutput = DoHayamiPropagation(m_RSKernel[ID], CurrentStep, m_Input[ID], m_MaxSteps, TimeStep);
  }  

  QOutput = QOutput + UpLatSUsOutputsSum;


  // MHYDAS_AppendDistributedVarValue(RS,wxT("qoutput"),QOutput);

  //3. Premier calcul de la hauteur d'eau 

  //if (!computeWaterHeightFromDischarge(ID,QOutput,&TmpValue)) std::cerr << "ça dépasse ID: " << ID <<std::endl; 
  if (!computeWaterHeightFromDischarge(ID,QOutput,&WaterHeight)) 
    mp_ExecMsgs->addWarning(wxT("water.surf.transfer-rs-exchange-gu.hayami-tank"),CurrentStep,wxT("water height is over reach height + buffer on RS ") + wxString::Format(wxT("%d"),ID));

  //MHYDAS_AppendDistributedVarValue(RS,wxT("waterheight"),WaterHeight);

  
  // Echanges nappes-rivières       
  //   4.a. Calcul du débit échangé entre la rivière et l'aquifère


  GURS = RS->getGUExchange();

  //MHYDAS_GetDistributedVarValue(RS,wxT("waterheight"),CurrentStep,&WaterHeight);
  MHYDAS_GetDistributedVarValue(GURS,wxT("water.sz.H.watertable"),CurrentStep-1,&WaterTable);


  ExchangeSurface = RS->getUsrLength() * ((2 * WaterHeight) + RS->getUsrWidth()); 

  /*  mp_ExecMsgs->setError(wxT("groundwater"),SimStatus->getCurrentStep(),wxT("watertable not present"));
          return false;
   */

  // computes water flux between RS and GU (qexchangersgu)
  RSHeight = RS->getUsrHeight();

  TmpQExchange = 0;

  if (WaterTable <= RSHeight)
  {
    TmpQExchange = m_CoeffDrainage* (RSHeight - WaterHeight - WaterTable) * ExchangeSurface;
  }
  else
  {
    TmpQExchange = -m_CoeffInfiltration * WaterHeight * ExchangeSurface;    
  }

  // 4.b. Calcul du bilan à l'échelle du troncon nTR puis calcul de la nouvelle hauteur d'eau   
  if ((-TmpQExchange) > QOutput)
  {
    TmpQExchange = -QOutput;
    QOutput = 0;
  }
  else
  {
    QOutput = QOutput + TmpQExchange;
  }


  if (!computeWaterHeightFromDischarge(ID,QOutput,&WaterHeight)) 
    mp_ExecMsgs->addWarning(wxT("water.surf.transfer-rs-exchange-gu.hayami-tank"),SimStatus->getCurrentStep(),wxT("overflow on RS") + wxString::Format(wxT("%d"),ID));

  MHYDAS_AppendDistributedVarValue(RS,wxT("water.surf.Q.downstream-rs"),QOutput);

  MHYDAS_AppendDistributedVarValue(RS,wxT("water.surf.H.level-rs"),WaterHeight);
  if (CurrentStep != 0)   MHYDAS_AppendDistributedVarValue(RS,wxT("water.surf-sz.Q.exchange-rs-gu"),TmpQExchange);
  

  END_LOOP

  if (CurrentStep != 0)
  {
    /* =======================================================================
     * exchange between GUs
     * ======================================================================= */       
    BEGIN_GU_ORDERED_LOOP(GU)


    // Computes recharge from SUs
    InputVol = 0;
    OutputVol = 0;

    SUList = GU->getSUsExchange();

    for (IterSU=SUList->begin(); IterSU != SUList->end(); IterSU++)
    {
      SU = *IterSU;
      MHYDAS_GetDistributedVarValue(SU,wxT("water.surf.H.infiltration"),CurrentStep,&SUInfiltration);
      InputVol = InputVol + (SUInfiltration * SU->getUsrArea());

    }

    // Add to the recharge upGUs fluxes
    GUList = GU->getGUsExchange();
    for (IterGW=GUList->begin(); IterGW != GUList->end(); IterGW++)
    {
      GUex = *IterGW;
      MHYDAS_GetDistributedVarValue(GUex,wxT("water.sz.Q.output"),CurrentStep-1,&QguOutput);
      InputVol = InputVol + QguOutput*TimeStep;
    }

    // computes the Output Volume in relation to the RSs
    RSList = GU->getRSsExchange();
    for (IterRS=RSList->begin(); IterRS != RSList->end(); IterRS++)
    {
      RS = *IterRS;



      MHYDAS_GetDistributedVarValue(RS,wxT("qexchangersgu"),CurrentStep,&TmpQExchange);
      OutputVol = OutputVol + TmpQExchange*TimeStep;


    }

    //updates the Output Volume with the downGU
    // get WaterTable(t-1)
    MHYDAS_GetDistributedVarValue(GU,wxT("water.sz.H.watertable"),CurrentStep-1,&WaterTable);

    QguOutput = 0;

    if (GU->getGUExchange() != NULL)
    {       
      GUdown = GU->getGUExchange(); 
      MHYDAS_GetDistributedVarValue(GUdown,wxT("water.sz.H.watertable"),CurrentStep-1,&WaterTableDown); // Watertable on downGU
      QguOutput = m_CoeffGW * (WaterTableDown - WaterTable);
      OutputVol = OutputVol + QguOutput*TimeStep;


    }
    else
    {
      QguOutput = 0;  
    }






    MHYDAS_AppendDistributedVarValue(GU,wxT("water.sz.Q.output"),QguOutput);

    // Updates WaterTable(t) according to the Input, Output volumes, and WaterTable(t-1)
    WaterTable = WaterTable - (InputVol - OutputVol) / (GU->getUsrArea()*(m_ThetaSat[GU->getID()] - m_ThetaIni[GU->getID()]));




    // Computes exfiltration if necessary, and updates WaterTable
    if (WaterTable < 0)
    {
      Exfiltration = -WaterTable * (m_ThetaSat[GU->getID()] - m_ThetaIni[GU->getID()]) / SimStatus->getTimeStep();
      WaterTable = 0;
    }
    else
    {
      Exfiltration = 0;
    }

    // TODO Exfiltration à rajouter au ruissellement des SU     


    // Append final WaterTable(t) and Exfiltration(t)
    MHYDAS_AppendDistributedVarValue(GU,wxT("water.sz.H.watertable"),WaterTable);
    MHYDAS_AppendDistributedVarValue(GU,wxT("water.sz-surf.Q.exfiltration"),Exfiltration);


    END_LOOP    
  }
  return true;
}


// =====================================================================
// =====================================================================


bool HayamiRSFunction::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)
{

  return true;
}

// =====================================================================
// =====================================================================

bool HayamiRSFunction::computeWaterHeightFromDischarge(int ID, float Discharge, float *Height)
{

  if (Discharge < 0) return false;
  if (Discharge == 0) *Height = 0;
  else
  {

    int i;
    float Q1, Q2, H1, H2;

    mhydasdk::core::MHYDASVectorValue* HeightDischarge = m_HeightDischarge[ID]; 


    // on determine par boucle le premier débit de la relation H/D supérieur au débit recherché
    i = 0;  

    while ((i < HeightDischarge->size()) && (HeightDischarge->at(i) < Discharge))
    {
      i++;            
    }


    if (i == HeightDischarge->size())
    {	
      *Height = (i-1) * m_CalibrationStep;
      return false;   
    }
    else
    {	
      Q1 = HeightDischarge->at(i-1);
      Q2 = HeightDischarge->at(i);

      H1 = (i-1) * m_CalibrationStep;
      H2 = i * m_CalibrationStep;

      // risque de division par 0 si Q1 == Q2 !! à revoir, comment fait-on?
      *Height = H1 + ((Discharge-Q1) * (H2-H1) / (Q2-Q1));
    }    

  }

  return true;

}
