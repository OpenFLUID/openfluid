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
  @file ModelItemGraphics.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/base/RunContextManager.hpp>

#include "builderconfig.hpp"

#include "ModelItemGraphics.hpp"
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


QSize ModelItemGraphics::m_DefaultSize = QSize(200,80);

QPointF ModelItemGraphics::m_RequiredIOFromCenter = QPoint(-60,-ModelItemGraphics::m_DefaultSize.height()/2);
QPointF ModelItemGraphics::m_UsedIOFromCenter = QPoint(0,-ModelItemGraphics::m_DefaultSize.height()/2);
QPointF ModelItemGraphics::m_UpInIOFromCenter = QPoint(60,-ModelItemGraphics::m_DefaultSize.height()/2);
QPointF ModelItemGraphics::m_UpOutIOFromCenter = QPoint(30,ModelItemGraphics::m_DefaultSize.height()/2);



// =====================================================================
// =====================================================================


ModelItemGraphics::ModelItemGraphics(const QPointF& Coords,
                                     const QString& ID, unsigned int Order,
                                     QGraphicsItem* Parent):
  QGraphicsRectItem(Coords.x(),Coords.y(),m_DefaultSize.width(),m_DefaultSize.height(),Parent),
  m_ID(ID), m_Ghost(false), m_Initialized(false)
{
  setPen(QPen(QBrush(QColor(BUILDER_MODELVIEW_BORDERCOLOR)),2));

  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

  setCursor(QCursor(Qt::OpenHandCursor));


  // Model item ID

  QGraphicsTextItem* IDGraphics = new QGraphicsTextItem(ID,Parent);
  IDGraphics->setParentItem(this);

  QFont TmpFont = IDGraphics->font();
  TmpFont.setPointSize(10);
  TmpFont.setWeight(QFont::Bold);
  IDGraphics->setFont(TmpFont);

  QRectF TmpRect = IDGraphics->boundingRect();

  if (rect().width() < TmpRect.width()+20)
  setRect(Coords.x(),Coords.y(),TmpRect.width()+20,m_DefaultSize.height());

  IDGraphics->setPos((rect().width()/2)-(TmpRect.width()/2),(m_DefaultSize.height()/2)-(TmpRect.height()/2));


  // Model item Order

  QGraphicsTextItem* OrderGraphics = new QGraphicsTextItem(QString("#%1").arg(Order),Parent);
  OrderGraphics->setParentItem(this);

  TmpFont = OrderGraphics->font();
  TmpFont.setPointSize(9);
  TmpFont.setStyle(QFont::StyleItalic);
  OrderGraphics->setFont(TmpFont);

  TmpRect = OrderGraphics->boundingRect();

  OrderGraphics->setPos(rect().width()-TmpRect.width()-2,m_DefaultSize.height()-TmpRect.height());

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
    openfluid::base::RunContextManager::instance()
      ->setProjectConfigValue("builder.model.graphicalview",m_ID,pos().toPoint());

    for (ConnectorGraphics* Conn : m_Connectors)
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
  for (ConnectorGraphics* Connector : m_Connectors)
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


bool ModelItemGraphics::hasProducedVariable(const QString& UnitClass, const QString& Name)
{
  return (m_ProducedVars.find(UnitClass)!=m_ProducedVars.end() && m_ProducedVars[UnitClass].contains(Name));
}


// =====================================================================
// =====================================================================


bool ModelItemGraphics::hasUpdatedVariable(const QString& UnitClass, const QString& Name)
{
  return (m_UpdatedVars.find(UnitClass)!=m_UpdatedVars.end() && m_UpdatedVars[UnitClass].contains(Name));
}
