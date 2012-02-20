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
 \file ToolBar.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "ToolBar.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>

#include "BuilderGraphicsHelper.hpp"


ToolBar::ToolBar() :
  m_CurrentZoomInMode(ZOOM_IN_CURSOR)
{
  // Add

  mp_AddLayerToolButton = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::ADD));
  mp_AddLayerToolButton->set_tooltip_markup(_("Add a new layer"));

  // Select

  Gtk::ToolButton* SelectModeToolButton = Gtk::manage(new Gtk::ToolButton(
      *BuilderGraphicsHelper::createBuilderIconStockId("left_ptr.png",
          "openfluid_mapview_select")));
  SelectModeToolButton->set_tooltip_markup(_("Selection mode"));

  // Move

  mp_MoveModeToggleToolButton = Gtk::manage(new Gtk::ToggleToolButton(
      *BuilderGraphicsHelper::createBuilderIconStockId("hand1.png",
          "openfluid_mapview_move")));
  mp_MoveModeToggleToolButton->set_tooltip_markup(_("Move mode"));

  // Zoom 100

  mref_Zoom100AllMenuAction = Gtk::Action::create("Zoom100All",
      Gtk::Stock::ZOOM_100, _("All"), _("Zoom at 100% of all layers"));
  mref_Zoom100AllMenuAction->set_is_important(true);
  mref_Zoom100AllMenuAction->signal_activate().connect(sigc::mem_fun(*this,
      &ToolBar::onZoom100AllMenuActionActivate));

  mref_Zoom100LayerMenuAction
      = Gtk::Action::create("Zoom100Layer", Gtk::Stock::ZOOM_100, _("Layer"),
          _("Zoom at 100% of the selected layer"));
  mref_Zoom100LayerMenuAction->set_is_important(true);
  mref_Zoom100LayerMenuAction->signal_activate().connect(sigc::mem_fun(*this,
      &ToolBar::onZoom100LayerMenuActionActivate));

  mref_Zoom100SelectionMenuAction = Gtk::Action::create("Zoom100Selection",
      Gtk::Stock::ZOOM_100, _("Selection"),
      _("Zoom at 100% of the selected units"));
  mref_Zoom100SelectionMenuAction->set_is_important(true);
  mref_Zoom100SelectionMenuAction->signal_activate().connect(sigc::mem_fun(
      *this, &ToolBar::onZoom100SelectionMenuActionActivate));

  Gtk::MenuItem* Zoom100AllMenuItem =
      mref_Zoom100AllMenuAction->create_menu_item();
  Zoom100AllMenuItem->set_tooltip_text(mref_Zoom100AllMenuAction->get_tooltip());

  Gtk::MenuItem* Zoom100LayerMenuItem =
      mref_Zoom100LayerMenuAction->create_menu_item();
  Zoom100LayerMenuItem->set_tooltip_text(
      mref_Zoom100LayerMenuAction->get_tooltip());

  Gtk::MenuItem* Zoom100SelectionMenuItem =
      mref_Zoom100SelectionMenuAction->create_menu_item();
  Zoom100SelectionMenuItem->set_tooltip_text(
      mref_Zoom100SelectionMenuAction->get_tooltip());

  Gtk::Menu* Zoom100Menu = Gtk::manage(new Gtk::Menu());
  Zoom100Menu->append(*Zoom100AllMenuItem);
  Zoom100Menu->append(*Zoom100LayerMenuItem);
  Zoom100Menu->append(*Zoom100SelectionMenuItem);

  mp_Zoom100MenuToolButton = Gtk::manage(new Gtk::MenuToolButton(
      Gtk::Stock::ZOOM_100));
  mp_Zoom100MenuToolButton->set_menu(*Zoom100Menu);
  mp_Zoom100MenuToolButton->set_arrow_tooltip_text(_("Change the 100% zoom"));

  // Zoom in

  mp_ZoomInToggleToolButton = Gtk::manage(new Gtk::ToggleToolButton(
      Gtk::Stock::ZOOM_IN));
  mp_ZoomInToggleToolButton->set_tooltip_text(_("Zoom in mode"));

  Glib::RefPtr<Gtk::Action> ZoomInCursorMenuAction = Gtk::Action::create(
      "ZoomCursor", Gtk::Stock::ZOOM_IN, _("Zoom Cursor"),
      _("Zoom in at the cursor position"));
  ZoomInCursorMenuAction->signal_activate().connect(sigc::mem_fun(*this,
      &ToolBar::onZoomInCursorMenuActionActivate));

  Glib::RefPtr<Gtk::Action> ZoomInFrameMenuAction = Gtk::Action::create(
      "ZoomFrame", Gtk::Stock::ZOOM_FIT, _("Zoom Frame"), _("Zoom in a frame"));
  ZoomInFrameMenuAction->signal_activate().connect(sigc::mem_fun(*this,
      &ToolBar::onZoomInFrameMenuActionActivate));

  Gtk::MenuItem* ZoomInCursorMenuItem =
      ZoomInCursorMenuAction->create_menu_item();
  ZoomInCursorMenuItem->set_tooltip_text(ZoomInCursorMenuAction->get_tooltip());

  Gtk::MenuItem* ZoomInFrameMenuItem =
      ZoomInFrameMenuAction->create_menu_item();
  ZoomInFrameMenuItem->set_tooltip_text(ZoomInFrameMenuAction->get_tooltip());

  mp_ZoomInMenu = Gtk::manage(new Gtk::Menu());
  mp_ZoomInMenu->append(*ZoomInCursorMenuItem);
  mp_ZoomInMenu->append(*ZoomInFrameMenuItem);

  Gtk::MenuToolButton* mp_ZoomInMenuToolButton = Gtk::manage(
      new Gtk::MenuToolButton());
  Gtk::HBox* TempHbox2 =
      static_cast<Gtk::HBox*> (mp_ZoomInMenuToolButton->get_child());
  TempHbox2->remove(**(TempHbox2->get_children().begin()));
  mp_ZoomInMenuToolButton->set_menu(*mp_ZoomInMenu);
  mp_ZoomInMenuToolButton->set_arrow_tooltip_text(_("Change the zoom in"));

  // Zoom out

  mp_ZoomOutToggleToolButton = Gtk::manage(new Gtk::ToggleToolButton(
      Gtk::Stock::ZOOM_OUT));
  mp_ZoomOutToggleToolButton->set_tooltip_markup(_("Zoom out mode"));

  // Info

  mp_InfoToolButton = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::INFO));
  mp_InfoToolButton->set_tooltip_markup(
      _("Get information on the selected units"));

  // Tools

  Glib::RefPtr<Gtk::Action> ToolsSelectAllAction = Gtk::Action::create(
      "SelectAll", Gtk::Stock::REFRESH, _("Select all units"),
      _("Select all units of the selected layer"));
  ToolsSelectAllAction->signal_activate().connect(sigc::mem_fun(*this,
      &ToolBar::onToolsSelectAllActivate));

  Glib::RefPtr<Gtk::Action> ToolsToggleSelectionAction = Gtk::Action::create(
      "ToggleSelected", Gtk::Stock::SELECT_ALL, _("Toggle selection"),
      _("Toggle selection"));
  ToolsToggleSelectionAction->signal_activate().connect(sigc::mem_fun(*this,
      &ToolBar::onToolsToggleSelectionActivate));

  Gtk::MenuItem* ToolsSelectAllMenuItem =
      ToolsSelectAllAction->create_menu_item();
  ToolsSelectAllMenuItem->set_tooltip_text(ToolsSelectAllAction->get_tooltip());

  Gtk::MenuItem* ToolsToggleSelectionMenuItem =
      ToolsToggleSelectionAction->create_menu_item();
  ToolsToggleSelectionMenuItem->set_tooltip_text(
      ToolsToggleSelectionAction->get_tooltip());

  mp_ToolsMenu = Gtk::manage(new Gtk::Menu());
  mp_ToolsMenu->append(*ToolsSelectAllMenuItem);
  mp_ToolsMenu->append(*ToolsToggleSelectionMenuItem);

  mp_ToolsToolButton = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::PROPERTIES));
  mp_ToolsToolButton->set_label(_("Tools"));
  mp_ToolsToolButton->set_is_important(true);
  mp_ToolsToolButton->signal_clicked().connect(sigc::mem_fun(*this,
      &ToolBar::onToolsToolButtonClicked));

  // TOOLBAR

  mp_ToolBar = Gtk::manage(new Gtk::Toolbar());
  mp_ToolBar->set_toolbar_style(Gtk::TOOLBAR_BOTH_HORIZ);
  mp_ToolBar->append(*mp_AddLayerToolButton, sigc::mem_fun(*this,
      &ToolBar::onAddLayerToolButtonClicked));
  mp_ToolBar->append(*getSeparator());
  mp_ToolBar->append(*SelectModeToolButton, sigc::mem_fun(*this,
      &ToolBar::onSelectModeToolButtonClicked));
  mp_ToolBar->append(*mp_MoveModeToggleToolButton, sigc::mem_fun(*this,
      &ToolBar::onMoveModeToggleToolButtonClicked));
  mp_ToolBar->append(*getSeparator());
  mp_ToolBar->append(*mp_Zoom100MenuToolButton);
  mp_ToolBar->append(*mp_ZoomInToggleToolButton, sigc::mem_fun(*this,
      &ToolBar::onZoomInToggleToolButtonClicked));
  mp_ToolBar->append(*mp_ZoomInMenuToolButton);
  mp_ToolBar->append(*mp_ZoomOutToggleToolButton, sigc::mem_fun(*this,
      &ToolBar::onZoomOutToggleToolButtonClicked));
  mp_ToolBar->append(*getSeparator());
  mp_ToolBar->append(*mp_InfoToolButton, sigc::mem_fun(*this,
      &ToolBar::onInfoToolButtonClicked));
  mp_ToolBar->append(*getSeparator());
  mp_ToolBar->append(*mp_ToolsToolButton);
  mp_ToolBar->show_all_children(true);

  setAtLeastALayerMode(false);
  mref_Zoom100AllMenuAction->connect_proxy(*mp_Zoom100MenuToolButton);
}

// =====================================================================
// =====================================================================

Gtk::Widget* ToolBar::asWidget()
{
  return mp_ToolBar;
}

// =====================================================================
// =====================================================================

Gtk::SeparatorToolItem * ToolBar::getSeparator()
{
  Gtk::SeparatorToolItem * p_Separator = Gtk::manage(
      new Gtk::SeparatorToolItem());

  return p_Separator;
}

// =====================================================================
// =====================================================================

void ToolBar::setAtLeastALayerMode(bool AtLeastALayer)
{
  for (int i = 0; i < mp_ToolBar->get_n_items(); i++)
    mp_ToolBar->get_nth_item(i)->set_sensitive(AtLeastALayer);

  mp_AddLayerToolButton->set_sensitive(true);
}

// =====================================================================
// =====================================================================

void ToolBar::setSelectionMode()
{
  mp_MoveModeToggleToolButton->set_active(false);
  mp_ZoomInToggleToolButton->set_active(false);
  mp_ZoomOutToggleToolButton->set_active(false);
}

// =====================================================================
// =====================================================================

void ToolBar::setMoveMode()
{
  mp_MoveModeToggleToolButton->set_active(true);
  mp_ZoomInToggleToolButton->set_active(false);
  mp_ZoomOutToggleToolButton->set_active(false);
}

// =====================================================================
// =====================================================================

void ToolBar::setZoomInMode()
{
  mp_MoveModeToggleToolButton->set_active(false);
  mp_ZoomInToggleToolButton->set_active(true);
  mp_ZoomOutToggleToolButton->set_active(false);
}

// =====================================================================
// =====================================================================

void ToolBar::setZoomOutMode()
{
  mp_MoveModeToggleToolButton->set_active(false);
  mp_ZoomInToggleToolButton->set_active(false);
  mp_ZoomOutToggleToolButton->set_active(true);
}

// =====================================================================
// =====================================================================

void ToolBar::onAddLayerToolButtonClicked()
{
  signal_AddLayerAsked().emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onSelectModeToolButtonClicked()
{
  signal_SelectModeAsked().emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onMoveModeToggleToolButtonClicked()
{
  if (mp_MoveModeToggleToolButton->get_active())
    signal_MoveModeAsked().emit();
  else
    signal_SelectModeAsked().emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoomInCursorMenuActionActivate()
{
  mp_ZoomInToggleToolButton->set_stock_id(Gtk::Stock::ZOOM_IN);

  m_CurrentZoomInMode = ZOOM_IN_CURSOR;

  mp_ZoomInToggleToolButton->set_active();

  onZoomInToggleToolButtonClicked();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoomInFrameMenuActionActivate()
{
  mp_ZoomInToggleToolButton->set_stock_id(Gtk::Stock::ZOOM_FIT);

  m_CurrentZoomInMode = ZOOM_IN_FRAME;

  mp_ZoomInToggleToolButton->set_active();

  onZoomInToggleToolButtonClicked();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoomInToggleToolButtonClicked()
{
  if (mp_ZoomInToggleToolButton->get_active())
  {
    switch (m_CurrentZoomInMode)
    {
      case ZOOM_IN_CURSOR:
        signal_ZoomInCursorAsked().emit();
        break;
      case ZOOM_IN_FRAME:
        signal_ZoomInFrameAsked().emit();
      default:
        break;
    }
  }
  else
    signal_SelectModeAsked().emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoomOutToggleToolButtonClicked()
{
  if (mp_ZoomOutToggleToolButton->get_active())
    m_signal_ZoomOutAsked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::ToolBar::onZoom100AllMenuActionActivate()
{
  mref_Zoom100AllMenuAction->connect_proxy(*mp_Zoom100MenuToolButton);

  signal_Zoom100AllAsked().emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoom100LayerMenuActionActivate()
{
  mref_Zoom100LayerMenuAction->connect_proxy(*mp_Zoom100MenuToolButton);

  signal_Zoom100LayerAsked().emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoom100SelectionMenuActionActivate()
{
  mref_Zoom100SelectionMenuAction->connect_proxy(*mp_Zoom100MenuToolButton);

  signal_Zoom100SelectionAsked().emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onInfoToolButtonClicked()
{
  m_signal_InfoAsked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onToolsSelectAllActivate()
{
  m_signal_SelectAllAsked.emit();
}


// =====================================================================
// =====================================================================

void ToolBar::onToolsToggleSelectionActivate()
{
  m_signal_ToggleSelectionAsked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onToolsToolButtonClicked()
{
  mp_ToolsMenu->popup(sigc::mem_fun(*this, &ToolBar::onToolsMenuPopupPosition),
      0, 0);
}

// =====================================================================
// =====================================================================

void ToolBar::onToolsMenuPopupPosition(int& x, int& y, bool& push_in)
{
  mp_ToolsToolButton->get_window()->get_origin(x, y);

  x += mp_ToolsToolButton->get_allocation().get_x();
  y += mp_ToolsToolButton->get_allocation().get_y()
      + mp_ToolsToolButton->get_allocation().get_height();
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_AddLayerAsked()
{
  return m_signal_AddLayerToolButtonClicked;
}


// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_SelectModeAsked()
{
  return m_signal_SelectModeAsked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_MoveModeAsked()
{
  return m_signal_MoveModeAsked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_ZoomInCursorAsked()
{
  return m_signal_ZoomInCursorAsked;
}

// =====================================================================
// =====================================================================


ToolBar::mtype_SignalToolBar ToolBar::signal_ZoomInFrameAsked()
{
  return m_signal_ZoomInFrameAsked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_ZoomOutAsked()
{
  return m_signal_ZoomOutAsked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_Zoom100AllAsked()
{
  return m_signal_Zoom100AllAsked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_Zoom100LayerAsked()
{
  return m_signal_Zoom100LayerAsked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_Zoom100SelectionAsked()
{
  return m_signal_Zoom100SelectionAsked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_InfoAsked()
{
  return m_signal_InfoAsked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_SelectAllAsked()
{
  return m_signal_SelectAllAsked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signal_ToggleSelectionAsked()
{
  return m_signal_ToggleSelectionAsked;
}

// =====================================================================
// =====================================================================
