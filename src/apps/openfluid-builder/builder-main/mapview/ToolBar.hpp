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

  private:

    bool m_BoolCursorState;
    bool m_BoolFrameState;
    //Gtkmm

    Gtk::HBox* mp_MainHbox;

    Gtk::Toolbar* mp_MainToolBar;

    Gtk::MenuBar* mp_PreferenceMenubar;

    Gtk::ToolButton* mp_AddLayerToolButton;
    Gtk::ToolButton* mp_InfoToolButton;
    Gtk::ToolButton* mp_FocusToolButton;

    Gtk::ToggleToolButton* mp_SelectObjectLayerToggleToolButton;
    Gtk::ToggleToolButton* mp_MoveLayerToggleToolButton;
    Gtk::ToggleToolButton* mp_UnzoomCursorToggleToolButton;
    Gtk::ToggleToolButton* mp_ZoomTypeToggleToolButton;


    Gtk::MenuToolButton* mp_FocusMenuToolButton;
    Gtk::MenuToolButton* mp_ZoomTypeMenuToolButton;

    Gtk::Menu* mp_FocusMenu;
    Gtk::Menu* mp_ZoomTypeMenu;
    Gtk::Menu* mp_PreferenceMenu;

    Glib::RefPtr<Gtk::Action> mref_Show100FocusMenuAction;
    Glib::RefPtr<Gtk::Action> mref_ZoomSelectionFocusMenuAction;
    Glib::RefPtr<Gtk::Action> mref_ZoomLayerFocusMenuAction;
    Glib::RefPtr<Gtk::Action> mref_ZoomCursorZoomTypeMenuAction;
    Glib::RefPtr<Gtk::Action> mref_ZoomFrameZoomTypeMenuAction;
    Glib::RefPtr<Gtk::Action> mref_SelectAllPreferenceAction;
    Glib::RefPtr<Gtk::Action> mref_ToggleSelectedPreferenceAction;

    Gtk::SeparatorToolItem * setSeparator();

    void onChangeShow100Focus();
    void onChangeZoomSelectionFocus();
    void onChangeZoomLayerFocus();

    void onChangeZoomCursorZoomType();
    void onChangeZoomFrameZoomType();
    //*******************Signal emit*******************
    void onShow100FocusButtonClicked();
    void onZoomSelectionFocusButtonClicked();
    void onZoomLayerFocusButtonClicked();

    void onZoomCursorZoomTypeButtonClicked();
    void onZoomFrameZoomTypeButtonClicked();

    void onSelectAllSelectOptionMenuClicked();
    void onToggleSelectedSelectOptionMenuClicked();

    void onAddLayerToolButtonClicked();
    void onInfoToolButtonClicked();
    void onSelectObjectLayerToggleToolButtonClicked();
    void onMoveLayerToggleToolButtonClicked();
    void onUnzoomCursorToggleToolButtonClicked();
    void onZoomTypeToggleToolButtonClicked();
    //*************************************************
    typedef sigc::signal<void> mtype_SignalToolBar;

    mtype_SignalToolBar m_signal_Show100FocusButtonClicked;
    mtype_SignalToolBar m_signal_ZoomSelectionFocusButtonClicked;
    mtype_SignalToolBar m_signal_ZoomLayerFocusButtonClicked;

    mtype_SignalToolBar m_signal_ZoomCursorZoomTypeButtonClicked;
    mtype_SignalToolBar m_signal_ZoomFrameZoomTypeButtonClicked;

    mtype_SignalToolBar m_signal_SelectAllPreferenceMenuClicked;
    mtype_SignalToolBar m_signal_ToggleSelectedPreferenceMenuClicked;

    mtype_SignalToolBar m_signal_AddLayerToolButtonClicked;
    mtype_SignalToolBar m_signal_InfoToolButtonClicked;
    mtype_SignalToolBar m_signal_SelectObjectLayerToggleToolButtonClicked;
    mtype_SignalToolBar m_signal_MoveLayerToggleToolButtonClicked;
    mtype_SignalToolBar m_signal_UnzoomCursorToggleToolButtonClicked;

  public:

    ToolBar();

    Gtk::Widget* asWidget();

    mtype_SignalToolBar signalShow100FocusButtonClicked();
    mtype_SignalToolBar signalZoomSelectionFocusButtonClicked();
    mtype_SignalToolBar signalZoomLayerFocusButtonClicked();

    mtype_SignalToolBar signalZoomCursorZoomTypeButtonClicked();
    mtype_SignalToolBar signalZoomFrameZoomTypeButtonClicked();

    mtype_SignalToolBar signalSelectAllPreferenceMenuClicked();
    mtype_SignalToolBar signalToggleSelectedPreferenceMenuClicked();

    mtype_SignalToolBar signalAddLayerToolButtonClicked();
    mtype_SignalToolBar signalInfoToolButtonClicked();
    mtype_SignalToolBar signalSelectObjectLayerToggleToolButtonClicked();
    mtype_SignalToolBar signalMoveLayerToggleToolButtonClicked();
    mtype_SignalToolBar signalUnzoomCursorToggleToolButtonClicked();

    void resetToolBar(int);
    void resetSensitiveToolBar(bool);
    void setSensitivePreferenceMenubar(bool);

    //accessor*************************************
    //get
    Gtk::ToggleToolButton* getSelectObjectLayerToggleToolButton();
    Gtk::ToggleToolButton* getMoveLayerToggleToolButton();
    Gtk::ToggleToolButton* getUnzoomCursorToggleToolButton();
    Gtk::ToggleToolButton* getZoomTypeToggleToolButton();
};

#endif /* __TOOLBAR_HPP__ */
