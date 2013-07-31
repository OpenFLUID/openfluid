/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file AppActions.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <QMenuBar>
#include <QToolBar>

#include <openfluid/base/ApplicationException.hpp>

#include "AppActions.hpp"



AppActions::AppActions():
  mp_SimulationMenu(NULL), mp_MainToolbar(NULL)
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


void AppActions::createActions()
{
  // Project menu

  m_Actions["ProjectNew"] = new QAction(tr("&New"), this);
  m_Actions["ProjectNew"]->setShortcuts(QKeySequence::New);
  m_Actions["ProjectNew"]->setIcon(QIcon(":/icons/file-new.png"));

  m_Actions["ProjectOpen"] = new QAction(tr("&Open..."), this);
  m_Actions["ProjectOpen"]->setShortcuts(QKeySequence::Open);

  m_Actions["ProjectOpen"]->setIcon(QIcon(":/icons/file-open.png"));

  m_Actions["ProjectSave"] = new QAction(tr("&Save"), this);
  m_Actions["ProjectSave"]->setShortcuts(QKeySequence::Save);
  m_Actions["ProjectSave"]->setIcon(QIcon(":/icons/file-save.png"));

  m_Actions["ProjectSaveAs"] = new QAction(tr("Save as..."), this);
  m_Actions["ProjectSaveAs"]->setIcon(QIcon(":/icons/file-save-as.png"));

  m_Actions["ProjectProperties"] = new QAction(tr("Properties"), this);

  m_Actions["ProjectClose"] = new QAction(tr("Close"), this);
  m_Actions["ProjectClose"]->setShortcuts(QKeySequence::Close);
  m_Actions["ProjectClose"]->setIcon(QIcon(":/icons/file-close.png"));

  m_Actions["ProjectQuit"] = new QAction(tr("Quit"), this);
  m_Actions["ProjectQuit"]->setShortcuts(QKeySequence::Quit);


  // Edit menu
  m_Actions["EditCut"] = new QAction(tr("&Cut"), this);
  m_Actions["EditCut"]->setShortcuts(QKeySequence::Cut);

  m_Actions["EditCopy"] = new QAction(tr("&Copy"), this);
  m_Actions["EditCopy"]->setShortcuts(QKeySequence::Copy);

  m_Actions["EditPaste"] = new QAction(tr("&Paste"), this);
  m_Actions["EditPaste"]->setShortcuts(QKeySequence::Paste);

  m_Actions["EditPreferences"] = new QAction(tr("Preferences..."), this);


  //Simulation menu
  m_Actions["WaresRefresh"] = new QAction(tr("Reload wares"), this);
  m_Actions["WaresRefresh"]->setIcon(QIcon(":/icons/refresh.png"));

  m_Actions["SimulationRun"] = new QAction(tr("Run simulation"), this);
  m_Actions["SimulationRun"]->setIcon(QIcon(":/icons/start.png"));


  //Help menu
  m_Actions["WaresRefresh"] = new QAction(tr("Reload wares"), this);
  m_Actions["WaresRefresh"]->setIcon(QIcon(":/icons/refresh.png"));

  m_Actions["SimulationRun"] = new QAction(tr("Run simulation"), this);
  m_Actions["SimulationRun"]->setIcon(QIcon(":/icons/start.png"));


  //Help menu
  m_Actions["HelpOnlineWeb"] = new QAction(tr("Web site"), this);

  m_Actions["HelpOnlineCommunity"] = new QAction(tr("Community site"), this);

  m_Actions["HelpExamplesOpen"] = new QAction(tr("Open an example project..."), this);
  m_Actions["HelpExamplesOpen"]->setIcon(QIcon(":/icons/file-open-example.png"));

  m_Actions["HelpExamplesRestore"] = new QAction(tr("Reinstall examples projects"), this);

  m_Actions["HelpAbout"] = new QAction(tr("About"), this);



  //Other
  m_Actions["MarketAccess"] = new QAction(tr("Access to OpenFLUID-Market"), this);
  m_Actions["MarketAccess"]->setIcon(QIcon(":/icons/market.png"));



}


// =====================================================================
// =====================================================================


QAction* AppActions::getAction(const std::string& ID) const
{
  std::map<std::string,QAction*>::const_iterator It = m_Actions.find(ID);

  if (It != m_Actions.end())
    return (*It).second;
  else
    throw openfluid::base::ApplicationException("openfluid-builder","AppActions::getAction","Required action \"" + ID + "\" does not exist.");
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

  mp_SimulationMenu->menuAction()->setVisible(true);
  mp_ExtensionsMenu->menuAction()->setVisible(true);
  if (mp_MainToolbar != NULL) mp_MainToolbar->show();
}


// =====================================================================
// =====================================================================


void AppActions::setHomeMode()
{
  m_Actions["ProjectSave"]->setVisible(false);
  m_Actions["ProjectSaveAs"]->setVisible(false);
  m_Actions["ProjectProperties"]->setVisible(false);
  m_Actions["ProjectClose"]->setVisible(false);

  mp_SimulationMenu->menuAction()->setVisible(false);
  mp_ExtensionsMenu->menuAction()->setVisible(false);
  if (mp_MainToolbar != NULL) mp_MainToolbar->hide();
}


// =====================================================================
// =====================================================================


void AppActions::createMenus(MainWindow& MainWin)
{
  QMenu* Menu;
  QMenu* SubMenu;

  Menu = MainWin.menuBar()->addMenu(tr("&Project"));
  Menu->addAction(getAction("ProjectNew"));
  Menu->addAction(getAction("ProjectOpen"));
  Menu->addAction(getAction("ProjectSave"));
  Menu->addAction(getAction("ProjectSaveAs"));
  Menu->addAction(getAction("ProjectProperties"));
  Menu->addAction(getAction("ProjectClose"));
  Menu->addSeparator();
  Menu->addAction(getAction("ProjectQuit"));


  Menu = MainWin.menuBar()->addMenu(tr("&Edit"));
  Menu->addAction(getAction("EditCut"));
  Menu->addAction(getAction("EditCopy"));
  Menu->addAction(getAction("EditPaste"));


  mp_SimulationMenu = MainWin.menuBar()->addMenu(tr("&Simulation"));
  mp_SimulationMenu->addAction(getAction("WaresRefresh"));
  mp_SimulationMenu->addSeparator();
  mp_SimulationMenu->addAction(getAction("SimulationRun"));

  mp_ExtensionsMenu = MainWin.menuBar()->addMenu(tr("&Extensions"));

  Menu = MainWin.menuBar()->addMenu(tr("&Help"));
  SubMenu = Menu->addMenu(tr("OpenFLUID online"));
  SubMenu->addAction(getAction("HelpOnlineWeb"));
  SubMenu->addAction(getAction("HelpOnlineCommunity"));
  Menu->addSeparator();
  SubMenu = Menu->addMenu(tr("Examples"));
  SubMenu->addAction(getAction("HelpExamplesOpen"));
  SubMenu->addAction(getAction("HelpExamplesRestore"));
  Menu->addSeparator();
  Menu->addAction(getAction("HelpAbout"));

}


// =====================================================================
// =====================================================================


void AppActions::createToolbar(MainWindow& MainWin)
{
  if (mp_MainToolbar == NULL)
  {
    mp_MainToolbar = new QToolBar(&MainWin);
    mp_MainToolbar->clear();
    mp_MainToolbar->setMovable(true);
    mp_MainToolbar->setFloatable(false);
    mp_MainToolbar->setIconSize(QSize(32,32));
    mp_MainToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mp_MainToolbar->addAction(getAction("ProjectNew"));
    mp_MainToolbar->addAction(getAction("ProjectOpen"));
    mp_MainToolbar->addAction(getAction("ProjectSave"));
    mp_MainToolbar->addAction(getAction("ProjectSaveAs"));
    mp_MainToolbar->addAction(getAction("ProjectClose"));
    mp_MainToolbar->addSeparator();
    mp_MainToolbar->addAction(getAction("WaresRefresh"));
    mp_MainToolbar->addSeparator();
    mp_MainToolbar->addAction(getAction("SimulationRun"));
  }

  MainWin.addToolBar(mp_MainToolbar);
}


