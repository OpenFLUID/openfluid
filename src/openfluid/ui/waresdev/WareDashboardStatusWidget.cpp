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
  @file WareDashboardStatusWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <QVBoxLayout>

#include <iostream>  // DIRTYCODE

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/waresdev/WareSrcChecker.hpp>
#include <openfluid/ui/waresdev/WareDashboardStatusWidget.hpp>

#include "ui_WareDashboardStatusWidget.h"


namespace openfluid { namespace ui { namespace waresdev {

WareDashboardStatusWidget::WareDashboardStatusWidget(QWidget* Parent, const std::string& WarePath):
  QWidget(Parent), ui(new Ui::WareDashboardStatusWidget), m_WarePath(WarePath), m_TitleColor("green")
{
  // TOIMPL refactor classes about dashboard

  ui->setupUi(this);

  ui->TitleLabel->setStyleSheet("background-color:white; font:bold;");
  ui->TitleLabel->setElideMode(Qt::ElideRight);

  ui->NoMessageLabel->setStyleSheet("font:italic;");
  
  ui->InnerWidget->setStyleSheet(QString("#InnerWidget "
                                        "{background-color: white;}"));
  
  ui->ContentsScrollArea->setStyleSheet("QFrame {background-color: white;}");

  ui->ContentsWidget->setStyleSheet("background-color: white;");


  QVBoxLayout* Layout = new QVBoxLayout;

  Layout->setContentsMargins(0,0,0,0);
  Layout->setSpacing(0);

  ui->ContentsWidget->setLayout(Layout);


  // TOIMPL replace by only one category widget containing all data
  std::list<std::string> OrderedCategories;
  mp_MessagesWidgets["base"] = new GroupStatusMessagesWidget("",this); // TODO remove name? remove group level?
  OrderedCategories.push_back("base");

  mp_MessagesWidgets["structure"] = new GroupStatusMessagesWidget(tr("structure"),this);
  OrderedCategories.push_back("structure");

  mp_MessagesWidgets["metainfo"] = new GroupStatusMessagesWidget(tr("metainfo"),this);
  OrderedCategories.push_back("metainfo");

  mp_MessagesWidgets["code"] = new GroupStatusMessagesWidget(tr("code"),this);
  OrderedCategories.push_back("code");

  for (const auto& c : OrderedCategories)
  {
    ((QBoxLayout*)ui->ContentsWidget->layout())->addWidget(mp_MessagesWidgets[c], 0,Qt::AlignTop);
    connect(mp_MessagesWidgets[c], SIGNAL(actionTriggered(QString)), this, SLOT(onActionTriggered(QString)));
  }

  ((QBoxLayout*)ui->ContentsWidget->layout())->addStretch();
  
  refresh();  // TOIMPL refresh at file save (connect in parent as slot) 
  // TODO (advanced: see if watcher catches external change)
}


// =====================================================================
// =====================================================================


WareDashboardStatusWidget::~WareDashboardStatusWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WareDashboardStatusWidget::refresh()
{
  try // DIRTYCODE improve try/catch filtering
  {
    // TOIMPL first clean existing data
    mp_MessagesWidgets;

    auto Checker = openfluid::waresdev::WareSrcChecker(m_WarePath); //TOIMPL check is checker for qstring/qstring?
    auto Report = Checker.performCheck(true);
    
    setLevel(Report.getStatus());

    for (const auto& CatItems : mp_MessagesWidgets) // TOIMPL refactor this to iterate on real categories from report
    {
      CatItems.second->clear();

      for (const auto& Item : Report.Categories[CatItems.first].Items)
      {
        // TOIMPL Externalize structure computation
        if (Item.Status == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::WARNING || 
            Item.Status == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::ERROR)
        {
          /*QString Message = QString::fromStdString(Item.Message);
          QString ActionCode = "";
          if (Item.Message == "version_iscorrect" && 
              Item.Status == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::ERROR)
          {
            Message = "Ware version is below current OpenFLUID version"; // TOIMPL replace by global dict?
            ActionCode = "migration";
          }*/
          //CatItems.second->addMessage(Message, Item.Status, ActionCode);
          CatItems.second->addMessage(Item);
        }
      }
    }
  }
  catch (std::exception& E)
  {
    std::cout << "WareSrcChecker error for ware " << m_WarePath << ": " << std::string(E.what()) << std::endl;
    setLevel(openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::DISABLED);
    //throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, std::string(E.what())); 
    // DIRTYCODE better way to transmit error
  }
}


// =====================================================================
// =====================================================================


void WareDashboardStatusWidget::onActionTriggered(const QString& ActionCode)
{
  if (ActionCode == "migration")
  {
    emit migrationRequested();
  }
  else
  {
    // TOIMPL Raise error saying that ActionCode is not recognized
  }
}


// =====================================================================
// =====================================================================


void WareDashboardStatusWidget::setLevel(const openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus Level)
{
  // TOIMPL change visual to remove colored background and change color of text to severity level
  if (Level == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::OK)
  {
    ui->TitleLabel->setText(tr("No issue"));
    ui->NoMessageLabel->setVisible(false); // TOIMPL totally remove widget? not if shared widget
    m_TitleColor = "#55B332";
  }
  else if (Level == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::WARNING)
  {
    ui->TitleLabel->setText(tr("Warnings raised"));
    ui->NoMessageLabel->setVisible(false);
    m_TitleColor = "#E3A740";
  }
  else if (Level == openfluid::waresdev::WareSrcChecker::ReportingData::ReportingStatus::ERROR)
  {
    ui->TitleLabel->setText(tr("Issues detected"));
    ui->NoMessageLabel->setVisible(false);
    m_TitleColor = "#B31717";
  }
  else
  {
    ui->TitleLabel->setText(tr("Check is disabled"));
    ui->NoMessageLabel->setVisible(false);
    m_TitleColor = "#757575";
  }

  applyStyleSheet();
}


// =====================================================================
// =====================================================================


void WareDashboardStatusWidget::applyStyleSheet()
{
  ui->TitleLabel->setStyleSheet("color:"+m_TitleColor+"; font:bold;");
}


} } } // namespaces
