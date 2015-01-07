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
  \file debug.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_DEBUG_HPP__
#define __OPENFLUID_DEBUG_HPP__

// =====================================================================
// =====================================================================


#ifndef NDEBUG

#include <iostream>
#include <openfluid/config.hpp>

#define OFDBG_ENABLED 1
#define OFDBG_OUTSTREAM std::cout

#else

#define OFDBG_ENABLED 0
#define OFDBG_OUTSTREAM

#endif


// =====================================================================
// =====================================================================


#ifndef NDEBUG

#define OFDBG_LOCATE { OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << " File " << __FILE__ << ", Line " << __LINE__ << std::endl; }

#else

#define OFDBG_LOCATE ;

#endif


// =====================================================================
// =====================================================================


#ifndef NDEBUG

#define OFDBG_BANNER \
  { \
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << std::endl; \
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << "   OpenFLUID debugging mode is enabled" << std::endl; \
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << std::endl; \
  }

#else

#define OFDBG_BANNER

#endif


// =====================================================================
// =====================================================================


#ifndef NDEBUG

#define OFDBG_MESSAGE(stream) \
  { OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << " " << stream << std::endl; }

#else

#define OFDBG_MESSAGE(stream)

#endif


// =====================================================================
// =====================================================================


#ifndef NDEBUG

#define OFDBG_UNIT(unitptr) \
  { OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << " Unit class " << (unitptr)->getClass() << ", ID " << (unitptr)->getID() << std::endl; }

#else

#define OFDBG_UNIT(unitptr)

#endif


// =====================================================================
// =====================================================================


#ifndef NDEBUG

#define OFDBG_UNIT_EXTENDED(unitptr) \
  { \
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << " Unit class " << (unitptr)->getClass() << ", ID " << (unitptr)->getID() << std::endl; \
    std::vector<openfluid::core::AttributeName_t> _M_DBG_AttrsNames = (unitptr)->attributes()->getAttributesNames(); \
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << "   - Attributes: "; \
    for (unsigned int i=0; i<_M_DBG_AttrsNames.size();i++) OFDBG_OUTSTREAM << _M_DBG_AttrsNames[i] << " , "; \
    OFDBG_OUTSTREAM << std::endl; \
    std::vector<openfluid::core::VariableName_t> _M_DBG_VarNames = (unitptr)->variables()->getVariablesNames(); \
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << "   - Variables: "; \
    for (unsigned int i=0; i<_M_DBG_VarNames.size();i++) OFDBG_OUTSTREAM << _M_DBG_VarNames[i] << " , "; \
    OFDBG_OUTSTREAM << std::endl; \
  }

#else

#define OFDBG_UNIT_EXTENDED(unitptr)

#endif


// =====================================================================
// =====================================================================


#ifndef NDEBUG

#define OFDBG_EVENT(eventptr) \
  { \
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << " Event at " << (eventptr)->getDateTime().getAsISOString() << std::endl; \
    openfluid::core::Event::EventInfosMap_t::iterator _M_DBG_EvInfoiter; \
    for (_M_DBG_EvInfoiter = (eventptr)->getInfos().begin();_M_DBG_EvInfoiter != (eventptr)->getInfos().end();++_M_DBG_EvInfoiter) \
    { \
      OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << "   - " << (*_M_DBG_EvInfoiter).first << " = " << (*_M_DBG_EvInfoiter).second.get() << std::endl; \
    } \
  }

#else

#define OFDBG_EVENT(eventptr)

#endif


// =====================================================================
// =====================================================================


#ifndef NDEBUG

#define OFDBG_EVENTCOLLECTION(eventcoll) \
  { \
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << " Event collection size : " << eventcoll.eventsList()->size() << std::endl; \
    openfluid::core::EventsList_t::iterator _M_DBG_EvListiter; \
    for (_M_DBG_EvListiter=(EvColl.eventsList())->begin(); _M_DBG_EvListiter != (EvColl.eventsList())->end(); _M_DBG_EvListiter++) \
    { \
      OFDBG_EVENT(&(*_M_DBG_EvListiter)); \
    } \
  }

#else

#define OFDBG_EVENTCOLLECTION(eventcoll)

#endif


#endif /* __OPENFLUID_DEBUG_HPP__ */
