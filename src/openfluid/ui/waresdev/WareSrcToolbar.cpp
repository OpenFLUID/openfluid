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
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcToolbar.hpp>

#include <QMenu>
#include <QToolButton>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcToolbar::WareSrcToolbar(bool IsIncluded, QWidget* Parent) :
    QToolBar(Parent)
{
  if (!IsIncluded)
  {
    setIconSize(QSize(32, 32));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  }

  createActions();

  addAction(m_Actions["NewFile"]);
  addAction(m_Actions["OpenFile"]);
  addAction(m_Actions["SaveFile"]);
  addAction(m_Actions["SaveAsFile"]);
  addAction(m_Actions["ConfigureWMenu"]);
  addAction(m_Actions["BuildWMenu"]);

  if (IsIncluded)
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
    SubMenu->addAction(m_Actions["Configure"]);
    QMenu* SubSubMenu = SubMenu->addMenu(tr("Set active configuration"));
    SubSubMenu->addAction(m_Actions["Release"]);
    SubSubMenu->addAction(m_Actions["Debug"]);
    SubMenu->addAction(m_Actions["Build"]);
    SubSubMenu = SubMenu->addMenu(tr("Set active build action"));
    SubSubMenu->addAction(m_Actions["BuildInstall"]);
    SubSubMenu->addAction(m_Actions["BuildOnly"]);

    SubMenu = Menu->addMenu(tr("Tools"));
    SubMenu->addAction(m_Actions["OpenTerminal"]);
    SubMenu->addAction(m_Actions["OpenExplorer"]);

    SubMenu = Menu->addMenu(tr("Help"));
    SubMenu->addAction(m_Actions["APIDoc"]);

    QToolButton* MenuButton = new QToolButton(this);
    MenuButton->setToolTip(tr("Menu"));
    MenuButton->setIcon(QIcon(":/ui/common/icons/menu.png"));
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
  m_Actions["NewFile"] = new QAction(QIcon(":/ui/common/icons/file-new.png"), tr("New..."), this);
  m_Actions["NewFile"]->setToolTip(tr("Create a new file"));
  m_Actions["OpenFile"] = new QAction(QIcon(":/ui/common/icons/file-open.png"), tr("Open..."), this);
  m_Actions["OpenFile"]->setToolTip(tr("Open a file"));

  m_Actions["SaveFile"] = new QAction(QIcon(":/ui/common/icons/file-save.png"), tr("Save"), this);
  m_Actions["SaveFile"]->setShortcut(QKeySequence::Save);
  m_Actions["SaveFile"]->setToolTip(tr("Save the current file"));
  m_Actions["SaveFile"]->setEnabled(false);

  m_Actions["SaveAsFile"] = new QAction(QIcon(":/ui/common/icons/file-save-as.png"), tr("Save as..."), this);
  m_Actions["SaveAsFile"]->setToolTip(tr("Save the current file as..."));
  m_Actions["SaveAsFile"]->setEnabled(false);

  m_Actions["SaveAllFiles"] = new QAction(QIcon(":/ui/common/icons/file-save.png"), tr("Save all"), this);
  m_Actions["SaveAllFiles"]->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
  m_Actions["SaveAllFiles"]->setToolTip(tr("Save all files of the ware"));
  m_Actions["SaveAllFiles"]->setEnabled(false);

  m_Actions["CloseFile"] = new QAction(QIcon(":/ui/common/icons/file-close.png"), tr("Close"), this);
  m_Actions["CloseFile"]->setToolTip(tr("Close the current file"));
  m_Actions["DeleteFile"] = new QAction(tr("Delete"), this);
  m_Actions["DeleteFile"]->setToolTip(tr("Delete the current file"));

  m_Actions["Copy"] = new QAction(tr("Copy"), this);
  m_Actions["Copy"]->setShortcuts(QKeySequence::Copy);

  m_Actions["Cut"] = new QAction(tr("Cut"), this);
  m_Actions["Cut"]->setShortcuts(QKeySequence::Cut);

  m_Actions["Paste"] = new QAction(tr("Paste"), this);
  m_Actions["Paste"]->setShortcuts(QKeySequence::Paste);

  m_Actions["FindReplace"] = new QAction(tr("Find/Replace"), this);
  m_Actions["FindReplace"]->setShortcuts(QKeySequence::Find);

  m_Actions["GoToLine"] = new QAction(tr("Go to line..."), this);
  m_Actions["GoToLine"]->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));

  m_Actions["Configure"] = new QAction(QIcon(":/ui/common/icons/configure.png"), tr("Configure ware"), this);
  m_Actions["Configure"]->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_B));
  m_Actions["ConfigureWMenu"] = new QAction(QIcon(":/ui/common/icons/configure.png"), tr("Configure"), this);
  QActionGroup* ConfigureGroup = new QActionGroup(this);
  QMenu* Menu = new QMenu();
  m_Actions["Release"] = new QAction(tr("Release"), ConfigureGroup);
  m_Actions["Release"]->setCheckable(true);
  m_Actions["Release"]->setChecked(true);
  Menu->addAction(m_Actions["Release"]);
  m_Actions["Debug"] = new QAction(tr("Debug"), ConfigureGroup);
  m_Actions["Debug"]->setCheckable(true);
  Menu->addAction(m_Actions["Debug"]);
  m_Actions["ConfigureWMenu"]->setMenu(Menu);
  connect(m_Actions["ConfigureWMenu"], SIGNAL(triggered()), m_Actions["Configure"], SLOT(trigger()));

  m_Actions["Build"] = new QAction(QIcon(":/ui/common/icons/build.png"), tr("Build ware"), this);
  m_Actions["Build"]->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
  m_Actions["BuildWMenu"] = new QAction(QIcon(":/ui/common/icons/build.png"), tr("Build"), this);
  QActionGroup* BuildGroup = new QActionGroup(this);
  Menu = new QMenu();
  m_Actions["BuildInstall"] = new QAction(tr("Build and install"), BuildGroup);
  m_Actions["BuildInstall"]->setCheckable(true);
  m_Actions["BuildInstall"]->setChecked(true);
  Menu->addAction(m_Actions["BuildInstall"]);
  m_Actions["BuildOnly"] = new QAction(tr("Build only"), BuildGroup);
  m_Actions["BuildOnly"]->setCheckable(true);
  Menu->addAction(m_Actions["BuildOnly"]);
  m_Actions["BuildWMenu"]->setMenu(Menu);
  connect(m_Actions["BuildWMenu"], SIGNAL(triggered()), m_Actions["Build"], SLOT(trigger()));

  m_Actions["OpenTerminal"] = new QAction(tr("Open terminal"), this);
  m_Actions["OpenExplorer"] = new QAction(tr("Open file explorer"), this);

  m_Actions["APIDoc"] = new QAction(tr("API documentation"), this);
  m_Actions["APIDoc"]->setShortcuts(QKeySequence::HelpContents);

  foreach(QAction* Action,m_Actions.values())Action->setIconVisibleInMenu(true);
}


// =====================================================================
// =====================================================================


QAction* WareSrcToolbar::action(const QString& ActionName)
{
  if (m_Actions.contains(ActionName))
    return m_Actions.value(ActionName);

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                            "Action \"" + ActionName.toStdString() + "\" does'nt exist.");
}


// =====================================================================
// =====================================================================


} } }  // namespaces
