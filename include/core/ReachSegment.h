/**
  \file ReachSegment.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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

    /**
      Returns the upstream node
     */
	nodeid_t getUpstreamNode() const;

    /**
      Returns the downstream node
     */
	nodeid_t getDownstreamNode() const;

	UnitID getDownstreamReachID() const;

    /**
      Returns the slope of this RS
     */
	double getUsrSlope() const;

    /**
      Returns the length of this RS
     */
	double getUsrLength() const;

    /**
      Returns the width of this RS
     */
	double getUsrWidth() const;

    /**
      Returns the height of this RS
     */
	double getUsrHeight() const;

	UnitID getGUExchangeID() const;

    /**
      Returns the GU exchanging with this RS
     */
	GroundwaterUnit *getGUExchange() const;

    /**
      Returns the downstream RS
     */
	ReachSegment *getDownstreamReach() const;

    /**
      Returns the list of the upstream RSs
     */
	std::list<ReachSegment*>* getUpstreamReaches() { return mp_UpstreamReaches; };

    /**
      Returns the list of the upstream lateral SUs
     */
	std::list<SurfaceUnit*>* getSrcUpstreamSUs() { return mp_SrcUpstreamSUs; };

    /**
      Returns the list of the upstream source SUs
     */
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
