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
 \file MapViewAction.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "MapViewAction.hpp"

#include <glibmm/i18n.h>
#include <iostream>

// =====================================================================
// =====================================================================

MapViewAction::MapViewAction()
{
  mref_UIToolBarManager = Gtk::UIManager::create();
  createToolBarUiXml();
  createToolBarActionGroup();
  mref_UIToolBarManager->insert_action_group(mref_ToolBarActionGroup, 1);
  try
  {
    mref_UIToolBarManager->add_ui_from_string(m_ToolBarUiXml);
  } catch (const Glib::Error& ex)
  {
    std::cerr << "building menus failed: " << ex.what();
  }
}

// =====================================================================
// =====================================================================


void MapViewAction::createToolBarUiXml()
{
  m_ToolBarUiXml = "<ui>"
    "  <toolbar  name='ToolBar'>"
    "    <toolitem action='AddNewLayer'/>"
    "    <toolitem action='RemoveLayer'/>"
    "    <separator/>"
    "    <toolitem action='Move'/>"
    "    <toolitem action='Select'/>"
    "    <separator/>"
    "    <toolitem action='Show100'/>"
    "    <separator/>"
    "    <separator/>"
    "    <toolitem action='Info'/>"
    "  </toolbar>"
    "</ui>";
}

// =====================================================================
// =====================================================================


void MapViewAction::createToolBarActionGroup()
{
  mref_ToolBarActionGroup = Gtk::ActionGroup::create();

  mref_ToolBarActionGroup->add(
      Gtk::Action::create("AddNewLayer", Gtk::Stock::ADD, _("Add"),
          _("Add a new layer")));
  mref_ToolBarActionGroup->add(
      Gtk::Action::create("RemoveLayer", Gtk::Stock::REMOVE, _("Remove"),
          _("Remove a layer")));
  mref_ToolBarActionGroup->add(
      Gtk::Action::create("Move", Gtk::Stock::GOTO_TOP, _("Move"),
          _("Move a layer")));
  mref_ToolBarActionGroup->add(
      Gtk::Action::create("Select", Gtk::Stock::CONVERT, _("Select"),
          _("Select a object of the current layer")));
  mref_ToolBarActionGroup->add(
      Gtk::Action::create("Show100", Gtk::Stock::ZOOM_100, _("Zoom_100%"),
          _("1:1 scale display")));
  mref_ToolBarActionGroup->add(
      Gtk::Action::create("Info", Gtk::Stock::INFO, _("Info"),
          _("Display an information of the current object")));

  mref_ZoomFrame = Gtk::Action::create("ZoomFrame", Gtk::Stock::ZOOM_FIT,
      _("ZoomFrame"), _("Zooms in on a frame"));
  mref_ZoomObject = Gtk::Action::create("ZoomObject", Gtk::Stock::ZOOM_FIT,
      _("ZoomObject"), _("Zooms in on the selected object"));
  mref_ZoomCursor = Gtk::Action::create("ZoomCursor", Gtk::Stock::ZOOM_IN,
      _("ZoomCursor"), _("Zooms in on the cursor position"));
  mref_UnzoomCursor = Gtk::Action::create("UnzoomCursor", Gtk::Stock::ZOOM_OUT,
      _("UnzoomCursor"), _("Zooms out on the cursor position"));
  mref_UnzoomObject = Gtk::Action::create("UnzoomObject", Gtk::Stock::ZOOM_IN,
      _("UnzoomObject"), _("Zooms out on the selected object"));

  mref_ChangeZoomFrame = Gtk::Action::create("ZoomFrame", Gtk::Stock::ZOOM_FIT,
      _("Zoom in Frame"), _("Zooms in on a frame"));
  mref_ChangeZoomObject = Gtk::Action::create("ZoomObject", Gtk::Stock::ZOOM_FIT,
      _("Zoom in Object"), _("Zooms in on the selected object"));
  mref_ChangeZoomCursor = Gtk::Action::create("ZoomCursor", Gtk::Stock::ZOOM_IN,
      _("Zoom in Cursor"), _("Zooms in on the cursor position"));
  mref_ChangeUnzoomCursor = Gtk::Action::create("UnzoomCursor", Gtk::Stock::ZOOM_OUT,
      _("Zoom out Cursor"), _("Zooms out on the cursor position"));
  mref_ChangeUnzoomObject = Gtk::Action::create("UnzoomObject", Gtk::Stock::ZOOM_IN,
      _("Zoom out Object"), _("Zooms out on the selected object"));

}

// =====================================================================
// =====================================================================

Gtk::Widget* MapViewAction::getToolBarWidget()
{
  return mref_UIToolBarManager->get_widget("/ToolBar");
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::AccelGroup> MapViewAction::getAccelGroup()
{
  return mref_UIToolBarManager->get_accel_group();
}

// =====================================================================
// =====================================================================


void MapViewAction::setToolBarActionGroupSensitive(bool Sensitive)
{
  mref_ToolBarActionGroup->set_sensitive(Sensitive);
}

// =====================================================================
// =====================================================================


void MapViewAction::setToolBarActionGroupVisible(bool Visible)
{
  mref_ToolBarActionGroup->set_visible(Visible);
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getAddNewLayerAction()
{
  return mref_ToolBarActionGroup->get_action("AddNewLayer");
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getRemoveLayerAction()
{
  return mref_ToolBarActionGroup->get_action("RemoveLayer");
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getMoveAction()
{
  return mref_ToolBarActionGroup->get_action("Move");
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getSelectAction()
{
  return mref_ToolBarActionGroup->get_action("Select");
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getShow100Action()
{
  return mref_ToolBarActionGroup->get_action("Show100");
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getZoomCursorAction()
{
  return mref_ZoomCursor;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getZoomFrameAction()
{
  return mref_ZoomFrame;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getZoomObjectAction()
{
  return mref_ZoomObject;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getUnzoomCursorAction()
{
  return mref_UnzoomCursor;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getUnzoomObjectAction()
{
  return mref_UnzoomObject;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getInfoAction()
{
  return mref_ToolBarActionGroup->get_action("Info");
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getChangeZoomCursorAction()
{
  return mref_ChangeZoomCursor;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getChangeZoomFrameAction()
{
  return mref_ChangeZoomFrame;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getChangeZoomObjectAction()
{
  return mref_ChangeZoomObject;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getChangeUnzoomCursorAction()
{
  return mref_ChangeUnzoomCursor;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::Action> MapViewAction::getChangeUnzoomObjectAction()
{
  return mref_ChangeUnzoomObject;
}

// =====================================================================
// =====================================================================

