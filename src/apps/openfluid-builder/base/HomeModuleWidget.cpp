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
  @file HomeModuleWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <QDir>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/ui/market/MarketClientAssistant.hpp>
#include <openfluid/ui/common/PreferencesDialog.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_HomeModuleWidget.h"
#include "HomeModuleWidget.hpp"
#include "RecentProjectLabel.hpp"
#include "AppActions.hpp"
#include "AppTools.hpp"
#include "ExtensionPluginsManager.hpp"


HomeModuleWidget::HomeModuleWidget(const AppActions* Actions, QWidget* Parent):
  AbstractModuleWidget(Parent),ui(new Ui::HomeModuleWidget), mp_Actions(Actions)
{
  ui->setupUi(this);

  ui->ImageLabel->setText("");
  ui->ImageLabel->setPixmap(QPixmap(":/builder/images/openfluid_official.png"));

  ui->VersionLabel->setText(std::string("OpenFLUID v"+openfluid::config::VERSION_FULL).c_str());

  ui->URLLabel->setText("<a href=\"http://www.openfluid-project.org\">http://www.openfluid-project.org</a>");
  connect(ui->URLLabel,SIGNAL(clicked()),Actions->action("HelpOnlineWeb"),SLOT(trigger()));

  // left buttons

  QPushButton* TheButton;
  QVBoxLayout* ButtonsLayout = new QVBoxLayout(ui->ButtonsFrame);

  TheButton = createButton(Actions->action("ProjectNew"),tr("Create a project..."),
                           openfluid::ui::common::getIcon("project-new","/ui/common"));
  ButtonsLayout->addWidget(TheButton);

  TheButton = createButton(Actions->action("ProjectOpen"),tr("Open a project..."),
                           openfluid::ui::common::getIcon("project-open","/ui/common"));
  ButtonsLayout->addWidget(TheButton);

  QFrame* ButtonsHLine = new QFrame(ui->ButtonsFrame);
  ButtonsHLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
  ButtonsHLine->setLineWidth(1);
  ButtonsLayout->addWidget(ButtonsHLine);

  TheButton = createButton(Actions->action("HelpExamplesOpen"),tr("Open an example project..."),
                           openfluid::ui::common::getIcon("project-open-example","/builder"));
  ButtonsLayout->addWidget(TheButton);

  ButtonsHLine = new QFrame(ui->ButtonsFrame);
  ButtonsHLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
  ButtonsHLine->setLineWidth(1);

#if defined(ENABLE_MARKET_INTEGRATION) || defined(ENABLE_WARESDEV_INTEGRATION)
  ButtonsLayout->addWidget(ButtonsHLine);
#endif

#ifdef ENABLE_MARKET_INTEGRATION
  TheButton = createButton(Actions->action("MarketAccess"),tr("Access to OpenFLUID-Market..."),
                           openfluid::ui::common::getIcon("market","/builder"));
  ButtonsLayout->addWidget(TheButton);
#endif

#ifdef ENABLE_WARESDEV_INTEGRATION
  TheButton = createButton(Actions->action("DevLaunchDevStudio"),tr("Launch OpenFLUID-DevStudio..."),
                           openfluid::ui::common::getIcon("dev-wares","/builder"));
  ButtonsLayout->addWidget(TheButton);
#endif

  ButtonsLayout->setAlignment(Qt::AlignTop);
  ui->ButtonsFrame->setLayout(ButtonsLayout);


  // Recent projects
  mp_RecentsLayout = new QVBoxLayout(ui->RecentsFrame);

  mp_RecentProjectsLabel = new QLabel("",ui->RecentsFrame);
  mp_RecentProjectsLabel->setStyleSheet("font : bold;");
  mp_RecentsLayout->addWidget(mp_RecentProjectsLabel);

  QPixmap DotPix(":/builder/images/dot.png");

  std::vector<QAction*> RecentActions = mp_Actions->recentProjectActions();

  for (int i=0; i<openfluid::base::PreferencesManager::RecentProjectsLimit;i++)
  {
    // the local layout will be given a correct parent below with the addLayout() command
    QHBoxLayout* LocalLayout = new QHBoxLayout();
    QLabel* DotLabel = new QLabel(ui->RecentsFrame);
    DotLabel->setPixmap(DotPix);
    DotLabel->setAlignment(Qt::AlignCenter);
    DotLabel->setVisible(false);

    LocalLayout->addWidget(DotLabel);
    LocalLayout->setContentsMargins(30,0,0,0);

    RecentProjectLabel* RecentLabel = new RecentProjectLabel("");
    RecentLabel->setContentsMargins(0,0,0,0);
    RecentLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    RecentLabel->setVisible(false);

    RecentLabel->setCursor(Qt::PointingHandCursor);
    connect(RecentLabel,SIGNAL(clicked()),RecentActions[i],SLOT(trigger()));

    LocalLayout->addWidget(RecentLabel);
    LocalLayout->addStretch();
    mp_RecentsLayout->addLayout(LocalLayout);
  }

  mp_RecentsLayout->addStretch();
  mp_RecentsLayout->setContentsMargins(30,30,30,30);
  mp_RecentsLayout->setSpacing(10);

  ui->RecentsFrame->setLayout(mp_RecentsLayout);

  refreshRecentProjects();

  QList<int> SplitSizes;
  SplitSizes.append(2);
  SplitSizes.append(1);

  //ui->RecentsNewsSplitter->setSizes(SplitSizes);
  ui->RecentsNewsSplitter->setStretchFactor(0, 2);
  ui->RecentsNewsSplitter->setStretchFactor(1, 1);
}


// =====================================================================
// =====================================================================


HomeModuleWidget::~HomeModuleWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


bool HomeModuleWidget::whenQuitAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


bool HomeModuleWidget::whenNewAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


bool HomeModuleWidget::whenOpenAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


bool HomeModuleWidget::whenReloadAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenSaveAsked()
{

}


// =====================================================================
// =====================================================================


bool HomeModuleWidget::whenSaveAsAsked()
{
  return false;
}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenPropertiesAsked()
{

}


// =====================================================================
// =====================================================================


bool HomeModuleWidget::whenCloseAsked()
{
  return false;
}


// =====================================================================
// =====================================================================


bool HomeModuleWidget::whenPreferencesAsked()
{
  openfluid::ui::common::PreferencesDialog PrefsDlg(QApplication::activeWindow(),
                                                    openfluid::ui::common::PreferencesDialog::MODE_BUILDER);

  PrefsDlg.exec();

  return PrefsDlg.isRestartRequired();
}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenRecentProjectsActionsChanged()
{
  refreshRecentProjects();
}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenRunAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenExtensionAsked(const QString& /*ID*/)
{

}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenMarketAsked()
{
#if defined(ENABLE_MARKET_INTEGRATION)
  openfluid::ui::market::MarketClientAssistant MarketAssistant(QApplication::activeWindow());
  MarketAssistant.exec();
#endif
}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenWaresRefreshAsked()
{

}


// =====================================================================
// =====================================================================


bool HomeModuleWidget::whenOpenExampleAsked()
{
  return true;
}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenNewSimulatorSrcAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenNewGhostSimulatorAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenOpenSimulatorSrcAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenNewObserverSrcAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenOpenObserverSrcAsked()
{

}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenLaunchDevStudioAsked()
{
  launchDevStudio();
}


// =====================================================================
// =====================================================================


void HomeModuleWidget::whenSrcEditAsked(const QString& /*ID*/,openfluid::ware::WareType /*WType*/,
                                        bool /*Ghost*/)
{

}


// =====================================================================
// =====================================================================


QPushButton* HomeModuleWidget::createButton(const QAction* Action, const QString& Text, const QIcon& AltIcon)
{
  QPushButton* Button = new QPushButton(Text,ui->ButtonsFrame);
  Button->setMinimumHeight(65);
  Button->setIconSize(QSize(48,48));
  Button->setStyleSheet("text-align:left; padding-left : 20px");
  if (AltIcon.isNull())
    Button->setIcon(Action->icon());
  else
    Button->setIcon(AltIcon);

  connect(Button,SIGNAL(clicked()),Action,SLOT(trigger()));

  return Button;
}


// =====================================================================
// =====================================================================


void HomeModuleWidget::refreshRecentProjects()
{
  std::vector<QAction*> RecentActions = mp_Actions->recentProjectActions();

  for (int i=0; i<openfluid::base::PreferencesManager::RecentProjectsLimit;i++)
  {
    QLayoutItem* Item = mp_RecentsLayout->itemAt(i+1);
    QLayout* ItemLayout = Item->layout();

    ItemLayout->itemAt(0)->widget()->setVisible(false);
    ItemLayout->itemAt(1)->widget()->setVisible(false);
    dynamic_cast<QLabel*>(ItemLayout->itemAt(1)->widget())->setText("");
    ItemLayout->itemAt(1)->widget()->setToolTip("");
  }

  if (!RecentActions[0]->isVisible())
    mp_RecentProjectsLabel->setText(QString("<big>%1</big>").arg(tr("No recent project")));
  else
  {
    mp_RecentProjectsLabel->setText(QString("<big>%1</big>").arg(tr("Recent projects:")));

    for (unsigned int i=0; i<RecentActions.size();i++)
    {
      QLayoutItem* Item = mp_RecentsLayout->itemAt(i+1);
      QLayout* ItemLayout = Item->layout();

      if (RecentActions[i]->isVisible())
      {
        dynamic_cast<QLabel*>(ItemLayout->itemAt(1)->widget())
            ->setText(QDir(RecentActions[i]->data().toString()).dirName());

        QString InfosStr = getProjectInfosAsHTML(RecentActions[i]->data().toString(),true);
        if (!InfosStr.isEmpty()) ItemLayout->itemAt(1)->widget()->setToolTip(InfosStr);

        ItemLayout->itemAt(0)->widget()->setVisible(true);
        ItemLayout->itemAt(1)->widget()->setVisible(true);
      }
    }
  }
}

