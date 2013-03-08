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
 \file MonitoringEditParamsDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "MonitoringEditParamsDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/separator.h>
#include <openfluid/fluidx/ObserverDescriptor.hpp>
#include <openfluid/machine/ObserverInstance.hpp>

// =====================================================================
// =====================================================================

MonitoringParamRow::MonitoringParamRow(std::string ParamName,
                                       std::string ParamValue, const int Index) :
    m_Index(Index)
{
  m_ColumnCount = 3;

  mp_ParamNameEntry = Gtk::manage(new Gtk::Entry());
  mp_ParamNameEntry->set_text(ParamName);
  mp_ParamNameEntry->set_visible(true);
  m_RowWidgets.push_back(mp_ParamNameEntry);

  mp_ParamValueEntry = Gtk::manage(new Gtk::Entry());
  mp_ParamValueEntry->set_text(ParamValue);
  mp_ParamValueEntry->set_visible(true);
  m_RowWidgets.push_back(mp_ParamValueEntry);

  mp_RemoveButton = Gtk::manage(new Gtk::Button());
  mp_RemoveButton->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::DELETE, Gtk::ICON_SIZE_BUTTON)));
  mp_RemoveButton->set_tooltip_text(_("Remove this parameter"));
  mp_RemoveButton->signal_clicked().connect(
      sigc::mem_fun(*this, &MonitoringParamRow::onRemoveButtonClicked));
  mp_RemoveButton->set_visible(true);
  m_RowWidgets.push_back(mp_RemoveButton);
}

// =====================================================================
// =====================================================================

std::string MonitoringParamRow::getName()
{
  return mp_ParamNameEntry->get_text();
}

// =====================================================================
// =====================================================================

std::string MonitoringParamRow::getValue()
{
  return mp_ParamValueEntry->get_text();
}

// =====================================================================
// =====================================================================

void MonitoringParamRow::onRemoveButtonClicked()
{
  m_signal_removeAsked.emit(m_Index);
}

// =====================================================================
// =====================================================================

sigc::signal<void, int> MonitoringParamRow::signal_removeAsked()
{
  return m_signal_removeAsked;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

MonitoringEditParamsDialog::MonitoringEditParamsDialog(
    openfluid::guicommon::BuilderMonitoring& Monit) :
    m_Monit(Monit)
{
  Gtk::Button* AddButton = Gtk::manage(new Gtk::Button());
  AddButton->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::ADD, Gtk::ICON_SIZE_BUTTON)));
  AddButton->set_tooltip_text(_("Add a parameter"));
  AddButton->signal_clicked().connect(
      sigc::mem_fun(*this, &MonitoringEditParamsDialog::onAddButtonClicked));

  Gtk::HBox* ButtonBox = Gtk::manage(new Gtk::HBox());
  ButtonBox->pack_start(*AddButton, Gtk::PACK_SHRINK, 0);

  Gtk::HSeparator* HSep = Gtk::manage(new Gtk::HSeparator());
  HSep->set_visible(true);

  mp_Table = Gtk::manage(new Gtk::Table(1, 3));
  mp_Table->set_col_spacings(10);
  mp_Table->set_border_width(5);
  mp_Table->set_visible(true);

  Gtk::ScrolledWindow* BottomWin = Gtk::manage(new Gtk::ScrolledWindow());
  BottomWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  BottomWin->set_visible(true);
  BottomWin->add(*mp_Table);

  mp_IdLabel = Gtk::manage(new Gtk::Label(""));
  mp_DescLabel = Gtk::manage(new Gtk::Label(""));

  mp_Dialog = new Gtk::Dialog(_("Edit observer parameters"));
  mp_Dialog->get_vbox()->pack_start(*mp_IdLabel, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*mp_DescLabel, Gtk::PACK_SHRINK, 10);
  mp_Dialog->get_vbox()->pack_start(*HSep, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*ButtonBox, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*BottomWin);
  mp_Dialog->set_default_size(600, 400);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================

MonitoringEditParamsDialog::~MonitoringEditParamsDialog()
{

}

// =====================================================================
// =====================================================================

bool MonitoringEditParamsDialog::show(std::string ObserverID)
{
  init(ObserverID);

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    mp_Dialog->hide();
    return true;
  }

  mp_Dialog->hide();
  return false;
}

// =====================================================================
// =====================================================================

void MonitoringEditParamsDialog::init(std::string ObserverID)
{
  mp_IdLabel->set_text(ObserverID);
  mp_DescLabel->set_text(
      m_Monit.getSignature(ObserverID).Signature->Description);

  m_ParamsByIndex.clear();

  std::map<std::string, std::string> Params =
      openfluid::fluidx::WareDescriptor::getParamsAsMap(
          m_Monit.getDescriptor(ObserverID).getParameters());

  int Index = 0;
  for (std::map<std::string, std::string>::iterator it = Params.begin();
      it != Params.end(); ++it, Index++)
    m_ParamsByIndex[Index] = *it;

  updateRows();
}

// =====================================================================
// =====================================================================

void MonitoringEditParamsDialog::updateRows()
{
  int TableWidgetCount = mp_Table->children().size();

  for (int i = 0; i < TableWidgetCount; i++)
    mp_Table->remove(*mp_Table->children().begin()->get_widget());

  m_ParamRowsByIndex.clear();
  m_CurrentTableBottom = 1;

  Gtk::Label* Lab1 = Gtk::manage(new Gtk::Label("Parameter Name"));
  Lab1->set_visible(true);
  Gtk::Label* Lab2 = Gtk::manage(new Gtk::Label("Parameter Value"));
  Lab2->set_visible(true);
  mp_Table->attach(*Lab1, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0);
  mp_Table->attach(*Lab2, 1, 2, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0);

  for (std::map<int, std::pair<std::string, std::string> >::iterator it =
      m_ParamsByIndex.begin(); it != m_ParamsByIndex.end(); ++it)
  {
    MonitoringParamRow* Row = new MonitoringParamRow(it->second.first,
                                                     it->second.second,
                                                     it->first);

    for (unsigned int i = 0; i < Row->getColumnCount(); i++)
    {
      mp_Table->attach(*Row->getWidgets()[i], i, i + 1, m_CurrentTableBottom,
                       m_CurrentTableBottom + 1, Gtk::FILL, Gtk::FILL, 0, 0);
    }

    Row->signal_removeAsked().connect(
        sigc::mem_fun(*this, &MonitoringEditParamsDialog::onRemoveAsked));

    m_ParamRowsByIndex[it->first] = Row;

    m_CurrentTableBottom++;
  }

}

// =====================================================================
// =====================================================================

void MonitoringEditParamsDialog::onAddButtonClicked()
{
  int Index = m_ParamRowsByIndex.size();

  MonitoringParamRow* Row = new MonitoringParamRow("", "", Index);

  for (unsigned int i = 0; i < Row->getColumnCount(); i++)
  {
    mp_Table->attach(*Row->getWidgets()[i], i, i + 1, m_CurrentTableBottom,
                     m_CurrentTableBottom + 1, Gtk::FILL, Gtk::FILL, 0, 0);
  }

  Row->signal_removeAsked().connect(
      sigc::mem_fun(*this, &MonitoringEditParamsDialog::onRemoveAsked));

  m_ParamRowsByIndex[Index] = Row;

  m_CurrentTableBottom++;
}

// =====================================================================
// =====================================================================

void MonitoringEditParamsDialog::onRemoveAsked(int Index)
{
  m_ParamsByIndex.clear();

  for (std::map<int, MonitoringParamRow*>::iterator it =
      m_ParamRowsByIndex.begin(); it != m_ParamRowsByIndex.end(); ++it)
  {
    int RowIndex = it->first;
    if (RowIndex < Index)
      m_ParamsByIndex[RowIndex] = std::make_pair(it->second->getName(),
                                                 it->second->getValue());
    //ignoreRowIndex == Index
    else if (RowIndex > Index)
      m_ParamsByIndex[RowIndex - 1] = std::make_pair(it->second->getName(),
                                                     it->second->getValue());
  }

  updateRows();
}

// =====================================================================
// =====================================================================

openfluid::ware::WareParams_t MonitoringEditParamsDialog::getParams()
{
  openfluid::ware::WareParams_t Params;

  for (std::map<int, MonitoringParamRow*>::iterator it =
      m_ParamRowsByIndex.begin(); it != m_ParamRowsByIndex.end(); ++it)
  {
    std::string Name = it->second->getName();

    if (!Name.empty())
      Params.put(Name, it->second->getValue());
  }

  return Params;
}

// =====================================================================
// =====================================================================

