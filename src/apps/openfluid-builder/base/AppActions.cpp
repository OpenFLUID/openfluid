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
  @file AppActions.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef QT_VERSION_MAJOR
#pragma message "Qt version not found in source"
#else
#pragma message "Qt version found in source"
#endif


#include <algorithm>

#include <QMenuBar>
#include <QToolBar>
#include <QActionGroup>
#include <QDir>

#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/common/DefaultAction.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "AppActions.hpp"
#include "ExtensionsRegistry.hpp"
#include "WaresTranslationsRegistry.hpp"


AppActions::AppActions():
  mp_DevelopmentMenu(nullptr),
  mp_SimulationMenu(nullptr),
  mp_ExtensionsMenu(nullptr),
  mp_SpatialExtensionsMenu(nullptr), mp_ModelExtensionsMenu(nullptr),
  mp_ResultsExtensionsMenu(nullptr), mp_OtherExtensionsMenu(nullptr),
  mp_ViewMenu(nullptr),
  mp_RecentProjectsMenu(nullptr),
  mp_MainToolbar(nullptr)
{
  createActions();
}


// =====================================================================
// =====================================================================


AppActions::~AppActions()
{

}


// =====================================================================
// =====================================================================


void AppActions::updateRecentProjectsActions()
{
  auto RecentProjects = openfluid::base::WorkspaceManager::instance()->getRecentProjects();

  // Remove extra recent projet if any
  while (RecentProjects.size() > openfluid::base::WorkspaceManager::RecentProjectsMax)
  {
    RecentProjects.pop_back();
  }

  unsigned int RecentIndex = 0;
  for (const auto& Prj : RecentProjects)
  {
    QString PrjName = QString::fromStdString(Prj.Name);
    QString PrjPath = QString::fromStdString(Prj.Path);

    m_RecentProjectsActions[RecentIndex]->setText(PrjName+" - " +QDir::toNativeSeparators(PrjPath));
    m_RecentProjectsActions[RecentIndex]->setData(QDir::fromNativeSeparators(PrjPath));
    m_RecentProjectsActions[RecentIndex]->setVisible(true);

    RecentIndex++;
  }

  for (auto i=RecentIndex;i<openfluid::base::WorkspaceManager::RecentProjectsMax;i++)
  {
    m_RecentProjectsActions[i]->setVisible(false);
  }

  if (mp_RecentProjectsMenu != nullptr)
  {
    mp_RecentProjectsMenu->setEnabled(!RecentProjects.empty());
  }
}


// =====================================================================
// =====================================================================


void AppActions::createActions()
{
  // Project menu
  m_Actions["ProjectNew"] = new openfluid::ui::common::DefaultAction(tr("&New..."), this);
  m_Actions["ProjectNew"]->setShortcuts(QKeySequence::New);
  m_Actions["ProjectNew"]->setIcon(openfluid::ui::common::getIcon("project-new","/ui/common",true));

  m_Actions["ProjectOpen"] = new openfluid::ui::common::DefaultAction(tr("&Open..."), this);
  m_Actions["ProjectOpen"]->setShortcuts(QKeySequence::Open);
  m_Actions["ProjectOpen"]->setIcon(openfluid::ui::common::getIcon("project-open","/ui/common",true));

  m_Actions["ProjectReload"] = new openfluid::ui::common::DefaultAction(tr("Reload"), this);
  m_Actions["ProjectReload"]->setIcon(openfluid::ui::common::getIcon("reload","/ui/common",true));

  m_Actions["ProjectSave"] = new openfluid::ui::common::DefaultAction(tr("&Save"), this);
  m_Actions["ProjectSave"]->setShortcuts(QKeySequence::Save);
  m_Actions["ProjectSave"]->setIcon(openfluid::ui::common::getIcon("project-save","/ui/common",true));

  m_Actions["ProjectSaveAs"] = new openfluid::ui::common::DefaultAction(tr("Save as..."), this);
  m_Actions["ProjectSaveAs"]->setIcon(openfluid::ui::common::getIcon("project-save-as","/ui/common",true));

  m_Actions["ProjectClose"] = new openfluid::ui::common::DefaultAction(tr("Close"), this);
  m_Actions["ProjectClose"]->setShortcuts(QKeySequence::Close);
  m_Actions["ProjectClose"]->setIcon(openfluid::ui::common::getIcon("close","/ui/common",true));

  m_Actions["ProjectProperties"] = new openfluid::ui::common::DefaultAction(tr("Properties"), this);

  m_Actions["ProjectExplore"] = new openfluid::ui::common::DefaultAction( tr("Open in file explorer"), this);
#if (QT_VERSION_MAJOR < 6)
  m_Actions["ProjectExplore"]->setShortcut(QKeySequence(Qt::ALT+Qt::Key_X));
#else
  m_Actions["ProjectExplore"]->setShortcut(QKeySequence(Qt::ALT | Qt::Key_X));
#endif

  m_Actions["ProjectQuit"] = new openfluid::ui::common::DefaultAction( tr("Quit"), this);
  m_Actions["ProjectQuit"]->setShortcuts(QKeySequence::Quit);
  m_Actions["ProjectQuit"]->setMenuRole(QAction::QuitRole);


  // Edit menu
  m_Actions["EditCut"] = new openfluid::ui::common::DefaultAction( tr("&Cut"), this);
  m_Actions["EditCut"]->setShortcuts(QKeySequence::Cut);

  m_Actions["EditCopy"] = new openfluid::ui::common::DefaultAction( tr("&Copy"), this);
  m_Actions["EditCopy"]->setShortcuts(QKeySequence::Copy);

  m_Actions["EditPaste"] = new openfluid::ui::common::DefaultAction( tr("&Paste"), this);
  m_Actions["EditPaste"]->setShortcuts(QKeySequence::Paste);

  m_Actions["EditPreferences"] = new openfluid::ui::common::DefaultAction( tr("Preferences..."), this);
  m_Actions["EditPreferences"]->setMenuRole(QAction::PreferencesRole);


  //Development menu
  m_Actions["DevNewSimulator"] = new openfluid::ui::common::DefaultAction( tr("New simulator..."), this);
  m_Actions["DevNewGhostSimulator"] = new openfluid::ui::common::DefaultAction( tr("New ghost simulator..."), this);
  m_Actions["DevNewObserver"] = new openfluid::ui::common::DefaultAction( tr("New observer..."), this);

  m_Actions["DevOpenSimulator"] = new openfluid::ui::common::DefaultAction( tr("Open simulator..."), this);
  m_Actions["DevOpenObserver"] = new openfluid::ui::common::DefaultAction( tr("Open observer..."), this);

  m_Actions["DevLaunchDevStudio"] = new openfluid::ui::common::DefaultAction( "OpenFLUID-DevStudio", this);
  m_Actions["DevLaunchDevStudio"]->setIcon(openfluid::ui::common::getIcon("dev-wares","/builder",false));


  //Simulation menu
  m_Actions["WaresRefresh"] = new openfluid::ui::common::DefaultAction( tr("Reload simulators and observers"), this);
#if (QT_VERSION_MAJOR < 6)
  m_Actions["WaresRefresh"]->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_R));
#else
  m_Actions["WaresRefresh"]->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_R));
#endif

  m_Actions["SimulationRun"] = new openfluid::ui::common::DefaultAction( tr("Run simulation"), this);
  m_Actions["SimulationRun"]->setIcon(openfluid::ui::common::getIcon("run","/ui/common",true));
#if (QT_VERSION_MAJOR < 6)
  m_Actions["SimulationRun"]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
#else
  m_Actions["SimulationRun"]->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
#endif

  m_Actions["SimulationMode"] = new openfluid::ui::common::DefaultAction( tr("Mode"), this);
  QActionGroup* ConfigureGroup = new QActionGroup(this);
  QMenu* SimModeMenu = new QMenu();
  m_Actions["SimulationModeDefault"] = new openfluid::ui::common::DefaultAction( tr("Integrated"), ConfigureGroup);
  m_Actions["SimulationModeDefault"]->setCheckable(true);
  m_Actions["SimulationModeDefault"]->setChecked(true);
  SimModeMenu->addAction(m_Actions["SimulationModeDefault"]);
  m_Actions["SimulationModeCLI"] = new openfluid::ui::common::DefaultAction( tr("Command line"), ConfigureGroup);
  m_Actions["SimulationModeCLI"]->setCheckable(true);
  SimModeMenu->addAction(m_Actions["SimulationModeCLI"]);
  m_Actions["SimulationMode"]->setMenu(SimModeMenu);


  //View Menu
  m_Actions["ViewDashboard"] = new openfluid::ui::common::DefaultAction( tr("Show/Hide project dashboard"), this);
  m_Actions["ViewToolbar"] = new openfluid::ui::common::DefaultAction( tr("Show/Hide main toolbar"), this);
  m_Actions["ViewRestore"] = new openfluid::ui::common::DefaultAction( tr("Restore default view"), this);


  //Help menu
  m_Actions["HelpOnlineWeb"] = new openfluid::ui::common::DefaultAction( tr("Web site"), this);
  m_Actions["HelpOnlineCommunity"] = new openfluid::ui::common::DefaultAction( tr("Community site"), this);
  m_Actions["HelpEmail"] = new openfluid::ui::common::DefaultAction( tr("Email"), this);

  m_Actions["HelpExamplesOpen"] = new openfluid::ui::common::DefaultAction( tr("Open an example project..."), this);
  m_Actions["HelpExamplesOpen"]->setIcon(openfluid::ui::common::getIcon("project-open-example","/builder"));

  m_Actions["HelpExamplesRestore"] = new openfluid::ui::common::DefaultAction( tr("Reinstall examples projects"), this);

  m_Actions["HelpAbout"] = new openfluid::ui::common::DefaultAction( tr("About"), this);
  m_Actions["HelpAbout"]->setMenuRole(QAction::AboutRole);


  // Recent projects
  for (unsigned int i=0; i<openfluid::base::WorkspaceManager::RecentProjectsMax;i++)
  {
    m_RecentProjectsActions.push_back(new openfluid::ui::common::DefaultAction( this));
    m_RecentProjectsActions.back()->setVisible(false);
  }
  updateRecentProjectsActions();
}


// =====================================================================
// =====================================================================


QAction* AppActions::action(const std::string& ID) const
{
  std::map<std::string,QAction*>::const_iterator It = m_Actions.find(ID);

  if (It != m_Actions.end())
  {
    return (*It).second;
  }
  else
  {
    openfluid::base::ExceptionContext Context =
        openfluid::base::ApplicationException::computeContext("openfluid-builder",
                                                              OPENFLUID_CODE_LOCATION);

    throw openfluid::base::ApplicationException(Context,
                                                "Required action \"" + ID + "\" does not exist.");
  }
}


// =====================================================================
// =====================================================================


void AppActions::setProjectMode()
{
  std::map<std::string,QAction*>::const_iterator Itb = m_Actions.begin();
  std::map<std::string,QAction*>::const_iterator Ite = m_Actions.end();
  std::map<std::string,QAction*>::const_iterator It;

  for (It=Itb;It!=Ite;++It)
  {
    ((*It).second)->setVisible(true);
  }

  m_Actions["HelpExamplesRestore"]->setVisible(false);

#ifdef ENABLE_WARESDEV_INTEGRATION
  mp_DevelopmentMenu->menuAction()->setVisible(true);
#else
  mp_DevelopmentMenu->menuAction()->setVisible(false);
#endif

  mp_SimulationMenu->menuAction()->setVisible(true);
  mp_ViewMenu->menuAction()->setVisible(true);
  mp_ExtensionsMenu->menuAction()->setVisible(true);
  
  if (mp_MainToolbar != nullptr) 
  {
    mp_MainToolbar->show();
  }

  auto RunCtxt = openfluid::base::RunContextManager::instance();

  if (RunCtxt->isProjectOpen())
  {
    std::string ModeStr;

    auto ModeValue = RunCtxt->getProjectContextValue("/builder/simulation/mode");
    if (!ModeValue.isNull() && ModeValue.is<std::string>())
    {
      ModeStr = ModeValue.get<std::string>();
    }
    

    if (ProjectCentral::getRunModeValue(QString::fromStdString(ModeStr)) == ProjectCentral::RunMode::CLI)
    {
      m_Actions["SimulationModeCLI"]->setChecked(true);
    }
    else
    {
      m_Actions["SimulationModeDefault"]->setChecked(true);
    }
  }
}


// =====================================================================
// =====================================================================


void AppActions::setHomeMode()
{
  std::map<std::string,QAction*>::const_iterator Itb = m_Actions.begin();
  std::map<std::string,QAction*>::const_iterator Ite = m_Actions.end();
  std::map<std::string,QAction*>::const_iterator It;

  for (It=Itb;It!=Ite;++It)
  {
    ((*It).second)->setVisible(true);
  }

  m_Actions["ProjectSave"]->setVisible(false);
  m_Actions["ProjectSaveAs"]->setVisible(false);
  m_Actions["ProjectReload"]->setVisible(false);
  m_Actions["ProjectClose"]->setVisible(false);
  m_Actions["ProjectProperties"]->setVisible(false);
  m_Actions["ProjectExplore"]->setVisible(false);

  mp_DevelopmentMenu->menuAction()->setVisible(false);
  mp_SimulationMenu->menuAction()->setVisible(false);
  mp_ViewMenu->menuAction()->setVisible(false);
  mp_ExtensionsMenu->menuAction()->setVisible(false);

  if (mp_MainToolbar != nullptr)
  {
    mp_MainToolbar->hide();
  }
}


// =====================================================================
// =====================================================================


void AppActions::updateExtensionsActionsAndMenus()
{
  ExtensionsRegistry* ExtReg = ExtensionsRegistry::instance();

  const auto& Extensions = ExtReg->registeredFeatureExtensions();

  ExtensionsRegistry::ExtensionsByID_t::const_iterator it;
  ExtensionsRegistry::ExtensionsByID_t::const_iterator itb = Extensions.begin();
  ExtensionsRegistry::ExtensionsByID_t::const_iterator ite = Extensions.end();

  mp_SpatialExtensionsMenu->clear();
  mp_ModelExtensionsMenu->clear();
  mp_ResultsExtensionsMenu->clear();
  mp_OtherExtensionsMenu->clear();

  for (it = itb; it!= ite; ++it)
  {

    QString MenuText = WaresTranslationsRegistry::instance()
      ->tryTranslate(QString::fromStdString(it->second.Container.getPath()),
                     "signature",QString::fromStdString(it->second.Container.signature()->MenuText));

    // Replace empty menu text by extension ID
    MenuText = QString::fromStdString(openfluid::tools::replaceEmptyString(MenuText.toStdString(),(*it).first));

    m_ExtensionsActions[(*it).first] = new openfluid::ui::common::DefaultAction( MenuText,this);

    // associate extension ID with QAction for use when action is triggered and launch the correct extension
    m_ExtensionsActions[(*it).first]->setData(QString((*it).first.c_str()));

    // set extension in the correct menu, taking into account the extension category
    if (it->second.Container.signature()->Category == openfluid::builderext::ExtensionCategory::SPATIAL)
    {
      mp_SpatialExtensionsMenu->addAction(m_ExtensionsActions[(*it).first]);
    }
    else if (it->second.Container.signature()->Category == openfluid::builderext::ExtensionCategory::MODEL)
    {
      mp_ModelExtensionsMenu->addAction(m_ExtensionsActions[(*it).first]);
    }
    else if (it->second.Container.signature()->Category == openfluid::builderext::ExtensionCategory::RESULTS)
    {
      mp_ResultsExtensionsMenu->addAction(m_ExtensionsActions[(*it).first]);
    }
    else
    {
      mp_OtherExtensionsMenu->addAction(m_ExtensionsActions[(*it).first]);
    }
  }


  // fill empty menus with a "none" action

  QAction *NoneAction;

  if (mp_SpatialExtensionsMenu->isEmpty())
  {
    NoneAction = new openfluid::ui::common::DefaultAction( tr("(none)"),this);
    NoneAction->setEnabled(false);
    mp_SpatialExtensionsMenu->addAction(NoneAction);
  }

  if (mp_ModelExtensionsMenu->isEmpty())
  {
    NoneAction = new openfluid::ui::common::DefaultAction( tr("(none)"),this);
    NoneAction->setEnabled(false);
    mp_ModelExtensionsMenu->addAction(NoneAction);
  }

  if (mp_ResultsExtensionsMenu->isEmpty())
  {
    NoneAction = new openfluid::ui::common::DefaultAction( tr("(none)"),this);
    NoneAction->setEnabled(false);
    mp_ResultsExtensionsMenu->addAction(NoneAction);
  }

  if (mp_OtherExtensionsMenu->isEmpty())
  {
    NoneAction = new openfluid::ui::common::DefaultAction( tr("(none)"),this);
    NoneAction->setEnabled(false);
    mp_OtherExtensionsMenu->addAction(NoneAction);
  }

}


// =====================================================================
// =====================================================================


void AppActions::createMenus(MainWindow& MainWin)
{
  QMenu* Menu;
  QMenu* SubMenu;

  Menu = MainWin.menuBar()->addMenu(tr("&Project"));
  Menu->addAction(action("ProjectNew"));
  Menu->addAction(action("ProjectOpen"));

  // recents
  mp_RecentProjectsMenu = Menu->addMenu(tr("Open recent"));
  for (unsigned int i=0;i<m_RecentProjectsActions.size();i++)
  {
    mp_RecentProjectsMenu->addAction(m_RecentProjectsActions[i]);
  }

  Menu->addAction(action("ProjectReload"));
  Menu->addAction(action("ProjectSave"));
  Menu->addAction(action("ProjectSaveAs"));
  Menu->addAction(action("ProjectClose"));
  Menu->addSeparator();
  Menu->addAction(action("ProjectProperties"));
  Menu->addAction(action("ProjectExplore"));
  Menu->addSeparator();
  Menu->addAction(action("ProjectQuit"));


  Menu = MainWin.menuBar()->addMenu(tr("&Edit"));
  Menu->addAction(action("EditCut"));
  Menu->addAction(action("EditCopy"));
  Menu->addAction(action("EditPaste"));
  Menu->addSeparator();
  Menu->addAction(action("EditPreferences"));


  mp_DevelopmentMenu = MainWin.menuBar()->addMenu(tr("&Development"));
  mp_DevelopmentMenu->addAction(action("DevNewSimulator"));
  mp_DevelopmentMenu->addAction(action("DevNewGhostSimulator"));
  mp_DevelopmentMenu->addAction(action("DevNewObserver"));
  mp_DevelopmentMenu->addSeparator();
  mp_DevelopmentMenu->addAction(action("DevOpenSimulator"));
  mp_DevelopmentMenu->addAction(action("DevOpenObserver"));
  mp_DevelopmentMenu->addSeparator();
  mp_DevelopmentMenu->addAction(action("DevLaunchDevStudio"));


  mp_SimulationMenu = MainWin.menuBar()->addMenu(tr("&Simulation"));
  mp_SimulationMenu->addAction(action("WaresRefresh"));
  mp_SimulationMenu->addSeparator();
  mp_SimulationMenu->addAction(action("SimulationRun"));
  mp_SimulationMenu->addAction(action("SimulationMode"));

  mp_ExtensionsMenu = MainWin.menuBar()->addMenu(tr("&Extensions"));

  mp_SpatialExtensionsMenu = mp_ExtensionsMenu->addMenu(tr("Spatial domain"));
  mp_ModelExtensionsMenu = mp_ExtensionsMenu->addMenu(tr("Model"));
  mp_ResultsExtensionsMenu = mp_ExtensionsMenu->addMenu(tr("Results"));
  mp_OtherExtensionsMenu = mp_ExtensionsMenu->addMenu(tr("Other"));


  mp_ViewMenu = MainWin.menuBar()->addMenu(tr("&View"));
  mp_ViewMenu->addAction(action("ViewDashboard"));
  mp_ViewMenu->addAction(action("ViewToolbar"));
  mp_ViewMenu->addAction(action("ViewRestore"));


  Menu = MainWin.menuBar()->addMenu(tr("Window"));
  Menu->addAction(MainWin.getResetGeometryAction());

  Menu = MainWin.menuBar()->addMenu(tr("&Help"));
  SubMenu = Menu->addMenu(tr("OpenFLUID online"));
  SubMenu->addAction(action("HelpOnlineWeb"));
  SubMenu->addAction(action("HelpOnlineCommunity"));
  Menu->addSeparator();
  SubMenu = Menu->addMenu(tr("Examples"));
  SubMenu->addAction(action("HelpExamplesOpen"));
  SubMenu->addAction(action("HelpExamplesRestore"));
  Menu->addSeparator();
  Menu->addAction(action("HelpAbout"));

  updateRecentProjectsActions();

  updateExtensionsActionsAndMenus();
}


// =====================================================================
// =====================================================================


void AppActions::createToolbar(MainWindow& MainWin)
{
  if (!mp_MainToolbar)
  {
    mp_MainToolbar = new QToolBar(&MainWin);
    mp_MainToolbar->setAllowedAreas(Qt::LeftToolBarArea |
                                    Qt::RightToolBarArea |
                                    Qt::TopToolBarArea);
    mp_MainToolbar->clear();
    mp_MainToolbar->setMovable(false);
    mp_MainToolbar->setFloatable(false);
    mp_MainToolbar->setIconSize(QSize(32,32));
    mp_MainToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mp_MainToolbar->addAction(action("ProjectNew"));
    mp_MainToolbar->addAction(action("ProjectOpen"));
    mp_MainToolbar->addAction(action("ProjectReload"));
    mp_MainToolbar->addAction(action("ProjectSave"));
    mp_MainToolbar->addAction(action("ProjectSaveAs"));
    mp_MainToolbar->addAction(action("ProjectClose"));
    mp_MainToolbar->addSeparator();
    mp_MainToolbar->addAction(action("SimulationRun"));

    mp_MainToolbar->setObjectName("MainToolbar");
    mp_MainToolbar->setStyleSheet(QString(R"(
QWidget {
  color: #f0f0f0; 
  padding-left : 10px; 
  padding-right : 10px;
} 

#MainToolbar {
  background-color: %1; 
  border: 1px solid %1;
}

QToolButton::hover { 
  background-color: %2; 
  border : 1px solid %3; 
  border-radius: 4px; }
                          )").arg(openfluid::ui::config::TOOLBAR_BGCOLOR,
                                  openfluid::ui::config::TOOLBARBUTTON_BGCOLOR,
                                  openfluid::ui::config::TOOLBARBUTTON_BORDERCOLOR));
  }

  MainWin.addToolBar(
    static_cast<Qt::ToolBarArea>(openfluid::base::PreferencesManager::instance()->getBuilderToolBarArea()),
    mp_MainToolbar);
}

