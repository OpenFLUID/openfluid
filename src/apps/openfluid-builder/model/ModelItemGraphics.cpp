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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/tools/QtHelpers.hpp>

#include "builderconfig.hpp"
#include "ModelItemGraphics.hpp"


QSize ModelItemGraphics::m_DefaultSize = QSize(200,80);

QPointF ModelItemGraphics::m_RequiredIOFromCenter = QPoint(-60,-ModelItemGraphics::m_DefaultSize.height()/2);
QPointF ModelItemGraphics::m_UsedIOFromCenter = QPoint(0,-ModelItemGraphics::m_DefaultSize.height()/2);
QPointF ModelItemGraphics::m_UpInIOFromCenter = QPoint(60,-ModelItemGraphics::m_DefaultSize.height()/2);
QPointF ModelItemGraphics::m_UpOutIOFromCenter = QPoint(30,ModelItemGraphics::m_DefaultSize.height()/2);


// =====================================================================
// =====================================================================


ModelItemGraphics::ModelItemGraphics(const QPointF& Coords,
                                     const QString& ID,
                                     const QString& DisplayedTitle,
                                     unsigned int Order,
                                     const QColor& BorderColor,
                                     QGraphicsItem* Parent):
  QGraphicsRectItem(Coords.x(),Coords.y(),m_DefaultSize.width(),m_DefaultSize.height(),Parent),
  m_ID(ID), m_Ghost(false), m_Initialized(false)
{
  if (BorderColor.isValid())
  {
    setPen(QPen(QBrush(BorderColor),2));
  }
  else
  {
    setPen(QPen(QBrush(QColor(BUILDER_MODELVIEW_BORDERCOLOR)),2));
  }

  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

  setCursor(QCursor(Qt::OpenHandCursor));


  // Model item ID

  QGraphicsTextItem* TitleGraphics = new QGraphicsTextItem(DisplayedTitle,Parent);
  TitleGraphics->setParentItem(this);

  QFont TmpFont = TitleGraphics->font();
  TmpFont.setPointSize(10);
  TmpFont.setWeight(QFont::Bold);
  TitleGraphics->setFont(TmpFont);

  QRectF TmpRect = TitleGraphics->boundingRect();

  if (rect().width() < TmpRect.width()+20)
  {
    setRect(Coords.x(),Coords.y(),TmpRect.width()+20,m_DefaultSize.height());
  }

  TitleGraphics->setPos((rect().width()/2)-(TmpRect.width()/2),(m_DefaultSize.height()/2)-(TmpRect.height()/2));


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


void ModelItemGraphics::updateFontColor(const QColor& BGColor)
{
  // Adjust color to background: black or white depending on luminance of color
  QColor FontColor;
  double Luminance = openfluid::ui::common::computeLuminance(BGColor);
  if (Luminance > 0.5)
  {
    FontColor = Qt::black;
  }
  else
  {
    FontColor = Qt::white;
  }
  
  for (auto& Item : childItems())
  {
    if (Item->type() == 8) //8: QGraphicsTextItem
    {
      ((QGraphicsTextItem*)(Item))->setDefaultTextColor(FontColor);
    }
    if (Item->type() == 9) //9: QGraphicsSimpleTextItem
    {
      ((QGraphicsSimpleTextItem*)(Item))->setBrush(QBrush(FontColor));
    }
  }
}


// =====================================================================
// =====================================================================


QVariant ModelItemGraphics::itemChange(GraphicsItemChange Change,
                                       const QVariant &Value)
{
  if (m_Initialized && Change == QGraphicsItem::ItemPositionChange)
  {
    openfluid::base::RunContextManager::instance()
      ->setProjectContextValue("/builder/model/graphicalview/positions",m_ID.toStdString(),
                                openfluid::tools::fromQPoint(pos().toPoint()));

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


void ModelItemGraphics::drawIOSlot(const QPointF& Pos, const SlotType& Type,
                                   const IOSet_t& VarsInfos)
{

  QString SlotName;

  if (Type == SlotType::SLOT_REQ)
  {
    SlotName = "Req";
  }
  else if (Type == SlotType::SLOT_US)
  {
    SlotName = "Us";
  }
  else if (Type == SlotType::SLOT_PROD)
  {
    SlotName = "Prod";
  }
  else
  {
    SlotName = "Upd";
  }

  QGraphicsEllipseItem* SlotShape = new QGraphicsEllipseItem(Pos.x()-6,Pos.y()-6,12,12);

  QGraphicsSimpleTextItem* TextShape = new QGraphicsSimpleTextItem(SlotName);

  QFont TmpFont = TextShape->font();
  TmpFont.setPointSize(8);
  TextShape->setFont(TmpFont);

  QRectF TextRect = TextShape->boundingRect();

  qreal TextShapeX = Pos.x() - (TextRect.width()/2);
  qreal TextShapeY = Pos.y() + 15 - (TextRect.height()/2);

  if (Pos.y() > getCenterFromOrigin().y())
  {
    TextShapeY = Pos.y() - 15 - (TextRect.height()/2);
  }

  TextShape->setPos(TextShapeX,TextShapeY);


  if (!VarsInfos.empty())
  {
    QString ToolTipString = "<h3>";

    if (Type == SlotType::SLOT_REQ)
    {
      ToolTipString += tr("Required variable(s)");
    }
    else if (Type == SlotType::SLOT_US)
    {
      ToolTipString += tr("Used variable(s)");
    }
    else if (Type == SlotType::SLOT_PROD)
    {
      ToolTipString += tr("Produced variable(s)");
    }
    else
    {
      ToolTipString += tr("Updated variable(s)");
    }

    ToolTipString += "</h3><ul>";

    // Build of the tooltip text
    for (auto& VarInfos : VarsInfos)
    {
      ToolTipString += "<li>";
      ToolTipString += "<p style='white-space:pre'><b>" + QString::fromStdString(VarInfos.Name) + "</b>" +
                       " {"+QString::fromStdString(VarInfos.UnitsClass)+ "}";

      if (VarInfos.DataType != openfluid::core::Value::NONE)
      {
        ToolTipString += "," +
                         QString::fromStdString(openfluid::core::Value::getStringFromValueType(VarInfos.DataType));
      }

      if (!VarInfos.Description.empty())
      {
        ToolTipString += ": " + QString::fromStdString(VarInfos.Description);
      }

      if (!VarInfos.SIUnit.empty())
      {
        ToolTipString += " (" + QString::fromStdString(VarInfos.SIUnit)+")";
      }

      ToolTipString += "</p></li>";
    }

    ToolTipString += "</ul>";


    // color for active slots
    SlotShape->setPen(QPen(QBrush(QColor(BUILDER_MODELVIEW_ACTIVECOLOR)),1));
    SlotShape->setBrush(QBrush(QColor(BUILDER_MODELVIEW_ACTIVECOLOR)));
    TextShape->setBrush(QBrush(QColor(BUILDER_MODELVIEW_ACTIVECOLOR)));

    // tooltip
    SlotShape->setToolTip(ToolTipString);
  }
  else
  {
    // color for inactive slots
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


bool ModelItemGraphics::hasProducedVariable(const std::string& UnitsClass, const std::string& Name)
{
  for (auto& Var : m_ProducedVars)
  {
    if (Var.UnitsClass == UnitsClass && Var.Name == Name)
    {
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool ModelItemGraphics::hasUpdatedVariable(const std::string& UnitsClass, const std::string& Name)
{
  for (auto& Var : m_UpdatedVars)
  {
    if (Var.UnitsClass == UnitsClass && Var.Name == Name)
    {
      return true;
    }
  }

  return false;
}
