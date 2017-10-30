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
  @file WaresManagementWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_WaresManagementWidget.h"
#include "WaresManagementWidget.hpp"
#include "WareWidget.hpp"


WaresManagementWidget::WaresManagementWidget(QWidget* Parent,
                                             bool WithSecondAddWareButton):
  QWidget(Parent),ui(new Ui::WaresManagementWidget)
{
  ui->setupUi(this);

  mp_ExpandAllWaresLabel = new openfluid::ui::common::ActionLabel(tr("expand all"),this);
  ui->WaresButtonsLayout->addWidget(mp_ExpandAllWaresLabel);
  connect(mp_ExpandAllWaresLabel,SIGNAL(clicked()),this,SLOT(expandAll()));

  mp_CollapseAllWaresLabel = new openfluid::ui::common::ActionLabel(tr("collapse all"),this);
  ui->WaresButtonsLayout->addWidget(mp_CollapseAllWaresLabel);
  connect(mp_CollapseAllWaresLabel,SIGNAL(clicked()),this,SLOT(collapseAll()));


  ui->AddWareFirstButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddWareFirstButton->setIconSize(QSize(20,20));

  ui->AddWareSecondButton->setVisible(WithSecondAddWareButton);

  if (WithSecondAddWareButton)
  {
    ui->AddWareSecondButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
    ui->AddWareSecondButton->setIconSize(QSize(20,20));

  }
}


// =====================================================================
// =====================================================================


WaresManagementWidget::~WaresManagementWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WaresManagementWidget::expandAll()
{
  for (int i=0;i<ui->WaresListAreaContents->layout()->count();i++)
  {
    WareWidget* W = static_cast<WareWidget*>(ui->WaresListAreaContents->layout()->itemAt(i)->widget());
    if (W != 0)
    {
      W->setExpanded(true);
      W->displayParams();
    }
  }
}


// =====================================================================
// =====================================================================


void WaresManagementWidget::collapseAll()
{
  for (int i=0;i<ui->WaresListAreaContents->layout()->count();i++)
  {
    WareWidget* W = static_cast<WareWidget*>(ui->WaresListAreaContents->layout()->itemAt(i)->widget());
    if (W != 0)
    {
      W->setExpanded(false);
      W->displayParams();
    }
  }
}


// =====================================================================
// =====================================================================


void WaresManagementWidget::updateIndexesAndButtons(bool WithFinalStretch)
{
  int LastIndex = ui->WaresListAreaContents->layout()->count()-WithFinalStretch-1;

  for (int i=0;i<=LastIndex;i++)
  {
    WareWidget* W = static_cast<WareWidget*>(ui->WaresListAreaContents->layout()->itemAt(i)->widget());

    W->setCurrentIndex(i);

    if (i==0)
    {
      W->setUpButtonEnabled(false);
      W->setDownButtonEnabled(true);
    }
    else if (i==LastIndex)
    {
      W->setUpButtonEnabled(true);
      W->setDownButtonEnabled(false);
    }
    else
    {
      W->setUpButtonEnabled(true);
      W->setDownButtonEnabled(true);
    }
  }
}


