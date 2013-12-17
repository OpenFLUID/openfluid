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
  \file ConnectorGraphics.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __CONNECTORGRAPHICS_HPP__
#define __CONNECTORGRAPHICS_HPP__

class ModelItemGraphics;

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>

class ConnectorGraphics : public QGraphicsPathItem
{
  public:

    enum InNodeType { NODE_REQ, NODE_US, NODE_INUP };

    enum OutNodeType { NODE_PROD, NODE_OUTUP };


  private:

    ModelItemGraphics* mp_FromItem;
    OutNodeType m_FromOutNode;

    ModelItemGraphics* mp_ToItem;
    InNodeType m_ToInNode;

    QStringList m_Variables;

    QGraphicsSimpleTextItem* mp_VarsText;

    QGraphicsRectItem* mp_VarsTextBox;


  public:

    ConnectorGraphics(ModelItemGraphics* FromItem, OutNodeType FromOutNode,
                      ModelItemGraphics* ToItem, InNodeType ToInNode,
                      QGraphicsItem* Parent = 0);

    ~ConnectorGraphics();

    void updatePosition();

    void addVariable(const QString& UnitClass, const QString& VarName);

    ModelItemGraphics* getFromItem()
    { return mp_FromItem; }

    OutNodeType getFromNode()
    { return m_FromOutNode; }

    ModelItemGraphics* getToItem()
    { return mp_ToItem; }

    InNodeType getToNode()
    { return m_ToInNode; }

    void setVariablesNamesVisible(bool Visible) const;

};


#endif /* __CONNECTORGRAPHICS_HPP__ */
