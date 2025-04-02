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
  @file MultiCSVObsParamsWidget.cpp

  @author Armel THONI <armel.thoni@inra.fr>
*/


#include <QMessageBox>

#include <openfluid/ui/QtHelpers.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "MultiEditSetDialog.hpp"
#include "MultiCSVObsParamsWidget.hpp"
#include "ui_MultiCSVObsParamsWidget.h"
#include "MultiEditFormatDialog.hpp"


MultiCSVObsParamsWidget::MultiCSVObsParamsWidget(): openfluid::ui::builderext::PluggableParameterizationExtension(),
  ui(new Ui::MultiCSVObsParamsWidget)
{
  ui->setupUi(this);
  setAutoFillBackground(true);

  ui->AddFormatButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddFormatButton->setIconSize(QSize(16,16));

  ui->EditFormatButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->EditFormatButton->setIconSize(QSize(16,16));

  ui->RemoveFormatButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveFormatButton->setIconSize(QSize(16,16));

  ui->AddSetButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddSetButton->setIconSize(QSize(16,16));

  ui->EditSetButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->EditSetButton->setIconSize(QSize(16,16));

  ui->RemoveSetButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveSetButton->setIconSize(QSize(16,16));

  ui->AutoButton->setIcon(openfluid::ui::common::getIcon("magic","/ui/common"));
  ui->AutoButton->setIconSize(QSize(16,16));

  connect(ui->AddFormatButton,SIGNAL(clicked()),this,SLOT(addFormat()));
  connect(ui->EditFormatButton,SIGNAL(clicked()),this,SLOT(editFormat()));
  connect(ui->FormatsTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editFormat()));
  connect(ui->RemoveFormatButton,SIGNAL(clicked()),this,SLOT(removeFormat()));
  connect(ui->AddSetButton,SIGNAL(clicked()),this,SLOT(addSet()));
  connect(ui->EditSetButton,SIGNAL(clicked()),this,SLOT(editSet()));
  connect(ui->SetsTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(editSet()));
  connect(ui->RemoveSetButton,SIGNAL(clicked()),this,SLOT(removeSet()));
  connect(ui->AutoButton,SIGNAL(clicked()),this,SLOT(generateAutomaticFormatAndSets()));
}


// =====================================================================
// =====================================================================


MultiCSVObsParamsWidget::~MultiCSVObsParamsWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::clearInternalDataAndWidgets()
{
  m_Formats.clear();

  while (ui->FormatsTableWidget->rowCount() > 0)
  {
    ui->FormatsTableWidget->removeRow(0);
  }

  ui->FormatsTableWidget->setRowCount(0);

  m_Sets.clear();

  while (ui->SetsTableWidget->rowCount() > 0)
  {
    ui->SetsTableWidget->removeRow(0);
  }

  ui->SetsTableWidget->setRowCount(0);
}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::removeParamsStartingWith(const QString& Str)
{
  for (auto it = mp_Params->begin();it!=mp_Params->end();)
  {
    QString ParamName = QString::fromStdString((*it).first);

    if (ParamName.startsWith(Str))
    {
      mp_Params->erase(it++);
    }
    else
    {
      ++it;
    }
  }
}


// =====================================================================
// =====================================================================


QString MultiCSVObsParamsWidget::getParamValue(const QString& Str, const QString& Default)
{
  QString Value = Default;

  auto it = mp_Params->find(Str.toStdString());
  if (it != mp_Params->end())
  {
    Value = QString::fromStdString((*it).second.toString());
  }

  return Value;
}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::update()
{
  openfluid::ware::WareParamsTree ParamsTree;
  int RowCount;
  QBrush WarnBrush(QColor("#FFC65E"));

  try
  {
    clearInternalDataAndWidgets();

    ParamsTree.setParams(*mp_Params);

    // Formats
    parseMultiColsFormatsFromParamsTree(ParamsTree,m_Formats);

    RowCount = 0;
    for (auto& Format : m_Formats)
    {
      RowCount++;
      ui->FormatsTableWidget->setRowCount(RowCount);
      
      int ColCount = 0;
      for (std::string& Field : Format.second.generateFormatFields(Format.first))
      {
        QTableWidgetItem *TableItem = new QTableWidgetItem(QString::fromStdString(Field));
        ui->FormatsTableWidget->setItem(RowCount-1, ColCount, TableItem);
        ColCount++;
      }
    }
    if (RowCount > 0)
    {
      ui->EditFormatButton->setEnabled(true);
      ui->RemoveFormatButton->setEnabled(true);
    }
    else
    {
      ui->EditFormatButton->setEnabled(false);
      ui->RemoveFormatButton->setEnabled(false);
    }

    // Sets
    parseMultiColsSetsFromParamsTree(ParamsTree,m_Sets);

    RowCount = 0;
    for (auto& Set : m_Sets)
    {
      RowCount++;
      ui->SetsTableWidget->setRowCount(RowCount);

      QTableWidgetItem *TableItem = new QTableWidgetItem(QString::fromStdString(Set.first));
      ui->SetsTableWidget->setItem(RowCount-1, 0, TableItem);

      TableItem = new QTableWidgetItem(QString::fromStdString(Set.second.FormatName));
      ui->SetsTableWidget->setItem(RowCount-1, 1, TableItem);
      if (Set.second.FormatName.empty() || m_Formats.find(Set.second.FormatName) == m_Formats.end())
      {
        TableItem->setBackground(WarnBrush);
      }

      TableItem = new QTableWidgetItem(QString::fromStdString(Set.second.Selection));
      ui->SetsTableWidget->setItem(RowCount-1, 2, TableItem);
      if (Set.second.Selection.empty())
      {
        TableItem->setBackground(WarnBrush);
      }
    }
    if (RowCount > 0)
    {
      ui->EditSetButton->setEnabled(true);
      ui->RemoveSetButton->setEnabled(true);
    }
    else
    {
      ui->EditSetButton->setEnabled(false);
      ui->RemoveSetButton->setEnabled(false);
    }

  }
  catch (openfluid::base::FrameworkException& E)
  {
    // ...
  }
  catch (openfluid::base::Exception& E)
  {
    // ...
  }

}


// =====================================================================
// =====================================================================


QStringList MultiCSVObsParamsWidget::getFormatNames()
{
  QStringList FormatNames;

  for (auto& Format : m_Formats)
  {
    FormatNames << QString::fromStdString(Format.first);
  }
  return FormatNames;
}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::addFormat()
{
  QStringList FormatNames = getFormatNames();

  MultiEditFormatDialog AddDlg(FormatNames,this);

  if (AddDlg.exec() == QDialog::Accepted)
  {
    openfluid::ware::WareParams_t Params = AddDlg.getFormatParams();

    mp_Params->insert(Params.begin(),Params.end());

    update();
    emit changed();
  }

}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::editFormat()
{
  int CurrentRow = ui->FormatsTableWidget->currentRow();

  if (CurrentRow >= 0)
  {
    QString EditedFormat = ui->FormatsTableWidget->item(CurrentRow,0)->text();

    QStringList FormatNames;

    for (auto& Format : m_Formats)
    {
      QString CurrentFormat = QString::fromStdString(Format.first);
      if (CurrentFormat != EditedFormat)
      {
        FormatNames << CurrentFormat;
      }
    }

    MultiEditFormatDialog EditDlg(FormatNames,this);

    QString FormatStr = "format."+EditedFormat+".";


    EditDlg.initialize(EditedFormat,
                       getParamValue(FormatStr+"header","colnames-as-comment"),
                       getParamValue(FormatStr+"colsep",";"),
                       getParamValue(FormatStr+"date","ISO"),
                       getParamValue(FormatStr+"precision","5"),
                       getParamValue(FormatStr+"float-format","Auto"),
                       getParamValue(FormatStr+"commentchar","#"),
                       getParamValue(FormatStr+"missingvalue","NA"));


    if (EditDlg.exec() == QDialog::Accepted)
    {
      removeParamsStartingWith(FormatStr);

      openfluid::ware::WareParams_t Params = EditDlg.getFormatParams();

      mp_Params->insert(Params.begin(),Params.end());

      update();
      emit changed();
    }
  }
}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::removeFormat()
{
  int CurrentRow = ui->FormatsTableWidget->currentRow();

  if (CurrentRow >= 0)
  {
    QString FormatStr = "format."+ui->FormatsTableWidget->item(CurrentRow,0)->text()+".";

    removeParamsStartingWith(FormatStr);

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::addSet()
{
  QStringList SetNames;
  for (auto& Set : m_Sets)
  {
    SetNames << QString::fromStdString(Set.first);
  }

  QStringList FormatNames = getFormatNames();
  QStringList ClassNames = openfluid::ui::toQStringList(mp_Desc->spatialDomain().getClassNames());
  MultiEditSetDialog AddDlg(SetNames,FormatNames,ClassNames,this);

  if (AddDlg.exec() == QDialog::Accepted)
  {
    openfluid::ware::WareParams_t Params = AddDlg.getMultiSetParams();

    mp_Params->insert(Params.begin(),Params.end());

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::editSet()
{
  int CurrentRow = ui->SetsTableWidget->currentRow();

  if (CurrentRow >= 0)
  {
    QString EditedSet = ui->SetsTableWidget->item(CurrentRow,0)->text();

    QStringList SetNames;
    for (auto& Set : m_Sets)
    {
      QString CurrentSet = QString::fromStdString(Set.first);
      if (CurrentSet != EditedSet)
      {
        SetNames << CurrentSet; 
      }
    }

    QStringList FormatNames = getFormatNames();
    QStringList ClassNames = openfluid::ui::toQStringList(mp_Desc->spatialDomain().getClassNames());
    MultiEditSetDialog EditDlg(SetNames,FormatNames,ClassNames,this);

    QString SetStr = "set."+EditedSet+".";


    EditDlg.initialize(EditedSet,
                       getParamValue(SetStr+"format",""),
                       getParamValue(SetStr+"selection",""));


    if (EditDlg.exec() == QDialog::Accepted)
    {
      removeParamsStartingWith(SetStr);

      openfluid::ware::WareParams_t Params = EditDlg.getMultiSetParams();

      mp_Params->insert(Params.begin(),Params.end());

      update();
      emit changed();
    }
  }
}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::removeSet()
{
  int CurrentRow = ui->SetsTableWidget->currentRow();

  if (CurrentRow >= 0)
  {
    QString SetStr = "set."+ui->SetsTableWidget->item(CurrentRow,0)->text()+".";

    removeParamsStartingWith(SetStr);

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void MultiCSVObsParamsWidget::generateAutomaticFormatAndSets()
{
  if (QMessageBox::warning(this,tr("Automatic generation"),
                           tr("A format and one or more sets configurations will be automatically generated.\n"
                               "This will remove existing formats and sets.\n\n"
                               "Proceed anyway?"),QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok)
  {
    clearInternalDataAndWidgets();
    mp_Params->clear();

    mp_Params->insert({"format.auto.colsep",";"});
    mp_Params->insert({"format.auto.date","ISO"});
    mp_Params->insert({"format.auto.header","colnames-as-comment"});


    std::string SetName = "All";
    mp_Params->insert({"set.auto"+SetName+".format","auto"});
    mp_Params->insert({"set.auto"+SetName+".selection","*#*:*"});

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(MultiCSVObsParamsWidget)
