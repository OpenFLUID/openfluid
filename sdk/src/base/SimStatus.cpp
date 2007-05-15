/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "SimStatus.h"

namespace mhydasdk { namespace base {


SimulationInfo::SimulationInfo(mhydasdk::core::DateTime StartTime,
                               mhydasdk::core::DateTime EndTime,
                               int TimeStep)

{
  m_StartTime = StartTime;
  m_EndTime = EndTime;

  m_TimeStep = TimeStep;

  m_StepsCount = ((EndTime.getRawTime() - StartTime.getRawTime()) / TimeStep) + 1;  
  
}

// =====================================================================
// =====================================================================

SimulationInfo::~SimulationInfo()
{

}



// =====================================================================
// =====================================================================



SimulationStatus::SimulationStatus(mhydasdk::core::DateTime StartTime,
                                   mhydasdk::core::DateTime EndTime,
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
  mhydasdk::core::DateTime NextTime(m_CurrentTime.getRawTime() + m_TimeStep);

  if (NextTime.getRawTime() <=  m_EndTime.getRawTime())
  {
    m_CurrentStep++;

    m_CurrentTime = mhydasdk::core::DateTime(m_CurrentTime.getRawTime() + m_TimeStep);
   
    m_IsFirstStep =  (m_CurrentStep == 0);
    m_IsLastStep = (NextTime.getRawTime() ==  m_EndTime.getRawTime());    
    
    return true;
  }
  else return false;

}


} } // namespace mhydasdk::core
