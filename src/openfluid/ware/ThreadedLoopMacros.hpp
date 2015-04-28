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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_WARE_THREADEDLOOPMACROS_HPP__
#define __OPENFLUID_WARE_THREADEDLOOPMACROS_HPP__


#include <functional>

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent>
#endif

#include <QtConcurrentRun>
#include <QFutureSynchronizer>

#include <openfluid/ware/LoopMacros.hpp>


// =====================================================================
// =====================================================================


#define _THREADSYNCID(_id) _M_##_id##_Sync


#define _APPLY_UNITS_ORDERED_LOOP_THREADED_WITHID(id,unitsclass,funcptr,...) \
  openfluid::core::UnitsList_t* _UNITSLISTID(id) = mp_SpatialData->spatialUnits(unitsclass)->list(); \
  if (_UNITSLISTID(id) != NULL) \
  { \
    openfluid::core::UnitsList_t::iterator _UNITSLISTITERID(id) = _UNITSLISTID(id)->begin(); \
    if (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end()) \
    { \
      openfluid::core::PcsOrd_t _PCSORDID(id) = _UNITSLISTITERID(id)->getProcessOrder(); \
      while (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end()) \
      { \
        QFutureSynchronizer<void> _THREADSYNCID(id); \
        while (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end() && \
               _UNITSLISTITERID(id)->getProcessOrder() == _PCSORDID(id)) \
        { \
          try \
          { \
            _THREADSYNCID(id).addFuture(QtConcurrent::run(std::bind(&funcptr,\
                                                                      this,\
                                                                      &(*_UNITSLISTITERID(id)),## __VA_ARGS__)));\
            if (_THREADSYNCID(id).futures().size() == OPENFLUID_GetSimulatorMaxThreads())\
            { \
              _THREADSYNCID(id).waitForFinished(); \
              _THREADSYNCID(id).clearFutures(); \
            }\
          }\
          catch (QtConcurrent::UnhandledException& E) \
          { \
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, \
                                                      "QtConcurrent::UnhandledException in threaded loop"); \
          } \
          ++_UNITSLISTITERID(id); \
        } \
        _THREADSYNCID(id).waitForFinished(); \
        _THREADSYNCID(id).clearFutures(); \
        if (_UNITSLISTITERID(id) != _UNITSLISTID(id)->end()) _PCSORDID(id) = _UNITSLISTITERID(id)->getProcessOrder(); \
      } \
    } \
  }

/**
  Macro for applying a threaded simulator to each unit of a class, following their process order
  @param[in] unitsclass name of the units class
  @param[in] funcptr member simulator name
  @param[in] ... extra parameters to pass to the member simulator
*/
#define APPLY_UNITS_ORDERED_LOOP_THREADED(unitsclass,funcptr,...) \
    _APPLY_UNITS_ORDERED_LOOP_THREADED_WITHID(__LINE__,unitsclass,funcptr,## __VA_ARGS__)



#define _APPLY_ALLUNITS_ORDERED_LOOP_THREADED_WITHID(id,funcptr,...) \
  openfluid::core::UnitsPtrList_t* _UNITSPTRLISTID(id) = mp_SpatialData->allSpatialUnits(); \
  if (_UNITSPTRLISTID(id) != NULL) \
  { \
    openfluid::core::UnitsPtrList_t::iterator _UNITSPTRLISTITERID(id) = _UNITSPTRLISTID(id)->begin(); \
    if (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end()) \
    { \
      openfluid::core::PcsOrd_t _PCSORDID(id) = (*_UNITSPTRLISTITERID(id))->getProcessOrder(); \
      while (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end()) \
      { \
        QFutureSynchronizer<void> _THREADSYNCID(id); \
        while (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end() && \
              (*_UNITSPTRLISTITERID(id))->getProcessOrder() == _PCSORDID(id)) \
        { \
          try \
          { \
            _THREADSYNCID(id).addFuture(QtConcurrent::run(std::bind(&funcptr,\
                                                                      this,\
                                                                      (*_UNITSPTRLISTITERID(id)),## __VA_ARGS__)));\
            if (_THREADSYNCID(id).futures().size() == OPENFLUID_GetSimulatorMaxThreads())\
            { \
              _THREADSYNCID(id).waitForFinished(); \
              _THREADSYNCID(id).clearFutures(); \
            }\
          }\
          catch(QtConcurrent::UnhandledException& E) \
          { \
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, \
                                                      "QtConcurrent::UnhandledException in threaded loop"); \
          } \
          ++_UNITSPTRLISTITERID(id); \
        } \
        _THREADSYNCID(id).waitForFinished(); \
        _THREADSYNCID(id).clearFutures(); \
        if (_UNITSPTRLISTITERID(id) != _UNITSPTRLISTID(id)->end())\
          _PCSORDID(id) = (*_UNITSPTRLISTITERID(id))->getProcessOrder(); \
      } \
    } \
  }

/**
  Macro for applying a threaded simulator to each unit of the domain, following their process order
  @param[in] funcptr member simulator name
  @param[in] ... extra parameters to pass to the member simulator
*/
#define APPLY_ALLUNITS_ORDERED_LOOP_THREADED(funcptr,...) \
    _APPLY_ALLUNITS_ORDERED_LOOP_THREADED_WITHID(__LINE__,funcptr,## __VA_ARGS__)




// =====================================================================
// =====================================================================


#endif /* __OPENFLUID_WARE_THREADEDLOOPMACROS_HPP__ */
