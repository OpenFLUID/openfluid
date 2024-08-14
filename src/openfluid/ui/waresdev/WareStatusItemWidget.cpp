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
  @file WareStatusItemWidget.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <iostream>
#include <vector>

#include <openfluid/config.hpp>

#include "ui_WareStatusItemWidget.h"
#include "WareStatusItemWidget.hpp"


namespace openfluid { namespace ui { namespace waresdev {


using ReportingData = openfluid::waresdev::WareSrcChecker::ReportingData;
using ReportingStatus = ReportingData::ReportingStatus;


std::map<std::pair<std::string, ReportingStatus>, 
             QString>  WareStatusItemWidget::ms_LabelForReportItem;
std::map<std::pair<std::string, ReportingStatus>, 
             std::pair<QString, QString>> WareStatusItemWidget::ms_ActionForReportItem;


// =====================================================================
// =====================================================================


void WareStatusItemWidget::populateReportItemLabels()
{
  std::vector<std::pair<std::string, QString>> WarningReports;
  std::vector<std::pair<std::string, QString>> ErrorReports;

  ErrorReports.push_back({"cmakelists_exists", tr("'CMakeLists.txt' file does not exist.")});
  WarningReports.push_back({"readme_exists", tr("Readme file does not exist")});
  WarningReports.push_back({"srcdir_exists", tr("'src' directory does not exist")});
  WarningReports.push_back({"docdir_exists", tr("'doc' directory does not exist")});
  WarningReports.push_back({"testsdir_exists", tr("'tests' directory does not exist")});

  ErrorReports.push_back({"file_iscorrect", tr("Metadata can not be read")});
  WarningReports.push_back({"migration_isclean_no_comments", tr("Comments from migration remain in code")});
  ErrorReports.push_back({"rootdir_exists", tr("")});
  ErrorReports.push_back({"version_iscorrect", tr("Ware version is below current OpenFLUID version")});
  WarningReports.push_back({"no_migration_files", 
                            tr("Migration folders remain in ware source path. Remove them or")}); 
                            // TODO : Find more consistent display
  ErrorReports.push_back({"no_migration_files", tr("Ware migration failed")});


  ErrorReports.push_back({"id_iscorrect", tr("No ID set in ware signature")});
  WarningReports.push_back({"name_exists", tr("No full ware name given in ware signature")});
  WarningReports.push_back({"authors_exist", tr("No author defined in ware signature")});
  WarningReports.push_back({"contacts_exist", tr("No contact provided in ware signature")});
  WarningReports.push_back({"licence_exists", tr("No license defined in ware signature")});
  WarningReports.push_back({"rootdir_matchesid", tr("Discrepency between ware ID and folder name")});
  WarningReports.push_back({"waretype_correct", tr("Unknown ware type")});
  WarningReports.push_back({"ware_description_exists", tr("No description in ware signature")});
  WarningReports.push_back({"data_description_exists", tr("No description for data in ware signature")});
  WarningReports.push_back({"data_unit_exists", tr("No SI unit set for data in ware signature")});

  for (const auto& Report : WarningReports)
  {
    ms_LabelForReportItem[std::make_pair<const std::string, const ReportingStatus>(
      std::string(Report.first), ReportingStatus::WARNING)] = Report.second;
  }

  for (const auto& Report : ErrorReports)
  {
    ms_LabelForReportItem[std::make_pair<const std::string, const ReportingStatus>(
      std::string(Report.first), ReportingStatus::ERROR_STATUS)] = Report.second;
  }

  ms_ActionForReportItem[std::make_pair<const std::string, const ReportingStatus>(
    "version_iscorrect", ReportingStatus::ERROR_STATUS)] = \
      std::pair<QString, QString>("migration", tr("Try to migrate the ware"));
  ms_ActionForReportItem[std::make_pair<const std::string, const ReportingStatus>(
    "migration_isclean_no_comments", ReportingStatus::WARNING)] = \
      std::pair<QString, QString>("", tr("Look for '%1' comments in CMakeLists.txt and main cpp file.").arg(
        QString::fromStdString(openfluid::config::MIGRATION_STRING)));
  ms_ActionForReportItem[std::make_pair<const std::string, const ReportingStatus>(
    "no_migration_files", ReportingStatus::ERROR_STATUS)] = \
      std::pair<QString, QString>("revert-migration", tr("Revert migration"));
  ms_ActionForReportItem[std::make_pair<const std::string, const ReportingStatus>(
    "no_migration_files", ReportingStatus::WARNING)] = \
      std::pair<QString, QString>("revert-migration", tr("Revert migration"));
}


// =====================================================================
// =====================================================================


WareStatusItemWidget::WareStatusItemWidget(const ReportingData::ReportingItem& Item, QWidget* Parent) :
  QWidget(Parent), ui(new Ui::WareStatusItemWidget)
{
  ui->setupUi(this);
  
  QString Message = QString::fromStdString(Item.Message);
  QString ActionCode = "";
  QString ActionLabel = "";
  QString CustomMessage = getLabelForItem(Item);
  if (!CustomMessage.isEmpty())
  {
    Message = CustomMessage;
  }
  auto CustomAction = getActionForItem(Item);
  if (CustomAction)
  {
    m_ActionCode = CustomAction.value().first;
    ActionLabel = CustomAction.value().second;
  }

  ui->MessageLabel->setText(Message);
  setStatusLevel(Item.Status);
  if (m_ActionCode.isEmpty())
  {
    ui->FixButton->setVisible(false);
    if (ActionLabel.isEmpty())
    {
      ui->HelpLabel->setVisible(false);
    }
    else
    {
      ui->HelpLabel->setText(ActionLabel);
    }
  }
  else
  {
    ui->FixButton->setText(ActionLabel);
    ui->HelpLabel->setVisible(false);
    connect(ui->FixButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
  }
  if (Item.SpecificInformation != "")
  {
    ui->InformationLabel->setText(QString::fromStdString(Item.SpecificInformation));
  }
  else
  {
    ui->InformationLabel->setVisible(false);
  }
}


// =====================================================================
// =====================================================================


void WareStatusItemWidget::onButtonPressed()
{
  emit actionTriggered(m_ActionCode);
}


// =====================================================================
// =====================================================================


WareStatusItemWidget::~WareStatusItemWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WareStatusItemWidget::setStatusLevel(const ReportingStatus Level)
{
  if (Level == ReportingStatus::WARNING)
  {
    ui->StatusIconLabel->setWarningStatus();
  }
  else if (Level == ReportingStatus::ERROR_STATUS)
  {
    ui->StatusIconLabel->setErrorStatus();
  }
  else if (Level == ReportingStatus::DISABLED)
  {
      ui->StatusIconLabel->setDisabledStatus();
  }
}


// =====================================================================
// =====================================================================


QString WareStatusItemWidget::getLabelForItem(const ReportingData::ReportingItem& Item)
{
  QString Label = "";
  for (auto KeyValue : ms_LabelForReportItem)
  {
    if (KeyValue.first.first == Item.Message && KeyValue.first.second == Item.Status)
    {
      Label = KeyValue.second;
    }
  }
  return Label;
}


// =====================================================================
// =====================================================================


std::optional<std::pair<QString, QString>> WareStatusItemWidget::getActionForItem(const
                                                                        ReportingData::ReportingItem& Item)
{
  QString Label = "";
  for (auto KeyValue : ms_ActionForReportItem)
  {
    if (KeyValue.first.first == Item.Message && KeyValue.first.second == Item.Status)
    {
      return KeyValue.second;
    }
  }
  return {};
}


} } } // namespaces
