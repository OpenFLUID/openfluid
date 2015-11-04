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
  @file AboutDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <QLabel>
#include <QPushButton>
#include <QPixmap>

#include <openfluid/config.hpp>
#include <openfluid/ui/common/ActionLabel.hpp>
#include <openfluid/ui/common/ClickableLabel.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_AboutDialog.h"
#include "AboutDialog.hpp"


namespace openfluid { namespace ui { namespace common {


AboutDialog::AboutDialog(QWidget *Parent, const QAction* WebAction, const QAction* ContactAction):
  QDialog(Parent), ui(new Ui::AboutDialog),
  mp_WebAction(WebAction), mp_ContactAction(ContactAction),
  m_InfoIsCredits(false)
{
  ui->setupUi(this);

  setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

  ui->IconLabel->setPixmap(QPixmap(":/ui/common/images/openfluid_icon_about.png"));
  ui->TitleLabel->setPixmap(QPixmap(":/ui/common/images/openfluid_title.png"));

  ui->VersionLabel->setText(QString::fromStdString(openfluid::config::VERSION_FULL));

  ui->WebLabel->setText("<A HREF='"+QString::fromStdString(openfluid::config::WEBSITE_URL)+"'>"+
                        QString::fromStdString(openfluid::config::WEBSITE_DOMAIN)+"</A>");
  ui->WebLabel->setCursor(Qt::PointingHandCursor);

  ui->ContactLabel->setText("<A HREF='"+QString::fromStdString(openfluid::config::CONTACTEMAIL_URL)+"'>"+
                            QString::fromStdString(openfluid::config::CONTACTEMAIL_ADDRESS)+"</A>");
  ui->ContactLabel->setCursor(Qt::PointingHandCursor);

  ui->IconLabel->setFocus();

  ui->ButtonBox->button(QDialogButtonBox::Close)->setCursor(Qt::PointingHandCursor);


  toggleInfos();

  ui->BottomFrame->setStyleSheet(QString("QFrame#BottomFrame {background-color: %1;}")
                                          .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));

  connect(ui->WebLabel,SIGNAL(clicked()),mp_WebAction,SLOT(trigger()));
  connect(ui->ContactLabel,SIGNAL(clicked()),mp_ContactAction,SLOT(trigger()));
  connect(ui->SwitchLabel,SIGNAL(clicked()),this,SLOT(toggleInfos()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


AboutDialog::~AboutDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void AboutDialog::toggleInfos()
{
  ui->IconLabel->setFocus();

  m_InfoIsCredits = !m_InfoIsCredits;

  if (m_InfoIsCredits)
  {
    ui->DescLabel->setText(tr("Credits")+":");
    ui->SwitchLabel->setText(tr("View license"));
    ui->InfosEdit->setText(openfluid::ui::config::AUTHORS_TEXT);
    ui->InfosEdit->setStyleSheet("font-size: 13px;");
  }
  else
  {
    ui->DescLabel->setText(tr("License")+":");
    ui->SwitchLabel->setText(tr("View credits"));
    ui->InfosEdit->setStyleSheet("font-size: 12px;");
    ui->InfosEdit->setText(openfluid::ui::config::LICENSE_TEXT);
  }
}


} } } // namespaces

