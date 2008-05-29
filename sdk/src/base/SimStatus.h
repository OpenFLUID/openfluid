/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __SIMSTATUS_H__
#define __SIMSTATUS_H__


#include "mhydasdk-core.h"
#include "wx/datetime.h"

namespace mhydasdk { namespace base {


/**
  Class ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class SimulationInfo 
{
  protected:
    wxDateTime m_StartTime;
    wxDateTime m_EndTime;
    int m_TimeStep;
    wxTimeSpan m_TimeStepSpan;
    
    unsigned int m_StepsCount;
  
    
    
  public:
    
    /**
      Constructor
    */
    SimulationInfo(wxDateTime StartTime,
                   wxDateTime EndTime,
                   int TimeStep);

    /**
      Destructor
    */
    ~SimulationInfo();    
  
    int getTimeStep() const { return m_TimeStep; };
    
    unsigned int getStepsCount() const { return m_StepsCount; };
    
    wxDateTime getStartTime() const { return m_StartTime; };
    wxDateTime getEndTime() const { return m_EndTime; };
    
    
};

/**

*/
class SimulationStatus : public SimulationInfo
{
  private:

    unsigned int m_CurrentStep;

    wxDateTime m_CurrentTime;
    
    bool m_BidouilledTime;

    bool m_IsFirstStep;
    
    bool m_IsLastStep;    

  public:
    /**
      Constructor
    */
    SimulationStatus(wxDateTime StartTime,
                     wxDateTime EndTime,
                     int TimeStep);


    /**
      Destructor
    */
    ~SimulationStatus();


    bool switchToNextStep();

    unsigned int getCurrentStep() const { return m_CurrentStep; };

    wxDateTime getCurrentTime() const { return m_CurrentTime; };

    bool isFirstStep() const { return m_IsFirstStep; };

    bool isLastStep() const { return m_IsLastStep; };
            

};



} } // namespace mhydasdk::core


#endif




