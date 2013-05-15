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
 \file SimulRunPresenter.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulRunPresenter.hpp"

#include "SimulRunModel.hpp"
#include "SimulRunView.hpp"

// =====================================================================
// =====================================================================


SimulRunPresenter::SimulRunPresenter(SimulRunModel& Model, SimulRunView& View) :
  m_Model(Model), m_View(View), m_HaveValuesToBeStore(true)
{
  m_Model.signal_FromAppDescriptorChanged().connect(sigc::mem_fun(*this,
      &SimulRunPresenter::whenFromAppDescriptorChanged));
  m_Model.signal_FromAppValidityChanged().connect(sigc::mem_fun(*this,
      &SimulRunPresenter::whenFromAppValidityChanged));

  m_View.signal_ValuesBuffToggle().connect(sigc::mem_fun(*this,
      &SimulRunPresenter::whenFromUserValuesBufferSetToggled));
  m_View.signal_BeginChanged().connect(sigc::mem_fun(*this,
      &SimulRunPresenter::whenFromUserBeginChanged));
  m_View.signal_EndChanged().connect(sigc::mem_fun(*this,
      &SimulRunPresenter::whenFromUserEndChanged));
  m_View.signal_DeltaTChanged().connect(sigc::mem_fun(*this,
      &SimulRunPresenter::whenFromUserDeltaTChanged));
  m_View.signal_ConstraintChanged().connect(sigc::mem_fun(*this,
        &SimulRunPresenter::whenFromUserConstraintChanged));
  m_View.signal_ValuesBuffChanged().connect(sigc::mem_fun(*this,
      &SimulRunPresenter::whenFromUserValuesBuffChanged));
}

// =====================================================================
// =====================================================================


void SimulRunPresenter::whenFromAppDescriptorChanged()
{
  m_HaveValuesToBeStore = false;

  m_View.setDeltaT(m_Model.getDeltaT());
  m_View.setConstraint(m_Model.getConstraint());
  m_View.setBegin(m_Model.getBegin());
  m_View.setEnd(m_Model.getEnd());

  m_View.setValuesBuff(m_Model.getValuesBuff());
  m_View.setValuesBuffIsSet(m_Model.isValuesBuffSet());

  m_HaveValuesToBeStore = true;

  whenFromAppValidityChanged();
}

// =====================================================================
// =====================================================================


void SimulRunPresenter::whenFromAppValidityChanged()
{
  m_View.setBeginBG(m_Model.getBeginColor());
  m_View.setEndBG(m_Model.getEndColor());
}

// =====================================================================
// =====================================================================


void SimulRunPresenter::whenFromUserValuesBufferSetToggled()
{
  if (m_HaveValuesToBeStore)
  {
    m_View.setValuesBuffIsSet(m_View.isValuesBuffSet());
    m_Model.setValuesBuffIsSet(m_View.isValuesBuffSet());
  }
}

// =====================================================================
// =====================================================================


void SimulRunPresenter::whenFromUserBeginChanged()
{
  m_Model.setBegin(m_View.getBegin());
}

// =====================================================================
// =====================================================================


void SimulRunPresenter::whenFromUserEndChanged()
{
  m_Model.setEnd(m_View.getEnd());
}


// =====================================================================
// =====================================================================


void SimulRunPresenter::whenFromUserDeltaTChanged()
{
  m_Model.setDeltaT(m_View.getDeltaT());
}


// =====================================================================
// =====================================================================


void SimulRunPresenter::whenFromUserConstraintChanged()
{
  m_Model.setConstraint(m_View.getConstraint());
}


// =====================================================================
// =====================================================================


void SimulRunPresenter::whenFromUserValuesBuffChanged()
{
  if (m_HaveValuesToBeStore)
    m_Model.setValuesBuff(m_View.getValuesBuff());
}

