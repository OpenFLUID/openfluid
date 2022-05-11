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
  @file AddSimulatorDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPushButton>

#include <openfluid/ui/config.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>

#include "ui_AddWareDialog.h"
#include "AddSimulatorDialog.hpp"
#include "SignatureWidget.hpp"


AddSimulatorDialog::AddSimulatorDialog(const QStringList& SimIDList, QWidget* Parent) :
  AddWareDialog(Parent)
{
  updateDefaultMessage(tr("Add simulator"));

  const auto& Containers = openfluid::machine::SimulatorRegistry::instance()->availableWares();

  for (auto it = Containers.begin(); it != Containers.end(); ++it)
  {
    QString ID = QString::fromStdString(it->second.signature()->ID);

    QListWidgetItem* Item = new QListWidgetItem();

    QStringList WareInfos = {
      ID,
      QString::fromStdString(it->second.signature()->Name)
    };

    Item->setData(Qt::UserRole,WareInfos);

    if (SimIDList.contains(ID))
    {
      Item->setFlags(Qt::ItemIsSelectable);
    }

    if (it->second.isGhost())
    {
      Item->setIcon(QIcon(":/builder/images/ware-sim-ghost.png"));
    }
    else
    {
      Item->setIcon(QIcon(":/builder/images/ware-sim-plugged.png"));
    }

    ui->WaresListWidget->addItem(Item);
  }

  labelizeItems(ui->UseNameCheckBox->isChecked());

  connect(ui->WaresListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(updateSignature()));
  connect(ui->UseNameCheckBox,SIGNAL(toggled(bool)),this,SLOT(labelizeItems(bool)));

  setMessage(tr("Select simulator to add"));
}


// =====================================================================
// =====================================================================


AddSimulatorDialog::~AddSimulatorDialog()
{

}


// =====================================================================
// =====================================================================


void AddSimulatorDialog::updateSignature()
{
  const auto& Container = openfluid::machine::SimulatorRegistry::instance()->wareContainer(
    ui->WaresListWidget->currentItem()->data(Qt::UserRole).toStringList().at(0).toStdString()
  );

  if (Container.isValid() && Container.hasSignature())
  {
    ui->WareSignatureWidget->update(Container);
  }

  setMessage();
}

