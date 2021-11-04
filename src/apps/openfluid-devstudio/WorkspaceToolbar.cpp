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
 @file WorkspaceToolbar.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <QMenu>
#include <QToolButton>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <openfluid/config.hpp>
#include <openfluid/ui/waresdev/WareBuildOptionsWidget.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "WorkspaceToolbar.hpp"


WorkspaceToolbar::WorkspaceToolbar(QWidget* Parent) :
    QToolBar(Parent)
{

  setIconSize(QSize(32, 32));
  setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

  layout()->setSpacing(0);
  layout()->setMargin(0);

  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();
  m_ExternalTools = 
    PrefMgr->getWaresdevExternalToolsInContext(openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE);

  createActions();


  QToolButton* NewWareButton = new QToolButton(this);
  NewWareButton->setPopupMode(QToolButton::InstantPopup);

  NewWareButton->setIcon(openfluid::ui::common::getIcon("file-new","/ui/common",true));
  NewWareButton->setText(tr("New ware"));
  NewWareButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  QMenu* newWareMenu=new QMenu(NewWareButton);
  newWareMenu->addAction(m_Actions["NewSimulator"]);
  newWareMenu->addAction(m_Actions["NewObserver"]);
  newWareMenu->addAction(m_Actions["NewExtension"]);

  NewWareButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  NewWareButton->setMenu(newWareMenu);
  addWidget(NewWareButton);

  QToolButton* OpenWareButton= new QToolButton(this);
  OpenWareButton->setPopupMode(QToolButton::InstantPopup);
  OpenWareButton->setIcon(openfluid::ui::common::getIcon("file-open","/ui/common",true));
  OpenWareButton->setText(tr("Open ware"));
  OpenWareButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  QMenu* OpenWareMenu=new QMenu(OpenWareButton);
  OpenWareMenu->addAction(m_Actions["OpenSimulator"]);
  OpenWareMenu->addAction(m_Actions["OpenObserver"]);
  OpenWareMenu->addAction(m_Actions["OpenExtension"]);
  OpenWareButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  OpenWareButton->setMenu(OpenWareMenu);
  addWidget(OpenWareButton);

  addAction(m_Actions["ImportWare"]);

  addSeparator();
  addAction(m_Actions["Dashboard"]);
  addAction(m_Actions["APIDocShort"]);
}


// =====================================================================
// =====================================================================


WorkspaceToolbar::~WorkspaceToolbar()
{

}


// =====================================================================
// =====================================================================


void WorkspaceToolbar::createActions()
{

  m_Actions["NewSimulator"] = new QAction(tr("Simulator..."), this);
  m_Actions["NewObserver"] = new QAction(tr("Observer..."), this);
  m_Actions["NewExtension"] = new QAction(tr("Builder extension..."), this);

  m_Actions["OpenSimulator"] = new QAction(tr("Simulator..."), this);
  m_Actions["OpenObserver"] = new QAction(tr("Observer..."), this);
  m_Actions["OpenExtension"] = new QAction(tr("Builder extension..."), this);

  m_Actions["ImportWare"] = new QAction(openfluid::ui::common::getIcon("import","/ui/common",true),
                                      tr("Import..."), this);
  m_Actions["ImportWare"]->setToolTip(tr("Import ware"));


  // ====== Generic operations ======

  m_Actions["Dashboard"] = new QAction(openfluid::ui::common::getIcon("dashboard","/ui/common",true),
                                           tr("Dashboard"), this);

  m_Actions["APIDoc"] = new QAction(openfluid::ui::common::getIcon("generate-doc","/ui/common",true),
                                         tr("API doc"), this);

  // ====== Tools ======

  m_Actions["OpenTerminal"] = new QAction(tr("Open in terminal"), this);
  m_Actions["OpenExplorer"] = new QAction(tr("Open in file explorer"), this);

  for (auto const& Tool : m_ExternalTools)
  {
    QString ToolName = QString::fromStdString(Tool.Name);
    m_ExternalToolsActions[ToolName] = new QAction(ToolName, this);
    m_ExternalToolsActions[ToolName]->setData(
      QString::fromStdString(Tool.getCommand(openfluid::base::PreferencesManager::ExternalToolContext::WARE)));
  }

  // ====== Help ======

  m_Actions["APIDocShort"] = new QAction(openfluid::ui::common::getIcon("doc","/ui/common", true), tr("API Doc"), this);

}


// =====================================================================
// =====================================================================


const QMap<QString, QAction*> WorkspaceToolbar::externalToolsActions()
{
  return m_ExternalToolsActions;
}


// =====================================================================
// =====================================================================


QAction* WorkspaceToolbar::action(const QString& ActionName)
{
  if (m_Actions.contains(ActionName))
  {
    return m_Actions.value(ActionName);
  }

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                            "Action \"" + ActionName.toStdString() + "\" does'nt exist.");
}
