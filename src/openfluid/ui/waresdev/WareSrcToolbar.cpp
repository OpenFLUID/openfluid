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
 @file WareSrcToolbar.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <QMenu>
#include <QToolButton>
#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <openfluid/config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/waresdev/WareBuildOptions.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/WareBuildOptionsWidget.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcToolbar::WareSrcToolbar(bool IsIncluded, 
                               openfluid::ui::waresdev::WareSrcActionsCollection* ActionsCollection, 
                               bool DisplayDocBuild,
                               QWidget* Parent) :
    QToolBar(Parent), m_IsIncluded(IsIncluded), mp_ActionsCollection(ActionsCollection), 
    m_DisplayDocBuild(DisplayDocBuild)
{
  if (!m_IsIncluded)
  {
    setIconSize(QSize(32, 32));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  }

  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();
  m_ExternalTools = 
    PrefMgr->getWaresdevExternalToolsInContext(openfluid::base::PreferencesManager::ExternalToolContext::WARE);

  createActions();
  
  addAction(mp_ActionsCollection->action("NewFile"));
  addAction(mp_ActionsCollection->action("OpenFile"));
  addAction(mp_ActionsCollection->action("SaveFile"));
  addAction(mp_ActionsCollection->action("SaveAsFile"));
  addSeparator();
  addAction(mp_ActionsCollection->action("FindReplace"));
  addSeparator();
  addAction(mp_ActionsCollection->action("ConfigureWare"));
  addAction(mp_ActionsCollection->action("BuildWare"));
#if OPENFLUID_SIM2DOC_ENABLED
  if (m_DisplayDocBuild)
  {
    addAction(mp_ActionsCollection->action("GenerateDoc"));
  }
#endif

  if (m_IsIncluded)
  {
    QWidget* Spacer = new QWidget();
    Spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addWidget(Spacer);

    QMenu* Menu = new QMenu();

    QMenu* SubMenu = Menu->addMenu(tr("File"));
    SubMenu->addAction(mp_ActionsCollection->action("NewFile"));
    SubMenu->addAction(mp_ActionsCollection->action("OpenFile"));
    SubMenu->addAction(mp_ActionsCollection->action("SaveFile"));
    SubMenu->addAction(mp_ActionsCollection->action("SaveAsFile"));
    SubMenu->addAction(mp_ActionsCollection->action("SaveAllFiles"));
    SubMenu->addAction(mp_ActionsCollection->action("CloseFile"));
    SubMenu->addAction(mp_ActionsCollection->action("DeleteFile"));

    SubMenu = Menu->addMenu(tr("Edit"));
    SubMenu->addAction(mp_ActionsCollection->action("Copy"));
    SubMenu->addAction(mp_ActionsCollection->action("Cut"));
    SubMenu->addAction(mp_ActionsCollection->action("Paste"));
    SubMenu->addAction(mp_ActionsCollection->action("FindReplace"));
    SubMenu->addAction(mp_ActionsCollection->action("GoToLine"));

    SubMenu = Menu->addMenu(tr("Build"));
    SubMenu->addAction(mp_ActionsCollection->action("ConfigureWare"));
    SubMenu->addAction(mp_ActionsCollection->action("BuildWare"));
#if OPENFLUID_SIM2DOC_ENABLED
    if (m_DisplayDocBuild)
    {
      SubMenu->addAction(mp_ActionsCollection->action("GenerateDoc"));
    }
#endif

    SubMenu->addAction(mp_ActionsCollection->action("WareOptionsDialog"));

    // TODO add doc generation?

    SubMenu = Menu->addMenu(tr("Ware"));
    SubMenu->addAction(mp_ActionsCollection->action("OpenTerminal"));
    SubMenu->addAction(mp_ActionsCollection->action("OpenExplorer"));

    QMenu* ExtToolSubSubMenu = SubMenu->addMenu(tr("Open in external tool"));
    ExtToolSubSubMenu->setEnabled(false);

    for (auto const& Tool : m_ExternalTools)
    {
      ExtToolSubSubMenu->setEnabled(true);
      ExtToolSubSubMenu->addAction(m_ExternalToolsActions[QString::fromStdString(Tool.Name)]);
    }
    
    SubMenu = Menu->addMenu(tr("Help"));
    SubMenu->addAction(mp_ActionsCollection->action("APIDoc"));

    QToolButton* MenuButton = new QToolButton(this);
    MenuButton->setToolTip(tr("Menu"));
    MenuButton->setIcon(openfluid::ui::common::getIcon("menu","/ui/common"));
    MenuButton->setPopupMode(QToolButton::InstantPopup);
    MenuButton->setMenu(Menu);
    addWidget(MenuButton);
  }
}


// =====================================================================
// =====================================================================


WareSrcToolbar::~WareSrcToolbar()
{

}


// =====================================================================
// =====================================================================


void WareSrcToolbar::createActions()
{
  for (auto const& Tool : m_ExternalTools)
  {
    QString ToolName = QString::fromStdString(Tool.Name);
    m_ExternalToolsActions[ToolName] = new QAction(ToolName, this);
    m_ExternalToolsActions[ToolName]->setData(
      QString::fromStdString(Tool.getCommand(openfluid::base::PreferencesManager::ExternalToolContext::WARE)));
  }
}


// =====================================================================
// =====================================================================


const QMap<QString, QAction*> WareSrcToolbar::externalToolsActions()
{
  return m_ExternalToolsActions;
}


} } }  // namespaces
