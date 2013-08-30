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
#include <QVBoxLayout>
#include <QFrame>
#include <QDir>

#include "HomeWidget.hpp"
#include "AppActions.hpp"


#include <openfluid/config.hpp>



// =====================================================================
// =====================================================================


void RecentProjectLabel::enterEvent(QEvent* /*Event*/)
{
  setStyleSheet("text-decoration : underline;");
}


// =====================================================================
// =====================================================================


void RecentProjectLabel::leaveEvent(QEvent* /*Event*/)
{
  setStyleSheet("text-decoration : none;");
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
  QWidget(Parent)
{

  QVBoxLayout* MainLayout;

  QFrame* UpperPart = new QFrame(this);
  UpperPart->setStyleSheet("QFrame { background-color: white; }");
  UpperPart->setMinimumSize(300,270);
  UpperPart->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);


  QLabel* OFLogo = new QLabel(this);
  QPixmap OFpix(":/images/openfluid_official.png");
  OFLogo->setPixmap(OFpix);
  OFLogo->setAlignment(Qt::AlignCenter);

  QLabel* Version = new QLabel(std::string("OpenFLUID v"+openfluid::config::FULL_VERSION).c_str(),this);
  Version->setAlignment(Qt::AlignCenter);

  ClickableLabel* URL = new ClickableLabel("<a href=\"http://www.openfluid-project.org\">http://www.openfluid-project.org</a>",this);
  URL->setAlignment(Qt::AlignCenter);
  connect(URL,SIGNAL(clicked()),Actions->getAction("HelpOnlineWeb"),SLOT(trigger()));


  QFrame* HLine = new QFrame(this);
  HLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
  HLine->setLineWidth(1);


  QVBoxLayout* UpLayout = new QVBoxLayout(this);
  UpLayout->addWidget(OFLogo);
  UpLayout->addWidget(Version);
  UpLayout->addWidget(URL);
  UpperPart->setLayout(UpLayout);


  /// left buttons

  QFrame* Buttons = new QFrame(this);
  Buttons->setMinimumSize(380,150);
  Buttons->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

  QVBoxLayout* ButtonsLayout = new QVBoxLayout(this);

  QPushButton* TheButton;
  TheButton = createButton(Actions->getAction("ProjectNew"),tr("Create a project..."));
  ButtonsLayout->addWidget(TheButton);

  TheButton = createButton(Actions->getAction("ProjectOpen"),tr("Open a project..."));
  ButtonsLayout->addWidget(TheButton);

  QFrame* ButtonsHLine = new QFrame(this);
  ButtonsHLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
  ButtonsHLine->setLineWidth(1);
  ButtonsLayout->addWidget(ButtonsHLine);

  TheButton = createButton(Actions->getAction("HelpExamplesOpen"),tr("Open an example project..."));
  ButtonsLayout->addWidget(TheButton);

  ButtonsHLine = new QFrame(this);
  ButtonsHLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
  ButtonsHLine->setLineWidth(1);
  ButtonsLayout->addWidget(ButtonsHLine);

  TheButton = createButton(Actions->getAction("MarketAccess"),tr("Access to OpenFLUID-Market..."));
  ButtonsLayout->addWidget(TheButton);



  ButtonsLayout->setAlignment(Qt::AlignTop);
  Buttons->setLayout(ButtonsLayout);



  QFrame* VLine = new QFrame(this);
  VLine->setFrameStyle(QFrame::VLine | QFrame::Sunken);
  VLine->setLineWidth(1);


  // Recent projects
  QFrame* Recent = new QFrame(this);
  QVBoxLayout *RecentLayout = new QVBoxLayout(this);

  std::vector<QAction*> RecentActions = Actions->getRecentProjectActions();

  QLabel* RecentProjectsLabel = new QLabel(tr("Recent projects:"),this);
  RecentProjectsLabel->setStyleSheet("font : bold;");

  if (!RecentActions[0]->isVisible())
    RecentProjectsLabel->setText(tr("No recent project"));

  RecentLayout->addWidget(RecentProjectsLabel);

  QPixmap DotPix(":/images/dot.png");

  for (unsigned int i=0; i<RecentActions.size();i++)
  {
    if (RecentActions[i]->isVisible())
    {
      // the local layout will be given a correct parent below with the addLayout() command
      QHBoxLayout* LocalLayout = new QHBoxLayout(NULL);
      QLabel* DotLabel = new QLabel(this);
      DotLabel->setPixmap(DotPix);
      DotLabel->setAlignment(Qt::AlignCenter);
      LocalLayout->addWidget(DotLabel);
      LocalLayout->setContentsMargins(30,0,0,0);

      RecentProjectLabel* RecentLabel = new RecentProjectLabel(QDir(RecentActions[i]->data().toString()).dirName(),this);
      RecentLabel->setContentsMargins(0,0,0,0);
      RecentLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
      // TODO set the tooltip with icon and project info, with adapted stylesheet
      // RecentLabel->setToolTip(RecentActions[i]->data().toString());
      RecentLabel->setCursor(Qt::PointingHandCursor);
      connect(RecentLabel,SIGNAL(clicked()),RecentActions[i],SLOT(trigger()));

      LocalLayout->addWidget(RecentLabel);
      LocalLayout->addStretch();
      RecentLayout->addLayout(LocalLayout);
    }
  }
  RecentLayout->addStretch();

  RecentLayout->setContentsMargins(30,30,30,30);
  RecentLayout->setSpacing(10);
  Recent->setLayout(RecentLayout);


  QFrame* LowerPart = new QFrame(this);
  QHBoxLayout* LowLayout = new QHBoxLayout(this);
  LowLayout->setContentsMargins(0,0,0,0);
  LowLayout->setSpacing(0);

  LowLayout->addWidget(Buttons);
  LowLayout->addWidget(VLine);
  LowLayout->addWidget(Recent);
  LowerPart->setLayout(LowLayout);


  MainLayout = new QVBoxLayout(this);
  MainLayout->addWidget(UpperPart);
  MainLayout->addWidget(HLine);
  MainLayout->addWidget(LowerPart);
  setLayout(MainLayout);

  layout()->setContentsMargins(0,0,0,0);
  layout()->setSpacing(0);
}


// =====================================================================
// =====================================================================


HomeWidget::~HomeWidget()
{

}


// =====================================================================
// =====================================================================


QPushButton* HomeWidget::createButton(const QAction* Action, const QString& Text)
{
  QPushButton* Button = new QPushButton(Text,this);
  Button->setMinimumHeight(65);
  Button->setIconSize(QSize(48,48));
  Button->setIcon(Action->icon());
//  Button->setStyleSheet("text-align: left; padding-left : 10px;");

  connect(Button,SIGNAL(clicked()),Action,SLOT(trigger()));

  return Button;
}
