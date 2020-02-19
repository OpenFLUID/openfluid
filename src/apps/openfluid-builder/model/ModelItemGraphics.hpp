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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_MODELITEMGRAPHICS_HPP__
#define __OPENFLUID_BUILDERAPP_MODELITEMGRAPHICS_HPP__


#include <QGraphicsRectItem>

#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>

#include "ConnectorGraphics.hpp"


class ModelItemGraphics : public QObject, public QGraphicsRectItem
{
  Q_OBJECT;

  public:

    enum class SlotType {SLOT_REQ, SLOT_US, SLOT_UPIN, SLOT_PROD, SLOT_UPOUT };

    /**
      Structure for storage of input/output variables in graphical view of models
    */
    typedef std::vector<openfluid::ware::SignatureTypedSpatialDataItem> IOSet_t;


  protected:

    static QSize m_DefaultSize;

    static QPointF m_RequiredIOFromCenter;

    static QPointF m_UsedIOFromCenter;

    static QPointF m_UpInIOFromCenter;

    static QPointF m_UpOutIOFromCenter;

    QString m_ID;

    unsigned int m_Order;

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

    void drawIOSlot(const QPointF& Pos, const SlotType& Type,
                    const IOSet_t& VarsInfos);

    QPointF getCenterFromOrigin();
    
    void updateFontColor(const QColor& BGColor);

  public:

    ModelItemGraphics(const QPointF &Coords,
                      const QString& ID,
                      const QString& DisplayedTitle,
                      unsigned int Order,
                      const QColor& BorderColor,
                      QGraphicsItem* Parent = nullptr);

    ~ModelItemGraphics();

    void initialize();

    QString getID() const
    { return m_ID; }

    QPointF getRequiredIOPosition();

    QPointF getUsedIOPosition();

    QPointF getUpInIOPosition();

    virtual QPointF getProducedIOPosition() = 0;

    QPointF getUpOutIOPosition();

    void addConnector(ConnectorGraphics* Connector);

    /* TODO to be definitely removed if unused
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

    bool hasProducedVariable(const std::string& UnitsClass, const std::string& Name);

    bool hasUpdatedVariable(const std::string& UnitsClass, const std::string& Name);


};


#endif /* __OPENFLUID_BUILDERAPP_MODELITEMGRAPHICS_HPP__ */

