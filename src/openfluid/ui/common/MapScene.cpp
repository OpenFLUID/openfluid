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
  @file MapScene.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QMessageBox>
#include <QApplication>

#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/DatastoreItem.hpp>

#include "MapScene.hpp"
#include "PointGraphics.hpp"
#include "MultiPointGraphics.hpp"
#include "LineStringGraphics.hpp"
#include "MultiLineStringGraphics.hpp"
#include "PolygonGraphics.hpp"
#include "MultiPolygonGraphics.hpp"


openfluid::ui::common::MapScene::MapScene(const openfluid::fluidx::SpatialDomainDescriptor& Domain,
                   QObject* Parent):
  QGraphicsScene(Parent), m_Domain(Domain), m_ActiveLayer(nullptr)
{

}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapScene::addLayer(const openfluid::fluidx::DatastoreItemDescriptor* DSItemDesc,
                        int ZLayer,
                        int LineWidth,
                        QColor LineColor,
                        QColor FillColor)
{
  if (DSItemDesc->getType() == openfluid::core::UnstructuredValue::UnstructuredType::VECTOR)
  {
    if (m_LocalDatastore.item(DSItemDesc->getID()) == nullptr)
    {
      m_LocalDatastore.addItem(openfluid::core::DatastoreItem(DSItemDesc->getID(),DSItemDesc->getPrefixPath(),
                                                              DSItemDesc->getRelativePath(),DSItemDesc->getType(),
                                                              DSItemDesc->getUnitsClass()));
    }

    openfluid::core::DatastoreItem* DSItem = m_LocalDatastore.item(DSItemDesc->getID());

    openfluid::core::GeoVectorValue* VectorData = dynamic_cast<openfluid::core::GeoVectorValue*>(DSItem->value());

    if (VectorData->data() != nullptr && VectorData->containsField("OFLD_ID",0))
    {
      OGRLayer* Layer = VectorData->layer();
      float ScaledLineWidth = LineWidth;
      if (LineWidth > 1)
      {
        try {
          if (m_Scale <= 0)
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Scene scale negative or null. Can't apply corresponding scaling."
                                                      );
          }
          else
          {
            ScaledLineWidth /= m_Scale;
          }
        }
        catch (const std::exception & E)
        {
          std::cerr << "std ERROR: " << E.what() << std::endl;
          ScaledLineWidth = 0;
        }
      }
      else
      {
        ScaledLineWidth = 0;
      }
      
      QPen FeaturePen(QBrush(LineColor),ScaledLineWidth);
      
      QBrush FeatureBrush(FillColor);

      std::string StdClassName = DSItemDesc->getUnitsClass();

      OGRFeature* Feature;
      Layer->ResetReading();

      while ((Feature = Layer->GetNextFeature()) != nullptr )
      {
        OGRGeometry* Geometry = Feature->GetGeometryRef();
        int ID = Feature->GetFieldAsInteger("OFLD_ID");

        if (Geometry && m_Domain.isSpatialUnitExist(StdClassName,ID))
        {
          OGRwkbGeometryType GeomType = wkbFlatten(Geometry->getGeometryType());

          if (GeomType == wkbPoint)
          {
            PointGraphics* PointG = new PointGraphics(dynamic_cast<OGRPoint*>(Geometry),FeaturePen);
            PointG->setZValue(ZLayer);
            PointG->setUnitID(ID);
            addItem(PointG);
            m_MapItems[StdClassName].append(PointG);
          }
          else if (GeomType == wkbLineString)
          {
            LineStringGraphics* LineSG = new LineStringGraphics(dynamic_cast<OGRLineString*>(Geometry),FeaturePen);
            LineSG->setZValue(ZLayer);
            LineSG->setUnitID(ID);
            addItem(LineSG);
            m_MapItems[StdClassName].append(LineSG);
          }
          else if (GeomType == wkbPolygon)
          {
            PolygonGraphics* PolyG = new PolygonGraphics(dynamic_cast<OGRPolygon*>(Geometry),
                                                         FeaturePen,FeatureBrush);
            PolyG->setZValue(ZLayer);
            PolyG->setUnitID(ID);
            addItem(PolyG);
            m_MapItems[StdClassName].append(PolyG);
          }
          else if (GeomType == wkbMultiPoint)
          {
            MultiPointGraphics* MultiPointG = new MultiPointGraphics(dynamic_cast<OGRMultiPoint*>(Geometry),
                                                                     FeaturePen);
            MultiPointG->setZValue(ZLayer);
            MultiPointG->setUnitID(ID);
            addItem(MultiPointG);
            m_MapItems[StdClassName].append(MultiPointG);
          }
          else if (GeomType == wkbMultiLineString)
          {
            MultiLineStringGraphics* MultiLineSG =
                new MultiLineStringGraphics(dynamic_cast<OGRMultiLineString*>(Geometry),FeaturePen);
            MultiLineSG->setZValue(ZLayer);
            MultiLineSG->setUnitID(ID);
            addItem(MultiLineSG);
            m_MapItems[StdClassName].append(MultiLineSG);
          }
          else if (GeomType == wkbMultiPolygon)
          {
            MultiPolygonGraphics* MultiPolyG =
                new MultiPolygonGraphics(dynamic_cast<OGRMultiPolygon*>(Geometry),
                                         FeaturePen,FeatureBrush);
            MultiPolyG->setZValue(ZLayer);
            MultiPolyG->setUnitID(ID);
            addItem(MultiPolyG);
            m_MapItems[StdClassName].append(MultiPolyG);
          }
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


const QList<openfluid::ui::common::MapItemGraphics*>* openfluid::ui::common::MapScene::activeLayer()
{
  return m_ActiveLayer;
}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapScene::setScale(const float Scale)
{
  m_Scale = Scale;
}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapScene::clear()
{
  m_MapItems.clear();
  QGraphicsScene::clear();
}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapScene::updateActiveLayer()
{
  for (QList<MapItemGraphics*> Layer : m_MapItems)
  {
    for (MapItemGraphics* Item : Layer)
    {
      Item->setFlag(QGraphicsItem::ItemIsSelectable,false);
    }
  }

  if (m_ActiveLayer != nullptr)
  {
    for (MapItemGraphics* Item :*m_ActiveLayer)
    {
      Item->setFlag(QGraphicsItem::ItemIsSelectable,true);
    }
  }
}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapScene::setActiveLayer(const QString& UnitClass)
{
  if (UnitClass.isEmpty() || !m_MapItems.contains(UnitClass.toStdString()))
  {
    m_ActiveLayer = nullptr;
  }
  else
  {
    m_ActiveLayer = &m_MapItems[UnitClass.toStdString()];
  }

  updateActiveLayer();
}


// =====================================================================
// =====================================================================


void openfluid::ui::common::MapScene::enableUnitsIDs(bool /*Enabled*/)
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),
                        QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}
