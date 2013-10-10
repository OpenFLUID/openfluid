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
  \file SpatialDomainWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include "ui_SpatialDomainWidget.h"
#include "SpatialDomainWidget.hpp"
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>


SpatialDomainWidget::SpatialDomainWidget(QWidget* Parent, openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc):
  WorkspaceWidget(Parent,AFXDesc), ui(new Ui::SpatialDomainWidget), m_Domain(AFXDesc.getDomain())
{
  ui->setupUi(this);

  connect(ui->UnitsTableWidget->horizontalHeader(),SIGNAL(sectionResized(int, int, int)),
          ui->UnitsTableWidget,SLOT(resizeRowsToContents()));

  refresh();
}


// =====================================================================
// =====================================================================


SpatialDomainWidget::~SpatialDomainWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void SpatialDomainWidget::refresh()
{
  ui->UnitsTableWidget->setRowCount(0);

  ui->UnitsTableWidget->setRowCount(m_Domain.getUnitsCount());

  int CurrentRow = 0;

  std::set<std::string> Classes = m_Domain.getClassNames();

  for (std::set<std::string>::iterator itc = Classes.begin(); itc != Classes.end(); ++itc)
  {
    std::set<int> IDs = m_Domain.getIDsOfClass(*itc);

    for (std::set<int>::iterator iti = IDs.begin(); iti != IDs.end(); ++iti)
    {
      QTableWidgetItem *Item;

      // ======== ID, class, process order

      Item = new QTableWidgetItem(QString::fromStdString(*itc));
      ui->UnitsTableWidget->setItem(CurrentRow, 0, Item);

      Item = new QTableWidgetItem(QString::fromStdString("%1").arg(*iti));
      ui->UnitsTableWidget->setItem(CurrentRow, 1, Item);

      const openfluid::fluidx::AdvancedUnitDescriptor& UnitDesc = m_Domain.getUnit(*itc,*iti);

      Item = new QTableWidgetItem(QString("%1").arg(UnitDesc.UnitDescriptor->getProcessOrder()));
      ui->UnitsTableWidget->setItem(CurrentRow, 2, Item);


      // ======== Connections
      std::list<openfluid::core::UnitClassID_t>::iterator itconn;
      std::list<openfluid::core::UnitClassID_t> ConnUnits;
      QString ConnStr;


      // to connections
      ConnUnits = m_Domain.getUnitsToOf(make_pair(*itc,*iti));
      ConnStr.clear();

      for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
      {
        if (itconn != ConnUnits.begin()) ConnStr += ", ";
        ConnStr += QString::fromStdString((*itconn).first) + QString("#%1").arg((*itconn).second);
      }

      Item = new QTableWidgetItem(ConnStr);
      ui->UnitsTableWidget->setItem(CurrentRow, 3, Item);


      // from connections
      ConnUnits = m_Domain.getUnitsFromOf(make_pair(*itc,*iti));
      ConnStr.clear();

      for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
      {
        if (itconn != ConnUnits.begin()) ConnStr += ", ";
        ConnStr += QString::fromStdString((*itconn).first) + QString("#%1").arg((*itconn).second);
      }

      Item = new QTableWidgetItem(ConnStr);
      ui->UnitsTableWidget->setItem(CurrentRow, 4, Item);


      // parent connections
      ConnUnits = m_Domain.getUnitsParentsOf(make_pair(*itc,*iti));
      ConnStr.clear();

      for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
      {
        if (itconn != ConnUnits.begin()) ConnStr += ", ";
        ConnStr += QString::fromStdString((*itconn).first) + QString("#%1").arg((*itconn).second);
      }

      Item = new QTableWidgetItem(ConnStr);
      ui->UnitsTableWidget->setItem(CurrentRow, 5, Item);


      // children connections
      ConnUnits = m_Domain.getUnitsChildrenOf(make_pair(*itc,*iti));
      ConnStr.clear();

      for (itconn = ConnUnits.begin(); itconn != ConnUnits.end(); ++itconn)
      {
        if (itconn != ConnUnits.begin()) ConnStr += ", ";
        ConnStr += QString::fromStdString((*itconn).first) + QString("#%1").arg((*itconn).second);
      }

      Item = new QTableWidgetItem(ConnStr);
      ui->UnitsTableWidget->setItem(CurrentRow, 6, Item);


      // ======== Attributes

      std::map<openfluid::core::AttributeName_t, std::string*> Attrs = UnitDesc.Attributes;

      QString AttrsStr;

      for (std::map<openfluid::core::AttributeName_t, std::string*>::iterator ita = Attrs.begin();
          ita != Attrs.end(); ++ita)
      {
        AttrsStr += QString::fromStdString((*ita).first) + "={" + QString::fromStdString(*((*ita).second))+"} ";
      }

      Item = new QTableWidgetItem(AttrsStr);
      ui->UnitsTableWidget->setItem(CurrentRow, 7, Item);

      CurrentRow++;
    }
  }

  ui->UnitsTableWidget->resizeRowsToContents();
}
