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
 \file WareSetWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "WareSetWidget.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/frame.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/notebook.h>

// =====================================================================
// =====================================================================

WareSetWidget::WareSetWidget(std::string AddText)
{
  Gtk::HBox* BtBox = Gtk::manage(new Gtk::HBox());
  BtBox->pack_start(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::ADD, Gtk::ICON_SIZE_BUTTON)),
      Gtk::PACK_SHRINK, 10);
  Gtk::Label* AddTextLabel = Gtk::manage(new Gtk::Label());
  AddTextLabel->set_markup(Glib::ustring::compose("<b>%1</b>", AddText));
  BtBox->pack_start(*AddTextLabel, Gtk::PACK_SHRINK);
  mp_AddButton = Gtk::manage(new Gtk::Button());
  mp_AddButton->add(*BtBox);
  mp_AddButton->signal_clicked().connect(
      sigc::mem_fun(*this, &WareSetWidget::onAddButtonClicked));

  mp_ExpandAllButton = Gtk::manage(new Gtk::Button(_("Expand all")));
  mp_ExpandAllButton->signal_clicked().connect(
      sigc::mem_fun(*this, &WareSetWidget::onExpandAllButtonClicked));

  mp_CollapseAllButton = Gtk::manage(new Gtk::Button(_("Collapse all")));
  mp_CollapseAllButton->signal_clicked().connect(
      sigc::mem_fun(*this, &WareSetWidget::onCollapseAllButtonClicked));

  Gtk::HBox* TopBox = Gtk::manage(new Gtk::HBox());
  TopBox->pack_start(*mp_AddButton, Gtk::PACK_SHRINK);
  TopBox->pack_end(*mp_CollapseAllButton, Gtk::PACK_SHRINK);
  TopBox->pack_end(*mp_ExpandAllButton, Gtk::PACK_SHRINK);

  mp_ListBox = Gtk::manage(new Gtk::VBox());

  Gtk::ScrolledWindow* ModelWin = Gtk::manage(new Gtk::ScrolledWindow());
  ModelWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  ModelWin->add(*mp_ListBox);
  ModelWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  pack_start(*TopBox, Gtk::PACK_SHRINK, 5);
  pack_start(*ModelWin, Gtk::PACK_EXPAND_WIDGET, 0);

  set_visible(true);
  show_all_children();
}

// =====================================================================
// =====================================================================

WareSetWidget::~WareSetWidget()
{

}

// =====================================================================
// =====================================================================

void WareSetWidget::clearItems()
{
  m_ItemWidgets.clear();

  int ItemsCount = mp_ListBox->children().size();

  for (int i = 0; i < ItemsCount; i++)
    mp_ListBox->remove(*mp_ListBox->children().begin()->get_widget());
}

// =====================================================================
// =====================================================================

void WareSetWidget::addItem(WareItemWidget* Item, std::string ID)
{
  mp_ListBox->pack_start(*Item, Gtk::PACK_SHRINK, 0, 1);

  m_ItemWidgets[ID] = Item;
}

// =====================================================================
// =====================================================================

void WareSetWidget::onAddButtonClicked()
{
  m_signal_AddAsked.emit();
}

// =====================================================================
// =====================================================================

void WareSetWidget::onExpandAllButtonClicked()
{
  for (std::map<std::string, WareItemWidget*>::iterator it =
      m_ItemWidgets.begin(); it != m_ItemWidgets.end(); ++it)
    it->second->setExpanded(true);
}

// =====================================================================
// =====================================================================

void WareSetWidget::onCollapseAllButtonClicked()
{
  for (std::map<std::string, WareItemWidget*>::iterator it =
      m_ItemWidgets.begin(); it != m_ItemWidgets.end(); ++it)
    it->second->setExpanded(false);
}

// =====================================================================
// =====================================================================

void WareSetWidget::storeExpanderStates()
{
  m_ExpanderStates.clear();

  for (std::map<std::string, WareItemWidget*>::iterator it =
      m_ItemWidgets.begin(); it != m_ItemWidgets.end(); ++it)
    m_ExpanderStates[it->first] = it->second->getExpanded();
}

// =====================================================================
// =====================================================================

void WareSetWidget::applyExpanderStates()
{
  for (std::map<std::string, WareItemWidget*>::iterator it =
      m_ItemWidgets.begin(); it != m_ItemWidgets.end(); ++it)
    it->second->setExpanded(m_ExpanderStates[it->first]);
}

// =====================================================================
// =====================================================================

void WareSetWidget::storeNotebookPages()
{
  m_NotebookPages.clear();

  for (std::map<std::string, WareItemWidget*>::iterator it =
      m_ItemWidgets.begin(); it != m_ItemWidgets.end(); ++it)
  {
    Gtk::Notebook* NB = it->second->getItemInfoWidgetAsNotebook();

    if (NB)
      m_NotebookPages[it->first] = NB->get_current_page();
  }
}

// =====================================================================
// =====================================================================

void WareSetWidget::applyNotebookPages()
{
  for (std::map<std::string, WareItemWidget*>::iterator it =
      m_ItemWidgets.begin(); it != m_ItemWidgets.end(); ++it)
  {
    Gtk::Notebook* NB = it->second->getItemInfoWidgetAsNotebook();

    if (NB)
      NB->set_current_page(m_NotebookPages[it->first]);
  }
}

// =====================================================================
// =====================================================================

sigc::signal<void> WareSetWidget::signal_AddAsked()
{
  return m_signal_AddAsked;
}

// =====================================================================
// =====================================================================
