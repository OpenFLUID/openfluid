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
  \file ConnectorGraphics.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "ModelItemGraphics.hpp"
#include "ConnectorGraphics.hpp"

#include "builderconfig.hpp"

#include <QPen>
#include <QFont>


ConnectorGraphics::ConnectorGraphics(ModelItemGraphics* FromItem, OutNodeType FromOutNode,
                                     ModelItemGraphics* ToItem, InNodeType ToInNode,
                                     QGraphicsItem* Parent):
  QGraphicsPathItem(Parent),
  mp_FromItem(FromItem),m_FromOutNode(FromOutNode),
  mp_ToItem(ToItem),m_ToInNode(ToInNode)
{
  setZValue(-3000);
  setPen(QPen(QBrush(QColor(BUILDER_MODELVIEW_CONNECTCOLOR)),3));

  mp_VarsTextBox = new QGraphicsRectItem();
  mp_VarsTextBox->setZValue(-2000);
  mp_VarsTextBox->setBrush(QBrush(QColor("white")));
  mp_VarsTextBox->setPen(QPen(Qt::NoPen));
  mp_VarsTextBox->setOpacity(0.8);
  mp_VarsTextBox->setParentItem(this);

  mp_VarsText = new QGraphicsSimpleTextItem("");
  mp_VarsText->setZValue(-1000);
  mp_VarsText->setBrush(QBrush(QColor(BUILDER_MODELVIEW_ACTIVECOLOR)));
  QFont TmpFont = mp_VarsText->font();
  TmpFont.setPointSize(10);
  TmpFont.setItalic(true);
  mp_VarsText->setFont(TmpFont);
  mp_VarsText->setParentItem(this);

  updatePosition();

}


// =====================================================================
// =====================================================================


ConnectorGraphics::~ConnectorGraphics()
{

}


// =====================================================================
// =====================================================================


void ConnectorGraphics::updatePosition()
{
  QPainterPath Path;

  QPointF FromPos;
  QPointF ToPos;

  if (m_FromOutNode == NODE_PROD)
    FromPos = mp_FromItem->getProducedIOPosition();
  else
    FromPos = mp_FromItem->getUpOutIOPosition();

  if (m_ToInNode == NODE_REQ)
    ToPos = mp_ToItem->getRequiredIOPosition();
  else if (m_ToInNode == NODE_US)
    ToPos = mp_ToItem->getUsedIOPosition();
  else
    ToPos = mp_ToItem->getUpInIOPosition();


  Path.moveTo(FromPos);

  // intermediate position helps for correct bezier curve shape
  // and for positionning of variable names as curve label
  QPointF InterPos;


  if (FromPos.y() < ToPos.y()) // "From" slot (source) is upper than "To" slot (destination)
  {

    InterPos = QPointF(FromPos.x()+((ToPos.x()-FromPos.x())/2.0),
                        FromPos.y()+((ToPos.y()-FromPos.y())/2.0));

    Path.quadTo(QPointF(FromPos.x(),InterPos.y()),InterPos);
    Path.quadTo(QPointF(ToPos.x(),InterPos.y()),ToPos);
  }
  else // "From" slot (source) is lower than "To" slot (destination)
  {
    InterPos = QPointF(FromPos.x()+((ToPos.x()-FromPos.x())/2.0)-200,
                       FromPos.y()+((ToPos.y()-FromPos.y())/2.0));

    Path.quadTo(QPointF(FromPos.x()-200,FromPos.y()+200),
                InterPos);
    Path.quadTo(QPointF(ToPos.x()-200,ToPos.y()-200),
                ToPos);
  }

  setPath(Path);



  // variables names
  mp_VarsText->setText(m_Variables.join("\n"));

  QPointF TextPos(InterPos.x()-mp_VarsText->boundingRect().width()/2,
                  InterPos.y()-mp_VarsText->boundingRect().height()/2);

  mp_VarsText->setPos(TextPos);



  // variables names background box
  mp_VarsTextBox->setRect(TextPos.x()-2,TextPos.y()-2,
                          mp_VarsText->boundingRect().width()+4, mp_VarsText->boundingRect().height()+4);
}


// =====================================================================
// =====================================================================


void ConnectorGraphics::addVariable(const QString& UnitClass, const QString& VarName)
{
  m_Variables.append(VarName+" {"+UnitClass+"}");
  updatePosition();
}


// =====================================================================
// =====================================================================


void ConnectorGraphics::setVariablesNamesVisible(bool Visible) const
{
  mp_VarsText->setVisible(Visible);
  mp_VarsTextBox->setVisible(Visible);
}

