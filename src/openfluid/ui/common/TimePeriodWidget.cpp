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
  @file TimePeriodWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_TimePeriodWidget.h"
#include "TimePeriodWidget.hpp"


namespace openfluid { namespace ui { namespace common {


TimePeriodWidget::TimePeriodWidget(QWidget* Parent):
  QWidget(Parent), ui(new Ui::TimePeriodWidget),
  m_LinkedIcon(openfluid::ui::common::getIcon("linked","/ui/common")),
  m_UnlinkedIcon(openfluid::ui::common::getIcon("unlinked","/ui/common")),
  m_LinkMode(false), m_FixedDiff(0)
{
  ui->setupUi(this);

  ui->UpCornerLabel->setPixmap(openfluid::ui::common::getImage("corner_upleft","ui/common"));
  ui->DownCornerLabel->setPixmap(openfluid::ui::common::getImage("corner_downleft","ui/common"));

  ui->LinkButton->setIcon(m_UnlinkedIcon);

  connect(ui->BeginDateEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(handleBeginChanged(const QDateTime&)));
  connect(ui->EndDateEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(handleEndChanged(const QDateTime&)));
  connect(ui->LinkButton,SIGNAL(clicked()),this,SLOT(handleLinkModeChanged()));
}


// =====================================================================
// =====================================================================


TimePeriodWidget::~TimePeriodWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void TimePeriodWidget::handleBeginChanged(const QDateTime& DT)
{
  emit beginChanged(DT);

  if (m_LinkMode)
  {
    ui->EndDateEdit->setDateTime(DT.addSecs(m_FixedDiff));
  }
  else
  {
    if (ui->EndDateEdit->dateTime() <= DT)
    {
      ui->EndDateEdit->setDateTime(DT.addSecs(1));
    }
  }
}


// =====================================================================
// =====================================================================


void TimePeriodWidget::handleEndChanged(const QDateTime& DT)
{
  emit endChanged(DT);

  if (m_LinkMode)
  {
    ui->BeginDateEdit->setDateTime(DT.addSecs(-m_FixedDiff));
  }
  else
  {
    if (ui->BeginDateEdit->dateTime() >= DT)
    {
      ui->BeginDateEdit->setDateTime(DT.addSecs(-1));
    }
  }
}


// =====================================================================
// =====================================================================


void TimePeriodWidget::initialize(const QDateTime& BeginDate, const QDateTime& EndDate)
{
  ui->BeginDateEdit->setDateTime(BeginDate);
  ui->EndDateEdit->setDateTime(EndDate);
}


// =====================================================================
// =====================================================================


QDateTime TimePeriodWidget::getBeginDateTime() const
{
  return ui->BeginDateEdit->dateTime();
}


// =====================================================================
// =====================================================================


void TimePeriodWidget::setBeginDateTime(const QDateTime& DT)
{
  ui->BeginDateEdit->setDateTime(DT);
}


// =====================================================================
// =====================================================================


QDateTime TimePeriodWidget::getEndDateTime() const
{
  return ui->EndDateEdit->dateTime();
}


// =====================================================================
// =====================================================================


void TimePeriodWidget::setEndDateTime(const QDateTime& DT)
{
  ui->EndDateEdit->setDateTime(DT);
}


// =====================================================================
// =====================================================================


void TimePeriodWidget::handleLinkModeChanged()
{
  m_LinkMode = !m_LinkMode;

  if (m_LinkMode)
  {
    m_FixedDiff = ui->BeginDateEdit->dateTime().secsTo(ui->EndDateEdit->dateTime());
    ui->LinkButton->setIcon(m_LinkedIcon);
  }
  else
  {
    m_FixedDiff = 0;
    ui->LinkButton->setIcon(m_UnlinkedIcon);
  }

  emit linkModeChanged(m_LinkMode);
}


} } } // namespaces

