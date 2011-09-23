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
 \file DrawingArea.hpp
 \brief Header of ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#ifndef __DRAWINGAREA_HPP__
#define __DRAWINGAREA_HPP__

#include <gtkmm/drawingarea.h>
//#include <sigc++-2.0/sigc++/connection.h>

class DrawingAreaInitialState;
class DrawingAreaMoveState;
class DrawingAreaSelectState;
class DrawingAreaUnzoomCursorState;
class DrawingAreaZoomCursorState;
class DrawingAreaZoomFrameState;
class DrawingAreaState;

class DrawingArea: public Gtk::DrawingArea
{

  private:

    DrawingAreaInitialState* mp_InitialState;
    DrawingAreaMoveState* mp_MoveState;
    DrawingAreaSelectState* mp_SelectState;
    DrawingAreaUnzoomCursorState* mp_UnzoomCursorState;
    DrawingAreaZoomCursorState* mp_ZoomCursorState;
    DrawingAreaZoomFrameState* mp_ZoomFrameState;
    DrawingAreaState* mp_CurrentState;

    double m_Scale;
    double m_XTranslate;
    double m_YTranslate;

    bool m_pressMultiSelect;
    bool m_LayerExist;
    bool m_ZoomFrame;
    bool m_SavePixBuf;

    Glib::RefPtr<Gdk::Pixbuf> mp_PixBufGDKWindow;

    sigc::signal<void> m_signal_ExposeEventChanged;
    sigc::signal<void, double, double> m_signal_CoordinateChanged;
    sigc::signal<void, double, double> m_signal_CoordinateSelected;

    bool whenOnKeyPressed(GdkEventKey*);
    bool whenOnKeyReleased(GdkEventKey*);

    sigc::connection m_KeyPress;
    sigc::connection m_KeyRelease;

  public:

    DrawingArea();
//        virtual ~DrawingArea();

    sigc::signal<void> signal_ExposeEventChanged();
    sigc::signal<void, double, double> signal_CoordinateChanged();
    sigc::signal<void, double, double> signal_CoordinateSelected();

    void changeToInitialState();
    void changeToMoveState();
    void changeToSelectState();
    void changeToUnzoomCursorState();
    void changeToZoomCursorState();
    void changeToZoomFrameState();

    void modifyScaleTranslate(
        std::pair<std::pair<double, double>, std::pair<double, double> >);

    //Accessor
    //get
    double getScale();
    double getXTranslate();
    double getYTranslate();
    bool getPressMultiSelect();
    //set
    void setScale(double);
    void setXTranslate(double);
    void setYTranslate(double);
    void setLayerExist(bool);
    void setZoomFrame(bool);
    void setSavePixBuf(bool);

  protected:

    virtual bool on_expose_event(GdkEventExpose*);

    virtual bool on_button_press_event(GdkEventButton* event);
    virtual bool on_button_release_event(GdkEventButton* event);
    virtual bool on_leave_notify_event(GdkEventCrossing* event);
    virtual bool on_enter_notify_event(GdkEventCrossing* event);
    virtual bool on_motion_notify_event(GdkEventMotion* event);
};

#endif /* __DRAWINGAREA_HPP__ */
