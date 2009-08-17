/**
  \file SimStatus.h
  \brief Header of simulation status management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __SIMSTATUS_H__
#define __SIMSTATUS_H__


#include "openfluid-core.h"


namespace openfluid { namespace base {


/**
  Class giving static information on simulation
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

    /**
      Returns the value of a time step (in seconds)

      \return the number of second in a time step
    */
    int getTimeStep() const { return m_TimeStep; };

    /**
      Returns the number of time step for the simulation

      \return the number of time steps
    */
    unsigned int getStepsCount() const { return m_StepsCount; };

    /**
      Returns the date and time of the beginning of the simulation

      \return the DateTime of the beginning of the simulation
    */
    openfluid::core::DateTime getStartTime() const { return m_StartTime; };

    /**
      Returns the date and time of the ending of the simulation

      \return the DateTime of the ending of the simulation
    */
    openfluid::core::DateTime getEndTime() const { return m_EndTime; };


};

/**
  Class giving dynamic information on simulation
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

    /**
      Returns the current time step of the simulation

      \return the current time step number
    */
    unsigned int getCurrentStep() const { return m_CurrentStep; };

    /**
      Returns the current time of the simulation

      \return the current time
    */
    openfluid::core::DateTime getCurrentTime() const { return m_CurrentTime; };

    /**
      Returns true if the current step is the first step of the simulation
    */
    bool isFirstStep() const { return m_IsFirstStep; };

    /**
      Returns true if the current step is the last step of the simulation
    */
    bool isLastStep() const { return m_IsLastStep; };


};



} } // namespace openfluid::core


#endif




