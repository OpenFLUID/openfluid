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
  @file MainWindow.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QCloseEvent>
#include <QLayout>
#include <QApplication>
#include <QDesktopWidget>

#include "MainWindow.hpp"


MainWindow::MainWindow():
  mp_QuitAction(nullptr), mp_CentralWidget(nullptr)
{
  QRect ScreenRect = QApplication::desktop()->screenGeometry();

  resize(ScreenRect.width()*0.9,ScreenRect.height()*0.9);
  move((ScreenRect.width()-width())/2,
       (ScreenRect.height()-height())/2);
  layout()->setContentsMargins(0,0,0,0);
  layout()->setSpacing(0);

  setProjectName();
  setWindowIcon(QIcon(":/builder/icons/openfluid_icon_builder.png"));
}


// =====================================================================
// =====================================================================


MainWindow::~MainWindow()
{

}


// =====================================================================
// =====================================================================


void MainWindow::setWidget(AbstractModuleWidget* Widget)
{
  mp_CentralWidget = Widget;
  setCentralWidget(mp_CentralWidget);
}


// =====================================================================
// =====================================================================


void MainWindow::closeEvent(QCloseEvent* Event)
{
  Event->ignore();
  mp_QuitAction->trigger();
}


// =====================================================================
// =====================================================================


void MainWindow::setProjectName(const QString& PrjName)
{
  if (PrjName.isEmpty())
    setWindowTitle("OpenFLUID-Builder");
  else
    setWindowTitle(QString("OpenFLUID-Builder [ %1 ]").arg(PrjName));
}
