/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __COREREPOSITORY_H__
#define __COREREPOSITORY_H__

#include "SpatialRepository.h"
#include "RainEvent.h"


namespace mhydasdk { namespace core {


/**

*/
class CoreRepository
{
  private:

    SpatialRepository *mp_SpatialData;
    RainEvent *mp_RainEvent;

  public:
    /**
      Constructor
    */
    CoreRepository();

    /**
      Destructor
    */
    ~CoreRepository();


    /**
      Integrates the spatial data structure
      \param[in] SpatialData The structure to integrate
    */
    void setSpatialData(SpatialRepository *SpatialData) { mp_SpatialData = SpatialData; };

    /**
      Returns the spatial data structure
      \return The spatial data structure
    */
    SpatialRepository* getSpatialData() const { return mp_SpatialData; };

    void setRainEvent(RainEvent* Event) { mp_RainEvent = Event; };

    RainEvent* getRainEvent() const { return mp_RainEvent; };



};




} } // namespace mhydasdk::core



#endif




