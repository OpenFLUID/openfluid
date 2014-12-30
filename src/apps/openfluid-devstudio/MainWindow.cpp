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


MainWindow::MainWindow() :
    QMainWindow(), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QRect ScreenRect = QApplication::desktop()->screenGeometry();
  resize(ScreenRect.width() * 0.9, ScreenRect.height() * 0.9);
  move((ScreenRect.width() - width()) / 2,
       (ScreenRect.height() - height()) / 2);

  QList<int> Sizes;
  Sizes << 180 << 1000;
  ui->splitter->setSizes(Sizes);

  mp_Toolbar = new openfluid::ui::waresdev::WareSrcToolbar(false, this);
  addToolBar(mp_Toolbar);

  ui->SimExplorer->setType(openfluid::waresdev::WareSrcManager::SIMULATOR);
  ui->ObsExplorer->setType(openfluid::waresdev::WareSrcManager::OBSERVER);
  ui->ExtExplorer->setType(openfluid::waresdev::WareSrcManager::BUILDEREXT);

  mp_Collection = new openfluid::ui::waresdev::WareSrcWidgetCollection(
      ui->WareSrcCollection);

  createLocalActions();
  createMenus();

  connect(m_Actions["NewSimulator"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["NewObserver"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["NewExtension"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["OpenSimulator"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["OpenObserver"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["OpenExtension"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["DeleteWare"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["SwitchWorkspace"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));

  connect(m_Actions.value("Quit"), SIGNAL(triggered()),
  qApp,
          SLOT(quit()));

  connect(mp_Toolbar->getAction("NewFile"), SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(mp_Toolbar->getAction("OpenFile"), SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(mp_Toolbar->getAction("SaveFile"), SIGNAL(triggered()), mp_Collection,
          SLOT(saveCurrent()));
  connect(mp_Toolbar->getAction("SaveAsFile"), SIGNAL(triggered()), mp_Collection,
          SLOT(saveCurrentAs()));
  connect(mp_Toolbar->getAction("CloseFile"), SIGNAL(triggered()), mp_Collection,
          SLOT(closeCurrent()));
  connect(mp_Toolbar->getAction("DeleteFile"), SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));

  connect(m_Actions["Copy"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["Cut"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["Paste"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["Find"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["Replace"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));
  connect(m_Actions["GoToLine"], SIGNAL(triggered()), this,
          SLOT(showNotYetImplemented()));

  connect(mp_Toolbar->getAction("Release"), SIGNAL(triggered()), mp_Collection,
          SLOT(setReleaseMode()));
  connect(mp_Toolbar->getAction("Debug"), SIGNAL(triggered()), mp_Collection,
          SLOT(setDebugMode()));
  connect(mp_Toolbar->getAction("BuildInstall"), SIGNAL(triggered()),
          mp_Collection, SLOT(setBuildWithInstallMode()));
  connect(mp_Toolbar->getAction("BuildOnly"), SIGNAL(triggered()),
          mp_Collection, SLOT(setBuildNoInstallMode()));
  connect(mp_Toolbar->getAction("Configure"), SIGNAL(triggered()),
          mp_Collection, SLOT(configure()));
  connect(mp_Toolbar->getAction("Build"), SIGNAL(triggered()), mp_Collection,
          SLOT(build()));
  connect(mp_Toolbar->getAction("OpenExplorer"), SIGNAL(triggered()),
          mp_Collection, SLOT(openExplorer()));
  connect(mp_Toolbar->getAction("OpenTerminal"), SIGNAL(triggered()),
          mp_Collection, SLOT(openTerminal()));

  connect(ui->SimExplorer, SIGNAL(openAsked(const QString&)), this,
          SLOT(onOpenAsked(const QString&)));
  connect(ui->ObsExplorer, SIGNAL(openAsked(const QString&)), this,
          SLOT(onOpenAsked(const QString&)));
  connect(ui->ExtExplorer, SIGNAL(openAsked(const QString&)), this,
          SLOT(onOpenAsked(const QString&)));

  connect(ui->SimExplorer, SIGNAL(setCurrentAsked(const QString&)),
          mp_Collection, SLOT(setCurrent(const QString&)));
  connect(ui->ObsExplorer, SIGNAL(setCurrentAsked(const QString&)),
          mp_Collection, SLOT(setCurrent(const QString&)));
  connect(ui->ExtExplorer, SIGNAL(setCurrentAsked(const QString&)),
          mp_Collection, SLOT(setCurrent(const QString&)));

  connect(ui->SimExplorer, SIGNAL(openExplorerAsked(const QString&)),
          mp_Collection, SLOT(openExplorer(const QString&)));
  connect(ui->ObsExplorer, SIGNAL(openExplorerAsked(const QString&)),
          mp_Collection, SLOT(openExplorer(const QString&)));
  connect(ui->ExtExplorer, SIGNAL(openExplorerAsked(const QString&)),
          mp_Collection, SLOT(openExplorer(const QString&)));

  connect(ui->SimExplorer, SIGNAL(openTerminalAsked(const QString&)),
          mp_Collection, SLOT(openTerminal(const QString&)));
  connect(ui->ObsExplorer, SIGNAL(openTerminalAsked(const QString&)),
          mp_Collection, SLOT(openTerminal(const QString&)));
  connect(ui->ExtExplorer, SIGNAL(openTerminalAsked(const QString&)),
          mp_Collection, SLOT(openTerminal(const QString&)));
}


// =====================================================================
// =====================================================================


MainWindow::~MainWindow()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void MainWindow::createLocalActions()
{
  m_Actions["NewSimulator"] = new QAction(tr("Simulator..."), this);
  m_Actions["NewObserver"] = new QAction(tr("Observer..."), this);
  m_Actions["NewExtension"] = new QAction(tr("Builder extension..."), this);

  m_Actions["OpenSimulator"] = new QAction(tr("Simulator..."), this);
  m_Actions["OpenObserver"] = new QAction(tr("Observer..."), this);
  m_Actions["OpenExtension"] = new QAction(tr("Builder extension..."), this);

  m_Actions["DeleteWare"] = new QAction(tr("Delete ware"), this);

  m_Actions["SwitchWorkspace"] = new QAction(tr("Switch workspace"), this);

  m_Actions["Quit"] = new QAction(tr("Quit"), this);
  m_Actions["Quit"]->setShortcuts(QKeySequence::Quit);

  /* TODO check it, because :
   "The menu role can only be changed before the actions are put into the menu bar in Mac OS X (usually just before the first application window is shown)."
   (http://qt-project.org/doc/qt-4.8/qaction.html#menuRole-prop)*/
  m_Actions["Quit"]->setMenuRole(QAction::QuitRole);

  m_Actions["Copy"] = new QAction(tr("Copy"), this);
  m_Actions["Copy"]->setShortcuts(QKeySequence::Copy);

  m_Actions["Cut"] = new QAction(tr("Cut"), this);
  m_Actions["Cut"]->setShortcuts(QKeySequence::Cut);

  m_Actions["Paste"] = new QAction(tr("Paste"), this);
  m_Actions["Paste"]->setShortcuts(QKeySequence::Paste);

  m_Actions["Find"] = new QAction(tr("Find"), this);
  m_Actions["Find"]->setShortcuts(QKeySequence::Find);

  m_Actions["Replace"] = new QAction(tr("Replace"), this);
  m_Actions["Replace"]->setShortcuts(QKeySequence::Replace);

  m_Actions["GoToLine"] = new QAction(tr("Go to line..."), this);
}


// =====================================================================
// =====================================================================


void MainWindow::createMenus()
{
  QMenu* Menu;
  QMenu* SubMenu;

  Menu = menuBar()->addMenu(tr("File"));
  SubMenu = Menu->addMenu(tr("New ware"));
  SubMenu->addAction(m_Actions["NewSimulator"]);
  SubMenu->addAction(m_Actions["NewObserver"]);
  SubMenu->addAction(m_Actions["NewExtension"]);
  SubMenu = Menu->addMenu(tr("Open ware"));
  SubMenu->addAction(m_Actions["OpenSimulator"]);
  SubMenu->addAction(m_Actions["OpenObserver"]);
  SubMenu->addAction(m_Actions["OpenExtension"]);
  Menu->addAction(m_Actions["DeleteWare"]);
  Menu->addSeparator();
  Menu->addAction(mp_Toolbar->getAction("NewFile"));
  Menu->addAction(mp_Toolbar->getAction("OpenFile"));
  Menu->addAction(mp_Toolbar->getAction("SaveFile"));
  Menu->addAction(mp_Toolbar->getAction("SaveAsFile"));
  Menu->addAction(mp_Toolbar->getAction("CloseFile"));
  Menu->addAction(mp_Toolbar->getAction("DeleteFile"));
  Menu->addSeparator();
  Menu->addAction(m_Actions.value("SwitchWorkspace"));
  Menu->addAction(m_Actions.value("Quit"));

  Menu = menuBar()->addMenu(tr("Edit"));
  Menu->addAction(m_Actions.value("Copy"));
  Menu->addAction(m_Actions.value("Cut"));
  Menu->addAction(m_Actions.value("Paste"));
  Menu->addAction(m_Actions.value("Find"));
  Menu->addAction(m_Actions.value("Replace"));
  Menu->addAction(m_Actions.value("GoToLine"));

  Menu = menuBar()->addMenu(tr("Build"));
  Menu->addAction(mp_Toolbar->getAction("Configure"));
  SubMenu = Menu->addMenu(tr("Set active configuration"));
  SubMenu->addAction(mp_Toolbar->getAction("Release"));
  SubMenu->addAction(mp_Toolbar->getAction("Debug"));
  Menu->addAction(mp_Toolbar->getAction("Build"));
  SubMenu = Menu->addMenu(tr("Set active build action"));
  SubMenu->addAction(mp_Toolbar->getAction("BuildInstall"));
  SubMenu->addAction(mp_Toolbar->getAction("BuildOnly"));

  Menu = menuBar()->addMenu(tr("Tools"));
  Menu->addAction(mp_Toolbar->getAction("OpenTerminal"));
  Menu->addAction(mp_Toolbar->getAction("OpenExplorer"));
}


// =====================================================================
// =====================================================================


void MainWindow::onOpenAsked(const QString& Path)
{
  mp_Collection->openPath(Path, false);
}


// =====================================================================
// =====================================================================


void MainWindow::showNotYetImplemented()
{
  QMessageBox::information(0, "", "Not yet implemented");
}

// =====================================================================
// =====================================================================

