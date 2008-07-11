/**
  \file SpatialRepository.cpp
  \brief implements management class for spatial objects and data

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/





  // TODO attention, methodes buildObjectLinkedTopologyFromIDs(), buildProcessOrders() à peaufiner!



#include "SpatialRepository.h"



// =====================================================================
// =====================================================================


namespace openfluid { namespace core {



struct SortByProcessOrder
{
  bool operator ()(HydroObject* HO1, HydroObject* HO2) const
  {
    return (HO1->getProcessOrder() <= HO2->getProcessOrder());
  }

};

SpatialRepository::SpatialRepository()
{
  mp_SUsCollection = new SUMap();
  mp_RSsCollection = new RSMap();
  mp_GUsCollection = new GUMap();


  mp_SUsProcessOrders = new std::vector<std::vector<SurfaceUnit*>*>();
  mp_RSsProcessOrders = new std::vector<std::vector<ReachSegment*>*>();
  mp_GUsProcessOrders = new std::vector<std::vector<GroundwaterUnit*>*>();

  mp_SUsOrderedList = new std::list<SurfaceUnit*>;
  mp_RSsOrderedList = new std::list<ReachSegment*>;
  mp_GUsOrderedList = new std::list<GroundwaterUnit*>;

}


// =====================================================================
// =====================================================================



SpatialRepository::~SpatialRepository()
{

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

  GroundwaterUnit* LinkedGU;
  ReachSegment* LinkedRS;
  SurfaceUnit* LinkedSU;
  
  SUDownstreamCode SUDownCode;


  // =========== SUs ============

  //std::cout << "rebuilding SHUs topology" << std::endl;

  SUMap::iterator SUit;

  for(SUit = mp_SUsCollection->begin(); SUit != mp_SUsCollection->end(); ++SUit )
  {

    // groundwater link
    if (SUit->second->getGUExchangeID() > 0)
    {
      LinkedGU = getGUByID(SUit->second->getGUExchangeID());
      if (LinkedGU == NULL) return false;      
      else 
      {       
        SUit->second->setGUExchange(LinkedGU);
        LinkedGU->getSUsExchange()->push_back(SUit->second);
      }  
    }

    // downstream object link
    if (SUit->second->getDownstreamID() > 0)
    {

      if (SUit->second->getDownstreamCode() == SUDownstream)
      {
        // downstream SU        
        LinkedSU = getSUByID(SUit->second->getDownstreamID());
        if (LinkedSU == NULL) return false;        
        else
        {
          // set downstream SU link
          SUit->second->setDownstreamObject(LinkedSU);
        
          // adds current SU to donwstream SU's upstream SUs list
          LinkedSU->getUpstreamSUs()->push_back(SUit->second);
//          std::cerr << "added upstream SU " << SUit->second->getID() << " to SU " << LinkedSU->getID() << std::endl;
        }  
        
      }

      SUDownCode = SUit->second->getDownstreamCode();
      if ( SUDownCode == RLatDownstream || SUDownCode == RSrcDownstream)
      {
        LinkedRS = getRSByID(SUit->second->getDownstreamID());
        if (LinkedRS == NULL) return false;
        else
        {
          // set downstream reach link
          SUit->second->setDownstreamObject(LinkedRS);
          
          // adds current SU to donwstream SU's upstream SUs list
          if (SUDownCode == RSrcDownstream)  LinkedRS->getSrcUpstreamSUs()->push_back(SUit->second);
          if (SUDownCode == RLatDownstream)  LinkedRS->getLatUpstreamSUs()->push_back(SUit->second);          
//          std::cerr << "added upstream SU " << SUit->second->getID() << " to RS " << LinkedRS->getID() << std::endl;
        }  
      }

      if (SUit->second->getDownstreamCode() == UnknownDownstreamCode) return false;
    }

  }


  // =========== hydro network ============

  RSMap::iterator RSit;

  for(RSit = mp_RSsCollection->begin(); RSit != mp_RSsCollection->end(); ++RSit )
  {

    // groundwater link
    if (RSit->second->getGUExchangeID() > 0)
    {
      LinkedGU = getGUByID(RSit->second->getGUExchangeID());
      if (LinkedGU == NULL) return false;
      else 
      {
        RSit->second->setGUExchange(LinkedGU);
        LinkedGU->getRSsExchange()->push_back(RSit->second);
      }  
    }

    // downstream reach link
    if (RSit->second->getDownstreamReachID() > 0)
    {
      LinkedRS = getRSByID(RSit->second->getDownstreamReachID());
      if (LinkedRS == NULL) return false;
      else
      {
        // set downstream reach link
        RSit->second->setDownstreamReach(LinkedRS);
        
        // adds current reach to donwstream reach's upstream reaches list
        LinkedRS->getUpstreamReaches()->push_back(RSit->second);        
  //      std::cerr << "added upstream RS " << RSit->second->getID() << " to RS " << LinkedRS->getID() << std::endl;        
      }  
    }
  }



  // =========== GUs ============

  GUMap::iterator GUit;
  SurfaceUnit* SU;
  std::list<SurfaceUnit*>::iterator SUiter;
  float UsrAreaSum;
  
  for(GUit = mp_GUsCollection->begin(); GUit != mp_GUsCollection->end(); ++GUit )
  {

    // groundwater link only if exchange
    if (GUit->second->getGUExchangeID() > 0)
    {
      LinkedGU = getGUByID(GUit->second->getGUExchangeID());
      if (LinkedGU == NULL) return false;
      else 
      {
        GUit->second->setGUExchange(LinkedGU);
        LinkedGU->getGUsExchange()->push_back(GUit->second);
      }  
    }
    
    // compute UsrArea

    std::list<SurfaceUnit*>* SUsList = GUit->second->getSUsExchange();
    UsrAreaSum = 0;

   
    for(SUiter=SUsList->begin(); SUiter != SUsList->end(); SUiter++)
    { 
      SU = *SUiter;
      UsrAreaSum = UsrAreaSum + SU->getUsrArea();
    }    
    GUit->second->setUsrArea(UsrAreaSum);
    
  }


  return true;

}

// =====================================================================
// =====================================================================


bool SpatialRepository::buildProcessOrders()
{

  // à revoir en utilisant le polymorphisme pour traiter ceci au niveau HydroObject
  // à revoir pour ne garder que les listes



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
  for (i=0; i<MaxOrder; i++)  mp_SUsProcessOrders->push_back(new std::vector<SurfaceUnit*>());


  // adding SU in its process order class
  for(SUit = mp_SUsCollection->begin(); SUit != mp_SUsCollection->end(); ++SUit)
  {
    mp_SUsProcessOrders->at(SUit->second->getProcessOrder()-1)->push_back(SUit->second);
  }

  // creating ordered list
  for(SUit = mp_SUsCollection->begin(); SUit != mp_SUsCollection->end(); ++SUit)
  {
    mp_SUsOrderedList->push_back(SUit->second);
  }
  mp_SUsOrderedList->sort(SortByProcessOrder());


  /*list<SurfaceUnit*>::iterator iter;
  for(iter=mp_SUsOrderedList->begin(); iter != mp_SUsOrderedList->end(); iter++)
  {
    SurfaceUnit* SU = *iter;
    std::cout << SU->getProcessOrder() << " " << SU->getID() << std::endl;
  }*/


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
  for (i=0; i<MaxOrder; i++)  mp_RSsProcessOrders->push_back(new std::vector<ReachSegment*>());


  // adding RS in its process order class
  for(RSit = mp_RSsCollection->begin(); RSit != mp_RSsCollection->end(); ++RSit)
  {
    mp_RSsProcessOrders->at(RSit->second->getProcessOrder()-1)->push_back(RSit->second);
  }

  // creating ordered list
  for(RSit = mp_RSsCollection->begin(); RSit != mp_RSsCollection->end(); ++RSit)
  {
    mp_RSsOrderedList->push_back(RSit->second);
  }
  mp_RSsOrderedList->sort(SortByProcessOrder());



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
  for (i=0; i<MaxOrder; i++)  mp_GUsProcessOrders->push_back(new std::vector<GroundwaterUnit*>());


  // adding GU in its process order class
  for(GUit = mp_GUsCollection->begin(); GUit != mp_GUsCollection->end(); ++GUit)
  {
    mp_GUsProcessOrders->at(GUit->second->getProcessOrder()-1)->push_back(GUit->second);
  }

  // creating ordered list
  for(GUit = mp_GUsCollection->begin(); GUit != mp_GUsCollection->end(); ++GUit)
  {
    mp_GUsOrderedList->push_back(GUit->second);
  }
  mp_GUsOrderedList->sort(SortByProcessOrder());



  return true;

}


// =====================================================================
// =====================================================================

bool SpatialRepository::checkHydroNetworkStructure()
{

    
}

// =====================================================================
// =====================================================================


bool SpatialRepository::reserveSimulationVars(int StepsNbr)
{
  
  SUMap::iterator SUit;
  RSMap::iterator RSit;
  GUMap::iterator GUit;
  
  openfluid::core::SimulatedVarsMap *VarsMap;
  SimulatedVarsMap::iterator Vit;

  openfluid::core::SimulatedVectorVarsMap *VectVarsMap;
  SimulatedVectorVarsMap::iterator VVit;  
  

  // pour les SU
  for(SUit = mp_SUsCollection->begin(); SUit != mp_SUsCollection->end(); ++SUit)
  {
    VarsMap = SUit->second->getSimulatedVars();    
    for (Vit = VarsMap->begin();Vit != VarsMap->end();++Vit)
    {
      Vit->second->reserve(StepsNbr);
    }

    VectVarsMap = SUit->second->getSimulatedVectorVars();    
    for (VVit = VectVarsMap->begin();VVit != VectVarsMap->end();++VVit)
    {
      VVit->second->reserve(StepsNbr);
    }
    
    
  }
  
  // pour les RS
  for(RSit = mp_RSsCollection->begin(); RSit != mp_RSsCollection->end(); ++RSit)
  {
    VarsMap = RSit->second->getSimulatedVars();    
    for (Vit = VarsMap->begin();Vit != VarsMap->end();++Vit)
    {
      Vit->second->reserve(StepsNbr);
    }
    
    VectVarsMap = RSit->second->getSimulatedVectorVars();    
    for (VVit = VectVarsMap->begin();VVit != VectVarsMap->end();++VVit)
    {
      VVit->second->reserve(StepsNbr);
    }
    
    
  }

  // pour les GU
  for(GUit = mp_GUsCollection->begin(); GUit != mp_GUsCollection->end(); ++GUit)
  {
    VarsMap = GUit->second->getSimulatedVars();    
    for (Vit = VarsMap->begin();Vit != VarsMap->end();++Vit)
    {
      Vit->second->reserve(StepsNbr);
    }
    
    VectVarsMap = GUit->second->getSimulatedVectorVars();    
    for (VVit = VectVarsMap->begin();VVit != VectVarsMap->end();++VVit)
    {
      VVit->second->reserve(StepsNbr);
    }
    
    
  }


  
    
}


} } // namespace openfluid::core




