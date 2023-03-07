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


#include <QDesktopServices>

#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/tools/FilesystemPath.hpp>

#include "ui_WareWidget.h"
#include "SimulatorWidget.hpp"
#include "ParameterWidget.hpp"
#include "AddParamDialog.hpp"
#include "builderconfig.hpp"
#include "ProjectCentral.hpp"
#include "ExtensionsRegistry.hpp"
#include "WaresTranslationsRegistry.hpp"


SimulatorWidget::SimulatorWidget(QWidget* Parent, openfluid::fluidx::ModelItemDescriptor* Desc,
                                 const openfluid::ware::WareID_t& ID,
                                 int Index):
  ModelItemWidget(Parent,Desc,ID,QString::fromStdString(ID),BUILDER_SIMULATOR_BGCOLOR,Index),
  m_IsTranslated(false),m_DocFilePath("")
{
  refresh();
  ui->GenerateSrcButton->setText("");
  ui->GenerateSrcButton->setIcon(openfluid::ui::common::getIcon("ghost2sim","/builder"));
  ui->GenerateSrcButton->setIconSize(QSize(16,16));

  ui->DocButton->setText("");
  ui->DocButton->setIcon(openfluid::ui::common::getIcon("view-doc","/builder"));
  ui->DocButton->setIconSize(QSize(16,16));


  connect(ui->AddParamButton,SIGNAL(clicked()),this,SLOT(addParameterToList()));
  connect(ui->GenerateSrcButton,SIGNAL(clicked()),this,SLOT(notifySrcGenerateAsked()));
  connect(ui->DocButton,SIGNAL(clicked()),this,SLOT(openDocFile()));
}


// =====================================================================
// =====================================================================


SimulatorWidget::~SimulatorWidget()
{

}


// =====================================================================
// =====================================================================


void SimulatorWidget::updateParametersListWithSignature(const openfluid::ware::SimulatorSignature* Signature)
{
  clearParameterWidgets();

  QStringList ParamsInSign = createParamWidgetsFromSignature(Signature);

  // Other params not in signature
  openfluid::ware::WareParams_t DescParams = mp_Desc->getParameters();
  for (const auto& DescParam : DescParams)
  {
    if (!ParamsInSign.contains(QString::fromStdString(DescParam.first)))
    { 
      addParam(DescParam.first, DescParam.second, "", ParamsInSign, false, true);
    }
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::updateParametersList()
{
  const auto& Container = openfluid::machine::SimulatorRegistry::instance()->wareContainer(m_ID);

  if (Container.isValid() && Container.hasSignature())
  {
    updateParametersListWithSignature(Container.signature().get());
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::findDocFile(const openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature>& 
                                  Container)
{
  m_DocFilePath.clear();
  if (Container.isValid() && Container.hasSignature())
  {
    std::string BasePath = openfluid::tools::FilesystemPath(Container.getPath()).dirname();
    std::string ExpectedFilePath = openfluid::tools::Filesystem::joinPath({BasePath,Container.signature()->ID+".pdf"});

    if (openfluid::tools::FilesystemPath(ExpectedFilePath).isFile())
    {
      m_DocFilePath = ExpectedFilePath;
    }
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::refresh()
{
  const auto& Container = openfluid::machine::SimulatorRegistry::instance()->wareContainer(m_ID);

  m_Ghost = false;

  if (Container.isValid() && Container.hasSignature())
  {
    findDocFile(Container);
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

    // TODO begin to be refactored, see also ObserverWidget =========

    mp_ParamsWidget = nullptr;

    if (ExtensionsRegistry::instance()->isParameterizationExtensionRegistered(Container.getLinkUID()))
    {
      if (!m_IsTranslated)
      {
        auto ParamsUIWarePath = QString::fromStdString(
          ExtensionsRegistry::instance()->getParameterizationExtensionPath(Container.getLinkUID())
        );
        WaresTranslationsRegistry::instance()->tryLoadWareTranslation(ParamsUIWarePath);
        m_IsTranslated = true;
      }

      mp_ParamsWidget = static_cast<openfluid::ui::builderext::PluggableParameterizationExtension*>(
          ExtensionsRegistry::instance()->instanciateParameterizationExtension(Container.getLinkUID()));
      mp_ParamsWidget->setParent(this);
      mp_ParamsWidget->linkParams(&(mp_Desc->parameters()));
      mp_ParamsWidget->setFluidXDescriptor(&(ProjectCentral::instance()->descriptors()));

      connect(mp_ParamsWidget,SIGNAL(changed()),this,SLOT(notifyChangedFromParameterizationWidget()));

      int Position = ui->ParameterizationStackWidget->addWidget(mp_ParamsWidget);

      mp_ParamsWidget->update();

      ui->ParameterizationStackWidget->setCurrentIndex(Position);
    }

    updateParameterizationSwitch();

    // end to be refactored =========
  }
  else
  {
    setAvailableWare(false);
  }

  ui->GenerateSrcButton->setVisible(m_Ghost);
}


// =====================================================================
// =====================================================================


void SimulatorWidget::addParameterToList()
{
  QStringList ExistPList;

  openfluid::ware::WareParams_t Params = mp_Desc->getParameters();

  for (openfluid::ware::WareParams_t::iterator it = Params.begin();it != Params.end(); ++it)
  {
    ExistPList.append(QString::fromStdString((*it).first));
  }


  AddParamDialog AddPDlg(ExistPList,QStringList(),this);

  if (AddPDlg.exec() == QDialog::Accepted)
  {
    if (addParameterWidget(AddPDlg.getParamName(),AddPDlg.getParamValue()))
    {
      mp_Desc->setParameter(AddPDlg.getParamName().toStdString(),AddPDlg.getParamValue().toStdString());
      emit changed();
    }
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::removeParameterFromList(const QString& Name)
{
  if (removeParameterWidget(Name))
  {
    mp_Desc->eraseParameter(Name.toStdString());
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::prepareWareUpdate()
{
  if (mp_ParamsWidget)
  {
    ui->ParameterizationStackWidget->removeWidget(mp_ParamsWidget);
    delete mp_ParamsWidget;
    mp_ParamsWidget = nullptr;
  }
}


// =====================================================================
// =====================================================================


void SimulatorWidget::updateWare()
{
  refresh();
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


void SimulatorWidget::openDocFile()
{
  if (!m_DocFilePath.empty())
  {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(m_DocFilePath)));
    QApplication::restoreOverrideCursor();
  }
}
