/**
  \file ReachSegment.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __REACH_H__
#define __REACH_H__


#include "HydroObject.h"
#include "GroundwaterUnit.h"


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
    hoid_t m_LowReachID;
    ReachSegment *mp_LowReach;
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

    hoid_t getLowReachID() const;

    double getUsrSlope() const;

    double getUsrLength() const;

    double getUsrWidth() const;

    double getUsrHeight() const;

		hoid_t getGUExchangeID() const;

    GroundwaterUnit *getGUExchange() const;

    ReachSegment *getLowReach() const;

    void setUpstreamNode(nodeid_t Node);

    void setDownstreamNode(nodeid_t Node);

    void setLowReachID(hoid_t ID);

    void setUsrSlope(double Slope);

    void setUsrLength(double Length);

    void setUsrWidth(double Width);

    void setUsrHeight(double Height);

		void setGUExchangeID(hoid_t ID);

    void setGUExchange(GroundwaterUnit *GU);

    void setLowReach(ReachSegment *RS);


};





} } // namespace mhydasdk::core



#endif
