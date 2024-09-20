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
  @file SimulatorWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_WareWidget.h"
#include "SimulatorWidget.hpp"
#include "ParameterWidget.hpp"
#include "AddParamDialog.hpp"
#include "ExtensionsRegistry.hpp"
#include "builderconfig.hpp"


SimulatorWidget::SimulatorWidget(QWidget* Parent, openfluid::fluidx::ModelItemDescriptor* Desc,
                                 const openfluid::ware::WareID_t& ID,
                                 int Index):
  ModelItemWidget(Parent,Desc,ID,QString::fromStdString(ID),BUILDER_SIMULATOR_BGCOLOR,Index)
{
  refresh();

  ui->GenerateSrcButton->setText("");
  ui->GenerateSrcButton->setIcon(openfluid::ui::common::getIcon("ghost2sim","/builder"));
  ui->GenerateSrcButton->setIconSize(QSize(16,16));

  connect(ui->AddParamButton,SIGNAL(clicked()),this,SLOT(addParameterToList()));
  connect(ui->GenerateSrcButton,SIGNAL(clicked()),this,SLOT(notifySrcGenerateAsked()));
}


// =====================================================================
// =====================================================================


SimulatorWidget::~SimulatorWidget()
{

}


// =====================================================================
// =====================================================================


void SimulatorWidget::refresh()
{
  const auto& Container = openfluid::machine::SimulatorRegistry::instance()->wareContainer(m_ID);

  m_Ghost = false;

  if (Container.isValid() && Container.hasSignature())
  {
    findDocFile(Container.getPath(),Container.signature()->ID);
    ui->DocButton->setVisible(!m_DocFilePath.empty());

    m_Ghost = Container.isGhost();
    if (!m_Ghost)
    {
      QString BuildType = QString::fromStdString(Container.signature()->BuildInfo.BuildType);
      updateBuildInfoIcons(BuildType.contains("DEB"),BuildType == "RELEASE" || BuildType == "RELWITHDEBINFO");
    }
    else
    {
      ui->DebugIconLabel->setVisible(false);
      ui->SpeedIconLabel->setVisible(false);
    }

    setAvailableWare(true);

    ui->NameLabel->setText(QString::fromStdString(Container.signature()->Name));
    ui->InfosSideWidget->update(Container);

    updateParametersListWithSignature(Container.signature().get());

    const auto& LinkUID = Container.getLinkUID();
    setupParamExtension(ExtensionsRegistry::instance()->isParameterizationExtensionRegistered(LinkUID), LinkUID);
  }
  else
  {
    setAvailableWare(false);
  }

  ui->GenerateSrcButton->setVisible(m_Ghost);
}


// =====================================================================
// =====================================================================


void SimulatorWidget::notifySrcGenerateAsked()
{
  if (m_Ghost)
  {
    emit srcGenerateAsked(QString::fromStdString(m_ID));
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::applyContainer()
{
  const auto& Container = openfluid::machine::SimulatorRegistry::instance()->wareContainer(m_ID);
  if (Container.isValid() && Container.hasSignature())
  {
    updateParametersListWithSignature(Container.signature().get());
  }
}
