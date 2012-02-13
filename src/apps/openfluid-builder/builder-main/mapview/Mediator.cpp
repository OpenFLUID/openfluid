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
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/core/Datastore.hpp>
#include <openfluid/core/DatastoreItem.hpp>

#include "LayerType.hpp"
#include "EngineHelper.hpp"
#include "MapViewAddLayersDialog.hpp"

Mediator::Mediator(DrawingArea& DrawingArea, Gtk::Statusbar& StatusBar,
    ToolBar& ToolBar) :
  mref_DrawingArea(DrawingArea), mref_StatusBar(StatusBar), mref_ToolBar(
      ToolBar), mp_CoreRepos(0), mp_Datastore(0), m_IsFirstExposeEvent(true)
{
  m_SelectedClassName = "";
  m_infoDialogCreate = false;

  mp_AddLayersDialog = new MapViewAddLayersDialog();

  mp_MainVBoxMediator = Gtk::manage(new Gtk::VBox());

  mref_DrawingArea.signal_ExposeEventChanged().connect(sigc::mem_fun(*this,
      &Mediator::whenDrawingAreaChanged));
  //***************Signal ToolBar*********************
  mref_ToolBar.signalAddLayerToolButtonClicked().connect(sigc::mem_fun(*this,
      &Mediator::whenOnAddLayerToolButtonClicked));
  mref_ToolBar.signalInfoToolButtonClicked().connect(sigc::mem_fun(*this,
      &Mediator::whenOnInfoToolButtonClicked));
  mref_ToolBar.signalMoveLayerToggleToolButtonClicked().connect(sigc::mem_fun(
      *this, &Mediator::whenOnMoveLayerToggleToolButtonClicked));
  mref_ToolBar.signalSelectAllPreferenceMenuClicked().connect(sigc::mem_fun(
      *this, &Mediator::whenOnSelectAllPreferenceMenuClicked));
  mref_ToolBar.signalSelectObjectLayerToggleToolButtonClicked().connect(
      sigc::mem_fun(*this,
          &Mediator::whenOnSelectObjectLayerToggleToolButtonClicked));
  mref_ToolBar.signalShow100FocusButtonClicked().connect(sigc::mem_fun(*this,
      &Mediator::whenOnShow100FocusButtonClicked));
  mref_ToolBar.signalToggleSelectedPreferenceMenuClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnToggleSelectedPreferenceMenuClicked));
  mref_ToolBar.signalUnzoomCursorToggleToolButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnUnzoomCursorToggleToolButtonClicked));
  mref_ToolBar.signalZoomCursorZoomTypeButtonClicked().connect(sigc::mem_fun(
      *this, &Mediator::whenOnZoomCursorZoomTypeButtonClicked));
  mref_ToolBar.signalZoomFrameZoomTypeButtonClicked().connect(sigc::mem_fun(
      *this, &Mediator::whenOnZoomFrameZoomTypeButtonClicked));
  mref_ToolBar.signalZoomLayerFocusButtonClicked().connect(sigc::mem_fun(*this,
      &Mediator::whenOnZoomLayerFocusButtonClicked));
  mref_ToolBar.signalZoomSelectionFocusButtonClicked().connect(sigc::mem_fun(
      *this, &Mediator::whenOnZoomSelectionFocusButtonClicked));
  //***************Signal DrawingArea*****************
  mref_DrawingArea.signal_CoordinateChanged().connect(sigc::mem_fun(*this,
      &Mediator::whenOnMotionNotifyChanged));
  mref_DrawingArea.signal_CoordinateSelected().connect(sigc::mem_fun(*this,
      &Mediator::whenOnSelectObjectChanged));

  mp_MainVBoxMediator->set_visible(true);
}

// =====================================================================
// =====================================================================

void Mediator::addAvailableLayersFromDatastore()
{
  openfluid::core::Datastore::DataItemsById_t Items = mp_Datastore->getItems();

  for (openfluid::core::Datastore::DataItemsById_t::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    openfluid::core::DatastoreItem* Item = it->second;

    if (!(hasADisplayableVectorValue(*Item, *mp_CoreRepos)
        || hasADisplayableRasterValue(*Item)))
      continue;

    Layer* ALayer = tryToCreateALayerFromADatastoreItem(*Item);

    if (!ALayer)
      continue;

    addALayer(*ALayer);
  }

  if (!m_Layers.empty())
  {
    removeAllObjectMainVBoxMediator();
    addAllObjectMainVBoxMediator();

    mref_ToolBar.resetSensitiveToolBar(true);
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
    default:
      return ALayer;
      break;
  }

  try
  {
    ALayer
        = new Layer(Type, Item.getValue(), Item.getID(), Item.getUnitClass());
  }
  catch (openfluid::base::OFException e)
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

  ALayer.update(*mp_CoreRepos);

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
}

// =====================================================================
// =====================================================================

bool Mediator::hasADisplayableVectorValue(openfluid::core::DatastoreItem& Item,
    openfluid::core::CoreRepository& CoreRepos)
{
  return Item.getValue() && Item.getValue()->getType()
      == openfluid::core::UnstructuredValue::GeoVectorValue
      && EngineHelper::getClassNames(&CoreRepos).count(Item.getUnitClass());
}

// =====================================================================
// =====================================================================

bool Mediator::hasADisplayableRasterValue(openfluid::core::DatastoreItem& Item)
{
  return Item.getValue() && Item.getValue()->getType()
      == openfluid::core::UnstructuredValue::GeoRasterValue;
}

// =====================================================================
// =====================================================================

Gtk::Widget* Mediator::asWidget()
{
  return mp_MainVBoxMediator;
}

// =====================================================================
// =====================================================================

void Mediator::setEngineRequirements(
    openfluid::machine::SimulationBlob& SimBlob)
{
  mp_CoreRepos = &SimBlob.getCoreRepository();
  mp_Datastore = &SimBlob.getDatastore();

  mp_AddLayersDialog->setEngineRequirements(SimBlob);

  addAvailableLayersFromDatastore();
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
    whenOnShow100FocusButtonClicked();
    m_IsFirstExposeEvent = false;
  }
  redraw();
  signal_DrawingAreaExposeEventChanged().emit();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Mediator::whenOnShow100FocusButtonClicked()
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

void Mediator::whenOnZoomSelectionFocusButtonClicked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitId.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't zoom in a selection without selection.\n\nPlease select a layer."));
  }
  else if (m_SelectedUnitId.empty())
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
            m_SelectedUnitId));
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomLayerFocusButtonClicked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitId.clear();
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

void Mediator::whenOnZoomCursorZoomTypeButtonClicked()
{
  if (mref_ToolBar.getZoomTypeToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(4);
    mref_DrawingArea.changeToZoomCursorState();
  }
  else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }

}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomFrameZoomTypeButtonClicked()
{
  if (mref_ToolBar.getZoomTypeToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(4);
    mref_DrawingArea.changeToZoomFrameState();
  }
  else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnSelectAllPreferenceMenuClicked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitId.clear();
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
        temp.swap(m_SelectedUnitId);
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnToggleSelectedPreferenceMenuClicked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitId.clear();
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
        for (itset = m_SelectedUnitId.begin(); itset != m_SelectedUnitId.end(); itset++)
        {
          std::set<int>::iterator it2;
          it2 = temp.find((*itset));
          temp.erase(it2);
        }
        temp.swap(m_SelectedUnitId);
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnAddLayerToolButtonClicked()
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

    mref_ToolBar.resetSensitiveToolBar(true);

    redraw();
  }

}

// =====================================================================
// =====================================================================

void Mediator::whenOnInfoToolButtonClicked()
{
  if (m_SelectedClassName == "" || m_SelectedUnitId.empty())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _("You can't have informations without select the corresponding layer"
            "before and the corresponding units.\n\nPlease select a layer and units."));
    return;
  }

  std::set<int> UnavailableIds;
  std::set<int> AvailableIds = m_SelectedUnitId;
  std::set<int>::iterator it;

  for (it = m_SelectedUnitId.begin(); it != m_SelectedUnitId.end(); ++it)
  {
    if (!mp_CoreRepos->getUnit(m_SelectedClassName, *it))
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
          _("Management"), *mp_CoreRepos);
      m_infoDialogCreate = true;
    }
    mp_InfoDialog->show(m_SelectedClassName, AvailableIds);
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnSelectObjectLayerToggleToolButtonClicked()
{
  if (mref_ToolBar.getSelectObjectLayerToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(2);
    mref_DrawingArea.changeToSelectState();
    mref_ToolBar.setSensitivePreferenceMenubar(true);
  }
  else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnMoveLayerToggleToolButtonClicked()
{
  if (mref_ToolBar.getMoveLayerToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(1);
    mref_DrawingArea.changeToMoveState();
  }
  else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnUnzoomCursorToggleToolButtonClicked()
{
  if (mref_ToolBar.getUnzoomCursorToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(3);
    mref_DrawingArea.changeToUnzoomCursorState();
  }
  else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }
}

// =====================================================================
// =====================================================================
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
  if (m_Layers.empty())
    mref_ToolBar.resetSensitiveToolBar(false);
  if (m_SelectedClassName == ClassName)
    m_SelectedUnitId.clear();
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnIsSelectedLayerClicked(std::string ClassName)
{
  m_SelectedClassName = ClassName;
  m_SelectedUnitId.clear();
  std::vector<Layer*>::iterator it;
  for (it = m_Layers.begin(); it < m_Layers.end(); it++)
  {
    if (ClassName != (*it)->getClassName())
      (*it)->setIsSelected(false);
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
    m_SelectedUnitId.clear();
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
          && !m_SelectedUnitId.empty())
      {
        int temp = (*it)->isObjectSelected(X, Y, mref_DrawingArea.getScale());
        if (temp != -1)
        {
          std::set<int>::iterator it2;
          it2 = m_SelectedUnitId.find(temp);
          if (it2 == m_SelectedUnitId.end()
              && mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.insert(temp);
          }
          else if (it2 == m_SelectedUnitId.end()
              && !mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.clear();
            m_SelectedUnitId.insert(temp);
          }
          else if (it2 != m_SelectedUnitId.end()
              && mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.erase(it2);
          }
          else if (it2 != m_SelectedUnitId.end()
              && !mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.clear();
          }
          isRedraw = true;
        }
        else
        {
          if (!m_SelectedUnitId.empty()
              && !mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.clear();
            isRedraw = true;
          }
        }
      }
      else if (m_SelectedClassName == (*it)->getClassName()
          && m_SelectedUnitId.empty())
      {
        int temp = (*it)->isObjectSelected(X, Y, mref_DrawingArea.getScale());
        if (temp != -1)
        {
          m_SelectedUnitId.insert(temp);
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
                m_SelectedUnitId, (*rit)->getDisplayID());
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
  //  if (m_Layers.empty())
  //    mref_DrawingArea.setLayerExist(false);
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


