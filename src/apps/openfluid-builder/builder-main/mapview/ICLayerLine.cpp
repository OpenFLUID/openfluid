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
 \file ICLayerLine.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "ICLayerLine.hpp"

ICLayerLine::ICLayerLine()
{
}

// =====================================================================
// =====================================================================

ICLayerLine::~ICLayerLine()
{

}

// =====================================================================
// =====================================================================

OGRLineString* ICLayerLine::recoverLine(int id)
{
  return (OGRLineString *) m_ObjectGeo[id];
}

// =====================================================================
// =====================================================================

void ICLayerLine::drawLine(Cairo::RefPtr<Cairo::Context> cr, int index,
    double scale)
{

  OGRLineString* Line = recoverLine(index);
//  cr->save();
  cr->move_to(Line->getX(0), Line->getY(0));
  for (int i = 1; i < Line->getNumPoints(); i++)
  {
    cr->line_to(Line->getX(i), Line->getY(i));
  }
//  cr->restore();

  cr->stroke();

//    cr->move_to(ICLine[index].at(0).first, ICLine[index].at(0).second);
//    for (int i = 1; i < ICLine[index].size(); i++)
//    {
//      cr->line_to(ICLine[index].at(i).first, ICLine[index].at(i).second);
//    }
//    cr->stroke();
}

// =====================================================================
// =====================================================================

void ICLayerLine::draw(Cairo::RefPtr<Cairo::Context> cr, double scale)
{

  for (unsigned int i = 0; i < m_ObjectGeo.size(); i++)
  {
    drawLine(cr, i, scale);
  }

}

// =====================================================================
// =====================================================================

void ICLayerLine::addObjectGeo(OGRGeometry* ObjectGeo)
{
  ICLayer::addObjectGeo(ObjectGeo);
//  std::vector< std::pair<double, double> > ICL;
//  for (int i = 0; i < ((OGRLineString *) ObjectGeo)->getNumPoints(); i++)
//  {
//    ICL.push_back(
//        std::make_pair(((OGRLineString *) ObjectGeo)->getX(i),
//            ((OGRLineString *) ObjectGeo)->getY(i)));
//    std::cout << ICL.at(i).first << " :: " << ICL.at(i).second << std::endl;
//  }
//  ICLine.push_back(ICL);
  OGREnvelope Env;

  ObjectGeo->getEnvelope(&Env);

  if (m_ObjectGeo.size() == 1)
  {
    m_maxX = Env.MaxX;
    m_maxY = Env.MaxY;
    m_minX = Env.MinX;
    m_minY = Env.MinY;
  } else
  {
    m_maxX = std::max(m_maxX, Env.MaxX);
    m_maxY = std::max(m_maxY, Env.MaxY);
    m_minX = std::min(m_minX, Env.MinX);
    m_minY = std::min(m_minY, Env.MinY);
  }

}

// =====================================================================
// =====================================================================
