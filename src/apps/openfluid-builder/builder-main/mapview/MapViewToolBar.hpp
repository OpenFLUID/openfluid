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
 \file MapViewToolBar.hpp
 \brief Header of ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#ifndef __MAPVIEWTOOLBAR_HPP__
#define __MAPVIEWTOOLBAR_HPP__

#include <iostream>

#include <gtkmm/toolbar.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/menutoolbutton.h>
#include <gtkmm/stock.h>
#include <gtkmm/menu.h>
#include <gtkmm/separatortoolitem.h>

#include <glibmm/i18n.h>

#include "MapViewAction.hpp"
#include "MapViewTreeLayer.hpp"

class MapViewToolBar: public Gtk::Toolbar
{
  private:

    Glib::RefPtr<Gtk::AccelGroup> mref_MenuToolButtonZoomAccelGroup;
    Glib::RefPtr<Gtk::AccelGroup> mref_MenuToolButtonUnzoomAccelGroup;

    Gtk::MenuToolButton * mp_MenuButtonZoom;
    Gtk::MenuToolButton * mp_MenuButtonUnzoom;

    Gtk::Menu * mp_MenuZoom;
    Gtk::Menu * mp_MenuUnzoom;

    Gtk::ImageMenuItem * mp_ZoomCursor;
    Gtk::ImageMenuItem * mp_ZoomFrame;
    Gtk::ImageMenuItem * mp_ZoomObject;

    Gtk::ImageMenuItem * mp_UnzoomCursor;
    Gtk::ImageMenuItem * mp_UnzoomObject;

    Gtk::ToggleToolButton * mp_ToggleButtonMove;
    Gtk::ToggleToolButton * mp_ToggleButtonSelect;

    Gtk::SeparatorToolItem * setSeparator();

    MapViewTreeLayer& mref_MapViewTreeLayer;

    void onAddNewLayer();
    void onRemoveLayer();
    void onSelect();
    void onMove();
    void onShow100();
    void onInfo();

    void onZoomCursor();
    void onZoomFrame();
    void onZoomObject();
    void onUnzoomCursor();
    void onUnzoomObject();

    void onChangeZoomCursor(MapViewAction&);
    void onChangeZoomFrame(MapViewAction&);
    void onChangeZoomObject(MapViewAction&);
    void onChangeUnzoomCursor(MapViewAction&);
    void onChangeUnzoomObject(MapViewAction&);

  public:

    MapViewToolBar(MapViewAction&, MapViewTreeLayer&);
    virtual ~MapViewToolBar();

};

#endif /* __MAPVIEWTOOLBAR_HPP__ */
