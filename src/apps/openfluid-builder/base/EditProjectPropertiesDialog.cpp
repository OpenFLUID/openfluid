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
  @file EditProjectPropertiesDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPushButton>
#include <QDateTime>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/config.hpp>


#include "ui_EditProjectPropertiesDialog.h"
#include "EditProjectPropertiesDialog.hpp"
#include "ProjectCentral.hpp"


EditProjectPropertiesDialog::EditProjectPropertiesDialog(QWidget* Parent):
  openfluid::ui::common::MessageDialog(Parent),ui(new Ui::EditProjectPropertiesDialog)
{
  openfluid::base::RunContextManager* PrjMan = openfluid::base::RunContextManager::instance();

  ui->setupUi(this);

  setupMessageUi(tr("Edit project properties"));
  setMessage();

  ui->ProjectNameLabel->setText(QString::fromStdString(PrjMan->getProjectName()));
  ui->DescriptionTextEdit->setPlainText(QString::fromStdString(PrjMan->getProjectDescription()));
  ui->AuthorsLineEdit->setText(QString::fromStdString(PrjMan->getProjectAuthors()));

  QDateTime DT = QDateTime::fromString(QString::fromStdString(PrjMan->getProjectCreationDate()),"yyyyMMdd'T'hhmmss");
  if (DT.isValid())
  {
    ui->CreationDateLabel->setText(DT.toString("yyyy-MM-dd, hh:mm:ss"));
  }
  else
  {
    ui->CreationDateLabel->setText(tr("unknown"));
  }

  DT = QDateTime::fromString(QString::fromStdString(PrjMan->getProjectLastModDate()),"yyyyMMdd'T'hhmmss");
  if (DT.isValid())
  {
    ui->LastModDateLabel->setText(DT.toString("yyyy-MM-dd, hh:mm:ss"));
  }
  else
  {
    ui->LastModDateLabel->setText(tr("unknown"));
  }

  // Project files status

  const auto& LoadingReport = ProjectCentral::instance()->loadingReport();
  const bool DeprecatedFound = PrjMan->projectContainsDeprecatedFile(PrjMan->getProjectPath());
  bool DetailsNeeded = false;

  if (DeprecatedFound)
  {
    ui->DeprecatedIconLabel->setWarningStatus();
    ui->DeprecatedTextLabel->setText(tr("Deprecated configuration files found"));
    DetailsNeeded = true;
  }
  else
  {
    ui->DeprecatedIconLabel->setOKStatus();
    ui->DeprecatedTextLabel->setText(tr("No deprecated configuration file"));    
  }

  if (LoadingReport.hasFormat())
  {
    ui->FormatInfoIconLabel->setOKStatus();
    ui->FormatInfoTextLabel->setText(tr("Format information found in .fluidx files"));

    if (LoadingReport.isCorrectFormat())
    {
      ui->FormatVersionIconLabel->setOKStatus();
      ui->FormatVersionTextLabel->setText(tr("Format version is correct in .fluidx files"));
    }
    else
    {
      ui->FormatVersionIconLabel->setWarningStatus();
      ui->FormatVersionTextLabel->setText(tr("Format version is incorrect in .fluidx files"));
      DetailsNeeded = true;
    }
  }
  else
  {
    ui->FormatInfoIconLabel->setWarningStatus();
    ui->FormatInfoTextLabel->setText(tr("Format information is missing in .fluidx files"));
    ui->FormatVersionIconLabel->setDisabledStatus();
    ui->FormatVersionTextLabel->setText(tr("Format version cannot be verified"));
    DetailsNeeded = true;
  }

  if (LoadingReport.hasUnknownTags())
  {
    ui->UnknownTagsIconLabel->setWarningStatus();
    ui->UnknownTagsTextLabel->setText(tr("Unknown data found in .fluidx files"));
    DetailsNeeded = true;
  }
  else
  {
    ui->UnknownTagsIconLabel->setOKStatus();
    ui->UnknownTagsTextLabel->setText(tr("No unknown data found in .fluidx files"));
  }

  ui->DetailsWidget->setVisible(DetailsNeeded);

  if (DetailsNeeded)
  {
    ui->DetailsTextEdit->setVisible(false);
    ui->ShowDetailsLabel->setText(tr("Show details"));
    ui->DetailsTextEdit->setPlainText(generateReport(QString::fromStdString(PrjMan->getProjectPath()),
                                                     DeprecatedFound,LoadingReport));

    connect(ui->ShowDetailsLabel,SIGNAL(clicked()),this,SLOT(toggleDetails()));
  }
}


// =====================================================================
// =====================================================================


EditProjectPropertiesDialog::~EditProjectPropertiesDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


QString EditProjectPropertiesDialog::generateReport(const QString& ProjectPath, bool DeprecatedFiles, 
                                                    const openfluid::fluidx::FluidXIO::LoadingReport& Report) const
{
  QStringList ReportLines;

  if (DeprecatedFiles)
  {
    ReportLines << tr("Deprecated configuration file(s) for project in %1").arg(ProjectPath);
  }

  for (const auto& File : Report)
  {
    const QString FilePath = QString::fromStdString(File.first);

    if (!File.second.hasFormat())
    {
      ReportLines << tr("No format for tag <openfluid> in %1").arg(FilePath);
    }
    else if (!File.second.isCorrectFormat())
    {
      ReportLines << tr("Wrong format version '%1' for tag <openfluid> in %2")
                     .arg(QString::fromStdString(File.second.FormatVersion),FilePath);
    }

    for (const auto& Tag : File.second.UnknownTags)
    {
      ReportLines << tr("Unknown tag <%1> in %2").arg(QString::fromStdString(Tag),FilePath);
    }
  }

  return ReportLines.join("\n");
}


// =====================================================================
// =====================================================================


void EditProjectPropertiesDialog::toggleDetails()
{
  if (ui->DetailsTextEdit->isVisible())
  {
    ui->DetailsTextEdit->setVisible(false);
    ui->ShowDetailsLabel->setText(tr("Show details"));    
  }
  else
  {
    ui->DetailsTextEdit->setVisible(true);    
    ui->ShowDetailsLabel->setText(tr("Hide details"));
  }
}


// =====================================================================
// =====================================================================


QString EditProjectPropertiesDialog::getDescription() const
{
  return ui->DescriptionTextEdit->toPlainText();
}


// =====================================================================
// =====================================================================


QString EditProjectPropertiesDialog::getAuthors() const
{
  return ui->AuthorsLineEdit->text();
}
