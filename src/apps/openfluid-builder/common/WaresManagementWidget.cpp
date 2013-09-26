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
  \file WaresManagementWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_WaresManagementWidget.h"
#include "WaresManagementWidget.hpp"
#include "WareWidget.hpp"


WaresManagementWidget::WaresManagementWidget(QWidget* Parent,
                                             bool WithSecondAddWareButton):
  QWidget(Parent),ui(new Ui::WaresManagementWidget)
{
  ui->setupUi(this);

  mp_ExpandAllWaresLabel = new ActionLabel(tr("expand all"),this);
  ui->WaresButtonsLayout->addWidget(mp_ExpandAllWaresLabel);
  connect(mp_ExpandAllWaresLabel,SIGNAL(clicked()),this,SLOT(expandAll()));

  mp_CollapseAllWaresLabel = new ActionLabel(tr("collapse all"),this);
  ui->WaresButtonsLayout->addWidget(mp_CollapseAllWaresLabel);
  connect(mp_CollapseAllWaresLabel,SIGNAL(clicked()),this,SLOT(collapseAll()));


  ui->AddWareFirstButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddWareFirstButton->setIconSize(QSize(20,20));

  ui->AddWareSecondButton->setVisible(WithSecondAddWareButton);

  if (WithSecondAddWareButton)
  {
    ui->AddWareSecondButton->setIcon(QIcon(":/icons/add.png"));
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
    WareWidget* W = (WareWidget*)(ui->WaresListAreaContents->layout()->itemAt(i)->widget());
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
    WareWidget* W = (WareWidget*)(ui->WaresListAreaContents->layout()->itemAt(i)->widget());
    if (W != 0)
    {
      W->setExpanded(false);
      W->displayParams();
    }
  }
}


// =====================================================================
// =====================================================================


void WaresManagementWidget::updateUpDownButtons(bool WithFinalStretch)
{
  int LastIndex = ui->WaresListAreaContents->layout()->count()-WithFinalStretch-1;

  for (int i=0;i<=LastIndex;i++)
  {
    WareWidget* W = (WareWidget*)(ui->WaresListAreaContents->layout()->itemAt(i)->widget());
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


