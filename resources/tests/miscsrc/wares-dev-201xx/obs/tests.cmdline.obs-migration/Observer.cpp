/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
 
*/

/**
  @file Observer.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#include <openfluid/ware/PluggableObserver.hpp>


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("tests.cmdline.obs-migration")

  DECLARE_NAME("Observer 2.1.x for migration")
  DECLARE_DESCRIPTION("This is an observer 2.1.x \"for migration\"")
  DECLARE_VERSION("2022-09")
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL)

  DECLARE_AUTHOR("John Doe","john.doe@foo.org")
  DECLARE_AUTHOR("Jane Doe","jane.doe@foo.org")
  DECLARE_AUTHOR("Nomen nescio","nn@bar.org")

END_OBSERVER_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class Observer : public openfluid::ware::PluggableObserver
{


  public:

    Observer() : PluggableObserver()
    {

    }


    // =====================================================================
    // =====================================================================


    ~Observer()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {

    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {

    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {

    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {

    }

};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(Observer)


DEFINE_WARE_LINKUID(WARE_LINKUID)

