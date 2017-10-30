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
  @file EditEventDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */



#include "ui_EditEventDialog.h"
#include "EditEventDialog.hpp"


EditEventDialog::EditEventDialog(const QString& ClassName, const QStringList& IDsList,
                                 const QString& CurrentUnitID,
                                 const openfluid::core::DateTime& CurrentDateTime,
                                 const openfluid::core::Event::EventInfosMap_t& CurrentInfos,
                                 QWidget* Parent):
  AddEventDialog(ClassName,IDsList,CurrentDateTime,Parent)
{
  m_DefaultMsg = tr("Edit of an event");

  ui->UnitIDComboBox->setCurrentIndex(ui->UnitIDComboBox->findText(CurrentUnitID));

  openfluid::core::Event::EventInfosMap_t::const_iterator it;
  openfluid::core::Event::EventInfosMap_t::const_iterator itb = CurrentInfos.begin();
  openfluid::core::Event::EventInfosMap_t::const_iterator ite = CurrentInfos.end();

  for (it=itb;it!=ite;++it)
  {
    int RowCount = ui->InfosTableWidget->rowCount();

    ui->InfosTableWidget->setRowCount(RowCount+1);

    ui->InfosTableWidget->setItem(RowCount,0,new QTableWidgetItem(QString::fromStdString((*it).first)));
    ui->InfosTableWidget->setItem(RowCount,1,new QTableWidgetItem(QString::fromStdString((*it).second)));
  }

  checkGlobal();
}

