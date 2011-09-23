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

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <iostream>

#include "ToolBar.hpp"

ToolBar::ToolBar()
{

  mp_MainHbox = Gtk::manage(new Gtk::HBox());
  mp_MainToolBar = Gtk::manage(new Gtk::Toolbar());

  mp_AddLayerToolButton = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::ADD));
  mp_AddLayerToolButton->set_tooltip_markup(_("Add A New Layer To Map View"));
  mp_AddLayerToolButton->set_visible(true);

  mp_InfoToolButton = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::INFO));
  mp_InfoToolButton->set_tooltip_markup(_("Information Of Current Selection"));
  mp_InfoToolButton->set_visible(true);

  //  //****************PreferenceMenubar*************
  mp_PreferenceMenubar = Gtk::manage(new Gtk::MenuBar());
  mp_PreferenceMenu = Gtk::manage(new Gtk::Menu());

  mref_SelectAllPreferenceAction = Gtk::Action::create("SelectAll",
      Gtk::Stock::REFRESH, _("Select All"),
      _("Select All Elements Of Current Layer"));
  mref_SelectAllPreferenceAction->set_visible(true);
  mref_SelectAllPreferenceAction->set_always_show_image(true);

  mref_ToggleSelectedPreferenceAction = Gtk::Action::create("ToggleSelected",
      Gtk::Stock::SELECT_ALL, _("Toggle Selected"),
      _("Toggle Elements Selected Of Current Layer"));
  mref_ToggleSelectedPreferenceAction->set_always_show_image(true);
  mref_ToggleSelectedPreferenceAction->set_visible(true);

  mp_PreferenceMenu->append(*mref_SelectAllPreferenceAction->create_menu_item());
  mp_PreferenceMenu->append(
      *mref_ToggleSelectedPreferenceAction->create_menu_item());
  Gtk::ImageMenuItem* MenuItem = Gtk::manage(
      new Gtk::ImageMenuItem(Gtk::Stock::PROPERTIES));
  MenuItem->set_label(_("Tools"));
  MenuItem->set_submenu(*mp_PreferenceMenu);
  mp_PreferenceMenubar->append(*MenuItem);
  MenuItem->set_visible(true);
  MenuItem->set_always_show_image(true);

  mp_PreferenceMenubar->modify_bg(Gtk::STATE_NORMAL,
      mp_PreferenceMenubar->get_style()->get_background(Gtk::STATE_SELECTED));

  mp_PreferenceMenu->set_visible(true);
  mp_PreferenceMenubar->set_visible(true);

  //*******************************************************

  mp_SelectObjectLayerToggleToolButton = Gtk::manage(
      new Gtk::ToggleToolButton(Gtk::Stock::SELECT_COLOR));
  mp_SelectObjectLayerToggleToolButton->set_tooltip_markup(
      _("Select Object(s) Of Current Layer"));
  mp_SelectObjectLayerToggleToolButton->set_visible(true);

  mp_MoveLayerToggleToolButton = Gtk::manage(
      new Gtk::ToggleToolButton(Gtk::Stock::FULLSCREEN));
  mp_MoveLayerToggleToolButton->set_tooltip_markup(_("Move View Of Layers"));
  mp_MoveLayerToggleToolButton->set_visible(true);

  //  //****************FocusMenuToolButton*************
  mp_FocusMenuToolButton = Gtk::manage(new Gtk::MenuToolButton());
  mp_FocusMenu = Gtk::manage(new Gtk::Menu());

  Gtk::HBox* TempHbox1 =
      static_cast<Gtk::HBox*> (mp_FocusMenuToolButton->get_child());
  mp_FocusToolButton = Gtk::manage(new Gtk::ToolButton());
  TempHbox1->remove(**(TempHbox1->get_children().begin()));
  mp_FocusToolButton->set_visible(true);

  mref_Show100FocusMenuAction = Gtk::Action::create("Display100",
      Gtk::Stock::ZOOM_100, _("Display 100%"),
      _("Scale 100% All Layer Display"));
  mref_Show100FocusMenuAction->set_visible(true);
  mref_Show100FocusMenuAction->set_always_show_image(true);

  mref_ZoomLayerFocusMenuAction = Gtk::Action::create("Display100Layer",
      Gtk::Stock::ZOOM_IN, _("Display 100% Layer"),
      _("Scale 100% Current Layer Display"));
  mref_ZoomLayerFocusMenuAction->set_always_show_image(true);
  mref_ZoomLayerFocusMenuAction->set_visible(true);

  mref_ZoomSelectionFocusMenuAction = Gtk::Action::create(
      "Display100Selection", Gtk::Stock::ZOOM_IN, _("Display 100% Selection"),
      _("Scale 100% Current Selection Display"));
  mref_ZoomSelectionFocusMenuAction->set_always_show_image(true);
  mref_ZoomSelectionFocusMenuAction->set_visible(true);

  mp_FocusMenu->append(*mref_Show100FocusMenuAction->create_menu_item());
  mp_FocusMenu->append(*mref_ZoomLayerFocusMenuAction->create_menu_item());
  mp_FocusMenu->append(*mref_ZoomSelectionFocusMenuAction->create_menu_item());
  mp_FocusMenuToolButton->set_menu(*mp_FocusMenu);

  mref_Show100FocusMenuAction->connect_proxy(*mp_FocusToolButton);
  //  //*******************************************************

  //****************ZoomTypeMenuToolButton*************

  mp_ZoomTypeMenuToolButton = Gtk::manage(new Gtk::MenuToolButton());
  mp_ZoomTypeMenu = Gtk::manage(new Gtk::Menu());

  Gtk::HBox* TempHbox2 =
      static_cast<Gtk::HBox*> (mp_ZoomTypeMenuToolButton->get_child());
  mp_ZoomTypeToggleToolButton = Gtk::manage(new Gtk::ToggleToolButton());
  TempHbox2->remove(**(TempHbox2->get_children().begin()));
  mp_ZoomTypeToggleToolButton->set_visible(true);

  mref_ZoomCursorZoomTypeMenuAction = Gtk::Action::create("ZoomCursor",
      Gtk::Stock::ZOOM_IN, _("Zoom Cursor"),
      _("Zooms In On The Cursor Position"));
  mref_ZoomCursorZoomTypeMenuAction->set_visible(true);
  mref_ZoomCursorZoomTypeMenuAction->set_always_show_image(true);

  mref_ZoomFrameZoomTypeMenuAction = Gtk::Action::create("ZoomFrame",
      Gtk::Stock::ZOOM_FIT, _("Zoom Frame"), _("Zooms In On A Frame"));
  mref_ZoomFrameZoomTypeMenuAction->set_always_show_image(true);
  mref_ZoomFrameZoomTypeMenuAction->set_visible(true);

  mp_ZoomTypeMenu->append(
      *mref_ZoomCursorZoomTypeMenuAction->create_menu_item());
  mp_ZoomTypeMenu->append(*mref_ZoomFrameZoomTypeMenuAction->create_menu_item());
  mp_ZoomTypeMenuToolButton->set_menu(*mp_ZoomTypeMenu);

  mp_ZoomTypeToggleToolButton->set_stock_id(Gtk::Stock::ZOOM_IN);

  m_BoolCursorState = true;
  m_BoolFrameState = false;

  //*******************************************************

  mp_UnzoomCursorToggleToolButton = Gtk::manage(
      new Gtk::ToggleToolButton(Gtk::Stock::ZOOM_OUT));
  mp_UnzoomCursorToggleToolButton->set_tooltip_markup(
      _("Zooms Out On The Cursor Position"));
  mp_UnzoomCursorToggleToolButton->set_visible(true);

  mp_MainToolBar->append(*mp_AddLayerToolButton);
  mp_MainToolBar->append(*setSeparator());
  mp_MainToolBar->append(*mp_SelectObjectLayerToggleToolButton);
  mp_MainToolBar->append(*mp_MoveLayerToggleToolButton);
  mp_MainToolBar->append(*mp_FocusToolButton);
  mp_MainToolBar->append(*mp_FocusMenuToolButton);
  mp_MainToolBar->append(*mp_ZoomTypeToggleToolButton);
  mp_MainToolBar->append(*mp_ZoomTypeMenuToolButton);
  mp_MainToolBar->append(*mp_UnzoomCursorToggleToolButton);
  mp_MainToolBar->append(*setSeparator());
  mp_MainToolBar->append(*mp_InfoToolButton);
  mp_MainToolBar->append(*setSeparator());
  mp_MainToolBar->set_show_arrow(false);
  mp_MainHbox->pack_start(*mp_MainToolBar, Gtk::PACK_SHRINK);
  mp_MainHbox->pack_start(*mp_PreferenceMenubar, Gtk::PACK_SHRINK);
  mp_MainHbox->set_visible(true);
  mp_MainToolBar->set_visible(true);

  //******************Signal connexion*********************

  mref_ZoomCursorZoomTypeMenuAction->signal_activate().connect(
      sigc::mem_fun(*this, &ToolBar::onChangeZoomCursorZoomType));
  mref_ZoomFrameZoomTypeMenuAction->signal_activate().connect(
      sigc::mem_fun(*this, &ToolBar::onChangeZoomFrameZoomType));
  mref_ZoomLayerFocusMenuAction->signal_activate().connect(
      sigc::mem_fun(*this, &ToolBar::onChangeZoomLayerFocus));
  mref_ZoomSelectionFocusMenuAction->signal_activate().connect(
      sigc::mem_fun(*this, &ToolBar::onChangeZoomSelectionFocus));
  mref_SelectAllPreferenceAction->signal_activate().connect(
      sigc::mem_fun(*this, &ToolBar::onSelectAllSelectOptionMenuClicked));
  mref_Show100FocusMenuAction->signal_activate().connect(
      sigc::mem_fun(*this, &ToolBar::onChangeShow100Focus));
  mref_ToggleSelectedPreferenceAction->signal_activate().connect(
      sigc::mem_fun(*this, &ToolBar::onToggleSelectedSelectOptionMenuClicked));

  mp_AddLayerToolButton->signal_clicked().connect(
      sigc::mem_fun(*this, &ToolBar::onAddLayerToolButtonClicked));
  mp_InfoToolButton->signal_clicked().connect(
      sigc::mem_fun(*this, &ToolBar::onInfoToolButtonClicked));
  mp_MoveLayerToggleToolButton->signal_toggled().connect(
      sigc::mem_fun(*this, &ToolBar::onMoveLayerToggleToolButtonClicked));
  mp_SelectObjectLayerToggleToolButton->signal_toggled().connect(
      sigc::mem_fun(*this, &ToolBar::onSelectObjectLayerToggleToolButtonClicked));
  mp_UnzoomCursorToggleToolButton->signal_toggled().connect(
      sigc::mem_fun(*this, &ToolBar::onUnzoomCursorToggleToolButtonClicked));
  mp_ZoomTypeToggleToolButton->signal_toggled().connect(
      sigc::mem_fun(*this, &ToolBar::onZoomTypeToggleToolButtonClicked));
  resetSensitiveToolBar(false);
}

// =====================================================================
// =====================================================================


Gtk::Widget* ToolBar::asWidget()
{
  return mp_MainHbox;
}

// =====================================================================
// =====================================================================

Gtk::SeparatorToolItem * ToolBar::setSeparator()
{
  Gtk::SeparatorToolItem * p_Separator = Gtk::manage(
      new Gtk::SeparatorToolItem());
  return p_Separator;
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void ToolBar::ToolBar::onChangeShow100Focus()
{
  mref_Show100FocusMenuAction->connect_proxy(*mp_FocusToolButton);
  onShow100FocusButtonClicked();
}

// =====================================================================
// =====================================================================

void ToolBar::onChangeZoomSelectionFocus()
{
  mref_ZoomSelectionFocusMenuAction->connect_proxy(*mp_FocusToolButton);
  onZoomSelectionFocusButtonClicked();
}

// =====================================================================
// =====================================================================

void ToolBar::onChangeZoomLayerFocus()
{
  mref_ZoomLayerFocusMenuAction->connect_proxy(*mp_FocusToolButton);
  onZoomLayerFocusButtonClicked();
}

// =====================================================================
// =====================================================================

void ToolBar::onChangeZoomCursorZoomType()
{
  mp_ZoomTypeToggleToolButton->set_stock_id(Gtk::Stock::ZOOM_IN);
  mp_ZoomTypeToggleToolButton->set_active(true);
  m_BoolCursorState = true;
  m_BoolFrameState = false;
  //  mref_ZoomCursorZoomTypeMenuAction->connect_proxy(*mp_ZoomTypeToggleToolButton);
  onZoomTypeToggleToolButtonClicked();
}

// =====================================================================
// =====================================================================

void ToolBar::onChangeZoomFrameZoomType()
{
  mp_ZoomTypeToggleToolButton->set_stock_id(Gtk::Stock::ZOOM_FIT);
  mp_ZoomTypeToggleToolButton->set_active(true);
  m_BoolCursorState = false;
  m_BoolFrameState = true;
  //  mref_ZoomFrameZoomTypeMenuAction->connect_proxy(*mp_ZoomTypeToggleToolButton);
  onZoomTypeToggleToolButtonClicked();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void ToolBar::onAddLayerToolButtonClicked()
{
  m_signal_AddLayerToolButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onInfoToolButtonClicked()
{
  m_signal_InfoToolButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onMoveLayerToggleToolButtonClicked()
{
  m_signal_MoveLayerToggleToolButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onSelectAllSelectOptionMenuClicked()
{
  m_signal_SelectAllPreferenceMenuClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onSelectObjectLayerToggleToolButtonClicked()
{
  m_signal_SelectObjectLayerToggleToolButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onShow100FocusButtonClicked()
{
  m_signal_Show100FocusButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onToggleSelectedSelectOptionMenuClicked()
{
  m_signal_ToggleSelectedPreferenceMenuClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onUnzoomCursorToggleToolButtonClicked()
{
  m_signal_UnzoomCursorToggleToolButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoomTypeToggleToolButtonClicked()
{
  //  Glib::RefPtr<Gtk::Action> TempAction =
  //      mp_ZoomTypeToggleToolButton->get_action();
  //  if (TempAction == mref_ZoomCursorZoomTypeMenuAction)
  if (m_BoolCursorState)
  {
//    std::cout << "ZoomCursor : " << std::endl;
    onZoomCursorZoomTypeButtonClicked();
    //  } else if (TempAction == mref_ZoomFrameZoomTypeMenuAction)
  } else if (m_BoolFrameState)
  {
    std::cout << "ZoomFrame : " << std::endl;
    onZoomFrameZoomTypeButtonClicked();
  } else
  {
    std::cout << "error : " << std::endl;
  }
}

// =====================================================================
// =====================================================================

void ToolBar::onZoomCursorZoomTypeButtonClicked()
{
  m_signal_ZoomCursorZoomTypeButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoomFrameZoomTypeButtonClicked()
{
  m_signal_ZoomFrameZoomTypeButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoomLayerFocusButtonClicked()
{
  m_signal_ZoomLayerFocusButtonClicked.emit();
}

// =====================================================================
// =====================================================================

void ToolBar::onZoomSelectionFocusButtonClicked()
{
  m_signal_ZoomSelectionFocusButtonClicked.emit();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalShow100FocusButtonClicked()
{
  return m_signal_Show100FocusButtonClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalZoomSelectionFocusButtonClicked()
{
  return m_signal_ZoomSelectionFocusButtonClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalZoomLayerFocusButtonClicked()
{
  return m_signal_ZoomLayerFocusButtonClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalZoomCursorZoomTypeButtonClicked()
{
  return m_signal_ZoomCursorZoomTypeButtonClicked;
}

// =====================================================================
// =====================================================================


ToolBar::mtype_SignalToolBar ToolBar::signalZoomFrameZoomTypeButtonClicked()
{
  return m_signal_ZoomFrameZoomTypeButtonClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalSelectAllPreferenceMenuClicked()
{
  return m_signal_SelectAllPreferenceMenuClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalToggleSelectedPreferenceMenuClicked()
{
  return m_signal_ToggleSelectedPreferenceMenuClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalAddLayerToolButtonClicked()
{
  return m_signal_AddLayerToolButtonClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalInfoToolButtonClicked()
{
  return m_signal_InfoToolButtonClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalSelectObjectLayerToggleToolButtonClicked()
{
  return m_signal_SelectObjectLayerToggleToolButtonClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalMoveLayerToggleToolButtonClicked()
{
  return m_signal_MoveLayerToggleToolButtonClicked;
}

// =====================================================================
// =====================================================================

ToolBar::mtype_SignalToolBar ToolBar::signalUnzoomCursorToggleToolButtonClicked()
{
  return m_signal_UnzoomCursorToggleToolButtonClicked;
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void ToolBar::resetToolBar(int i)
{
  if (mp_MoveLayerToggleToolButton->get_active() && i != 1)
    mp_MoveLayerToggleToolButton->set_active(false);
  if (mp_SelectObjectLayerToggleToolButton->get_active() && i != 2)
    mp_SelectObjectLayerToggleToolButton->set_active(false);
  if (mp_UnzoomCursorToggleToolButton->get_active() && i != 3)
    mp_UnzoomCursorToggleToolButton->set_active(false);
  if (mp_ZoomTypeToggleToolButton->get_active() && i != 4)
  {
    mp_ZoomTypeToggleToolButton->set_active(false);
  }
}

// =====================================================================
// =====================================================================

void ToolBar::resetSensitiveToolBar(bool Sensitive)
{
  if (mp_MoveLayerToggleToolButton->get_active())
    mp_MoveLayerToggleToolButton->set_active(false);
  if (mp_SelectObjectLayerToggleToolButton->get_active())
    mp_SelectObjectLayerToggleToolButton->set_active(false);
  if (mp_UnzoomCursorToggleToolButton->get_active())
    mp_UnzoomCursorToggleToolButton->set_active(false);
  if (mp_ZoomTypeToggleToolButton->get_active())
    mp_ZoomTypeToggleToolButton->set_active(false);

  if (!Sensitive)
  {
    mp_PreferenceMenubar->set_sensitive(false);

    mp_InfoToolButton->set_sensitive(false);
    mp_FocusToolButton->set_sensitive(false);

    mp_SelectObjectLayerToggleToolButton->set_sensitive(false);
    mp_MoveLayerToggleToolButton->set_sensitive(false);
    mp_UnzoomCursorToggleToolButton->set_sensitive(false);
    mp_ZoomTypeToggleToolButton->set_sensitive(false);

    mp_FocusMenuToolButton->set_sensitive(false);
    mp_ZoomTypeMenuToolButton->set_sensitive(false);
  } else
  {
    mp_InfoToolButton->set_sensitive(true);
    mp_FocusToolButton->set_sensitive(true);

    mp_SelectObjectLayerToggleToolButton->set_sensitive(true);
    mp_MoveLayerToggleToolButton->set_sensitive(true);
    mp_UnzoomCursorToggleToolButton->set_sensitive(true);
    mp_ZoomTypeToggleToolButton->set_sensitive(true);

    mp_FocusMenuToolButton->set_sensitive(true);
    mp_ZoomTypeMenuToolButton->set_sensitive(true);
  }

}

// =====================================================================
// =====================================================================

void ToolBar::setSensitivePreferenceMenubar(bool Sensitive)
{
  mp_PreferenceMenubar->set_sensitive(Sensitive);
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

Gtk::ToggleToolButton* ToolBar::getSelectObjectLayerToggleToolButton()
{
  return mp_SelectObjectLayerToggleToolButton;
}

// =====================================================================
// =====================================================================

Gtk::ToggleToolButton* ToolBar::getMoveLayerToggleToolButton()
{
  return mp_MoveLayerToggleToolButton;
}

// =====================================================================
// =====================================================================

Gtk::ToggleToolButton* ToolBar::getUnzoomCursorToggleToolButton()
{
  return mp_UnzoomCursorToggleToolButton;
}

// =====================================================================
// =====================================================================

Gtk::ToggleToolButton* ToolBar::getZoomTypeToggleToolButton()
{
  return mp_ZoomTypeToggleToolButton;
}

