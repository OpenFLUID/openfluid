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
 \file LineStringUnit.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef LINESTRINGUNIT_HPP_
#define LINESTRINGUNIT_HPP_

#include <geos/planargraph/Edge.h>

#include <ogrsf_frmts.h>
#include <map>
#include <boost/any.hpp>

namespace geos {
namespace geom {
class LineString;
}
}

namespace openfluid {
namespace landr {

class LineStringUnit: public geos::planargraph::Edge
{
  private:

    const geos::geom::LineString* mp_Line;

    OGRFeature* mp_Feature;

    std::map<std::string, boost::any> m_Attributes;

    // for limiting access to LineStringUnit attributes creation
    friend class LineStringGraph;

  public:

    /**
     * Takes ownership of LineString and Feature
     */
    LineStringUnit(const geos::geom::LineString* NewLine, OGRFeature* Feat);

    LineStringUnit(const openfluid::landr::LineStringUnit& Other);

    ~LineStringUnit();

    const geos::geom::LineString* getLine() const;

    OGRFeature* getFeature();

    int getSelfId();

    geos::planargraph::Node* getStartNode();

    geos::planargraph::Node* getEndNode();

    std::vector<openfluid::landr::LineStringUnit*> getUpNeighbours();

    std::vector<openfluid::landr::LineStringUnit*> getDownNeighbours();

    bool getAttributeValue(std::string AttributeName, boost::any& Value);

    bool setAttributeValue(std::string AttributeName, boost::any Value);

};

} // namespace landr
} /* namespace openfluid */

#endif /* LINESTRINGUNIT_HPP_ */
