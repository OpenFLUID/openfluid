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
  \file debug.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

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
    std::vector<openfluid::core::InputDataName_t> _M_DBG_IDataNames = (unitptr)->getInputData()->getInputDataNames(); \
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << "   - Input data: "; \
    for (unsigned int i=0; i<_M_DBG_IDataNames.size();i++) OFDBG_OUTSTREAM << _M_DBG_IDataNames[i] << " , "; \
    OFDBG_OUTSTREAM << std::endl; \
    std::vector<openfluid::core::VariableName_t> _M_DBG_VarNames = (unitptr)->getVariables()->getVariablesNames(); \
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
      OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << "   - " << (*_M_DBG_EvInfoiter).first << " = " << (*_M_DBG_EvInfoiter).second << std::endl; \
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
    OFDBG_OUTSTREAM << openfluid::config::DEBUG_PREFIX << " Event collection size : " << eventcoll.getEventsList()->size() << std::endl; \
    openfluid::core::EventsList_t::iterator _M_DBG_EvListiter; \
    for (_M_DBG_EvListiter=(EvColl.getEventsList())->begin(); _M_DBG_EvListiter != (EvColl.getEventsList())->end(); _M_DBG_EvListiter++) \
    { \
      OFDBG_EVENT(&(*_M_DBG_EvListiter)); \
    } \
  }

#else

#define OFDBG_EVENTCOLLECTION(eventcoll)

#endif


#endif /* __DEBUG_HPP__ */
