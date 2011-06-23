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
 \file MapViewToolBar.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "MapViewToolBar.hpp"

MapViewToolBar::MapViewToolBar(MapViewAction& ToolBarAction,
    MapViewTreeLayer& MapViewTreeLayer) :
  mref_MapViewTreeLayer(MapViewTreeLayer)
{

  mp_MenuButtonZoom = Gtk::manage(new Gtk::MenuToolButton());
  mp_MenuButtonUnzoom = Gtk::manage(new Gtk::MenuToolButton());

  mp_MenuUnzoom = Gtk::manage(new Gtk::Menu());
  mp_MenuZoom = Gtk::manage(new Gtk::Menu());

  mp_ZoomCursor = Gtk::manage(new Gtk::ImageMenuItem());
  mp_ZoomFrame = Gtk::manage(new Gtk::ImageMenuItem());
  mp_ZoomObject = Gtk::manage(new Gtk::ImageMenuItem());
  mp_UnzoomCursor = Gtk::manage(new Gtk::ImageMenuItem());
  mp_UnzoomObject = Gtk::manage(new Gtk::ImageMenuItem());

  mp_ToggleButtonMove = Gtk::manage(new Gtk::ToggleToolButton());
  mp_ToggleButtonSelect = Gtk::manage(new Gtk::ToggleToolButton());

  ToolBarAction.getUnzoomCursorAction()->connect_proxy(*mp_MenuButtonUnzoom);
  ToolBarAction.getZoomCursorAction()->connect_proxy(*mp_MenuButtonZoom);
  ToolBarAction.getMoveAction()->connect_proxy(*mp_ToggleButtonMove);
  ToolBarAction.getSelectAction()->connect_proxy(*mp_ToggleButtonSelect);

  mp_ZoomCursor
      = ((Gtk::ImageMenuItem *) (ToolBarAction.getChangeZoomCursorAction())->create_menu_item());
  mp_ZoomFrame
      = ((Gtk::ImageMenuItem *) (ToolBarAction.getChangeZoomFrameAction())->create_menu_item());
  mp_ZoomObject
      = ((Gtk::ImageMenuItem *) (ToolBarAction.getChangeZoomObjectAction())->create_menu_item());
  mp_UnzoomCursor
      = ((Gtk::ImageMenuItem *) (ToolBarAction.getChangeUnzoomCursorAction())->create_menu_item());
  mp_UnzoomObject
      = ((Gtk::ImageMenuItem *) (ToolBarAction.getChangeUnzoomObjectAction())->create_menu_item());

  mp_ZoomCursor->set_always_show_image(true);
  mp_ZoomObject->set_always_show_image(true);
  mp_ZoomFrame->set_always_show_image(true);
  mp_UnzoomCursor->set_always_show_image(true);
  mp_UnzoomObject->set_always_show_image(true);

  mp_MenuZoom->append(*mp_ZoomCursor);
  mp_MenuZoom->append(*mp_ZoomObject);
  mp_MenuZoom->append(*mp_ZoomFrame);
  mp_MenuUnzoom->append(*mp_UnzoomCursor);
  mp_MenuUnzoom->append(*mp_UnzoomObject);

  mp_MenuButtonUnzoom->set_menu(*mp_MenuUnzoom);
  mp_MenuButtonZoom->set_menu(*mp_MenuZoom);

  ToolBarAction.getMoveAction()->signal_activate().connect(
      sigc::mem_fun(*this, &MapViewToolBar::onMove));
  ToolBarAction.getSelectAction()->signal_activate().connect(
      sigc::mem_fun(*this, &MapViewToolBar::onSelect));

  ToolBarAction.getAddNewLayerAction()->signal_activate().connect(
          sigc::mem_fun(*this, &MapViewToolBar::onAddNewLayer));
  ToolBarAction.getRemoveLayerAction()->signal_activate().connect(
          sigc::mem_fun(*this, &MapViewToolBar::onRemoveLayer));
  ToolBarAction.getShow100Action()->signal_activate().connect(
          sigc::mem_fun(*this, &MapViewToolBar::onShow100));
  ToolBarAction.getInfoAction()->signal_activate().connect(sigc::mem_fun(*this, &MapViewToolBar::onInfo));
  ToolBarAction.getZoomCursorAction()->signal_activate().connect(
          sigc::mem_fun(*this, &MapViewToolBar::onZoomCursor));
  ToolBarAction.getZoomFrameAction()->signal_activate().connect(
          sigc::mem_fun(*this, &MapViewToolBar::onZoomFrame));
  ToolBarAction.getZoomObjectAction()->signal_activate().connect(
          sigc::mem_fun(*this, &MapViewToolBar::onZoomObject));
  ToolBarAction.getUnzoomCursorAction()->signal_activate().connect(
          sigc::mem_fun(*this, &MapViewToolBar::onUnzoomCursor));
  ToolBarAction.getUnzoomObjectAction()->signal_activate().connect(
          sigc::mem_fun(*this, &MapViewToolBar::onUnzoomObject));

  ToolBarAction.getChangeZoomCursorAction()->signal_activate().connect(
      sigc::bind<MapViewAction&>(
          sigc::mem_fun(*this, &MapViewToolBar::onChangeZoomCursor),
          ToolBarAction));
  ToolBarAction.getChangeZoomFrameAction()->signal_activate().connect(
      sigc::bind<MapViewAction&>(
          sigc::mem_fun(*this, &MapViewToolBar::onChangeZoomFrame),
          ToolBarAction));
  ToolBarAction.getChangeZoomObjectAction()->signal_activate().connect(
      sigc::bind<MapViewAction&>(
          sigc::mem_fun(*this, &MapViewToolBar::onChangeZoomObject),
          ToolBarAction));
  ToolBarAction.getChangeUnzoomCursorAction()->signal_activate().connect(
      sigc::bind<MapViewAction&>(
          sigc::mem_fun(*this, &MapViewToolBar::onChangeUnzoomCursor),
          ToolBarAction));
  ToolBarAction.getChangeUnzoomObjectAction()->signal_activate().connect(
      sigc::bind<MapViewAction&>(
          sigc::mem_fun(*this, &MapViewToolBar::onChangeUnzoomObject),
          ToolBarAction));

  append(*ToolBarAction.getAddNewLayerAction()->create_tool_item());
  append(*ToolBarAction.getRemoveLayerAction()->create_tool_item());
  append(*setSeparator());
  append(*mp_ToggleButtonSelect);
  append(*mp_ToggleButtonMove);
  append(*setSeparator());
  append(*ToolBarAction.getShow100Action()->create_tool_item());
  append(*mp_MenuButtonZoom);
  append(*mp_MenuButtonUnzoom);
  append(*setSeparator());
  append(*ToolBarAction.getInfoAction()->create_tool_item());
}

// =====================================================================
// =====================================================================

MapViewToolBar::~MapViewToolBar()
{

}

// =====================================================================
// =====================================================================

Gtk::SeparatorToolItem * MapViewToolBar::setSeparator()
{
  Gtk::SeparatorToolItem * pSeparator = Gtk::manage(
      new Gtk::SeparatorToolItem());
  return pSeparator;
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onAddNewLayer()
{
  Gtk::FileChooserDialog dialog(_("Please choose a new layer"),
      Gtk::FILE_CHOOSER_ACTION_OPEN);
  //    dialog.set_transient_for(*this);

  //Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  //Add filters, so that only certain file types can be selected:

  Gtk::FileFilter filter_shapefile;
  filter_shapefile.set_name("Shapefiles");
  filter_shapefile.add_pattern("*.shp");
  dialog.add_filter(filter_shapefile);

  Gtk::FileFilter filter_any;
  filter_any.set_name("Any files");
  filter_any.add_pattern("*");
  dialog.add_filter(filter_any);

  //Show the dialog and wait for a user response:
  int result = dialog.run();

  //Handle the response:
  switch (result)
  {
    case (Gtk::RESPONSE_OK):
    {
      std::cout << "Open clicked." << std::endl;

      //Notice that this is a std::string, not a Glib::ustring.
      std::string fileuri = dialog.get_filename();
      std::string folderuri = dialog.get_current_folder() + "/";
      std::string filename;
      filename = fileuri.substr(folderuri.size(),
          fileuri.size() - folderuri.size() - 4);
      std::cout << "File selected: " << folderuri << "->" << filename
          << std::endl;
      mref_MapViewTreeLayer.addLayer((Glib::ustring) filename, folderuri,
          filename);
      break;
    }
    case (Gtk::RESPONSE_CANCEL):
    {
      std::cout << "Cancel clicked." << std::endl;
      break;
    }
    default:
    {
      std::cout << "Unexpected button clicked." << std::endl;
      break;
    }
  }
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onRemoveLayer()
{
  std::cout << "clicked 2" << std::endl;
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onSelect()
{
  if (mref_MapViewTreeLayer.getDrawLayer().getMove())
  {
    mp_ToggleButtonMove->set_active(false);
  }
  mref_MapViewTreeLayer.getDrawLayer().setSelect(
      !mref_MapViewTreeLayer.getDrawLayer().getSelect());
  mref_MapViewTreeLayer.getDrawLayer().setZoomFrame(false);
  mref_MapViewTreeLayer.getDrawLayer().setZoomCursor(false);
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onMove()
{
  if (mref_MapViewTreeLayer.getDrawLayer().getSelect())
  {
    mp_ToggleButtonSelect->set_active(false);
  }
  mref_MapViewTreeLayer.getDrawLayer().setMove(
      !mref_MapViewTreeLayer.getDrawLayer().getMove());
  mref_MapViewTreeLayer.getDrawLayer().setZoomFrame(false);
  mref_MapViewTreeLayer.getDrawLayer().setZoomCursor(false);
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onShow100()
{
  mref_MapViewTreeLayer.getDrawLayer().setInitTranslate(false);
  mref_MapViewTreeLayer.getDrawLayer().calulMaxMinCoordinate();
  mref_MapViewTreeLayer.getDrawLayer().setScale(mref_MapViewTreeLayer.getDrawLayer().getScaleOrigine());
  mref_MapViewTreeLayer.getDrawLayer().geton_expose_event();
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onInfo()
{
  std::cout << "clicked 6" << std::endl;
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onZoomCursor()
{
    if (mref_MapViewTreeLayer.getDrawLayer().getSelect())
      {
        mp_ToggleButtonSelect->set_active(false);
      }
      if (mref_MapViewTreeLayer.getDrawLayer().getMove())
        {
          mp_ToggleButtonMove->set_active(false);
        }
      Gdk::Cursor Cursor(Gdk::BOGOSITY);
      mref_MapViewTreeLayer.getDrawLayer().get_window()->set_cursor(Cursor);
      mref_MapViewTreeLayer.getDrawLayer().setZoomFrame(false);
      mref_MapViewTreeLayer.getDrawLayer().setUnzoomCursor(false);
      mref_MapViewTreeLayer.getDrawLayer().setZoomCursor(
          !mref_MapViewTreeLayer.getDrawLayer().getZoomCursor());

}

// =====================================================================
// =====================================================================

void MapViewToolBar::onZoomFrame()
{
  if (mref_MapViewTreeLayer.getDrawLayer().getSelect())
  {
    mp_ToggleButtonSelect->set_active(false);
  }
  if (mref_MapViewTreeLayer.getDrawLayer().getMove())
    {
      mp_ToggleButtonMove->set_active(false);
    }
  Gdk::Cursor Cursor(Gdk::SIZING);
  mref_MapViewTreeLayer.getDrawLayer().get_window()->set_cursor(Cursor);
  mref_MapViewTreeLayer.getDrawLayer().setZoomCursor(false);
  mref_MapViewTreeLayer.getDrawLayer().setUnzoomCursor(false);
  mref_MapViewTreeLayer.getDrawLayer().setZoomFrame(
      !mref_MapViewTreeLayer.getDrawLayer().getZoomFrame());
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onZoomObject()
{
  std::cout << "clicked 9" << std::endl;
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onUnzoomCursor()
{
      if (mref_MapViewTreeLayer.getDrawLayer().getSelect())
        {
          mp_ToggleButtonSelect->set_active(false);
        }
        if (mref_MapViewTreeLayer.getDrawLayer().getMove())
          {
            mp_ToggleButtonMove->set_active(false);
          }
        Gdk::Cursor Cursor(Gdk::BOX_SPIRAL);
        mref_MapViewTreeLayer.getDrawLayer().get_window()->set_cursor(Cursor);
        mref_MapViewTreeLayer.getDrawLayer().setZoomFrame(false);
        mref_MapViewTreeLayer.getDrawLayer().setZoomCursor(false);
        mref_MapViewTreeLayer.getDrawLayer().setUnzoomCursor(
            !mref_MapViewTreeLayer.getDrawLayer().getUnzoomCursor());
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onUnzoomObject()
{
  std::cout << "clicked 11" << std::endl;
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onChangeZoomCursor(MapViewAction& ToolBarAction)
{
  ToolBarAction.getZoomCursorAction()->connect_proxy(*mp_MenuButtonZoom);
  onZoomCursor();
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onChangeZoomFrame(MapViewAction& ToolBarAction)
{
  ToolBarAction.getZoomFrameAction()->connect_proxy(*mp_MenuButtonZoom);
  onZoomFrame();
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onChangeZoomObject(MapViewAction& ToolBarAction)
{
  ToolBarAction.getZoomObjectAction()->connect_proxy(*mp_MenuButtonZoom);
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onChangeUnzoomCursor(MapViewAction& ToolBarAction)
{
  ToolBarAction.getUnzoomCursorAction()->connect_proxy(*mp_MenuButtonUnzoom);
  onUnzoomCursor();
}

// =====================================================================
// =====================================================================

void MapViewToolBar::onChangeUnzoomObject(MapViewAction& ToolBarAction)
{
  ToolBarAction.getUnzoomObjectAction()->connect_proxy(*mp_MenuButtonUnzoom);
}

// =====================================================================
// =====================================================================

