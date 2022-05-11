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

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_FLUIDX_WARESETDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_WARESETDESCRIPTOR_HPP__


#include <list>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>


namespace openfluid { namespace fluidx {


template<class T>
class OPENFLUID_API WareSetDescriptor
{
  public:

    typedef std::list<T*> SetDescription_t;


  protected:

    SetDescription_t m_SetDescription;

    openfluid::ware::WareParams_t m_Params;


  public:

    WareSetDescriptor<T>()
    {

    }


    // =====================================================================
    // =====================================================================


    virtual ~WareSetDescriptor<T>()
    {

    }


    // =====================================================================
    // =====================================================================


    inline SetDescription_t& items()
    {
      return m_SetDescription;
    }


    // =====================================================================
    // =====================================================================


    inline const SetDescription_t& items() const
    {
      return m_SetDescription;
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
      {
        m_Params[it->first] = it->second;
      }
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


    void appendItem(T* Item)
    {
      m_SetDescription.push_back(Item);
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the ID of a ware given by a pointer on its descriptor
      @param[in] Item the pointer to the descriptor
      @return the ID of the ware
    */
    virtual openfluid::ware::WareID_t getID(const T* Item) const = 0;


    // =====================================================================
    // =====================================================================


    /**
      Sets the list of pointers to ware descriptors
      @param[in] Items the list of pointers to descriptors
    */
    void setItems(const SetDescription_t& Items)
    {
      m_SetDescription = Items;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a reference to the ware descriptor at a given index
      @param[in] Index the given index
      @return the descriptor
      @throws openfluid::base::FrameworkException Index out of bounds
    */
    T& itemAt(unsigned int Index) const
    {

      if (Index < m_SetDescription.size())
      {
        typename SetDescription_t::const_iterator it = m_SetDescription.begin();
        std::advance(it, Index);

        return **it;
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Index out of bounds");
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Inserts a pointer to ware descriptor at a given index in the wares set descriptor
      @param[in] Item the descriptor to insert
      @param[in] Index the index where the descriptor is inserted
      @throws openfluid::base::FrameworkException Index out of bounds
    */
    void insertItem(T* Item, unsigned int Index)
    {
      if (Index == 0)
      {
        m_SetDescription.insert(m_SetDescription.begin(), Item);
      }
      else if (Index < m_SetDescription.size())
      {
        typename SetDescription_t::iterator it = m_SetDescription.begin();
        std::advance(it, Index);

        m_SetDescription.insert(it, Item);
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Index out of bounds");
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Removes the ware descriptor at a given index from the wares set descriptor
      @param[in] Index the index of the descriptor to remove
      @throws openfluid::base::FrameworkException Index out of bounds
    */
    void removeItem(unsigned int Index)
    {
      if (Index < m_SetDescription.size())
      {
        typename SetDescription_t::iterator it = m_SetDescription.begin();
        std::advance(it, Index);

        m_SetDescription.erase(it);
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Index out of bounds");
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Moves a ware descriptor at a given index to another index in the wares set descriptor
      @param[in] indexFrom initial index of the descriptor
      @param[in] indexTo destination index of the descriptor
      @throws openfluid::base::FrameworkException Index out of bounds
    */
    void moveItem(unsigned int indexFrom, unsigned int indexTo)
    {
      if (indexFrom == indexTo)
      {
        return;
      }

      unsigned int Last = m_SetDescription.size() - 1;

      if (indexFrom > Last || indexTo > Last)
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Index out of bounds");
      }

      typename SetDescription_t::const_iterator itFrom = m_SetDescription.begin();
      std::advance(itFrom, indexFrom);

      T* Item = *itFrom;

      removeItem(indexFrom);

      if (indexTo == Last)
      {
        appendItem(Item);
      }
      else
      {
        insertItem(Item, indexTo);
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Finds the index of the first ware descriptor with the given ID
      @param[in] ID the ID of the searched descriptor
      @return the index of the searched descriptor, -1 otherwise
    */
    int findFirstItem(const openfluid::ware::WareID_t& ID) const
    {
      for (typename SetDescription_t::const_iterator it = m_SetDescription.begin(); it != m_SetDescription.end(); ++it)
      {
        if (getID(*it) == ID)
        {
          return std::distance(m_SetDescription.begin(), it);
        }
      }

      return -1;
    }


    // =====================================================================
    // =====================================================================


    /**
      Finds the index of the first ware descriptor corresponding to the given descriptor
      @param[in] Item the searched descriptor
      @return the index of the searched descriptor, -1 otherwise
    */
    int findFirstItem(const T* Item) const
    {
      typename std::list<T*>::iterator it = std::find(m_SetDescription.begin(), m_SetDescription.end(), Item);

      if (it != m_SetDescription.end())
      {
        return std::distance(m_SetDescription.begin(), it);
      }

      return -1;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns an ordered vector of IDs of wares contained in the wares set descriptor
      @return the vector of IDs
    */
    std::vector<openfluid::ware::WareID_t> getOrderedIDs() const
    {
      std::vector<std::string> IDs;

      for (typename SetDescription_t::const_iterator it = m_SetDescription.begin(); it != m_SetDescription.end(); ++it)
      {
        IDs.push_back(getID(*it));
      }

      return IDs;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the number of ware descriptors in the wares set descriptor
      @return the number of ware descriptors
    */
    int getItemsCount()
    {
      return m_SetDescription.size();
    }

};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_WARESETDESCRIPTOR_HPP__ */
