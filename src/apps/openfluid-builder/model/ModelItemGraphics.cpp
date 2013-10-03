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
  \file ModelItemGraphics.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/base/ProjectManager.hpp>

#include "builderconfig.hpp"

#include "ModelItemGraphics.hpp"
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


#include <iostream>


QSize ModelItemGraphics::m_DefaultSize = QSize(200,70);

QPointF ModelItemGraphics::m_RequiredIOFromCenter = QPoint(-60,-ModelItemGraphics::m_DefaultSize.height()/2);
QPointF ModelItemGraphics::m_UsedIOFromCenter = QPoint(0,-ModelItemGraphics::m_DefaultSize.height()/2);
QPointF ModelItemGraphics::m_UpInIOFromCenter = QPoint(60,-ModelItemGraphics::m_DefaultSize.height()/2);
QPointF ModelItemGraphics::m_UpOutIOFromCenter = QPoint(30,ModelItemGraphics::m_DefaultSize.height()/2);



// =====================================================================
// =====================================================================


ModelItemGraphics::ModelItemGraphics(const QPointF& Coords, const QString& ID,
                                     QGraphicsItem* Parent):
  QGraphicsRectItem(Coords.x(),Coords.y(),m_DefaultSize.width(),m_DefaultSize.height(),Parent),
  m_ID(ID), m_Initialized(false)
{
  setPen(QPen(QBrush(QColor(BUILDER_MODELVIEW_BORDERCOLOR)),2));

  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

  setCursor(QCursor(Qt::OpenHandCursor));


  // Model item ID
  QGraphicsSimpleTextItem* IDGraphics = new QGraphicsSimpleTextItem(ID,Parent);
  IDGraphics->setParentItem(this);

  QFont TmpFont = IDGraphics->font();
  TmpFont.setPointSize(10);
  IDGraphics->setFont(TmpFont);

  QRectF IDRect = IDGraphics->boundingRect();

  if (rect().width() < IDRect.width()+20)
  setRect(Coords.x(),Coords.y(),IDRect.width()+20,m_DefaultSize.height());

  IDGraphics->setPos((rect().width()/2)-(IDRect.width()/2),(m_DefaultSize.height()/2)-(IDRect.height()/2));
}


// =====================================================================
// =====================================================================


ModelItemGraphics::~ModelItemGraphics()
{
//  removeConnectors();
}


// =====================================================================
// =====================================================================



QVariant ModelItemGraphics::itemChange(GraphicsItemChange Change,
                                       const QVariant &Value)
{
  if (m_Initialized && Change == QGraphicsItem::ItemPositionChange)
  {
    openfluid::base::ProjectManager::getInstance()->setConfigValue("builder.model.graphicalview",m_ID,pos().toPoint());

    foreach (ConnectorGraphics* Conn, m_Connectors)
    {
      Conn->updatePosition();
    }
  }

  return QGraphicsRectItem::itemChange(Change, Value);
}


// =====================================================================
// =====================================================================


void ModelItemGraphics::mousePressEvent(QGraphicsSceneMouseEvent *Event)
{
  setCursor(QCursor(Qt::ClosedHandCursor));

  QGraphicsItem::mousePressEvent(Event);
}


// =====================================================================
// =====================================================================


void ModelItemGraphics::mouseReleaseEvent(QGraphicsSceneMouseEvent *Event)
{
  setCursor(QCursor(Qt::OpenHandCursor));

  QGraphicsItem::mouseReleaseEvent(Event);
}


// =====================================================================
// =====================================================================


void ModelItemGraphics::initialize()
{
  m_Initialized = true;
}


// =====================================================================
// =====================================================================


QPointF ModelItemGraphics::getCenterFromOrigin()
{
  return QPointF(rect().width()/2,rect().height()/2);
}


// =====================================================================
// =====================================================================


void ModelItemGraphics::drawIOSlot(const QPointF& Pos, const QString& Name, bool Active)
{

  QGraphicsEllipseItem* SlotShape = new QGraphicsEllipseItem(Pos.x()-6,Pos.y()-6,12,12);

  QGraphicsSimpleTextItem* TextShape = new QGraphicsSimpleTextItem(Name);

  QFont TmpFont = TextShape->font();
  TmpFont.setPointSize(8);
  TextShape->setFont(TmpFont);

  QRectF TextRect = TextShape->boundingRect();

  qreal TextShapeX = Pos.x() - (TextRect.width()/2);
  qreal TextShapeY = Pos.y() + 15 - (TextRect.height()/2);

  if (Pos.y() > getCenterFromOrigin().y())
    TextShapeY = Pos.y() - 15 - (TextRect.height()/2);

  TextShape->setPos(TextShapeX,TextShapeY);


  if (Active)
  {
    SlotShape->setPen(QPen(QBrush(QColor(BUILDER_MODELVIEW_ACTIVECOLOR)),1));
    SlotShape->setBrush(QBrush(QColor(BUILDER_MODELVIEW_ACTIVECOLOR)));
    TextShape->setBrush(QBrush(QColor(BUILDER_MODELVIEW_ACTIVECOLOR)));
  }
  else
  {
    SlotShape->setPen(QPen(QBrush(QColor(BUILDER_MODELVIEW_INACTIVECOLOR)),1));
    SlotShape->setBrush(QBrush(QColor(BUILDER_MODELVIEW_INACTIVECOLOR)));
    TextShape->setBrush(QBrush(QColor(BUILDER_MODELVIEW_INACTIVECOLOR)));
  }

  SlotShape->setParentItem(this);
  TextShape->setParentItem(this);



}


// =====================================================================
// =====================================================================


QPointF ModelItemGraphics::getRequiredIOPosition()
{
  return scenePos()+getCenterFromOrigin()+m_RequiredIOFromCenter;
}


// =====================================================================
// =====================================================================


QPointF ModelItemGraphics::getUsedIOPosition()
{
  return scenePos()+getCenterFromOrigin()+m_UsedIOFromCenter;
}


// =====================================================================
// =====================================================================


QPointF ModelItemGraphics::getUpInIOPosition()
{
  return scenePos()+getCenterFromOrigin()+m_UpInIOFromCenter;
}


// =====================================================================
// =====================================================================


QPointF ModelItemGraphics::getUpOutIOPosition()
{
  return scenePos()+getCenterFromOrigin()+m_UpOutIOFromCenter;
}


// =====================================================================
// =====================================================================


void ModelItemGraphics::addConnector(ConnectorGraphics* Connector)
{
  m_Connectors.append(Connector);
}


// =====================================================================
// =====================================================================

/*
void ModelItemGraphics::removeConnector(ConnectorGraphics* Connector)
{
  m_Connectors.removeAll(Connector);
}


// =====================================================================
// =====================================================================


void ModelItemGraphics::removeConnectors()
{
  foreach (ConnectorGraphics* Connector, m_Connectors)
  {
    Connector->getFromItem()->removeConnector(Connector);
    Connector->getToItem()->removeConnector(Connector);
    scene()->removeItem(Connector);
    delete Connector;
  }
}
*/

// =====================================================================
// =====================================================================


bool ModelItemGraphics::hasProducedVar(const QString& UnitClass, const QString& Name)
{
  return (m_ProducedVars.find(UnitClass)!=m_ProducedVars.end() && m_ProducedVars[UnitClass].contains(Name));
}


// =====================================================================
// =====================================================================


bool ModelItemGraphics::hasUpdatedVar(const QString& UnitClass, const QString& Name)
{
  return (m_UpdatedVars.find(UnitClass)!=m_UpdatedVars.end() && m_UpdatedVars[UnitClass].contains(Name));
}
