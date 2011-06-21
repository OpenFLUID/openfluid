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
 \file ModelGlobalParamRow.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelGlobalParamRow.hpp"

#include <glibmm/i18n.h>

// =====================================================================
// =====================================================================


ModelGlobalParamRow::ModelGlobalParamRow(std::string ParamName,
    std::string ParamUnit) :
  m_ParamName(ParamName)
{
  m_ColumnCount = 4;

  Gtk::Label* ParamNameLabel = Gtk::manage(new Gtk::Label(ParamName,
      Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  ParamNameLabel->set_visible(true);
  m_RowWidgets.push_back(ParamNameLabel);

  mp_ParamValueEntry = Gtk::manage(new Gtk::Entry());
  mp_ParamValueEntry->signal_changed().connect(sigc::mem_fun(*this,
      &ModelGlobalParamRow::onEntryChanged));
  mp_ParamValueEntry->set_visible(true);
  m_RowWidgets.push_back(mp_ParamValueEntry);

  Gtk::Label* ParamUnitLabel = Gtk::manage(new Gtk::Label(ParamUnit,
      Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  ParamUnitLabel->set_visible(true);
  m_RowWidgets.push_back(ParamUnitLabel);

  mp_RemoveButton = Gtk::manage(new Gtk::Button());
  mp_RemoveButton->set_image(*Gtk::manage(new Gtk::Image(Gtk::Stock::DELETE,
      Gtk::ICON_SIZE_BUTTON)));
  mp_RemoveButton->set_tooltip_text(_("Unset this parameter as global"));
  mp_RemoveButton->signal_clicked().connect(sigc::mem_fun(*this,
      &ModelGlobalParamRow::onRemoveButtonClicked));
  mp_RemoveButton->set_visible(true);
  m_RowWidgets.push_back(mp_RemoveButton);
}

// =====================================================================
// =====================================================================


std::string ModelGlobalParamRow::getValue()
{
  return mp_ParamValueEntry->get_text();
}

// =====================================================================
// =====================================================================


void ModelGlobalParamRow::setValue(std::string Value)
{
  mp_ParamValueEntry->set_text(Value);
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string> ModelGlobalParamRow::signal_valueChanged()
{
  return m_signal_valueChanged;
}

// =====================================================================
// =====================================================================


void ModelGlobalParamRow::onEntryChanged()
{
  m_signal_valueChanged.emit(m_ParamName);
}

// =====================================================================
// =====================================================================


void ModelGlobalParamRow::onRemoveButtonClicked()
{
  m_signal_removeAsked.emit(m_ParamName);
}

// =====================================================================
// =====================================================================

sigc::signal<void, std::string> ModelGlobalParamRow::signal_removeAsked()
{
  return m_signal_removeAsked;
}
