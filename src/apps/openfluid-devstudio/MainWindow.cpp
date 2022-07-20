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
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <QApplication>
#include <QDesktopWidget>
#include <QAction>
#include <QUrl>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QCloseEvent>
#include <QWidgetAction>

#include <openfluid/config.hpp>
#include <openfluid/global.hpp>
#include <openfluid/ui/waresdev/WareBuildOptionsDialog.hpp>
#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>
#include <openfluid/ui/common/AboutDialog.hpp>
#include <openfluid/ui/common/PreferencesDialog.hpp>
#include <openfluid/ui/common/DefaultAction.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/waresdev/WaresSrcExportDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WorkspaceDevDashboardDialog.hpp>
#include <openfluid/ui/waresdev/WaresDevPackage.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/waresdev/WareSrcEnquirer.hpp>
#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/tools/QtHelpers.hpp>

#include "MainWindow.hpp"
#include "ui_MainWindow.h"


MainWindow::MainWindow(openfluid::ui::common::OpenFLUIDSplashScreen* Splash) :
    openfluid::ui::common::AppMainWindow("devstudio"), ui(new Ui::MainWindow)
{
  openfluid::base::PreferencesManager* PrefsMgr = openfluid::base::PreferencesManager::instance();


  Splash->setMessage(tr("Initializing workspace"));

  openfluid::base::WorkspaceManager::instance()->openWorkspace(PrefsMgr->getCurrentWorkspacePath());


  Splash->setMessage(tr("Preparing user interface"));

  ui->setupUi(this);

  QList<int> Sizes;
  Sizes << 220 << 1000;
  ui->splitter->setSizes(Sizes);

  createLocalActions();
  mp_ActionsCollection = new openfluid::ui::waresdev::WareSrcActionsCollection(false, true, this);
  mp_Toolbar = new WorkspaceToolbar(this);

  mp_Toolbar->setObjectName("WorkspaceToolbar");
  mp_Toolbar->setIconSize(QSize(32,32));
  mp_Toolbar->setStyleSheet(
      QString(R"(
QToolButton {
  color: #f0f0f0;
  padding-left : 10px;
  padding-right : 10px;
  padding-top : 5px;
  padding-bottom : 5px;
  margin-bottom : 5px;
  margin-top : 5px;
}

#WorkspaceToolbar {
  background-color: %1;
  border: 1px solid %1;
}

QToolButton, QLabel {
  background-color: %1;
  color: white;
}

QToolButton[popupMode=1] {
  background-color: %1;
  border: 1px solid %1;
  padding-left : 10px;
  padding-right : 20px;
}

QToolButton::hover {
  background-color: %2;
  border : 1px solid %3;
  border-radius: 4px;
}

QToolButton::menu-button {
  background-color: %1;
  border: 1px solid %1;
  border-radius: 4px;
}

QToolButton::menu-button:pressed, QToolButton::menu-button:hover {
  background-color: %2;
  border : 1px solid %3;
  border-radius: 4px;
}
             )").arg(
          openfluid::ui::config::TOOLBAR_BGCOLOR, openfluid::ui::config::TOOLBARBUTTON_BGCOLOR,
          openfluid::ui::config::TOOLBARBUTTON_BORDERCOLOR));


  addToolBar(Qt::LeftToolBarArea, mp_Toolbar);

  Splash->setMessage(tr("Initializing sources codes management"));

  auto Manager = openfluid::base::WorkspaceManager::instance();

  ui->WaresTabWidget->setCurrentIndex(0);

  ui->SimExplorer->configure(QString::fromStdString(Manager->getWaresPath(openfluid::ware::WareType::SIMULATOR)),true);
  ui->ObsExplorer->configure(QString::fromStdString(Manager->getWaresPath(openfluid::ware::WareType::OBSERVER)),true);
  ui->ExtExplorer->configure(QString::fromStdString(Manager->getWaresPath(openfluid::ware::WareType::BUILDEREXT)),true);

  mp_WidgetsCollection = new openfluid::ui::waresdev::WareSrcWidgetCollection(ui->WareSrcCollection, false);

  mp_BuildStatusWidget = new WareBuildStatusWidget(this);

  Splash->setMessage(tr("Loading external tools settings"));

  std::list<openfluid::base::PreferencesManager::ExternalTool_t> ExternalTools = 
    PrefsMgr->getWaresdevExternalToolsInContext(openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE);

  for (auto const& Tool : ExternalTools)
  {
    QString ToolName = QString::fromStdString(Tool.Name);
    QAction* Action = new openfluid::ui::common::DefaultAction(ToolName,this);
    Action->setData(
      QString::fromStdString(Tool.getCommand(openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE))
    );
    m_ExternalToolsActions.push_back(Action);
  }

  Splash->setMessage(tr("Configuring UI"));

  createMenus();

  connect(mp_Toolbar->action("NewSimulator"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(newSimulator()));
  connect(mp_Toolbar->action("NewObserver"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(newObserver()));
  connect(mp_Toolbar->action("NewExtension"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(newBuilderExtension()));
  connect(mp_Toolbar->action("OpenSimulator"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(openSimulator()));
  connect(mp_Toolbar->action("OpenObserver"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(openObserver()));
  connect(mp_Toolbar->action("OpenExtension"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(openBuilderExtension()));
  connect(m_Actions["CloseWares"], SIGNAL(triggered()), this, SLOT(onCloseAllWaresRequested()));
  connect(m_Actions["DeleteWare"], SIGNAL(triggered()), this, SLOT(onDeleteWareRequested()));
  connect(m_Actions["Quit"], SIGNAL(triggered()), this, SLOT(onQuitRequested()));

  connect(m_Actions["Preferences"], SIGNAL(triggered()), this, SLOT(onPreferencesAsked()));

  connect(m_Actions["ImportWareSources"], SIGNAL(triggered()), this, SLOT(onImportWareSourcesAsked()));
  connect(m_Actions["ExportWareSources"], SIGNAL(triggered()), this, SLOT(onExportWareSourcesAsked()));
  connect(m_Actions["DevDashboard"], SIGNAL(triggered()), this, SLOT(onDevDashboardAsked()));

  connect(m_Actions["HelpAbout"], SIGNAL(triggered()), this, SLOT(onAboutAsked()));
  connect(m_Actions["HelpOnlineWeb"], SIGNAL(triggered()), this, SLOT(onOnlineWebAsked()));
  connect(m_Actions["HelpOnlineCommunity"], SIGNAL(triggered()), this, SLOT(onOnlineCommunityAsked()));
  connect(mp_ActionsCollection->action("APIDoc"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(openAPIDoc()));

  connect(mp_ActionsCollection->action("NewFile"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(newFile()));
  connect(mp_ActionsCollection->action("OpenFile"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(openFile()));
  connect(mp_ActionsCollection->action("SaveFile"), SIGNAL(triggered()), 
          mp_WidgetsCollection, SLOT(saveCurrentEditor()));
  connect(mp_ActionsCollection->action("SaveAsFile"), SIGNAL(triggered()), 
          mp_WidgetsCollection, SLOT(saveAs()));
  connect(mp_ActionsCollection->action("SaveAllFiles"), SIGNAL(triggered()), 
          mp_WidgetsCollection, SLOT(saveAllCurrent()));
  connect(mp_ActionsCollection->action("CloseFile"), SIGNAL(triggered()), 
          mp_WidgetsCollection, SLOT(closeCurrentEditor()));
  connect(mp_ActionsCollection->action("DeleteFile"), SIGNAL(triggered()), 
          mp_WidgetsCollection, SLOT(deleteCurrentFile()));

  connect(mp_ActionsCollection->action("Copy"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(copyText()));
  connect(mp_ActionsCollection->action("Cut"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(cutText()));
  connect(mp_ActionsCollection->action("Paste"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(pasteText()));
  connect(mp_ActionsCollection->action("FindReplace"), SIGNAL(triggered()), 
          mp_WidgetsCollection, SLOT(showFindReplaceDialog()));
  connect(mp_ActionsCollection->action("GoToLine"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(goToLine()));

  connect(mp_ActionsCollection->action("ConfigureWare"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(configure()));
  connect(mp_ActionsCollection->action("BuildWare"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(build()));

  connect(mp_ActionsCollection->action("OpenWareOptions"), SIGNAL(triggered()),
          this, SLOT(displayBuildOptionsDialog()));

  connect(mp_ActionsCollection->action("GenerateDoc"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(generateDoc()));

  connect(mp_ActionsCollection->action("OpenExplorer"), SIGNAL(triggered()), 
          mp_WidgetsCollection, SLOT(openExplorer()));
  connect(mp_ActionsCollection->action("OpenTerminal"), SIGNAL(triggered()), 
          mp_WidgetsCollection, SLOT(openTerminal()));

  connect(mp_Toolbar->action("APIDoc"), SIGNAL(triggered()), mp_WidgetsCollection, SLOT(openAPIDoc()));
  connect(mp_Toolbar->action("ImportWare"), SIGNAL(triggered()), this, SLOT(onImportWareSourcesAsked()));
  connect(mp_Toolbar->action("Dashboard"), SIGNAL(triggered()), this, SLOT(onDevDashboardAsked()));

  for (auto Action : m_ExternalToolsActions)
  {
    connect(Action, SIGNAL(triggered()), this, SLOT(onOpenExternalToolAsked()));
  }

  connect(this, SIGNAL(openExternalToolAsked(const QString&, const QString&)),
          mp_WidgetsCollection, SLOT(openExternalTool(const QString&, const QString&)));
  

  QList<openfluid::ui::waresdev::WareSrcExplorer*> Explorers( { ui->SimExplorer, ui->ObsExplorer, ui->ExtExplorer });

  for (openfluid::ui::waresdev::WareSrcExplorer* Explorer : Explorers)
  {
    connect(Explorer, SIGNAL(doubleClicked(const QString&)),
            mp_WidgetsCollection, SLOT(openPath(const QString&)));
    connect(Explorer, SIGNAL(clicked(const QString&)),
            mp_WidgetsCollection, SLOT(setCurrent(const QString&)));
    connect(Explorer, SIGNAL(openExplorerAsked(const QString&)),
            mp_WidgetsCollection, SLOT(openExplorer(const QString&)));
    connect(Explorer, SIGNAL(openTerminalAsked(const QString&)),
            mp_WidgetsCollection, SLOT(openTerminal(const QString&)));
    connect(Explorer, SIGNAL(openExternalToolAsked(const QString&, const QString&)),
            mp_WidgetsCollection, SLOT(openExternalTool(const QString&, const QString&)));
    connect(Explorer, SIGNAL(openPathAsked(const QString&)),
            mp_WidgetsCollection, SLOT(openPath(const QString&)));
    connect(Explorer, SIGNAL(deleteWareAsked()),
            this, SLOT(onDeleteWareRequested()));
    connect(Explorer, SIGNAL(fileDeleted(const QString&)),
            mp_WidgetsCollection, SLOT(closeEditor(const QString&)));
  }

  connect(mp_WidgetsCollection, SIGNAL(currentTabChanged(const QString&)),
          this, SLOT(setCurrentPath(const QString&)));
  connect(mp_WidgetsCollection, SIGNAL(modifiedStatusChanged(bool, bool, bool)),
          this, SLOT(updateSaveButtonsStatus(bool, bool, bool)));
  connect(mp_WidgetsCollection, SIGNAL(editorSaved()),
          this, SLOT(updateExplorer()));

  connect(ui->CollapseAllSimActionLabel, SIGNAL(clicked()), ui->SimExplorer, SLOT(collapseAll()));
  connect(ui->CollapseAllObsActionLabel, SIGNAL(clicked()), ui->ObsExplorer, SLOT(collapseAll()));
  connect(ui->CollapseAllExtActionLabel, SIGNAL(clicked()), ui->ExtExplorer, SLOT(collapseAll()));

  Splash->setMessage(tr("Initializing workspace"));

  setWorkspaceDefaults();

  updateSaveButtonsStatus(false, mp_WidgetsCollection->isFileOpen(), false);


  QString TmpLabel = 
    tr("Current workspace: %1")
      .arg(QDir::toNativeSeparators(
        QString::fromStdString(openfluid::base::PreferencesManager::instance()->getCurrentWorkspacePath())));

  statusBar()->addPermanentWidget(new QLabel(TmpLabel),1);

  statusBar()->addPermanentWidget(mp_BuildStatusWidget);  

  connect(mp_BuildStatusWidget, SIGNAL(settingsButtonClicked()), this, SLOT(displayBuildOptionsDialog()));
  connect(mp_BuildStatusWidget, SIGNAL(settingsChanged(openfluid::waresdev::WareBuildOptions)), 
          this, SLOT(onBuildOptionsChanged(openfluid::waresdev::WareBuildOptions)));

  applyWindowGeometry();
}


// =====================================================================
// =====================================================================


MainWindow::~MainWindow()
{
  openfluid::base::PreferencesManager::kill();
  openfluid::base::RunContextManager::kill();
  openfluid::base::WorkspaceManager::kill();
  delete mp_WidgetsCollection;
  delete mp_ActionsCollection;
  delete ui;
}


// =====================================================================
// =====================================================================


void MainWindow::displayBuildOptionsDialog()
{
  openfluid::ui::waresdev::WareBuildOptionsDialog SettingsDialog(m_BuildOptions, QApplication::activeWindow());

  if (SettingsDialog.exec() == QDialog::Accepted)
  {
    openfluid::waresdev::WareBuildOptions ResultingOptions = SettingsDialog.getOptions();
    onBuildOptionsChanged(ResultingOptions, true);
  }
}


// =====================================================================
// =====================================================================


void MainWindow::createLocalActions()
{

  m_Actions["CloseWares"] = new openfluid::ui::common::DefaultAction(tr("Close all wares"), this);

  m_Actions["DeleteWare"] = new openfluid::ui::common::DefaultAction(tr("Delete ware"), this);

  m_Actions["Quit"] = new openfluid::ui::common::DefaultAction(tr("Quit"), this);
  m_Actions["Quit"]->setShortcuts(QKeySequence::Quit);

  /* TODO check it, because :
   "The menu role can only be changed before the actions are put into the menu bar in Mac OS X
   (usually just before the first application window is shown)."
   (http://qt-project.org/doc/qt-4.8/qaction.html#menuRole-prop)*/
  m_Actions["Quit"]->setMenuRole(QAction::QuitRole);

  m_Actions["Preferences"] = new openfluid::ui::common::DefaultAction( tr("Preferences..."), this);
  m_Actions["Preferences"]->setMenuRole(QAction::PreferencesRole);


  // Tools menu
  m_Actions["ImportWareSources"] = new openfluid::ui::common::DefaultAction(tr("Import wares sources..."), this);
  m_Actions["ExportWareSources"] = new openfluid::ui::common::DefaultAction(tr("Export wares sources..."), this);
  m_Actions["GitDashboard"] = new openfluid::ui::common::DefaultAction(tr("Dashboard..."), this);
  m_Actions["DevDashboard"] = new openfluid::ui::common::DefaultAction(tr("Development dashboard..."), this);
  m_Actions["DevDashboard"]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_D));

  // Help menu
  m_Actions["HelpOnlineWeb"] = new openfluid::ui::common::DefaultAction(tr("Web site"), this);
  m_Actions["HelpOnlineCommunity"] = new openfluid::ui::common::DefaultAction(tr("Community site"), this);
  m_Actions["HelpEmail"] = new openfluid::ui::common::DefaultAction(tr("Email"), this);

  m_Actions["HelpAbout"] = new openfluid::ui::common::DefaultAction(tr("About"), this);
  m_Actions["HelpAbout"]->setMenuRole(QAction::AboutRole);
}


// =====================================================================
// =====================================================================


void MainWindow::createMenus()
{
  QMenu* Menu;
  QMenu* SubMenu;
  QMenu* ToolSubMenu;

  // --- File
  Menu = menuBar()->addMenu(tr("File"));
  SubMenu = Menu->addMenu(tr("New ware"));
  SubMenu->addAction(mp_Toolbar->action("NewSimulator"));
  SubMenu->addAction(mp_Toolbar->action("NewObserver"));
  SubMenu->addAction(mp_Toolbar->action("NewExtension"));
  SubMenu = Menu->addMenu(tr("Open ware"));
  SubMenu->addAction(mp_Toolbar->action("OpenSimulator"));
  SubMenu->addAction(mp_Toolbar->action("OpenObserver"));
  SubMenu->addAction(mp_Toolbar->action("OpenExtension"));
  Menu->addAction(m_Actions["CloseWares"]);
  Menu->addAction(m_Actions["DeleteWare"]);
  Menu->addSeparator();
  Menu->addAction(mp_ActionsCollection->action("NewFile"));
  Menu->addAction(mp_ActionsCollection->action("OpenFile"));
  Menu->addAction(mp_ActionsCollection->action("SaveFile"));
  Menu->addAction(mp_ActionsCollection->action("SaveAsFile"));
  Menu->addAction(mp_ActionsCollection->action("SaveAllFiles"));
  Menu->addAction(mp_ActionsCollection->action("CloseFile"));
  Menu->addAction(mp_ActionsCollection->action("DeleteFile"));
  Menu->addSeparator();
  Menu->addAction(m_Actions.value("Quit"));

  // --- Edit
  Menu = menuBar()->addMenu(tr("Edit"));
  Menu->addAction(mp_ActionsCollection->action("Copy"));
  Menu->addAction(mp_ActionsCollection->action("Cut"));
  Menu->addAction(mp_ActionsCollection->action("Paste"));
  Menu->addAction(mp_ActionsCollection->action("FindReplace"));
  Menu->addAction(mp_ActionsCollection->action("GoToLine"));
  Menu->addSeparator();
  Menu->addAction(m_Actions["Preferences"]);

  // --- Build
  Menu = menuBar()->addMenu(tr("Build"));
  Menu->addAction(mp_ActionsCollection->action("ConfigureWare"));
  Menu->addAction(mp_ActionsCollection->action("BuildWare"));
  Menu->addAction(mp_ActionsCollection->action("GenerateDoc"));

  Menu->addSeparator();
  Menu->addAction(mp_ActionsCollection->action("OpenWareOptions"));

  // --- Workspace
  Menu = menuBar()->addMenu(tr("Workspace"));
  Menu->addAction(mp_ActionsCollection->action("OpenTerminal"));
  Menu->addAction(mp_ActionsCollection->action("OpenExplorer"));
  ToolSubMenu = Menu->addMenu(tr("Open in external tool"));
  ToolSubMenu->setEnabled(false);

  for (auto Action : m_ExternalToolsActions)
  {
    ToolSubMenu->setEnabled(true);
    ToolSubMenu->addAction(Action);
  }

  Menu->addSeparator();
  Menu->addAction(m_Actions["ImportWareSources"]);
  Menu->addAction(m_Actions["ExportWareSources"]);
  Menu->addSeparator();
  Menu->addAction(m_Actions["DevDashboard"]);

  // --- Window
  Menu = menuBar()->addMenu(tr("Window"));
  Menu->addAction(getResetGeometryAction());

  // --- Help
  Menu = menuBar()->addMenu(tr("&Help"));
  SubMenu = Menu->addMenu(tr("OpenFLUID online"));
  SubMenu->addAction(m_Actions["HelpOnlineWeb"]);
  SubMenu->addAction(m_Actions["HelpOnlineCommunity"]);
  SubMenu->addAction(mp_ActionsCollection->action("APIDoc"));
  Menu->addSeparator();
  Menu->addAction(m_Actions["HelpAbout"]);

}


// =====================================================================
// =====================================================================


void MainWindow::setWorkspaceDefaults()
{
  auto* WMgr = openfluid::base::WorkspaceManager::instance();

  WMgr->openWorkspace(openfluid::base::PreferencesManager::instance()->getCurrentWorkspacePath());

  std::vector<std::string> LastOpenWares = WMgr->getOpenWaresPaths();

  for (const auto& WarePath : LastOpenWares)
  {
    mp_WidgetsCollection->openPath(QString::fromStdString(WarePath));
  }

  mp_WidgetsCollection->setCurrent(QString::fromStdString(WMgr->getActiveWarePath()));

  mp_BuildStatusWidget->setBuildOptions(m_BuildOptions);
}


// =====================================================================
// =====================================================================


void MainWindow::onOpenExternalToolAsked()
{
  QAction* Sender = (QAction*)(QObject::sender());
  if (Sender != nullptr)
  {
    emit openExternalToolAsked(Sender->data().toString(), 
                               QString::fromStdString(openfluid::base::WorkspaceManager::instance()->getWaresPath()));
  }
}


// =====================================================================
// =====================================================================


void MainWindow::onQuitRequested()
{
  if (QMessageBox::question(this,
                            tr("Quit"),tr("Are you sure you want to quit OpenFLUID-DevStudio?"),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    auto* WMgr = openfluid::base::WorkspaceManager::instance();

    WMgr->setOpenWaresPaths(openfluid::tools::toStdStringVector(mp_WidgetsCollection->getOpenWarePaths()));

    WMgr->setActiveWarePath(mp_WidgetsCollection->getCurrentWarePath().toStdString());

    if (mp_WidgetsCollection->closeAllWidgets())
    {
      qApp->quit();
    }
  }
}


// =====================================================================
// =====================================================================


void MainWindow::onPreferencesAsked()
{
  openfluid::ui::common::PreferencesDialog PrefsDlg(QApplication::activeWindow(),
    openfluid::ui::common::PreferencesDialog::DisplayMode::DEVSTUDIO);

  connect(&PrefsDlg, SIGNAL(applyTextEditorSettingsAsked()), mp_WidgetsCollection, SLOT(updateEditorsSettings()));

  PrefsDlg.exec();

  if (PrefsDlg.isTextEditorSettingsChanged())
  {
    mp_WidgetsCollection->updateEditorsSettings();
  }

  if (PrefsDlg.isRestartRequired() &&
      openfluid::ui::common::OpenFLUIDDialog::confirmRestartAfterPreferences(this))
  {
    QCoreApplication::exit(openfluid::ui::config::EXIT_CODE_FOR_RESTART);
  }
}


// =====================================================================
// =====================================================================


void MainWindow::onImportWareSourcesAsked()
{
  if (openfluid::utils::CMakeProxy::isAvailable() || openfluid::utils::GitProxy::isAvailable())
  {
    openfluid::ui::waresdev::WaresSrcImportDialog Dialog(this);
    Dialog.exec();
  }
  else
  {
    QMessageBox::warning(this, tr("Import not available"), tr("Neither CMake program nor Git program can be found."));
  }
}


// =====================================================================
// =====================================================================


void MainWindow::onExportWareSourcesAsked()
{
  if (openfluid::utils::CMakeProxy::isAvailable())
  {
    openfluid::ui::waresdev::WaresSrcExportDialog Dialog(this);
    Dialog.exec();
  }
  else
  {
    QMessageBox::warning(this, tr("Export not available"), tr("CMake program can not be found."));
  }

}


// =====================================================================
// =====================================================================


void MainWindow::onBuildOptionsChanged(openfluid::waresdev::WareBuildOptions BuildOptions, bool RefreshVisual)
{
  m_BuildOptions = BuildOptions;
  m_BuildOptions.writeOptionsInWorkspace();

  if (RefreshVisual)
  {
    mp_BuildStatusWidget->setBuildOptions(m_BuildOptions);
  }
}


// =====================================================================
// =====================================================================


void MainWindow::onDevDashboardAsked()
{
  openfluid::ui::waresdev::WorkspaceDevDashboardDialog Dialog(this, m_BuildOptions);
  Dialog.setWindowTitle(tr("Development dashboard"));

  Dialog.exec();
  
  onBuildOptionsChanged(Dialog.getBuildOptions(), true);
}


// =====================================================================
// =====================================================================


void MainWindow::onOnlineWebAsked()
{
  QDesktopServices::openUrl(QUrl(QString::fromStdString(openfluid::config::URL_OFFICIAL), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void MainWindow::onOnlineCommunityAsked()
{
  QDesktopServices::openUrl(QUrl(QString::fromStdString(openfluid::config::URL_COMMUNITY), QUrl::TolerantMode));
}


// =====================================================================
// =====================================================================


void MainWindow::onAboutAsked()
{
  openfluid::ui::common::AboutDialog AboutDlg(this, m_Actions["HelpOnlineWeb"], m_Actions["HelpEmail"]);

  AboutDlg.exec();
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


void MainWindow::setCurrentPath(const QString& Path)
{
  if (ui->SimExplorer->setCurrentPath(Path))
  {
    ui->WaresTabWidget->setCurrentWidget(ui->SimPage);
  }
  else if (ui->ObsExplorer->setCurrentPath(Path))
  {
    ui->WaresTabWidget->setCurrentWidget(ui->ObsPage);
  }
  else if (ui->ExtExplorer->setCurrentPath(Path))
  {
    ui->WaresTabWidget->setCurrentWidget(ui->ExtPage);
  }
}


// =====================================================================
// =====================================================================


void MainWindow::updateSaveButtonsStatus(bool FileModified, bool FileOpen, bool WareModified)
{
  mp_ActionsCollection->action("SaveFile")->setEnabled(FileModified);
  mp_ActionsCollection->action("SaveAsFile")->setEnabled(FileOpen);
  mp_ActionsCollection->action("SaveAllFiles")->setEnabled(WareModified);
}


// =====================================================================
// =====================================================================


void MainWindow::onDeleteWareRequested()
{
  QWidget* CurrentWidget = ui->WaresTabWidget->currentWidget();
  QString SelectedPath = "";

  if (CurrentWidget == ui->SimPage)
  {
    SelectedPath = ui->SimExplorer->getCurrentPath();
  }
  else if (CurrentWidget == ui->ObsPage)
  {
    SelectedPath = ui->ObsExplorer->getCurrentPath();
  }
  else if (CurrentWidget == ui->ExtPage)
  {
    SelectedPath = ui->ExtExplorer->getCurrentPath();
  }

  if (SelectedPath != "")
  {
    QString WarePath = 
      QString::fromStdString(
        openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(SelectedPath.toStdString()).AbsoluteWarePath);
    if (WarePath != "")
    {
      mp_WidgetsCollection->deleteWare(WarePath);
    }
  }
}


// =====================================================================
// =====================================================================


void MainWindow::onCloseAllWaresRequested()
{
  mp_WidgetsCollection->closeAllWidgets();
}


// =====================================================================
// =====================================================================


void MainWindow::updateExplorer()
{
  QString CurrentWarePath = mp_WidgetsCollection->getCurrentWarePath();
  QWidget* CurrentWidget = ui->WaresTabWidget->currentWidget();

  if (CurrentWidget == ui->SimPage)
  {
    ui->SimExplorer->updateExplorerModel(CurrentWarePath);
  }
  else if (CurrentWidget == ui->ObsPage)
  {
    ui->ObsExplorer->updateExplorerModel(CurrentWarePath);
  }
  else if (CurrentWidget == ui->ExtPage)
  {
    ui->ExtExplorer->updateExplorerModel(CurrentWarePath);
  }
}

