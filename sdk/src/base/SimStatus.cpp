/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "SimStatus.h"
#include <iostream>

namespace mhydasdk { namespace base {


SimulationInfo::SimulationInfo(wxDateTime StartTime,
                               wxDateTime EndTime,
                               int TimeStep)

{
  wxTimeSpan DeltaTime;
  
  m_StartTime = StartTime;
  m_EndTime = EndTime;

  m_TimeStep = TimeStep;
  m_TimeStepSpan = wxTimeSpan(0,0,m_TimeStep,0);

  
  DeltaTime = EndTime-StartTime;  
  m_StepsCount = int(((DeltaTime.GetSeconds().ToLong())) / TimeStep) + 1;  
  
}

// =====================================================================
// =====================================================================

SimulationInfo::~SimulationInfo()
{

}



// =====================================================================
// =====================================================================



SimulationStatus::SimulationStatus(wxDateTime StartTime,
                                   wxDateTime EndTime,
                                   int TimeStep)
                : SimulationInfo(StartTime,EndTime,TimeStep)                   

{

  m_CurrentStep = 0;
  m_CurrentTime = m_StartTime;
  
  m_IsFirstStep = true;
    
  m_IsLastStep = false;        
}

// =====================================================================
// =====================================================================

SimulationStatus::~SimulationStatus()
{

}

// =====================================================================
// =====================================================================



bool SimulationStatus::switchToNextStep()
{
  wxDateTime NextTime(m_CurrentTime + m_TimeStepSpan);
  
  if (NextTime < m_EndTime)
  {
    m_CurrentStep++;

    m_CurrentTime = NextTime;
   
    m_IsFirstStep = (m_CurrentStep == 0);    

    // the new step is the last one if (new time + time step value) is after end time 
    m_IsLastStep = (wxDateTime(NextTime + m_TimeStepSpan) >  m_EndTime);
    
    return true;
  }
  else return false;

}


} } // namespace mhydasdk::core
