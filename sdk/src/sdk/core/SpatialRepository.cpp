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
  mp_SUsCollection = new SUMap();
	mp_RSsCollection = new RSMap();
	mp_GUsCollection = new GUMap();


  mp_SUsProcessOrders = new vector<vector<SurfaceUnit*>*>();
  mp_RSsProcessOrders = new vector<vector<ReachSegment*>*>();
  mp_GUsProcessOrders = new vector<vector<GroundwaterUnit*>*>();

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

  if (mp_SUsCollection->find(SU->getID()) != mp_SUsCollection->end())
  {
     return false;
  }
  else mp_SUsCollection->insert(SUMap::value_type(SU->getID(),SU));

  return true;
}



// =====================================================================
// =====================================================================


SurfaceUnit* SpatialRepository::getSUByID(int ID)
{
  SurfaceUnit* Unit = NULL;

  if (mp_SUsCollection->find(ID) != mp_SUsCollection->end()) Unit = mp_SUsCollection->find(ID)->second;

  return Unit;

}



// =====================================================================
// =====================================================================



SUMap* SpatialRepository::getSUsCollection()
{
  return mp_SUsCollection;
}



bool SpatialRepository::addRS(ReachSegment *RS)
{
  if (mp_RSsCollection->find(RS->getID()) != mp_RSsCollection->end()) return false;
  else mp_RSsCollection->insert(RSMap::value_type(RS->getID(),RS));

  return true;
}


// =====================================================================
// =====================================================================


ReachSegment* SpatialRepository::getRSByID(int ID)
{
  ReachSegment* Reach = NULL;

  if (mp_RSsCollection->find(ID) != mp_RSsCollection->end()) Reach = mp_RSsCollection->find(ID)->second;

  return Reach;

}


// =====================================================================
// =====================================================================


RSMap* SpatialRepository::getRSsCollection()
{
  return mp_RSsCollection;
}



// =====================================================================
// =====================================================================

bool SpatialRepository::addGU(GroundwaterUnit *GU)
{
  if (mp_GUsCollection->find(GU->getID()) != mp_GUsCollection->end()) return false;
  else mp_GUsCollection->insert(GUMap::value_type(GU->getID(),GU));

  return true;

}

// =====================================================================
// =====================================================================

GroundwaterUnit* SpatialRepository::getGUByID(int ID)
{
  GroundwaterUnit* Unit = NULL;

  if (mp_GUsCollection->find(ID) != mp_GUsCollection->end()) Unit = mp_GUsCollection->find(ID)->second;

  return Unit;


}

// =====================================================================
// =====================================================================

GUMap* SpatialRepository::getGUsCollection()
{
  return mp_GUsCollection;
}



// =====================================================================
// =====================================================================


bool SpatialRepository::buildObjectLinkedTopologyFromIDs()
{
  if (mp_SUsCollection->size() != 0 && mp_RSsCollection->size() != 0
      && mp_GUsCollection->size() != 0)
  {

    GroundwaterUnit* LinkedGU;
    ReachSegment* LinkedRS;
    SurfaceUnit* LinkedSU;

    // =========== SHUs ============

    //std::cout << "rebuilding SHUs topology" << std::endl;

    SUMap::iterator SUit;

    for(SUit = mp_SUsCollection->begin(); SUit != mp_SUsCollection->end(); ++SUit )
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

    for(RSit = mp_RSsCollection->begin(); RSit != mp_RSsCollection->end(); ++RSit )
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

    for(GUit = mp_GUsCollection->begin(); GUit != mp_GUsCollection->end(); ++GUit )
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



  if ((mp_SUsCollection->size() != 0) && (mp_RSsCollection->size() !=0)
      && (mp_GUsCollection->size() !=0))
  {

    int MaxOrder, i;

    // =============== SUs =============
    MaxOrder = -1;

    SUMap::iterator SUit;

    // searching for highest process order
    for(SUit = mp_SUsCollection->begin(); SUit != mp_SUsCollection->end(); ++SUit )
    {
      if (SUit->second->getProcessOrder() > MaxOrder)
      MaxOrder = SUit->second->getProcessOrder();
    }

    // creating process order classes
    for (i=0; i<MaxOrder; i++)  mp_SUsProcessOrders->push_back(new vector<SurfaceUnit*>());


    // adding SU in its process order class
    for(SUit = mp_SUsCollection->begin(); SUit != mp_SUsCollection->end(); ++SUit)
    {
      mp_SUsProcessOrders->at(SUit->second->getProcessOrder()-1)->push_back(SUit->second);
    }


    // =============== Reaches =================

    MaxOrder = -1;

    RSMap::iterator RSit;

    // searching for highest process order
    for(RSit = mp_RSsCollection->begin(); RSit != mp_RSsCollection->end(); ++RSit )
    {
      if (RSit->second->getProcessOrder() > MaxOrder)
      MaxOrder = RSit->second->getProcessOrder();
    }

    // creating process order classes
    for (i=0; i<MaxOrder; i++)  mp_RSsProcessOrders->push_back(new vector<ReachSegment*>());


    // adding RS in its process order class
    for(RSit = mp_RSsCollection->begin(); RSit != mp_RSsCollection->end(); ++RSit)
    {
      mp_RSsProcessOrders->at(RSit->second->getProcessOrder()-1)->push_back(RSit->second);
    }



    // =============== GUs =======================

    MaxOrder = -1;

    GUMap::iterator GUit;

    // searching for highest process order
    for(GUit = mp_GUsCollection->begin(); GUit != mp_GUsCollection->end(); ++GUit )
    {
      if (GUit->second->getProcessOrder() > MaxOrder)
      MaxOrder = GUit->second->getProcessOrder();
    }

    // creating process order classes
    for (i=0; i<MaxOrder; i++)  mp_GUsProcessOrders->push_back(new vector<GroundwaterUnit*>());


    // adding GU in its process order class
    for(GUit = mp_GUsCollection->begin(); GUit != mp_GUsCollection->end(); ++GUit)
    {
      mp_GUsProcessOrders->at(GUit->second->getProcessOrder()-1)->push_back(GUit->second);
    }


    return true;

  }
  else return false;

}




} } // namespace mhydasdk::core




