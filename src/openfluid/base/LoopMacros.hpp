/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/

/**
  \file LoopMacros.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __LOOPMACROS_HPP__
#define __LOOPMACROS_HPP__



// =====================================================================
// =====================================================================


#define _UNITID(_id) _M_##_id##_Unit

#define _UNITSLISTID(_id) _M_##_id##_UList

#define _UNITSPTRLISTID(_id) _M_##_id##_UList

#define _THREADPOOLID(_id) _M_##_id##_Pool

#define _PCSORDID(_id) _M_##_id##_PcsOrd

#define _UNITSLISTITERID(_id) _M_##_id##_UListit

#define _UNITSPTRLISTITERID(_id) _M_##_id##_UPtrListit

#define _EVENTSLISTITERID(_id) _M_##_id##_EListit



#define _OPENFLUID_UNITS_ORDERED_LOOP_WITHID(unitclass,unitptr,id) \
    openfluid::core::UnitsList_t* _UNITSLISTID(id) = mp_CoreData->getUnits(unitclass)->getList(); \
    if (_UNITSLISTID(id) != NULL && !(_UNITSLISTID(id)->empty())) \
      for (openfluid::core::UnitsList_t::iterator _UNITSLISTITERID(id) = _UNITSLISTID(id)->begin(); \
           unitptr = &(*_UNITSLISTITERID(id)),_UNITSLISTITERID(id) != _UNITSLISTID(id)->end(); \
           ++_UNITSLISTITERID(id))

/**
  Macro for a loop processing all units of a class, following their process order
  @param[in] unitclass name of the unit class
  @param[out] unitptr pointer to a openfluid::core::Unit objects, automatically pointing to the current processed unit
*/
#define OPENFLUID_UNITS_ORDERED_LOOP(unitclass,unitptr) \
    _OPENFLUID_UNITS_ORDERED_LOOP_WITHID(unitclass,unitptr,__LINE__)



#define _APPLY_UNITS_ORDERED_LOOP_THREADED_WITHID(id,unitclass,funcptr,...) \
  openfluid::core::UnitsList_t* _UNITSLISTID(id) = mp_CoreData->getUnits(unitclass)->getList(); \
  if (_UNITSLISTID(id) != NULL) \
  { \
    openfluid::core::UnitsList_t::iterator _UNITSLISTITERID(id) = _UNITSLISTID(id)->begin(); \
    if (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end()) \
    { \
      openfluid::core::PcsOrd_t _PCSORDID(id) = _UNITSLISTITERID(id)->getProcessOrder(); \
      while (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end()) \
      { \
        Glib::ThreadPool _THREADPOOLID(id)(OPENFLUID_GetFunctionMaxThreads(),true); \
        while (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end() && _UNITSLISTITERID(id)->getProcessOrder() == _PCSORDID(id)) \
        { \
          _THREADPOOLID(id).push(sigc::bind(sigc::mem_fun(*this,&funcptr),&(*_UNITSLISTITERID(id)), ## __VA_ARGS__)); \
          ++_UNITSLISTITERID(id); \
        } \
        _THREADPOOLID(id).shutdown(); \
        if (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end()) _PCSORDID(id) = _UNITSLISTITERID(id)->getProcessOrder(); \
      } \
    } \
  }

/**
  Macro for applying a threaded function to each unit of a class, following their process order
  @param[in] unitclass name of the unit class
  @param[in] funcptr member function name
  @param[in] ... extra parameters to pass to the member function
*/
#define APPLY_UNITS_ORDERED_LOOP_THREADED(unitclass,funcptr,...) \
    _APPLY_UNITS_ORDERED_LOOP_THREADED_WITHID(__LINE__,unitclass,funcptr,## __VA_ARGS__)



#define _OPENFLUID_ALLUNITS_ORDERED_LOOP_WITHID(unitptr,id) \
    openfluid::core::UnitsPtrList_t* _UNITSPTRLISTID(id) = mp_CoreData->getUnitsGlobally(); \
    for (openfluid::core::UnitsPtrList_t::iterator _UNITSPTRLISTITERID(id) = _UNITSPTRLISTID(id)->begin(); \
         unitptr = (*_UNITSPTRLISTITERID(id)),_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end(); \
         ++_UNITSPTRLISTITERID(id))

/**
  Macro for a loop processing all units, following their process order
  @param[out] unitptr pointer to a openfluid::core::Unit objects, automatically pointing to the current processed unit
*/
#define OPENFLUID_ALLUNITS_ORDERED_LOOP(unitptr) \
    _OPENFLUID_ALLUNITS_ORDERED_LOOP_WITHID(unitptr,__LINE__)



#define _APPLY_ALLUNITS_ORDERED_LOOP_THREADED_WITHID(id,funcptr,...) \
  openfluid::core::UnitsPtrList_t* _UNITSPTRLISTID(id) = mp_CoreData->getUnitsGlobally(); \
  if (_UNITSPTRLISTID(id) != NULL) \
  { \
    openfluid::core::UnitsPtrList_t::iterator _UNITSPTRLISTITERID(id) = _UNITSPTRLISTID(id)->begin(); \
    if (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end()) \
    { \
      openfluid::core::PcsOrd_t _PCSORDID(id) = (*_UNITSPTRLISTITERID(id))->getProcessOrder(); \
      while (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end()) \
      { \
        Glib::ThreadPool _THREADPOOLID(id)(OPENFLUID_GetFunctionMaxThreads(),true); \
        while (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end() && (*_UNITSPTRLISTITERID(id))->getProcessOrder() == _PCSORDID(id)) \
        { \
          _THREADPOOLID(id).push(sigc::bind(sigc::mem_fun(*this,&funcptr),(*_UNITSPTRLISTITERID(id)), ## __VA_ARGS__)); \
          ++_UNITSPTRLISTITERID(id); \
        } \
        _THREADPOOLID(id).shutdown(); \
        if (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end()) _PCSORDID(id) = (*_UNITSPTRLISTITERID(id))->getProcessOrder(); \
      } \
    } \
  }

/**
  Macro for applying a threaded function to each unit of the domain, following their process order
  @param[in] funcptr member function name
  @param[in] ... extra parameters to pass to the member function
*/
#define APPLY_ALLUNITS_ORDERED_LOOP_THREADED(funcptr,...) \
    _APPLY_ALLUNITS_ORDERED_LOOP_THREADED_WITHID(__LINE__,funcptr,## __VA_ARGS__)



#define _OPENFLUID_UNITSLIST_LOOP_WITHID(unitslist,unitptr,id) \
    if (unitslist != NULL) \
      for (openfluid::core::UnitsPtrList_t::iterator _UNITSLISTITERID(id) = unitslist->begin(); \
           unitptr = *_UNITSLISTITERID(id),_UNITSLISTITERID(id) != unitslist->end(); \
           ++_UNITSLISTITERID(id))

/**
  Macro for a loop processing a list of units
  @param[in] unitslist pointer to a list of openfluid::core::Unit
  @param[out] unitptr pointer to a openfluid::core::Unit objects, pointing to the current processed unit
*/#define OPENFLUID_UNITSLIST_LOOP(unitslist,unitptr) \
    _OPENFLUID_UNITSLIST_LOOP_WITHID(unitslist,unitptr,__LINE__)



#define _OPENFLUID_EVENT_COLLECTION_LOOP_WITHID(id,evlist,evobj) \
    for(openfluid::core::EventsList_t::iterator _EVENTSLISTITERID(id) = (evlist)->begin(); \
        evobj = &(*_EVENTSLISTITERID(id)), _EVENTSLISTITERID(id) != (evlist)->end(); \
       ++_EVENTSLISTITERID(id))

/**
  Macro for a loop processing an events list from an event collection
  @param[in] evlist pointer to a list of events
  @param[out] evobj the current processed event
*/
#define OPENFLUID_EVENT_COLLECTION_LOOP(evlist,evobj) \
    _OPENFLUID_EVENT_COLLECTION_LOOP_WITHID(__LINE__,evlist,evobj)


// =====================================================================
// =====================================================================


#endif /* __LOOPMACROS_HPP__ */
