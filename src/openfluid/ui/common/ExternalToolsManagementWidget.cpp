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
  @file ExternalToolsManagementWidget.cpp

  @author Armel THONI <armel.thoni@inrae.fr>
*/


#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/common/EditExtToolDialog.hpp>
#include <openfluid/ui/common/ExternalToolsManagementWidget.hpp>

#include "ui_ExternalToolsManagementWidget.h"


namespace openfluid { namespace ui { namespace common {


ExternalToolsManagementWidget::ExternalToolsManagementWidget(QWidget* Parent):
  QWidget(Parent),ui(new Ui::ExternalToolsManagementWidget)
{
  ui->setupUi(this);

  ui->AddButton->setText("");
  ui->AddButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddButton->setIconSize(QSize(20,20));

  ui->EditButton->setText("");
  ui->EditButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->EditButton->setIconSize(QSize(20,20));

  ui->RemoveButton->setText("");
  ui->RemoveButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveButton->setIconSize(QSize(20,20));

  ui->UpButton->setText("");
  ui->UpButton->setIcon(openfluid::ui::common::getIcon("go-up","/ui/common"));
  ui->UpButton->setIconSize(QSize(20,20));

  ui->DownButton->setText("");
  ui->DownButton->setIcon(openfluid::ui::common::getIcon("go-down","/ui/common"));
  ui->DownButton->setIconSize(QSize(20,20));

  connect(ui->AddButton,SIGNAL(clicked()),this,SLOT(addTool()));
  connect(ui->EditButton,SIGNAL(clicked()),this,SLOT(editTool()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removeTool()));
  connect(ui->UpButton,SIGNAL(clicked()),this,SLOT(moveupTool()));
  connect(ui->DownButton,SIGNAL(clicked()),this,SLOT(movedownTool()));

  m_ToolCommands = openfluid::base::PreferencesManager::instance()->getWaresdevExternalToolsCommands();
  QList<QString> FullToolsOrder = openfluid::base::PreferencesManager::instance()->getWaresdevExternalToolsOrder();

  for (const auto& Command : FullToolsOrder)
  {
    if (m_ToolCommands.contains(Command) && !m_ToolsOrder.contains(Command))
    {
      m_ToolsOrder.append(Command);
    }
  }

  if (m_ToolsOrder.isEmpty())
  {
    m_ToolsOrder = m_ToolCommands.keys();
  }

  update();
}


// =====================================================================
// =====================================================================


ExternalToolsManagementWidget::~ExternalToolsManagementWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void ExternalToolsManagementWidget::update()
{
  ui->ToolsListWidget->clear();
  for (const auto& Tool : m_ToolsOrder)
  {
    ui->ToolsListWidget->addItem(Tool);
  }
}


// =====================================================================
// =====================================================================


void ExternalToolsManagementWidget::addTool()
{
  EditExtToolDialog ExtToolDlg(this, "", QStringList(), m_ToolCommands);

  if (ExtToolDlg.exec() == QDialog::Accepted)
  {
    QString ToolName = ExtToolDlg.getName();
    QStringList FullCommands = ExtToolDlg.getFullCommands();
    m_ToolCommands[ToolName] = FullCommands;
    m_ToolsOrder << ToolName;
    m_NeedRestart = true;
    update();
    emit toolsUpdated();
  }
}


// =====================================================================
// =====================================================================


void ExternalToolsManagementWidget::editTool()
{
  int CurrentRow = ui->ToolsListWidget->currentRow();

  if (CurrentRow >= 0)
  {
    QListWidgetItem* CurrentItem = ui->ToolsListWidget->item(CurrentRow);
    QString SelectedToolName = CurrentItem->text();
    EditExtToolDialog ExtToolDlg(this, SelectedToolName, 
                                 m_ToolCommands[SelectedToolName], m_ToolCommands);

    if (ExtToolDlg.exec() == QDialog::Accepted)
    {
      QString FromDlgToolName = ExtToolDlg.getName();
      QStringList FullCommands = ExtToolDlg.getFullCommands();
      if (SelectedToolName != FromDlgToolName)
      {
        m_ToolCommands.insert(FromDlgToolName, FullCommands);
        m_ToolCommands.remove(SelectedToolName);
        m_ToolsOrder.removeAt(CurrentRow);
        m_ToolsOrder << FromDlgToolName;
      }
      else
      {
        m_ToolCommands[FromDlgToolName] = FullCommands;
      }
      m_NeedRestart = true;
      update();
      emit toolsUpdated();
    }
  }
}


// =====================================================================
// =====================================================================


void ExternalToolsManagementWidget::removeTool()
{
  if (ui->ToolsListWidget->currentRow() >= 0)
  {
    if (QMessageBox::question(this,tr("Delete tool from list"),
                              tr("Deleting this tool will erase all corresponding commands.")+
                              "\n\n"+
                              tr("Proceed anyway?"),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
      m_ToolCommands.remove(ui->ToolsListWidget->currentItem()->text());
      m_ToolsOrder.removeAt(ui->ToolsListWidget->currentRow());
      m_NeedRestart = true;
      update();
      emit toolsUpdated();
    }
  }
}


// =====================================================================
// =====================================================================


void ExternalToolsManagementWidget::moveupTool()
{
  int Index = ui->ToolsListWidget->currentRow();

  if (Index >= 1)
  {
    m_ToolsOrder.move(Index, Index-1);
    m_NeedRestart = true;
    update();
    ui->ToolsListWidget->setCurrentRow(Index-1);
    emit toolsUpdated();
  }
}


// =====================================================================
// =====================================================================


void ExternalToolsManagementWidget::movedownTool()
{
  int Index = ui->ToolsListWidget->currentRow();

  if (Index + 1 < ui->ToolsListWidget->count())
  {
    m_ToolsOrder.move(Index+1, Index);
    m_NeedRestart = true;
    update();
    ui->ToolsListWidget->setCurrentRow(Index+1);
    emit toolsUpdated();
  }
}


// =====================================================================
// =====================================================================


openfluid::base::PreferencesManager::ExternalToolsCommands_t ExternalToolsManagementWidget::getToolsList() const
{
  return m_ToolCommands;
}


// =====================================================================
// =====================================================================


QList<QString> ExternalToolsManagementWidget::getToolsOrder() const
{
  return m_ToolsOrder;
}


} } } // namespaces

