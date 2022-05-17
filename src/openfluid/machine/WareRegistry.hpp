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
  @file WareRegistry.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_WAREREGISTRY_HPP__
#define __OPENFLUID_MACHINE_WAREREGISTRY_HPP__


#include <openfluid/machine/WareContainer.hpp>


namespace openfluid { namespace machine {


template<class SignatureType>
class WareRegistry
{
  public:

    using WaresByID_t = std::map<openfluid::ware::WareID_t,WareContainer<SignatureType>>;

    using WaresByPath_t = std::map<std::string,WareContainer<SignatureType>>;


  private:

    /**
      Returns an empty built container
      @return The container
    */
    virtual WareContainer<SignatureType> createWareContainer() const = 0;


  protected:

    WaresByID_t m_AvailableWares;

    WaresByPath_t m_ErroredWares;

    WareContainer<SignatureType> m_InvalidWareContainer = 
      WareContainer<SignatureType>(openfluid::ware::WareType::UNDEFINED);

    virtual ~WareRegistry()
    {  }

    bool add(WareContainer<SignatureType> Container)
    {
      if (Container.isValid())
      {
        if (Container.hasSignature())
        {
          const auto ID = Container.signature()->ID;
          m_AvailableWares.emplace(ID,std::move(Container));
        }
        else
        {
          const auto Path = Container.getPath();
          m_ErroredWares.emplace(Path,std::move(Container));
        }
        return true;
      }
      return false;
    }


  public:

    virtual bool addWare(const openfluid::ware::WareID_t& ID) = 0;

    /**
      Clears and updates the registry with discovered plugins
    */
    virtual void discoverWares(const std::string IDPattern) = 0;

    /**
      Clear all wares containers from the registry
    */
    void clearWares()
    {
      m_AvailableWares.clear();
      m_ErroredWares.clear();
    }

    /**
      Returns the container of the ware given by its ID
      @param[in] ID The ID of the ware
      @return The container of the requested ware, an invalid container if not found
    */
    const WareContainer<SignatureType>& wareContainer(const openfluid::ware::WareID_t& ID) const
    {
      auto it = m_AvailableWares.find(ID);

      if (it != m_AvailableWares.end())
      {
        return it->second;
      }

      return m_InvalidWareContainer;
    }

    const WaresByID_t& availableWares() const
    {
      return m_AvailableWares;
    }

    const WaresByPath_t& erroredWares() const
    {
      return m_ErroredWares;
    }

    bool hasAvailableWare(const openfluid::ware::WareID_t& ID) const
    {
      return (m_AvailableWares.find(ID) != m_AvailableWares.end());
    }

    virtual void clear() = 0;

};


} }  // namespaces


#endif /* __OPENFLUID_MACHINE_WAREREGISTRY_HPP__ */
