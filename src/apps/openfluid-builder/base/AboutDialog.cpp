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
  \file AboutDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <QLabel>
#include <QPushButton>
#include <QPixmap>

#include "ActionLabel.hpp"
#include "ClickableLabel.hpp"

#include "ui_AboutDialog.h"
#include "AboutDialog.hpp"

#include "builderconfig.hpp"


AboutDialog::AboutDialog(QWidget *Parent, const QAction* WebAction, const QAction* ContactAction):
  QDialog(Parent), ui(new Ui::AboutDialog),
  mp_WebAction(WebAction), mp_ContactAction(ContactAction),
  m_InfoIsCredits(false)
{
  ui->setupUi(this);

  setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

  mp_WebLabel = new ClickableLabel("<A HREF='http://www.openfluid-project.org'>www.openfluid-project.org</A>",this);
  mp_WebLabel->setCursor(Qt::PointingHandCursor);
  mp_ContactLabel = new ClickableLabel("<A HREF='mailto:contact@openfluid-project.org'>contact@openfluid-project.org</A>",this);
  mp_ContactLabel->setCursor(Qt::PointingHandCursor);

  ui->InfosLayout->insertWidget(1,mp_WebLabel);
  ui->InfosLayout->insertWidget(2,mp_ContactLabel);

  ui->IconLabel->setPixmap(QPixmap(":/icons/openfluid_icon_about.png"));
  ui->TitleLabel->setPixmap(QPixmap(":/images/openfluid_title.png"));

  ui->IconLabel->setFocus();

  ui->ButtonBox->button(QDialogButtonBox::Close)->setCursor(Qt::PointingHandCursor);

  mp_SwitchLabel = new ActionLabel("action",this);
  mp_SwitchLabel->setAlignment(Qt::AlignBottom);
  ui->DescLayout->addWidget(mp_SwitchLabel);

  toggleInfos();

  connect(mp_WebLabel,SIGNAL(clicked()),mp_WebAction,SLOT(trigger()));
  connect(mp_ContactLabel,SIGNAL(clicked()),mp_ContactAction,SLOT(trigger()));
  connect(mp_SwitchLabel,SIGNAL(clicked()),this,SLOT(toggleInfos()));

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
    mp_SwitchLabel->setText(tr("View license"));
    ui->InfosEdit->setText(BUILDER_AUTHORS_TEXT);
    ui->InfosEdit->setStyleSheet("font-size: 13px;");
  }
  else
  {
    ui->DescLabel->setText(tr("License")+":");
    mp_SwitchLabel->setText(tr("View credits"));
    ui->InfosEdit->setStyleSheet("font-size: 12px;");
    ui->InfosEdit->setText(BUILDER_LICENSE_TEXT);
  }
}

