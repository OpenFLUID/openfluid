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
  \file DashboardWidget.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_DASHBOARDFRAME_HPP__
#define __OPENFLUID_BUILDERAPP_DASHBOARDFRAME_HPP__

#include "ProjectCentral.hpp"

#include <QFrame>
#include <QBoxLayout>


class DashboardInfosWidget;
class DashboardStatusWidget;


class DashboardFrame : public QFrame
{
  Q_OBJECT;

  private:

    DashboardInfosWidget* mp_InfosWidget;
    DashboardStatusWidget* mp_StatusWidget;

    QBoxLayout* mp_Layout;


  public slots:

    void updateOrientation(Qt::DockWidgetArea Area);


  public slots:

    void refresh();

    void refreshProjectInfos();


  public:

    DashboardFrame(const ProjectCentral* PrjCentral, QWidget* Parent = 0);

    ~DashboardFrame();

};


#endif /* __OPENFLUID_BUILDERAPP_DASHBOARDWIDGET_HPP__ */
