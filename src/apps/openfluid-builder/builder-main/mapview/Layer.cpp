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
 \file Layer.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */
#include <glibmm/i18n.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/liststore.h>

#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/core/UnstructuredValue.hpp>
#include <openfluid/core/GeoVectorValue.hpp>

#include "ICLayer.hpp"
#include "ICLayerPoint.hpp"
#include "ICLayerLineString.hpp"
#include "ICLayerPolygon.hpp"
#include "ICLayerMultiPolygon.hpp"
#include "WidgetLayerObject.hpp"
#include "ToolBox.hpp"

#include "Layer.hpp"

Layer::Layer(LayerType::LayerTypes LayerType,
    openfluid::core::UnstructuredValue* Value,
    std::string Id, std::string ClassName) :
  mp_ICLayer(0), mp_WidgetLayerObject(0), m_LayerType(LayerType),
  m_Value(Value), m_Id(Id), m_ClassName(ClassName)
{
  m_IsDisplay = true;
  m_IsSelected = false;
  m_DisplayID = false;
//  m_DisplayGraph = false;

  mp_WidgetLayerObject = new WidgetLayerObject(m_LayerType, m_ClassName, m_Id);


  if(LayerType == LayerType::LAYER_BASE)
  {
    //***************Signal WidgetObjectBase*********************
    mp_WidgetLayerObject->getWidgetObjectBase()->signalUpLayerButtonClicked().connect(
        sigc::mem_fun(*this, &Layer::whenOnUpLayerButtonClicked));
    mp_WidgetLayerObject->getWidgetObjectBase()->signalDownLayerButtonClicked().connect(
        sigc::mem_fun(*this, &Layer::whenOnDownLayerButtonClicked));
    mp_WidgetLayerObject->getWidgetObjectBase()->signalRemoveLayerButtonClicked().connect(
        sigc::mem_fun(*this, &Layer::whenOnRemoveLayerButtonClicked));
    mp_WidgetLayerObject->getWidgetObjectBase()->signalIsDisplayButtonChecked().connect(
        sigc::mem_fun(*this, &Layer::whenOnIsDisplayButtonChecked));
    mp_WidgetLayerObject->getWidgetObjectBase()->signalIsSelectedLayerClicked().connect(
        sigc::mem_fun(*this, &Layer::whenOnIsSelectedLayerClicked));
    mp_WidgetLayerObject->getWidgetExpanderBase()->signalWidgetExpanderBaseChanged().connect(
        sigc::mem_fun(*this, &Layer::whenOnWidgetExpanderBaseChanged));

    mp_WidgetLayerObject->getWidgetExpanderBase()->onWidgetExpanderBaseChanged();

    try
    {
      loadShapefile();
    }
    catch (openfluid::base::OFException e)
    {
      delete mp_WidgetLayerObject;
      delete mp_ICLayer;
      throw;
    }
  }
}

// =====================================================================
// =====================================================================

Layer::~Layer()
{

}

// =====================================================================
// =====================================================================

Gtk::Widget* Layer::asWidget()
{
  return mp_WidgetLayerObject->asWidget();
}


// =====================================================================
// =====================================================================

void Layer::loadShapefile()
{
  OGRDataSource* DataSource = (static_cast<openfluid::core::GeoVectorValue*>(m_Value))->get();

  if(DataSource->GetLayerCount() < 1)
    throw openfluid::base::OFException("OpenFLUID MapView","Layer::loadShapefile","No layer");

  OGRLayer* Layer = DataSource->GetLayer(0);
  if (!Layer)
    throw openfluid::base::OFException("OpenFLUID MapView","Layer::loadShapefile","Problem loading layer");

  if (Layer->GetFeatureCount() < 0)
    throw openfluid::base::OFException("OpenFLUID MapView","Layer::loadShapefile","No feature");

  OGRFeatureDefn* FeatureDef = Layer->GetLayerDefn();

  int SelfIDIndex = FeatureDef->GetFieldIndex("SELF_ID");
  if(SelfIDIndex < 0)
    throw openfluid::base::OFException("OpenFLUID MapView","Layer::loadShapefile","No SELF_ID field");

  OGRwkbGeometryType Type = FeatureDef->GetGeomType();
  switch (Type)
  {
    case wkbPoint:
      mp_ICLayer = new ICLayerPoint();
      break;
    case wkbLineString:
      mp_ICLayer = new ICLayerLineString();
      break;
    case wkbPolygon:
      mp_ICLayer = new ICLayerPolygon();
      break;
    case wkbMultiPolygon:
      mp_ICLayer = new ICLayerMultiPolygon();
      break;
    default:
      throw openfluid::base::OFException("OpenFLUID MapView","Layer::loadShapefile","Unknown Geometry type");
      break;
  }
  mp_WidgetLayerObject->getWidgetObjectBase()->setOGRGeometryType(Type);

  OGRFeature* Feature;
  OGRGeometry* GeometryCopy;
  int SelfID;
  Layer->ResetReading();
  while ((Feature = Layer->GetNextFeature()) != NULL)
  {
    GeometryCopy = Feature->GetGeometryRef()->clone();
    SelfID = Feature->GetFieldAsInteger(SelfIDIndex);

    mp_ICLayer->addObjectGeo(SelfID, GeometryCopy);

    OGRFeature::DestroyFeature(Feature);
  }
}

// =====================================================================
// =====================================================================

void Layer::initialiseLayerContext(Cairo::RefPtr<Cairo::Context> Context,
    double Scale)
{
  Context->set_line_width(m_SizeLine / Scale);
  Context->set_source_rgba(m_Red, m_Green, m_Blue, m_Alpha);
}

// =====================================================================
// =====================================================================

void Layer::draw(Cairo::RefPtr<Cairo::Context> Context, double Scale,
    std::set<int> Select, bool DisplayID)
{
  mp_ICLayer->draw(Context, Scale, Select, DisplayID, m_Alpha);
}

// =====================================================================
// =====================================================================

int Layer::isObjectSelected(double X, double Y, double Scale)
{
  return mp_ICLayer->isSelected(X, Y, Scale);
}

// =====================================================================
// =====================================================================

std::set<int> Layer::ObjectSelected()
{
  return mp_ICLayer->selectObject(m_ClassName);
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

std::pair<std::pair<double, double>, std::pair<double, double> > Layer::getMinMaxLayer()
{
  return m_MinMaxLayer;
}

// =====================================================================
// =====================================================================

std::pair<std::pair<double, double>, std::pair<double, double> > Layer::getMinMaxSelection(
    std::set<int> Selection)
{
  return mp_ICLayer->getMinMax(Selection);
}

// =====================================================================
// =====================================================================

bool Layer::getIsDisplay()
{
  return m_IsDisplay;
}

// =====================================================================
// =====================================================================

//std::string Layer::getFileName()
//{
//  return m_FileName;
//}

// =====================================================================
// =====================================================================

std::string Layer::getClassName()
{
  return m_ClassName;
}

// =====================================================================
// =====================================================================

std::string Layer::getId()
{
  return m_Id;
}

// =====================================================================
// =====================================================================

//std::string Layer::getFolderUri()
//{
//  return m_FolderUri;
//}

// =====================================================================
// =====================================================================

//bool Layer::getLoadShapeFile()
//{
//  return m_LoadShapeFile;
//}

// =====================================================================
// =====================================================================

bool Layer::getIsSelected()
{
  return m_IsSelected;
}

// =====================================================================
// =====================================================================

bool Layer::getDisplayID()
{
  return m_DisplayID;
}

// =====================================================================
// =====================================================================

//bool Layer::getDisplayGraph()
//{
//  return m_DisplayGraph;
//}

// =====================================================================
// =====================================================================

ICLayer* Layer::getICLayer()
{
  return mp_ICLayer;
}

// =====================================================================
// =====================================================================

void Layer::setIsSelected(bool IsSelected)
{
  m_IsSelected = IsSelected;
  mp_WidgetLayerObject->getWidgetObjectBase()->setIsSelected(IsSelected);
}

// =====================================================================
// =====================================================================

void Layer::setWidgetDownSensitive(bool Bool)
{
  if (m_LayerType == LayerType::LAYER_BASE)
    mp_WidgetLayerObject->getWidgetObjectBase()->setDownButtonSensitive(Bool);
}

// =====================================================================
// =====================================================================

void Layer::setWidgetUpSensitive(bool Bool)
{
  if (m_LayerType == LayerType::LAYER_BASE)
    mp_WidgetLayerObject->getWidgetObjectBase()->setUpButtonSensitive(Bool);
}

// =====================================================================
// =====================================================================

void Layer::whenOnUpLayerButtonClicked()
{
  m_signal_UpLayerButtonClicked.emit(m_ClassName);
}

// =====================================================================
// =====================================================================

void Layer::whenOnDownLayerButtonClicked()
{
  m_signal_DownLayerButtonClicked.emit(m_ClassName);
}

// =====================================================================
// =====================================================================

void Layer::whenOnRemoveLayerButtonClicked()
{
  m_signal_RemoveLayerButtonClicked.emit(m_ClassName);
}

// =====================================================================
// =====================================================================

void Layer::whenOnIsDisplayButtonChecked()
{
  m_IsDisplay = !m_IsDisplay;
  m_signal_IsDisplayButtonChecked.emit();
}

// =====================================================================
// =====================================================================

void Layer::whenOnIsSelectedLayerClicked()
{
  bool TmpIsNowSelected = !m_IsSelected;

  m_signal_IsSelectedLayerClicked.emit(TmpIsNowSelected ? m_ClassName : "");
}

// =====================================================================
// =====================================================================

void Layer::whenOnWidgetExpanderBaseChanged(int SizeLine, double Red,
    double Green, double Blue, double Alpha, bool DisplayID/*, bool DisplayGraph*/)
{
  m_SizeLine = SizeLine;
  m_Red = Red;
  m_Green = Green;
  m_Blue = Blue;
  m_Alpha = Alpha;
  m_DisplayID = DisplayID;
//  m_DisplayGraph = DisplayGraph;

  m_signal_WidgetExpanderBaseChanged.emit();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

Layer::mtype_SignalLayerString Layer::signalUpLayerButtonClicked()
{
  return m_signal_UpLayerButtonClicked;
}

// =====================================================================
// =====================================================================

Layer::mtype_SignalLayerString Layer::signalDownLayerButtonClicked()
{
  return m_signal_DownLayerButtonClicked;
}

// =====================================================================
// =====================================================================

Layer::mtype_SignalLayerString Layer::signalRemoveLayerButtonClicked()
{
  return m_signal_RemoveLayerButtonClicked;
}

// =====================================================================
// =====================================================================

Layer::mtype_SignalLayerString Layer::signalIsSelectedLayerClicked()
{
  return m_signal_IsSelectedLayerClicked;
}

// =====================================================================
// =====================================================================

Layer::mtype_SignalLayer Layer::signalIsDisplayButtonChecked()
{
  return m_signal_IsDisplayButtonChecked;
}

// =====================================================================
// =====================================================================

Layer::mtype_SignalLayer Layer::signalWidgetExpanderBaseChanged()
{
  return m_signal_WidgetExpanderBaseChanged;
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Layer::update(openfluid::fluidx::AdvancedDomainDescriptor& Domain)
{
  mp_ICLayer->setEngineRequirements(Domain);
  mp_ICLayer->update(m_ClassName);
  m_MinMaxLayer = mp_ICLayer->getMinMax();
}
