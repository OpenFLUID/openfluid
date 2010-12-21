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
  \file ModelParam_V.cpp
  \brief Implements ...

  \author Aline LIBRES <libres@supagro.inra.fr>
 */


#include <iostream>

#include <glibmm/i18n.h>

#include "ModelParam.hpp"
#include "ModelParam_V.hpp"


// =====================================================================
// =====================================================================


ModelParam_V::ModelParam_V(ModelParam * Control)
: mp_Control(Control)
{
  mp_Check = Gtk::manage(new Gtk::CheckButton());

  mp_Label = Gtk::manage(new Gtk::Label("",Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));

  mp_Entry = Gtk::manage(new Gtk::Entry());

  mp_Unit = Gtk::manage(new Gtk::Label("",Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));

  mp_LabelGlobal = Gtk::manage(new Gtk::Label("",Gtk::ALIGN_LEFT,Gtk::ALIGN_CENTER));


  mp_Check->signal_toggled().connect(
      sigc::mem_fun(*this,&ModelParam_V::onCheckToggled));

  mp_Entry->signal_focus_out_event().connect(
      sigc::mem_fun(*this,&ModelParam_V::onEntryFocusOut));
  mp_Entry->signal_activate().connect(
      sigc::mem_fun(*this,&ModelParam_V::onEntryActivate));


  mp_Check->set_visible(true);

  mp_Entry->set_visible(true);

  mp_Label->set_visible(true);

  mp_Unit->set_visible(true);

  mp_LabelGlobal->set_visible(true);


  m_WidgetRow.push_back(mp_Check);
  m_WidgetRow.push_back(mp_Label);
  m_WidgetRow.push_back(mp_Entry);
  m_WidgetRow.push_back(mp_Unit);
  m_WidgetRow.push_back(mp_LabelGlobal);
}


// =====================================================================
// =====================================================================


std::vector<Gtk::Widget *> ModelParam_V::asWidgetVector()
{
  return m_WidgetRow;
}


// =====================================================================
// =====================================================================


void ModelParam_V::setLabel(Glib::ustring Label)
{
  mp_Label->set_text(Label);
}


// =====================================================================
// =====================================================================


void ModelParam_V::setUnit(Glib::ustring Unit)
{
  mp_Unit->set_text(Unit);
}


// =====================================================================
// =====================================================================


void ModelParam_V::setLocalValue(std::string Value)
{
  mp_Entry->set_text(Value);
}


// =====================================================================
// =====================================================================


void ModelParam_V::setGlobalValue(std::string GlobalValue)
{
  mp_LabelGlobal->set_text(GlobalValue);
}


// =====================================================================
// =====================================================================


void ModelParam_V::setLocalValueUsed(bool Used)
{
  mp_Check->set_active(Used);

  mp_Entry->set_sensitive(Used);

  mp_LabelGlobal->set_sensitive(!Used);
}


// =====================================================================
// =====================================================================


void ModelParam_V::setGlobalValueActivated(bool Activated)
{
  mp_LabelGlobal->set_visible(Activated);
}


// =====================================================================
// =====================================================================


void ModelParam_V::onCheckToggled()
{
  mp_Control->V_localValueUsedChanged(mp_Check->get_active());
}


// =====================================================================
// =====================================================================


bool ModelParam_V::onEntryFocusOut(GdkEventFocus * /*Event*/)
{
  onEntryActivate();

  return true;
}


// =====================================================================
// =====================================================================


void ModelParam_V::onEntryActivate()
{
  mp_Control->V_localValueChanged(mp_Entry->get_text());
}
