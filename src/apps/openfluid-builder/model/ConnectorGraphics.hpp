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
  @file ConnectorGraphics.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_BUILDERAPP_CONNECTORGRAPHICS_HPP__
#define __OPENFLUID_BUILDERAPP_CONNECTORGRAPHICS_HPP__


class ModelItemGraphics;


#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>


class ConnectorGraphics : public QGraphicsPathItem
{
  public:

    enum class InNodeType { NODE_REQ, NODE_US, NODE_INUP };

    enum class OutNodeType { NODE_PROD, NODE_OUTUP };


  private:

    ModelItemGraphics* mp_FromItem;
    OutNodeType m_FromOutNode;

    ModelItemGraphics* mp_ToItem;
    InNodeType m_ToInNode;

    QList<openfluid::ware::SignatureSpatialDataItem> m_VariablesInfos;

    QGraphicsSimpleTextItem* mp_VarsText;

    QGraphicsRectItem* mp_VarsTextBox;

    QString getVariablesString() const;

    QString getToolTipString() const;


  public:

    ConnectorGraphics(ModelItemGraphics* FromItem, OutNodeType FromOutNode,
                      ModelItemGraphics* ToItem, InNodeType ToInNode,
                      QGraphicsItem* Parent = nullptr);

    ~ConnectorGraphics();

    void updatePosition();

    void addVariable(const openfluid::ware::SignatureSpatialDataItem& VarInfos);

    ModelItemGraphics* fromItem()
    { return mp_FromItem; }

    OutNodeType getFromNode()
    { return m_FromOutNode; }

    ModelItemGraphics* toItem()
    { return mp_ToItem; }

    InNodeType getToNode()
    { return m_ToInNode; }

    void setVariablesNamesVisible(bool Visible) const;

};


#endif /* __OPENFLUID_BUILDERAPP_CONNECTORGRAPHICS_HPP__ */
