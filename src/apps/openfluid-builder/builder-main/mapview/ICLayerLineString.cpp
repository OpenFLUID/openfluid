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
 \file ICLayerLineString.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "ICLayerLineString.hpp"

ICLayerLineString::ICLayerLineString()
{

}

// =====================================================================
// =====================================================================

void ICLayerLineString::drawLine(Cairo::RefPtr<Cairo::Context> cr,
    OGRGeometry* ObjectGeo, double scale, bool select)
{

  OGRLineString* Line = static_cast<OGRLineString*> (ObjectGeo);
  cr->move_to(Line->getX(0), Line->getY(0));
  double lw = cr->get_line_width();

  for (int i = 1; i < Line->getNumPoints(); i++)
  {
    cr->line_to(Line->getX(i), Line->getY(i));
  }
  cr->save();
  if (select)
  {
    cr->set_line_width(lw + (4 / scale));
  }
  cr->stroke();
  cr->restore();
}

// =====================================================================
// =====================================================================

void ICLayerLineString::draw(Cairo::RefPtr<Cairo::Context> cr, double scale,
    std::set<int> select)
{
  std::map<int, ICLayerObject*>::iterator it;
  for (it = m_ICLayerObject.begin(); it != m_ICLayerObject.end(); it++)
  {
    if ((*it).second->selfIdExisting())
    {
      if (!select.empty())
      {
        std::set<int>::iterator it2;
        it2 = select.find((*it).first);
        if (it2 != select.end() && (*it2) == (*it).first)
          drawLine(cr, (*it).second->getOGRGeometryObject(), scale, true);
        else
          drawLine(cr, (*it).second->getOGRGeometryObject(), scale, false);
      } else
        drawLine(cr, (*it).second->getOGRGeometryObject(), scale, false);
    }
  }
}

// =====================================================================
// =====================================================================


std::pair<std::pair<double, double>, std::pair<double, double> > ICLayerLineString::getMinMax()
{
  std::pair<std::pair<double, double>, std::pair<double, double> > MinMaxTemp;

  OGREnvelope Env;
  bool first = true;
  std::map<int, ICLayerObject*>::iterator it;
  for (it = m_ICLayerObject.begin(); it != m_ICLayerObject.end(); it++)
  {
    if ((*it).second->selfIdExisting())
    {
      (*it).second->getOGRGeometryObject()->getEnvelope(&Env);

      if (first)
      {
        (MinMaxTemp.second).first = Env.MaxX;
        (MinMaxTemp.second).second = Env.MaxY;
        (MinMaxTemp.first).first = Env.MinX;
        (MinMaxTemp.first).second = Env.MinY;
        first = false;
      } else
      {
        (MinMaxTemp.second).first = std::max((MinMaxTemp.second).first,
            Env.MaxX);
        (MinMaxTemp.second).second = std::max((MinMaxTemp.second).second,
            Env.MaxY);
        (MinMaxTemp.first).first = std::min((MinMaxTemp.first).first, Env.MinX);
        (MinMaxTemp.first).second = std::min((MinMaxTemp.first).second,
            Env.MinY);
      }
    }
  }
  return MinMaxTemp;
}

// =====================================================================
// =====================================================================

int ICLayerLineString::isSelected(double x, double y, double scale)
{
  OGRPolygon* Poly =
      static_cast<OGRPolygon*> (OGRGeometryFactory::createGeometry(wkbPolygon));

  OGRLinearRing* Ring =
      static_cast<OGRLinearRing*> (OGRGeometryFactory::createGeometry(
          wkbLinearRing));

  Ring->setPoint(0, new OGRPoint(x - 3 / scale, y - 3 / scale));
  Ring->setPoint(1, new OGRPoint(x + 3 / scale, y - 3 / scale));
  Ring->setPoint(2, new OGRPoint(x + 3 / scale, y + 3 / scale));
  Ring->setPoint(3, new OGRPoint(x - 3 / scale, y + 3 / scale));

  Ring->closeRings();

  Poly->addRingDirectly(Ring);

  std::map<int, ICLayerObject*>::iterator it;
  for (it = m_ICLayerObject.begin(); it != m_ICLayerObject.end(); it++)
  {
    if (Poly->Intersects(
        static_cast<OGRGeometry*> ((*it).second->getOGRGeometryObject())))
    {
      return (*it).first;
    }
  }
  return -1;

}

