/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __COREREPOSITORY_H__
#define __COREREPOSITORY_H__

#include "SpatialRepository.h"
#include "RainSources.h"



namespace mhydasdk { namespace core {


/**

*/
class CoreRepository
{
  private:

    SpatialRepository *mp_SpatialData;
    RainSources *mp_RainSources;
//    ProcessedRainEvent *mp_PcssRainEvent;

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

    void setRainEvent(RainSources* Sources) { mp_RainSources = Sources; };

    RainSources* getRainSources() const { return mp_RainSources; };
    
    /*
    bool ProcessRainEvent(int TimeStep);
    
    ProcessedRainEvent* getProcessedRainEvent() const { return mp_PcssRainEvent; };
*/
   

};




} } // namespace mhydasdk::core



#endif




