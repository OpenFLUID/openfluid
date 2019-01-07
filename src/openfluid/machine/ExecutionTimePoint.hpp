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
  @file ExecutionTimePoint.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_EXECUTIONTIMEPOINT_HPP__
#define __OPENFLUID_MACHINE_EXECUTIONTIMEPOINT_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>


// =====================================================================
// =====================================================================


namespace openfluid { namespace machine {

struct SortModelItemsByOriginalPosition
{
  bool operator()(ModelItemInstance* a, ModelItemInstance* b) const
  {
    return (a->OriginalPosition < b->OriginalPosition);
  }
};


// =====================================================================
// =====================================================================


class OPENFLUID_API ExecutionTimePoint
{
  private:
  
    std::list<ModelItemInstance*> m_ItemsPtrList;

    openfluid::core::TimeIndex_t m_TimeIndex;


  public:

    /**
      Constructor
      @param[in] TimeIndex the time index of the execution time point
    */
    ExecutionTimePoint(openfluid::core::TimeIndex_t TimeIndex);

    ~ExecutionTimePoint();

    /**
      Appends a model item instance to the stack of the execution time point
      @param[in] Item a pointer to the model item instance to append
    */
    void appendItem(openfluid::machine::ModelItemInstance* Item);

    /**
      Returns true if there is at least one model item to process in the execution time point
      @return true if there is at least one model item
    */
    inline bool hasItemsToProcess() const
    {
      return !m_ItemsPtrList.empty();
    }

    /**
      Executes the runStep() method of the next item and pops it out of the stack
      @return the scheduling request returned by the runStep() once it is completed
    */
    openfluid::base::SchedulingRequest processNextItem();

    /**
      Returns the next item to process
      @return a pointer to the next item to process
    */
    inline openfluid::machine::ModelItemInstance* nextItem() const
    {
      return m_ItemsPtrList.front();
    }

    /**
      Returns the time index of the execution time point
      @return the time index
    */
    inline openfluid::core::TimeIndex_t getTimeIndex() const
    {
      return m_TimeIndex;
    }

    /**
      Sorts model items of the execution time point using their original position in the coupled model
    */
    inline void sortByOriginalPosition()
    {
      m_ItemsPtrList.sort(SortModelItemsByOriginalPosition());
    }

};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_EXECUTIONTIMEPOINT_HPP__ */
