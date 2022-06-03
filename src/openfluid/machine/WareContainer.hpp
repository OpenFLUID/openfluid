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


/**
  @tparam SignatureType the type of the signature to hold in the container
*/
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

    /**
      Returns the ware type
      @return The ware type
    */
    openfluid::ware::WareType getWareType() const
    {
      return m_WareType;
    }

    /**
      Returns the path of the ware
      @return The ware path
    */
    std::string getPath() const
    {
      return m_Path;
    }

    /**
      Sets the path of the ware on disk
      @param[in] Path the path
      @throw openfluid::base::FrameworkException if path is already set
    */
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

    /**
      Returns the link UID for parameterization UI
      @return the link UID
    */
    UUID_t getLinkUID() const
    {
      return m_LinkUID;
    }

    /**
      Sets the link UID for parameterization UI
      @param[in] UID the UID
      @throw openfluid::base::FrameworkException if UID is already set
    */
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

    /**
      Gives the validation status
      @return true if the conrtainer is valid
    */
    bool isValid() const
    {
      return m_Valid;
    }

    /**
      Sets the container as valid
      @note a container cannot be invalidated
    */
    void validate()
    {
      m_Valid = true;
    }

    /**
      Returns the optional message aosciated to the container
      @return the message
    */
    std::string getMessage() const
    {
      return m_Message;
    }

    /**
      Sets the optional message aosciated to the container
      @param[in] Message the message
    */
    void setMessage(const std::string& Message)
    {
      m_Message = Message;
    }

    /**
      Returns a const reference to the signature as a unique pointer
      @return the signature held in the container
    */
    const std::unique_ptr<SignatureType>& signature() const
    {
      return m_Signature;
    }

    /**
      Indicates if a signature is held by the container
      @return true if a signature is held
    */
    bool hasSignature() const
    {
      return (m_Signature.get() != nullptr);
    }

    /**
      Sets the signature
      @throw openfluid::base::FrameworkException if signature is already set
      @note after using this method, the signature is owned by the container
    */
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

    /**
      Returns true if the ware is a ghost
      @return true if the ware is a ghost
    */
    bool isGhost() const
    {
      return m_Ghost;
    }

    /**
      Marks the ware as ghost
      @note a container marked as ghost cannot be "unghosted"
    */
    void setGhost()
    {
      m_Ghost = true;
    }

};


} }  // namespaces


#endif /* __OPENFLUID_MACHINE_WARECONTAINER_HPP__ */
