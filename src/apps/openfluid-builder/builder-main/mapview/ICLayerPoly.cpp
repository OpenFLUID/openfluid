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
 \file ICLayerPoly.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "ICLayerPoly.hpp"

ICLayerPoly::ICLayerPoly()
{

}

// =====================================================================
// =====================================================================

ICLayerPoly::~ICLayerPoly()
{
}

// =====================================================================
// =====================================================================

OGRPolygon* ICLayerPoly::recoverPoly(int id)
{
  return (OGRPolygon *) m_ObjectGeo[id];
}

// =====================================================================
// =====================================================================

void ICLayerPoly::drawPoly(Cairo::RefPtr<Cairo::Context> cr, int index,
    double scale)
{

    OGRPolygon* Poly = recoverPoly(index);
    //  OGRLinearRing* poLinearRing = new OGRLinearRing();
    //  poLinearRing = Poly->getExteriorRing();
    OGRLinearRing* poLinearRing = Poly->getExteriorRing();

    cr->move_to(poLinearRing->getX(0), poLinearRing->getY(0));
    for (int i = 1; i < poLinearRing->getNumPoints(); i++)
    {
      cr->line_to(poLinearRing->getX(i), poLinearRing->getY(i));

    }

    cr->close_path();

    cr->stroke();

//  cr->move_to(ICPoly[index].at(0).first, ICPoly[index].at(0).second);
//  for (int i = 1; i < ICPoly[index].size(); i++)
//  {
//    cr->line_to(ICPoly[index].at(i).first, ICPoly[index].at(i).second);
//  }
//
//  cr->close_path();
//
//  cr->stroke();
}

// =====================================================================
// =====================================================================

void ICLayerPoly::draw(Cairo::RefPtr<Cairo::Context> cr, double scale)
{

  for (unsigned int i = 0; i < m_ObjectGeo.size(); i++)
  {
    drawPoly(cr, i, scale);
  }

}

// =====================================================================
// =====================================================================

void ICLayerPoly::addObjectGeo(OGRGeometry* ObjectGeo)
{
  ICLayer::addObjectGeo(ObjectGeo);

  std::vector<std::pair<double, double> > ICP;
  OGRPolygon * O = (OGRPolygon *) ObjectGeo;
  for (int i = 0; i < O->getExteriorRing()->getNumPoints(); i++)
  {
    ICP.push_back(
        std::make_pair(O->getExteriorRing()->getX(i),
            O->getExteriorRing()->getY(i)));
//    std::cout << ICP.at(i).first << " :: " << ICP.at(i).second << std::endl;
  }
  ICPoly.push_back(ICP);

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
