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
  @file SourceAddDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include "ui_SourceAddDialog.h"
#include "SourceAddDialog.hpp"
#include "SourceWorker.hpp"

#include <QThread>
#include <QMessageBox>


SourceAddDialog::SourceAddDialog(QWidget* Parent):
  QDialog(Parent), ui(new Ui::SourceAddDialog),
  mp_DataSource(NULL), m_CurrentSourceURI("")
{
  ui->setupUi(this);

  ui->StatusLabel->setText(tr("No layer"));
  ui->StatusProgressBar->setVisible(false);

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
  ui->LayersTableWidget->setEnabled(false);
  ui->ButtonBox->setEnabled(false);
  ui->StatusProgressBar->setValue(0);
  ui->LayersTableWidget->setRowCount(0);
  ui->StatusProgressBar->setTextVisible(false);
  ui->StatusProgressBar->setVisible(true);
  ui->StatusLabel->setText(tr("Connecting to source"));
  setCursor(Qt::WaitCursor);


  QThread* WThread = new QThread;
  SourceWorker* Worker = new SourceWorker(m_CurrentSourceURI,mp_DataSource);
  Worker->moveToThread(WThread);

  connect(Worker, SIGNAL(finished()), this, SLOT(handleSourceFinished()));
  connect(Worker, SIGNAL(sourceLinked(void*)), this, SLOT(handleSourceLinked(void*)));
  connect(Worker, SIGNAL(layerCounted(int)), this, SLOT(handleLayerCounted(int)));
  connect(Worker, SIGNAL(layerFetched(int,QString,QString)), this, SLOT(handleLayerFetched(int,QString,QString)));
  connect(Worker, SIGNAL(errorHappened(QString)), this, SLOT(handleSourceError(QString)));
  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));

  WThread->start();
}


// =====================================================================
// =====================================================================


void SourceAddDialog::handleSourceLinked(void* Src)
{
  mp_DataSource = (OGRDataSource*)Src;
}


// =====================================================================
// =====================================================================


void SourceAddDialog::handleLayerCounted(int Count)
{
  ui->StatusLabel->setText(tr("Fetching layers..."));

  ui->StatusProgressBar->setTextVisible(true);
  ui->StatusProgressBar->setMaximum(Count);
  ui->StatusProgressBar->setValue(0);
  ui->LayersTableWidget->setRowCount(Count);
}


// =====================================================================
// =====================================================================


void SourceAddDialog::handleLayerFetched(int Index, QString Name, QString GeomStr)
{
  ui->StatusProgressBar->setValue(Index+1);
  ui->LayersTableWidget->setItem(Index,0,new QTableWidgetItem(Name));
  ui->LayersTableWidget->setItem(Index,1,new QTableWidgetItem(GeomStr));
}


// =====================================================================
// =====================================================================


void SourceAddDialog::handleSourceFinished()
{
  updateAfterOpen();

  ui->LayersTableWidget->selectRow(0);
  ui->StatusProgressBar->setVisible(false);
  ui->BrowseButton->setEnabled(true);
  ui->ConnectButton->setEnabled(true);
  ui->URLLineEdit->setEnabled(true);
  ui->LayersTableWidget->setEnabled(true);
  ui->ButtonBox->setEnabled(true);
  setCursor(Qt::ArrowCursor);

  globalCheck();
}


// =====================================================================
// =====================================================================


void SourceAddDialog::handleSourceError(QString Message)
{
  if (!Message.isEmpty())
    QMessageBox::critical(this,tr("Source error"),Message);
  else
    QMessageBox::critical(this,tr("Source error"),tr("Error opening source")+"\n" +
                                                  m_CurrentSourceURI+"\n\n" +
                                                  tr("Aborting."));

  m_CurrentSourceURI.clear();

  updateAfterOpen();

  ui->StatusProgressBar->setVisible(false);
  ui->LayersTableWidget->setRowCount(0);
  ui->BrowseButton->setEnabled(true);
  ui->ConnectButton->setEnabled(true);
  ui->URLLineEdit->setEnabled(true);
  ui->LayersTableWidget->setEnabled(true);
  ui->ButtonBox->setEnabled(true);
  setCursor(Qt::ArrowCursor);

  globalCheck();
}


// =====================================================================
// =====================================================================


void SourceAddDialog::globalCheck()
{
  int LayerCount = ui->LayersTableWidget->rowCount();

  if (LayerCount == 0) ui->StatusLabel->setText(tr("No layer"));
  else ui->StatusLabel->setText(tr("%1 layer(s) available").arg(LayerCount));

  bool OK = mp_DataSource != NULL &&
            !m_CurrentSourceURI.isEmpty() &&
            ui->LayersTableWidget->selectedItems().size() > 0;

  ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(OK);
}


// =====================================================================
// =====================================================================


bool SourceAddDialog::checkRequiredFields()
{
  if (mp_DataSource == NULL) return false;

  OGRFeatureDefn* Defn = mp_DataSource->GetLayerByName(m_SrcInfos.LayerName.toStdString().c_str())->GetLayerDefn();

  int FIndex = Defn->GetFieldIndex(OGRGDAL_UNITID_FIELD);
  if (FIndex >=0 && (Defn->GetFieldDefn(FIndex)->GetType() == OFTInteger ||
                     Defn->GetFieldDefn(FIndex)->GetType() == OFTReal ||
                     Defn->GetFieldDefn(FIndex)->GetType() == OFTString)) return true;

    return false;
}


// =====================================================================
// =====================================================================


void SourceAddDialog::proceedToImport()
{
  if (prepareToImport())
  {
    m_SrcInfos.SourceGeomType = mp_DataSource->GetLayer(ui->LayersTableWidget->currentRow())->GetGeomType();
    m_SrcInfos.LayerName = QString(mp_DataSource->GetLayer(ui->LayersTableWidget->currentRow())->GetName());

    if (checkRequiredFields())
    {
      OGRFeatureDefn* Defn = mp_DataSource->GetLayerByName(m_SrcInfos.LayerName.toStdString().c_str())->GetLayerDefn();

      for (int i=0; i< Defn->GetFieldCount();i++)
        m_SrcInfos.AvailableFields.append(Defn->GetFieldDefn(i)->GetNameRef());

      if (m_SrcInfos.AvailableFields.contains(OGRGDAL_PSORD_FIELD))
        m_SrcInfos.UnitsPcsOrdField = OGRGDAL_PSORD_FIELD;

      if (m_SrcInfos.AvailableFields.contains(OGRGDAL_TOCONN_FIELD))
        m_SrcInfos.ToConnectionsField = OGRGDAL_TOCONN_FIELD;

      if (m_SrcInfos.AvailableFields.contains(OGRGDAL_CHILDOF_FIELD))
        m_SrcInfos.ChildofConnectionsField = OGRGDAL_CHILDOF_FIELD;

      accept();
    }
    else
    {
      QMessageBox::critical(this,tr("Import error"),
                            tr("Error importing from source")+"\n"+
                            m_CurrentSourceURI+"\n\n"+
                            tr("The mandatory field \"OFLD_ID\" cannot be found.")+"\n\n"+
                            tr("Aborting."));
      reject();
    }
  }
  else
  {
    QMessageBox::critical(this,tr("Import error"),
                          tr("Error importing from source")+"\n"+
                          m_CurrentSourceURI+"\n\n"+
                          tr("Aborting."));
    reject();
  }
}


