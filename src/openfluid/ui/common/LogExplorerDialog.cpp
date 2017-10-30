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
  @file LogExplorerDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QFile>
#include <QFileDialog>

#include <openfluid/ui/common/LogExplorerDialog.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/config.hpp>

#include "ui_LogExplorerDialog.h"


namespace openfluid { namespace ui { namespace common {


LogExplorerDialog::LogExplorerDialog(QString LogDir, QWidget* Parent):
  QDialog(Parent), ui(new Ui::LogExplorerDialog), m_CurrentDir(LogDir), m_CurrentCount(0)
{
  ui->setupUi(this);

  ui->HorizontalSplitter->setStretchFactor(1,3);
  ui->VerticalSplitter->setStretchFactor(2,2);

  connect(ui->ChangeDirButton,SIGNAL(clicked()),this,SLOT(changeCurrentDirectory()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  connect(ui->LogTableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(updateDetails(int)));

  connect(ui->AppFilterCheckBox,SIGNAL(stateChanged(int)),this,SLOT(enableFilterApply()));
  connect(ui->FrameworkFilterCheckBox,SIGNAL(stateChanged(int)),this,SLOT(enableFilterApply()));
  connect(ui->TypeFilterListWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(enableFilterApply()));
  connect(ui->SimulatorFilterListWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(enableFilterApply()));
  connect(ui->ObserverFilterListWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(enableFilterApply()));

  connect(ui->ApplyFilteringButton,SIGNAL(clicked()),this,SLOT(applyFilters()));

  reloadFromFiles();
}


// =====================================================================
// =====================================================================


LogExplorerDialog::~LogExplorerDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void LogExplorerDialog::changeCurrentDirectory()
{
  QString SelectedDir = QFileDialog::getExistingDirectory(this,tr("Select directory"));

  if (SelectedDir !=  "")
  {
    m_CurrentDir = SelectedDir;
    reloadFromFiles();
  }
}


// =====================================================================
// =====================================================================


void LogExplorerDialog::setDirectoryVisible(bool Visible)
{
  ui->DirectoryWidget->setVisible(Visible);
}


// =====================================================================
// =====================================================================


void LogExplorerDialog::reloadFromFiles()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  m_LogInfos.clear();
  m_CurrentCount = 0;

  ui->DirectoryLabel->setText(QDir::toNativeSeparators(m_CurrentDir));


  QFile MsgFile(m_CurrentDir+"/"+QString::fromStdString(openfluid::config::MESSAGES_LOG_FILE));
  if (MsgFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    while (!MsgFile.atEnd())
    {
      QByteArray LineBytes = MsgFile.readLine();
      appendLogItem(processLogLine(QString(LineBytes)));
    }

    MsgFile.close();
  }


  rebuildFilters();

  updateTable();

  QApplication::restoreOverrideCursor();
}


// =====================================================================
// =====================================================================


LogExplorerDialog::LogItemInfos LogExplorerDialog::processLogLine(const QString& Line)
{
  LogItemInfos Item;

  QStringList SplittedLine = Line.split(']');

  if (SplittedLine.size() > 2)
  {
    m_CurrentCount++;

    Item["order"] = QString("%1").arg(m_CurrentCount);

    // Type
    QString TypeStr = SplittedLine[0];
    TypeStr.remove(0,TypeStr.indexOf('[')+1);
    Item["type"] = TypeStr;

    SplittedLine.removeFirst();

    // Context
    QString ContextStr = SplittedLine[0];
    ContextStr.remove(0,ContextStr.indexOf('[')+1);
    QStringList SplittedContext = ContextStr.split(',');

    for (const QString& KeyValStr : SplittedContext)
    {
      QStringList SplittedKeyVal = KeyValStr.split('=');
      if (SplittedKeyVal.size() == 2)
        Item["context."+SplittedKeyVal[0]] = SplittedKeyVal[1];
    }

    SplittedLine.removeFirst();

    // Message
    Item["msg"] = SplittedLine.join("]");
  }

  return Item;
}


// =====================================================================
// =====================================================================


void LogExplorerDialog::appendLogItem(const LogItemInfos& Item)
{
  m_LogInfos.append(Item);
}


// =====================================================================
// =====================================================================


void LogExplorerDialog::updateTable(bool WithFiltering)
{

  while (ui->LogTableWidget->rowCount() > 0)
    ui->LogTableWidget->removeRow(0);

  ui->LogTableWidget->setRowCount(0);
  updateDetails(-1);



  QStringList FilteredTypes;
  QStringList FilteredSimulators;
  QStringList FilteredObservers;

  if (WithFiltering)
  {
    for (int i=0;i<ui->TypeFilterListWidget->count();i++)
      if (ui->TypeFilterListWidget->item(i)->checkState() == Qt::Checked)
        FilteredTypes.append(ui->TypeFilterListWidget->item(i)->text());

    for (int i=0;i<ui->SimulatorFilterListWidget->count();i++)
      if (ui->SimulatorFilterListWidget->item(i)->checkState() == Qt::Checked)
        FilteredSimulators.append(ui->SimulatorFilterListWidget->item(i)->text());

    for (int i=0;i<ui->ObserverFilterListWidget->count();i++)
      if (ui->ObserverFilterListWidget->item(i)->checkState() == Qt::Checked)
        FilteredObservers.append(ui->ObserverFilterListWidget->item(i)->text());
  }


  int RowCount = 0;

  for (int i=0; i< m_LogInfos.count(); i++)
  {
    LogItemInfos Item = m_LogInfos[i];

    QString TypeStr = Item["type"];
    QString ContextSrcStr = Item["context.source"];

    if (!(TypeStr.isEmpty() || ContextSrcStr.isEmpty()))
    {

      bool OKToDisplay = true;

      if (WithFiltering)
      {
        if (!FilteredTypes.contains(TypeStr))
          OKToDisplay = false;
        else if (Item.contains("context.source"))
        {
          if (ContextSrcStr == "app" && ui->AppFilterCheckBox->checkState() == Qt::Unchecked)
            OKToDisplay = false;
          else if (ContextSrcStr == "framework" && ui->FrameworkFilterCheckBox->checkState() == Qt::Unchecked)
            OKToDisplay = false;
          else if (ContextSrcStr == "ware")
          {
            QString WareTypeStr = Item["context.waretype"];
            QString WareIDStr = Item["context.wareid"];

            if (WareTypeStr == "simulator" && (FilteredSimulators.empty() || !FilteredSimulators.contains(WareIDStr)))
              OKToDisplay = false;
            else if (WareTypeStr == "observer" && (FilteredObservers.empty() || !FilteredObservers.contains(WareIDStr)))
              OKToDisplay = false;
          }
        }
      }


      if (OKToDisplay)
      {
        RowCount++;
        ui->LogTableWidget->setRowCount(RowCount);

        QTableWidgetItem *TableItem = new QTableWidgetItem(TypeStr);
        TableItem->setTextAlignment(Qt::AlignCenter);
        TableItem->setData(Qt::UserRole,i);

        if (TypeStr.toLower() == "warning")
          TableItem->setBackground(QBrush(QColor("#FFCF5A")));
        else if (TypeStr.toLower() == "info")
          TableItem->setBackground(QBrush(QColor("#6CC2FF")));
        else if (TypeStr.toLower() == "debug")
          TableItem->setBackground(QBrush(QColor("#D0D0D0")));
        else if (TypeStr.toLower() == "error")
          TableItem->setBackground(QBrush(QColor("#FF625A")));

        ui->LogTableWidget->setItem(RowCount-1, 0, TableItem);


        TableItem = new QTableWidgetItem(Item["order"]);
        TableItem->setTextAlignment(Qt::AlignCenter);
        ui->LogTableWidget->setItem(RowCount-1, 1, TableItem);

        if (ContextSrcStr == "ware" && Item.contains("context.waretype") && Item.contains("context.wareid"))
        {
          TableItem = new QTableWidgetItem(Item["context.waretype"]+": "+Item["context.wareid"]);
          TableItem->setTextAlignment(Qt::AlignCenter);
          ui->LogTableWidget->setItem(RowCount-1, 2, TableItem);
        }
        else if (ContextSrcStr == "framework")
        {
          TableItem = new QTableWidgetItem("framework");
          TableItem->setTextAlignment(Qt::AlignCenter);
          ui->LogTableWidget->setItem(RowCount-1, 2, TableItem);
        }
        else if (ContextSrcStr == "app")
        {
          TableItem = new QTableWidgetItem("application");
          TableItem->setTextAlignment(Qt::AlignCenter);
          ui->LogTableWidget->setItem(RowCount-1, 2, TableItem);
        }


        if (Item.contains("context.stage"))
        {
          TableItem = new QTableWidgetItem(Item["context.stage"]);
          TableItem->setTextAlignment(Qt::AlignCenter);
          ui->LogTableWidget->setItem(RowCount-1, 3, TableItem);
        }

        if (Item.contains("context.timeindex"))
        {
          TableItem = new QTableWidgetItem(Item["context.timeindex"]);
          TableItem->setTextAlignment(Qt::AlignCenter);
          ui->LogTableWidget->setItem(RowCount-1, 4, TableItem);
        }

        TableItem = new QTableWidgetItem(Item["msg"]);
        ui->LogTableWidget->setItem(RowCount-1, 5, TableItem);
      }
    }
  }

  ui->LogTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  if (ui->LogTableWidget->rowCount())
    ui->MessagesCountLabel->setText(tr("%1 message(s)").arg(ui->LogTableWidget->rowCount()));
  else
    ui->MessagesCountLabel->setText(tr("no message"));

}


// =====================================================================
// =====================================================================


void LogExplorerDialog::rebuildFilters()
{
  QStringList Types;
  QStringList Simulators;
  QStringList Observers;


  ui->TypeFilterListWidget->clear();
  ui->SimulatorFilterListWidget->clear();
  ui->ObserverFilterListWidget->clear();


  for (int i=0; i< m_LogInfos.count(); i++)
  {
    LogItemInfos Item = m_LogInfos[i];

    QString TypeStr = m_LogInfos[i]["type"];
    if (!(TypeStr.isEmpty() || Types.contains(TypeStr)))
      Types.append(TypeStr);

    if (m_LogInfos[i].contains("context.source") && m_LogInfos[i]["context.source"] == "ware" &&
        m_LogInfos[i].contains("context.waretype") && m_LogInfos[i].contains("context.wareid"))
    {
      QString WareTypeStr = m_LogInfos[i]["context.waretype"];
      QString WareIDStr = m_LogInfos[i]["context.wareid"];

      if (WareTypeStr.toLower() == "simulator" && !Simulators.contains(WareIDStr))
        Simulators.append(WareIDStr);
      else if (WareTypeStr.toLower() == "observer" && !Observers.contains(WareIDStr))
        Observers.append(WareIDStr);
    }
  }

  Types.sort();
  Simulators.sort();
  Observers.sort();

  for (int i=0; i< Types.count(); i++)
  {
    QListWidgetItem* ListItem = new QListWidgetItem(Types[i]);
    ListItem->setFlags(ListItem->flags() | Qt::ItemIsUserCheckable);
    ListItem->setCheckState(Qt::Checked);
    ui->TypeFilterListWidget->addItem(ListItem);
  }

  for (int i=0; i< Simulators.count(); i++)
  {
    QListWidgetItem* ListItem = new QListWidgetItem(Simulators[i]);
    ListItem->setFlags(ListItem->flags() | Qt::ItemIsUserCheckable);
    ListItem->setCheckState(Qt::Checked);
    ui->SimulatorFilterListWidget->addItem(ListItem);
  }

  for (int i=0; i< Observers.count(); i++)
  {
    QListWidgetItem* ListItem = new QListWidgetItem(Observers[i]);
    ListItem->setFlags(ListItem->flags() | Qt::ItemIsUserCheckable);
    ListItem->setCheckState(Qt::Checked);
    ui->ObserverFilterListWidget->addItem(ListItem);
  }

  ui->FrameworkFilterCheckBox->setCheckState(Qt::Checked);
  ui->AppFilterCheckBox->setCheckState(Qt::Checked);

  ui->ApplyFilteringButton->setEnabled(false);
}


// =====================================================================
// =====================================================================


void LogExplorerDialog::updateDetails(int Row)
{
  if (Row >=0)
  {
    LogItemInfos Infos = m_LogInfos[ui->LogTableWidget->item(Row,0)->data(Qt::UserRole).toInt()];
    ui->MessageDetailLabel->setText(Infos["msg"]);
    ui->TypeDetailLabel->setText(Infos["type"]);

    QStringList Context;

    QMapIterator<QString, QString> it(Infos);
    while (it.hasNext())
    {
      it.next();

      if (it.key().startsWith("context."))
        Context.append(QString(it.key()).remove(0,8)+"="+it.value());
    }

    ui->ContextDetailLabel->setText(Context.join("\n"));

    ui->DetailsArea->setEnabled(true);
  }
  else
  {
    ui->DetailsArea->setEnabled(false);
    ui->MessageDetailLabel->setText("-");
    ui->TypeDetailLabel->setText("-");
    ui->ContextDetailLabel->setText("-");
  }
}


// =====================================================================
// =====================================================================


void LogExplorerDialog::enableFilterApply()
{
  ui->ApplyFilteringButton->setEnabled(true);
}


// =====================================================================
// =====================================================================


void LogExplorerDialog::applyFilters()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  ui->ApplyFilteringButton->setEnabled(false);

  updateTable(true);

  QApplication::restoreOverrideCursor();
}



} } }  // namespaces


