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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <QMenuBar>
#include <QToolBar>
#include <QDir>

#include <openfluid/base/ApplicationException.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/ui/config.hpp>

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
  openfluid::base::PreferencesManager::RecentProjectsList_t RPList;

  RPList = openfluid::base::PreferencesManager::instance()->getBuilderRecentProjects();

  int RFCount = qMin(int(RPList.size()),openfluid::base::PreferencesManager::RecentProjectsLimit);

  for (int i=0; i<RFCount;i++)
  {
    m_RecentProjectsActions[i]->setText(RPList[i].Name+" - " +QDir::toNativeSeparators(RPList[i].Path));
    m_RecentProjectsActions[i]->setData(QDir::fromNativeSeparators(RPList[i].Path));
    m_RecentProjectsActions[i]->setVisible(true);
  }

  for (int i=RFCount;i<openfluid::base::PreferencesManager::RecentProjectsLimit;i++)
    m_RecentProjectsActions[i]->setVisible(false);

  if (mp_RecentProjectsMenu != nullptr)
    mp_RecentProjectsMenu->setEnabled(!RPList.empty());
}


// =====================================================================
// =====================================================================


void AppActions::createActions()
{
  // Project menu
  m_Actions["ProjectNew"] = new QAction(tr("&New..."), this);
  m_Actions["ProjectNew"]->setShortcuts(QKeySequence::New);
  m_Actions["ProjectNew"]->setIcon(QIcon(":/ui/common/icons/file-new.png"));

  m_Actions["ProjectOpen"] = new QAction(tr("&Open..."), this);
  m_Actions["ProjectOpen"]->setShortcuts(QKeySequence::Open);
  m_Actions["ProjectOpen"]->setIcon(QIcon(":/ui/common/icons/file-open.png"));

  m_Actions["ProjectReload"] = new QAction(tr("Reload"), this);
  m_Actions["ProjectReload"]->setIcon(QIcon(":/ui/common/icons/reload.png"));

  m_Actions["ProjectSave"] = new QAction(tr("&Save"), this);
  m_Actions["ProjectSave"]->setShortcuts(QKeySequence::Save);
  m_Actions["ProjectSave"]->setIcon(QIcon(":/ui/common/icons/file-save.png"));

  m_Actions["ProjectSaveAs"] = new QAction(tr("Save as..."), this);
  m_Actions["ProjectSaveAs"]->setIcon(QIcon(":/ui/common/icons/file-save-as.png"));

  m_Actions["ProjectProperties"] = new QAction(tr("Properties"), this);

  m_Actions["ProjectClose"] = new QAction(tr("Close"), this);
  m_Actions["ProjectClose"]->setShortcuts(QKeySequence::Close);
  m_Actions["ProjectClose"]->setIcon(QIcon(":/ui/common/icons/file-close.png"));

  m_Actions["ProjectQuit"] = new QAction(tr("Quit"), this);
  m_Actions["ProjectQuit"]->setShortcuts(QKeySequence::Quit);
  m_Actions["ProjectQuit"]->setMenuRole(QAction::QuitRole);


  // Edit menu
  m_Actions["EditCut"] = new QAction(tr("&Cut"), this);
  m_Actions["EditCut"]->setShortcuts(QKeySequence::Cut);

  m_Actions["EditCopy"] = new QAction(tr("&Copy"), this);
  m_Actions["EditCopy"]->setShortcuts(QKeySequence::Copy);

  m_Actions["EditPaste"] = new QAction(tr("&Paste"), this);
  m_Actions["EditPaste"]->setShortcuts(QKeySequence::Paste);

  m_Actions["EditPreferences"] = new QAction(tr("Preferences..."), this);
  m_Actions["EditPreferences"]->setMenuRole(QAction::PreferencesRole);


  //Development menu
  m_Actions["DevNewSimulator"] = new QAction(tr("New simulator..."), this);
  m_Actions["DevNewGhostSimulator"] = new QAction(tr("New ghost simulator..."), this);
  m_Actions["DevNewObserver"] = new QAction(tr("New observer..."), this);

  m_Actions["DevOpenSimulator"] = new QAction(tr("Open simulator..."), this);
  m_Actions["DevOpenObserver"] = new QAction(tr("Open observer..."), this);

  m_Actions["DevLaunchDevStudio"] = new QAction("OpenFLUID-DevStudio", this);
  m_Actions["DevLaunchDevStudio"]->setIcon(QIcon(":/icons/wares-development.png"));


  //Simulation menu
  m_Actions["WaresRefresh"] = new QAction(tr("Reload simulators and observers"), this);
  m_Actions["WaresRefresh"]->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_R));

  m_Actions["SimulationRun"] = new QAction(tr("Run simulation"), this);
  m_Actions["SimulationRun"]->setIcon(QIcon(":/ui/common/icons/start.png"));


  //View Menu
  m_Actions["ViewDashboard"] = new QAction(tr("Show/Hide project dashboard"), this);
  m_Actions["ViewToolbar"] = new QAction(tr("Show/Hide main toolbar"), this);
  m_Actions["ViewRestore"] = new QAction(tr("Restore default view"), this);


  //Help menu
  m_Actions["HelpOnlineWeb"] = new QAction(tr("Web site"), this);
  m_Actions["HelpOnlineCommunity"] = new QAction(tr("Community site"), this);
  m_Actions["HelpEmail"] = new QAction(tr("Email"), this);

  m_Actions["HelpExamplesOpen"] = new QAction(tr("Open an example project..."), this);
  m_Actions["HelpExamplesOpen"]->setIcon(QIcon(":/icons/file-open-example.png"));

  m_Actions["HelpExamplesRestore"] = new QAction(tr("Reinstall examples projects"), this);

  m_Actions["HelpAbout"] = new QAction(tr("About"), this);
  m_Actions["HelpAbout"]->setMenuRole(QAction::AboutRole);


  //Other
  m_Actions["MarketAccess"] = new QAction(tr("Access to OpenFLUID-Market"), this);
  m_Actions["MarketAccess"]->setIcon(QIcon(":/icons/market.png"));


  for (int i=0; i<openfluid::base::PreferencesManager::RecentProjectsLimit;i++)
  {
    m_RecentProjectsActions.push_back(new QAction(this));
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
    return (*It).second;
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
    ((*It).second)->setVisible(true);

  m_Actions["HelpExamplesRestore"]->setVisible(false);

#ifdef ENABLE_WARESDEV_INTEGRATION
  mp_DevelopmentMenu->menuAction()->setVisible(true);
#else
  mp_DevelopmentMenu->menuAction()->setVisible(false);
#endif

  mp_SimulationMenu->menuAction()->setVisible(true);
  mp_ViewMenu->menuAction()->setVisible(true);
  mp_ExtensionsMenu->menuAction()->setVisible(true);
  if (mp_MainToolbar != nullptr) mp_MainToolbar->show();
}


// =====================================================================
// =====================================================================


void AppActions::setHomeMode()
{
  std::map<std::string,QAction*>::const_iterator Itb = m_Actions.begin();
  std::map<std::string,QAction*>::const_iterator Ite = m_Actions.end();
  std::map<std::string,QAction*>::const_iterator It;

  for (It=Itb;It!=Ite;++It)
    ((*It).second)->setVisible(true);

  m_Actions["ProjectSave"]->setVisible(false);
  m_Actions["ProjectSaveAs"]->setVisible(false);
  m_Actions["ProjectProperties"]->setVisible(false);
  m_Actions["ProjectReload"]->setVisible(false);
  m_Actions["ProjectClose"]->setVisible(false);

  mp_DevelopmentMenu->menuAction()->setVisible(false);
  mp_SimulationMenu->menuAction()->setVisible(false);
  mp_ViewMenu->menuAction()->setVisible(false);
  mp_ExtensionsMenu->menuAction()->setVisible(false);
  if (mp_MainToolbar != nullptr) mp_MainToolbar->hide();
}


// =====================================================================
// =====================================================================


void AppActions::updateExtensionsActionsAndMenus()
{
  ExtensionsRegistry* ExtReg = ExtensionsRegistry::instance();

  const ExtensionsRegistry::ExtensionsByName_t* Extensions = ExtReg->registeredFeatureExtensions();

  ExtensionsRegistry::ExtensionsByName_t::const_iterator it;
  ExtensionsRegistry::ExtensionsByName_t::const_iterator itb = Extensions->begin();
  ExtensionsRegistry::ExtensionsByName_t::const_iterator ite = Extensions->end();

  mp_SpatialExtensionsMenu->clear();
  mp_ModelExtensionsMenu->clear();
  mp_ResultsExtensionsMenu->clear();
  mp_OtherExtensionsMenu->clear();

  for (it = itb; it!= ite; ++it)
  {

    QString MenuText = WaresTranslationsRegistry::instance()
      ->tryTranslate(QString::fromStdString((*it).second->FileFullPath),
                     "signature",(*it).second->Signature->MenuText);

    // Replace empty menu text by extension ID
    MenuText = QString::fromStdString(openfluid::tools::replaceEmptyString(MenuText.toStdString(),(*it).first));

    m_ExtensionsActions[(*it).first] = new QAction(MenuText,this);

    // associate extension ID with QAction for use when action is triggered and launch the correct extension
    m_ExtensionsActions[(*it).first]->setData(QString((*it).first.c_str()));

    // set extension in the correct menu, taking into account the extension category
    if ((*it).second->Signature->Category == openfluid::builderext::CAT_SPATIAL)
    {
      mp_SpatialExtensionsMenu->addAction(m_ExtensionsActions[(*it).first]);
    }
    else if ((*it).second->Signature->Category == openfluid::builderext::CAT_MODEL)
    {
      mp_ModelExtensionsMenu->addAction(m_ExtensionsActions[(*it).first]);
    }
    else if ((*it).second->Signature->Category == openfluid::builderext::CAT_RESULTS)
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
    NoneAction = new QAction(tr("(none)"),this);
    NoneAction->setEnabled(false);
    mp_SpatialExtensionsMenu->addAction(NoneAction);
  }

  if (mp_ModelExtensionsMenu->isEmpty())
  {
    NoneAction = new QAction(tr("(none)"),this);
    NoneAction->setEnabled(false);
    mp_ModelExtensionsMenu->addAction(NoneAction);
  }

  if (mp_ResultsExtensionsMenu->isEmpty())
  {
    NoneAction = new QAction(tr("(none)"),this);
    NoneAction->setEnabled(false);
    mp_ResultsExtensionsMenu->addAction(NoneAction);
  }

  if (mp_OtherExtensionsMenu->isEmpty())
  {
    NoneAction = new QAction(tr("(none)"),this);
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
  for (int i=0;i<openfluid::base::PreferencesManager::RecentProjectsLimit;i++)
    mp_RecentProjectsMenu->addAction(m_RecentProjectsActions[i]);

  Menu->addAction(action("ProjectReload"));
  Menu->addAction(action("ProjectSave"));
  Menu->addAction(action("ProjectSaveAs"));
  Menu->addAction(action("ProjectProperties"));
  Menu->addAction(action("ProjectClose"));
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


  mp_ExtensionsMenu = MainWin.menuBar()->addMenu(tr("&Extensions"));

  mp_SpatialExtensionsMenu = mp_ExtensionsMenu->addMenu(tr("Spatial domain"));
  mp_ModelExtensionsMenu = mp_ExtensionsMenu->addMenu(tr("Model"));
  mp_ResultsExtensionsMenu = mp_ExtensionsMenu->addMenu(tr("Results"));
  mp_OtherExtensionsMenu = mp_ExtensionsMenu->addMenu(tr("Other"));


  mp_ViewMenu = MainWin.menuBar()->addMenu(tr("&View"));
  mp_ViewMenu->addAction(action("ViewDashboard"));
  mp_ViewMenu->addAction(action("ViewToolbar"));
  mp_ViewMenu->addAction(action("ViewRestore"));


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
    mp_MainToolbar->setStyleSheet("QWidget {padding-left : 10px; padding-right : 10px;}");
    mp_MainToolbar->setStyleSheet(QString("QWidget {color: white; padding-left : 10px; padding-right : 10px;} "
                                          "#MainToolbar {background-color: %1; border: 1px solid %1;}"
                                          "QToolButton::hover "
                                            "{ background-color: %2; border : 1px solid %3; border-radius: 4px; }")
                                         .arg(openfluid::ui::config::TOOLBAR_BGCOLOR,
                                              openfluid::ui::config::TOOLBARBUTTON_BGCOLOR,
                                              openfluid::ui::config::TOOLBARBUTTON_BORDERCOLOR));
  }

  MainWin.addToolBar(openfluid::base::PreferencesManager::instance()->getBuilderToolBarPosition(),
                     mp_MainToolbar);
}




