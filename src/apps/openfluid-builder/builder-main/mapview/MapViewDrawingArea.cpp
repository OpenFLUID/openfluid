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
 \file MapViewDrawingArea.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "MapViewDrawingArea.hpp"

MapViewDrawingArea::MapViewDrawingArea(MapViewStatusBar& Statusbar) :
  mref_Statusbar(Statusbar)
{

  m_NumLayer = 0;
  m_IndexCourrant = 0;

  m_Width = 1;
  m_Height = 1;

  m_XTranslate = 0;
  m_YTranslate = 0;
  m_XTranslateOrigine = 0;
  m_YTranslateOrigine = 0;

  m_scale = 0;
  m_scaleOrigine = 0;

  m_InitTranslate = false;
  m_Move = false;
  m_Select = false;
  m_ZoomFrame = false;
  m_ZoomCursor = false;
  m_UnzoomCursor = false;

  this->add_events(Gdk::BUTTON_PRESS_MASK);
  this->add_events(Gdk::POINTER_MOTION_MASK);
  this->add_events(Gdk::BUTTON_RELEASE_MASK);
  this->signal_event().connect(
      sigc::mem_fun(*this, &MapViewDrawingArea::on_event_happend));

  modify_bg(Gtk::StateType(NULL), Gdk::Color("#FFFFFF"));

#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
  //Connect the signal handler if it isn't already a virtual method override:
  signal_expose_event().connect(
      sigc::mem_fun(*this, &MapViewDrawingArea::on_expose_event), false);
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
}

// =====================================================================
// =====================================================================

MapViewDrawingArea::~MapViewDrawingArea()
{
  for (unsigned int i = 0; i < m_ICLayer.size(); i++)
  {
    delete m_ICLayer[i];
    m_ICLayer[i] = 0;
  }
}

// =====================================================================
// =====================================================================

bool MapViewDrawingArea::on_expose_event(GdkEventExpose* event)
{
  geton_expose_event();

  return true;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::calulMaxMinCoordinate()
{
  for (unsigned int index = 0; index < m_ICLayer.size(); index++)
  {
    if (index == 0)
    {
      m_maxX = m_ICLayer[0]->getmaxX();
      m_maxY = m_ICLayer[0]->getmaxY();
      m_minX = m_ICLayer[0]->getminX();
      m_minY = m_ICLayer[0]->getminY();
    } else
    {
      if (m_maxX < m_ICLayer[index]->getmaxX())
      {
        m_maxX = m_ICLayer[index]->getmaxX();
      }
      if (m_maxY < m_ICLayer[index]->getmaxY())
      {
        m_maxY = m_ICLayer[index]->getmaxY();
      }
      if (m_minX > m_ICLayer[index]->getminX())
      {
        m_minX = m_ICLayer[index]->getminX();
      }
      if (m_minY > m_ICLayer[index]->getminY())
      {
        m_minY = m_ICLayer[index]->getminY();
      }
    }
  }
  if (!m_InitTranslate)
  {
    m_XTranslate = -m_minX;
    m_YTranslate = -m_maxY;
    m_InitTranslate = true;
  }
  m_XTranslateOrigine = -m_minX;
  m_YTranslateOrigine = -m_maxY;

  std::cout << " max min X  = " << m_maxX << " :: " << m_minX << std::endl;
  std::cout << " max min Y  = " << m_maxY << " :: " << m_minY << std::endl;

  setWidthHeight();
  setScaleOrigine();
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setWidthHeight()
{
  Gtk::Allocation allocation = get_allocation();
  m_Width = allocation.get_width();
  m_Height = allocation.get_height();
  std::cout << allocation.get_width() << " :: " << allocation.get_height()
      << std::endl;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::addLayer(std::string path, std::string file)
{
  loadShapefile(path, file);
  calulMaxMinCoordinate();
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::loadShapefile(std::string path, std::string file)
{
  OGRRegisterAll();

  OGRDataSource *File;

  File = OGRSFDriverRegistrar::Open((char*) path.c_str(), FALSE);
  if (File == NULL)
  {

    std::cout << "Open failed.\n" << std::endl;
    exit(1);

  } else
    std::cout << "Open OK !!!" << std::endl;

  OGRLayer *Layer = 0;
  Layer = File->GetLayerByName((char*) file.c_str());

  if (Layer)
  {
    Layer->ResetReading();

    OGRFeature* Feature;
    int type;
    ICLayer* ICLayer = 0;
    if (Layer->GetFeatureCount() > 0)
    {
      switch (Layer->GetNextFeature()->GetGeometryRef()->getGeometryType())
      {
        case wkbPoint:
        {
          type = wkbPoint;
          ICLayer = new ICLayerPoint();
          break;
        }
        case wkbPolygon:
        {
          type = wkbPolygon;
          ICLayer = new ICLayerPoly();
          break;
        }
        case wkbLineString:
        {
          type = wkbLineString;
          ICLayer = new ICLayerLine();
          break;
        }
        default:
        {
          type = wkbUnknown;
          std::cerr << "bad geometry ref type" << std::endl;
        }
          break;
      }
    } else
      std::cerr << "no feature" << std::endl;

    Layer->ResetReading();

    while ((Feature = Layer->GetNextFeature()) != NULL)
    {
      OGRGeometry* poGeometry = Feature->GetGeometryRef();
      OGRGeometry* GeomCopy = poGeometry->clone();

      ICLayer->addObjectGeo(GeomCopy);

      OGRFeature::DestroyFeature(Feature);
    }
    m_ICLayer.push_back(ICLayer);
    ICLayer->setId(m_NumLayer);
    ICLayer->setPosition(m_NumLayer);
    ICLayer->setIsDisplay(true);
    ICLayer->settype(type);
    ICLayer->setIsSelected(false);

  } else
    std::cerr << "layer of file " << file << "not found" << std::endl;

  OGRDataSource::DestroyDataSource(File);

  calulMaxMinCoordinate();
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setNumLayer(int NumLayer)
{
  m_NumLayer = NumLayer;
}

// =====================================================================
// =====================================================================

int MapViewDrawingArea::getNumLayer()
{
  return m_NumLayer;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::removeICLayer(int Position)
{
  m_ICLayer.erase(m_ICLayer.begin() + Position);
  m_maxX = 0;
  m_maxY = 0;
  m_minX = 0;
  m_minY = 0;
  if (!m_ICLayer.empty())
  {
    calulMaxMinCoordinate();
  } else
  {
    mref_Statusbar.setXEntryText("0");
    mref_Statusbar.setYEntryText("0");
    m_InitTranslate = false;
  }
}

// =====================================================================
// =====================================================================

std::vector<ICLayer*> MapViewDrawingArea::getICLayer()
{
  return m_ICLayer;
}

// =====================================================================
// =====================================================================

bool MapViewDrawingArea::geton_expose_event()
{
  // This is where we draw on the window

  get_window()->clear();
  Glib::RefPtr<Gdk::Window> window = get_window();

  if (window)
  {

    //TODO check div par zero
    if (!m_ICLayer.empty())
    {
      if (m_scale == 0)
        m_scale = m_scaleOrigine;
      //      m_scale = std::min(m_Width / (m_maxX - m_minX),
      //          m_Height / (m_maxY - m_minY));
      Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

      // clip to the area indicated by the expose event so that we only redraw
      // the portion of the window that needs to be redrawn
      //      cr->rectangle(0, 0, m_Width, m_Height);
      //      cr->clip();

      cr->set_antialias(Cairo::ANTIALIAS_SUBPIXEL);

      // draw red lines out from the center of the window

      //      cr->scale(m_scale - (m_scale / 100), -(m_scale - (m_scale / 100)));
      cr->scale(m_scale, -m_scale);
      //      cr->translate(-m_minX + (10 * m_scale), -m_maxY - (10 * m_scale));
      cr->translate(m_XTranslate, m_YTranslate);
      //      clock_t start, end;
      //      double elapsed;
      //
      //      start = clock();
      int compt = m_ICLayer.size();
      //      Gdk::Cursor Cursor(Gdk::WATCH);
      //      get_window()->set_cursor(Cursor);
      while (compt >= 0)
      {
        for (unsigned int index = 0; index < m_ICLayer.size(); index++)
        {
          //TODO finir le changement pour que les couleur s'affiche en fonction des position

          if (m_ICLayer.at(index)->getPosition() == compt)
          {
            if (m_ICLayer.at(index)->getIsDisplay() == true)
            {
              m_IndexCourrant = index;
              cr->set_line_width(m_ICLayer.at(index)->getSizeLine() / m_scale);
              cr->set_source_rgba(m_ICLayer.at(index)->getRed(),
                  m_ICLayer.at(index)->getGreen(),
                  m_ICLayer.at(index)->getBlue(),
                  m_ICLayer.at(index)->getAlpha());

              m_ICLayer.at(index)->draw(cr, m_scale);
            }

          }

        }
        compt -= 1;
      }
      //      end = clock(); /* Arret de la mesure     */
      //
      //      elapsed = ((double) end - start) / CLOCKS_PER_SEC; /* Conversion en seconde  */
      //
      //      printf("%.2f secondes entre start et end.\n", elapsed);
    }
  }
  return true;
}

bool MapViewDrawingArea::on_event_happend(GdkEvent* event)
{
  if (event->type == GDK_MOTION_NOTIFY)
  {
    if (!m_ICLayer.empty())
    {
      double X = ((((GdkEventMotion *) event)->x) / m_scale) + m_minX;
      double Y =
          (m_maxY - ((((GdkEventMotion *) event)->y / m_scale) + m_minY))
              + m_minY;

      X += m_XTranslateOrigine - m_XTranslate;
      Y += m_YTranslateOrigine - m_YTranslate;

      std::stringstream str1;
      std::stringstream str2;
      str1 << X;
      str2 << Y;
      std::string Xstr = str1.str();
      std::string Ystr = str2.str();

      mref_Statusbar.setXEntryText(Xstr);
      mref_Statusbar.setYEntryText(Ystr);
    } else
    {
      mref_Statusbar.setXEntryText("0");
      mref_Statusbar.setYEntryText("0");
    }
    if (m_Move)
    {
      Gdk::Cursor Cursor(Gdk::HAND1);
      get_window()->set_cursor(Cursor);
    } else if (m_Select)
    {
      Gdk::Cursor Cursor(Gdk::CROSSHAIR);
      get_window()->set_cursor(Cursor);
    } else if (m_ZoomFrame)
    {
      Gdk::Cursor Cursor(Gdk::SIZING);
      get_window()->set_cursor(Cursor);
    } else if (m_ZoomCursor)
    {
      Gdk::Cursor Cursor(Gdk::BOGOSITY);
      get_window()->set_cursor(Cursor);
    } else if (m_UnzoomCursor)
    {
      Gdk::Cursor Cursor(Gdk::BOX_SPIRAL);
      get_window()->set_cursor(Cursor);
    } else
    {
      Gdk::Cursor Cursor(Gdk::LEFT_PTR);
      get_window()->set_cursor(Cursor);
    }

  }
  if (event->type == GDK_BUTTON_PRESS)
  {

    m_XPress = (event->button.x / m_scale) + m_minX;
    m_YPress = (m_maxY - ((event->button.y / m_scale) + m_minY)) + m_minY;
    //    std::cout << "Press1: X= " << m_XPress << " Y= " << m_YPress << std::endl;
    m_XPress += m_XTranslateOrigine - m_XTranslate;
    m_YPress += m_YTranslateOrigine - m_YTranslate;
//    std::cout << "Press2: X= " << m_XPress << " Y= " << m_YPress << std::endl;
    if (m_ZoomCursor)
    {
      //      std::cout << m_scale << " :: " << m_FactorScale << std::endl;
      m_scale += (m_scale / 4);

      m_XTranslate = -(m_XPress - ((m_Width / m_scale) / 2));
      m_YTranslate = -(m_YPress + ((m_Height / m_scale) / 2));
      geton_expose_event();
    }
    if (m_UnzoomCursor)
    {
      //      std::cout << m_scale << " :: " << m_FactorScale << std::endl;
      m_scale -= (m_scale / 4);

      m_XTranslate = -(m_XPress - ((m_Width / m_scale) / 2));
      m_YTranslate = -(m_YPress + ((m_Height / m_scale) / 2));
      geton_expose_event();
    }
  }
  if (event->type == GDK_BUTTON_RELEASE)
  {
    if (m_Move)
    {
      m_XRelease = (event->button.x / m_scale) + m_minX;
      m_YRelease = (m_maxY - ((event->button.y / m_scale) + m_minY)) + m_minY;
      m_XRelease += m_XTranslateOrigine - m_XTranslate;
      m_YRelease += m_YTranslateOrigine - m_YTranslate;
      m_XTranslate = -(abs(m_XTranslate) + (m_XPress - m_XRelease));
      m_YTranslate = -(abs(m_YTranslate) + (m_YPress - m_YRelease));
      geton_expose_event();
    }
    if (m_ZoomFrame)
    {
      m_XRelease = (event->button.x / m_scale) + m_minX;
      m_YRelease = (m_maxY - ((event->button.y / m_scale) + m_minY)) + m_minY;
      m_XRelease += m_XTranslateOrigine - m_XTranslate;
      m_YRelease += m_YTranslateOrigine - m_YTranslate;
      //      std::cout << "Release2 : X= " << m_XRelease << " Y= " << m_YRelease
      //          << " :: " << m_XTranslate << std::endl;
      double x, x1, x2, xt;
      double y, y1, y2, yt;

      x1 = m_XPress;
      x2 = m_XRelease;
      y1 = m_YPress;
      y2 = m_YRelease;

      if (x1 > x2)
      {
        x = x1 - x2;
        xt = x2;
      } else
      {
        x = x2 - x1;
        xt = x1;
      }
      if (y1 > y2)
      {
        y = y1 - y2;
        yt = y2;
      } else
      {
        y = y2 - y1;
        yt = y1;
      }

      m_scale = std::min(m_Width / abs(x), m_Height / abs(y));
      //      std::cout << m_scale << " :: " << xt << " :: " << yt << std::endl;
      m_XTranslate = -(m_minX + abs(m_minX - xt));
      m_YTranslate = -(m_minY + abs(m_maxY - yt));
      geton_expose_event();
    }

  }
  return false;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setMove(bool Move)
{
  m_Move = Move;
}

// =====================================================================
// =====================================================================

bool MapViewDrawingArea::getMove()
{
  return m_Move;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setSelect(bool Select)
{
  m_Select = Select;
}

// =====================================================================
// =====================================================================

bool MapViewDrawingArea::getSelect()
{
  return m_Select;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setZoomFrame(bool ZoomFrame)
{
  m_ZoomFrame = ZoomFrame;
}

// =====================================================================
// =====================================================================

bool MapViewDrawingArea::getZoomFrame()
{
  return m_ZoomFrame;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setZoomCursor(bool ZoomCursor)
{
  m_ZoomCursor = ZoomCursor;
}

// =====================================================================
// =====================================================================

bool MapViewDrawingArea::getZoomCursor()
{
  return m_ZoomCursor;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setUnzoomCursor(bool UnzoomCursor)
{
  m_UnzoomCursor = UnzoomCursor;
}

// =====================================================================
// =====================================================================

bool MapViewDrawingArea::getUnzoomCursor()
{
  return m_UnzoomCursor;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setInitTranslate(bool InitTranslate)
{
  m_InitTranslate = InitTranslate;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setScaleOrigine()
{
  m_scaleOrigine = std::min(m_Width / (m_maxX - m_minX),
      m_Height / (m_maxY - m_minY));
}

// =====================================================================
// =====================================================================

double MapViewDrawingArea::getScaleOrigine()
{
  return m_scaleOrigine;
}

// =====================================================================
// =====================================================================

void MapViewDrawingArea::setScale(double Scale)
{
  m_scale = Scale;
}

// =====================================================================
// =====================================================================
