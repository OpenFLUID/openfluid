/**
  \file SpatialRepository.h
  \brief header de la classe de gestion des données spatiales

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __SPATIALREPOSITORY_H__
#define __SPATIALREPOSITORY_H__


#include <vector>
#include <wx/hashmap.h>

#include "SurfaceUnit.h"
#include "ReachSegment.h"
#include "GroundwaterUnit.h"




namespace mhydasdk { namespace core {



WX_DECLARE_HASH_MAP(int, SurfaceUnit*, wxIntegerHash, wxIntegerEqual, SUMap);
WX_DECLARE_HASH_MAP(int, GroundwaterUnit*, wxIntegerHash, wxIntegerEqual, GUMap);
WX_DECLARE_HASH_MAP(int, ReachSegment*, wxIntegerHash, wxIntegerEqual, RSMap);


/**
  Classe de gestion de l'ensemble des données spatiales

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class SpatialRepository
{
	private:
		SUMap m_SUsCollection;
		RSMap m_RSsCollection;
		GUMap m_GUsCollection;

    vector<vector<SurfaceUnit*>*> m_SUsProcessOrders;
    vector<vector<ReachSegment*>*> m_RSsProcessOrders;
    vector<vector<GroundwaterUnit*>*> m_GUsProcessOrders;


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
		  Ajoute une SHU à l'ensemble des SHUs
		  \param SHU l'unité à ajouter
		*/
		bool addSU(SurfaceUnit *SU);

		/**
		  Retourne la SHU correspondant à l'ID passé en paramètre
		  \param ID l'ID recherché
		  \return la SHU recherchée, NULL si non trouvée
		*/
		SurfaceUnit* getSUByID(int ID);

		/**
		  Renvoie la liste des définitions de SHUs sous la forme d'un vecteur de SHUs
		  \return le vecteur de SHUs
		*/
		SUMap getSUsCollection();


		/**
		  Ajoute un bief à l'ensemble des bief constituant le réseau hydro
		  \param ReachUnit le bief à ajouter
		*/
		bool addRS(ReachSegment* RS);


		/**
		  Retourne le bief correspondant à l'ID passé en paramètre
		  \param ID l'ID recherché
		  \return le bief recherché, NULL si non trouvé
		*/
		ReachSegment* getRSByID(int ID);

		/**
		  Renvoie la liste des définitions de biefs sous la forme d'un vecteur de biefs
		  \return le vecteur de biefs
		*/
		RSMap getRSsCollection();


		/**
		  Ajoute une AHU bief à l'ensemble des AHUs
		  \param AHU l' AHU à ajouter
		*/
		bool addGU(GroundwaterUnit* GU);

		/**
		  Retourne l'AHU correspondante à l'ID passé en paramètre
		  \param ID l'ID recherché
		  \return l'AHU recherché, NULL si non trouvé
		*/
		GroundwaterUnit* getGUByID(int ID);

		/**
		  Renvoie la liste des définitions de AHUs sous la forme d'un vecteur de AHUs
		  \return le vecteur de AHUs
		*/
		GUMap getGUsCollection();


    bool buildObjectLinkedTopologyFromIDs();


		/**
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

