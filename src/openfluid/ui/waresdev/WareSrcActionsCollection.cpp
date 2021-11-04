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
 @file WareSrcActionsCollection.cpp

 @author Armel THÖNI <armel.thoni@inrae.fr>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/
 

#include <QMenu>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/config.hpp>
#include <openfluid/ui/waresdev/WareSrcActionsCollection.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcActionsCollection::WareSrcActionsCollection(bool IsIncluded, bool WithShortcuts, QWidget* Parent) : 
    QObject(Parent), m_IsIncluded(IsIncluded), m_WithShortcuts(WithShortcuts)
{
  createActions();
}


// =====================================================================
// =====================================================================


void WareSrcActionsCollection::createActions()
{

  // "WithShortcuts" switch allows to have identical actions in the same context in two widgets 
  // without having a shortcut conflict, when only one collection is created with this switch activated

  // ====== File ======

  m_Actions["NewFile"] = new QAction(openfluid::ui::common::getIcon("file-new","/ui/common",false),
                                     tr("New file..."), this);
  m_Actions["NewFile"]->setToolTip(tr("Create a new file"));
  m_Actions["OpenFile"] = new QAction(openfluid::ui::common::getIcon("file-open","/ui/common",false),
                                      tr("Open file..."), this);
  m_Actions["OpenFile"]->setToolTip(tr("Open a file"));

  m_Actions["SaveFile"] = new QAction(openfluid::ui::common::getIcon("file-save","/ui/common",false),
                                      tr("Save file"), this);
  if (m_WithShortcuts)
  {
    m_Actions["SaveFile"]->setShortcut(QKeySequence::Save);
  }
  m_Actions["SaveFile"]->setToolTip(tr("Save the current file"));
  m_Actions["SaveFile"]->setEnabled(false);

  m_Actions["SaveAsFile"] = new QAction(openfluid::ui::common::getIcon("file-save-as","/ui/common",false),
                                        tr("Save as..."), this);
  m_Actions["SaveAsFile"]->setToolTip(tr("Save the current file as..."));
  m_Actions["SaveAsFile"]->setEnabled(false);

  m_Actions["SaveAllFiles"] = new QAction(openfluid::ui::common::getIcon("file-save","/ui/common",false),
                                          tr("Save all"), this);
  if (m_WithShortcuts)
  {
    m_Actions["SaveAllFiles"]->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
  }
  m_Actions["SaveAllFiles"]->setToolTip(tr("Save all files of the ware"));
  m_Actions["SaveAllFiles"]->setEnabled(false);

  m_Actions["CloseFile"] = new QAction(openfluid::ui::common::getIcon("close","/ui/common",false),
                                       tr("Close"), this);
  m_Actions["CloseFile"]->setToolTip(tr("Close the current file"));
  m_Actions["DeleteFile"] = new QAction(tr("Delete"), this);
  m_Actions["DeleteFile"]->setToolTip(tr("Delete the current file"));


  // ====== Edit ======

  m_Actions["Copy"] = new QAction(tr("Copy"), this);
  if (m_WithShortcuts)
  {
    m_Actions["Copy"]->setShortcuts(QKeySequence::Copy);
  }

  m_Actions["Cut"] = new QAction(tr("Cut"), this);
  if (m_WithShortcuts)
  {
    m_Actions["Cut"]->setShortcuts(QKeySequence::Cut);
  }

  m_Actions["Paste"] = new QAction(tr("Paste"), this);
  if (m_WithShortcuts)
  {
    m_Actions["Paste"]->setShortcuts(QKeySequence::Paste);
  }

  m_Actions["FindReplace"] = new QAction(openfluid::ui::common::getIcon("search","/ui/common",false), 
                                         tr("Find/Replace"), this);
  if (m_WithShortcuts)
  {
    m_Actions["FindReplace"]->setShortcuts(QKeySequence::Find);
  }

  m_Actions["GoToLine"] = new QAction(tr("Go to line..."), this);
  if (m_WithShortcuts)
  {
    m_Actions["GoToLine"]->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
  }


  // ====== Build ======

  // Configure ware

  m_Actions["ConfigureWare"] = new QAction(openfluid::ui::common::getIcon("configure","/ui/common",false),
                                           tr("Configure"), this);
  if (m_WithShortcuts)
  {
    m_Actions["ConfigureWare"]->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_B));
  }


  // Build ware

  m_Actions["BuildWare"] = new QAction(openfluid::ui::common::getIcon("build","/ui/common",false),
                                       tr("Build"), this);
  if (m_WithShortcuts)
  {
    m_Actions["BuildWare"]->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
  }


  // Generate doc

#if OPENFLUID_SIM2DOC_ENABLED
    m_Actions["GenerateDoc"] = new QAction(openfluid::ui::common::getIcon("generate-doc","/ui/common",false),
                                          tr("Generate doc"), this);
    if (m_WithShortcuts)
    {
      m_Actions["GenerateDoc"]->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    }
#endif


  // Options

  m_Actions["WareOptionsDialog"] =  new QAction(tr("Build options..."),this);


  // ====== Tools ======

  m_Actions["OpenTerminal"] = new QAction(tr("Open in terminal"), this);
  m_Actions["OpenExplorer"] = new QAction(tr("Open in file explorer"), this);


  // ====== Help ======

  m_Actions["APIDoc"] = new QAction(tr("API documentation"), this);
  m_Actions["APIDoc"]->setShortcuts(QKeySequence::HelpContents);

}


// =====================================================================
// =====================================================================


QAction* WareSrcActionsCollection::action(const QString& ActionName)
{
  if (m_Actions.contains(ActionName))
  {
    return m_Actions.value(ActionName);
  }

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                            "Action \"" + ActionName.toStdString() + "\" does'nt exist.");
}


} } }  // namespaces

