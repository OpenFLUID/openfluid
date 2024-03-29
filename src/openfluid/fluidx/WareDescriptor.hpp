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
  @file WareDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_FLUIDX_WAREDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_WAREDESCRIPTOR_HPP__


#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace fluidx {


class OPENFLUID_API WareDescriptor
{
  protected:

    openfluid::ware::WareType m_WareType;

    const openfluid::ware::WareID_t m_ID;

    openfluid::ware::WareParams_t m_Params;  // Simulator parameters set

    bool m_Enabled;


  public:

    WareDescriptor(const openfluid::ware::WareID_t& ID);

    virtual ~WareDescriptor()
    { }

    const openfluid::ware::WareID_t getID() const
    {
      return m_ID;
    }

    void setParameter(const openfluid::ware::WareParamKey_t& Key,
                      const openfluid::ware::WareParamValue_t& Value);

    void setParameters(const openfluid::ware::WareParams_t& Params);

    openfluid::ware::WareParams_t getParameters() const;

    openfluid::ware::WareParams_t& parameters();

    bool isType(openfluid::ware::WareType MIType) const;

    openfluid::ware::WareType getType() const;

    void eraseParameter(const openfluid::ware::WareParamKey_t& Key);

    void clearParameters();

    bool hasParameter(const std::string& ParameterKey) const;

    void setEnabled(bool Enabled);

    bool isEnabled() const;

};


} }  // namespaces


#endif /* __OPENFLUID_FLUIDX_WAREDESCRIPTOR_HPP__ */
