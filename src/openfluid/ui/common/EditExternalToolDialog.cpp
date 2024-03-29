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
  @file EditExternalToolDialog.cpp

  @author Armel THONI <armel.thoni@inrae.fr>
*/


#include <QFileDialog>
#include <QPushButton>

#include <openfluid/ui/common/EditExternalToolDialog.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_EditExternalToolDialog.h"


namespace openfluid { namespace ui { namespace common {


EditExternalToolDialog::EditExternalToolDialog(
  QWidget* Parent,
  const openfluid::base::PreferencesManager::ExternalTool_t& ExistingTool, 
  std::list<openfluid::base::PreferencesManager::ExternalTool_t>& AllTools):
    MessageDialog(Parent), ui(new Ui::EditExternalToolDialog),
    m_IsEditMode(!ExistingTool.Name.empty()), m_OriginalLabel(ExistingTool.Name), 
    m_AllTools(AllTools)
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
    setupMessageUi(tr("Edit an external tool"));
  }
  else
  {
    setupMessageUi(tr("Add an external tool"));
  }


  ui->LabelEdit->setText(QString::fromStdString(ExistingTool.Name));

  ui->WorkspaceCommandEdit->setText(QString::fromStdString(
    ExistingTool.getCommand(openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE)));
  ui->WareCommandEdit->setText(QString::fromStdString(
    ExistingTool.getCommand(openfluid::base::PreferencesManager::ExternalToolContext::WARE)));
  ui->FileCommandEdit->setText(QString::fromStdString(
    ExistingTool.getCommand(openfluid::base::PreferencesManager::ExternalToolContext::FILE)));
  

  // "required" placeholder
  QString PlaceholderStr = getPlaceholderRequired();
  ui->LabelEdit->setPlaceholderText(PlaceholderStr);

  connect(ui->LabelEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobally()));

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


EditExternalToolDialog::~EditExternalToolDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditExternalToolDialog::checkGlobally()
{
  auto checkLabelExist = [&]()
  {
    for (const auto& C : m_AllTools)
    {
      if (C.Name == ui->LabelEdit->text().toStdString())
      {
        return true;
      }
    }
    return false;
  };

  if (ui->LabelEdit->text().isEmpty())
  {
    setMessage(tr("The label cannot be empty"));
  }
  else if (checkLabelExist() && 
           (!m_IsEditMode || (m_IsEditMode && m_OriginalLabel != ui->LabelEdit->text().toStdString())))
  {
    setMessage(tr("This label already exists"));
  }
  else
  {
    setMessage();
  }
}


// =====================================================================
// =====================================================================


void EditExternalToolDialog::addAppToWorkspaceLine()
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


void EditExternalToolDialog::addAppToWareLine()
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


void EditExternalToolDialog::addAppToFileLine()
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


void EditExternalToolDialog::addTargetToWorkspaceLine()
{
    ui->WorkspaceCommandEdit->modifyText("%%W%%");
}


// =====================================================================
// =====================================================================


void EditExternalToolDialog::addTargetToWareLine()
{
  ui->WareCommandEdit->modifyText("%%S%%");
}


// =====================================================================
// =====================================================================


void EditExternalToolDialog::addTargetToFileLine()
{
  ui->FileCommandEdit->modifyText("%%C%%");
}


// =====================================================================
// =====================================================================


openfluid::base::PreferencesManager::ExternalTool_t EditExternalToolDialog::getTool() const
{
  openfluid::base::PreferencesManager::ExternalTool_t Tool;

  Tool.Name = ui->LabelEdit->text().toStdString();

  if (!ui->WorkspaceCommandEdit->text().isEmpty())
  {
    Tool.Commands[openfluid::base::PreferencesManager::ExternalToolContext::WORKSPACE] = 
      ui->WorkspaceCommandEdit->text().toStdString();
  }

  if (!ui->WareCommandEdit->text().isEmpty())
  {
    Tool.Commands[openfluid::base::PreferencesManager::ExternalToolContext::WARE] = 
      ui->WareCommandEdit->text().toStdString();
  }

  if (!ui->FileCommandEdit->text().isEmpty())
  {
    Tool.Commands[openfluid::base::PreferencesManager::ExternalToolContext::FILE] = 
      ui->FileCommandEdit->text().toStdString();
  }  

  return Tool;
}

} } } // namespaces

