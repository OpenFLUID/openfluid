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
  @file ParamsWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <iostream>


#include "ParamsWidget.hpp"
#include "ui_ParamsWidget.h"



ParamsWidget::ParamsWidget(): openfluid::ui::ware::ParameterizationWidget(),
  ui(new Ui::ParamsWidget)
{
  ui->setupUi(this);

  connect(ui->AddButton,SIGNAL(clicked()),this,SLOT(addParam()));
  connect(ui->ClearButton,SIGNAL(clicked()),this,SLOT(clearParams()));
}


// =====================================================================
// =====================================================================


ParamsWidget::~ParamsWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void ParamsWidget::update()
{
  ui->ParamsEdit->clear();

  openfluid::ware::WareParams_t::const_iterator it;
  openfluid::ware::WareParams_t::const_iterator itb = mp_Params->begin();
  openfluid::ware::WareParams_t::const_iterator ite = mp_Params->end();

  for (it=itb; it!= ite; ++it)
  {
    ui->ParamsEdit->append(QString::fromStdString((*it).first)+"="+QString::fromStdString((*it).second));
  }
}


// =====================================================================
// =====================================================================


void ParamsWidget::addParam()
{
  std::string ParamName;
  int i=0;

  do
  {
    i++;
    ParamName = (QString("param%1").arg(i)).toStdString();
  } while (mp_Params->find(ParamName) != mp_Params->end());

  mp_Params->operator[](ParamName) =  (QString("%1").arg(i)).toStdString();

  update();

  emit changed();
}


// =====================================================================
// =====================================================================


void ParamsWidget::clearParams()
{
  mp_Params->clear();

  update();

  emit changed();
}
