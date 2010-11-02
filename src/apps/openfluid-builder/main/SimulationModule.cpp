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
  \file SimulationModule.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <iostream>

#include <glibmm/i18n.h>

#include "BuilderHelper.hpp"
#include "SimulationModule.hpp"


// =====================================================================
// =====================================================================

SimulationModule::SimulationModule(openfluid::base::RunDescriptor & RunDesc)
  : ModuleInterface("Simulation.glade", "ViewportSimulation", "", "", ""),
    m_RunDesc(RunDesc)
{
  m_ModuleName = _("_Simulation");
  m_ModuleLongName = _("Simulation");
  mp_StockId = (Gtk::StockID *)&Gtk::Stock::PROPERTIES;

  mp_Builder->get_widget("SpinDeltat",mp_SpinDeltat);
  mp_Builder->get_widget("EntryBegin",mp_EntryBegin);
  mp_Builder->get_widget("EntryEnd",mp_EntryEnd);
  mp_Builder->get_widget("SpinBufferValues",mp_SpinBufferValues);
  mp_Builder->get_widget("SpinBufferFiles",mp_SpinBufferFiles);

  initRunPanel();

}


// =====================================================================
// =====================================================================


SimulationModule::~SimulationModule()
{

}


// =====================================================================
// =====================================================================


void SimulationModule::initRunPanel()
{
  mp_SpinDeltat->set_value(m_RunDesc.getDeltaT());
  mp_EntryBegin->set_text(m_RunDesc.getBeginDate().getAsISOString());
  mp_EntryEnd->set_text(m_RunDesc.getEndDate().getAsISOString());
  mp_SpinBufferValues->set_value(m_RunDesc.getValuesBufferSize());
  mp_SpinBufferFiles->set_value(m_RunDesc.getFilesBufferSizeInKB());

  mp_SpinDeltat->signal_value_changed().connect(
      sigc::mem_fun(*this,&SimulationModule::onSpinDeltatValueChanged));

  mp_EntryBegin->signal_changed().connect(sigc::bind<Gtk::Entry*>(
      sigc::mem_fun(*this,&SimulationModule::onEntryPeriodChanged),
      mp_EntryBegin));

  mp_EntryEnd->signal_changed().connect(sigc::bind<Gtk::Entry*>(
      sigc::mem_fun(*this,&SimulationModule::onEntryPeriodChanged),
      mp_EntryEnd));

  mp_SpinBufferValues->signal_value_changed().connect(
      sigc::mem_fun(*this,&SimulationModule::onSpinBufferValuesValueChanged));

  mp_SpinBufferFiles->signal_value_changed().connect(
      sigc::mem_fun(*this,&SimulationModule::onSpinBufferFilesValueChanged));

}



// =====================================================================
// =====================================================================


void SimulationModule::onSpinDeltatValueChanged()
{
  m_RunDesc.setDeltaT(mp_SpinDeltat->get_value());
}


// =====================================================================
// =====================================================================


void SimulationModule::onSpinBufferValuesValueChanged()
{
  m_RunDesc.setValuesBufferSize(mp_SpinBufferValues->get_value());
}


// =====================================================================
// =====================================================================


void SimulationModule::onSpinBufferFilesValueChanged()
{
  m_RunDesc.setFilesBufferSizeInKB(mp_SpinBufferFiles->get_value());
}


// =====================================================================
// =====================================================================


void SimulationModule::onEntryPeriodChanged(Gtk::Entry * Entry)
{
  openfluid::core::DateTime * DT = new openfluid::core::DateTime();

  DT->setFromISOString(Entry->get_text());

  if(openfluid::core::DateTime::isValidDateTime(DT->getYear(),DT->getMonth(),DT->getDay(),DT->getHour(),DT->getMinute(),DT->getSecond()))
  {
    if(Entry == mp_EntryBegin)
      m_RunDesc.setBeginDate(*DT);
    else if(Entry == mp_EntryEnd)
      m_RunDesc.setEndDate(*DT);

    Entry->modify_base(Gtk::STATE_NORMAL,Gdk::Color("white"));
  }
  else
  {
    Entry->modify_base(Gtk::STATE_NORMAL,Gdk::Color("red"));
  }

  delete DT;
}


// =====================================================================
// =====================================================================


