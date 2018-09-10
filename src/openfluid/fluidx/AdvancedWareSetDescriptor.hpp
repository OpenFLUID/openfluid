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
  @file AdvancedWareSetDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Aline LIBRES <aline.libres@gmail.com>
*/


#ifndef __OPENFLUID_FLUIDX_ADVANCEDWARESETDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_ADVANCEDWARESETDESCRIPTOR_HPP__


#include <list>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/ware/PluggableWare.hpp>


namespace openfluid { namespace fluidx {


template<class M, class I>
class OPENFLUID_API AdvancedWareSetDescriptor
{
  protected:

    M* mp_Descriptor;


  public:


    AdvancedWareSetDescriptor(M& Desc):
      mp_Descriptor(&Desc)
    {

    }


    // =====================================================================
    // =====================================================================


    virtual ~AdvancedWareSetDescriptor()
    {  }


    // =====================================================================
    // =====================================================================


    /**
      Runs checking of descriptors
    */
    virtual void check() = 0;


    // =====================================================================
    // =====================================================================


    /**
      Returns the ID of a ware given by a pointer on its descriptor
      @param[in] Item the pointer to the descriptor
      @return the ID of the ware
    */
    virtual openfluid::ware::WareID_t getID(I* Item) const = 0;


    // =====================================================================
    // =====================================================================


    /**
      Returns a const reference to the list of pointers to ware descriptors
      @return the list of pointers to descriptors
    */
    const std::list<I*>& items() const
    {
      return mp_Descriptor->items();
    }


    // =====================================================================
    // =====================================================================


    /**
      Sets the list of pointers to ware descriptors
      @param[in] Items the list of pointers to descriptors
    */
    void setItems(const std::list<I*>& Items)
    {
        mp_Descriptor->items() = Items;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a reference to the ware descriptor at a given index
      @param[in] Index the given index
      @return the descriptor
      @throws openfluid::base::FrameworkException Index out of bounds
    */
    I& itemAt(unsigned int Index) const
    {
      std::list<I*>& Items = mp_Descriptor->items();

      if (Index < Items.size())
      {
        typename std::list<I*>::iterator it = Items.begin();
        std::advance(it, Index);

        return **it;
      }
      else
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Index out of bounds");
    }


    // =====================================================================
    // =====================================================================


    /**
      Appends a pointer to ware descriptor at the end of the wares set descriptor
      @param[in] Item the descriptor to add
    */
    void appendItem(I* Item)
    {
      if (Item)
        mp_Descriptor->appendItem(Item);
    }


    // =====================================================================
    // =====================================================================


    /**
      Inserts a pointer to ware descriptor at a given index in the wares set descriptor
      @param[in] Item the descriptor to insert
      @param[in] Index the index where the descriptor is inserted
      @throws openfluid::base::FrameworkException Index out of bounds
    */
    void insertItem(I* Item, unsigned int Index)
    {
      std::list<I*>& Items = mp_Descriptor->items();

      if (Index == 0)
        Items.insert(Items.begin(), Item);
      else if (Index < Items.size())
      {
        typename std::list<I*>::iterator it = Items.begin();
        std::advance(it, Index);

        Items.insert(it, Item);
      }
      else
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Index out of bounds");
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
      std::list<I*>& Items = mp_Descriptor->items();

      if (Index < Items.size())
      {
        typename std::list<I*>::iterator it = Items.begin();
        std::advance(it, Index);

        Items.erase(it);
      }
      else
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Index out of bounds");
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
        return;

      std::list<I*>& Items = mp_Descriptor->items();

      unsigned int Last = Items.size() - 1;

      if (indexFrom > Last || indexTo > Last)
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Index out of bounds");

      typename std::list<I*>::const_iterator itFrom = Items.begin();
      std::advance(itFrom, indexFrom);

      I* Item = *itFrom;

      removeItem(indexFrom);

      if (indexTo == Last)
        appendItem(Item);
      else
        insertItem(Item, indexTo);
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
      std::list<I*>& Items = mp_Descriptor->items();

      for (typename std::list<I*>::iterator it = Items.begin(); it != Items.end(); ++it)
      {
        if (getID(*it) == ID)
          return std::distance(Items.begin(), it);
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
    int findFirstItem(const I* Item) const
    {
      std::list<I*>& Items = mp_Descriptor->items();

      typename std::list<I*>::iterator it = std::find(Items.begin(), Items.end(), Item);

      if (it != Items.end())
        return std::distance(Items.begin(), it);

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

      std::list<I*>& Items = mp_Descriptor->items();

      for (typename std::list<I*>::const_iterator it = Items.begin(); it != Items.end(); ++it)
        IDs.push_back(getID(*it));

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
      return mp_Descriptor->items().size();
    }


    // =====================================================================
    // =====================================================================


    void setGlobalParameter(const openfluid::ware::WareParamKey_t& Key,
                            const openfluid::ware::WareParamValue_t& Value)
    {
      mp_Descriptor->setGlobalParameter(Key, Value);
    }


    // =====================================================================
    // =====================================================================


    void setGlobalParameters(const openfluid::ware::WareParams_t& Params)
    {
      mp_Descriptor->setGlobalParameters(Params);
    }


    // =====================================================================
    // =====================================================================


    openfluid::ware::WareParams_t getGlobalParameters() const
    {
      return mp_Descriptor->getGlobalParameters();
    }


    // =====================================================================
    // =====================================================================


    void eraseGlobalParameter(const openfluid::ware::WareParamKey_t& Key)
    {
      mp_Descriptor->eraseGlobalParameter(Key);
    }
};


} }


#endif /* __OPENFLUID_FLUIDX_ADVANCEDWARESETDESCRIPTOR_HPP__ */
