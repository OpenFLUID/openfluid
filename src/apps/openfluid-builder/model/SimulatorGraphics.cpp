/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file SimulatorGraphics.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QBrush>

#include "builderconfig.hpp"
#include "SimulatorGraphics.hpp"


SimulatorGraphics::SimulatorGraphics(
  const QPointF& Coords,
  const QString& ID, unsigned int Order,
  const openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature>& Container, 
  const QColor& BGColor, const QColor& BorderColor,
  QGraphicsItem* Parent):
  ModelItemGraphics(Coords,ID,ID,Order,BorderColor,Parent)
{

  m_Ghost = Container.isGhost();

  m_RequiredVars = Container.signature()->SimulatorHandledData.RequiredVars;
  m_UsedVars = Container.signature()->SimulatorHandledData.UsedVars;
  m_ProducedVars = Container.signature()->SimulatorHandledData.ProducedVars;
  m_UpdatedVars = Container.signature()->SimulatorHandledData.UpdatedVars;

  // In/Out slots
  drawIOSlot(getRequiredIOPosition(),SlotType::SLOT_REQ,m_RequiredVars);
  drawIOSlot(getUsedIOPosition(),SlotType::SLOT_US,m_UsedVars);
  drawIOSlot(getUpInIOPosition(),SlotType::SLOT_UPIN,m_UpdatedVars);
  drawIOSlot(getProducedIOPosition(),SlotType::SLOT_PROD,m_ProducedVars);
  drawIOSlot(getUpOutIOPosition(),SlotType::SLOT_UPOUT,m_UpdatedVars);

  QColor EffectiveBGColor;
  if (BGColor.isValid())
  {
    EffectiveBGColor = BGColor;
  }
  else
  {
    if (!m_Ghost)
    {
      EffectiveBGColor = QColor(BUILDER_SIMULATOR_BGCOLOR);
    }
    else
    {
      EffectiveBGColor = QColor(BUILDER_GHOST_BGCOLOR);
    }
  }
  setBrush(QBrush(EffectiveBGColor));
  updateFontColor(EffectiveBGColor);
  

}


// =====================================================================
// =====================================================================


SimulatorGraphics::~SimulatorGraphics()
{

}


// =====================================================================
// =====================================================================


QPointF SimulatorGraphics::getProducedIOPosition()
{
  return scenePos()+getCenterFromOrigin()+QPoint(-30,m_DefaultSize.height()/2);
}


// =====================================================================
// =====================================================================


void SimulatorGraphics::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* /*Event*/)
{
  if (m_Ghost)
  {
    emit srcEditAsked(m_ID,m_Ghost);
  }
#ifdef ENABLE_WARESDEV_INTEGRATION
  else
  {
    emit srcEditAsked(m_ID,m_Ghost);
  }
#endif

  // FIXME The line below causes the application to crash
  //QGraphicsItem::mousePressEvent(Event);
}

