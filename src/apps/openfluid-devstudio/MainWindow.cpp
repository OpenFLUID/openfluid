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
#include <QAction>
#include <QMessageBox>

#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>
#include <openfluid/ui/waresdev/WareSrcActions.hpp>


MainWindow::MainWindow() :
    QMainWindow(), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QRect ScreenRect = QApplication::desktop()->screenGeometry();
  resize(ScreenRect.width() * 0.9, ScreenRect.height() * 0.9);
  move((ScreenRect.width() - width()) / 2,
       (ScreenRect.height() - height()) / 2);

  createActions();
  createMenus();
  addToolBar(new openfluid::ui::waresdev::WareSrcToolbar(false, this));

  connect(m_Actions.value("Quit"), SIGNAL(triggered()),
  qApp,
          SLOT(quit()));
  connect(m_Actions.value("SwitchWorkspace"), SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));

  QMap<QString, QAction*>& Actions =
      openfluid::ui::waresdev::WareSrcActions::getInstance()->getActions();
  foreach(QAction* Action,Actions)connect(Action, SIGNAL(triggered()), this,
      SLOT(showNotYetImplemented()));


  ui->WareSrcCollection->addTab(
      new openfluid::ui::waresdev::WareSrcWidget(false, this),
      "a DevStudio ware src widget");
  ui->WareSrcCollection->addTab(
      new openfluid::ui::waresdev::WareSrcWidget(true, this),
      "a Builder ware src widget");
}


// =====================================================================
// =====================================================================


MainWindow::~MainWindow()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void MainWindow::createActions()
{
  m_Actions["SwitchWorkspace"] = new QAction(tr("Switch workspace"), this);

  m_Actions["Quit"] = new QAction(tr("Quit"), this);
  m_Actions["Quit"]->setShortcuts(QKeySequence::Quit);

  /* TODO check it, because :
   "The menu role can only be changed before the actions are put into the menu bar in Mac OS X (usually just before the first application window is shown)."
   (http://qt-project.org/doc/qt-4.8/qaction.html#menuRole-prop)*/
  m_Actions["Quit"]->setMenuRole(QAction::QuitRole);
}


// =====================================================================
// =====================================================================


void MainWindow::createMenus()
{
  openfluid::ui::waresdev::WareSrcActions* Actions =
      openfluid::ui::waresdev::WareSrcActions::getInstance();

  QMenu* Menu;
  QMenu* SubMenu;

  Menu = menuBar()->addMenu(tr("File"));
  SubMenu = Menu->addMenu(tr("New ware"));
  SubMenu->addAction(Actions->getAction("NewSimulator"));
  SubMenu->addAction(Actions->getAction("NewObserver"));
  SubMenu->addAction(Actions->getAction("NewExtension"));
  SubMenu = Menu->addMenu(tr("Open ware"));
  SubMenu->addAction(Actions->getAction("OpenSimulator"));
  SubMenu->addAction(Actions->getAction("OpenObserver"));
  SubMenu->addAction(Actions->getAction("OpenExtension"));
  Menu->addAction(Actions->getAction("DeleteWare"));
  Menu->addSeparator();
  Menu->addAction(Actions->getAction("NewFile"));
  Menu->addAction(Actions->getAction("OpenFile"));
  Menu->addAction(Actions->getAction("SaveFile"));
  Menu->addAction(Actions->getAction("SaveAsFile"));
  Menu->addAction(Actions->getAction("CloseFile"));
  Menu->addAction(Actions->getAction("DeleteFile"));
  Menu->addSeparator();
  Menu->addAction(m_Actions.value("SwitchWorkspace"));
  Menu->addAction(m_Actions.value("Quit"));
}


// =====================================================================
// =====================================================================


void MainWindow::showNotYetImplemented()
{
  QMessageBox::information(this, "", "Not yet implemented");
}


