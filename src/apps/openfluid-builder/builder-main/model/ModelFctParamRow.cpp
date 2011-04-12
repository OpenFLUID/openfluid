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
 \file ModelFctParamRow.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelFctParamRow.hpp"

// =====================================================================
// =====================================================================


ModelFctParamRow::ModelFctParamRow(std::string ParamName, std::string ParamUnit)
{
  m_ColumnCount = 5;

  mp_GlobalCheck = Gtk::manage(new Gtk::CheckButton());
  mp_GlobalCheck->set_visible(true);
  mp_GlobalCheck->signal_activate().connect(sigc::mem_fun(*this,
      &ModelFctParamRow::onGlobalCheckActivated));
  m_RowWidgets.push_back(mp_GlobalCheck);

  mp_ParamNameLabel = Gtk::manage(new Gtk::Label(ParamName, Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER));
  mp_ParamNameLabel->set_visible(true);
  m_RowWidgets.push_back(mp_ParamNameLabel);

  mp_ParamValueEntry = Gtk::manage(new Gtk::Entry());
  mp_ParamValueEntry->set_visible(true);
  mp_ParamValueEntry->signal_changed().connect(sigc::mem_fun(*this,
      &ModelFctParamRow::onParamValueChanged));
  m_RowWidgets.push_back(mp_ParamValueEntry);

  mp_ParamUnitLabel = Gtk::manage(new Gtk::Label(ParamUnit, Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER));
  mp_ParamUnitLabel->set_visible(true);
  m_RowWidgets.push_back(mp_ParamUnitLabel);

  mp_GlobalValueLabel = Gtk::manage(new Gtk::Label("", Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER));
  mp_GlobalValueLabel->set_visible(true);
  m_RowWidgets.push_back(mp_GlobalValueLabel);

}

// =====================================================================
// =====================================================================


sigc::signal<void> ModelFctParamRow::signal_GobalDefinedAsked()
{
  return m_signal_GobalDefinedAsked;
}

// =====================================================================
// =====================================================================


sigc::signal<void> ModelFctParamRow::signal_ValueChanged()
{
  return m_signal_ValueChanged;
}

// =====================================================================
// =====================================================================


void ModelFctParamRow::onGlobalCheckActivated()
{
  if (mp_GlobalCheck->activate())
    m_signal_GobalDefinedAsked.emit();
  else
    m_signal_ValueChanged.emit();
}

// =====================================================================
// =====================================================================


void ModelFctParamRow::onParamValueChanged()
{
  m_signal_ValueChanged.emit();
}


// =====================================================================
// =====================================================================


void ModelFctParamRow::setValue(std::string Value)
{
  mp_ParamValueEntry->set_text(Value);
}


// =====================================================================
// =====================================================================


std::string ModelFctParamRow::getValue()
{
  return mp_ParamValueEntry->get_text();
}


// =====================================================================
// =====================================================================


std::string ModelFctParamRow::getParamName()
{
  return mp_ParamNameLabel->get_text();
}

