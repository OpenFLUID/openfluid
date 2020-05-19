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
#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>
#include <openfluid/ui/waresdev/WareBuildOptionsWidget.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcToolbar::WareSrcToolbar(bool IsIncluded, QWidget* Parent) :
    QToolBar(Parent), mp_OptionsWidget(nullptr), m_IsIncluded(IsIncluded)
{
  if (!m_IsIncluded)
  {
    setIconSize(QSize(32, 32));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  }

  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();
  m_ExternalTools = PrefMgr->getWaresdevExternalToolsCommandsInContext(
    openfluid::base::PreferencesManager::ExternalToolContext::WARE);

  createActions();

  addAction(m_Actions["NewFile"]);
  addAction(m_Actions["OpenFile"]);
  addAction(m_Actions["SaveFile"]);
  addAction(m_Actions["SaveAsFile"]);
  addAction(m_Actions["ConfigureWare"]);
  addAction(m_Actions["BuildWare"]);
#if OPENFLUID_SIM2DOC_ENABLED
  addAction(m_Actions["GenerateDoc"]);
#endif

  if (m_IsIncluded)
  {
    QWidget* Spacer = new QWidget();
    Spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addWidget(Spacer);

    QMenu* Menu = new QMenu();

    QMenu* SubMenu = Menu->addMenu(tr("File"));
    SubMenu->addAction(m_Actions["NewFile"]);
    SubMenu->addAction(m_Actions["OpenFile"]);
    SubMenu->addAction(m_Actions["SaveFile"]);
    SubMenu->addAction(m_Actions["SaveAsFile"]);
    SubMenu->addAction(m_Actions["SaveAllFiles"]);
    SubMenu->addAction(m_Actions["CloseFile"]);
    SubMenu->addAction(m_Actions["DeleteFile"]);

    SubMenu = Menu->addMenu(tr("Edit"));
    SubMenu->addAction(m_Actions["Copy"]);
    SubMenu->addAction(m_Actions["Cut"]);
    SubMenu->addAction(m_Actions["Paste"]);
    SubMenu->addAction(m_Actions["FindReplace"]);
    SubMenu->addAction(m_Actions["GoToLine"]);

    SubMenu = Menu->addMenu(tr("Build"));
    SubMenu->addAction(m_Actions["ConfigureWare"]);
    SubMenu->addAction(m_Actions["BuildWare"]);
#if OPENFLUID_SIM2DOC_ENABLED
    SubMenu->addAction(m_Actions["GenerateDoc"]);
#endif

    QMenu* SubSubMenu = SubMenu->addMenu(tr("Options"));
    SubSubMenu->addAction(m_Actions["WareOptionsRelease"]);
    SubSubMenu->addAction(m_Actions["WareOptionsDebug"]);
    SubSubMenu->addSeparator();
    SubSubMenu->addAction(m_Actions["WareOptionsInstall"]);

    // TODO add doc generation?

    SubMenu = Menu->addMenu(tr("Ware"));
    SubMenu->addAction(m_Actions["OpenTerminal"]);
    SubMenu->addAction(m_Actions["OpenExplorer"]);

    QMenu* ExtToolSubSubMenu = SubMenu->addMenu(tr("Open in external tool"));
    ExtToolSubSubMenu->setEnabled(false);

    QList<QString> ExternalToolsOrder = PrefMgr->getWaresdevExternalToolsOrder();
    for (auto const& Alias : ExternalToolsOrder)
    {
      if (m_ExternalToolsActions.contains(Alias))
      {
        ExtToolSubSubMenu->setEnabled(true);
        ExtToolSubSubMenu->addAction(m_ExternalToolsActions[Alias]);
      }
    }
    
    SubMenu = Menu->addMenu(tr("Help"));
    SubMenu->addAction(m_Actions["APIDoc"]);

    QToolButton* MenuButton = new QToolButton(this);
    MenuButton->setToolTip(tr("Menu"));
    MenuButton->setIcon(openfluid::ui::common::getIcon("menu","/ui/common"));
    MenuButton->setPopupMode(QToolButton::InstantPopup);
    MenuButton->setMenu(Menu);
    addWidget(MenuButton);
  }
  else
  {
    addSeparator();

    mp_OptionsWidget = new openfluid::ui::waresdev::WareBuildOptionsWidget(this);

    addWidget(mp_OptionsWidget);
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

  // ====== File ======

  m_Actions["NewFile"] = new QAction(openfluid::ui::common::getIcon("file-new","/ui/common",!m_IsIncluded),
                                     tr("New..."), this);
  m_Actions["NewFile"]->setToolTip(tr("Create a new file"));
  m_Actions["OpenFile"] = new QAction(openfluid::ui::common::getIcon("file-open","/ui/common",!m_IsIncluded),
                                      tr("Open..."), this);
  m_Actions["OpenFile"]->setToolTip(tr("Open a file"));

  m_Actions["SaveFile"] = new QAction(openfluid::ui::common::getIcon("file-save","/ui/common",!m_IsIncluded),
                                      tr("Save"), this);
  m_Actions["SaveFile"]->setShortcut(QKeySequence::Save);
  m_Actions["SaveFile"]->setToolTip(tr("Save the current file"));
  m_Actions["SaveFile"]->setEnabled(false);

  m_Actions["SaveAsFile"] = new QAction(openfluid::ui::common::getIcon("file-save-as","/ui/common",!m_IsIncluded),
                                        tr("Save as..."), this);
  m_Actions["SaveAsFile"]->setToolTip(tr("Save the current file as..."));
  m_Actions["SaveAsFile"]->setEnabled(false);

  m_Actions["SaveAllFiles"] = new QAction(openfluid::ui::common::getIcon("file-save","/ui/common",!m_IsIncluded),
                                          tr("Save all"), this);
  m_Actions["SaveAllFiles"]->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
  m_Actions["SaveAllFiles"]->setToolTip(tr("Save all files of the ware"));
  m_Actions["SaveAllFiles"]->setEnabled(false);

  m_Actions["CloseFile"] = new QAction(openfluid::ui::common::getIcon("close","/ui/common",!m_IsIncluded),
                                       tr("Close"), this);
  m_Actions["CloseFile"]->setToolTip(tr("Close the current file"));
  m_Actions["DeleteFile"] = new QAction(tr("Delete"), this);
  m_Actions["DeleteFile"]->setToolTip(tr("Delete the current file"));


  // ====== Edit ======

  m_Actions["Copy"] = new QAction(tr("Copy"), this);
  m_Actions["Copy"]->setShortcuts(QKeySequence::Copy);

  m_Actions["Cut"] = new QAction(tr("Cut"), this);
  m_Actions["Cut"]->setShortcuts(QKeySequence::Cut);

  m_Actions["Paste"] = new QAction(tr("Paste"), this);
  m_Actions["Paste"]->setShortcuts(QKeySequence::Paste);

  m_Actions["FindReplace"] = new QAction(tr("Find/Replace"), this);
  m_Actions["FindReplace"]->setShortcuts(QKeySequence::Find);

  m_Actions["GoToLine"] = new QAction(tr("Go to line..."), this);
  m_Actions["GoToLine"]->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));


  // ====== Build ======

  // Configure ware

  m_Actions["ConfigureWare"] = new QAction(openfluid::ui::common::getIcon("configure","/ui/common",!m_IsIncluded),
                                           tr("Configure"), this);
  m_Actions["ConfigureWare"]->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_B));


  // Build ware

  m_Actions["BuildWare"] = new QAction(openfluid::ui::common::getIcon("build","/ui/common",!m_IsIncluded),
                                       tr("Build"), this);
  m_Actions["BuildWare"]->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));


  // Generate doc

#if OPENFLUID_SIM2DOC_ENABLED
  m_Actions["GenerateDoc"] = new QAction(openfluid::ui::common::getIcon("generate-doc","/ui/common",!m_IsIncluded),
                                         tr("Generate doc"), this);
  m_Actions["GenerateDoc"]->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
#endif


  // Options

  m_Actions["WareOptionsMenu"] = new QAction(openfluid::ui::common::getIcon("configure","/ui/common",!m_IsIncluded),
                                             tr("Options"), this);
  QActionGroup* ConfigureGroup = new QActionGroup(this);
  QMenu* Menu = new QMenu();
  m_Actions["WareOptionsRelease"] = new QAction("Release mode", ConfigureGroup);
  m_Actions["WareOptionsRelease"]->setCheckable(true);
  m_Actions["WareOptionsRelease"]->setChecked(true);
  Menu->addAction(m_Actions["WareOptionsRelease"]);
  m_Actions["WareOptionsDebug"] = new QAction("Debug mode", ConfigureGroup);
  m_Actions["WareOptionsDebug"]->setCheckable(true);
  Menu->addAction(m_Actions["WareOptionsDebug"]);
  Menu->addSeparator();
  m_Actions["WareOptionsInstall"] = new QAction(tr("Make successful builds automatically available"),this);
  m_Actions["WareOptionsInstall"]->setCheckable(true);
  m_Actions["WareOptionsInstall"]->setChecked(true);
  Menu->addAction(m_Actions["WareOptionsInstall"]);
  m_Actions["WareOptionsMenu"]->setMenu(Menu);


  // ====== Tools ======

  m_Actions["OpenTerminal"] = new QAction(tr("Open in terminal"), this);
  m_Actions["OpenExplorer"] = new QAction(tr("Open in file explorer"), this);

  for (auto const& Command : m_ExternalTools.keys())
  {
    m_ExternalToolsActions[Command] = new QAction(Command, this);
    m_ExternalToolsActions[Command]->setData(m_ExternalTools.value(Command));
  }

  // ====== Help ======

  m_Actions["APIDoc"] = new QAction(tr("API documentation"), this);
  m_Actions["APIDoc"]->setShortcuts(QKeySequence::HelpContents);

}


// =====================================================================
// =====================================================================


const QMap<QString, QAction*> WareSrcToolbar::externalToolsActions()
{
  return m_ExternalToolsActions;
}


// =====================================================================
// =====================================================================


QAction* WareSrcToolbar::action(const QString& ActionName)
{
  if (m_Actions.contains(ActionName))
  {
    return m_Actions.value(ActionName);
  }

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                            "Action \"" + ActionName.toStdString() + "\" does'nt exist.");
}


} } }  // namespaces
