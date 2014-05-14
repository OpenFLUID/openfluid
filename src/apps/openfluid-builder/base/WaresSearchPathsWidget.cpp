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
  \file WaresSearchPathsWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_WaresSearchPathsWidget.h"
#include "WaresSearchPathsWidget.hpp"


WaresSearchPathsWidget::WaresSearchPathsWidget(QWidget* Parent):
  QWidget(Parent), ui(new Ui::WaresSearchPathsWidget)
{
  ui->setupUi(this);

  ui->AddButton->setText("");
  ui->AddButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddButton->setIconSize(QSize(20,20));

  ui->RemoveButton->setText("");
  ui->RemoveButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveButton->setIconSize(QSize(20,20));

  ui->UpButton->setText("");
  ui->UpButton->setIcon(QIcon(":/icons/go-up.png"));
  ui->UpButton->setIconSize(QSize(20,20));

  ui->DownButton->setText("");
  ui->DownButton->setIcon(QIcon(":/icons/go-down.png"));
  ui->DownButton->setIconSize(QSize(20,20));

}


// =====================================================================
// =====================================================================


WaresSearchPathsWidget::~WaresSearchPathsWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WaresSearchPathsWidget::initialize(const QStringList& UserPaths, const QStringList& PredefPaths)
{
  for (int i=0;i<UserPaths.size();++i)
    new QListWidgetItem(UserPaths[i],ui->UserListWidget);

  for (int i=0;i<PredefPaths.size();++i)
    new QListWidgetItem(PredefPaths[i],ui->PredefListWidget);
}
