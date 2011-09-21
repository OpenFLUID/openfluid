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

#include "ICLayer.hpp"
#include "ICLayerPoint.hpp"
#include "ICLayerLineString.hpp"
#include "ICLayerPolygon.hpp"
#include "ICLayerMultiPolygon.hpp"
#include "WidgetLayerObject.hpp"
#include "ToolBox.hpp"

#include "Layer.hpp"

Layer::Layer(const LayerType::LayerTypes LayerType) :
  mp_ICLayer(0), m_LayerType(LayerType)//, mp_WidgetLayerObject(0)
{

  m_LoadShapeFile = false;
  m_IsDisplay = true;
  m_IsSelected = false;
  m_ClassName = "";
  m_FileName = "";
  m_FolderUri = "";
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

void Layer::addNewLayer(
    std::pair<std::pair<std::string, std::string>, std::string> Data)
{

  std::cout << "Open clicked." << std::endl;

  //Notice that this is a std::string, not a Glib::ustring.
  //  std::string FileURI = get_filename();
  //  std::string FolderURI = dialog.get_current_folder() + "/";
  //  std::string FileName;
  //
  //  FileName = FileURI.substr(FolderURI.size(),
  //      FileURI.size() - FolderURI.size() - 4);


  m_FileName = Data.first.second;
  m_FolderUri = Data.first.first;
  m_ClassName = Data.second;

  mp_WidgetLayerObject = new WidgetLayerObject(m_LayerType, m_ClassName,
      m_FileName);

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
  if (m_FileName != "" && m_FolderUri != "" && m_ClassName != "")
  {
    loadShapefile(m_FolderUri, m_FileName);
    m_LoadShapeFile = true;
  }
}

// =====================================================================
// =====================================================================
//// =====================================================================
//// =====================================================================
//
//void Layer::addNewLayer(std::vector<std::string> ClassNames)
//{
//  std::cout << __FILE__ << " " << __LINE__ << std::endl;
//  if (!ClassNames.empty())
//  {
//    Gtk::FileChooserDialog dialog(_("Please choose a new layer"),
//        Gtk::FILE_CHOOSER_ACTION_OPEN);
////    dialog.set_transient_for(dynamic_cast<Gtk::Window*>(&asWidget()->get_parent_window()));
//
//    Gtk::HBox* Hbox = Gtk::manage(new Gtk::HBox());
//    Hbox->set_border_width(5);//spacing(10);
//    Gtk::Label lab;
//    //    lab.set_padding(10,2);
//    lab.set_label(_("Choose An Unit Class :"));
//    lab.set_visible(true);
//
//    Gtk::ComboBoxText* FilterUnitClass = Gtk::manage(new Gtk::ComboBoxText());
//
//    std::vector<std::string>::iterator it;
//
//    for (it = ClassNames.begin(); it != ClassNames.end(); it++)
//    {
//      FilterUnitClass->append(static_cast<Glib::ustring> (*it));
//    }
//    std::cout << __FILE__ << " " << __LINE__ << std::endl;
//    FilterUnitClass->signal_changed().connect(
//        sigc::bind<Gtk::ComboBoxText*>(
//            sigc::mem_fun(*this, &Layer::onComboChanged), FilterUnitClass));
//
//    FilterUnitClass->set_sensitive(true);
//    FilterUnitClass->set_visible(true);
//    FilterUnitClass->show_all_children(true);
//
//    Hbox->pack_start(lab, Gtk::PACK_SHRINK);
//    Hbox->pack_end(*FilterUnitClass, Gtk::PACK_SHRINK);
//    Hbox->set_visible(true);
//
//    Gtk::VBox* VBox = static_cast<Gtk::VBox*> (dialog.get_child());
//
//    VBox->pack_start(*ToolBox::setHSeparator(), Gtk::PACK_SHRINK);
//    VBox->pack_start(*Hbox, Gtk::PACK_SHRINK);
//    VBox->pack_start(*ToolBox::setHSeparator(), Gtk::PACK_SHRINK);
//
//    //Add response buttons the the dialog:
//    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
//    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
//
//    //Add filters, so that only certain file types can be selected:
//    std::cout << __FILE__ << " " << __LINE__ << std::endl;
//    Gtk::FileFilter filter_shapefile;
//    filter_shapefile.set_name("Shapefiles");
//    filter_shapefile.add_pattern("*.shp");
//    dialog.add_filter(filter_shapefile);
//
//    Gtk::FileFilter filter_any;
//    filter_any.set_name("Any files");
//    filter_any.add_pattern("*");
//    dialog.add_filter(filter_any);
//    std::cout << __FILE__ << " " << __LINE__ << std::endl;
//    //Show the dialog and wait for a user response:
//    //    dialog.set_receives_default(true);
//    int result = dialog.run();
//
//    std::cout << __FILE__ << " " << __LINE__ << std::endl;
//    //Handle the response:
//    switch (result)
//    {
//      case (Gtk::RESPONSE_OK):
//      {
//        if (FilterUnitClass->get_active_text().empty())
//        {
//          openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
//              _(
//                  "You can't added layer without selected a unit class before.\n\nPlease select a unit class."));
//          addNewLayer(ClassNames);
//        } else
//        {
//          std::cout << "Open clicked." << std::endl;
//
//          //Notice that this is a std::string, not a Glib::ustring.
//          std::string FileURI = dialog.get_filename();
//          std::string FolderURI = dialog.get_current_folder() + "/";
//          std::string FileName;
//
//          FileName = FileURI.substr(FolderURI.size(),
//              FileURI.size() - FolderURI.size() - 4);
//
//          m_FileName = FileName;
//          m_FolderUri = FolderURI;
//
//          mp_WidgetLayerObject = new WidgetLayerObject(m_LayerType,
//              m_ClassName, m_FileName);
//
//          //***************Signal WidgetObjectBase*********************
//          mp_WidgetLayerObject->getWidgetObjectBase()->signalUpLayerButtonClicked().connect(
//              sigc::mem_fun(*this, &Layer::whenOnUpLayerButtonClicked));
//          mp_WidgetLayerObject->getWidgetObjectBase()->signalDownLayerButtonClicked().connect(
//              sigc::mem_fun(*this, &Layer::whenOnDownLayerButtonClicked));
//          mp_WidgetLayerObject->getWidgetObjectBase()->signalRemoveLayerButtonClicked().connect(
//              sigc::mem_fun(*this, &Layer::whenOnRemoveLayerButtonClicked));
//          mp_WidgetLayerObject->getWidgetObjectBase()->signalIsDisplayButtonChecked().connect(
//              sigc::mem_fun(*this, &Layer::whenOnIsDisplayButtonChecked));
//          mp_WidgetLayerObject->getWidgetObjectBase()->signalIsSelectedLayerClicked().connect(
//              sigc::mem_fun(*this, &Layer::whenOnIsSelectedLayerClicked));
//          mp_WidgetLayerObject->getWidgetExpanderBase()->signalWidgetExpanderBaseChanged().connect(
//              sigc::mem_fun(*this, &Layer::whenOnWidgetExpanderBaseChanged));
//
//          mp_WidgetLayerObject->getWidgetExpanderBase()->onWidgetExpanderBaseChanged();
//
//          loadShapefile(FolderURI, FileName);
//          m_LoadShapeFile = true;
//        }
//        break;
//      }
//      case (Gtk::RESPONSE_CANCEL):
//      {
//        std::cout << "Cancel clicked." << std::endl;
//        break;
//      }
//      default:
//      {
//        std::cout << "Unexpected button clicked." << std::endl;
//        break;
//      }
//    }
//  } else
//  {
//    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
//        _(
//            "You can't added a new layer because there aren't unit class free.\n\nPlease build new unit class or destruct an existing unit class to add a new layer."));
//
//  }
//}
//
//// =====================================================================
//// =====================================================================

void Layer::loadShapefile(std::string FolderURI, std::string FileName)
{
  OGRRegisterAll();

  OGRDataSource *File;

  File = OGRSFDriverRegistrar::Open((char*) FolderURI.c_str(), FALSE);
  if (File == NULL)
  {

    std::cout << "Open failed.\n" << std::endl;
    exit(1);

  } else
  {
    std::cout << "Open OK !!!" << std::endl;
  }
  OGRLayer *Layer = 0;
  Layer = File->GetLayerByName((char*) FileName.c_str());

  if (Layer)
  {
    Layer->ResetReading();

    OGRFeature* Feature;
    int type;
    bool selfIDExist = false;
    //     ICLayer* ICLayer = 0;
    if (Layer->GetFeatureCount() > 0)
    {
      type = Layer->GetNextFeature()->GetGeometryRef()->getGeometryType();

      OGRFeatureDefn *poFDefn = Layer->GetLayerDefn();
      int iField;

      for (iField = 0; iField < poFDefn->GetFieldCount(); iField++)
      {
        OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
        if (static_cast<std::string> (poFieldDefn->GetNameRef()) == "SELF_ID")
          selfIDExist = true;
      }
      switch (type)
      {
        case wkbPoint:
        {
          mp_ICLayer = new ICLayerPoint();
          break;
        }
        case wkbLineString:
        {
          mp_ICLayer = new ICLayerLineString();
          break;
        }
        case wkbPolygon:
        {
          mp_ICLayer = new ICLayerPolygon();
          break;
        }
        case wkbMultiPolygon:
        {
          mp_ICLayer = new ICLayerMultiPolygon();
          break;
        }
        default:
        {
          type = wkbUnknown;
          std::cerr << "bad geometry ref type" << std::endl;
        }
          break;
      }
      mp_WidgetLayerObject->getWidgetObjectBase()->setOGRGeometryType(type);
    } else
      std::cerr << "no feature" << std::endl;
    if (selfIDExist)
    {
      Layer->ResetReading();

      while ((Feature = Layer->GetNextFeature()) != NULL)
      {
        OGRGeometry* poGeometry = Feature->GetGeometryRef();
        OGRGeometry* poGeomCopy = poGeometry->clone();

        OGRFeatureDefn *poFDefn = Layer->GetLayerDefn();
        int iField;
        int selfID;

        for (iField = 0; iField < poFDefn->GetFieldCount(); iField++)
        {
          OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
          if (static_cast<std::string> (poFieldDefn->GetNameRef()) == "SELF_ID")
          {
            selfID = Feature->GetFieldAsInteger(iField);
          }
        }
        mp_ICLayer->addObjectGeo(selfID, poGeomCopy);

        OGRFeature::DestroyFeature(Feature);
      }
      //TODO     m_ICLayer.push_back(ICLayer);
      //     ICLayer->setId(m_NumLayer);
      //     ICLayer->setPosition(m_NumLayer);
      //     ICLayer->setIsDisplay(true);
      //     ICLayer->settype(type);
      //     ICLayer->setIsSelected(false);
      //      m_MinMaxLayer = mp_ICLayer->getMinMax();
    }
  } else
    std::cerr << "layer of file " << FileName << "not found" << std::endl;

  OGRDataSource::DestroyDataSource(File);

  //   calulMaxMinCoordinate();
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
    std::set<int> Select)
{
  mp_ICLayer->draw(Context, Scale, Select);
}

// =====================================================================
// =====================================================================

int Layer::isObjectSelected(double X, double Y, double Scale)
{
  return mp_ICLayer->isSelected(X, Y, Scale);
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

bool Layer::getIsDisplay()
{
  return m_IsDisplay;
}

// =====================================================================
// =====================================================================

std::string Layer::getFileName()
{
  return m_FileName;
}

// =====================================================================
// =====================================================================

std::string Layer::getClassName()
{
  return m_ClassName;
}

// =====================================================================
// =====================================================================

std::string Layer::getFolderUri()
{
  return m_FolderUri;
}

// =====================================================================
// =====================================================================

bool Layer::getLoadShapeFile()
{
  return m_LoadShapeFile;
}

// =====================================================================
// =====================================================================

bool Layer::getIsSelected()
{
  return m_IsSelected;
}

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
    mp_WidgetLayerObject->getWidgetObjectBase()->getButtonDown()->set_sensitive(
        Bool);
}

// =====================================================================
// =====================================================================

void Layer::setWidgetUpSensitive(bool Bool)
{
  if (m_LayerType == LayerType::LAYER_BASE)
    mp_WidgetLayerObject->getWidgetObjectBase()->getButtonUp()->set_sensitive(
        Bool);
}

// =====================================================================
// =====================================================================
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
  m_IsSelected = !m_IsSelected;
  if (m_IsSelected)
    m_signal_IsSelectedLayerClicked.emit(m_ClassName);
  else
    //
    m_signal_IsSelectedLayerClicked.emit("");
}

// =====================================================================
// =====================================================================

void Layer::whenOnWidgetExpanderBaseChanged(int SizeLine, double Red,
    double Green, double Blue, double Alpha)
{
  m_SizeLine = SizeLine;
  m_Red = Red;
  m_Green = Green;
  m_Blue = Blue;
  m_Alpha = Alpha;
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

void Layer::onComboChanged(Gtk::ComboBoxText* Combo)
{
  Glib::ustring text = Combo->get_active_text();
  m_ClassName = static_cast<std::string> (text);
  if (!(text.empty()))
    std::cout << "Combo changed: " << text << std::endl;
}

void Layer::update(openfluid::core::CoreRepository& CoreRepos)
{
  mp_ICLayer->setEngineRequirements(CoreRepos);
  mp_ICLayer->update(m_ClassName);
  m_MinMaxLayer = mp_ICLayer->getMinMax();
}
