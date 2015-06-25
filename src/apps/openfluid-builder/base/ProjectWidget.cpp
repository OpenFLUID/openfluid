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
  @file ProjectWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTabWidget>

#include <openfluid/builderext/PluggableBuilderExtension.hpp>

#include <openfluid/ui/waresdev/WareSrcWidgetCollection.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>

#include "ui_ProjectWidget.h"
#include "ProjectWidget.hpp"

#include "ExtensionsRegistry.hpp"


WorkspaceTabWidget::WorkspaceTabWidget(QWidget* Parent):
  QTabWidget(Parent)
{
  setTabsClosable(true);
  connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
}


// =====================================================================
// =====================================================================


void WorkspaceTabWidget::closeTab(int Index)
{
  if(qobject_cast<openfluid::ui::waresdev::WareSrcWidget*>(widget(Index)))
    return;

  ExtensionsRegistry::instance()->releaseExtension(widget(Index)->property("ID").toString().toStdString());

  widget(Index)->deleteLater();
  removeTab(Index);
}


// =====================================================================
// =====================================================================


void WorkspaceTabWidget::addWorkspaceTab(QWidget* Tab, const QString& Label, bool Closable)
{
  int Pos = addTab(Tab,Label);

  if (!Closable)
  {
    if (tabBar()->tabButton(Pos, QTabBar::RightSide) != NULL)
    {
      tabBar()->tabButton(Pos, QTabBar::RightSide)->resize(0,0);
      tabBar()->tabButton(Pos, QTabBar::RightSide)->setVisible(false);
    }
    if (tabBar()->tabButton(Pos, QTabBar::LeftSide) != NULL)
    {
      tabBar()->tabButton(Pos, QTabBar::LeftSide)->resize(0,0);
      tabBar()->tabButton(Pos, QTabBar::LeftSide)->setVisible(false);
    }
  }
}


// =====================================================================
// =====================================================================



ProjectWidget::ProjectWidget(QWidget* Parent):
  AbstractMainWidget(Parent), ui(new Ui::ProjectWidget), mp_WorkspaceTabWidget(NULL)
{
  ui->setupUi(this);

  mp_WorkspaceTabWidget = new WorkspaceTabWidget(this);
  layout()->addWidget(mp_WorkspaceTabWidget);

  mp_WareSrcCollection = new openfluid::ui::waresdev::WareSrcWidgetCollection(mp_WorkspaceTabWidget, true);
}


// =====================================================================
// =====================================================================


ProjectWidget::~ProjectWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void ProjectWidget::addWorkspaceTab(QWidget* Tab, const QString& Label)
{
  mp_WorkspaceTabWidget->addWorkspaceTab(Tab,Label);
}


// =====================================================================
// =====================================================================


void ProjectWidget::addWorkspaceExtensionTab(QWidget* Tab, const QString& Label)
{
  mp_WorkspaceTabWidget->addWorkspaceTab(Tab,Label,true);
}


// =====================================================================
// =====================================================================


void ProjectWidget::addWorkspaceWareSrcTab(const QString& Path)
{
  mp_WareSrcCollection->openPath(Path);
}


// =====================================================================
// =====================================================================


void ProjectWidget::openSimulatorSrc()
{
  mp_WareSrcCollection->openSimulator();
}


// =====================================================================
// =====================================================================


void ProjectWidget::openObserverSrc()
{
  mp_WareSrcCollection->openObserver();
}


// =====================================================================
// =====================================================================


void ProjectWidget::newSimulator()
{
  mp_WareSrcCollection->newSimulator();
}


// =====================================================================
// =====================================================================


void ProjectWidget::newSimulatorFromGhost(const openfluid::ware::SimulatorSignature& Signature)
{
  mp_WareSrcCollection->newSimulatorFromGhost(Signature);
}


// =====================================================================
// =====================================================================


void ProjectWidget::newObserver()
{
  mp_WareSrcCollection->newObserver();
}
