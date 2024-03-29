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
  @file ModelScene.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_MODELSCENE_HPP__
#define __OPENFLUID_BUILDERAPP_MODELSCENE_HPP__


#include <QGraphicsScene>

#include <openfluid/fluidx/CoupledModelDescriptor.hpp>

#include "ModelItemGraphics.hpp"


class ModelScene : public QGraphicsScene
{
  Q_OBJECT;

  private slots:

    void notifySrcEditAsked(const QString& ID, bool Ghost);


  private:

    const openfluid::fluidx::CoupledModelDescriptor& m_Model;
    
    unsigned int m_ColouringModeIndex;

    QList<ModelItemGraphics*> m_GraphicsItems;

    QList<ConnectorGraphics*> m_GraphicsConnections;

    void buildConnections();

    void addConnection(ModelItemGraphics* FromItem, ConnectorGraphics::OutNodeType FromOutNode,
                       ModelItemGraphics* ToItem, ConnectorGraphics::InNodeType ToInNode,
                       const openfluid::ware::SignatureSpatialDataItem& VarInfos);


  public slots:

    void showVariables(bool Show);
    
    void refresh();


  signals:

    void srcEditAsked(const QString&, bool Ghost);


  public:

    ModelScene(const openfluid::fluidx::CoupledModelDescriptor& ModelDesc, QObject* Parent = nullptr);

    ~ModelScene();

};


#endif /* __OPENFLUID_BUILDERAPP_MODELSCENE_HPP__ */
