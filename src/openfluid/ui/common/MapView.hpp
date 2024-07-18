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
  @file MapView.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_MAPVIEW_HPP__
#define __OPENFLUID_UICOMMON_MAPVIEW_HPP__


#include <QGraphicsView>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace ui { namespace common {

class OPENFLUID_API MapView : public QGraphicsView
{
  Q_OBJECT;

  private:

    bool m_AutomaticViewEnabled;

    void wheelEvent(QWheelEvent* Event);


  protected:

    void resizeEvent(QResizeEvent *Event);


  signals:

    void automaticViewEnabled(bool);

    void scaling();


  public slots:

    void fitViewToItems();

    void enableAutomaticView(bool Enabled);


  public:

    MapView(QWidget* Parent = nullptr);

};

} } }

#endif /* __OPENFLUID_UICOMMON_MAPVIEW_HPP__ */
