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
  @file ObserverWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/machine/ObserverInstance.hpp>
#include <openfluid/machine/ObserverRegistry.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>

#include "ui_WareWidget.h"
#include "ObserverWidget.hpp"
#include "ParameterWidget.hpp"
#include "AddParamDialog.hpp"

#include "builderconfig.hpp"
#include "ProjectCentral.hpp"
#include "ExtensionsRegistry.hpp"
#include "WaresTranslationsRegistry.hpp"


ObserverWidget::ObserverWidget(QWidget* Parent,
                               openfluid::fluidx::ObserverDescriptor* Desc,
                               const openfluid::ware::WareID_t& ID,
                               int Index):
  WareWidget(Parent,ID,QString::fromStdString(ID),Desc->isEnabled(),BUILDER_OBSERVER_BGCOLOR,Index),
  mp_Desc(Desc)
{
  refresh();

  connect(ui->AddParamButton,SIGNAL(clicked()),this,SLOT(addParameterToList()));
}


// =====================================================================
// =====================================================================


ObserverWidget::~ObserverWidget()
{

}


// =====================================================================
// =====================================================================


void ObserverWidget::setEnabledWare(bool Enabled)
{
  WareWidget::setEnabled(Enabled);
  getWareDescriptor()->setEnabled(Enabled);
}


// =====================================================================
// =====================================================================


openfluid::fluidx::WareDescriptor* ObserverWidget::getWareDescriptor()
{
  return mp_Desc;
}


// =====================================================================
// =====================================================================


void ObserverWidget::refresh()
{
  const auto& Container = openfluid::machine::ObserverRegistry::instance()->wareContainer(m_ID);

  if (Container.isValid() && Container.hasSignature())
  {
    findDocFile(Container.getPath(),Container.signature()->ID);
    ui->DocButton->setVisible(!m_DocFilePath.empty());

    QString BuildType = QString::fromStdString(Container.signature()->BuildInfo.BuildType);
    updateBuildInfoIcons(BuildType.contains("DEB"),BuildType == "RELEASE" || BuildType == "RELWITHDEBINFO");

    setAvailableWare(true);
    ui->NameLabel->setText(QString::fromStdString(Container.signature()->Name));
    ui->InfosSideWidget->update(Container);

    updateParametersList();

    const auto& LinkUID = Container.getLinkUID();
    setupParamExtension(ExtensionsRegistry::instance()->isParameterizationExtensionRegistered(LinkUID), LinkUID);  }
  else
  {
    setAvailableWare(false);
  }

  updateWidgetBackground();
}


// =====================================================================
// =====================================================================


void ObserverWidget::applyContainer()
{
  const auto& Container = openfluid::machine::ObserverRegistry::instance()->wareContainer(m_ID);
  if (Container.isValid() && Container.hasSignature())
  {
    updateParametersListWithSignature(Container.signature().get());
  }
}
