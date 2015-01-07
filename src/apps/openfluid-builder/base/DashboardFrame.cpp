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
  \file DashboardWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "DashboardFrame.hpp"
#include "DashboardInfosWidget.hpp"
#include "DashboardStatusWidget.hpp"
#include "builderconfig.hpp"


DashboardFrame::DashboardFrame(const ProjectCentral* PrjCentral, QWidget* Parent):
  QFrame(Parent)
{
  setObjectName("DashboardWidget");
  setStyleSheet(QString("QFrame#DashboardWidget {background-color: %1;}").arg(BUILDER_TOOLBAR_BGCOLOR));
  setFrameShape(QFrame::NoFrame);

  mp_InfosWidget = new DashboardInfosWidget(PrjCentral->advancedDescriptors(),this);
  mp_StatusWidget = new DashboardStatusWidget(PrjCentral,this);

  mp_Layout = new QBoxLayout(QBoxLayout::TopToBottom,this);
  mp_Layout->setContentsMargins(16,16,16,16);
  mp_Layout->setSpacing(16);

  mp_Layout->addWidget(mp_InfosWidget);
  mp_Layout->addWidget(mp_StatusWidget);

  mp_Layout->setStretch(0,1);
  mp_Layout->setStretch(1,2);

  setLayout(mp_Layout);
}


// =====================================================================
// =====================================================================


DashboardFrame::~DashboardFrame()
{

}


// =====================================================================
// =====================================================================


void DashboardFrame::updateOrientation(Qt::DockWidgetArea Area)
{
  if (Area == Qt::LeftDockWidgetArea || Area == Qt::RightDockWidgetArea)
    mp_Layout->setDirection(QBoxLayout::TopToBottom);
  else if (Area == Qt::TopDockWidgetArea || Area == Qt::BottomDockWidgetArea)
    mp_Layout->setDirection(QBoxLayout::LeftToRight);
}


// =====================================================================
// =====================================================================


void DashboardFrame::refresh()
{
  mp_InfosWidget->refresh();
  mp_StatusWidget->refresh();
}


// =====================================================================
// =====================================================================


void DashboardFrame::refreshProjectInfos()
{
  mp_InfosWidget->refreshProjectInfos();
}
