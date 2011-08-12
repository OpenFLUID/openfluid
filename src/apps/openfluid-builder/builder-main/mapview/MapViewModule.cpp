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
#include "Info.hpp"
#include "StatusBar.hpp"
#include "ToolBar.hpp"
#include "Mediator.hpp"
#include "MapViewModule.hpp"

MapViewModule::MapViewModule()
{

  mp_Statusbar = new StatusBar();
  mp_DrawingArea = Gtk::manage(new DrawingArea());
  mp_ToolBar = new ToolBar();
  mp_Info = new Info();
  mp_Mediator = new Mediator(*mp_DrawingArea, *mp_Info, *mp_Statusbar,
      *mp_ToolBar);

  //ScrolledWindow
  mp_MainScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_DrawScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_MenuScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_MenuControlScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  //Paned
  mp_HVisuPaned = Gtk::manage(new Gtk::HPaned());
  mp_VMenuPaned = Gtk::manage(new Gtk::VPaned());
  //Box
  mp_VBoxToolFrame = Gtk::manage(new Gtk::VBox());
  mp_VBoxStatusbarDrawingArea = Gtk::manage(new Gtk::VBox());
  //Frame
//  mp_DrawFrame = Gtk::manage(new Gtk::Frame());
  mp_ControlMenuFrame = Gtk::manage(new Gtk::Frame());
  mp_InfoMenuFrame = Gtk::manage(new Gtk::Frame());

  mp_VBoxStatusbarDrawingArea->set_spacing(2);

  mp_MainScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC,
      Gtk::POLICY_AUTOMATIC);
  mp_DrawScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC,
      Gtk::POLICY_AUTOMATIC);
  mp_MenuScrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC,
      Gtk::POLICY_AUTOMATIC);

//  mp_DrawFrame->set_label(_("Map"));
  mp_ControlMenuFrame->set_label(_("Layer"));
  mp_InfoMenuFrame->set_label(_("Info"));

//  mp_DrawFrame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
  mp_ControlMenuFrame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
  mp_InfoMenuFrame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

  mp_MenuControlScrolledWindow->add(*mp_Mediator->asWidget());
  mp_ControlMenuFrame->add(*mp_MenuControlScrolledWindow);
  ;
  mp_DrawScrolledWindow->add(*mp_DrawingArea);
//  mp_DrawFrame->add(*mp_DrawScrolledWindow);
  mp_VMenuPaned->pack1(*mp_ControlMenuFrame);
  mp_VMenuPaned->pack2(*mp_InfoMenuFrame);
  mp_MenuScrolledWindow->add(*mp_VMenuPaned);
  mp_VBoxStatusbarDrawingArea->pack_start(*mp_DrawScrolledWindow,
      Gtk::PACK_EXPAND_WIDGET);
  mp_VBoxStatusbarDrawingArea->pack_start(*mp_Statusbar->asWidget(),
      Gtk::PACK_SHRINK);
  mp_HVisuPaned->pack1(*mp_VBoxStatusbarDrawingArea, Gtk::EXPAND);
  mp_HVisuPaned->pack2(*mp_MenuScrolledWindow, Gtk::FILL | Gtk::EXPAND);
  mp_VBoxToolFrame->pack_start(*mp_ToolBar->asWidget(), Gtk::PACK_SHRINK);
  mp_VBoxToolFrame->pack_end(*mp_HVisuPaned, Gtk::PACK_EXPAND_WIDGET);
  mp_MainScrolledWindow->add(*mp_VBoxToolFrame);

  static_cast<Gtk::Viewport*> (mp_Mediator->asWidget()->get_parent())->set_shadow_type(
      Gtk::SHADOW_NONE);
  static_cast<Gtk::Viewport*> (mp_DrawingArea->get_parent())->set_shadow_type(
      Gtk::SHADOW_NONE);
  static_cast<Gtk::Viewport*> (mp_VMenuPaned->get_parent())->set_shadow_type(
      Gtk::SHADOW_NONE);
  static_cast<Gtk::Viewport*> (mp_VBoxToolFrame->get_parent())->set_shadow_type(
      Gtk::SHADOW_NONE);

  mp_MainScrolledWindow->show_all_children();
  mp_MainScrolledWindow->set_visible(true);

  mp_Mediator->signal_DrawingAreaExposeEventChanged().connect( sigc::mem_fun(*this, &MapViewModule::whenChanged));
}

// =====================================================================
// =====================================================================

Gtk::Widget* MapViewModule::asWidget()
{
  return mp_MainScrolledWindow;
}

// =====================================================================
// =====================================================================

void MapViewModule::setEngineRequirements(
    openfluid::machine::ModelInstance& /*ModelInstance*/,
    openfluid::machine::SimulationBlob& SimBlob)
{
  mp_Mediator->setEngineRequirements(SimBlob.getCoreRepository());
}

sigc::signal<void> MapViewModule::signal_ModuleChanged()
{
  return m_signal_MapViewChanged;
}

void MapViewModule::whenChanged()
{
  m_signal_MapViewChanged.emit();
}
