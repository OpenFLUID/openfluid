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
  @file WorkspaceTabWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QVariant>
#include <QTabBar>

#include <openfluid/ui/waresdev/WareSrcWidget.hpp>

#include "WorkspaceTabWidget.hpp"
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

  ExtensionsRegistry::instance()->releaseFeatureExtension(widget(Index)->property("ID").toString().toStdString());

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
    if (tabBar()->tabButton(Pos, QTabBar::RightSide) != nullptr)
    {
      tabBar()->tabButton(Pos, QTabBar::RightSide)->resize(0,0);
      tabBar()->tabButton(Pos, QTabBar::RightSide)->setVisible(false);
    }
    if (tabBar()->tabButton(Pos, QTabBar::LeftSide) != nullptr)
    {
      tabBar()->tabButton(Pos, QTabBar::LeftSide)->resize(0,0);
      tabBar()->tabButton(Pos, QTabBar::LeftSide)->setVisible(false);
    }
  }
}
