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
 \file MapViewModule.hpp
 \brief Header of ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#ifndef __MAPVIEWMODULE_HPP__
#define __MAPVIEWMODULE_HPP__

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/paned.h>
#include <gtkmm/frame.h>
#include <gtkmm/viewport.h>
#include <gtkmm/statusbar.h>

#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/guicommon/ProjectWorkspaceModule.hpp>

class DrawingArea;
class Info;
class ToolBar;
class Mediator;

class MapViewModule: public openfluid::guicommon::ProjectWorkspaceModule
{
  private:
  
    Gtk::VBox* mp_MainBox;
    DrawingArea* mp_DrawingArea;
    ToolBar* mp_ToolBar;
    Gtk::Statusbar* mp_Statusbar;

    Gtk::ScrolledWindow* mp_DrawingAreaScrolledWindow;
    Gtk::HPaned* mp_ContentPaned;

    Mediator* mp_Mediator;

    int m_ScrollbarSpacing;

    sigc::signal<void> m_signal_MapViewChanged;

    void whenChanged();

    void resizeDrawingAreaScrolledWindow();

  public:

    MapViewModule();

    void compose();

    Gtk::Widget* asWidget();

    void setEngineRequirements(
        openfluid::machine::ModelInstance& ModelInstance,
        openfluid::machine::SimulationBlob& SimBlob,
        openfluid::fluidx::FluidXDescriptor& FXDesc);

    void update()
    {
    }
    ;

    sigc::signal<void> signal_ModuleChanged();

};

#endif /* __MAPVIEWMODULE_HPP__ */
