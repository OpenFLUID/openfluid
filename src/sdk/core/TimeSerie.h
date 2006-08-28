/**
  \file TimeSerie.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __TIMESERIE_H__
#define __TIMESERIE_H__


#include "TimeSerieItem.h"

#include <vector>
#include <string>


using namespace std;

namespace mhydasdk { namespace core {


/**
  class for temporal serie management

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class TimeSerie
{
  private:
    vector<TimeSerieItem*> *m_Serie;
    std::string m_Unit;


  public:
    /**
      Constructor
    */
    TimeSerie(std::string Unit);

    /**
      Destructor
    */
    ~TimeSerie();

    /**
      Sorts the serie
    */
    void sort();

    /**
      Adds a value to the serie
      \param[in] Item the item to add
    */
    void addValue(TimeSerieItem *Item);

    /**
      Returns the unit of the serie
    */
    std::string getUnit() const;

    /**
      Returns the full items collection
    */
    vector<TimeSerieItem*> *getItemsCollection() const;

    /**
      Returns the first item
    */
    TimeSerieItem* getFirstItem() const;

    /**
      Returns the last item
    */
    TimeSerieItem* getLastItem() const;

    /**
      Tries to determine the serie time step, or -1 if there is no time step
      \return the time step, or -1 if no time step
    */
    int computeTimeStep();

    /**
      Determines if the serie is sorted
      \return true if sorted
    */
    bool isSorted();

    /**
      Determines if the serie begins at the specified RawBeginTime,
      ends at the specified RawEndTime and has the specified TimeStep.
      \param[in] RawBeginTime the begin time in raw format
      \param[in] TimeStep the time step
    */
    bool isMatching(rawtime_t RawBeginTime, rawtime_t RawEndTime, int TimeStep);

};


} } // namespace mhydasdk::core

#endif
