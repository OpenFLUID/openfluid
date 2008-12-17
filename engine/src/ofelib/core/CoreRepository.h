/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __COREREPOSITORY_H__
#define __COREREPOSITORY_H__

#include "SpatialRepository.h"



namespace openfluid { namespace core {


/**

*/
class CoreRepository
{
  private:

    SpatialRepository *mp_SpatialData;

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


};




} } // namespace openfluid::core



#endif




