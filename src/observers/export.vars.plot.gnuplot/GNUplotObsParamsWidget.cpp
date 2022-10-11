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
  @file GNUplotObsParamsWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QFileDialog>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>

#include <openfluid/utilsq/QtHelpers.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/tools/StringHelpers.hpp>

#include "GNUplotObsParamsWidget.hpp"
#include "ui_GNUplotObsParamsWidget.h"


BEGIN_BUILDEREXT_SIGNATURE(WARE_ID)

END_BUILDEREXT_SIGNATURE


// =====================================================================
// =====================================================================


GNUplotObsParamsWidget::GNUplotObsParamsWidget(): openfluid::ui::builderext::PluggableParameterizationExtension(),
  ui(new Ui::GNUplotObsParamsWidget)
{
  ui->setupUi(this);
  setAutoFillBackground(true);

  ui->MainTabWidget->setCurrentIndex(0);


  ui->AddSerieButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddSerieButton->setIconSize(QSize(16,16));

  ui->RemoveSerieButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveSerieButton->setIconSize(QSize(16,16));

  ui->RenameSerieButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->RenameSerieButton->setIconSize(QSize(16,16));

  ui->AddGraphButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddGraphButton->setIconSize(QSize(16,16));

  ui->RemoveGraphButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveGraphButton->setIconSize(QSize(16,16));

  ui->RenameGraphButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->RenameGraphButton->setIconSize(QSize(16,16));


  ui->RemoveSerieButton->setEnabled(false);
  ui->RenameSerieButton->setEnabled(false);

  ui->RemoveGraphButton->setEnabled(false);
  ui->RenameGraphButton->setEnabled(false);


  QStringList OutputFormats { "pdfcairo size 11.7,8.3 (A4)", "pdfcairo size 11.7,16.5 (A3)",
                              "pngcairo size 827,1170 (A4)", "pngcairo size 1170,1654 (A3)",
                              "svg size 827,1170 (A4)", "svg size 1170,1654 (A3)" };

  ui->TerminalComboBox->addItems(OutputFormats);

  QStringList SerieStyles { "line", "points", "lines" };
  ui->SerieStyleComboBox->addItems(SerieStyles);

  QStringList Legends { "default", "off", "outside" };
  ui->GraphLegendComboBox->addItems(Legends);


  // General tab

  connect(ui->TerminalComboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
          this, &GNUplotObsParamsWidget::setTerminal);
  connect(ui->OutputEdit, &QLineEdit::editingFinished, this, &GNUplotObsParamsWidget::setOutputFileName);
  connect(ui->GnuplotCheckBox, &QCheckBox::clicked, this, &GNUplotObsParamsWidget::setGNUplotOption);
  connect(ui->KeepGnuplotCheckBox, &QCheckBox::clicked, this, &GNUplotObsParamsWidget::setPersistantOption);


  // Series tab

  ui->SerieInfosWidget->setCurrentIndex(0);

  connect(ui->AddSerieButton, &QPushButton::clicked, this, &GNUplotObsParamsWidget::addSerie);
  connect(ui->RemoveSerieButton, &QPushButton::clicked, this, &GNUplotObsParamsWidget::removeSerie);
  connect(ui->RenameSerieButton, &QPushButton::clicked, this, &GNUplotObsParamsWidget::editSerieName);
  connect(ui->SeriesListWidget, &QListWidget::itemChanged, this, &GNUplotObsParamsWidget::renameSerie);
  connect(ui->SeriesListWidget, &QListWidget::currentRowChanged, this, &GNUplotObsParamsWidget::setCurrentSerie);

  ui->DataFileSourceWidget->hide();
  connect(ui->SerieVariableRadio, &QRadioButton::clicked, this, &GNUplotObsParamsWidget::changeSerieType);
  connect(ui->SerieDataFileRadio, &QRadioButton::clicked, this, &GNUplotObsParamsWidget::changeSerieType);

  connect(ui->VariableNameEdit, &QLineEdit::editingFinished, this, &GNUplotObsParamsWidget::setVariableName);
  connect(ui->UnitsClassComboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::activated),
            this, &GNUplotObsParamsWidget::setUnitsClass);
  connect(ui->UnitIDComboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::activated),
            this, &GNUplotObsParamsWidget::setUnitID);

  connect(ui->BrowseButton, &QPushButton::clicked, this, &GNUplotObsParamsWidget::setSourceFileName);

  connect(ui->SerieLabelEdit, &QLineEdit::editingFinished, this, &GNUplotObsParamsWidget::setSerieLabel);
  connect(ui->SerieStyleComboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            this, &GNUplotObsParamsWidget::setSerieStyle);


  // Graphs tab

  ui->GraphInfosWidget->setCurrentIndex(0);

  connect(ui->AddGraphButton, &QPushButton::clicked, this, &GNUplotObsParamsWidget::addGraph);
  connect(ui->RemoveGraphButton, &QPushButton::clicked, this, &GNUplotObsParamsWidget::removeGraph);
  connect(ui->RenameGraphButton, &QPushButton::clicked, this, &GNUplotObsParamsWidget::editGraphName);
  connect(ui->GraphsListWidget, &QListWidget::itemChanged, this, &GNUplotObsParamsWidget::renameGraph);
  connect(ui->GraphsListWidget, &QListWidget::currentRowChanged, this, &GNUplotObsParamsWidget::setCurrentGraph);

  connect(ui->GraphTitleEdit, &QLineEdit::editingFinished, this, &GNUplotObsParamsWidget::setGraphTitle);
  connect(ui->GraphYLabelEdit, &QLineEdit::editingFinished, this, &GNUplotObsParamsWidget::setGraphYLabel);
  connect(ui->GraphLegendComboBox, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            this, &GNUplotObsParamsWidget::setGraphLegend);
  connect(ui->GraphSeriesListWidget, &QListWidget::itemClicked, this, &GNUplotObsParamsWidget::setGraphSeries);
}


// =====================================================================
// =====================================================================


GNUplotObsParamsWidget::~GNUplotObsParamsWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::removeParamsStartingWith(const QString& Str)
{
  for (auto it = mp_Params->begin(); it!=mp_Params->end();)
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


void GNUplotObsParamsWidget::renameParamsStartingWith(const QString& Before, const QString& After)
{
  for (auto it = mp_Params->begin(); it != mp_Params->end();)
  {
    QString ParamName = QString::fromStdString((*it).first);

    if (ParamName.startsWith(Before))
    {
      ParamName.replace(Before, After);

      mp_Params->insert({ParamName.toStdString(), it->second});
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


QString GNUplotObsParamsWidget::generateListItemName(const QListWidget* ListWidget,const QString& NameRoot)
{
  QString Name;
  int Counter = 1;

  QStringList ExistingNames;

  for (auto i = 0; i < ListWidget->count();i++ )
  {
    ExistingNames.append(ListWidget->item(i)->text());
  }

  Name = QString("%1%2").arg(NameRoot).arg(Counter);
  while (ExistingNames.contains(Name))
  {
    Counter++;
    Name = QString("%1%2").arg(NameRoot).arg(Counter);
  }
  return Name;
}


// =====================================================================
// =====================================================================


int GNUplotObsParamsWidget::findItemRowInList(const QListWidget* ListWidget,const QString& ItemText)
{
  for (int i=0; i<ListWidget->count(); i++)
  {
    if (ListWidget->item(i)->text() == ItemText)
    {
      return i;
    }
  }

  return -1;
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::update()
{
  openfluid::ware::WareParamsTree ParamsTree;

  try
  {
    ParamsTree.setParams(*mp_Params);

    parsePlotFromParamsTree(ParamsTree, m_Plot);
  }
  catch (openfluid::base::FrameworkException& E)
  {
    OPENFLUID_RaiseError(E.getMessage());
  }

  // Clear ui
  int CurrentSeriesRow = ui->SeriesListWidget->currentRow();
  int CurrentGraphsRow = ui->GraphsListWidget->currentRow();

  ui->SeriesListWidget->clear();
  ui->GraphsListWidget->clear();
  ui->GraphSeriesListWidget->clear();

  QString Terminal = QString::fromStdString(m_Plot.Terminal);
  if (!Terminal.isEmpty() && ui->TerminalComboBox->findText(Terminal) == -1)
  {
    ui->TerminalComboBox->addItem(Terminal);
  }

  ui->TerminalComboBox->setCurrentText(Terminal);

  ui->OutputEdit->setText(QString::fromStdString(m_Plot.Output));
  ui->GnuplotCheckBox->setChecked(m_Plot.TryOpenGNUplot);
  ui->KeepGnuplotCheckBox->setChecked(m_Plot.Persistent);

  for (const auto& Serie : m_Plot.Series)
  {
    QListWidgetItem *SerieItem = nullptr;

    SerieItem = new QListWidgetItem(QString::fromStdString(Serie.first));
    SerieItem->setFlags(SerieItem->flags() | Qt::ItemIsEditable);
    ui->SeriesListWidget->addItem(SerieItem);

    SerieItem = new QListWidgetItem(QString::fromStdString(Serie.first));
    SerieItem->setFlags(SerieItem->flags() | Qt::ItemIsUserCheckable);
    SerieItem->setCheckState(Qt::Unchecked);
    ui->GraphSeriesListWidget->addItem(SerieItem);
  }

  for (const auto& Graph : m_Plot.Graphs)
  {
    QListWidgetItem *GraphItem = new QListWidgetItem(QString::fromStdString(Graph.first));
    GraphItem->setFlags(GraphItem->flags() | Qt::ItemIsEditable);
    ui->GraphsListWidget->addItem(GraphItem);
  }

  ui->SeriesListWidget->setCurrentRow(CurrentSeriesRow);
  ui->GraphsListWidget->setCurrentRow(CurrentGraphsRow);
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setTerminal(const QString& Terminal)
{
  mp_Params->erase("terminal");
  mp_Params->insert({"terminal", Terminal.toStdString()});

  emit changed();
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setOutputFileName()
{
  mp_Params->erase("output");
  mp_Params->insert({"output", ui->OutputEdit->text().toStdString()});

  emit changed();
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setGNUplotOption(bool Checked)
{
  mp_Params->erase("tryopengnuplot");
  mp_Params->insert({"tryopengnuplot", openfluid::core::StringValue(Checked)});

  emit changed();
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setPersistantOption(bool Checked)
{
  mp_Params->erase("persistent");
  mp_Params->insert({"persistent", openfluid::core::StringValue(Checked)});

  emit changed();
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::addSerie()
{
  QString SerieName = generateListItemName(ui->SeriesListWidget,"s");
  std::string Prefix = "serie." + SerieName.toStdString() + ".";

  openfluid::ware::WareParams_t Params({{Prefix + "var", ""},
                                        {Prefix + "unitsclass", ""},
                                        {Prefix + "unitID", ""},
                                        {Prefix + "label", ""},
                                        {Prefix + "style", "line"}});

  mp_Params->insert(Params.begin(), Params.end());

  update();

  ui->SeriesListWidget->setCurrentRow(findItemRowInList(ui->SeriesListWidget,SerieName));

  emit changed();

}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::removeSerie()
{
  int CurrentRow = ui->SeriesListWidget->currentRow();

  if (CurrentRow >= 0)
  {
    QString SerieName = ui->SeriesListWidget->item(CurrentRow)->text();

    if (QMessageBox::question(QApplication::activeWindow(),
                              tr("Removing serie"),
                              tr("You are removing serie %1.\n"
                                 "The parameters of this serie will also be deleted.\n"
                                 "\n"
                                 "Proceed anyway?").arg(SerieName),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {
      QString SerieStr = "serie." + SerieName + ".";

      removeParamsStartingWith(SerieStr);

      std::map<std::string, GraphInfo>::iterator Git;
      std::map<std::string, GraphInfo>::iterator Gitb = m_Plot.Graphs.begin();
      std::map<std::string, GraphInfo>::iterator Gite = m_Plot.Graphs.end();

      // Remove of serie in graph series lists
      for (Git = Gitb; Git != Gite; ++Git)
      {
        std::string GraphSeriesKey = "graph." + Git->first + ".series";
        std::string GraphSeriesStr = "";

        auto Pos = mp_Params->find(GraphSeriesKey);
        if (Pos != mp_Params->end())
        {
          GraphSeriesStr = Pos->second;
        }

        std::vector<std::string> GraphSeries = openfluid::tools::split(GraphSeriesStr, ";");
        GraphSeriesStr.clear();

        for (auto GSit = GraphSeries.begin(); GSit != GraphSeries.end(); ++GSit)
        {
          if (*GSit != SerieName.toStdString())
          {
            GraphSeriesStr += *GSit + ";";
          }
        }

        if (!GraphSeriesStr.empty())
        {
          GraphSeriesStr.erase(GraphSeriesStr.size() - 1);
        }

        mp_Params->erase(GraphSeriesKey);
        mp_Params->insert({GraphSeriesKey, GraphSeriesStr});
      }

      update();
      emit changed();
    }
  }

  if (!ui->SeriesListWidget->count())
  {
    m_CurrentSerieName = "";
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::renameSerie(QListWidgetItem* Item)
{
  if (!m_CurrentSerieName.isEmpty() and Item)
  {
    QString NewSerieName = Item->text();

    // Perform renaming only if the new serie name is not already used
    if (ui->SeriesListWidget->findItems(NewSerieName,Qt::MatchExactly).size() == 1)
    {
      QString CurrentSeriePrefix = "serie." + m_CurrentSerieName + ".";
      QString NewSeriePrefix = "serie." + NewSerieName + ".";

      renameParamsStartingWith(CurrentSeriePrefix, NewSeriePrefix);

      std::map<std::string, GraphInfo>::iterator Git;
      std::map<std::string, GraphInfo>::iterator Gitb = m_Plot.Graphs.begin();
      std::map<std::string, GraphInfo>::iterator Gite = m_Plot.Graphs.end();

      // Rename of serie in graph series lists
      for (Git = Gitb; Git != Gite; ++Git)
      {
        std::string GraphSeriesKey = "graph." + Git->first + ".series";
        std::string GraphSeriesStr = "";

        auto Pos = mp_Params->find(GraphSeriesKey);
        if (Pos != mp_Params->end())
        {
          GraphSeriesStr = Pos->second;
        }

        std::vector<std::string> GraphSeries = openfluid::tools::split(GraphSeriesStr, ";");
        GraphSeriesStr.clear();

        for (auto GSit = GraphSeries.begin(); GSit != GraphSeries.end(); ++GSit)
        {
          if (*GSit != m_CurrentSerieName.toStdString())
          {
            GraphSeriesStr += *GSit + ";";
          }
          else
          {
            GraphSeriesStr += NewSerieName.toStdString() + ";";
          }
        }

        if (!GraphSeriesStr.empty())
        {
          GraphSeriesStr.erase(GraphSeriesStr.size() - 1);
        }

        mp_Params->erase(GraphSeriesKey);
        mp_Params->insert({GraphSeriesKey, GraphSeriesStr});
      }

      m_CurrentSerieName = NewSerieName;
    }
    else
    {
      QMessageBox::critical(this, tr("Serie renaming"),tr("The serie name %1 already exists.").arg(NewSerieName),
                            QMessageBox::Close);
    }

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::editSerieName()
{
  QListWidgetItem* CurrentItem = ui->SeriesListWidget->currentItem();
  if (CurrentItem)
  {
    ui->SeriesListWidget->editItem(CurrentItem);
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setCurrentSerie(int CurrentRow)
{
  QListWidgetItem *CurrentItem = ui->SeriesListWidget->item(CurrentRow);
  if (CurrentItem)
  {
    ui->RemoveSerieButton->setEnabled(true);
    ui->RenameSerieButton->setEnabled(true);

    m_CurrentSerieName = CurrentItem->text();

    std::string CurrentSerieStr = m_CurrentSerieName.toStdString();
    const SerieInfo& Serie = m_Plot.Series[CurrentSerieStr];

    if (Serie.Type == SerieInfo::SerieType::SERIE_UNKNOWN
        || Serie.Type == SerieInfo::SerieType::SERIE_VAR)
    {
      ui->SerieVariableRadio->setChecked(true);
      ui->VariableSourceWidget->show();
      ui->DataFileSourceWidget->hide();

      ui->VariableNameEdit->setText(QString::fromStdString(Serie.VarName));
      ui->UnitsClassComboBox->clear();

      QStringList ClassNames = openfluid::utils::toQStringList(mp_Desc->spatialDomain().getClassNames());
      for (auto& ClassName : ClassNames)
      {
        ui->UnitsClassComboBox->addItem(ClassName);
      }

      QString UnitsClass = QString::fromStdString(Serie.UnitsClass);
      if (ui->UnitsClassComboBox->findText(UnitsClass) != -1)
      {
        ui->UnitsClassComboBox->setCurrentText(UnitsClass);
        updateClassIDs(UnitsClass);

        QString UnitID = QString::number(Serie.UnitID);
        if (ui->UnitIDComboBox->findText(UnitID) != -1)
        {
          ui->UnitIDComboBox->setCurrentText(UnitID);
        }
        else
        {
          ui->UnitIDComboBox->setCurrentIndex(0);
          setUnitID(ui->UnitIDComboBox->currentText());
        }
      }
      else
      {
        ui->UnitsClassComboBox->setCurrentIndex(0);
        setUnitsClass(ui->UnitsClassComboBox->currentText());
      }
    }
    else if (Serie.Type == SerieInfo::SerieType::SERIE_FILE)
    {
      ui->SerieDataFileRadio->setChecked(true);
      ui->DataFileSourceWidget->show();
      ui->VariableSourceWidget->hide();

      ui->SourceFileEdit->setText(QString::fromStdString(Serie.SourceFile));
    }

    ui->SerieLabelEdit->setText(QString::fromStdString(Serie.Label));

    QString Style = QString::fromStdString(Serie.Style);
    if (!Style.isEmpty() && ui->SerieStyleComboBox->findText(Style) == -1)
    {
      ui->SerieStyleComboBox->addItem(Style);
    }

    ui->SerieStyleComboBox->setCurrentText(Style);

    ui->SerieInfosWidget->setCurrentIndex(1);
  }
  else
  {
    ui->SerieInfosWidget->setCurrentIndex(0);
    m_CurrentSerieName = "";
    ui->RemoveSerieButton->setEnabled(false);
    ui->RenameSerieButton->setEnabled(false);
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::changeSerieType()
{
  if (ui->SeriesListWidget->currentRow() >= 0)
  {
    std::string SeriePrefix = "serie." + ui->SeriesListWidget->currentItem()->text().toStdString() + ".";

    if (ui->SerieVariableRadio->isChecked())
    {
      mp_Params->erase(SeriePrefix + "sourcefile");

      mp_Params->insert({SeriePrefix + "var", ui->VariableNameEdit->text().toStdString()});
      mp_Params->insert({SeriePrefix + "unitsclass", ui->UnitsClassComboBox->currentText().toStdString()});
      mp_Params->insert({SeriePrefix + "unitID", ui->UnitIDComboBox->currentText().toStdString()});
    }
    else if (ui->SerieDataFileRadio->isChecked())
    {
      mp_Params->erase(SeriePrefix + "var");
      mp_Params->erase(SeriePrefix + "unitsclass");
      mp_Params->erase(SeriePrefix + "unitclass");
      mp_Params->erase(SeriePrefix + "unitID");

      mp_Params->insert({SeriePrefix + "sourcefile", ui->SourceFileEdit->text().toStdString()});
    }

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setVariableName()
{
  if (ui->SeriesListWidget->currentRow() >= 0)
  {
    std::string SeriePrefix = "serie." + ui->SeriesListWidget->currentItem()->text().toStdString() + ".";

    mp_Params->erase(SeriePrefix + "var");
    mp_Params->insert({SeriePrefix + "var", ui->VariableNameEdit->text().toStdString()});

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setUnitsClass(const QString& UnitsClass)
{
  if (ui->SeriesListWidget->currentRow() >= 0)
  {
    std::string SeriePrefix = "serie." + ui->SeriesListWidget->currentItem()->text().toStdString() + ".";

    mp_Params->erase(SeriePrefix + "unitsclass");
    mp_Params->insert({SeriePrefix + "unitsclass", UnitsClass.toStdString()});

    updateClassIDs(UnitsClass);

    ui->UnitIDComboBox->setCurrentIndex(0);
    setUnitID(ui->UnitIDComboBox->currentText());
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::updateClassIDs(const QString& UnitsClass)
{
  ui->UnitIDComboBox->clear();

  std::set<int> IDs = mp_Desc->spatialDomain().getIDsOfClass(UnitsClass.toStdString());

  for (auto ID : IDs)
  {
    ui->UnitIDComboBox->addItem(QString::number(ID));
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setUnitID(const QString& UnitID)
{
  if (ui->SeriesListWidget->currentRow() >= 0)
  {
    std::string SeriePrefix = "serie." + ui->SeriesListWidget->currentItem()->text().toStdString() + ".";

    mp_Params->erase(SeriePrefix + "unitID");
    mp_Params->insert({SeriePrefix + "unitID", UnitID.toStdString()});

    emit changed();
    update();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setSourceFileName()
{
  if (ui->SeriesListWidget->currentRow() >= 0)
  {
    std::string InputDir;
    OPENFLUID_GetRunEnvironment("dir.input", InputDir);

    QString Filepath = QFileDialog::getOpenFileName(this, tr("Select source file"),
                                    QString::fromStdString(InputDir), tr("Data files (*.dat)"));

    if (!Filepath.isNull())
    {
      QFileInfo FileInfo(Filepath);

      if (!FileInfo.absolutePath().startsWith(QString::fromStdString(InputDir)))
      {
          QMessageBox::critical(this, tr("Source file"),
              tr("Chosen file is not located in the IN folder of the project."), QMessageBox::Close);
      }
      else
      {
        std::string SeriePrefix = "serie." + ui->SeriesListWidget->currentItem()->text().toStdString() + ".";

        mp_Params->erase(SeriePrefix + "sourcefile");
        mp_Params->insert({SeriePrefix + "sourcefile", Filepath.toStdString()});

        update();
        emit changed();
      }
    }
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setSerieLabel()
{
  if (ui->SeriesListWidget->currentRow() >= 0)
  {
    std::string SeriePrefix = "serie." + ui->SeriesListWidget->currentItem()->text().toStdString() + ".";

    mp_Params->erase(SeriePrefix + "label");
    mp_Params->insert({SeriePrefix + "label", ui->SerieLabelEdit->text().toStdString()});

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setSerieStyle(const QString& Style)
{
  if (ui->SeriesListWidget->currentRow() >= 0)
  {
    std::string SeriePrefix = "serie." + ui->SeriesListWidget->currentItem()->text().toStdString() + ".";

    mp_Params->erase(SeriePrefix + "style");
    mp_Params->insert({SeriePrefix + "style", Style.toStdString()});

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::addGraph()
{
  QString GraphName = generateListItemName(ui->GraphsListWidget,"g");
  std::string Prefix = "graph." + GraphName.toStdString() + ".";

  openfluid::ware::WareParams_t Params({{Prefix+"series",""},
                                        {Prefix+"title",""},
                                        {Prefix+"key","default"},
                                        {Prefix+"ylabel",""}});

  mp_Params->insert(Params.begin(), Params.end());

  update();

  ui->GraphsListWidget->setCurrentRow(findItemRowInList(ui->GraphsListWidget,GraphName));

  emit changed();
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::removeGraph()
{
  int CurrentRow = ui->GraphsListWidget->currentRow();

  if (CurrentRow >= 0)
  {
    QString GraphName = ui->GraphsListWidget->item(CurrentRow)->text();

    if (QMessageBox::question(QApplication::activeWindow(),
                                tr("Removing graph"),
                                tr("You are removing graph %1.\n"
                                   "The parameters of this graph will also be deleted.\n"
                                   "The series associated with this graph will not be deleted.\n"
                                   "\n"
                                   "Proceed anyway?").arg(GraphName),
                                 QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {

      QString GraphPrefix = "graph." + GraphName + ".";

      removeParamsStartingWith(GraphPrefix);


      update();
      emit changed();
    }
  }

  if (!ui->GraphsListWidget->count())
  {
    m_CurrentGraphName = "";
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::renameGraph(QListWidgetItem* Item)
{
  if (!m_CurrentGraphName.isEmpty() and Item)
  {
    QString NewGraphName = Item->text();

    // Perform renaming only if the new graph name is not already used
    if (ui->GraphsListWidget->findItems(NewGraphName,Qt::MatchExactly).size() == 1)
    {
      QString CurrentGraphPrefix = "graph." + m_CurrentGraphName + ".";
      QString NewGrahPrefix = "graph." + NewGraphName + ".";

      renameParamsStartingWith(CurrentGraphPrefix, NewGrahPrefix);

      m_CurrentGraphName = NewGraphName;
    }
    else
    {
      QMessageBox::critical(this,tr("Graph renaming"),tr("The graph name %1 already exists.").arg(NewGraphName),
                            QMessageBox::Close);
    }

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::editGraphName()
{
  QListWidgetItem* CurrentItem = ui->GraphsListWidget->currentItem();
  if (CurrentItem)
  {
    ui->GraphsListWidget->editItem(CurrentItem);
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setCurrentGraph(int CurrentRow)
{
  QListWidgetItem *CurrentItem = ui->GraphsListWidget->item(CurrentRow);
  if (CurrentItem)
  {
    ui->RemoveGraphButton->setEnabled(true);
    ui->RenameGraphButton->setEnabled(true);

    m_CurrentGraphName = CurrentItem->text();

    std::string CurrentGraphStr = m_CurrentGraphName.toStdString();

    for (const auto& Graph : m_Plot.Graphs)
    {
      if (Graph.first == CurrentGraphStr)
      {
        ui->GraphTitleEdit->setText(QString::fromStdString(Graph.second.Title));
        ui->GraphYLabelEdit->setText(QString::fromStdString(Graph.second.YLabel));

        QString Legend = QString::fromStdString(Graph.second.Key);
        if (!Legend.isEmpty() && ui->GraphLegendComboBox->findText(Legend) == -1)
        {
          ui->GraphLegendComboBox->addItem(Legend);
        }

        ui->GraphLegendComboBox->setCurrentText(Legend);

        for (int Row = 0; Row < ui->GraphSeriesListWidget->count(); Row++)
        {
          QListWidgetItem *Item = ui->GraphSeriesListWidget->item(Row);

          Qt::CheckState State;
          auto SeriePos = std::find(Graph.second.Series.begin(), Graph.second.Series.end(),
                                      &m_Plot.Series[Item->text().toStdString()]);

          if (SeriePos != Graph.second.Series.end())
          {
            State = Qt::Checked;
          }
          else
          {
            State = Qt::Unchecked;
          }

          Item->setCheckState(State);
        }

        ui->GraphInfosWidget->setCurrentIndex(1);

        return;
      }
    }
  }
  else
  {
    ui->GraphInfosWidget->setCurrentIndex(0);
    m_CurrentGraphName = "";
    ui->RemoveGraphButton->setEnabled(false);
    ui->RenameGraphButton->setEnabled(false);
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setGraphTitle()
{
  if (ui->GraphsListWidget->currentRow() >= 0)
  {
    std::string GraphPrefix = "graph." + ui->GraphsListWidget->currentItem()->text().toStdString() + ".";

    mp_Params->erase(GraphPrefix + "title");
    mp_Params->insert({GraphPrefix + "title", ui->GraphTitleEdit->text().toStdString()});

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setGraphYLabel()
{
  if (ui->GraphsListWidget->currentRow() >= 0)
  {
    std::string GraphPrefix = "graph." + ui->GraphsListWidget->currentItem()->text().toStdString() + ".";

    mp_Params->erase(GraphPrefix + "ylabel");
    mp_Params->insert({GraphPrefix + "ylabel", ui->GraphYLabelEdit->text().toStdString()});

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setGraphLegend(const QString& Key)
{
  if (ui->GraphsListWidget->currentRow() >= 0)
  {
    std::string GraphPrefix = "graph." + ui->GraphsListWidget->currentItem()->text().toStdString() + ".";

    mp_Params->erase(GraphPrefix + "key");
    mp_Params->insert({GraphPrefix + "key", Key.toStdString()});

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void GNUplotObsParamsWidget::setGraphSeries()
{
  if (ui->GraphsListWidget->currentRow() >= 0)
  {
    std::string GraphPrefix = "graph." + ui->GraphsListWidget->currentItem()->text().toStdString() + ".";
    std::string SeriesStr = "";

    for (int Row = 0; Row < ui->GraphSeriesListWidget->count(); Row++)
    {
      QListWidgetItem *Item = ui->GraphSeriesListWidget->item(Row);
      if (Item->checkState() == Qt::Checked)
      {
        SeriesStr += Item->text().toStdString() + ";";
      }
    }

    if (!SeriesStr.empty())
    {
      SeriesStr.erase(SeriesStr.size() - 1);
    }

    mp_Params->erase(GraphPrefix + "series");
    mp_Params->insert({GraphPrefix + "series", SeriesStr});

    update();
    emit changed();
  }
}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(GNUplotObsParamsWidget)

DEFINE_WARE_LINKUID(WARE_LINKUID)
