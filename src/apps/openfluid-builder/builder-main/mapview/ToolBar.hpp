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
 \file ToolBar.hpp
 \brief Header of ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#ifndef __TOOLBAR_HPP__
#define __TOOLBAR_HPP__

#include <gtkmm/toolbar.h>
#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/action.h>
#include <gtkmm/menutoolbutton.h>
#include <gtkmm/separatortoolitem.h>

class ToolBar
{
  public:

    enum ZoomInMode { ZOOM_IN_CURSOR, ZOOM_IN_FRAME };

  private:

    ZoomInMode m_CurrentZoomInMode;

    Gtk::Toolbar* mp_ToolBar;

    Gtk::ToolButton* mp_AddLayerToolButton;
    Gtk::ToggleToolButton* mp_MoveModeToggleToolButton;

    Gtk::MenuToolButton* mp_Zoom100MenuToolButton;
    Glib::RefPtr<Gtk::Action> mref_Zoom100AllMenuAction;
    Glib::RefPtr<Gtk::Action> mref_Zoom100LayerMenuAction;
    Glib::RefPtr<Gtk::Action> mref_Zoom100SelectionMenuAction;

    Gtk::ToggleToolButton* mp_ZoomInToggleToolButton;
    Gtk::Menu* mp_ZoomInMenu;

    Gtk::ToggleToolButton* mp_ZoomOutToggleToolButton;

    Gtk::ToolButton* mp_InfoToolButton;

    Gtk::Menu* mp_ToolsMenu;
    Gtk::ToolButton* mp_ToolsToolButton;

    Gtk::SeparatorToolItem * getSeparator();

    void onAddLayerToolButtonClicked();
    void onSelectModeToolButtonClicked();
    void onMoveModeToggleToolButtonClicked();

    void onZoomInCursorMenuActionActivate();
    void onZoomInFrameMenuActionActivate();
    void onZoomInToggleToolButtonClicked();
    void onZoomOutToggleToolButtonClicked();

    void onZoom100AllMenuActionActivate();
    void onZoom100SelectionMenuActionActivate();
    void onZoom100LayerMenuActionActivate();

    void onToolsSelectAllActivate();
    void onToolsToggleSelectionActivate();

    void onInfoToolButtonClicked();

    void onToolsToolButtonClicked();
    void onToolsMenuPopupPosition(int& x, int& y, bool& push_in);

    //*************************************************
    typedef sigc::signal<void> mtype_SignalToolBar;

    mtype_SignalToolBar m_signal_AddLayerToolButtonClicked;
    mtype_SignalToolBar m_signal_SelectModeAsked;
    mtype_SignalToolBar m_signal_MoveModeAsked;

    mtype_SignalToolBar m_signal_ZoomInCursorAsked;
    mtype_SignalToolBar m_signal_ZoomInFrameAsked;
    mtype_SignalToolBar m_signal_ZoomOutAsked;

    mtype_SignalToolBar m_signal_Zoom100AllAsked;
    mtype_SignalToolBar m_signal_Zoom100LayerAsked;
    mtype_SignalToolBar m_signal_Zoom100SelectionAsked;

    mtype_SignalToolBar m_signal_InfoAsked;

    mtype_SignalToolBar m_signal_SelectAllAsked;
    mtype_SignalToolBar m_signal_ToggleSelectionAsked;

  public:

    ToolBar();

    Gtk::Widget* asWidget();

    void setAtLeastALayerMode(bool AtLeastALayer);

    void setSelectionMode();
    void setMoveMode();
    void setZoomInMode();
    void setZoomOutMode();

    mtype_SignalToolBar signal_AddLayerAsked();
    mtype_SignalToolBar signal_SelectModeAsked();
    mtype_SignalToolBar signal_MoveModeAsked();

    mtype_SignalToolBar signal_ZoomInCursorAsked();
    mtype_SignalToolBar signal_ZoomInFrameAsked();
    mtype_SignalToolBar signal_ZoomOutAsked();

    mtype_SignalToolBar signal_Zoom100AllAsked();
    mtype_SignalToolBar signal_Zoom100LayerAsked();
    mtype_SignalToolBar signal_Zoom100SelectionAsked();

    mtype_SignalToolBar signal_InfoAsked();

    mtype_SignalToolBar signal_SelectAllAsked();
    mtype_SignalToolBar signal_ToggleSelectionAsked();
};

#endif /* __TOOLBAR_HPP__ */
