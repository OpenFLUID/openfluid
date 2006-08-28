/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#include "SimStatus.h"

namespace mhydasdk { namespace base {


SimulationStatus::SimulationStatus(mhydasdk::core::DateTime StartTime,
                                   mhydasdk::core::DateTime EndTime,
                                   int TimeStep)

{
  m_StartTime = StartTime;
  m_EndTime = EndTime;

  m_TimeStep = TimeStep;

  m_CurrentStep = 0;
  m_CurrentTime = m_StartTime;
}

// =====================================================================
// =====================================================================

SimulationStatus::~SimulationStatus()
{

}


bool SimulationStatus::switchToNextStep()
{
  mhydasdk::core::DateTime NextTime(m_CurrentTime.getRawTime() + m_TimeStep);

  if (NextTime.getRawTime() <=  m_EndTime.getRawTime())
  {
    m_CurrentStep++;
    m_CurrentTime = mhydasdk::core::DateTime(m_CurrentTime.getRawTime() + m_TimeStep);
    return true;
  }
  else return false;

}


} } // namespace mhydasdk::core
