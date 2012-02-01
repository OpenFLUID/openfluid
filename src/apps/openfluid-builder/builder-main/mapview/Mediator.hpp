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
 \file Mediator.hpp
 \brief Header of ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#ifndef __MEDIATOR_HPP__
#define __MEDIATOR_HPP__

#include <iostream>
#include <vector>
#include <set>
#include <string>

#include <gtkmm/box.h>
#include <gtkmm/separator.h>

#include "DrawingArea.hpp"
#include "Info.hpp"
#include "StatusBar.hpp"
#include "ToolBar.hpp"
#include "Layer.hpp"

namespace openfluid {
namespace core {
class Datastore;
class DatastoreItem;
}
namespace machine {
class SimulationBlob;
}
}

class MapViewAddLayersDialog;

class Mediator
{

  private:

    bool m_infoDialogCreate;

    MapViewAddLayersDialog* mp_AddLayersDialog;

    DrawingArea& mref_DrawingArea;
    StatusBar& mref_StatusBar;
    ToolBar& mref_ToolBar;

    openfluid::core::CoreRepository* mp_CoreRepos;
    openfluid::core::Datastore* mp_Datastore;

    bool m_IsFirstExposeEvent;

    std::vector<Layer*> m_Layers;
    std::set<std::string> m_LayersIds;

    std::string m_SelectedClassName;
    std::set<int> m_SelectedUnitId;

    //GTKmm

    Info* mp_InfoDialog;

    Gtk::VBox* mp_MainVBoxMediator;

    sigc::signal<void> m_signal_DrawingAreaExposeEventChanged;

    void addAvailableLayersFromDatastore();
    void addALayer(Layer& ALayer);

    void whenDrawingAreaChanged();
    void whenDrawingAreaRealized();
    void whenDrawingAreaMapped();
    bool whenDrawingAreaMapEvent(GdkEventAny* Event);

    //***************Signal ToolBar*********************
    void whenOnShow100FocusButtonClicked();
    void whenOnZoomSelectionFocusButtonClicked();
    void whenOnZoomLayerFocusButtonClicked();

    void whenOnZoomCursorZoomTypeButtonClicked();
    void whenOnZoomFrameZoomTypeButtonClicked();

    void whenOnSelectAllPreferenceMenuClicked();
    void whenOnToggleSelectedPreferenceMenuClicked();

    void whenOnAddLayerToolButtonClicked();
    void whenOnInfoToolButtonClicked();
    void whenOnSelectObjectLayerToggleToolButtonClicked();
    void whenOnMoveLayerToggleToolButtonClicked();
    void whenOnUnzoomCursorToggleToolButtonClicked();
    //***************Signal Layer***********************
    void whenOnUpLayerButtonClicked(std::string);
    void whenOnDownLayerButtonClicked(std::string);
    void whenOnRemoveLayerButtonClicked(std::string);
    void whenOnIsSelectedLayerClicked(std::string);
    void whenOnIsDisplayButtonChecked();
    void whenOnWidgetExpanderBaseChanged();
    //***************Signal DrawingArea*****************
    void whenOnMotionNotifyChanged(double, double);
    void whenOnSelectObjectChanged(double, double);

    void upLayer(std::string);
    void downLayer(std::string);
    void removeLayer(std::string);
    void removeAllObjectMainVBoxMediator();
    void addAllObjectMainVBoxMediator();

  public:

    Mediator(DrawingArea&, StatusBar&, ToolBar&);

    Gtk::Widget* asWidget();

    void setEngineRequirements(openfluid::machine::SimulationBlob& SimBlob);

    sigc::signal<void> signal_DrawingAreaExposeEventChanged();
    void redraw();

    static bool hasADisplayableVectorValue(
        openfluid::core::DatastoreItem& Item,
        openfluid::core::CoreRepository& CoreRepos);

    static bool
        hasADisplayableRasterValue(openfluid::core::DatastoreItem& Item);

    static Layer* tryToCreateALayerFromADatastoreItem(
        openfluid::core::DatastoreItem& Item);

};

#endif /* __MEDIATOR_HPP__ */
