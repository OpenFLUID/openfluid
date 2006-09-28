/**
  \file SpatialRepository.h
  \brief header of management class for spatial objects and data

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __SPATIALREPOSITORY_H__
#define __SPATIALREPOSITORY_H__


#include <vector>
#include <list>
#include <wx/hashmap.h>

#include "SurfaceUnit.h"
#include "ReachSegment.h"
#include "GroundwaterUnit.h"




namespace mhydasdk { namespace core {



WX_DECLARE_HASH_MAP(int, SurfaceUnit*, wxIntegerHash, wxIntegerEqual, SUMap);
WX_DECLARE_HASH_MAP(int, GroundwaterUnit*, wxIntegerHash, wxIntegerEqual, GUMap);
WX_DECLARE_HASH_MAP(int, ReachSegment*, wxIntegerHash, wxIntegerEqual, RSMap);


/**
  Management class for spatial objects and data

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class SpatialRepository
{
	private:
		SUMap* mp_SUsCollection;
		RSMap* mp_RSsCollection;
		GUMap* mp_GUsCollection;

    vector<vector<SurfaceUnit*>*>* mp_SUsProcessOrders;
    vector<vector<ReachSegment*>*>* mp_RSsProcessOrders;
    vector<vector<GroundwaterUnit*>*>* mp_GUsProcessOrders;

    list<SurfaceUnit*>* mp_SUsOrderedList;
    list<ReachSegment*>* mp_RSsOrderedList;
    list<GroundwaterUnit*>* mp_GUsOrderedList;

	public:

		/**
		  Constructeur
		*/
		SpatialRepository();

		/**
		  Destructeur
		*/
		~SpatialRepository();

		/**
		  Adds a Surface Unit to the SUs collection
		  \param SU the surface unit to add
		*/
		bool addSU(SurfaceUnit *SU);

		/**
		  Returns SU object corresponding to the ID
		  \param ID searched ID
		  \return found SU, NULL otherwise
		*/
		SurfaceUnit* getSUByID(int ID);

		/**

		*/
		SUMap* getSUsCollection();

    /**

    */
    list<SurfaceUnit*>* getSUsOrderedList() { return mp_SUsOrderedList; };

		/**
		  Adds a Reach Segment to the RSs collection
		  \param RS the reach segment to add
		*/
		bool addRS(ReachSegment* RS);


		/**
		  Returns RS object corresponding to the ID
		  \param ID searched ID
		  \return found RU, NULL otherwise
		*/
		ReachSegment* getRSByID(int ID);

		/**

		*/
		RSMap* getRSsCollection();

    /**

    */
    list<ReachSegment*>* getRSsOrderedList() { return mp_RSsOrderedList; };

		/**
		  Adds a groundwater Unit to the GUs collection
		  \param GU the groundwater unit to add
		*/
		bool addGU(GroundwaterUnit* GU);

		/**
		  Returns GU object corresponding to the ID
		  \param ID searched ID
		  \return found GU, NULL otherwise
		*/
		GroundwaterUnit* getGUByID(int ID);

		/**

		*/
		GUMap* getGUsCollection();

    /**

    */
    list<GroundwaterUnit*>* getGUsOrderedList() { return mp_GUsOrderedList; };


    bool buildObjectLinkedTopologyFromIDs();


		/**

      Builds ordered lists for each kind of objects (SU,RS,GU)

      !! commentaire à revoir !!

      Regroupe les objets hydrologiques définissant l'espace étudié par classe d'ordre de traitement.
      Cette méthode crée 3 vecteurs (un pour chaque type d'objet).
      Chacun de ces vecteurs contient un ensemble de vecteurs indexés par le numéro d'ordre de traitement [0,ordre-1].
      Ainsi l'ensemble des objets hydrologiques indexés [2] ont un ordre de traitement de 3.
		  \return true si la constitution par classe a bien fonctionné
		*/
    bool buildProcessOrders();


};


} } // namespace mhydasdk::core



#endif

