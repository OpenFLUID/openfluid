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
  \file SourceAddDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_SourceAddDialog.h"
#include "SourceAddDialog.hpp"


SourceAddDialog::SourceAddDialog(QWidget* Parent = NULL):
  QDialog(Parent), ui(new Ui::SourceAddDialog),
  mp_DataSource(NULL), m_CurrentSourceURI("")
{
  ui->setupUi(this);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  ui->LayersTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#else
  ui->LayersTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif

  connect(ui->LayersTableWidget,SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),
          this,SLOT(globalCheck()));

  ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(proceedToImport()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


SourceAddDialog::~SourceAddDialog()
{
  OGRDataSource::DestroyDataSource(mp_DataSource);
  delete ui;
}


// =====================================================================
// =====================================================================


void SourceAddDialog::openDataSource()
{
  ui->BrowseButton->setEnabled(false);
  ui->ConnectButton->setEnabled(false);
  ui->URLLineEdit->setEnabled(false);
  ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

  setCursor(Qt::WaitCursor);

  // TODO move processing to thread

  OGRDataSource::DestroyDataSource(mp_DataSource);
  mp_DataSource = NULL;

  ui->LayersTableWidget->setRowCount(0);

  OGRRegisterAll();

  mp_DataSource = OGRSFDriverRegistrar::Open(m_CurrentSourceURI.toStdString().c_str(), FALSE );

  if (mp_DataSource != NULL)
  {
    ui->LayersTableWidget->setRowCount(mp_DataSource->GetLayerCount());

    for (int i=0; i < mp_DataSource->GetLayerCount();i++)
    {
      ui->LayersTableWidget->setItem(i,0,new QTableWidgetItem(QString(mp_DataSource->GetLayer(i)->GetName())));
      ui->LayersTableWidget->setItem(i,1,new QTableWidgetItem(QString(OGRGeometryTypeToName(mp_DataSource->GetLayer(i)->GetGeomType()))));
    }

    ui->LayersTableWidget->selectRow(0);
  }

  ui->BrowseButton->setEnabled(true);
  ui->ConnectButton->setEnabled(true);
  ui->URLLineEdit->setEnabled(true);
  setCursor(Qt::ArrowCursor);

  globalCheck();
}


// =====================================================================
// =====================================================================


void SourceAddDialog::globalCheck()
{
  bool OK = mp_DataSource != NULL &&
            !m_CurrentSourceURI.isEmpty() &&
            ui->LayersTableWidget->selectedItems().size() > 0;

  ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(OK);
}


// =====================================================================
// =====================================================================


void SourceAddDialog::proceedToImport()
{
  prepareToImport();

  m_SrcInfos.SourceGeomType = mp_DataSource->GetLayer(ui->LayersTableWidget->currentRow())->GetGeomType();
  m_SrcInfos.LayerName = QString(mp_DataSource->GetLayer(ui->LayersTableWidget->currentRow())->GetName());

  OGRFeatureDefn* Defn = mp_DataSource->GetLayerByName(m_SrcInfos.LayerName.toStdString().c_str())->GetLayerDefn();

  for (int i=0; i< Defn->GetFieldCount();i++)
    m_SrcInfos.AvailableFields.append(Defn->GetFieldDefn(i)->GetNameRef());

  accept();
}


