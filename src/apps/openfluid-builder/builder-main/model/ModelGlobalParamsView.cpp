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
 \file ModelGlobalParamsView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelGlobalParamsView.hpp"

#include <glibmm/i18n.h>

#include "ModelGlobalParamRow.hpp"

// =====================================================================
// =====================================================================


ModelGlobalParamsViewImpl::ModelGlobalParamsViewImpl() :
  m_CurrentTableBottom(0)
{
  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->set_border_width(5);

  Gtk::Label* ParamsLabel = Gtk::manage(new Gtk::Label(
      _("Available parameters:")));

  mp_Combo = Gtk::manage(new Gtk::ComboBoxText());

  mp_AddButton = Gtk::manage(new Gtk::Button(_("Set global")));
  mp_AddButton->set_tooltip_text(_("Unset this parameter as global"));
  mp_AddButton->signal_clicked().connect(sigc::mem_fun(*this,
      &ModelGlobalParamsViewImpl::onAddButtonClicked));

  Gtk::HBox* TopBox = Gtk::manage(new Gtk::HBox());
  TopBox->pack_start(*ParamsLabel, Gtk::PACK_SHRINK, 0);
  TopBox->pack_start(*mp_Combo, Gtk::PACK_SHRINK, 10);
  TopBox->pack_start(*mp_AddButton, Gtk::PACK_SHRINK, 0);
  TopBox->set_visible(true);
  TopBox->show_all_children();

  Gtk::HSeparator* HSep = Gtk::manage(new Gtk::HSeparator());
  HSep->set_visible(true);

  mp_Table = Gtk::manage(new Gtk::Table());
  mp_Table->set_col_spacings(10);
  mp_Table->set_visible(true);

  mp_MainBox->pack_start(*TopBox, Gtk::PACK_SHRINK);
  mp_MainBox->pack_start(*HSep, Gtk::PACK_SHRINK, 15);
  mp_MainBox->pack_start(*mp_Table);
  mp_MainBox->set_visible(true);
}

// =====================================================================
// =====================================================================


void ModelGlobalParamsViewImpl::setComboParams(std::set<std::string> Params)
{
  mp_Combo->clear();

  for (std::set<std::string>::iterator it = Params.begin(); it != Params.end(); ++it)
    mp_Combo->append_text(*it);

  mp_Combo->set_active(0);

  mp_AddButton->set_sensitive(!Params.empty());
}

// =====================================================================
// =====================================================================

void ModelGlobalParamsViewImpl::removeGlobalParamsRow(std::string ParamName)
{
  if (m_ByParamNameParamRow.find(ParamName) != m_ByParamNameParamRow.end())
  {
    for (unsigned int j = 0; j
        < m_ByParamNameParamRow[ParamName]->getWidgetCount(); j++)
    {
      mp_Table->remove(*m_ByParamNameParamRow[ParamName]->getWidgets()[j]);
    }
    /* no, apparently Gtk::Table doesn't update attachment indexes
     when a widget is removed from container directly */
    //    m_CurrentTableBottom--;

    m_ByParamNameParamRow.erase(ParamName);
  }
}

// =====================================================================
// =====================================================================


void ModelGlobalParamsViewImpl::removeGlobalParamsRows(
    std::vector<std::string> ParamNames)
{
  for (unsigned int i = 0; i < ParamNames.size(); i++)
  {
    removeGlobalParamsRow(ParamNames[i]);
  }
}

// =====================================================================
// ==================================================================


void ModelGlobalParamsViewImpl::addGlobalParamsRow(std::string ParamName,
    std::string ParamUnit)
{
  if (ParamName == "")
    return;

  ModelGlobalParamRow* Row = new ModelGlobalParamRow(ParamName, ParamUnit);

  for (unsigned int i = 0; i < Row->getColumnCount(); i++)
  {
    mp_Table->attach(*Row->getWidgets()[i], i, i + 1, m_CurrentTableBottom,
        m_CurrentTableBottom + 1, Gtk::FILL, Gtk::FILL, 0, 0);
  }
  m_CurrentTableBottom++;

  Row->signal_removeAsked().connect(sigc::mem_fun(*this,
      &ModelGlobalParamsViewImpl::onRowRemoveAsked));
  Row->signal_valueChanged().connect(sigc::mem_fun(*this,
      &ModelGlobalParamsViewImpl::onGlobalValueChanged));

  m_ByParamNameParamRow[ParamName] = Row;
}

// =====================================================================
// =====================================================================

void ModelGlobalParamsViewImpl::onRowRemoveAsked(std::string ParamName)
{
  m_signal_GlobalParamUnsetAsked.emit(ParamName);
}

// =====================================================================
// =====================================================================


void ModelGlobalParamsViewImpl::onGlobalValueChanged(std::string ParamName)
{
  m_signal_GlobalValueChanged.emit(ParamName);
}

// =====================================================================
// =====================================================================


void ModelGlobalParamsViewImpl::onAddButtonClicked()
{
  m_signal_GlobalParamSetAsked.emit(mp_Combo->get_active_text());
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string> ModelGlobalParamsViewImpl::signal_GlobalValueChanged()
{
  return m_signal_GlobalValueChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string> ModelGlobalParamsViewImpl::signal_GlobalParamSetAsked()
{
  return m_signal_GlobalParamSetAsked;
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string> ModelGlobalParamsViewImpl::signal_GlobalParamUnsetAsked()
{
  return m_signal_GlobalParamUnsetAsked;
}

// =====================================================================
// =====================================================================


std::string ModelGlobalParamsViewImpl::getGlobalValue(std::string ParamName)
{
  if (m_ByParamNameParamRow.find(ParamName) != m_ByParamNameParamRow.end())
    return m_ByParamNameParamRow[ParamName]->getValue();

  return "";
}

// =====================================================================
// =====================================================================


Gtk::Widget* ModelGlobalParamsViewImpl::asWidget()
{
  return mp_MainBox;
}

