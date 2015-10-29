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
  @file ObserverSignatureRegistry.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_OBSERVERSIGNATUREREGISTRY_HPP__
#define __OPENFLUID_MACHINE_OBSERVERSIGNATUREREGISTRY_HPP__

#include <vector>
#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/machine/WareSignatureRegistry.hpp>
#include <openfluid/utils/MeyerSingleton.hpp>


namespace openfluid { namespace machine {


class ObserverSignatureInstance;


class OPENFLUID_API ObserverSignatureRegistry : public WareSignatureRegistry<ObserverSignatureInstance>,
                                                public openfluid::utils::MeyerSingleton<ObserverSignatureRegistry>
{
  friend class openfluid::utils::MeyerSingleton<ObserverSignatureRegistry>;


  private:

    std::vector<ObserverSignatureInstance*> m_AvailableSignatures;

    ObserverSignatureRegistry();

    ~ObserverSignatureRegistry();


  public:

    /**
     * @brief Returns the Signature of the Observer with ObserverID if available
     * @throw openfluid::base::FrameworkException if this Observer plugin is not available
     */
    const openfluid::machine::ObserverSignatureInstance* signature(const openfluid::ware::WareID_t& ID) const;

    /**
     * @brief Updates the list of available signatures, according to Runtime environment paths
     */
    void update();

    const std::vector<openfluid::machine::ObserverSignatureInstance*>& getAvailableSignatures() const;

    void unloadAll();

};

} } // namespaces


#endif /* OBSERVERSIGNATUREREGISTRY_HPP_ */
