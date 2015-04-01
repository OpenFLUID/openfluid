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
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>

#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>

#include "DevStudioPreferencesManager.hpp"


MainWindow::MainWindow() :
    QMainWindow(), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QRect ScreenRect = QApplication::desktop()->screenGeometry();
  resize(ScreenRect.width() * 0.9, ScreenRect.height() * 0.9);
  move((ScreenRect.width() - width()) / 2, (ScreenRect.height() - height()) / 2);

  QList<int> Sizes;
  Sizes << 180 << 1000;
  ui->splitter->setSizes(Sizes);

  mp_Toolbar = new openfluid::ui::waresdev::WareSrcToolbar(false, this);
  addToolBar(mp_Toolbar);

  openfluid::waresdev::WareSrcManager* Manager = openfluid::waresdev::WareSrcManager::instance();

  ui->SimExplorer->configure(Manager->getWareTypePath(openfluid::waresdev::WareSrcManager::SIMULATOR), true);
  ui->ObsExplorer->configure(Manager->getWareTypePath(openfluid::waresdev::WareSrcManager::OBSERVER), true);
  ui->ExtExplorer->configure(Manager->getWareTypePath(openfluid::waresdev::WareSrcManager::BUILDEREXT), true);

  mp_Collection = new openfluid::ui::waresdev::WareSrcWidgetCollection(ui->WareSrcCollection, false);

  createLocalActions();
  createMenus();

  connect(m_Actions["NewSimulator"], SIGNAL(triggered()), mp_Collection, SLOT(newSimulator()));
  connect(m_Actions["NewObserver"], SIGNAL(triggered()), mp_Collection, SLOT(newObserver()));
  connect(m_Actions["NewExtension"], SIGNAL(triggered()), mp_Collection, SLOT(newBuilderExtension()));
  connect(m_Actions["OpenSimulator"], SIGNAL(triggered()), mp_Collection, SLOT(openSimulator()));
  connect(m_Actions["OpenObserver"], SIGNAL(triggered()), mp_Collection, SLOT(openObserver()));
  connect(m_Actions["OpenExtension"], SIGNAL(triggered()), mp_Collection, SLOT(openBuilderExtension()));
  connect(m_Actions["SaveAsFile"], SIGNAL(triggered()), mp_Collection, SLOT(saveAsMayBeAboveWare()));
  connect(m_Actions["DeleteWare"], SIGNAL(triggered()), mp_Collection, SLOT(deleteCurrentWare()));
  connect(m_Actions["SwitchWorkspace"], SIGNAL(triggered()), this, SLOT(showNotYetImplemented()));
  connect(m_Actions["Quit"], SIGNAL(triggered()), this, SLOT(onQuitRequested()));

  connect(mp_Toolbar->action("NewFile"), SIGNAL(triggered()), mp_Collection, SLOT(newFile()));
  connect(mp_Toolbar->action("OpenFile"), SIGNAL(triggered()), mp_Collection, SLOT(openFile()));
  connect(mp_Toolbar->action("SaveFile"), SIGNAL(triggered()), mp_Collection, SLOT(saveCurrentEditor()));
  connect(mp_Toolbar->action("SaveAsFile"), SIGNAL(triggered()), mp_Collection, SLOT(saveAs()));
  connect(mp_Toolbar->action("CloseFile"), SIGNAL(triggered()), mp_Collection, SLOT(closeCurrentEditor()));
  connect(mp_Toolbar->action("DeleteFile"), SIGNAL(triggered()), mp_Collection, SLOT(deleteCurrentFile()));

  connect(m_Actions["Copy"], SIGNAL(triggered()), this, SLOT(showNotYetImplemented()));
  connect(m_Actions["Cut"], SIGNAL(triggered()), this, SLOT(showNotYetImplemented()));
  connect(m_Actions["Paste"], SIGNAL(triggered()), this, SLOT(showNotYetImplemented()));
  connect(m_Actions["Find"], SIGNAL(triggered()), this, SLOT(showNotYetImplemented()));
  connect(m_Actions["Replace"], SIGNAL(triggered()), this, SLOT(showNotYetImplemented()));
  connect(m_Actions["GoToLine"], SIGNAL(triggered()), this, SLOT(showNotYetImplemented()));

  connect(mp_Toolbar->action("Release"), SIGNAL(triggered()), mp_Collection, SLOT(setReleaseMode()));
  connect(mp_Toolbar->action("Debug"), SIGNAL(triggered()), mp_Collection, SLOT(setDebugMode()));
  connect(mp_Toolbar->action("BuildInstall"), SIGNAL(triggered()), mp_Collection, SLOT(setBuildWithInstallMode()));
  connect(mp_Toolbar->action("BuildOnly"), SIGNAL(triggered()), mp_Collection, SLOT(setBuildNoInstallMode()));
  connect(mp_Toolbar->action("Configure"), SIGNAL(triggered()), mp_Collection, SLOT(configure()));
  connect(mp_Toolbar->action("Build"), SIGNAL(triggered()), mp_Collection, SLOT(build()));
  connect(mp_Toolbar->action("OpenExplorer"), SIGNAL(triggered()), mp_Collection, SLOT(openExplorer()));
  connect(mp_Toolbar->action("OpenTerminal"), SIGNAL(triggered()), mp_Collection, SLOT(openTerminal()));

  connect(ui->SimExplorer, SIGNAL(doubleClicked(const QString&)), mp_Collection, SLOT(openPath(const QString&)));
  connect(ui->ObsExplorer, SIGNAL(doubleClicked(const QString&)), mp_Collection, SLOT(openPath(const QString&)));
  connect(ui->ExtExplorer, SIGNAL(doubleClicked(const QString&)), mp_Collection, SLOT(openPath(const QString&)));

  connect(ui->SimExplorer, SIGNAL(clicked(const QString&)), mp_Collection, SLOT(setCurrent(const QString&)));
  connect(ui->ObsExplorer, SIGNAL(clicked(const QString&)), mp_Collection, SLOT(setCurrent(const QString&)));
  connect(ui->ExtExplorer, SIGNAL(clicked(const QString&)), mp_Collection, SLOT(setCurrent(const QString&)));

  connect(ui->SimExplorer, SIGNAL(openExplorerAsked(const QString&)), mp_Collection,
          SLOT(openExplorer(const QString&)));
  connect(ui->ObsExplorer, SIGNAL(openExplorerAsked(const QString&)), mp_Collection,
          SLOT(openExplorer(const QString&)));
  connect(ui->ExtExplorer, SIGNAL(openExplorerAsked(const QString&)), mp_Collection,
          SLOT(openExplorer(const QString&)));

  connect(ui->SimExplorer, SIGNAL(openTerminalAsked(const QString&)), mp_Collection,
          SLOT(openTerminal(const QString&)));
  connect(ui->ObsExplorer, SIGNAL(openTerminalAsked(const QString&)), mp_Collection,
          SLOT(openTerminal(const QString&)));
  connect(ui->ExtExplorer, SIGNAL(openTerminalAsked(const QString&)), mp_Collection,
          SLOT(openTerminal(const QString&)));

  setWorkspaceDefaults();
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

  m_Actions["SaveAsFile"] = new QAction(QIcon(":/ui/common/icons/file-save-as.png"), tr("Save as..."), this);
  m_Actions["SaveAsFile"]->setToolTip(tr("Save the current file as..."));

  m_Actions["DeleteWare"] = new QAction(tr("Delete ware"), this);

  m_Actions["SwitchWorkspace"] = new QAction(tr("Switch workspace"), this);

  m_Actions["Quit"] = new QAction(tr("Quit"), this);
  m_Actions["Quit"]->setShortcuts(QKeySequence::Quit);

  /* TODO check it, because :
   "The menu role can only be changed before the actions are put into the menu bar in Mac OS X
   (usually just before the first application window is shown)."
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
  Menu->addAction(mp_Toolbar->action("NewFile"));
  Menu->addAction(mp_Toolbar->action("OpenFile"));
  Menu->addAction(mp_Toolbar->action("SaveFile"));
  Menu->addAction(m_Actions["SaveAsFile"]);
  Menu->addAction(mp_Toolbar->action("CloseFile"));
  Menu->addAction(mp_Toolbar->action("DeleteFile"));
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
  Menu->addAction(mp_Toolbar->action("Configure"));
  SubMenu = Menu->addMenu(tr("Set active configuration"));
  SubMenu->addAction(mp_Toolbar->action("Release"));
  SubMenu->addAction(mp_Toolbar->action("Debug"));
  Menu->addAction(mp_Toolbar->action("Build"));
  SubMenu = Menu->addMenu(tr("Set active build action"));
  SubMenu->addAction(mp_Toolbar->action("BuildInstall"));
  SubMenu->addAction(mp_Toolbar->action("BuildOnly"));

  Menu = menuBar()->addMenu(tr("Tools"));
  Menu->addAction(mp_Toolbar->action("OpenTerminal"));
  Menu->addAction(mp_Toolbar->action("OpenExplorer"));
}


// =====================================================================
// =====================================================================


void MainWindow::setWorkspaceDefaults()
{
  DevStudioPreferencesManager* Mgr = DevStudioPreferencesManager::instance();

  QStringList Mode = Mgr->getConfigBuildMode().split("|");
  mp_Toolbar->action(Mode.contains("DEBUG", Qt::CaseInsensitive) ? "Debug" : "Release")->trigger();
  mp_Toolbar->action(Mode.contains("BUILDONLY", Qt::CaseInsensitive) ? "BuildOnly" : "BuildInstall")->trigger();

  QStringList LastOpenWares = Mgr->getLastOpenWares();
  foreach(QString WarePath,LastOpenWares)mp_Collection->openPath(WarePath);

  mp_Collection->setCurrent(Mgr->getLastActiveWare());
}


// =====================================================================
// =====================================================================


void MainWindow::showNotYetImplemented()
{
  QMessageBox::information(0, "", "Not yet implemented");
}


// =====================================================================
// =====================================================================

void MainWindow::onQuitRequested()
{
  DevStudioPreferencesManager* Mgr = DevStudioPreferencesManager::instance();

  QStringList Mode;
  Mode << (mp_Collection->isDebugMode() ? "DEBUG" : "RELEASE");
  Mode << (mp_Collection->isBuildNoInstallMode() ? "BUILDONLY" : "BUILDINSTALL");
  Mgr->setConfigBuildMode(Mode.join("|"));

  Mgr->setLastOpenWares(mp_Collection->getOpenWarePaths());

  Mgr->setLastActiveWare(mp_Collection->getCurrentWarePath());

  if (mp_Collection->closeAllWidgets())
    qApp->quit();
}


// =====================================================================
// =====================================================================


void MainWindow::closeEvent(QCloseEvent* Event)
{
  Event->ignore();
  onQuitRequested();
}


// =====================================================================
// =====================================================================

