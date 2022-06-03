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
  @file ObserverRegistry.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_OBSERVERREGISTRY_HPP__
#define __OPENFLUID_MACHINE_OBSERVERREGISTRY_HPP__


#include <vector>
#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/machine/WareRegistry.hpp>
#include <openfluid/ware/ObserverSignature.hpp>
#include <openfluid/utils/SingletonMacros.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API ObserverRegistry : public WareRegistry<openfluid::ware::ObserverSignature>
{

  OPENFLUID_SINGLETON_DEFINITION(ObserverRegistry)


  private:

    ObserverRegistry();

    WareContainer<openfluid::ware::ObserverSignature> createWareContainer() const
    {
      return WareContainer<openfluid::ware::ObserverSignature>(openfluid::ware::WareType::OBSERVER);
    }


  public:

    /**
      Loads an observer ware and adds it in the registry if everything went fine
      @param[in] ID the ID of the ware to load
      @return true if the wares is successfully added
    */
    bool addWare(const openfluid::ware::WareID_t& ID);

    /**
      Automatically discovers observer wares and adds it in the registry, 
      optionally filtered on IDs using a given pattern
      @param[in] IDPattern the pattern to filter the wares on ID. Default is empty (no filtering)
    */
    void discoverWares(const std::string IDPattern = "");

    /**
      Clears the registry
    */
    void clear()
    {
      clearWares();
    }
};


} } // namespaces


#endif /* __OPENFLUID_MACHINE_OBSERVERREGISTRY_HPP__ */
