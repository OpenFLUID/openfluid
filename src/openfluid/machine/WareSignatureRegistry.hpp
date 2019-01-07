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
  @file WareSignatureRegistry.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_WARESIGNATUREREGISTRY_HPP__
#define __OPENFLUID_MACHINE_WARESIGNATUREREGISTRY_HPP__


#include <openfluid/ware/WareSignature.hpp>


namespace openfluid { namespace machine {


template<class S>
class WareSignatureRegistry
{

  public:

    /**
      Updates the registry from available plugins
    */
    virtual void update() = 0;

    /**
      Unloads all signatures of the registry
    */
    virtual void unloadAll() = 0;

    /**
      Returns the signature of the ware given by its ID
      @param[in] ID The ID of the ware
      @return The signature of the ware, nullptr if not found
    */
    virtual  const S* signature(const openfluid::ware::WareID_t& ID) const = 0;

};


} }  // namespaces


#endif /* __OPENFLUID_MACHINE_WARESIGNATUREREGISTRY_HPP__ */
