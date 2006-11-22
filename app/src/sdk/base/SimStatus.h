/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __SIMSTATUS_H__
#define __SIMSTATUS_H__


#include "sdk-core.h"

namespace mhydasdk { namespace base {

/**

*/
class SimulationStatus
{
  private:
    mhydasdk::core::DateTime m_StartTime;
    mhydasdk::core::DateTime m_EndTime;
    int m_TimeStep;

    unsigned int m_CurrentStep;

    mhydasdk::core::DateTime m_CurrentTime;

    bool m_IsFirstStep;
    
    bool m_IsLastStep;    

  public:
    /**
      Constructor
    */
    SimulationStatus(mhydasdk::core::DateTime StartTime,
                     mhydasdk::core::DateTime EndTime,
                     int TimeStep);


    /**
      Destructor
    */
    ~SimulationStatus();


    bool switchToNextStep();

    unsigned int getCurrentStep() const { return m_CurrentStep; };

    mhydasdk::core::DateTime getCurrentTime() const { return m_CurrentTime; };

    bool isFirstStep() const { return m_IsFirstStep; };

    bool isLastStep() const { return m_IsLastStep; };
    
    int getTimeStep() const { return m_TimeStep; };    

};



} } // namespace mhydasdk::core


#endif




