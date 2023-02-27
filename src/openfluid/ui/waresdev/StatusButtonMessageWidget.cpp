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
  @file StatusButtonMessageWidget.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <iostream>

#include "ui_StatusButtonMessageWidget.h"
#include "StatusButtonMessageWidget.hpp"


namespace openfluid { namespace ui { namespace waresdev {


typedef openfluid::waresdev::WareSrcChecker::ReportingData ReportingData; // DIRTYCODE redundant

std::map<std::pair<std::string, ReportingData::ReportingStatus>, 
             QString>  StatusButtonMessageWidget::s_LabelForReportItem;
std::map<std::pair<std::string, ReportingData::ReportingStatus>, 
             std::pair<QString, QString>> StatusButtonMessageWidget::s_ActionForReportItem;


// =====================================================================
// =====================================================================


void StatusButtonMessageWidget::populateReportItemLabels() // FIXME: check if tr works with static functions
{
  s_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "version_iscorrect", ReportingData::ReportingStatus::ERROR)] = \
      tr("Ware version is below current OpenFLUID version");
  s_LabelForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "migration_isclean", ReportingData::ReportingStatus::WARNING)] = \
      tr("Comments from migration remain in code");


  s_ActionForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "version_iscorrect", ReportingData::ReportingStatus::ERROR)] = \
      std::pair<QString, QString>("migration", tr("Try to migrate the ware"));
  s_ActionForReportItem[std::make_pair<const std::string, const ReportingData::ReportingStatus>(
    "migration_isclean", ReportingData::ReportingStatus::WARNING)] = \
      std::pair<QString, QString>("", tr("Look for '[MIGRATION]' comments in CMakeLists.txt and main cpp file."));
}


// =====================================================================
// =====================================================================


StatusButtonMessageWidget::StatusButtonMessageWidget(const ReportingData::ReportingItem& Item, QWidget* Parent) :
  QWidget(Parent), ui(new Ui::StatusButtonMessageWidget)
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
    if (!ActionLabel.isEmpty())
    {
      ui->HelpLabel->setText(ActionLabel);
    }
  }
  else
  {
    ui->HelpLabel->setVisible(false);
    ui->FixButton->setText(ActionLabel);
    connect(ui->FixButton, SIGNAL(pressed()), this, SLOT(onButtonPressed()));
  }
}


// =====================================================================
// =====================================================================


void StatusButtonMessageWidget::onButtonPressed()
{
  emit actionTriggered(m_ActionCode);
}


// =====================================================================
// =====================================================================


StatusButtonMessageWidget::~StatusButtonMessageWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void StatusButtonMessageWidget::setStatusLevel(const ReportingData::ReportingStatus Level)
{
  if (Level == ReportingData::ReportingStatus::WARNING)
  {
    ui->StatusIconLabel->setWarningStatus();
  }
  else if (Level == ReportingData::ReportingStatus::ERROR)
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


QString StatusButtonMessageWidget::getLabelForItem(const ReportingData::ReportingItem& Item)
{
  QString Label = "";
  for (auto KeyValue : s_LabelForReportItem)
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


std::optional<std::pair<QString, QString>> StatusButtonMessageWidget::getActionForItem(const
                                                                        ReportingData::ReportingItem& Item)
{
  QString Label = "";
  for (auto KeyValue : s_ActionForReportItem)
  {
    if (KeyValue.first.first == Item.Message && KeyValue.first.second == Item.Status)
    {
      return KeyValue.second;
    }
  }
  return {};
}


} } } // namespaces
