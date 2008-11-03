/**
  \file ReachSegment.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __REACH_H__
#define __REACH_H__

#include <list>

#include "HydroObject.h"
#include "GroundwaterUnit.h"
#include "SurfaceUnit.h"


namespace openfluid { namespace core {


typedef UnitID nodeid_t;

/**
  \brief Definition class for reach segments

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class ReachSegment : public HydroObject
{

  private:

    nodeid_t m_UpstreamNode;
    nodeid_t m_DownstreamNode;
    UnitID m_DownstreamReachID;
    ReachSegment *mp_DownstreamReach;
    std::list<ReachSegment*>* mp_UpstreamReaches;
    std::list<SurfaceUnit*>* mp_SrcUpstreamSUs;    
    std::list<SurfaceUnit*>* mp_LatUpstreamSUs;    
    
    double m_UsrSlope;
    double m_UsrLength;
    double m_UsrWidth;
    double m_UsrHeight;
    UnitID m_GUExchangeID;   
    GroundwaterUnit *mp_GUExchange;

  public:
	  /**
	    Default Constructor
	  */
	  ReachSegment();

	  /**
	    Constructor
	  */
		ReachSegment(UnitID ID, int ProcessOrder,
              nodeid_t UpstreamNode, nodeid_t DownstreamNode, UnitID LowReachID,
              double UsrSlope, double UsrLength, double UsrWidth, double UsrHeight,
              UnitID GUExchangeID);

	  /**
	    Destructor
	  */
		~ReachSegment();

    nodeid_t getUpstreamNode() const;

    nodeid_t getDownstreamNode() const;

    UnitID getDownstreamReachID() const;

    double getUsrSlope() const;

    double getUsrLength() const;

    double getUsrWidth() const;

    double getUsrHeight() const;

		UnitID getGUExchangeID() const;

    GroundwaterUnit *getGUExchange() const;

    ReachSegment *getDownstreamReach() const;
    
    std::list<ReachSegment*>* getUpstreamReaches() { return mp_UpstreamReaches; };
    
    std::list<SurfaceUnit*>* getSrcUpstreamSUs() { return mp_SrcUpstreamSUs; };
    
    std::list<SurfaceUnit*>* getLatUpstreamSUs() { return mp_LatUpstreamSUs; };  

    void setUpstreamNode(nodeid_t Node);

    void setDownstreamNode(nodeid_t Node);

    void setDownstreamReachID(UnitID ID);

    void setUsrSlope(double Slope);

    void setUsrLength(double Length);

    void setUsrWidth(double Width);

    void setUsrHeight(double Height);

		void setGUExchangeID(UnitID ID);

    void setGUExchange(GroundwaterUnit *GU);

    void setDownstreamReach(ReachSegment *RS);
    
};





} } // namespace openfluid::core



#endif
