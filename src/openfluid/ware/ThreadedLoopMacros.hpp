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
  @file ThreadedLoopMacros.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_WARE_THREADEDLOOPMACROS_HPP__
#define __OPENFLUID_WARE_THREADEDLOOPMACROS_HPP__


#include <functional>
#include <thread>
#include <vector>
#include <system_error>

#include <openfluid/ware/LoopMacros.hpp>


// =====================================================================
// =====================================================================


#define _THREADGROUPID(_id) _M_##_id##_ThreadGroup
#define _THREADID(_id) _M_##_id##_Thread


// =====================================================================
// =====================================================================


#define _APPLY_UNITS_ORDERED_LOOP_THREADED_WITHID(id,unitsclass,funcptr,...) \
  openfluid::core::UnitsList_t* _UNITSLISTID(id) = mp_SpatialData->spatialUnits(unitsclass)->list(); \
  if (_UNITSLISTID(id) != nullptr) \
  { \
    openfluid::core::UnitsList_t::iterator _UNITSLISTITERID(id) = _UNITSLISTID(id)->begin(); \
    if (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end()) \
    { \
      openfluid::core::PcsOrd_t _PCSORDID(id) = _UNITSLISTITERID(id)->getProcessOrder(); \
      while (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end()) \
      { \
        std::vector<std::thread> _THREADGROUPID(id); \
        while (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end() && \
               _UNITSLISTITERID(id)->getProcessOrder() == _PCSORDID(id)) \
        { \
          try \
          { \
            _THREADGROUPID(id).push_back(std::thread(std::bind(&funcptr,this,\
                                                    &(*_UNITSLISTITERID(id)),## __VA_ARGS__))); \
            if (_THREADGROUPID(id).size() == (unsigned int)OPENFLUID_GetSimulatorMaxThreads()) \
            { \
              for (auto& _THREADID(id) : _THREADGROUPID(id)) \
                _THREADID(id).join(); \
              _THREADGROUPID(id).clear(); \
            } \
          } \
          catch (std::system_error& E) \
          { \
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, \
                                                      "Error in threaded loop (" + std::string(E.what()) +")"); \
          } \
          ++_UNITSLISTITERID(id); \
        } \
        for (auto& _THREADID(id) : _THREADGROUPID(id)) \
          _THREADID(id).join(); \
        _THREADGROUPID(id).clear(); \
        if (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end()) \
          _PCSORDID(id) = _UNITSLISTITERID(id)->getProcessOrder(); \
      } \
    } \
  }

/**
  Macro for applying a threaded simulator to each unit of a class, following their process order
  @param[in] unitsclass name of the units class
  @param[in] funcptr member simulator name
  @param[in] ... extra parameters to pass to the member simulator

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Compute code", "Loops"],
    "title" : "Threaded ordered loop on spatial units of a class",
    "text" : "APPLY_UNITS_ORDERED_LOOP_THREADED(\"%%SEL_START%%UnitsClass%%SEL_END%%\",FuncPtr)"
  }
  @endcond
*/
#define APPLY_UNITS_ORDERED_LOOP_THREADED(unitsclass,funcptr,...) \
    _APPLY_UNITS_ORDERED_LOOP_THREADED_WITHID(__LINE__,unitsclass,funcptr,## __VA_ARGS__)


// =====================================================================
// =====================================================================


#define _APPLY_ALLUNITS_ORDERED_LOOP_THREADED_WITHID(id,funcptr,...) \
  openfluid::core::UnitsPtrList_t* _UNITSPTRLISTID(id) = mp_SpatialData->allSpatialUnits(); \
  if (_UNITSPTRLISTID(id) != nullptr) \
  { \
    openfluid::core::UnitsPtrList_t::iterator _UNITSPTRLISTITERID(id) = _UNITSPTRLISTID(id)->begin(); \
    if (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end()) \
    { \
      openfluid::core::PcsOrd_t _PCSORDID(id) = (*_UNITSPTRLISTITERID(id))->getProcessOrder(); \
      while (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end()) \
      { \
        std::vector<std::thread> _THREADGROUPID(id); \
        while (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end() && \
              (*_UNITSPTRLISTITERID(id))->getProcessOrder() == _PCSORDID(id)) \
        { \
          try \
          { \
            _THREADGROUPID(id).push_back(std::thread(std::bind(&funcptr,this,\
                                                    (*_UNITSPTRLISTITERID(id)),## __VA_ARGS__))); \
            if (_THREADGROUPID(id).size() == (unsigned int)OPENFLUID_GetSimulatorMaxThreads()) \
            { \
              for (auto& _THREADID(id) : _THREADGROUPID(id)) \
                _THREADID(id).join(); \
              _THREADGROUPID(id).clear(); \
            } \
          }\
          catch (std::system_error& E) \
          { \
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, \
                                                      "Error in threaded loop (" + std::string(E.what()) +")"); \
          } \
          ++_UNITSPTRLISTITERID(id); \
        } \
        for (auto& _THREADID(id) : _THREADGROUPID(id)) \
          _THREADID(id).join(); \
        _THREADGROUPID(id).clear(); \
        if (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end())\
          _PCSORDID(id) = (*_UNITSPTRLISTITERID(id))->getProcessOrder(); \
      } \
    } \
  }

/**
  Macro for applying a threaded simulator to each unit of the domain, following their process order
  @param[in] funcptr member simulator name
  @param[in] ... extra parameters to pass to the member simulator

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Compute code", "Loops"],
    "title" : "Threaded ordered loop on all spatial units",
    "text" : "APPLY_ALLUNITS_ORDERED_LOOP_THREADED(%%SEL_START%%FuncPtr%%SEL_END%%)"
  }
  @endcond
*/
#define APPLY_ALLUNITS_ORDERED_LOOP_THREADED(funcptr,...) \
    _APPLY_ALLUNITS_ORDERED_LOOP_THREADED_WITHID(__LINE__,funcptr,## __VA_ARGS__)


// =====================================================================
// =====================================================================


#endif /* __OPENFLUID_WARE_THREADEDLOOPMACROS_HPP__ */
