/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file HomeWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <QLabel>
#include <QFrame>
#include <QDir>

#include "ui_HomeWidget.h"
#include "HomeWidget.hpp"
#include "AppActions.hpp"
#include "AppTools.hpp"

#include <openfluid/guicommon/PreferencesManager.hpp>
#include <openfluid/config.hpp>



// =====================================================================
// =====================================================================


void RecentProjectLabel::enterEvent(QEvent* /*Event*/)
{
  // TODO check why underline does not work on QLabel
  setStyleSheet("QLabel {text-decoration : underline;} QToolTip { text-decoration : none; padding: 5px; }");
}


// =====================================================================
// =====================================================================


void RecentProjectLabel::leaveEvent(QEvent* /*Event*/)
{
  setStyleSheet("QLabel {text-decoration : none;} QToolTip { text-decoration : none; padding: 5px; }");
}


// =====================================================================
// =====================================================================


RecentProjectLabel::RecentProjectLabel(const QString& Text, QWidget* Parent):
  ClickableLabel(Text,Parent)
{

}


// =====================================================================
// =====================================================================


HomeWidget::HomeWidget(QWidget* Parent, const AppActions* Actions):
  QWidget(Parent), ui(new Ui::HomeWidget), mp_Actions(Actions)
{
  ui->setupUi(this);

  ui->ImageLabel->setText("");
  ui->ImageLabel->setPixmap(QPixmap(":/images/openfluid_official.png"));

  ui->VersionLabel->setText(std::string("OpenFLUID v"+openfluid::config::FULL_VERSION).c_str());

  ui->URLLabel->setText("<a href=\"http://www.openfluid-project.org\">http://www.openfluid-project.org</a>");
  connect(ui->URLLabel,SIGNAL(clicked()),Actions->getAction("HelpOnlineWeb"),SLOT(trigger()));

  // left buttons

  QVBoxLayout* ButtonsLayout = new QVBoxLayout(ui->ButtonsFrame);

  QPushButton* TheButton;
  TheButton = createButton(Actions->getAction("ProjectNew"),tr("Create a project..."));
  ButtonsLayout->addWidget(TheButton);

  TheButton = createButton(Actions->getAction("ProjectOpen"),tr("Open a project..."));
  ButtonsLayout->addWidget(TheButton);

  QFrame* ButtonsHLine = new QFrame(ui->ButtonsFrame);
  ButtonsHLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
  ButtonsHLine->setLineWidth(1);
  ButtonsLayout->addWidget(ButtonsHLine);

  TheButton = createButton(Actions->getAction("HelpExamplesOpen"),tr("Open an example project..."));
  ButtonsLayout->addWidget(TheButton);

  ButtonsHLine = new QFrame(ui->ButtonsFrame);
  ButtonsHLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
  ButtonsHLine->setLineWidth(1);
  ButtonsLayout->addWidget(ButtonsHLine);

  TheButton = createButton(Actions->getAction("MarketAccess"),tr("Access to OpenFLUID-Market..."));
  ButtonsLayout->addWidget(TheButton);

  ButtonsLayout->setAlignment(Qt::AlignTop);
  ui->ButtonsFrame->setLayout(ButtonsLayout);


  // Recent projects
  mp_RecentsLayout = new QVBoxLayout(ui->RecentsFrame);

  mp_RecentProjectsLabel = new QLabel("",ui->RecentsFrame);
  mp_RecentProjectsLabel->setStyleSheet("font : bold;");
  mp_RecentsLayout->addWidget(mp_RecentProjectsLabel);

  QPixmap DotPix(":/images/dot.png");

  std::vector<QAction*> RecentActions = mp_Actions->getRecentProjectActions();

  for (int i=0; i<openfluid::guicommon::PreferencesManager::RecentProjectsLimit;i++)
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


HomeWidget::~HomeWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


QPushButton* HomeWidget::createButton(const QAction* Action, const QString& Text)
{
  QPushButton* Button = new QPushButton(Text,ui->ButtonsFrame);
  Button->setMinimumHeight(65);
  Button->setIconSize(QSize(48,48));
  Button->setIcon(Action->icon());

  connect(Button,SIGNAL(clicked()),Action,SLOT(trigger()));

  return Button;
}


// =====================================================================
// =====================================================================


void HomeWidget::refreshRecentProjects()
{
  std::vector<QAction*> RecentActions = mp_Actions->getRecentProjectActions();

  for (int i=0; i<openfluid::guicommon::PreferencesManager::RecentProjectsLimit;i++)
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
        dynamic_cast<QLabel*>(ItemLayout->itemAt(1)->widget())->setText(QDir(RecentActions[i]->data().toString()).dirName());

        QString InfosStr = getProjectInfosAsHTML(RecentActions[i]->data().toString(),true);
        if (!InfosStr.isEmpty()) ItemLayout->itemAt(1)->widget()->setToolTip(InfosStr);

        ItemLayout->itemAt(0)->widget()->setVisible(true);
        ItemLayout->itemAt(1)->widget()->setVisible(true);
      }
    }
  }
}
