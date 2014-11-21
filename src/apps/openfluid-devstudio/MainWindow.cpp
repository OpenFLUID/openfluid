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
 \file MainWindow.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QApplication>
#include <QDesktopWidget>

#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>


MainWindow::MainWindow() :
    QMainWindow(), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QRect ScreenRect = QApplication::desktop()->screenGeometry();
  resize(ScreenRect.width() * 0.9, ScreenRect.height() * 0.9);
  move((ScreenRect.width() - width()) / 2,
       (ScreenRect.height() - height()) / 2);

  addToolBar(new openfluid::ui::waresdev::WareSrcToolbar(this));

  ui->WareSrcCollection->addTab(
      new openfluid::ui::waresdev::WareSrcWidget(this), "a ware src widget");
  ui->WareSrcCollection->addTab(
      new openfluid::ui::waresdev::WareSrcWidget(this),
      "another ware src widget");
}


// =====================================================================
// =====================================================================


MainWindow::~MainWindow()
{
  delete ui;
}


// =====================================================================
// =====================================================================

