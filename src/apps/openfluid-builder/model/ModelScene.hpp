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
  \file ModelScene.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_MODELSCENE_HPP__
#define __OPENFLUID_BUILDERAPP_MODELSCENE_HPP__

#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>
#include <QGraphicsScene>

#include "ModelItemGraphics.hpp"


class ModelScene : public QGraphicsScene
{
  Q_OBJECT;

  private:

    const openfluid::fluidx::AdvancedModelDescriptor& m_Model;

    QList<ModelItemGraphics*> m_GraphicsItems;

    QList<ConnectorGraphics*> m_GraphicsConnections;

    void buildConnections();

    void addConnection(ModelItemGraphics* FromItem, ConnectorGraphics::OutNodeType FromOutNode,
                       ModelItemGraphics* ToItem, ConnectorGraphics::InNodeType ToInNode,
                       const QString& UnitClass, const QString& VarName);

  public slots:

    void showVariables(bool Show);

  public:

    ModelScene(const openfluid::fluidx::AdvancedModelDescriptor& ModelDesc, QObject* Parent = 0);

    ~ModelScene();

    void refresh();


};



#endif /* __OPENFLUID_BUILDERAPP_MODELSCENE_HPP__ */
