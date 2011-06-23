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
 \file MapViewDrawingArea.hpp
 \brief Header of ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#ifndef __MAPVIEWDRAWINGAREA_HPP__
#define __MAPVIEWDRAWINGAREA_HPP__

#include <iostream>
#include <vector>
//#include <gtkmm-2.4/gtkmm/drawingarea.h>
#include <cairomm-1.0/cairomm/context.h>
//#include <cairomm-1.0/cairomm/matrix.h>
#include <gdal/ogr_geometry.h>
#include <gdal/ogr_feature.h>
#include <time.h>

#include "ICLayer.hpp"
#include "ICLayerPoint.hpp"
#include "ICLayerLine.hpp"
#include "ICLayerPoly.hpp"
#include "MapViewStatusBar.hpp"

#include <glibmm.h>
#include <gtkmm.h>
#include <gdkmm.h>

class MapViewDrawingArea: public Gtk::DrawingArea
{
  private:

    int m_NumLayer;
    int m_IndexCourrant;

    bool m_InitTranslate;

    std::vector<ICLayer*> m_ICLayer;

    bool m_Move;
    bool m_Select;
    bool m_ZoomFrame;
    bool m_ZoomCursor;
    bool m_UnzoomCursor;


    double m_XTranslate;
    double m_YTranslate;
    double m_XTranslateOrigine;
    double m_YTranslateOrigine;

    double m_XPress;
    double m_YPress;
    double m_XRelease;
    double m_YRelease;

    double m_maxX;
    double m_maxY;
    double m_minX;
    double m_minY;

    double m_scale;
    double m_scaleOrigine;

    Glib::RefPtr<Gdk::Window> window;
    Gtk::Allocation allocation;

    int m_Width;
    int m_Height;

    MapViewStatusBar& mref_Statusbar;

    void drawPoly(const std::vector<std::pair<double, double> > &,
        Cairo::RefPtr<Cairo::Context>);
    void drawPoint(const std::pair<double, double> &,
        Cairo::RefPtr<Cairo::Context>);

    void loadShapefile(std::string, std::string);

    void setScaleOrigine();

  public:

    MapViewDrawingArea(MapViewStatusBar&);
    virtual ~MapViewDrawingArea();

    void addLayer(std::string, std::string);

    void calulMaxMinCoordinate();

    void setWidthHeight();

    void setNumLayer(int);
    int getNumLayer();

    bool getMove();
    void setMove(bool);

    bool getSelect();
    void setSelect(bool);

    bool getZoomFrame();
    void setZoomFrame(bool);

    bool getZoomCursor();
    void setZoomCursor(bool);

    bool getUnzoomCursor();
    void setUnzoomCursor(bool);

    void setInitTranslate(bool);

    void setScale(double);

    double getScaleOrigine();

    std::vector<ICLayer*> getICLayer();

    void removeICLayer(int);
    bool geton_expose_event();

    double transformCoordinate(double);

  protected:

    //Override default signal handler:
    virtual bool on_expose_event(GdkEventExpose* event);
    bool on_event_happend(GdkEvent* event);

};

#endif /* __MAPVIEWDRAWINGAREA_HPP__ */

