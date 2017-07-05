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
  @file WareSetDescriptor.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_FLUIDX_WARESETDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_WARESETDESCRIPTOR_HPP__


#include <list>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace fluidx {


template<class T>
class OPENFLUID_API WareSetDescriptor
{
  public:

    typedef std::list<T*> SetDescription_t;


  private:

    SetDescription_t m_SetDescription;

    openfluid::ware::WareParams_t m_Params;


  public:

    WareSetDescriptor<T>()
    {  }


    // =====================================================================
    // =====================================================================


    ~WareSetDescriptor<T>()
    {
      // TODO Manage deletion of items, in relation with advanced descriptors
    }


    // =====================================================================
    // =====================================================================


    inline SetDescription_t& items()
    {
      return (m_SetDescription);
    }


    // =====================================================================
    // =====================================================================


    inline const SetDescription_t& items() const
    {
      return (m_SetDescription);
    }


    // =====================================================================
    // =====================================================================


    void setGlobalParameter(const openfluid::ware::WareParamKey_t& Key,
                            const openfluid::ware::WareParamValue_t& Value)
    {
      m_Params[Key] = Value;
    }


    // =====================================================================
    // =====================================================================


    void setGlobalParameters(const openfluid::ware::WareParams_t& Params)
    {
      openfluid::ware::WareParams_t::const_iterator it;

      for (it=Params.begin();it!=Params.end();++it)
        m_Params[it->first] = it->second;
    }


    // =====================================================================
    // =====================================================================


    openfluid::ware::WareParams_t getGlobalParameters() const
    {
      return m_Params;
    }


    // =====================================================================
    // =====================================================================


    void eraseGlobalParameter(const openfluid::ware::WareParamKey_t& Key)
    {
      m_Params.erase(Key);
    }


    // =====================================================================
    // =====================================================================


    void appendItem(T *Item)
    {
      m_SetDescription.push_back(Item);
    }

};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_MODELDESCRIPTOR_HPP__ */
