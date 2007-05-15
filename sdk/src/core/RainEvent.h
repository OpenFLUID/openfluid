/**
  \file RainRepository.h
  \brief header de ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#ifndef __RAINREPOSITORY_H__
#define __RAINREPOSITORY_H__

#include <wx/hashmap.h>
#include "ChronDataSource.h"


namespace mhydasdk { namespace core {

WX_DECLARE_HASH_MAP(int, ChronDataSource*, wxIntegerHash, wxIntegerEqual, RainSourceMap);

/**
  Management class for rain event

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class RainEvent
{
  private:
    RainSourceMap m_RainSrcCollection;

    rawtime_t m_FirstTime;
    rawtime_t m_LastTime;
    rawtime_t m_FirstCommonTime;
    rawtime_t m_LastCommonTime;

    bool m_ComputedTime;

    bool m_FirstSerieConstraint;
    bool m_TimeStepConstraint;
    int m_ConstrainedTimeStep;

  public:

    /**
      Constructor
    */
    RainEvent();

    /**
      Destructor
    */
    ~RainEvent();

    /**
      Adds a rain source to the repository
      \param[in] Source the rain source to add
      \return true if added (satisfying the constraints), false otherwise
    */
    bool addRainSource(ChronDataSource *Source);

    /**
      Returns the rain sources composing the rain event
      \return a rain source map
    */
    RainSourceMap getRainSourceCollection();

    /**
      Returns the rain source corresponding to the searchedID
      \param[in] Searched rain source ID
      \return the rain source if exists, NULL otherwise
    */
    ChronDataSource *getRainSourceByID(cdsid_t ID);


    bool computeTimeRange(rawtime_t &First, rawtime_t &Last, rawtime_t &FirstCommon, rawtime_t &LastCommon);

    /**
      Enable or Disable the constraint on time step for series to add
      \param[in] Enabled
      \param[in] TimeStep
    */
    void enableTimeStepConstraint(bool Enabled, int TimeStep = -1);

    /**

    */
    bool isTimeStepConstraintEnabled() const { return  m_TimeStepConstraint; };

    /**

    */
    int getTimeStepConstraintValue() const { return m_ConstrainedTimeStep; };


    /**
      Enable or Disable the constraint that every added serie must
      match the first serie time range and time step
      \param[in] Enabled
    */
    void enableFirstSerieConstraint(bool Enabled);

    /**

    */
    bool isFirstSerieConstraintEnabled() const { return  m_FirstSerieConstraint; };

    /**
      Returns the starting date of the event
      \return a datetime
    */
    DateTime getEventStartingTime();

    /**
      Returns the ending date of the event
      \return a datetime
    */
    DateTime getEventEndingTime();


    bool ProcessRainSources(int TimeStep);

};


} } // namespace mhydasdk::core



#endif


