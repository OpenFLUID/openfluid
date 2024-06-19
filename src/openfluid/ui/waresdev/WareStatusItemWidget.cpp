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

#include "ui_WareStatusItemWidget.h"
#include "WareStatusItemWidget.hpp"


namespace openfluid { namespace ui { namespace waresdev {


using ReportingData = openfluid::waresdev::WareSrcChecker::ReportingData;


std::map<std::pair<std::string, ReportingData::ReportingStatus>, 
             QString>  WareStatusItemWidget::ms_LabelForReportItem;
std::map<std::pair<std::string, ReportingData::ReportingStatus>, 
             std::pair<QString, QString>> WareStatusItemWidget::ms_ActionForReportItem;


// =====================================================================
// =====================================================================


void WareStatusItemWidget::populateReportItemLabels()
{
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "cmakelists_exists", ReportingData::ReportingStatus::ERROR_STATUS)] = \
      tr("'CMakeLists.txt' file does not exist.");
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "readme_exists", ReportingData::ReportingStatus::WARNING)] = \
      tr("Readme file does not exist");
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "srcdir_exists", ReportingData::ReportingStatus::WARNING)] = \
      tr("'src' directory does not exist");
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "docdir_exists", ReportingData::ReportingStatus::WARNING)] = \
      tr("'doc' directory does not exist");
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "testsdir_exists", ReportingData::ReportingStatus::WARNING)] = \
      tr("'tests' directory does not exist");

  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "file_iscorrect", ReportingData::ReportingStatus::ERROR_STATUS)] = \
      tr("Metadata can not be read");
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "migration_isclean_no_comments", ReportingData::ReportingStatus::WARNING)] = \
      tr("Comments from migration remain in code");
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "rootdir_exists", ReportingData::ReportingStatus::ERROR_STATUS)] = \
      tr("");
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "version_iscorrect", ReportingData::ReportingStatus::ERROR_STATUS)] = \
      tr("Ware version is below current OpenFLUID version");
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "no_migration_files", ReportingData::ReportingStatus::WARNING)] = \
      tr("Migration folders remain in ware source path. Remove them or"); // TODO : Find better solution
  ms_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "no_migration_files", ReportingData::ReportingStatus::ERROR_STATUS)] = \
      tr("Ware migration failed");


  ms_ActionForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "version_iscorrect", ReportingData::ReportingStatus::ERROR_STATUS)] = \
      std::pair<QString, QString>("migration", tr("Try to migrate the ware"));
  ms_ActionForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "migration_isclean_no_comments", ReportingData::ReportingStatus::WARNING)] = \
      std::pair<QString, QString>("", tr("Look for '[MIGRATION]' comments in CMakeLists.txt and main cpp file."));
  ms_ActionForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "no_migration_files", ReportingData::ReportingStatus::ERROR_STATUS)] = \
      std::pair<QString, QString>("revert-migration", tr("Revert migration"));
  ms_ActionForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "no_migration_files", ReportingData::ReportingStatus::WARNING)] = \
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


void WareStatusItemWidget::setStatusLevel(const ReportingData::ReportingStatus Level)
{
  if (Level == ReportingData::ReportingStatus::WARNING)
  {
    ui->StatusIconLabel->setWarningStatus();
  }
  else if (Level == ReportingData::ReportingStatus::ERROR_STATUS)
  {
    ui->StatusIconLabel->setErrorStatus();
  }
  else if (Level == ReportingData::ReportingStatus::DISABLED)
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
