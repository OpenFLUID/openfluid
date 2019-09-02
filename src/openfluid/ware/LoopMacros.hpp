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
  @file LoopMacros.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_WARE_LOOPMACROS_HPP__
#define __OPENFLUID_WARE_LOOPMACROS_HPP__


// =====================================================================
// =====================================================================


#define _UNITID(_id) _M_##_id##_Unit

#define _UNITSLISTID(_id) _M_##_id##_UList

#define _UNITSPTRLISTID(_id) _M_##_id##_UList

#define _PCSORDID(_id) _M_##_id##_PcsOrd

#define _UNITSLISTITERID(_id) _M_##_id##_UListit

#define _UNITSPTRLISTITERID(_id) _M_##_id##_UPtrListit

#define _EVENTSLISTITERID(_id) _M_##_id##_EListit


#define _OPENFLUID_UNITS_ORDERED_LOOP_WITHID(unitsclass,unitptr,id) \
    openfluid::core::UnitsList_t* _UNITSLISTID(id) = mp_SpatialData->spatialUnits(unitsclass)->list(); \
    if (_UNITSLISTID(id) != nullptr && !(_UNITSLISTID(id)->empty())) \
      for (openfluid::core::UnitsList_t::iterator _UNITSLISTITERID(id) = _UNITSLISTID(id)->begin(); \
           unitptr = &(*_UNITSLISTITERID(id)),_UNITSLISTITERID(id) != _UNITSLISTID(id)->end(); \
           ++_UNITSLISTITERID(id))

/**
  Macro for a loop processing all units of a class, following their process order
  @param[in] unitsclass name of the units class
  @param[out] unitptr pointer to a openfluid::core::SpatialUnit object,
  automatically pointing to the current processed unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Compute code", "Loops"],
    "title" : "Ordered loop on spatial units of a class",
    "text" : ["OPENFLUID_UNITS_ORDERED_LOOP(\"%%SEL_START%%unitsclass%%SEL_END%%\",UnitPtr)\n",
              "%%INDENT%%{\n%%INDENT%%\n%%INDENT%%}"]
  }
  @endcond
*/
#define OPENFLUID_UNITS_ORDERED_LOOP(unitsclass,unitptr) \
    _OPENFLUID_UNITS_ORDERED_LOOP_WITHID(unitsclass,unitptr,__LINE__)


#define _OPENFLUID_ALLUNITS_ORDERED_LOOP_WITHID(unitptr,id) \
    openfluid::core::UnitsPtrList_t* _UNITSPTRLISTID(id) = mp_SpatialData->allSpatialUnits(); \
    for (openfluid::core::UnitsPtrList_t::iterator _UNITSPTRLISTITERID(id) = _UNITSPTRLISTID(id)->begin(); \
         unitptr = (*_UNITSPTRLISTITERID(id)),_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end(); \
         ++_UNITSPTRLISTITERID(id))

/**
  Macro for a loop processing all units, following their process order
  @param[out] unitptr pointer to a openfluid::core::SpatialUnit object,
  automatically pointing to the current processed unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Compute code", "Loops"],
    "title" : "Ordered loop on all spatial units",
    "text" : "OPENFLUID_ALLUNITS_ORDERED_LOOP(%%SEL_START%%UnitPtr%%SEL_END%%)\n%%INDENT%%{\n%%INDENT%%\n%%INDENT%%}"
  }
  @endcond
*/
#define OPENFLUID_ALLUNITS_ORDERED_LOOP(unitptr) \
    _OPENFLUID_ALLUNITS_ORDERED_LOOP_WITHID(unitptr,__LINE__)


#define _OPENFLUID_UNITSLIST_LOOP_WITHID(unitslist,unitptr,id) \
    if (unitslist != nullptr) \
      for (openfluid::core::UnitsPtrList_t::iterator _UNITSLISTITERID(id) = unitslist->begin(); \
           unitptr = *_UNITSLISTITERID(id),_UNITSLISTITERID(id) != unitslist->end(); \
           ++_UNITSLISTITERID(id))

/**
  Macro for a loop processing a list of units
  @param[in] unitslist pointer to a list of openfluid::core::Unit
  @param[out] unitptr unitptr pointer to a openfluid::core::SpatialUnit object,
  pointing to the current processed unit

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Loops"],
    "title" : "Loop on a spatial units list",
    "text" : ["OPENFLUID_UNITSLIST_LOOP(%%SEL_START%%UnitsList%%SEL_END%%,UnitPtr)\n",
              "%%INDENT%%{\n%%INDENT%%\n%%INDENT%%}"]
  }
  @endcond
*/
#define OPENFLUID_UNITSLIST_LOOP(unitslist,unitptr) \
    _OPENFLUID_UNITSLIST_LOOP_WITHID(unitslist,unitptr,__LINE__)


#define _OPENFLUID_EVENT_COLLECTION_LOOP_WITHID(id,evlist,evobj) \
    for(openfluid::core::EventsList_t::iterator _EVENTSLISTITERID(id) = (evlist)->begin(); \
        evobj = &(*_EVENTSLISTITERID(id)), _EVENTSLISTITERID(id) != (evlist)->end(); \
       ++_EVENTSLISTITERID(id))

/**
  Macro for a loop processing an events list from an event collection
  @param[in] evlist pointer to a list of events
  @param[out] evobj the current processed event

  @cond OpenFLUID:completion
  {
    "contexts" : ["ANYWARE"],
    "menupath" : ["Compute code", "Loops"],
    "title" : "Loop on an events collection",
    "text" : ["OPENFLUID_EVENT_COLLECTION_LOOP(%%SEL_START%%EvList%%SEL_END%%,Event)",
              "\n%%INDENT%%{\n%%INDENT%%\n%%INDENT%%}"]
  }
  @endcond
*/
#define OPENFLUID_EVENT_COLLECTION_LOOP(evlist,evobj) \
    _OPENFLUID_EVENT_COLLECTION_LOOP_WITHID(__LINE__,evlist,evobj)


#endif /* __OPENFLUID_WARE_LOOPMACROS_HPP__ */
