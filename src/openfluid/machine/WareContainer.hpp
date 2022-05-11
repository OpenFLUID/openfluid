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
  @file WareContainer.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_WARECONTAINER_HPP__
#define __OPENFLUID_MACHINE_WARECONTAINER_HPP__


#include <string>
#include <memory>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>


namespace openfluid { namespace machine {


typedef std::string UUID_t;


template<class SignatureType> 
class OPENFLUID_API WareContainer
{  
  private:

    const openfluid::ware::WareType m_WareType = openfluid::ware::WareType::UNDEFINED;

    bool m_Valid = false;

    std::string m_Path;

    std::unique_ptr<SignatureType> m_Signature;

    bool m_Ghost = false;

    std::string m_Message;

    UUID_t m_LinkUID;


  public:

    WareContainer() = delete;

    WareContainer(const WareContainer&) = delete;
    
    WareContainer(WareContainer&&) = default;

    WareContainer& operator=(const WareContainer&) = delete;

    WareContainer& operator=(WareContainer&&) = default;

    virtual ~WareContainer() = default;

    WareContainer(openfluid::ware::WareType WType) : m_WareType(WType)
    {  }

    openfluid::ware::WareType getWareType() const
    {
      return m_WareType;
    }

    UUID_t getPath() const
    {
      return m_Path;
    }

    void setPath(const std::string& Path)
    {
      if (m_Path.empty())
      {
        m_Path = Path;
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Path already set");
      }
    }

    UUID_t getLinkUID() const
    {
      return m_LinkUID;
    }

    void setLinkUID(const UUID_t& UID)
    {
      if (m_LinkUID.empty())
      {
        m_LinkUID = UID;
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Link UID already set");
      }
    }

    bool isValid() const
    {
      return m_Valid;
    }

    void validate()
    {
      m_Valid = true;
    }

    std::string getMessage() const
    {
      return m_Message;
    }

    void setMessage(const std::string& Message)
    {
      m_Message = Message;
    }

    const std::unique_ptr<SignatureType>& signature() const
    {
      return m_Signature;
    }

    bool hasSignature() const
    {
      return (m_Signature.get() != nullptr);
    }

    void setSignature(SignatureType* Signature)
    {
      if (m_Signature)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Signature is already set");
      }
      else
      {
        m_Signature.reset(Signature);
      }
    }

    bool isGhost() const
    {
      return m_Ghost;
    }

    void setGhost()
    {
      m_Ghost = true;
    }

};


} }  // namespaces


#endif /* __OPENFLUID_MACHINE_WARECONTAINER_HPP__ */
