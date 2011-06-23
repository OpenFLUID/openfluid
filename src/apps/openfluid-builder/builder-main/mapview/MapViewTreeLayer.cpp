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
 \file MapViewTreeLayer.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "MapViewTreeLayer.hpp"

MapViewTreeLayer::MapViewTreeLayer(MapViewDrawingArea& DrawLayer) :
  mref_DrawLayer(DrawLayer)
{
  m_NumLayer = 0;
  mp_MainVBox = Gtk::manage(new Gtk::VBox());
  mp_VBoxBackgroundResultLayer = Gtk::manage(new Gtk::VBox());
  mp_VBoxLayer = Gtk::manage(new Gtk::VBox());
  mp_ScrolledWindowLayer = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ViewportLayer = Gtk::manage(
      new Gtk::Viewport(*Gtk::manage(new Gtk::Adjustment(0, 0, 1)),
          *Gtk::manage(new Gtk::Adjustment(0, 0, 1))));

  mp_ResultLayer = new MapViewTreeLayerObjectResult("Result", -1);
  mp_BackgroundLayer = new MapViewTreeLayerObjectBackground("Background", -2);

  Gtk::HSeparator* seperator = Gtk::manage(new Gtk::HSeparator());

  mp_VBoxBackgroundResultLayer->pack_start(*mp_ResultLayer->asWidget(),
      Gtk::PACK_SHRINK);
  mp_VBoxBackgroundResultLayer->pack_start(*seperator, Gtk::PACK_SHRINK);
  mp_VBoxBackgroundResultLayer->pack_start(*mp_BackgroundLayer->asWidget(),
      Gtk::PACK_SHRINK);
  mp_ScrolledWindowLayer->set_shadow_type(Gtk::SHADOW_NONE);

  mp_ViewportLayer->add(*mp_VBoxLayer);
  mp_ScrolledWindowLayer->add(*mp_ViewportLayer);
  mp_MainVBox->pack_start(*mp_ScrolledWindowLayer, Gtk::PACK_EXPAND_WIDGET);
  mp_MainVBox->pack_end(*mp_VBoxBackgroundResultLayer, Gtk::PACK_SHRINK);
  seperator = Gtk::manage(new Gtk::HSeparator());
  mp_MainVBox->pack_end(*seperator, Gtk::PACK_SHRINK);
  seperator = Gtk::manage(new Gtk::HSeparator());
  mp_MainVBox->pack_end(*seperator, Gtk::PACK_SHRINK);
  seperator = Gtk::manage(new Gtk::HSeparator());
  mp_MainVBox->pack_end(*seperator, Gtk::PACK_SHRINK);

}

// =====================================================================
// =====================================================================

MapViewTreeLayer::~MapViewTreeLayer()
{

}

// =====================================================================
// =====================================================================

Gtk::Widget * MapViewTreeLayer::asWidget()
{
  return mp_MainVBox;
}

// =====================================================================
// =====================================================================

void MapViewTreeLayer::addLayer(Glib::ustring label, std::string fileuri,
    std::string filename)
{
  mref_DrawLayer.addLayer(fileuri, filename);
  MapViewTreeLayerObjectBase* Layer = new MapViewTreeLayerObjectBase(label,
      m_NumLayer, mref_DrawLayer.getICLayer().at(m_NumLayer)->gettype());
  Layer->signalUpLayer().connect(
      sigc::mem_fun(*this, &MapViewTreeLayer::onUpLayer));
  Layer->signalDownLayer().connect(
      sigc::mem_fun(*this, &MapViewTreeLayer::onDownLayer));
  Layer->signalRemoveLayer().connect(
      sigc::mem_fun(*this, &MapViewTreeLayer::onRemoveLayer));
  Layer->getMapViewTreeLayerObjectBaseExpander()->signalColor().connect(
      sigc::mem_fun(*this, &MapViewTreeLayer::onColorLayer));
  Layer->signalDisplay().connect(
      sigc::mem_fun(*this, &MapViewTreeLayer::onIsDisplay));

  Layer->getMapViewTreeLayerObjectBaseExpander()->onColor();

  mref_DrawLayer.setNumLayer(mref_DrawLayer.getNumLayer() + 1);
  m_NumLayer += 1;

  m_ObjectLayer.push_back(Layer);

  if (Layer->getPosition() != 0)
  {
    m_ObjectLayer.at(Layer->getPosition() - 1)->getDownLayerAction()->set_sensitive(
        true);
    m_ObjectLayer.at(Layer->getPosition())->getDownLayerAction()->set_sensitive(
        false);
  }

  mp_VBoxLayer->pack_start(*Layer->asWidget(), Gtk::PACK_SHRINK);
  mp_VBoxLayer->show_all_children(true);
}

// =====================================================================
// =====================================================================

void MapViewTreeLayer::deleteLayer(int Position)
{
  int positionCourante = 0;
  int positionSup = 0;
  int positionInf = 0;
  for (int i = 0; i < m_ObjectLayer.size(); i++)
  {
    if (m_ObjectLayer.at(i)->getPosition() == Position)
      positionCourante = i;
    if (m_ObjectLayer.at(i)->getPosition() == (Position + 1))
      positionSup = i;
    if (m_ObjectLayer.at(i)->getPosition() == (Position - 1))
      positionInf = i;
  }

  if (positionCourante == 0)
  {
    m_ObjectLayer.at(positionSup)->getUpLayerAction()->set_sensitive(false);
  }
  if (positionCourante == m_NumLayer - 1)
  {
    m_ObjectLayer.at(positionInf)->getDownLayerAction()->set_sensitive(false);
  }

  mp_VBoxLayer->remove(*m_ObjectLayer.at(positionCourante)->asWidget());
  mref_DrawLayer.removeICLayer(positionCourante);
  m_ObjectLayer.erase(m_ObjectLayer.begin() + positionCourante);

  for (int i = 0; i < m_ObjectLayer.size(); i++)
  {
    if (m_ObjectLayer.at(i)->getPosition() > Position)
    {
      m_ObjectLayer.at(i)->setPosition(m_ObjectLayer.at(i)->getPosition() - 1);
      m_ObjectLayer.at(i)->getMapViewTreeLayerObjectBaseExpander()->setPosition(
          m_ObjectLayer.at(i)->getMapViewTreeLayerObjectBaseExpander()->getPosition()
              - 1);
      mref_DrawLayer.getICLayer().at(i)->setPosition(
          mref_DrawLayer.getICLayer().at(i)->getPosition() - 1);
    }
  }
  m_NumLayer -= 1;
  mref_DrawLayer.setNumLayer(mref_DrawLayer.getNumLayer() - 1);
}

// =====================================================================
// =====================================================================

void MapViewTreeLayer::onUpLayer(int Position)
{
  int positionCourante = 0;
  int positionInf = 0;
  for (int i = 0; i < m_ObjectLayer.size(); i++)
  {
    if (m_ObjectLayer.at(i)->getPosition() == Position)
      positionCourante = i;
    if (m_ObjectLayer.at(i)->getPosition() == (Position - 1))
      positionInf = i;
  }

  mp_VBoxLayer->reorder_child(*m_ObjectLayer.at(positionCourante)->asWidget(),
      Position - 1);

  m_ObjectLayer.at(positionCourante)->setPosition(Position - 1);
  m_ObjectLayer.at(positionInf)->setPosition(Position);

  m_ObjectLayer.at(positionCourante)->getMapViewTreeLayerObjectBaseExpander()->setPosition(
      Position - 1);
  m_ObjectLayer.at(positionInf)->getMapViewTreeLayerObjectBaseExpander()->setPosition(
      Position);

  mref_DrawLayer.getICLayer().at(positionCourante)->setPosition(Position - 1);
  mref_DrawLayer.getICLayer().at(positionInf)->setPosition(Position);

  if (m_ObjectLayer.at(positionCourante)->getPosition() == 0)
  {
    m_ObjectLayer.at(positionInf)->getUpLayerAction()->set_sensitive(true);
    m_ObjectLayer.at(positionCourante)->getUpLayerAction()->set_sensitive(false);
  }
  if (m_ObjectLayer.at(positionInf)->getPosition()
      == (m_ObjectLayer.size() - 1))
  {
    m_ObjectLayer.at(positionCourante)->getDownLayerAction()->set_sensitive(
        true);
    m_ObjectLayer.at(positionInf)->getDownLayerAction()->set_sensitive(false);
  }
  mref_DrawLayer.geton_expose_event();
}

// =====================================================================
// =====================================================================

void MapViewTreeLayer::onDownLayer(int Position)
{
  int positionCourante = 0;
  int positionSup = 0;
  for (int i = 0; i < m_ObjectLayer.size(); i++)
  {
    if (m_ObjectLayer.at(i)->getPosition() == Position)
      positionCourante = i;
    if (m_ObjectLayer.at(i)->getPosition() == (Position + 1))
      positionSup = i;
  }

  mp_VBoxLayer->reorder_child(*m_ObjectLayer.at(positionCourante)->asWidget(),
      Position + 1);

  m_ObjectLayer.at(positionCourante)->setPosition(Position + 1);
  m_ObjectLayer.at(positionSup)->setPosition(Position);

  m_ObjectLayer.at(positionCourante)->getMapViewTreeLayerObjectBaseExpander()->setPosition(
      Position + 1);
  m_ObjectLayer.at(positionSup)->getMapViewTreeLayerObjectBaseExpander()->setPosition(
      Position);

  mref_DrawLayer.getICLayer().at(positionCourante)->setPosition(Position + 1);
  mref_DrawLayer.getICLayer().at(positionSup)->setPosition(Position);

  if (m_ObjectLayer.at(positionCourante)->getPosition()
      == (m_ObjectLayer.size() - 1))
  {
    m_ObjectLayer.at(positionSup)->getDownLayerAction()->set_sensitive(true);
    m_ObjectLayer.at(positionCourante)->getDownLayerAction()->set_sensitive(
        false);
  }

  if (m_ObjectLayer.at(positionSup)->getPosition() == 0)
  {
    m_ObjectLayer.at(positionCourante)->getUpLayerAction()->set_sensitive(true);
    m_ObjectLayer.at(positionSup)->getUpLayerAction()->set_sensitive(false);
  }
  mref_DrawLayer.geton_expose_event();
}

// =====================================================================
// =====================================================================

void MapViewTreeLayer::onRemoveLayer(int Position)
{
  deleteLayer(Position);
  mref_DrawLayer.geton_expose_event();
}

// =====================================================================
// =====================================================================


void MapViewTreeLayer::onColorLayer(int Id, int Position, int SizeLine,
    double Red, double Green, double Blue, double Alpha)
{
  for (int i = 0; i < mref_DrawLayer.getICLayer().size(); i++)
  {
    if (mref_DrawLayer.getICLayer().at(i)->getPosition() == Position)
    {
      mref_DrawLayer.getICLayer().at(i)->setId(Id);
      mref_DrawLayer.getICLayer().at(i)->setSizeLine(SizeLine);
      mref_DrawLayer.getICLayer().at(i)->setPosition(Position);
      mref_DrawLayer.getICLayer().at(i)->setRed(Red);
      mref_DrawLayer.getICLayer().at(i)->setGreen(Green);
      mref_DrawLayer.getICLayer().at(i)->setBlue(Blue);
      mref_DrawLayer.getICLayer().at(i)->setAlpha(Alpha);
      mref_DrawLayer.geton_expose_event();
    }
  }

}

// =====================================================================
// =====================================================================


void MapViewTreeLayer::onIsDisplay(int Position, bool IsDisplay)
{
  for (int i = 0; i < mref_DrawLayer.getICLayer().size(); i++)
  {
    if (mref_DrawLayer.getICLayer().at(i)->getPosition() == Position)
    {
      mref_DrawLayer.getICLayer().at(i)->setIsDisplay(IsDisplay);
      mref_DrawLayer.geton_expose_event();
    }
  }
}

// =====================================================================
// =====================================================================



MapViewDrawingArea& MapViewTreeLayer::getDrawLayer()
{
  return mref_DrawLayer;
}

// =====================================================================
// =====================================================================
