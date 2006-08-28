/**
  \file SpatialRepository.cpp
  \brief implémentation de la classe de gestion des données spatiales

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



/*

  attention, méthodes:
  - buildObjectLinkedTopologyFromIDs();
  - buildProcessOrders()
  à peaufiner!

*/


#include "SpatialRepository.h"

#include <iostream>

// =====================================================================
// =====================================================================


namespace mhydasdk { namespace core {



SpatialRepository::SpatialRepository()
{
  m_SUsCollection.clear();
	m_RSsCollection.clear();
	m_GUsCollection.clear();


  m_SUsProcessOrders.clear();
  m_RSsProcessOrders.clear();
  m_GUsProcessOrders.clear();

}


// =====================================================================
// =====================================================================



SpatialRepository::~SpatialRepository()
{
//  std::cout << "~SpatialRepository() in" << std::endl;


//  std::cout << "~SpatialRepository() out" << std::endl;
}



// =====================================================================
// =====================================================================


bool SpatialRepository::addSU(SurfaceUnit *SU)
{

  if (m_SUsCollection.find(SU->getID()) != m_SUsCollection.end())
  {
     std::cerr << "found!" << std::endl;
     return false;
  }
  else m_SUsCollection[SU->getID()] = SU;

  return true;
}



// =====================================================================
// =====================================================================


SurfaceUnit* SpatialRepository::getSUByID(int ID)
{
  SurfaceUnit* Unit = NULL;

  if (m_SUsCollection.find(ID) != m_SUsCollection.end()) Unit = m_SUsCollection[ID];

  return Unit;

}



// =====================================================================
// =====================================================================



SUMap SpatialRepository::getSUsCollection()
{
  return m_SUsCollection;
}



bool SpatialRepository::addRS(ReachSegment *RS)
{
  if (m_RSsCollection.find(RS->getID()) != m_RSsCollection.end()) return false;
  else m_RSsCollection[RS->getID()] = RS;

  return true;
}


// =====================================================================
// =====================================================================


ReachSegment* SpatialRepository::getRSByID(int ID)
{
  ReachSegment* Reach = NULL;

  if (m_RSsCollection.find(ID) != m_RSsCollection.end()) Reach = m_RSsCollection[ID];

  return Reach;

}


// =====================================================================
// =====================================================================


RSMap SpatialRepository::getRSsCollection()
{
  return m_RSsCollection;
}



// =====================================================================
// =====================================================================

bool SpatialRepository::addGU(GroundwaterUnit *GU)
{
  if (m_GUsCollection.find(GU->getID()) != m_GUsCollection.end()) return false;
  else m_GUsCollection[GU->getID()] = GU;

  return true;

}

// =====================================================================
// =====================================================================

GroundwaterUnit* SpatialRepository::getGUByID(int ID)
{
  GroundwaterUnit* Unit = NULL;

  if (m_GUsCollection.find(ID) != m_GUsCollection.end()) Unit = m_GUsCollection[ID];

  return Unit;


}

// =====================================================================
// =====================================================================

GUMap SpatialRepository::getGUsCollection()
{
  return m_GUsCollection;
}



// =====================================================================
// =====================================================================


bool SpatialRepository::buildObjectLinkedTopologyFromIDs()
{
  if (m_SUsCollection.size() != 0 && m_RSsCollection.size() != 0
      && m_GUsCollection.size() != 0)
  {

    GroundwaterUnit* LinkedGU;
    ReachSegment* LinkedRS;
    SurfaceUnit* LinkedSU;

    // =========== SHUs ============

    //std::cout << "rebuilding SHUs topology" << std::endl;

    SUMap::iterator SUit;

    for(SUit = m_SUsCollection.begin(); SUit != m_SUsCollection.end(); ++SUit )
    {

      //std::cerr << SUit->second->getID() << std::endl;
        //wxString key = it->first, value = it->second;
        // do something useful with key and value

      // groundwater link
      LinkedGU = getGUByID(SUit->second->getGUExchangeID());
      if (LinkedGU == NULL) return false;
      else SUit->second->setGUExchange(LinkedGU);

      // flow object link
      if (SUit->second->getFlowCode() == SU)
      {
        LinkedSU = getSUByID(SUit->second->getFlowID());
        if (LinkedSU == NULL) return false;
        else SUit->second->setFlowObject(LinkedSU);
      }

      if (SUit->second->getFlowCode() == RLat || SUit->second->getFlowCode() == RSrc)
      {
        LinkedRS = getRSByID(SUit->second->getFlowID());
        if (LinkedRS == NULL) return false;
        else SUit->second->setFlowObject(LinkedRS);
      }

      if (SUit->second->getFlowCode() == UnknownFlowCode) return false;

    }

    // =========== rézo hydro ============

    RSMap::iterator RSit;

    for(RSit = m_RSsCollection.begin(); RSit != m_RSsCollection.end(); ++RSit )
    {

       // groundwater link
      LinkedGU = getGUByID(RSit->second->getGUExchangeID());
      if (LinkedGU == NULL) return false;
      else RSit->second->setGUExchange(LinkedGU);

      // low reach link
      if (RSit->second->getLowReachID() > 0)
      {
        LinkedRS = getRSByID(RSit->second->getLowReachID());
        if (LinkedRS == NULL) return false;
        else RSit->second->setLowReach(LinkedRS);
      }


    }

    // =========== AHUs ============

    GUMap::iterator GUit;

    for(GUit = m_GUsCollection.begin(); GUit != m_GUsCollection.end(); ++GUit )
    {

      // groundwater link only if exchange
      if (GUit->second->getGUExchangeID() > 0)
      {
        LinkedGU = getGUByID(GUit->second->getGUExchangeID());
        if (LinkedGU == NULL) return false;
        else GUit->second->setGUExchange(LinkedGU);
      }

    }

    return true;
  }
  else return false;
}

// =====================================================================
// =====================================================================


bool SpatialRepository::buildProcessOrders()
{

  // à revoir en utilisant le polymorphisme pour traiter ceci au niveau HydroObject



  if ((m_SUsCollection.size() != 0) && (m_RSsCollection.size() !=0)
      && (m_GUsCollection.size() !=0))
  {

    int MaxOrder, i;

    // =============== SUs =============
    MaxOrder = -1;

    SUMap::iterator SUit;

    // searching for highest process order
    for(SUit = m_SUsCollection.begin(); SUit != m_SUsCollection.end(); ++SUit )
    {
      if (SUit->second->getProcessOrder() > MaxOrder)
      MaxOrder = SUit->second->getProcessOrder();
    }

    // creating process order classes
    for (i=0; i<MaxOrder; i++)  m_SUsProcessOrders.push_back(new vector<SurfaceUnit*>());


    // adding SU in its process order class
    for(SUit = m_SUsCollection.begin(); SUit != m_SUsCollection.end(); ++SUit)
    {
      m_SUsProcessOrders.at(SUit->second->getProcessOrder()-1)->push_back(SUit->second);
    }


    // =============== Reaches =================

    MaxOrder = -1;

    RSMap::iterator RSit;

    // searching for highest process order
    for(RSit = m_RSsCollection.begin(); RSit != m_RSsCollection.end(); ++RSit )
    {
      if (RSit->second->getProcessOrder() > MaxOrder)
      MaxOrder = RSit->second->getProcessOrder();
    }

    // creating process order classes
    for (i=0; i<MaxOrder; i++)  m_RSsProcessOrders.push_back(new vector<ReachSegment*>());


    // adding RS in its process order class
    for(RSit = m_RSsCollection.begin(); RSit != m_RSsCollection.end(); ++RSit)
    {
      m_RSsProcessOrders.at(RSit->second->getProcessOrder()-1)->push_back(RSit->second);
    }



    // =============== GUs =======================

    MaxOrder = -1;

    GUMap::iterator GUit;

    // searching for highest process order
    for(GUit = m_GUsCollection.begin(); GUit != m_GUsCollection.end(); ++GUit )
    {
      if (GUit->second->getProcessOrder() > MaxOrder)
      MaxOrder = GUit->second->getProcessOrder();
    }

    // creating process order classes
    for (i=0; i<MaxOrder; i++)  m_GUsProcessOrders.push_back(new vector<GroundwaterUnit*>());


    // adding GU in its process order class
    for(GUit = m_GUsCollection.begin(); GUit != m_GUsCollection.end(); ++GUit)
    {
      m_GUsProcessOrders.at(GUit->second->getProcessOrder()-1)->push_back(GUit->second);
    }


    return true;

  }
  else return false;

}




} } // namespace mhydasdk::core




