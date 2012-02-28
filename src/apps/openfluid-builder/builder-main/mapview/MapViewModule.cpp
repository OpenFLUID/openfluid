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
 \file MapViewModule.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include <glibmm/i18n.h>

#include "DrawingArea.hpp"
#include "ToolBar.hpp"
#include "Mediator.hpp"
#include "MapViewModule.hpp"
#include "BuilderFrame.hpp"

MapViewModule::MapViewModule() :
  m_ScrollbarSpacing(3)
{
  mp_DrawingArea = Gtk::manage(new DrawingArea());
  mp_ToolBar = new ToolBar();
  mp_Statusbar = Gtk::manage(new Gtk::Statusbar());

  mp_Mediator = new Mediator(*mp_DrawingArea, *mp_Statusbar, *mp_ToolBar);

  mp_Mediator->signal_DrawingAreaExposeEventChanged().connect(sigc::mem_fun(
      *this, &MapViewModule::whenChanged));

  mp_Mediator->signal_ZoomWithRescaleAsked().connect(sigc::mem_fun(*this,
      &MapViewModule::resizeDrawingAreaScrolledWindow));
}

// =====================================================================
// =====================================================================

void MapViewModule::compose()
{
  mp_Statusbar->set_has_resize_grip(false);

  BuilderFrame* MenuFrame = Gtk::manage(new BuilderFrame());
  MenuFrame->setLabelText(_("Layers"));
  MenuFrame->add(*mp_Mediator->asWidget());

  Gtk::ScrolledWindow* MenuScrolledWindow = Gtk::manage(
      new Gtk::ScrolledWindow());
  MenuScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  MenuScrolledWindow->set_shadow_type(Gtk::SHADOW_NONE);
  MenuScrolledWindow->add(*MenuFrame);

  mp_DrawingAreaScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_DrawingAreaScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC,
      Gtk::POLICY_AUTOMATIC);
  mp_DrawingAreaScrolledWindow->set_shadow_type(Gtk::SHADOW_NONE);
  mp_DrawingAreaScrolledWindow->add(*mp_DrawingArea);

  mp_DrawingAreaScrolledWindow->get_style_property("scrollbar-spacing",
      m_ScrollbarSpacing);

  Gtk::VBox* MapBox = Gtk::manage(new Gtk::VBox());
  MapBox->pack_start(*mp_DrawingAreaScrolledWindow, Gtk::PACK_EXPAND_WIDGET);
  MapBox->pack_start(*mp_Statusbar, Gtk::PACK_SHRINK);

  mp_ContentPaned = Gtk::manage(new Gtk::HPaned());
  mp_ContentPaned->pack1(*MapBox, Gtk::FILL | Gtk::EXPAND);
  mp_ContentPaned->pack2(*MenuScrolledWindow, Gtk::FILL | Gtk::EXPAND);

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->pack_start(*mp_ToolBar->asWidget(), Gtk::PACK_SHRINK);
  mp_MainBox->pack_end(*mp_ContentPaned, Gtk::PACK_EXPAND_WIDGET);
  mp_MainBox->show_all_children();
  mp_MainBox->set_visible(true);
  mp_MainBox->set_can_focus(true);
}

// =====================================================================
// =====================================================================

Gtk::Widget* MapViewModule::asWidget()
{
  return mp_MainBox;
}

// =====================================================================
// =====================================================================

void MapViewModule::setEngineRequirements(
    openfluid::machine::ModelInstance& /*ModelInstance*/,
    openfluid::machine::SimulationBlob& SimBlob)
{
  mp_Mediator->setEngineRequirements(SimBlob);
}

// =====================================================================
// =====================================================================

sigc::signal<void> MapViewModule::signal_ModuleChanged()
{
  return m_signal_MapViewChanged;
}

// =====================================================================
// =====================================================================

void MapViewModule::whenChanged()
{
  m_signal_MapViewChanged.emit();
}

// =====================================================================
// =====================================================================

void MapViewModule::resizeDrawingAreaScrolledWindow()
{
  Gtk::Allocation DrawingAreaViewportAllocation =
      mp_DrawingArea->get_parent()->get_allocation();

  double RequiredWidth = DrawingAreaViewportAllocation.get_width()
      - m_ScrollbarSpacing;
  double RequiredHeight = DrawingAreaViewportAllocation.get_height()
      - m_ScrollbarSpacing;

  mp_DrawingArea->set_size_request(RequiredWidth, RequiredHeight);

  mp_DrawingAreaScrolledWindow->resize_children();
}
