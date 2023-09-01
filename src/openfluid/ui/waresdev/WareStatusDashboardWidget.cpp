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
  @file WareStatusDashboardWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <QVBoxLayout>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/waresdev/WareSrcChecker.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/waresdev/WareStatusDashboardWidget.hpp>
#include <openfluid/ui/waresdev/WareStatusItemWidget.hpp>

#include "ui_WareStatusDashboardWidget.h"


namespace openfluid { namespace ui { namespace waresdev {


using ReportingData = openfluid::waresdev::WareSrcChecker::ReportingData;


WareStatusDashboardWidget::WareStatusDashboardWidget(QWidget* Parent, const std::string& WarePath):
  QWidget(Parent), ui(new Ui::WareStatusDashboardWidget), m_WarePath(WarePath), m_TitleColor("green")
{
  ui->setupUi(this);

  ui->TitleLabel->setStyleSheet("background-color:white; font:bold;");
  ui->TitleLabel->setElideMode(Qt::ElideRight);
  
  ui->InnerWidget->setStyleSheet(QString("#InnerWidget "
                                        "{background-color: white;}"));
  
  ui->ContentsScrollArea->setStyleSheet("QFrame {background-color: white;}");

  ui->ContentsWidget->setStyleSheet("background-color: white;");


  QVBoxLayout* Layout = new QVBoxLayout;
  Layout->setContentsMargins(0,0,0,0);
  Layout->setSpacing(0);
  ui->ContentsWidget->setLayout(Layout);

  mp_MessageContainer = new QWidget(this);
  QVBoxLayout* MessageLayout = new QVBoxLayout;
  MessageLayout->setContentsMargins(0,0,0,0);
  MessageLayout->setSpacing(0);
  mp_MessageContainer->setLayout(MessageLayout);

  ((QBoxLayout*)ui->ContentsWidget->layout())->addWidget(mp_MessageContainer, 0,Qt::AlignTop);

  ((QBoxLayout*)ui->ContentsWidget->layout())->addStretch();
  
  refresh();
  // TODO (advanced: see if watcher catches external change)
}


// =====================================================================
// =====================================================================


WareStatusDashboardWidget::~WareStatusDashboardWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WareStatusDashboardWidget::addMessage(openfluid::waresdev::WareSrcChecker::ReportingData::ReportingItem Item)
{
  WareStatusItemWidget* MessageWidget = new WareStatusItemWidget(Item);
  mp_MessageContainer->layout()->addWidget(MessageWidget);

  connect(MessageWidget, SIGNAL(actionTriggered(QString)), this, SLOT(onActionTriggered(QString))); 
}


// =====================================================================
// =====================================================================


void WareStatusDashboardWidget::clear()
{
  while (QLayoutItem* item = mp_MessageContainer->layout()->takeAt(0))
  {
    if (item->widget())
    {
      delete item->widget();
    }
    delete item;
  }
}


// =====================================================================
// =====================================================================


void WareStatusDashboardWidget::refresh()
{
  // First clean existing data
  clear();
  
  try // TOIMPL improve try/catch filtering
  {
    auto Checker = openfluid::waresdev::WareSrcChecker(m_WarePath);
    auto Report = Checker.performCheck(true);
    
    setLevel(Report.getStatus());

    // TODO check order?
    for (const auto& CatItems : Report.Categories)
    {

      for (const auto& Item : CatItems.second.Items)
      {
        if (Item.Status == ReportingData::ReportingStatus::WARNING || 
            Item.Status == ReportingData::ReportingStatus::ERROR_STATUS)
        {
          addMessage(Item);
        }
      }
    }
  }
  catch (std::exception& E)
  {
    std::string ErrorMsg = "WareSrcChecker error: " + std::string(E.what());
    setLevel(ReportingData::ReportingStatus::DISABLED);
    ui->TitleLabel->setToolTip(QString::fromStdString(ErrorMsg));
  }
}


// =====================================================================
// =====================================================================


void WareStatusDashboardWidget::onActionTriggered(const QString& ActionCode)
{
  emit operationRequested(ActionCode);
}


// =====================================================================
// =====================================================================


void WareStatusDashboardWidget::setLevel(const ReportingData::ReportingStatus Level)
{
  if (Level == ReportingData::ReportingStatus::OK)
  {
    ui->TitleLabel->setText(tr("No issue"));
    m_TitleColor = openfluid::ui::config::STATUS_OKCOLOR;
  }
  else if (Level == ReportingData::ReportingStatus::WARNING)
  {
    ui->TitleLabel->setText(tr("Warnings raised"));
    m_TitleColor = openfluid::ui::config::STATUS_WARNCOLOR;
  }
  else if (Level == ReportingData::ReportingStatus::ERROR_STATUS)
  {
    ui->TitleLabel->setText(tr("Issues detected"));
    m_TitleColor = openfluid::ui::config::STATUS_ERRCOLOR;
  }
  else
  {
    ui->TitleLabel->setText(tr("Check is disabled"));
    m_TitleColor = openfluid::ui::config::STATUS_DISABLEDCOLOR;
  }

  applyStyleSheet();
}


// =====================================================================
// =====================================================================


void WareStatusDashboardWidget::applyStyleSheet()
{
  ui->TitleLabel->setStyleSheet("color:"+m_TitleColor+"; font:bold;");
}


} } } // namespaces
