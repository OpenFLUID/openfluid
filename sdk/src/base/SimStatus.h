/**
  \file SimStatus.h
  \brief Header of simulation status management class

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __SIMSTATUS_H__
#define __SIMSTATUS_H__


#include "openfluid-core.h"


namespace openfluid { namespace base {


/**
  Class ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class SimulationInfo
{
  protected:
    openfluid::core::DateTime m_StartTime;
    openfluid::core::DateTime m_EndTime;
    int m_TimeStep;

    unsigned int m_StepsCount;



  public:

    /**
      Constructor
    */
    SimulationInfo(openfluid::core::DateTime StartTime,
                   openfluid::core::DateTime EndTime,
                   int TimeStep);

    /**
      Destructor
    */
    ~SimulationInfo();

    int getTimeStep() const { return m_TimeStep; };

    unsigned int getStepsCount() const { return m_StepsCount; };

    openfluid::core::DateTime getStartTime() const { return m_StartTime; };
    openfluid::core::DateTime getEndTime() const { return m_EndTime; };


};

/**

*/
class SimulationStatus : public SimulationInfo
{
  private:

    unsigned int m_CurrentStep;

    openfluid::core::DateTime m_CurrentTime;

    bool m_IsFirstStep;

    bool m_IsLastStep;

  public:
    /**
      Constructor
    */
    SimulationStatus(openfluid::core::DateTime StartTime,
                     openfluid::core::DateTime EndTime,
                     int TimeStep);


    /**
      Destructor
    */
    ~SimulationStatus();


    bool switchToNextStep();

    unsigned int getCurrentStep() const { return m_CurrentStep; };

    openfluid::core::DateTime getCurrentTime() const { return m_CurrentTime; };

    bool isFirstStep() const { return m_IsFirstStep; };

    bool isLastStep() const { return m_IsLastStep; };


};



} } // namespace openfluid::core


#endif




