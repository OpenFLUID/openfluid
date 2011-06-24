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
 \file MapViewTreeLayerObjectBase.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "MapViewTreeLayerObjectBase.hpp"
#include "BuilderGraphicsHelper.hpp"

MapViewTreeLayerObjectBase::MapViewTreeLayerObjectBase(
    const Glib::ustring & Label, int IdPositionInit, int Type) :
  MapViewTreeLayerObject(Label, IdPositionInit)
{
  m_Position = IdPositionInit;
  mp_VBoxButtonUpDown = Gtk::manage(new Gtk::VBox());

  mp_MapViewTreeLayerObjectBaseExpander
      = new MapViewTreeLayerObjectBaseExpander();
  mp_MapViewTreeLayerObjectBaseExpander->setId(getId());
  mp_MapViewTreeLayerObjectBaseExpander->setPosition(getPosition());
  Glib::RefPtr<Gdk::Pixbuf> im;

  switch (Type)
  {
    case 1:
      im = Gdk::Pixbuf::create_from_file(BuilderGraphicsHelper::getPathForFileName("points.png"),22,22,true);
      break;
    case 2:
      im = Gdk::Pixbuf::create_from_file(BuilderGraphicsHelper::getPathForFileName("lines.png"),22,22,true);
      break;
    case 3:
      im = Gdk::Pixbuf::create_from_file(BuilderGraphicsHelper::getPathForFileName("polygons.png"),22,22,true);
      break;
    default:
      break;
  }

  mp_ImageType = Gtk::manage(new Gtk::Image(im));
  Gtk::Toolbar* tup = Gtk::manage(new Gtk::Toolbar());
  Gtk::Toolbar* tdown = Gtk::manage(new Gtk::Toolbar());

  tup->set_icon_size(Gtk::ICON_SIZE_MENU);
  tdown->set_icon_size(Gtk::ICON_SIZE_MENU);

  tup->set_show_arrow(false);
  tdown->set_show_arrow(false);

  sigc::signal<void, int> signalUpLayer;
  sigc::signal<void, int> signalDownLayer;

  mref_UpLayer = Gtk::Action::create("UpLayer", Gtk::Stock::GO_UP,
      _("Up Layer"), _("Go up the layer"));
  mref_DownLayer = Gtk::Action::create("DownLayer", Gtk::Stock::GO_DOWN,
      _("Down Layer"), _("Go down the layer"));
  mref_RemoveLayer = Gtk::Action::create("RemoveLayer", Gtk::Stock::REMOVE,
      _("Remove Layer"), _("Remove the layer"));

  mp_CheckButton->signal_toggled().connect(
      sigc::mem_fun(*this, &MapViewTreeLayerObjectBase::onIsDisplay));

  mp_ButtonUp = Gtk::manage(new Gtk::ToolItem());
  mp_ButtonDown = Gtk::manage(new Gtk::ToolItem());
  mp_ButtonRemove = Gtk::manage(new Gtk::ToolItem());

  mp_ButtonUp = mref_UpLayer->create_tool_item();
  mp_ButtonDown = mref_DownLayer->create_tool_item();
  mp_ButtonRemove = mref_RemoveLayer->create_tool_item();

  if (m_Position == 0)
  {
    mref_UpLayer->set_sensitive(false);
    mref_DownLayer->set_sensitive(false);
  }
  tup->append(*mp_ButtonUp);
  tdown->append(*mp_ButtonDown);
  mp_VBoxButtonUpDown->pack_start(*tup, Gtk::PACK_SHRINK);
  mp_VBoxButtonUpDown->pack_end(*tdown, Gtk::PACK_SHRINK);

  mref_UpLayer->signal_activate().connect(
      sigc::mem_fun(*this, &MapViewTreeLayerObjectBase::onUpLayer));
  mref_DownLayer->signal_activate().connect(
      sigc::mem_fun(*this, &MapViewTreeLayerObjectBase::onDownLayer));
  mref_RemoveLayer->signal_activate().connect(
      sigc::mem_fun(*this, &MapViewTreeLayerObjectBase::onRemoveLayer));

  mp_ViewportExpander->add(*mp_MapViewTreeLayerObjectBaseExpander->asWidget());
  mp_MainTable->attach(*mp_ImageType, 2, 3, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  mp_MainTable->attach(*mp_ButtonRemove, 3, 4, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
  mp_MainTable->attach(*mp_VBoxButtonUpDown, 4, 5, 0, 1, Gtk::SHRINK,
      Gtk::SHRINK);

}

// =====================================================================
// =====================================================================

MapViewTreeLayerObjectBase::~MapViewTreeLayerObjectBase()
{

}

// =====================================================================
// =====================================================================

int MapViewTreeLayerObjectBase::getPosition()
{
  return m_Position;
}

// =====================================================================
// =====================================================================

Gtk::ToolItem* MapViewTreeLayerObjectBase::getButtonUp()
{
  return mp_ButtonUp;
}

// =====================================================================
// =====================================================================

Gtk::ToolItem* MapViewTreeLayerObjectBase::getButtonDown()
{
  return mp_ButtonDown;
}

// =====================================================================
// =====================================================================

void MapViewTreeLayerObjectBase::setPosition(int Position)
{
  m_Position = Position;
}

// =====================================================================
// =====================================================================

void MapViewTreeLayerObjectBase::onUpLayer()
{
  msigc_signalUpLayer.emit(m_Position);
}

// =====================================================================
// =====================================================================

void MapViewTreeLayerObjectBase::onDownLayer()
{
  msigc_signalDownLayer.emit(m_Position);
}

// =====================================================================
// =====================================================================

void MapViewTreeLayerObjectBase::onRemoveLayer()
{
  msigc_signalRemoveLayer.emit(m_Position);
}

// =====================================================================
// =====================================================================

MapViewTreeLayerObjectBase::mtype_Signal MapViewTreeLayerObjectBase::signalUpLayer()
{
  return msigc_signalUpLayer;
}

// =====================================================================
// =====================================================================

MapViewTreeLayerObjectBase::mtype_Signal MapViewTreeLayerObjectBase::signalDownLayer()
{
  return msigc_signalDownLayer;
}

// =====================================================================
// =====================================================================

MapViewTreeLayerObjectBase::mtype_Signal MapViewTreeLayerObjectBase::signalRemoveLayer()
{
  return msigc_signalRemoveLayer;
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> MapViewTreeLayerObjectBase::getUpLayerAction()
{
  return mref_UpLayer;
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> MapViewTreeLayerObjectBase::getDownLayerAction()
{
  return mref_DownLayer;
}

// =====================================================================
// =====================================================================

Glib::RefPtr<Gtk::Action> MapViewTreeLayerObjectBase::getRemoveLayerAction()
{
  return mref_RemoveLayer;
}

// =====================================================================
// =====================================================================


MapViewTreeLayerObjectBaseExpander* MapViewTreeLayerObjectBase::getMapViewTreeLayerObjectBaseExpander()
{
  return mp_MapViewTreeLayerObjectBaseExpander;
}

// =====================================================================
// =====================================================================


MapViewTreeLayerObjectBase::mtype_SignalDisplay MapViewTreeLayerObjectBase::signalDisplay()
{
  return msigc_signalDisplay;
}

// =====================================================================
// =====================================================================

void MapViewTreeLayerObjectBase::onIsDisplay()
{
  m_IsDisplay = mp_CheckButton->get_active();
  msigc_signalDisplay.emit(m_Position, m_IsDisplay);
}

// =====================================================================
// =====================================================================
