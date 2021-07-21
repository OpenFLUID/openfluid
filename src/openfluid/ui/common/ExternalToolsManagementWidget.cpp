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
#include <openfluid/ui/common/EditExternalToolDialog.hpp>
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

  m_ExternalTools = openfluid::base::PreferencesManager::instance()->getWaresdevExternalTools();

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
  for (const auto& Tool : m_ExternalTools)
  {
    ui->ToolsListWidget->addItem(QString::fromStdString(Tool.Name));
  }
}


// =====================================================================
// =====================================================================


void ExternalToolsManagementWidget::addTool()
{
  EditExternalToolDialog ExtToolDlg(this, openfluid::base::PreferencesManager::ExternalTool_t(), m_ExternalTools);

  if (ExtToolDlg.exec() == QDialog::Accepted)
  {
    m_ExternalTools.push_back(ExtToolDlg.getTool());

    update();
    emit toolsUpdated();
  }
}


// =====================================================================
// =====================================================================


void ExternalToolsManagementWidget::editTool()
{
  auto Index = ui->ToolsListWidget->currentRow();

  if (Index >= 0 && static_cast<unsigned int>(Index) < m_ExternalTools.size())
  {
    auto it = m_ExternalTools.begin();
    std::advance(it,Index);
    EditExternalToolDialog ExtToolDlg(this,*it,m_ExternalTools);

    if (ExtToolDlg.exec() == QDialog::Accepted)
    {
      auto EditedTool = ExtToolDlg.getTool();
      *it = EditedTool;

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
  auto Index = ui->ToolsListWidget->currentRow();

  if (Index >= 0)
  {
    if (QMessageBox::question(this,tr("Delete tool from list"),
                              tr("Deleting this tool will erase all corresponding commands.")+
                              "\n\n"+
                              tr("Proceed anyway?"),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
      auto it = m_ExternalTools.begin();
      std::advance(it,Index);

      m_ExternalTools.erase(it);

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
  auto Index = ui->ToolsListWidget->currentRow();

  if (Index >= 1 && static_cast<unsigned int>(Index) < m_ExternalTools.size())
  {
    auto itFirst = m_ExternalTools.begin();
    std::advance(itFirst,Index-1);
    auto itSecond = m_ExternalTools.begin();
    std::advance(itSecond,Index);
   
    std::iter_swap(itFirst,itSecond);

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
  auto Index = ui->ToolsListWidget->currentRow();

  if (Index>= 0 && static_cast<unsigned int>(Index+1) < m_ExternalTools.size())
  {
    auto itFirst = m_ExternalTools.begin();
    std::advance(itFirst,Index);
    auto itSecond = m_ExternalTools.begin();
    std::advance(itSecond,Index+1);
   
    std::iter_swap(itFirst,itSecond);

    m_NeedRestart = true;
    update();
    ui->ToolsListWidget->setCurrentRow(Index+1);
    emit toolsUpdated();    
  }
}


// =====================================================================
// =====================================================================


const std::list<openfluid::base::PreferencesManager::ExternalTool_t>& 
  ExternalToolsManagementWidget::getToolsList() const
{
  return m_ExternalTools;
}


} } } // namespaces

