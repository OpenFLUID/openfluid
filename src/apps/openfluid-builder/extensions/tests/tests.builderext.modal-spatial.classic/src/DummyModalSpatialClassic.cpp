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
  @file DummyModalSpatialClassic.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QInputDialog>

#include "ui_DummyModalSpatialClassic.h"
#include "DummyModalSpatialClassic.hpp"


DummyModalSpatialClassic::DummyModalSpatialClassic() :
  openfluid::ui::builderext::PluggableModalExtension(),ui(new Ui::DummyModalSpatialClassic)
{
  Q_INIT_RESOURCE(spatialclassic);

  ui->setupUi(this);

  ui->ImageLabel->setText("");
  ui->ImageLabel->setPixmap(QPixmap(":/logo_title.png"));


  connect(ui->AddButton,SIGNAL(clicked()),this,SLOT(addUnitClass()));
  connect(ui->ClearButton,SIGNAL(clicked()),this,SLOT(clearSpatialDomain()));
  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


DummyModalSpatialClassic::~DummyModalSpatialClassic()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void DummyModalSpatialClassic::addUnitClass()
{
  bool OK;
  QString UnitClass = QInputDialog::getText(this, "Add a unit Class",
                                            "Unit class name:",
                                            QLineEdit::Normal,
                                            "",
                                            &OK);

  if (OK && !UnitClass.isEmpty())
  {
    openfluid::fluidx::SpatialUnitDescriptor UDesc;

    UDesc.setUnitsClass(UnitClass.toStdString());
    UDesc.setID(1);
    UDesc.setProcessOrder(1);
    mp_Desc->spatialDomain().addUnit(UDesc);

    emit fluidxChanged(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_SPATIALSTRUCT);
  }
}


// =====================================================================
// =====================================================================


void DummyModalSpatialClassic::clearSpatialDomain()
{
  mp_Desc->spatialDomain().clearDomain();

  emit fluidxChanged(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_SPATIALSTRUCT |
                     openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_SPATIALATTRS);

}


// =====================================================================
// =====================================================================


void DummyModalSpatialClassic::update(openfluid::ui::builderext::FluidXUpdateFlags::Flags UpdateFlags)
{
  if (UpdateFlags.testFlag(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_ALL) ||
      UpdateFlags.testFlag(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_SPATIALATTRS) ||
      UpdateFlags.testFlag(openfluid::ui::builderext::FluidXUpdateFlags::Flag::FLUIDX_SPATIALSTRUCT))
  {
    ui->CountLabel->setText(QString("The spatial domain is made of %1 units class(es)")
                            .arg(mp_Desc->spatialDomain().getClassNames().size()));
  }
}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(DummyModalSpatialClassic)

