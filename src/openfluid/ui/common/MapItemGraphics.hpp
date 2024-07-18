/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file MapItemGraphics.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UICOMMON_MAPITEMGRAPHICS_HPP__
#define __OPENFLUID_UICOMMON_MAPITEMGRAPHICS_HPP__


#include <QGraphicsPathItem>
#include <QColor>

#include <openfluid/core/TypeDefs.hpp>


namespace openfluid { namespace ui { namespace common {

class OPENFLUID_API MapItemGraphics : public QGraphicsPathItem
{
  protected:

    openfluid::core::UnitID_t m_UnitID;

    QColor m_MainColor;

    static QColor m_SelectionColor;

    void paint(QPainter *Painter, const QStyleOptionGraphicsItem *Option, QWidget *Widget);

  public:

    MapItemGraphics(const QColor& MainColor);

    void setUnitID(openfluid::core::UnitID_t UnitID) { m_UnitID = UnitID; }

    openfluid::core::UnitID_t getUnitID() const { return m_UnitID; }

};

} } }

#endif /* __OPENFLUID_UICOMMON_MAPITEMGRAPHICS_HPP__ */
