/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "SimStatus.h"
#include <iostream>

namespace openfluid { namespace base {


SimulationInfo::SimulationInfo(openfluid::core::DateTime StartTime,
                               openfluid::core::DateTime EndTime,
                               int TimeStep)

{


  openfluid::core::rawtime_t DeltaTime;

  m_StartTime = StartTime;
  m_EndTime = EndTime;

  m_TimeStep = TimeStep;

  DeltaTime = EndTime.diffInSeconds(StartTime);
  m_StepsCount = int(DeltaTime / TimeStep);
  if ((DeltaTime % TimeStep) != 0) m_StepsCount++;





}

// =====================================================================
// =====================================================================

SimulationInfo::~SimulationInfo()
{

}



// =====================================================================
// =====================================================================



SimulationStatus::SimulationStatus(openfluid::core::DateTime StartTime,
                                   openfluid::core::DateTime EndTime,
                                   int TimeStep)
                : SimulationInfo(StartTime,EndTime,TimeStep)

{

  m_CurrentStep = 0;
  m_CurrentTime = m_StartTime;

  m_IsFirstStep = true;

  m_IsLastStep = false;
  if (m_StepsCount == 1) m_IsLastStep = true;


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
  openfluid::core::DateTime NextTime(m_CurrentTime + m_TimeStep);

  if (NextTime < m_EndTime)
  {
    m_CurrentStep++;

    m_CurrentTime = NextTime;

    m_IsFirstStep = (m_CurrentStep == 0);
    m_IsLastStep = (m_CurrentStep == (m_StepsCount-1));

    return true;
  }
  else return false;

}


} } // namespace openfluid::core

