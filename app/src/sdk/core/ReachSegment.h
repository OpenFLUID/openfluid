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


namespace mhydasdk { namespace core {



typedef hoid_t nodeid_t;

/**
  \brief Definition class for reach segments

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class ReachSegment : public HydroObject
{

  private:

    nodeid_t m_UpstreamNode;
    nodeid_t m_DownstreamNode;
    hoid_t m_DownstreamReachID;
    ReachSegment *mp_DownstreamReach;
    std::list<ReachSegment*>* mp_UpstreamReaches;
    std::list<SurfaceUnit*>* mp_UpstreamSUs;    
    
    double m_UsrSlope;
    double m_UsrLength;
    double m_UsrWidth;
    double m_UsrHeight;
		hoid_t m_GUExchangeID;
    GroundwaterUnit *mp_GUExchange;

  public:
	  /**
	    Default Constructor
	  */
	  ReachSegment();

	  /**
	    Constructor
	  */
		ReachSegment(hoid_t ID, int ProcessOrder,
              nodeid_t UpstreamNode, nodeid_t DownstreamNode, hoid_t LowReachID,
              double UsrSlope, double UsrLength, double UsrWidth, double UsrHeight,
              hoid_t GUExchangeID);

	  /**
	    Destructor
	  */
		~ReachSegment();

    nodeid_t getUpstreamNode() const;

    nodeid_t getDownstreamNode() const;

    hoid_t getDownstreamReachID() const;

    double getUsrSlope() const;

    double getUsrLength() const;

    double getUsrWidth() const;

    double getUsrHeight() const;

		hoid_t getGUExchangeID() const;

    GroundwaterUnit *getGUExchange() const;

    ReachSegment *getDownstreamReach() const;
    
    std::list<ReachSegment*>* getUpstreamReaches() { return mp_UpstreamReaches; };
    
    std::list<SurfaceUnit*>* getUpstreamSUs() { return mp_UpstreamSUs; };

    void setUpstreamNode(nodeid_t Node);

    void setDownstreamNode(nodeid_t Node);

    void setDownstreamReachID(hoid_t ID);

    void setUsrSlope(double Slope);

    void setUsrLength(double Length);

    void setUsrWidth(double Width);

    void setUsrHeight(double Height);

		void setGUExchangeID(hoid_t ID);

    void setGUExchange(GroundwaterUnit *GU);

    void setDownstreamReach(ReachSegment *RS);


};





} } // namespace mhydasdk::core



#endif
