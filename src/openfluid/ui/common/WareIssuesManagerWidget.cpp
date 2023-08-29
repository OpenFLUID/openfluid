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
  @file WareIssuesManagerWidget.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <fstream>

#include <QTextStream>
#include <QMessageBox>

#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/ui/common/WareIssuesManagerWidget.hpp>
#include <openfluid/ui/common/WareshubIssueDialog.hpp>
#include <openfluid/base/FrameworkException.hpp>

#include "ui_WareIssuesManagerWidget.h"


namespace openfluid { namespace ui { namespace common {


WareIssuesManagerWidget::WareIssuesManagerWidget(QWidget* Parent) :
    QWidget(Parent), ui(new Ui::WareIssuesManagerWidget)
{
  ui->setupUi(this);

  connect(ui->RemoveButton, SIGNAL(clicked()), this, SLOT(onRemoveIssueClicked()));
  connect(ui->EditButton, SIGNAL(clicked()), this, SLOT(onEditIssueClicked()));
  connect(ui->AddButton, SIGNAL(clicked()), this, SLOT(onAddIssueClicked()));

  connect(ui->IssuesTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onEditIssueClicked()));
}


// =====================================================================
// =====================================================================


WareIssuesManagerWidget::~WareIssuesManagerWidget()
{

}


// =====================================================================
// =====================================================================


void WareIssuesManagerWidget::loadContent(const openfluid::ware::WareIssues& WareIssues)
{
  m_IssuesByID.clear();
  for (const auto& Issue : WareIssues.getAll())
  {
    m_IssuesByID[Issue.first] = Issue.second;
  }
  issuesMapToJsonIssues();
  updateContent();
}


// =====================================================================
// =====================================================================


openfluid::ware::WareIssues WareIssuesManagerWidget::getIssues() const
{
  openfluid::ware::WareIssues WareIssues;
  for (const auto& I : m_IssuesByID)
  {
    auto WI = I.toWareIssue();
    WareIssues.insert(WI, WI.ID);
  }
  return WareIssues;
}


// =====================================================================
// =====================================================================


void WareIssuesManagerWidget::issuesMapToJsonIssues()
{
  openfluid::thirdparty::json JSONIssuesObject(openfluid::thirdparty::json::value_t::array);

  for (const auto& Issue : m_IssuesByID.values())
  {
    JSONIssuesObject.push_back(Issue.toWareIssue().toJSON());
  }

  m_Doc = JSONIssuesObject;
}


// =====================================================================
// =====================================================================


void WareIssuesManagerWidget::updateContent()
{
  jsonIssuesToIssuesMap();
  updateIssuesTable();
}


// =====================================================================
// =====================================================================


void WareIssuesManagerWidget::jsonIssuesToIssuesMap()
{
  QStringList Values;

  const openfluid::thirdparty::json& IssuesValue = m_Doc;

  if (IssuesValue.is_array())
  {
    for (const auto& [Index,Info] : IssuesValue.items())
    {

      if (Info.is_object())
      {
        WareshubIssueDialog::Issue I(Info);

        m_IssuesByID[I.m_ID] = I;
      }
    }
  }
}


// =====================================================================
// =====================================================================


void WareIssuesManagerWidget::updateIssuesTable()
{
  ui->IssuesTable->clear();

  ui->IssuesTable->setHorizontalHeaderLabels(QStringList( { tr("ID"), tr("Title"), tr("Tags"), tr("State") }));
  ui->IssuesTable->verticalHeader()->setVisible(false);
  ui->IssuesTable->horizontalHeader()->setStretchLastSection(false);
  ui->IssuesTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

  ui->IssuesTable->setRowCount(m_IssuesByID.size());

  int row = 0;
  for (const auto& ID : m_IssuesByID.keys())
  {
    QTableWidgetItem* IDItem = new QTableWidgetItem("");
    IDItem->setData(Qt::DisplayRole, ID);
    ui->IssuesTable->setItem(row, 0, IDItem);

    ui->IssuesTable->setItem(row, 1, new QTableWidgetItem(m_IssuesByID[ID].m_Title));
    ui->IssuesTable->setItem(row, 2, new QTableWidgetItem(m_IssuesByID[ID].m_Tags));
    ui->IssuesTable->setItem(row, 3, new QTableWidgetItem(m_IssuesByID[ID].m_State));

    row++;
  }

  ui->IssuesTable->resizeColumnsToContents();
}


// =====================================================================
// =====================================================================


QWidget* WareIssuesManagerWidget::getWidget()
{
  return this;
}


// =====================================================================
// =====================================================================


void WareIssuesManagerWidget::onRemoveIssueClicked()
{
  QModelIndexList selectedList = ui->IssuesTable->selectionModel()->selectedRows();

  if (!selectedList.isEmpty())
  {
    QModelIndex Selected = selectedList.at(0);
    unsigned int ID = Selected.data(Qt::DisplayRole).toUInt();

    if (QMessageBox::question(
                              this,
                              tr("Remove an issue"),
                              tr("Are you sure you want to remove issue \"%1\" ?").arg(ID),
                              QMessageBox::Ok | QMessageBox::Cancel)
        == QMessageBox::Ok)
    {
      ui->IssuesTable->removeRow(Selected.row());

      m_IssuesByID.remove(ID);
    }
  }
}


// =====================================================================
// =====================================================================


void WareIssuesManagerWidget::onEditIssueClicked()
{
   QModelIndexList selectedList = ui->IssuesTable->selectionModel()->selectedRows();

  if (!selectedList.isEmpty())
  {
    QModelIndex Selected = selectedList.at(0);
    unsigned int ID = Selected.data(Qt::DisplayRole).toUInt();

    WareshubIssueDialog Dialog(m_IssuesByID.keys(), this, m_IssuesByID[ID]);

    if (Dialog.exec() == QDialog::Accepted)
    {
      int Row = Selected.row();

      WareshubIssueDialog::Issue I = Dialog.getIssue();

      if (ID != I.m_ID)
      {
        QTableWidgetItem* IDItem = ui->IssuesTable->item(Row, 0);
        IDItem->setData(Qt::DisplayRole, I.m_ID);

        Row = IDItem->row();

        m_IssuesByID.remove(I.m_ID);
      }

      ui->IssuesTable->item(Row, 1)->setData(Qt::DisplayRole, I.m_Title);
      ui->IssuesTable->item(Row, 2)->setData(Qt::DisplayRole, I.m_Tags);
      ui->IssuesTable->item(Row, 3)->setData(Qt::DisplayRole, I.m_State);

      m_IssuesByID[I.m_ID] = I;

      ui->IssuesTable->selectRow(Row);
    }
  }

}


// =====================================================================
// =====================================================================


void WareIssuesManagerWidget::onAddIssueClicked()
{
  WareshubIssueDialog Dialog(m_IssuesByID.keys(), this);

  if (Dialog.exec() == QDialog::Accepted)
  {
    WareshubIssueDialog::Issue I = Dialog.getIssue();

    int RowCountBefore = ui->IssuesTable->rowCount();

    ui->IssuesTable->setRowCount(RowCountBefore + 1);

    QTableWidgetItem* IDItem = new QTableWidgetItem("");
    IDItem->setData(Qt::DisplayRole, I.m_ID);
    ui->IssuesTable->setItem(RowCountBefore, 0, IDItem);

    int NewRow = IDItem->row();

    ui->IssuesTable->setItem(NewRow, 1, new QTableWidgetItem(I.m_Title));
    ui->IssuesTable->setItem(NewRow, 2, new QTableWidgetItem(I.m_Tags));
    ui->IssuesTable->setItem(NewRow, 3, new QTableWidgetItem(I.m_State));

    m_IssuesByID[I.m_ID] = I;

    ui->IssuesTable->selectRow(NewRow);
  }
}


// =====================================================================
// =====================================================================


} } } // namespaces
