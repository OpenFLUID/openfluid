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
  @file CSVObsParamsWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QMessageBox>

#include <observers/export.vars.files.csv/EditSetDialog.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "CSVObsParamsWidget.hpp"
#include "ui_CSVObsParamsWidget.h"
#include "EditFormatDialog.hpp"


BEGIN_BUILDEREXT_SIGNATURE(WARE_ID)

END_BUILDEREXT_SIGNATURE



CSVObsParamsWidget::CSVObsParamsWidget(): openfluid::builderext::PluggableParameterizationExtension(),
  ui(new Ui::CSVObsParamsWidget)
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


CSVObsParamsWidget::~CSVObsParamsWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void CSVObsParamsWidget::clearInternalDataAndWidgets()
{
  m_Formats.clear();

  while (ui->FormatsTableWidget->rowCount() > 0)
    ui->FormatsTableWidget->removeRow(0);

  ui->FormatsTableWidget->setRowCount(0);

  m_Sets.clear();

  while (ui->SetsTableWidget->rowCount() > 0)
        ui->SetsTableWidget->removeRow(0);

  ui->SetsTableWidget->setRowCount(0);
}


// =====================================================================
// =====================================================================


void CSVObsParamsWidget::removeParamsStartingWith(const QString& Str)
{
  for (auto it = mp_Params->begin();it!=mp_Params->end();)
  {
    QString ParamName = QString::fromStdString((*it).first);

    if (ParamName.startsWith(Str))
      mp_Params->erase(it++);
    else
      ++it;
  }
}


// =====================================================================
// =====================================================================


QString CSVObsParamsWidget::getParamValue(const QString& Str, const QString Default)
{
  QString Value = Default;

  auto it = mp_Params->find(Str.toStdString());
  if (it != mp_Params->end())
    Value = QString::fromStdString((*it).second.toString());

  return Value;
}


// =====================================================================
// =====================================================================


void CSVObsParamsWidget::update()
{
  openfluid::ware::WareParamsTree ParamsTree;
  int RowCount;
  QBrush WarnBrush(QColor("#FFC65E"));

  try
  {
    clearInternalDataAndWidgets();

    ParamsTree.setParams(*mp_Params);


    // Formats

    parseFormatsFromParamsTree(ParamsTree,m_Formats);

    RowCount = 0;
    for (auto& Format : m_Formats)
    {
      RowCount++;
      ui->FormatsTableWidget->setRowCount(RowCount);

      QTableWidgetItem *TableItem = new QTableWidgetItem(QString::fromStdString(Format.first));
      ui->FormatsTableWidget->setItem(RowCount-1, 0, TableItem);

      TableItem = new QTableWidgetItem(QString::fromStdString(Format.second.ColSeparator).replace("\t","\\t"));
      ui->FormatsTableWidget->setItem(RowCount-1, 1, TableItem);

      TableItem = new QTableWidgetItem(QString::fromStdString(Format.second.DateFormat));
      ui->FormatsTableWidget->setItem(RowCount-1, 2, TableItem);

      TableItem = new QTableWidgetItem(QString("%1").arg(Format.second.Precision));
      ui->FormatsTableWidget->setItem(RowCount-1, 3, TableItem);

      TableItem = new QTableWidgetItem(QString::fromStdString(HeaderTypeToStr(Format.second.Header)));
      ui->FormatsTableWidget->setItem(RowCount-1, 4, TableItem);

      TableItem = new QTableWidgetItem(QString::fromStdString(Format.second.CommentChar));
      ui->FormatsTableWidget->setItem(RowCount-1, 5, TableItem);
    }


    // Sets

    parseSetsFromParamsTree(ParamsTree,m_Sets);

    RowCount = 0;
    for (auto& Set : m_Sets)
    {
      RowCount++;
      ui->SetsTableWidget->setRowCount(RowCount);

      QTableWidgetItem *TableItem = new QTableWidgetItem(QString::fromStdString(Set.first));
      ui->SetsTableWidget->setItem(RowCount-1, 0, TableItem);

      TableItem = new QTableWidgetItem(QString::fromStdString(Set.second.UnitsClass));
      ui->SetsTableWidget->setItem(RowCount-1, 1, TableItem);
      if (Set.second.UnitsClass.empty())
        TableItem->setBackground(WarnBrush);

      TableItem = new QTableWidgetItem(QString::fromStdString(Set.second.UnitsIDsStr));
      ui->SetsTableWidget->setItem(RowCount-1, 2, TableItem);
      if (Set.second.UnitsIDsStr.empty())
        TableItem->setBackground(WarnBrush);

      TableItem = new QTableWidgetItem(QString::fromStdString(Set.second.VariablesStr));
      ui->SetsTableWidget->setItem(RowCount-1, 3, TableItem);
      if (Set.second.VariablesStr.empty())
        TableItem->setBackground(WarnBrush);

      TableItem = new QTableWidgetItem(QString::fromStdString(Set.second.FormatName));
      ui->SetsTableWidget->setItem(RowCount-1, 4, TableItem);
      if (Set.second.FormatName.empty() || m_Formats.find(Set.second.FormatName) == m_Formats.end())
        TableItem->setBackground(WarnBrush);
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


void CSVObsParamsWidget::addFormat()
{
  QStringList FormatNames;

  for (auto& Format : m_Formats)
    FormatNames << QString::fromStdString(Format.first);

  EditFormatDialog AddDlg(FormatNames,this);

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


void CSVObsParamsWidget::editFormat()
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
        FormatNames << CurrentFormat;
    }

    EditFormatDialog EditDlg(FormatNames,this);

    QString FormatStr = "format."+EditedFormat+".";


    EditDlg.initialize(EditedFormat,
                       getParamValue(FormatStr+"header","colnames-as-comment"),
                       getParamValue(FormatStr+"colsep",";"),
                       getParamValue(FormatStr+"date","ISO"),
                       getParamValue(FormatStr+"precision","5"),
                       getParamValue(FormatStr+"commentchar","#"));


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


void CSVObsParamsWidget::removeFormat()
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


void CSVObsParamsWidget::addSet()
{
  QStringList SetNames;
  for (auto& Set : m_Sets)
    SetNames << QString::fromStdString(Set.first);

  QStringList FormatNames;
  for (auto& Format : m_Formats)
    FormatNames << QString::fromStdString(Format.first);

  QStringList ClassNames = openfluid::tools::toQStringList(mp_AdvancedDesc->spatialDomain().getClassNames());

  EditSetDialog AddDlg(SetNames,FormatNames,ClassNames,this);

  if (AddDlg.exec() == QDialog::Accepted)
  {
    openfluid::ware::WareParams_t Params = AddDlg.getSetParams();

    mp_Params->insert(Params.begin(),Params.end());

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void CSVObsParamsWidget::editSet()
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
        SetNames << CurrentSet;
    }

    QStringList FormatNames;
    for (auto& Format : m_Formats)
      FormatNames << QString::fromStdString(Format.first);

    QStringList ClassNames = openfluid::tools::toQStringList(mp_AdvancedDesc->spatialDomain().getClassNames());

    EditSetDialog EditDlg(SetNames,FormatNames,ClassNames,this);

    QString SetStr = "set."+EditedSet+".";


    EditDlg.initialize(EditedSet,
                       getParamValue(SetStr+"format",""),
                       getParamValue(SetStr+"unitsclass",""),
                       getParamValue(SetStr+"unitsIDs","*"),
                       getParamValue(SetStr+"vars","*"));


    if (EditDlg.exec() == QDialog::Accepted)
    {
      removeParamsStartingWith(SetStr);

      openfluid::ware::WareParams_t Params = EditDlg.getSetParams();

      mp_Params->insert(Params.begin(),Params.end());

      update();
      emit changed();
    }
  }
}


// =====================================================================
// =====================================================================


void CSVObsParamsWidget::removeSet()
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


void CSVObsParamsWidget::generateAutomaticFormatAndSets()
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

    std::set<std::string> ClassNames = mp_AdvancedDesc->spatialDomain().getClassNames();

    for (auto& CName : ClassNames)
    {
      mp_Params->insert({"set.auto"+CName+".format","auto"});
      mp_Params->insert({"set.auto"+CName+".unitsclass",CName});
      mp_Params->insert({"set.auto"+CName+".unitsIDs","*"});
      mp_Params->insert({"set.auto"+CName+".vars","*"});
    }

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(CSVObsParamsWidget)

DEFINE_WARE_LINKUID(WARE_LINKUID)
