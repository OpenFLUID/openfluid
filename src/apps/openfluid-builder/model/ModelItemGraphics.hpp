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
  @file ModelItemGraphics.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_MODELITEMGRAPHICS_HPP__
#define __OPENFLUID_BUILDERAPP_MODELITEMGRAPHICS_HPP__


#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>


#include <QGraphicsRectItem>

#include "ConnectorGraphics.hpp"


class ModelItemGraphics : public QGraphicsRectItem
{
  public:

    typedef QMap<QString,QStringList> IOSet_t;

  protected:

    static QSize m_DefaultSize;

    static QPointF m_RequiredIOFromCenter;

    static QPointF m_UsedIOFromCenter;

    static QPointF m_UpInIOFromCenter;

    static QPointF m_UpOutIOFromCenter;

    QString m_ID;

    bool m_Ghost;

    bool m_Initialized;

    QList<ConnectorGraphics*> m_Connectors;

    IOSet_t m_ProducedVars;

    IOSet_t m_UpdatedVars;

    IOSet_t m_UsedVars;

    IOSet_t m_RequiredVars;

    QVariant itemChange(GraphicsItemChange Change,const QVariant &Value);

    void mousePressEvent(QGraphicsSceneMouseEvent *Event);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *Event);

    void drawIOSlot(const QPointF& Pos, const QString& Name, bool Active = false);

    QPointF getCenterFromOrigin();

  public:

    ModelItemGraphics(const QPointF &Coords, const QString& ID,
                      QGraphicsItem* Parent = 0);

    ~ModelItemGraphics();

    void initialize();

    QPointF getRequiredIOPosition();

    QPointF getUsedIOPosition();

    QPointF getUpInIOPosition();

    virtual QPointF getProducedIOPosition() =0;

    QPointF getUpOutIOPosition();

    void addConnector(ConnectorGraphics* Connector);

    /* TODo to be definitely removed if unused
    void removeConnector(ConnectorGraphics* Connector);

    void removeConnectors();*/

    const IOSet_t* producedVariables() const
    { return &m_ProducedVars; }

    const IOSet_t* updatedVariables() const
    { return &m_UpdatedVars; }

    const IOSet_t* requiredVariables() const
    { return &m_RequiredVars; }

    const IOSet_t* usedVariables() const
    { return &m_UsedVars; }

    bool hasProducedVariable(const QString& UnitClass, const QString& Name);

    bool hasUpdatedVariable(const QString& UnitClass, const QString& Name);


};

#endif /* __OPENFLUID_BUILDERAPP_MODELITEMGRAPHICS_HPP__ */


