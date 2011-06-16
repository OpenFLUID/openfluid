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
 \file SimulRunModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulRunModel.hpp"

#include <glibmm/i18n.h>

// =====================================================================
// =====================================================================


SimulRunModelImpl::SimulRunModelImpl() :
  m_isCurrentBeginValid(true), m_isCurrentEndValid(true), m_isDateTimeCoherent(
      true), m_isDateTimeValid(true), m_BeginColorString("white"),
      m_EndColorString("white")
{
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunModelImpl::signal_FromAppDescriptorChanged()
{
  return m_signal_FromAppDescriptorChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunModelImpl::signal_FromAppValidityChanged()
{
  return m_signal_FromAppValidityChanged;
}


// =====================================================================
// =====================================================================


sigc::signal<void> SimulRunModelImpl::signal_SimulRunChanged()
{
  return m_signal_SimulRunChanged;
}

// =====================================================================
// =====================================================================


void SimulRunModelImpl::setEngineRequirements(
    openfluid::base::RunDescriptor& RunDesc)
{
  mp_RunDesc = &RunDesc;
  m_signal_FromAppDescriptorChanged.emit();
  checkDateTimeCoherence();
}

// =====================================================================
// =====================================================================


int SimulRunModelImpl::getDelta()
{
  return mp_RunDesc->getDeltaT();
}

// =====================================================================
// =====================================================================


std::string SimulRunModelImpl::getBeginColor()
{
  return m_BeginColorString;
}

// =====================================================================
// =====================================================================


std::string SimulRunModelImpl::getEndColor()
{
  return m_EndColorString;
}

// =====================================================================
// =====================================================================


bool SimulRunModelImpl::isDateTimeCoherent()
{
  return m_isDateTimeCoherent;
}

// =====================================================================
// =====================================================================


bool SimulRunModelImpl::isDateTimeValid()
{
  return m_isDateTimeValid;
}

// =====================================================================
// =====================================================================


std::string SimulRunModelImpl::getBegin()
{
  return mp_RunDesc->getBeginDate().getAsISOString();
}

// =====================================================================
// =====================================================================


std::string SimulRunModelImpl::getEnd()
{
  return mp_RunDesc->getEndDate().getAsISOString();
}

// =====================================================================
// =====================================================================


int SimulRunModelImpl::getValuesBuff()
{
  return mp_RunDesc->getValuesBufferSize();
}

// =====================================================================
// =====================================================================


bool SimulRunModelImpl::isValuesBuffSet()
{
  return mp_RunDesc->isUserValuesBufferSize();
}

// =====================================================================
// =====================================================================


int SimulRunModelImpl::getFilesBuff()
{
  return mp_RunDesc->getFilesBufferSizeInKB();
}

// =====================================================================
// =====================================================================


void SimulRunModelImpl::setDelta(int Value)
{
  mp_RunDesc->setDeltaT(Value);

  m_signal_SimulRunChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulRunModelImpl::setBegin(std::string Begin)
{
  if (Begin != getBegin())
  {
    openfluid::core::DateTime DT;
    if (DT.setFromISOString(Begin))
    {
      mp_RunDesc->setBeginDate(DT);
      m_isCurrentBeginValid = true;
    } else
      m_isCurrentBeginValid = false;

    checkDateTimeCoherence();
  }
}

// =====================================================================
// =====================================================================


void SimulRunModelImpl::setEnd(std::string End)
{
  if (End != getEnd())
  {
    openfluid::core::DateTime DT;
    if (DT.setFromISOString(End))
    {
      mp_RunDesc->setEndDate(DT);
      m_isCurrentEndValid = true;
    } else
      m_isCurrentEndValid = false;

    checkDateTimeCoherence();
  }
}

// =====================================================================
// =====================================================================


void SimulRunModelImpl::checkDateTimeCoherence()
{
  m_isDateTimeCoherent
      = (mp_RunDesc->getBeginDate() < mp_RunDesc->getEndDate());

  if (!m_isCurrentBeginValid)
    m_BeginColorString = "red";
  if (!m_isCurrentEndValid)
    m_EndColorString = "red";

  if (m_isCurrentBeginValid && m_isDateTimeCoherent)
    m_BeginColorString = "white";
  if (m_isCurrentEndValid && m_isDateTimeCoherent)
    m_EndColorString = "white";

  if (m_isCurrentBeginValid && !m_isDateTimeCoherent)
    m_BeginColorString = "orange";
  if (m_isCurrentEndValid && !m_isDateTimeCoherent)
    m_EndColorString = "orange";

  m_isDateTimeValid = (m_isCurrentBeginValid && m_isCurrentEndValid && m_isDateTimeCoherent);

  m_signal_FromAppValidityChanged.emit();

  m_signal_SimulRunChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulRunModelImpl::setValuesBuffIsSet(bool IsSet)
{
  if (!IsSet)
    mp_RunDesc->unsetUserValuesBufferSize();
  else
    setValuesBuff(getValuesBuff());

  m_signal_SimulRunChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulRunModelImpl::setValuesBuff(int Value)
{
  mp_RunDesc->setValuesBufferSize(Value);

  m_signal_SimulRunChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulRunModelImpl::setFilesBuff(int Value)
{
  mp_RunDesc->setFilesBufferSizeInKB(Value);

  m_signal_SimulRunChanged.emit();
}

