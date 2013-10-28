/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file MapScene.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "MapScene.hpp"

#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/DatastoreItem.hpp>

#include "MultiPolygonGraphics.hpp"
#include "PolygonGraphics.hpp"
#include "LineStringGraphics.hpp"

#include <iostream>
#include <QMessageBox>
#include <QApplication>


MapScene::MapScene(const openfluid::fluidx::AdvancedDomainDescriptor& Domain,
                   QObject* Parent):
  QGraphicsScene(Parent), m_Domain(Domain), m_ActiveLayer(NULL)
{

}


// =====================================================================
// =====================================================================


void MapScene::addLayer(const openfluid::fluidx::DatastoreItemDescriptor* DSItemDesc,
                        int ZLayer,
                        int LineWidth,
                        QColor LineColor,
                        QColor FillColor)
{
  if (DSItemDesc->getType() == openfluid::core::UnstructuredValue::GeoVectorValue)
  {
    openfluid::core::DatastoreItem* DSItem;

    if (m_LocalDatastore.getItem(DSItemDesc->getID()) == NULL)
    {
      DSItem = new openfluid::core::DatastoreItem(DSItemDesc->getID(),
                                                  DSItemDesc->getPrefixPath(),
                                                  DSItemDesc->getRelativePath(),
                                                  DSItemDesc->getType(),
                                                  DSItemDesc->getUnitClass());
      m_LocalDatastore.addItem(DSItem);
    }
    else
      DSItem = m_LocalDatastore.getItem(DSItemDesc->getID());

    openfluid::core::GeoVectorValue* VectorData =
        dynamic_cast<openfluid::core::GeoVectorValue*>(DSItem->getValue());

    if (VectorData->get() != NULL && VectorData->containsField("SELF_ID",0))
    {

      OGRLayer* Layer = VectorData->getLayer();

      // TODO fix for correct line width with QPen FeaturePen(QBrush(LineColor),LineWidth), FeaturePen.setCosmetic(true);
      QPen FeaturePen(QBrush(LineColor),0);
      QBrush FeatureBrush(FillColor);
      std::string StdClassName = DSItemDesc->getUnitClass();


      OGRFeature *Feature;
      Layer->ResetReading();

      while ((Feature = Layer->GetNextFeature()) != NULL )
      {
        OGRGeometry *Geometry = Feature->GetGeometryRef();
        OGRwkbGeometryType GeomType =  Geometry->getGeometryType();

        int ID = Feature->GetFieldAsInteger("SELF_ID");

        if (Geometry != NULL && m_Domain.isUnitExist(StdClassName,ID))
        {
          if (GeomType == wkbPoint)
          {
            // TODO
          }
          else if (GeomType == wkbMultiLineString)
          {
            // TODO
          }
          else if (GeomType == wkbLineString)
          {
            LineStringGraphics* LineSG = new LineStringGraphics(dynamic_cast<OGRLineString*>(Geometry),FeaturePen);
            LineSG->setZValue(ZLayer);
            LineSG->setUnitID(ID);
            addItem(LineSG);
            m_MapItems[StdClassName].append(LineSG);
          }
          else if (GeomType == wkbMultiPolygon)
          {
            MultiPolygonGraphics* MultiPolyG = new MultiPolygonGraphics(dynamic_cast<OGRMultiPolygon*>(Geometry),
                                                                        FeaturePen,FeatureBrush);
            MultiPolyG->setZValue(ZLayer);
            MultiPolyG->setUnitID(ID);
            addItem(MultiPolyG);
            m_MapItems[StdClassName].append(MultiPolyG);
          }
          else if (GeomType == wkbPolygon)
          {
            PolygonGraphics* PolyG = new PolygonGraphics(dynamic_cast<OGRPolygon*>(Geometry),FeaturePen,FeatureBrush);
            PolyG->setZValue(ZLayer);
            PolyG->setUnitID(ID);
            addItem(PolyG);
            m_MapItems[StdClassName].append(PolyG);
          }
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


void MapScene::clear()
{
  m_MapItems.clear();
  QGraphicsScene::clear();
}


// =====================================================================
// =====================================================================


void MapScene::updateActiveLayer()
{
  foreach(QList<MapItemGraphics*> Layer,m_MapItems)
  {
    foreach(MapItemGraphics* Item,Layer)
    {
      Item->setFlag(QGraphicsItem::ItemIsSelectable,false);
    }
  }

  if (m_ActiveLayer != NULL)
  {
    foreach(MapItemGraphics* Item,*m_ActiveLayer)
    {
      Item->setFlag(QGraphicsItem::ItemIsSelectable,true);
    }
  }
}


// =====================================================================
// =====================================================================


void MapScene::setActiveLayer(const QString& UnitClass)
{
  if (UnitClass.isEmpty() || !m_MapItems.contains(UnitClass.toStdString()))
    m_ActiveLayer = NULL;
  else
    m_ActiveLayer = &m_MapItems[UnitClass.toStdString()];

  updateActiveLayer();
}


// =====================================================================
// =====================================================================


void MapScene::enableUnitsIDs(bool Enabled)
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}
