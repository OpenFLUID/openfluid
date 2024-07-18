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
  @file MapScene.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_MAPSCENE_HPP__
#define __OPENFLUID_UICOMMON_MAPSCENE_HPP__


#include <QGraphicsScene>

#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>
#include <openfluid/fluidx/SpatialDomainDescriptor.hpp>
#include <openfluid/core/Datastore.hpp>

#include "MapItemGraphics.hpp"


namespace openfluid { namespace ui { namespace common {

class OPENFLUID_API MapScene : public QGraphicsScene
{
  Q_OBJECT;

  private:

    const openfluid::fluidx::SpatialDomainDescriptor& m_Domain;

    openfluid::core::Datastore m_LocalDatastore;

    QMap<std::string,QList<MapItemGraphics*>> m_MapItems;

    QList<MapItemGraphics*>* m_ActiveLayer;

    void updateActiveLayer();
    
    float m_Scale = -1;


  public slots:

    void enableUnitsIDs(bool Enabled);


  public:

    MapScene(const openfluid::fluidx::SpatialDomainDescriptor& Domain,
             QObject* Parent = nullptr);

    void addLayer(const openfluid::fluidx::DatastoreItemDescriptor* DSItemDesc,
                     int ZLayer,
                     int LineWidth,
                     QColor LineColor,
                     QColor FillColor);

    void setActiveLayer(const QString& UnitClass);
    
    const QList<MapItemGraphics*>* activeLayer();
    
    void setScale(const float Scale);

    void clear();
};

} } }

#endif /* __OPENFLUID_UICOMMON_MAPSCENE_HPP__ */
