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
 \file Mediator.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

//#include <set>


#include "Mediator.hpp"

#include <iomanip>

#include <glibmm/i18n.h>
#include <openfluid/guicommon-gtk/DialogBoxFactory.hpp>
#include <openfluid/core/Datastore.hpp>
#include <openfluid/core/DatastoreItem.hpp>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/machine/Factory.hpp>

#include "LayerType.hpp"
#include "EngineHelper.hpp"
#include "MapViewAddLayersDialog.hpp"

Mediator::Mediator(DrawingArea& DrawingArea, Gtk::Statusbar& StatusBar,
    ToolBar& ToolBar, openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc) :
  mref_DrawingArea(DrawingArea), mref_StatusBar(StatusBar), mref_ToolBar(
      ToolBar), mp_Domain(&AdvancedDesc.getDomain()),
      m_IsFirstExposeEvent(true)
{
  mp_Datastore = new openfluid::core::Datastore();
  openfluid::machine::Factory::buildDatastoreFromDescriptor(AdvancedDesc.getDatastoreDescriptor(),*mp_Datastore);

  m_SelectedClassName = "";
  m_infoDialogCreate = false;

  mp_AddLayersDialog = new MapViewAddLayersDialog(AdvancedDesc.getDomain(),*mp_Datastore);

  mp_MainVBoxMediator = Gtk::manage(new Gtk::VBox());

  mref_DrawingArea.signal_ExposeEventChanged().connect(sigc::mem_fun(*this,
      &Mediator::whenDrawingAreaChanged));
  //***************Signal ToolBar*********************
  mref_ToolBar.signal_AddLayerAsked().connect(sigc::mem_fun(*this,
      &Mediator::whenAddLayerAsked));

  mref_ToolBar.signal_SelectModeAsked().connect(
      sigc::mem_fun(*this,
          &Mediator::whenSelectModeAsked));
  mref_ToolBar.signal_MoveModeAsked().connect(sigc::mem_fun(
      *this, &Mediator::whenMoveModeAsked));

  mref_ToolBar.signal_ZoomInCursorAsked().connect(sigc::mem_fun(
      *this, &Mediator::whenZoomInCursorAsked));
  mref_ToolBar.signal_ZoomInFrameAsked().connect(sigc::mem_fun(
      *this, &Mediator::whenZoomInFrameAsked));
  mref_ToolBar.signal_ZoomOutAsked().connect(
      sigc::mem_fun(*this, &Mediator::whenZoomOutAsked));

  mref_ToolBar.signal_Zoom100AllAsked().connect(sigc::mem_fun(*this,
      &Mediator::whenZoom100AllAsked));
  mref_ToolBar.signal_Zoom100LayerAsked().connect(sigc::mem_fun(*this,
      &Mediator::whenZoom100LayerAsked));
  mref_ToolBar.signal_Zoom100SelectionAsked().connect(sigc::mem_fun(
      *this, &Mediator::whenZoom100SelectionAsked));

  mref_ToolBar.signal_InfoAsked().connect(sigc::mem_fun(*this,
      &Mediator::whenInfoAsked));
  mref_ToolBar.signal_SelectAllAsked().connect(sigc::mem_fun(
      *this, &Mediator::whenSelectAllAsked));
  mref_ToolBar.signal_ToggleSelectionAsked().connect(
      sigc::mem_fun(*this, &Mediator::whenToggleSelectionAsked));

  //***************Signal DrawingArea*****************
  mref_DrawingArea.signal_CoordinateChanged().connect(sigc::mem_fun(*this,
      &Mediator::whenOnMotionNotifyChanged));
  mref_DrawingArea.signal_CoordinateSelected().connect(sigc::mem_fun(*this,
      &Mediator::whenOnSelectObjectChanged));

  mp_MainVBoxMediator->set_visible(true);

  addAvailableLayersFromDatastore();
}

// =====================================================================
// =====================================================================

void Mediator::addAvailableLayersFromDatastore()
{
  std::set<std::string> DisplayedLayers;

  openfluid::core::Datastore::DataItemsById_t Items = mp_Datastore->getItems();

  for (openfluid::core::Datastore::DataItemsById_t::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    openfluid::core::DatastoreItem* Item = it->second;

    // check for automatically adding only one layer for a given unit class
    if (DisplayedLayers.count((*Item).getUnitClass()))
      continue;

    if (!(hasADisplayableVectorValue(*Item, *mp_Domain)
        || hasADisplayableRasterValue(*Item)))
      continue;

    Layer* ALayer = tryToCreateALayerFromADatastoreItem(*Item);

    if (!ALayer)
      continue;

    addALayer(*ALayer);
    DisplayedLayers.insert((*Item).getUnitClass());
  }

  if (!m_Layers.empty())
  {
    removeAllObjectMainVBoxMediator();
    addAllObjectMainVBoxMediator();
  }

}

// =====================================================================
// =====================================================================

Layer* Mediator::tryToCreateALayerFromADatastoreItem(
    openfluid::core::DatastoreItem& Item)
{
  Layer* ALayer = 0;

  LayerType::LayerTypes Type;

  switch (Item.getValue()->getType())
  {
    case openfluid::core::UnstructuredValue::GeoVectorValue:
      Type = LayerType::LAYER_BASE;
      break;
    case openfluid::core::UnstructuredValue::GeoRasterValue:
      Type = LayerType::LAYER_BACKGROUND;
      //break;
    default:
      return ALayer;
      break;
  }

  try
  {
    ALayer
        = new Layer(Type, Item.getValue(), Item.getID(), Item.getUnitClass());
  }
  catch (openfluid::base::Exception& e)
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(
        Glib::ustring::compose("%1: %2", Item.getID(), e.what()));
  }

  return ALayer;
}

// =====================================================================
// =====================================================================

void Mediator::addALayer(Layer& ALayer)
{
  m_Layers.push_back(&ALayer);
  m_LayersIds.insert(ALayer.getId());

  ALayer.update(*mp_Domain);

  ALayer.signalUpLayerButtonClicked().connect(sigc::mem_fun(*this,
      &Mediator::whenOnUpLayerButtonClicked));
  ALayer.signalDownLayerButtonClicked().connect(sigc::mem_fun(*this,
      &Mediator::whenOnDownLayerButtonClicked));
  ALayer.signalRemoveLayerButtonClicked().connect(sigc::mem_fun(*this,
      &Mediator::whenOnRemoveLayerButtonClicked));
  ALayer.signalIsDisplayButtonChecked().connect(sigc::mem_fun(*this,
      &Mediator::whenOnIsDisplayButtonChecked));
  ALayer.signalIsSelectedLayerClicked().connect(sigc::mem_fun(*this,
      &Mediator::whenOnIsSelectedLayerClicked));
  ALayer.signalWidgetExpanderBaseChanged().connect(sigc::mem_fun(*this,
      &Mediator::whenOnWidgetExpanderBaseChanged));

  if(m_Layers.size() == 1)
    setToFirstLayerMode();

}

// =====================================================================
// =====================================================================

bool Mediator::hasADisplayableVectorValue(openfluid::core::DatastoreItem& Item,
    openfluid::fluidx::AdvancedDomainDescriptor& Domain)
{
  return Item.getValue() && Item.getValue()->getType()
      == openfluid::core::UnstructuredValue::GeoVectorValue
      && Domain.isClassNameExists(Item.getUnitClass());
}

// =====================================================================
// =====================================================================

bool Mediator::hasADisplayableRasterValue(openfluid::core::DatastoreItem& /*Item*/)
{
  return false;
//  return Item.getValue() && Item.getValue()->getType()
//      == openfluid::core::UnstructuredValue::GeoRasterValue;
}

// =====================================================================
// =====================================================================

Gtk::Widget* Mediator::asWidget()
{
  return mp_MainVBoxMediator;
}

// =====================================================================
// =====================================================================

sigc::signal<void> Mediator::signal_DrawingAreaExposeEventChanged()
{
  return m_signal_DrawingAreaExposeEventChanged;
}

// =====================================================================
// =====================================================================

sigc::signal<void> Mediator::signal_ZoomWithRescaleAsked()
{
  return m_signal_ZoomWithRescaleAsked;
}
// =====================================================================
// =====================================================================

void Mediator::whenDrawingAreaChanged()
{
  if (m_IsFirstExposeEvent)
  {
    if(m_Layers.empty())
      setToNoLayerMode();
    else
      setToFirstLayerMode();

    m_IsFirstExposeEvent = false;
  }
  redraw();
  signal_DrawingAreaExposeEventChanged().emit();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Mediator::whenZoom100AllAsked()
{
  if (!m_Layers.empty())
  {
    std::vector<Layer*>::iterator it;
    std::pair<std::pair<double, double>, std::pair<double, double> > MinMax;
    MinMax.first.first = (*m_Layers.begin())->getMinMaxLayer().first.first;
    MinMax.first.second = (*m_Layers.begin())->getMinMaxLayer().first.second;
    MinMax.second.first = (*m_Layers.begin())->getMinMaxLayer().second.first;
    MinMax.second.second = (*m_Layers.begin())->getMinMaxLayer().second.second;
    for (it = m_Layers.begin(); it < m_Layers.end(); it++)
    {
      if (MinMax.first.first > (*it)->getMinMaxLayer().first.first)
        MinMax.first.first = (*it)->getMinMaxLayer().first.first;
      if (MinMax.first.second > (*it)->getMinMaxLayer().first.second)
        MinMax.first.second = (*it)->getMinMaxLayer().first.second;
      if (MinMax.second.first < (*it)->getMinMaxLayer().second.first)
        MinMax.second.first = (*it)->getMinMaxLayer().second.first;
      if (MinMax.second.second < (*it)->getMinMaxLayer().second.second)
        MinMax.second.second = (*it)->getMinMaxLayer().second.second;
    }
    signal_ZoomWithRescaleAsked().emit();
    mref_DrawingArea.modifyScaleTranslate(MinMax);
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenZoom100SelectionAsked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitIds.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't zoom in a selection without selection.\n\nPlease select a layer."));
  }
  else if (m_SelectedUnitIds.empty())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't zoom in a selection without selection.\n\nPlease select objects on a current layer."));
  }
  else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layers.begin(); it < m_Layers.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName())
      {
        signal_ZoomWithRescaleAsked().emit();
        mref_DrawingArea.modifyScaleTranslate((*it)->getMinMaxSelection(
            m_SelectedUnitIds));
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenZoom100LayerAsked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitIds.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't zoom in a layer without select the corresponding layer before.\n\nPlease select a layer."));
  }
  else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layers.begin(); it < m_Layers.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName())
      {
        signal_ZoomWithRescaleAsked().emit();
        mref_DrawingArea.modifyScaleTranslate((*it)->getMinMaxLayer());
      }
    }
    redraw();
  }
}



// =====================================================================
// =====================================================================

void Mediator::whenSelectAllAsked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitIds.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't select objects without select the corresponding layer before.\n\nPlease select a layer."));
  }
  else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layers.begin(); it < m_Layers.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName())
      {
        std::set<int> temp = (*it)->ObjectSelected();
        temp.swap(m_SelectedUnitIds);
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenToggleSelectionAsked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitIds.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't select objects without select the corresponding layer before.\n\nPlease select a layer."));
  }
  else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layers.begin(); it < m_Layers.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName())
      {
        std::set<int> temp = (*it)->ObjectSelected();
        std::set<int>::iterator itset;
        for (itset = m_SelectedUnitIds.begin(); itset != m_SelectedUnitIds.end(); itset++)
        {
          std::set<int>::iterator it2;
          it2 = temp.find((*itset));
          temp.erase(it2);
        }
        temp.swap(m_SelectedUnitIds);
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenAddLayerAsked()
{
  std::set<std::string> SelectedLayersIds = mp_AddLayersDialog->show(
      m_LayersIds);

  for (std::set<std::string>::const_iterator it = SelectedLayersIds.begin(); it
      != SelectedLayersIds.end(); ++it)
  {
    openfluid::core::DatastoreItem* Item = mp_Datastore->getItem(*it);

    if (!Item)
      continue;

    Layer* Layer = tryToCreateALayerFromADatastoreItem(*Item);

    if (!Layer)
      continue;

    addALayer(*Layer);
  }

  if (!m_Layers.empty())
  {
    removeAllObjectMainVBoxMediator();
    addAllObjectMainVBoxMediator();

    redraw();
  }

}

// =====================================================================
// =====================================================================

void Mediator::whenInfoAsked()
{
  if (m_SelectedClassName == "" || m_SelectedUnitIds.empty())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _("You can't have informations without select the corresponding layer"
            "before and the corresponding units.\n\nPlease select a layer and units."));
    return;
  }

  std::set<int> UnavailableIds;
  std::set<int> AvailableIds = m_SelectedUnitIds;
  std::set<int>::iterator it;

  for (it = m_SelectedUnitIds.begin(); it != m_SelectedUnitIds.end(); ++it)
  {
    try
    {
      mp_Domain->getUnit(m_SelectedClassName, *it);
    }
    catch(openfluid::base::Exception& e)
    {
      UnavailableIds.insert(*it);
      AvailableIds.erase(*it);
    }
  }

  if (UnavailableIds.size() > 1)
  {
    Glib::ustring UnavailUnitsStr = Glib::ustring::compose("%1",
        *UnavailableIds.begin());

    for (it = UnavailableIds.begin().operator ++(); it != UnavailableIds.end(); ++it)
      UnavailUnitsStr += Glib::ustring::compose(" - %1", *it);

    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        Glib::ustring::compose(_("Units %1 are no more available.\n"
            "Information about them can not be displayed."), UnavailUnitsStr));
  }
  else if (UnavailableIds.size() == 1)
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        Glib::ustring::compose(_("Unit %1 is no more available.\n"
            "Information about it can not be displayed."),
            *UnavailableIds.begin()));
  }

  if (!AvailableIds.empty())
  {
    if (!m_infoDialogCreate)
    {
      mp_InfoDialog = new Info(
          dynamic_cast<Gtk::Window&> (*asWidget()->get_toplevel()),
          _("Management"), *mp_Domain);
      m_infoDialogCreate = true;
    }
    mp_InfoDialog->show(m_SelectedClassName, AvailableIds);
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenSelectModeAsked()
{
  setToSelectState();
}

// =====================================================================
// =====================================================================

void Mediator::whenMoveModeAsked()
{
  mref_ToolBar.setMoveMode();
  mref_DrawingArea.changeToMoveState();
}

// =====================================================================
// =====================================================================

void Mediator::whenZoomInCursorAsked()
{
  mref_ToolBar.setZoomInMode();
  mref_DrawingArea.changeToZoomCursorState();
}

// =====================================================================
// =====================================================================

void Mediator::whenZoomInFrameAsked()
{
  mref_ToolBar.setZoomInMode();
  mref_DrawingArea.changeToZoomFrameState();
}
// =====================================================================
// =====================================================================

void Mediator::whenZoomOutAsked()
{
  mref_ToolBar.setZoomOutMode();
  mref_DrawingArea.changeToUnzoomCursorState();
}

// =====================================================================
// =====================================================================

void Mediator::setToSelectState()
{
  mref_ToolBar.setSelectionMode();

  if(mref_DrawingArea.is_realized())
    mref_DrawingArea.changeToSelectState();
}
// =====================================================================
// =====================================================================

void Mediator::whenOnUpLayerButtonClicked(std::string ClassName)
{
  upLayer(ClassName);
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnDownLayerButtonClicked(std::string ClassName)
{
  downLayer(ClassName);
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnRemoveLayerButtonClicked(std::string ClassName)
{
  removeLayer(ClassName);

  if (m_SelectedClassName == ClassName)
  {
    m_SelectedUnitIds.clear();
    m_SelectedClassName = "";
  }
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnIsSelectedLayerClicked(std::string ClassName)
{
  m_SelectedClassName = ClassName;
  m_SelectedUnitIds.clear();
  std::vector<Layer*>::iterator it;
  for (it = m_Layers.begin(); it < m_Layers.end(); ++it)
  {
    (*it)->setIsSelected(m_SelectedClassName == (*it)->getClassName());
  }
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnIsDisplayButtonChecked()
{
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnWidgetExpanderBaseChanged()
{
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnMotionNotifyChanged(double X, double Y)
{
  if (m_Layers.empty())
  {
    mref_StatusBar.push("");
    return;
  }

  Glib::ustring Msg = Glib::ustring::compose("X : %1   Y : %2   (unknown coordinate system)",
      Glib::ustring::format(std::fixed, std::setprecision(6), X),
      Glib::ustring::format(std::fixed, std::setprecision(6), Y));

  mref_StatusBar.push(Msg);
}

// =====================================================================
// =====================================================================

void Mediator::whenOnSelectObjectChanged(double X, double Y)
{
  bool isRedraw = false;
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitIds.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't select objects without select the corresponding layer before.\n\nPlease select a layer."));
  }
  else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layers.begin(); it < m_Layers.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName()
          && !m_SelectedUnitIds.empty())
      {
        int temp = (*it)->isObjectSelected(X, Y, mref_DrawingArea.getScale());
        if (temp != -1)
        {
          std::set<int>::iterator it2;
          it2 = m_SelectedUnitIds.find(temp);
          if (it2 == m_SelectedUnitIds.end()
              && mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitIds.insert(temp);
          }
          else if (it2 == m_SelectedUnitIds.end()
              && !mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitIds.clear();
            m_SelectedUnitIds.insert(temp);
          }
          else if (it2 != m_SelectedUnitIds.end()
              && mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitIds.erase(it2);
          }
          else if (it2 != m_SelectedUnitIds.end()
              && !mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitIds.clear();
          }
          isRedraw = true;
        }
        else
        {
          if (!m_SelectedUnitIds.empty()
              && !mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitIds.clear();
            isRedraw = true;
          }
        }
      }
      else if (m_SelectedClassName == (*it)->getClassName()
          && m_SelectedUnitIds.empty())
      {
        int temp = (*it)->isObjectSelected(X, Y, mref_DrawingArea.getScale());
        if (temp != -1)
        {
          m_SelectedUnitIds.insert(temp);
          isRedraw = true;
        }
      }
    }
  }

  if (isRedraw)
    redraw();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Mediator::redraw()
{
  Glib::RefPtr<Gdk::Window> Window = mref_DrawingArea.get_window();

  if (Window)
  {
    Window->clear();

    if (!m_Layers.empty())
    {
      Cairo::RefPtr<Cairo::Context> Context = Window->create_cairo_context();
      Gtk::Allocation allocation = mref_DrawingArea.get_allocation();
      const int width = allocation.get_width();
      const int height = allocation.get_height();

      Context->rectangle(0, 0, width, height);
      Context->clip();
      Context->set_antialias(Cairo::ANTIALIAS_SUBPIXEL);
      Context->scale(mref_DrawingArea.getScale(), -mref_DrawingArea.getScale());
      Context->translate(-mref_DrawingArea.getXTranslate(),
          -mref_DrawingArea.getYTranslate());
      std::vector<Layer*>::reverse_iterator rit;
      for (rit = m_Layers.rbegin(); rit < m_Layers.rend(); ++rit)
      {
        if ((*rit)->getIsDisplay())
        {
          (*rit)->initialiseLayerContext(Context, mref_DrawingArea.getScale());
          if ((*rit)->getClassName() == m_SelectedClassName)
          {
            (*rit)->draw(Context, mref_DrawingArea.getScale(),
                m_SelectedUnitIds, (*rit)->getDisplayID());
          }
          else
          {
            std::set<int> tempVoidVector;
            (*rit)->draw(Context, mref_DrawingArea.getScale(), tempVoidVector,
                (*rit)->getDisplayID());
          }
        }
      }
    }
  }
}

// =====================================================================
// =====================================================================

void Mediator::upLayer(std::string ClassName)
{
  removeAllObjectMainVBoxMediator();
  Layer* pLayer;
  std::vector<Layer*>::reverse_iterator rit;
  for (rit = m_Layers.rbegin(); rit < m_Layers.rend(); ++rit)
  {
    if ((*rit)->getClassName() == ClassName)
    {
      pLayer = (*rit);
      m_Layers.erase(--rit.base());
      ++rit;
      m_Layers.insert(--rit.base(), pLayer);
    }
  }
  addAllObjectMainVBoxMediator();
}

// =====================================================================
// =====================================================================

void Mediator::downLayer(std::string ClassName)
{
  removeAllObjectMainVBoxMediator();
  Layer* pLayer;
  std::vector<Layer*>::iterator it;
  for (it = m_Layers.begin(); it < m_Layers.end(); it++)
  {
    if ((*it)->getClassName() == ClassName)
    {
      pLayer = (*it);
      m_Layers.erase(it);
      it++;
      m_Layers.insert(it, pLayer);
    }
  }
  addAllObjectMainVBoxMediator();
}

// =====================================================================
// =====================================================================

void Mediator::removeLayer(std::string ClassName)
{
  removeAllObjectMainVBoxMediator();
  std::vector<Layer*>::iterator it;
  for (it = m_Layers.begin(); it < m_Layers.end(); it++)
  {
    if ((*it)->getClassName() == ClassName)
    {
      m_LayersIds.erase((*it)->getId());
      m_Layers.erase(it);
    }
  }

  addAllObjectMainVBoxMediator();

  if (m_Layers.empty())
    setToNoLayerMode();
}

// =====================================================================
// =====================================================================

void Mediator::setToNoLayerMode()
{
  mref_ToolBar.setAtLeastALayerMode(false);

  if(mref_DrawingArea.is_realized())
    mref_DrawingArea.changeToInitialState();
}

// =====================================================================
// =====================================================================

void Mediator::setToFirstLayerMode()
{
  mref_ToolBar.setAtLeastALayerMode(true);

  if(mref_DrawingArea.is_realized())
    whenZoom100AllAsked();

  whenOnIsSelectedLayerClicked(m_Layers[0]->getClassName());

  setToSelectState();
}

// =====================================================================
// =====================================================================

void Mediator::removeAllObjectMainVBoxMediator()
{
  while (!mp_MainVBoxMediator->children().empty())
    mp_MainVBoxMediator->remove(**mp_MainVBoxMediator->get_children().begin());
}

// =====================================================================
// =====================================================================

void Mediator::addAllObjectMainVBoxMediator()
{
  if (!m_Layers.empty())
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layers.begin(); it < m_Layers.end(); it++)
    {
      mp_MainVBoxMediator->pack_start(*(*it)->asWidget(), Gtk::PACK_SHRINK);
      (*it)->setWidgetUpSensitive(true);
      (*it)->setWidgetDownSensitive(true);
    }
    m_Layers.front()->setWidgetUpSensitive(false);
    m_Layers.back()->setWidgetDownSensitive(false);
  }
}

// =====================================================================
// =====================================================================


