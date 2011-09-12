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

#include "LayerType.hpp"
#include "Mediator.hpp"

#include "EngineHelper.hpp"

Mediator::Mediator(DrawingArea& DrawingArea, Info& Info, StatusBar& StatusBar,
    ToolBar& ToolBar) :
  mref_DrawingArea(DrawingArea), mref_Info(Info), mref_StatusBar(StatusBar),
      mref_ToolBar(ToolBar), mp_CoreRepos(0)
{
  m_SelectedClassName = "";

  mp_MainVBoxMediator = Gtk::manage(new Gtk::VBox());

  mref_DrawingArea.signal_ExposeEventChanged().connect(
      sigc::mem_fun(*this, &Mediator::whenDrawingAreaChanged));
  //***************Signal ToolBar*********************
  mref_ToolBar.signalAddLayerToolButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnAddLayerToolButtonClicked));
  mref_ToolBar.signalInfoToolButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnInfoToolButtonClicked));
  mref_ToolBar.signalMoveLayerToggleToolButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnMoveLayerToggleToolButtonClicked));
  mref_ToolBar.signalSelectAllPreferenceMenuClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnSelectAllPreferenceMenuClicked));
  mref_ToolBar.signalSelectObjectLayerToggleToolButtonClicked().connect(
      sigc::mem_fun(*this,
          &Mediator::whenOnSelectObjectLayerToggleToolButtonClicked));
  mref_ToolBar.signalShow100FocusButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnShow100FocusButtonClicked));
  mref_ToolBar.signalToggleSelectedPreferenceMenuClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnToggleSelectedPreferenceMenuClicked));
  mref_ToolBar.signalUnzoomCursorToggleToolButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnUnzoomCursorToggleToolButtonClicked));
  mref_ToolBar.signalZoomCursorZoomTypeButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnZoomCursorZoomTypeButtonClicked));
  mref_ToolBar.signalZoomFrameZoomTypeButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnZoomFrameZoomTypeButtonClicked));
  mref_ToolBar.signalZoomLayerFocusButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnZoomLayerFocusButtonClicked));
  mref_ToolBar.signalZoomSelectionFocusButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnZoomSelectionFocusButtonClicked));
  //***************Signal DrawingArea*****************
  mref_DrawingArea.signal_CoordinateChanged().connect(
      sigc::mem_fun(*this, &Mediator::whenOnMotionNotifyChanged));
  mref_DrawingArea.signal_CoordinateSelected().connect(
      sigc::mem_fun(*this, &Mediator::whenOnSelectObjectChanged));

  mp_MainVBoxMediator->set_visible(true);
}

// =====================================================================
// =====================================================================

Gtk::Widget* Mediator::asWidget()
{
  return mp_MainVBoxMediator;
}

// =====================================================================
// =====================================================================

void Mediator::setEngineRequirements(openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;
}

// =====================================================================
// =====================================================================

sigc::signal<void> Mediator::signal_DrawingAreaExposeEventChanged()
{
  return m_signal_DrawingAreaExposeEventChanged;
}

// =====================================================================
// =====================================================================

void Mediator::whenDrawingAreaChanged()
{
  //  m_signal_DrawingAreaExposeEventChanged.emit();
  redraw();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Mediator::whenOnShow100FocusButtonClicked()
{
  std::vector<Layer*>::iterator it;
  std::pair<std::pair<double, double>, std::pair<double, double> > MinMax;
  MinMax.first.first = (*m_Layer.begin())->getMinMaxLayer().first.first;
  MinMax.first.second = (*m_Layer.begin())->getMinMaxLayer().first.second;
  MinMax.second.first = (*m_Layer.begin())->getMinMaxLayer().second.first;
  MinMax.second.second = (*m_Layer.begin())->getMinMaxLayer().second.second;
  for (it = m_Layer.begin(); it < m_Layer.end(); it++)
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
  mref_DrawingArea.modifyScaleTranslate(MinMax);
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomSelectionFocusButtonClicked()
{

}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomLayerFocusButtonClicked()
{

}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomCursorZoomTypeButtonClicked()
{
  if (mref_ToolBar.getZoomTypeToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(4);
    mref_DrawingArea.changeToZoomCursorState();
  } else
    mref_DrawingArea.changeToInitialState();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomFrameZoomTypeButtonClicked()
{
  if (mref_ToolBar.getZoomTypeToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(4);
    mref_DrawingArea.changeToZoomFrameState();
  } else
    mref_DrawingArea.changeToInitialState();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnSelectAllPreferenceMenuClicked()
{

}

// =====================================================================
// =====================================================================

void Mediator::whenOnToggleSelectedPreferenceMenuClicked()
{

}

// =====================================================================
// =====================================================================

void Mediator::whenOnAddLayerToolButtonClicked()
{
  Layer* pLayer;
  pLayer = new Layer(LayerType::LAYER_BASE);
  m_Layer.push_back(pLayer);

  mref_DrawingArea.setLayerExist(true);

  std::vector<std::string> ClassNames;

  std::set<std::string>::iterator it;
  for (it = EngineHelper::getClassNames(mp_CoreRepos).begin(); it
      != EngineHelper::getClassNames(mp_CoreRepos).end(); it++)
  {
    std::string Temp = *it;
    bool ClassNameExist = false;
    std::vector<Layer*>::iterator ite;
    for (ite = m_Layer.begin(); ite < m_Layer.end(); ite++)
    {
      if (Temp == (*ite)->getClassName())
        ClassNameExist = true;
    }
    if (ClassNameExist == false)
      ClassNames.push_back(Temp);
  }

  pLayer->addNewLayer(ClassNames);

  if (pLayer->getLoadShapeFile())
  {
    //    pLayer->getICLayer()->setEngineRequirements(*mp_CoreRepos);
    pLayer->update(*mp_CoreRepos);
    if (m_Layer.size() == 1)
    {
      mref_DrawingArea.modifyScaleTranslate(pLayer->getMinMaxLayer());
    }
    removeAllObjectMainVBoxMediator();
    addAllObjectMainVBoxMediator();

    //***************Signal Layer*********************
    pLayer->signalUpLayerButtonClicked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnUpLayerButtonClicked));
    pLayer->signalDownLayerButtonClicked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnDownLayerButtonClicked));
    pLayer->signalRemoveLayerButtonClicked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnRemoveLayerButtonClicked));
    pLayer->signalIsDisplayButtonChecked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnIsDisplayButtonChecked));
    pLayer->signalIsSelectedLayerClicked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnIsSelectedLayerClicked));
    pLayer->signalWidgetExpanderBaseChanged().connect(
        sigc::mem_fun(*this, &Mediator::whenOnWidgetExpanderBaseChanged));

    redraw();
  } else
    m_Layer.pop_back();

}

// =====================================================================
// =====================================================================

void Mediator::whenOnInfoToolButtonClicked()
{

}

// =====================================================================
// =====================================================================

void Mediator::whenOnSelectObjectLayerToggleToolButtonClicked()
{
  if (mref_ToolBar.getSelectObjectLayerToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(2);
    mref_DrawingArea.changeToSelectState();
  } else
    mref_DrawingArea.changeToInitialState();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnMoveLayerToggleToolButtonClicked()
{
  if (mref_ToolBar.getMoveLayerToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(1);
    mref_DrawingArea.changeToMoveState();
  } else
    mref_DrawingArea.changeToInitialState();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnUnzoomCursorToggleToolButtonClicked()
{
  if (mref_ToolBar.getUnzoomCursorToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(3);
    mref_DrawingArea.changeToUnzoomCursorState();
  } else
    mref_DrawingArea.changeToInitialState();
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
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnIsSelectedLayerClicked(std::string ClassName)
{
  m_SelectedClassName = ClassName;
  std::vector<Layer*>::iterator it;
  for (it = m_Layer.begin(); it < m_Layer.end(); it++)
  {
    if (ClassName != (*it)->getClassName())
      (*it)->setIsSelected(false);
  }

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
  std::stringstream str1;
  std::stringstream str2;
  std::string Xstr;
  std::string Ystr;
  if (!m_Layer.empty())
  {
    str1 << X;
    str2 << Y;
    Xstr = str1.str();
    Ystr = str2.str();
  } else
  {
    str1 << 0;
    str2 << 0;
    Xstr = str1.str();
    Ystr = str2.str();
  }
  mref_StatusBar.setXLabelCoordinate(Xstr);
  mref_StatusBar.setYLabelCoordinate(Ystr);
}

// =====================================================================
// =====================================================================

void Mediator::whenOnSelectObjectChanged(double X, double Y)
{
  redraw();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

Gtk::HSeparator* Mediator::setHSeparator()
{
  Gtk::HSeparator * p_Separator = Gtk::manage(new Gtk::HSeparator());
  p_Separator->set_visible(true);
  return p_Separator;
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Mediator::redraw()
{
  mref_DrawingArea.get_window()->clear();
  Glib::RefPtr<Gdk::Window> Window = mref_DrawingArea.get_window();
  //
  if (Window)
  {
    if (!m_Layer.empty())
    {
      Cairo::RefPtr<Cairo::Context> Context = Window->create_cairo_context();
      Gtk::Allocation allocation = mref_DrawingArea.get_allocation();
      const int width = allocation.get_width();
      const int height = allocation.get_height();

      // clip to the area indicated by the expose event so that we only redraw
      // the portion of the window that needs to be redrawn
      Context->rectangle(0, 0, width, height);
      Context->clip();
      Context->set_antialias(Cairo::ANTIALIAS_SUBPIXEL);
      Context->scale(mref_DrawingArea.getScale(), -mref_DrawingArea.getScale());
      Context->translate(-mref_DrawingArea.getXTranslate(),
          -mref_DrawingArea.getYTranslate());
      std::vector<Layer*>::reverse_iterator rit;
      for (rit = m_Layer.rbegin(); rit < m_Layer.rend(); ++rit)
      {
        if ((*rit)->getIsDisplay())
        {
          (*rit)->initialiseLayerContext(Context, mref_DrawingArea.getScale());
          (*rit)->draw(Context, mref_DrawingArea.getScale());
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
  for (rit = m_Layer.rbegin(); rit < m_Layer.rend(); ++rit)
  {
    if ((*rit)->getClassName() == ClassName)
    {
      pLayer = (*rit);
      m_Layer.erase(--rit.base());
      ++rit;
      m_Layer.insert(--rit.base(), pLayer);
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
  for (it = m_Layer.begin(); it < m_Layer.end(); it++)
  {
    if ((*it)->getClassName() == ClassName)
    {
      pLayer = (*it);
      m_Layer.erase(it);
      it++;
      m_Layer.insert(it, pLayer);
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
  for (it = m_Layer.begin(); it < m_Layer.end(); it++)
  {
    if ((*it)->getClassName() == ClassName)
    {
      m_Layer.erase(it);
    }
  }
  addAllObjectMainVBoxMediator();
  if (m_Layer.empty())
    mref_DrawingArea.setLayerExist(false);
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
  if (!m_Layer.empty())
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layer.begin(); it < m_Layer.end(); it++)
    {
      mp_MainVBoxMediator->pack_start(*(*it)->asWidget(), Gtk::PACK_SHRINK);
      (*it)->setWidgetUpSensitive(true);
      (*it)->setWidgetDownSensitive(true);
    }
    m_Layer.front()->setWidgetUpSensitive(false);
    m_Layer.back()->setWidgetDownSensitive(false);
  }

}

// =====================================================================
// =====================================================================
