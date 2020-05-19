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
  @file EditExtToolDialog.cpp

  @author Armel THONI <armel.thoni@inrae.fr>
*/


#include <QFileDialog>
#include <QPushButton>

#include <openfluid/ui/common/EditExtToolDialog.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_EditExtToolDialog.h"


namespace openfluid { namespace ui { namespace common {


EditExtToolDialog::EditExtToolDialog(QWidget* Parent,
                                     const QString& Name, 
                                     const QStringList& ToolCommands,
                                     const openfluid::base::PreferencesManager::ExternalToolsCommands_t& AllCommands):
  MessageDialog(Parent), ui(new Ui::EditExtToolDialog),
  m_IsEditMode(!Name.isEmpty()), m_OriginalName(Name), m_AllCommands(AllCommands)
{
  ui->setupUi(this);

  for (auto& Button : {ui->AppWorkspaceButton, ui->AppWareButton, ui->AppFileButton})
  {
    Button->setText("");
    Button->setIcon(openfluid::ui::common::getIcon("application","/ui/common"));
    Button->setIconSize(QSize(20,20));
  }

  for (auto& Button : {ui->FolderWorkspaceButton, ui->FolderWareButton, ui->FolderFileButton})
  {
    Button->setText("");
    Button->setIcon(openfluid::ui::common::getIcon("folder-pound","/ui/common"));
    Button->setIconSize(QSize(20,20));
  }

  if (m_IsEditMode)
  {
    setupMessageUi(tr("Edit an external command"));
  }
  else
  {
    setupMessageUi(tr("Add an external command"));
  }


  ui->NameEdit->setText(Name);

  for (int ContextNumber=0 ; ContextNumber<ToolCommands.size() ; ContextNumber++)
  {
    QString Command = ToolCommands[ContextNumber];
    if (ContextNumber == static_cast<int>(openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE))
    {
      ui->WorkspaceCommandEdit->setText(Command);
    }
    else if (ContextNumber == static_cast<int>(openfluid::base::PreferencesManager::ExternalToolContext::WARE))
    {
      ui->WareCommandEdit->setText(Command);
    }
    else if (ContextNumber == static_cast<int>(openfluid::base::PreferencesManager::ExternalToolContext::FILE))
    {
      ui->FileCommandEdit->setText(Command);
    }
  }

  // "required" placeholder
  QString PlaceholderStr = getPlaceholderRequired();
  ui->NameEdit->setPlaceholderText(PlaceholderStr);

  connect(ui->NameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobally()));

  connect(ui->AppWorkspaceButton,SIGNAL(released()),this,SLOT(addAppToWorkspaceLine()));
  connect(ui->AppWareButton,SIGNAL(released()),this,SLOT(addAppToWareLine()));
  connect(ui->AppFileButton,SIGNAL(released()),this,SLOT(addAppToFileLine()));

  connect(ui->FolderWorkspaceButton,SIGNAL(released()),this,SLOT(addTargetToWorkspaceLine()));
  connect(ui->FolderWareButton,SIGNAL(released()),this,SLOT(addTargetToWareLine()));
  connect(ui->FolderFileButton,SIGNAL(released()),this,SLOT(addTargetToFileLine()));

  checkGlobally();
}


// =====================================================================
// =====================================================================


EditExtToolDialog::~EditExtToolDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditExtToolDialog::checkGlobally()
{
  if (ui->NameEdit->text().isEmpty())
  {
    setMessage(tr("Alias cannot be empty"));
  }
  else if (m_AllCommands.contains(ui->NameEdit->text()) && 
            (!m_IsEditMode || 
            (m_IsEditMode && m_OriginalName != ui->NameEdit->text())))
  {
    setMessage(tr("Alias already exists"));
  }
  else
  {
    setMessage();
  }
}


// =====================================================================
// =====================================================================


void EditExtToolDialog::addAppToWorkspaceLine()
{
  QString SelectedCommand = QFileDialog::getOpenFileName(this, tr("Select program path"));

  if (SelectedCommand != "")
  {
    if (SelectedCommand.contains(" "))
    {
      SelectedCommand = "\""+SelectedCommand+"\"";
    }
    ui->WorkspaceCommandEdit->modifyText(SelectedCommand);
  }
}


// =====================================================================
// =====================================================================


void EditExtToolDialog::addAppToWareLine()
{
  QString SelectedCommand = QFileDialog::getOpenFileName(this, tr("Select program path"));
  if (SelectedCommand != "")
  {
    if (SelectedCommand.contains(" "))
    {
      SelectedCommand = "\""+SelectedCommand+"\"";
    }
    ui->WareCommandEdit->modifyText(SelectedCommand);
  }
}


// =====================================================================
// =====================================================================


void EditExtToolDialog::addAppToFileLine()
{
  QString SelectedCommand = QFileDialog::getOpenFileName(this, tr("Select program path"));
  if (SelectedCommand != "")
  {
    if (SelectedCommand.contains(" "))
    {
      SelectedCommand = "\""+SelectedCommand+"\"";
    }
    ui->FileCommandEdit->modifyText(SelectedCommand);
  }
}


// =====================================================================
// =====================================================================


void EditExtToolDialog::addTargetToWorkspaceLine()
{
    ui->WorkspaceCommandEdit->modifyText("%%W%%");
}


// =====================================================================
// =====================================================================


void EditExtToolDialog::addTargetToWareLine()
{
  ui->WareCommandEdit->modifyText("%%S%%");
}


// =====================================================================
// =====================================================================


void EditExtToolDialog::addTargetToFileLine()
{
  ui->FileCommandEdit->modifyText("%%C%%");
}


// =====================================================================
// =====================================================================


QStringList EditExtToolDialog::getFullCommands() const
{
  QStringList Commands;
  Commands << ui->WorkspaceCommandEdit->text()
           << ui->WareCommandEdit->text()
           << ui->FileCommandEdit->text();
  return Commands;
}


// =====================================================================
// =====================================================================


QString EditExtToolDialog::getName() const
{
  return ui->NameEdit->text();
}


} } } // namespaces

