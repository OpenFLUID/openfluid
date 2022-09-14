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
  @file DatastoreWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_api.h>
#include <ogrsf_frmts.h>

#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

#include <openfluid/utilsq/QtHelpers.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/utils/GDALCompatibility.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_DatastoreWidget.h"
#include "DatastoreWidget.hpp"
#include "AddDatastoreItemDialog.hpp"


DatastoreWidget::DatastoreWidget(QWidget* Parent, openfluid::fluidx::FluidXDescriptor& FXDesc):
  WorkspaceWidget(Parent,FXDesc), ui(new Ui::DatastoreWidget), m_Datastore(FXDesc.datastore())
{
  ui->setupUi(this);

  ui->AddButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddButton->setIconSize(QSize(20,20));

  ui->EditButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->EditButton->setIconSize(QSize(20,20));

  ui->RemoveButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveButton->setIconSize(QSize(20,20));

  ui->UpButton->setIcon(openfluid::ui::common::getIcon("go-up","/ui/common"));
  ui->UpButton->setIconSize(QSize(20,20));

  ui->DownButton->setIcon(openfluid::ui::common::getIcon("go-down","/ui/common"));
  ui->DownButton->setIconSize(QSize(20,20));

  // TODO to re-enable
  ui->EditButton->setVisible(false);

  connect(ui->AddButton,SIGNAL(clicked()),this,SLOT(addItem()));
  connect(ui->EditButton,SIGNAL(clicked()),this,SLOT(editItem()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removeItem()));

  connect(ui->UpButton,SIGNAL(clicked()),this,SLOT(moveItemUp()));
  connect(ui->DownButton,SIGNAL(clicked()),this,SLOT(moveItemDown()));

  connect(ui->DatastoreTableWidget->horizontalHeader(),SIGNAL(sectionResized(int, int, int)),
          ui->DatastoreTableWidget,SLOT(resizeRowsToContents()));

  refresh();
}


// =====================================================================
// =====================================================================


DatastoreWidget::~DatastoreWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void DatastoreWidget::refresh()
{
  ui->DatastoreTableWidget->setRowCount(0);

  const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& Items = m_Datastore.items();

  ui->DatastoreTableWidget->setRowCount(Items.size());

  int CurrentRow = 0;

  for (std::list<openfluid::fluidx::DatastoreItemDescriptor*>::const_iterator it = Items.begin();
       it != Items.end(); ++it)
  {
    QTableWidgetItem *Item;

    Item = new QTableWidgetItem(QString::fromStdString((*it)->getID()));
    ui->DatastoreTableWidget->setItem(CurrentRow, 0, Item);

    Item = new QTableWidgetItem(QString::fromStdString(
        openfluid::core::UnstructuredValue::getStringFromValueType((*it)->getType())));
    ui->DatastoreTableWidget->setItem(CurrentRow, 1, Item);

    Item = new QTableWidgetItem(QString::fromStdString((*it)->getUnitsClass()));
    ui->DatastoreTableWidget->setItem(CurrentRow, 2, Item);

    Item = new QTableWidgetItem(QDir::toNativeSeparators(QString::fromStdString((*it)->getRelativePath())));
    ui->DatastoreTableWidget->setItem(CurrentRow, 3, Item);

    CurrentRow++;
  }

  ui->DatastoreTableWidget->resizeRowsToContents();
}


// =====================================================================
// =====================================================================


void DatastoreWidget::addItem()
{
  AddDatastoreItemDialog AddItemDlg(openfluid::utils::toQStringList(m_Datastore.getItemsIDs()),this);

  if (AddItemDlg.exec() == QDialog::Accepted)
  {

    bool OKToCreateItem = true;

    // by default, the file associated to the item is in the inputdir path
    QString DSItemFile = AddItemDlg.getSourceFilePath();

    // copy of source file(s) if file is not in the inputdir path
    if (!AddItemDlg.isSourceFileInInputDataset())
    {
      // Prepare copy in dataset

      QString Subdir = QDir::fromNativeSeparators(AddItemDlg.getCopySubdir());
      QString SourceFilename = QDir::fromNativeSeparators(QFileInfo(AddItemDlg.getSourceFilePath()).fileName());

      QString DestFile =
          QDir::fromNativeSeparators(QString::fromStdString(
              openfluid::base::RunContextManager::instance()->getInputDir()) + "/"+Subdir+"/"+SourceFilename);

      if (AddItemDlg.getItemType() == openfluid::core::UnstructuredValue::UnstructuredType::VECTOR)
      {
        // copy vector file
        GDALDataset_COMPAT* SrcDS =
          GDALOpenRO_COMPAT(QDir::fromNativeSeparators(AddItemDlg.getSourceFilePath()).toStdString().c_str());

        if (SrcDS != nullptr)
        {
          GDALDriver_COMPAT* CopyDriver = SrcDS->GetDriver();

          if (CopyDriver != nullptr)
          {
            // creation of the destination directory if not exists
            QDir().mkpath(QFileInfo(DestFile).absolutePath());

            // temporary datasource to see if the file already exists
            GDALDataset_COMPAT* DestAlreadyExists = GDALOpenRO_COMPAT(DestFile.toStdString().c_str());

            if (DestAlreadyExists)
            {
              GDALClose_COMPAT(DestAlreadyExists);

              OKToCreateItem =
                  (QMessageBox::question(QApplication::activeWindow(),
                                         "OpenFLUID-Builder",
                                         tr("The file %1 is about to be copied in the input dataset,\n"
                                             "as the source file for the added datastore item.\n"
                                             "\n"
                                             "A file with the same name already exists in the input dataset.\n"
                                             "It will be replaced by the new file.\n"
                                             "\n"
                                             "Proceed anyway?").arg(SourceFilename),
                                             QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
              if (OKToCreateItem)
              {
                // if file already exists, it is deleted
                GDALDelete_COMPAT(CopyDriver,DestFile.toStdString().c_str());
              }
            }

            if (OKToCreateItem)
            {
              GDALDataset_COMPAT* DestDS = GDALCopy_COMPAT(CopyDriver,SrcDS,DestFile.toStdString().c_str());
              GDALClose_COMPAT(DestDS);
            }

            GDALClose_COMPAT(SrcDS);
          }
          else
          {
            QMessageBox::critical(QApplication::activeWindow(),
                                  "OpenFLUID-Builder",
                                  tr("OGR driver error while copying geovector file source\n"
                                     "The datastore item will not be added"),
                                  QMessageBox::Close);
            return;
          }
        }
        else
        {
          QMessageBox::critical(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                tr("OGR error while copying geovector file source\n"
                                   "The datastore item will not be added"),
                                QMessageBox::Close);
          return;
        }
      }
      else
      {
        // copy raster file

        GDALDataset* SrcDS = (GDALDataset *)GDALOpen(
            QDir::fromNativeSeparators(AddItemDlg.getSourceFilePath()).toStdString().c_str(),
            GA_ReadOnly);

        if (SrcDS != nullptr)
        {
          GDALDriver *CopyDriver = SrcDS->GetDriver();

          if (CopyDriver != nullptr)
          {
            QDir().mkpath(QFileInfo(DestFile).absolutePath());

            if (CopyDriver->CopyFiles(
                    DestFile.toStdString().c_str(),
                    QDir::fromNativeSeparators(AddItemDlg.getSourceFilePath()).toStdString().c_str()) == CE_Failure)
            {
              QMessageBox::critical(QApplication::activeWindow(),
                                    "OpenFLUID-Builder",
                                    tr("GDAL error while copying georaster file source\n"
                                       "The datastore item will not be added"),
                                    QMessageBox::Close);
              return;
            }

          }
          else
          {
            QMessageBox::critical(QApplication::activeWindow(),
                                  "OpenFLUID-Builder",
                                  tr("GDAL driver error while opening georaster file source to copy\n"
                                     "The datastore item will not be added"),
                                  QMessageBox::Close);
            return;
          }
        }
        else
        {
          QMessageBox::critical(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                tr("GDAL error while opening georaster file source to copy\n"
                                   "The datastore item will not be added"),
                                QMessageBox::Close);
          return;
        }
        GDALClose(SrcDS);
      }

      // adjust the file associated to the item with the copied file
      DSItemFile = DestFile;
    }


    if (OKToCreateItem)
    {
      // creation of datastore item

      // build of the relative path for the file associated with the item
      QString RelativeDSItemFile =
          QDir(QString::fromStdString(openfluid::base::RunContextManager::instance()->getInputDir()))
          .relativeFilePath(DSItemFile);

      openfluid::fluidx::DatastoreItemDescriptor* DSItemDesc =
          new openfluid::fluidx::DatastoreItemDescriptor(AddItemDlg.getItemID().toStdString(),
                                                         openfluid::base::RunContextManager::instance()->getInputDir(),
                                                         QDir::fromNativeSeparators(RelativeDSItemFile).toStdString(),
                                                         AddItemDlg.getItemType());

      if (AddItemDlg.isUnitsClass())
      {
        DSItemDesc->setUnitsClass(AddItemDlg.getUnitsClass().toStdString());
      }

      m_Datastore.appendItem(DSItemDesc);

      emit changed(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_DATASTORE);

      refresh();
    }
  }
}


// =====================================================================
// =====================================================================


void DatastoreWidget::editItem()
{
  // TODO
  QMessageBox::critical(QApplication::activeWindow(),QString("not implemented"),
                        QString(__PRETTY_FUNCTION__),QMessageBox::Close);
}


// =====================================================================
// =====================================================================


void DatastoreWidget::removeItem()
{
  bool OK = true;

  int DSItemIndex = ui->DatastoreTableWidget->currentRow();

  if (DSItemIndex >=0)
  {
    QString DSItemID = ui->DatastoreTableWidget->item(DSItemIndex,0)->text();

    OK = (QMessageBox::question(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                tr("You are removing %1 item from datastore.\n"
                                   "This item will not be available anymore, but data will be kept on disk.\n"
                                   "\n"
                                   "Proceed anyway?").arg(DSItemID),
                                QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);

    if (OK)
    {
      m_Datastore.removeItem(DSItemIndex);

      emit changed(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_DATASTORE);

      refresh();
    }
  }
}


// =====================================================================
// =====================================================================


void DatastoreWidget::moveItemUp()
{
  if (ui->DatastoreTableWidget->rowCount() < 2)
  {
    return;
  }

  int DSItemIndex = ui->DatastoreTableWidget->currentRow();

  if (DSItemIndex >=1)
  {
    int From = DSItemIndex;
    int To = DSItemIndex-1;

    m_Datastore.moveItem(From,To);

    emit changed(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_DATASTORE);

    refresh();

    ui->DatastoreTableWidget->selectRow(To);
  }
}


// =====================================================================
// =====================================================================


void DatastoreWidget::moveItemDown()
{
  int RowCount = ui->DatastoreTableWidget->rowCount();

  if (RowCount < 2)
  {
    return;
  }

  int DSItemIndex = ui->DatastoreTableWidget->currentRow();

  if (DSItemIndex>=0 && DSItemIndex<RowCount-1)
  {
    int From = DSItemIndex;
    int To = DSItemIndex+1;

    m_Datastore.moveItem(From,To);

    emit changed(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_DATASTORE);

    refresh();

    ui->DatastoreTableWidget->selectRow(To);
  }
}

