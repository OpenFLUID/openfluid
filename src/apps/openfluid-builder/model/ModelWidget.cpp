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
  @file ModelWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QImage>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/buddies/ExamplesBuddy.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_WaresManagementWidget.h"
#include "ui_ModelWidget.h"
#include "builderconfig.hpp"
#include "ModelWidget.hpp"
#include "SimulatorWidget.hpp"
#include "GeneratorWidget.hpp"
#include "ParameterWidget.hpp"
#include "AddSimulatorDialog.hpp"
#include "AddGeneratorDialog.hpp"
#include "AddParamDialog.hpp"


ModelWidget::ModelWidget(QWidget* Parent, openfluid::fluidx::FluidXDescriptor& FXDesc):
  WorkspaceWidget(Parent, FXDesc), ui(new Ui::ModelWidget), m_Model(FXDesc.model())
{
  ui->setupUi(this);

  mp_WaresManWidget = new WaresManagementWidget(this,true);
  ui->ModelTabWidget->insertTab(0,mp_WaresManWidget,"Management");
  ui->ModelTabWidget->setCurrentIndex(0);

  ui->GraphicalView->setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);
  mp_ModelScene = new ModelScene(m_Model,this);
  ui->GraphicalView->setScene(mp_ModelScene);

  mp_ShowHideGlobalParamsLabel = new openfluid::ui::common::ActionLabel(tr("show"),ui->GlobalParamsWidget);
  ui->GlobalParamsTitleLayout->insertWidget(1,mp_ShowHideGlobalParamsLabel);
  ui->GlobalParamsManagementWidget->setVisible(false);

  ui->AddGlobalParamButton->setText("");
  ui->AddGlobalParamButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddGlobalParamButton->setIconSize(QSize(20,20));

  mp_WaresManWidget->ui->AddWareFirstButton->setText(tr("Add simulator"));
  mp_WaresManWidget->ui->AddWareSecondButton->setText(tr("Add generator"));

  ui->PNGExportButton->setIcon(openfluid::ui::common::getIcon("export","/ui/common"));
  ui->PNGExportButton->setIconSize(QSize(20,20));

  ui->SVGExportButton->setIcon(openfluid::ui::common::getIcon("export","/ui/common"));
  ui->SVGExportButton->setIconSize(QSize(20,20));
  
  const QStringList ColouringModeCodes(QStringList() << "NONE" << "BACKGROUND" << "BORDER");
  const QStringList ColouringModeTexts(QStringList() << tr("none") << tr("background") << tr("border"));
  
  for (int i=0;i<ColouringModeCodes.size();i++)
  {
    ui->ColoringComboBox->addItem(ColouringModeTexts[i], ColouringModeCodes[i]);
  }
  
  QString ColouringMode = openfluid::base::RunContextManager::instance()->getProjectConfigValue(
    PROJECT_COLORMODE_CATEGORY.first, PROJECT_COLORMODE_CATEGORY.second).toString();
  for (int i=0; i<ColouringModeCodes.size();i++)
  {
    if (ColouringModeCodes[i] == ColouringMode)
    {
      ui->ColoringComboBox->setCurrentIndex(i);
    }
  }
  

  connect(mp_ShowHideGlobalParamsLabel,SIGNAL(clicked()),this,SLOT(updateShowHideGlobalParams()));
  connect(ui->AddGlobalParamButton,SIGNAL(clicked()),this,SLOT(addGlobalParam()));

  connect(mp_WaresManWidget->ui->AddWareFirstButton,SIGNAL(clicked()),this,SLOT(addSimulator()));
  connect(mp_WaresManWidget->ui->AddWareSecondButton,SIGNAL(clicked()),this,SLOT(addGenerator()));

  connect(ui->FitViewButton,SIGNAL(clicked()),ui->GraphicalView,SLOT(fitViewToItems()));
  connect(ui->ResetViewButton,SIGNAL(clicked()),ui->GraphicalView,SLOT(resetView()));
  connect(ui->PNGExportButton,SIGNAL(clicked()),ui->GraphicalView,SLOT(exportSceneAsPNG()));
  connect(ui->SVGExportButton,SIGNAL(clicked()),ui->GraphicalView,SLOT(exportSceneAsSVG()));
  connect(ui->ColoringComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeColouringMode(int)));

  refresh();

  connect(ui->ShowVarsCheckBox,SIGNAL(clicked(bool)),mp_ModelScene,SLOT(showVariables(bool)));

  connect(mp_ModelScene,SIGNAL(srcEditAsked(const QString&,bool)),this,SLOT(notifySrcEditAsked(const QString&,bool)));
}


// =====================================================================
// =====================================================================


ModelWidget::~ModelWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void ModelWidget::changeColouringMode(int /*index*/)
{
  openfluid::base::RunContextManager::instance()->setProjectConfigValue(PROJECT_COLORMODE_CATEGORY.first, 
                                                                        PROJECT_COLORMODE_CATEGORY.second,
                                                                        ui->ColoringComboBox->currentData());
  mp_ModelScene->refresh();
}


// =====================================================================
// =====================================================================


void ModelWidget::updateShowHideGlobalParams()
{
  if (ui->GlobalParamsManagementWidget->isVisible())
  {
    ui->GlobalParamsManagementWidget->setVisible(false);
    mp_ShowHideGlobalParamsLabel->setText(tr("show"));
  }
  else
  {
    ui->GlobalParamsManagementWidget->setVisible(true);
    mp_ShowHideGlobalParamsLabel->setText(tr("hide"));
  }
}


// =====================================================================
// =====================================================================


void ModelWidget::addGlobalParam()
{

  // existing parameters list
  QStringList ExistPList;
  openfluid::ware::WareParams_t GParams = m_Model.getGlobalParameters();

  for (openfluid::ware::WareParams_t::iterator it = GParams.begin();it != GParams.end(); ++it)
  {
    ExistPList.append(QString::fromStdString((*it).first));
  }


  // parameters list for completion, based on model items parameters
  QStringList CompPList;

  openfluid::machine::SimulatorSignatureRegistry* Reg =
      openfluid::machine::SimulatorSignatureRegistry::instance();

  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items =
      m_Model.items();

  for (std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    const openfluid::machine::ModelItemSignatureInstance* Sign = Reg->signature(*it);

    if (Sign)
    {
      std::vector<openfluid::ware::SignatureDataItem> Items =
          Sign->Signature->HandledData.UsedParams;

      for (std::vector<openfluid::ware::SignatureDataItem>::iterator it =
          Items.begin(); it != Items.end(); ++it)
      {
        if (!CompPList.contains(QString::fromStdString((*it).DataName)))
        {
          CompPList.append(QString::fromStdString((*it).DataName));
        }
      }
    }
  }


  AddParamDialog AddPDlg(ExistPList,CompPList,this);

  if (AddPDlg.exec() == QDialog::Accepted)
  {
    m_Model.setGlobalParameter(AddPDlg.getParamName().toStdString(),AddPDlg.getParamValue().toStdString());

    ParameterWidget* ParamWidget = new ParameterWidget(this,
                                                       AddPDlg.getParamName(),AddPDlg.getParamValue(),
                                                       QString::fromStdString(""),false,true);

    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),
            this, SLOT(updateGlobalParamValue(const QString&,const QString&)));
    connect(ParamWidget,SIGNAL(removeClicked(const QString&)),
            this, SLOT(removeGlobalParam(const QString&)));

    int Position = ui->GlobalParamsAreaContents->layout()->count()-1;
    ((QBoxLayout*)(ui->GlobalParamsAreaContents->layout()))->insertWidget(Position,ParamWidget);

    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELPARAMS);
  }
}


// =====================================================================
// =====================================================================


void ModelWidget::removeGlobalParam(const QString& Name)
{
  for (int i=0;i<ui->GlobalParamsAreaContents->layout()->count();i++)
  {
    ParameterWidget* W = (ParameterWidget*)(ui->GlobalParamsAreaContents->layout()->itemAt(i)->widget());
    if (W != 0 && W->getName() == Name)
    {
      W->deleteLater();
      m_Model.eraseGlobalParameter(Name.toStdString());

      emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELPARAMS);
      return;
    }
  }
}


// =====================================================================
// =====================================================================


void ModelWidget::updateGlobalParamValue(const QString& Name, const QString& Value)
{
  m_Model.setGlobalParameter(Name.toStdString(),Value.toStdString());
  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELPARAMS);
}


// =====================================================================
// =====================================================================


void ModelWidget::updateGlobalParams()
{
  openfluid::ware::WareParams_t GParams = m_Model.getGlobalParameters();

  for (openfluid::ware::WareParams_t::iterator it = GParams.begin();it != GParams.end(); ++it)
  {
    ParameterWidget* ParamWidget =
        new ParameterWidget(this,
                            QString::fromStdString((*it).first),QString::fromStdString((*it).second),
                            QString::fromStdString(""),false,true);

    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),
            this, SLOT(updateGlobalParamValue(const QString&,const QString&)));
    connect(ParamWidget,SIGNAL(removeClicked(const QString&)),
            this, SLOT(removeGlobalParam(const QString&)));

    ((QBoxLayout*)(ui->GlobalParamsAreaContents->layout()))->addWidget(ParamWidget);
  }

  ((QBoxLayout*)(ui->GlobalParamsAreaContents->layout()))->addStretch();
}


// =====================================================================
// =====================================================================


void ModelWidget::addSimulator()
{

  QStringList SimList = openfluid::tools::toQStringList(m_Model.getOrderedIDs());

  AddSimulatorDialog AddSimDlg(SimList,this);

  if (AddSimDlg.exec() == QDialog::Accepted)
  {
    QString ID = AddSimDlg.getSelectedID();

    openfluid::fluidx::SimulatorDescriptor* SimDesc = new openfluid::fluidx::SimulatorDescriptor(ID.toStdString());
    
    
    // Add simulator source to examples waresdev folder (if existing)
    openfluid::buddies::BuddiesListener Listener;
    openfluid::buddies::ExamplesBuddy ExBuddy(&Listener);
    ExBuddy.parseOptions("selection=single,simulator="+ID.toStdString());
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    ExBuddy.run();
    QApplication::restoreOverrideCursor();

    m_Model.appendItem(SimDesc);

    SimulatorWidget* SimWidget = new SimulatorWidget(this,SimDesc,ID.toStdString(),0);

    connect(SimWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
    connect(SimWidget,SIGNAL(srcEditAsked(const QString&,bool)),this,SLOT(notifySrcEditAsked(const QString&,bool)));
    connect(SimWidget,SIGNAL(srcGenerateAsked(const QString&)),this,SLOT(notifySrcGenerateAsked(const QString&)));
    connect(SimWidget,SIGNAL(upClicked(const QString&,int)),this,SLOT(moveModelItemUp(const QString&,int)));
    connect(SimWidget,SIGNAL(downClicked(const QString&,int)),this,SLOT(moveModelItemDown(const QString&,int)));
    connect(SimWidget,SIGNAL(removeClicked(const QString&,int)),this,SLOT(removeModelItem(const QString&,int)));

    // compute position in layout, taking into account the ending spacer
    int Position = mp_WaresManWidget->ui->WaresListAreaContents->layout()->count()-1;
    ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(Position,SimWidget);

    mp_WaresManWidget->updateIndexesAndButtons();

    mp_ModelScene->refresh();
    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELDEF);
  }
}


// =====================================================================
// =====================================================================


void ModelWidget::addGenerator()
{
  AddGeneratorDialog AddGenDlg(this);

  if (AddGenDlg.exec() == QDialog::Accepted)
  {
    openfluid::fluidx::GeneratorDescriptor* GenDesc =
        new openfluid::fluidx::GeneratorDescriptor(AddGenDlg.getVariableName().toStdString(),
                                                   AddGenDlg.getUnitClass().toStdString(),
                                                   AddGenDlg.getMethod(),
                                                   AddGenDlg.getVariableSize());

    GenDesc->setParameters(AddGenDlg.getParams());

    m_Model.insertItem(GenDesc,0);

    std::string ID = m_Model.getID(GenDesc);

    GeneratorWidget* GenWidget =
        new GeneratorWidget(this,GenDesc,
                            ID,
                            0,
                            openfluid::machine::SimulatorSignatureRegistry::instance()
                            ->signature(AddGenDlg.getMethod()));

    connect(GenWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
    connect(GenWidget,SIGNAL(upClicked(const QString&,int)),this,SLOT(moveModelItemUp(const QString&,int)));
    connect(GenWidget,SIGNAL(downClicked(const QString&,int)),this,SLOT(moveModelItemDown(const QString&,int)));
    connect(GenWidget,SIGNAL(removeClicked(const QString&,int)),this,SLOT(removeModelItem(const QString&,int)));

    // Insert newly created generator at the top of the model items list
    ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(0,GenWidget);

    mp_WaresManWidget->updateIndexesAndButtons();

    mp_ModelScene->refresh();
    emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELDEF);

  }
}


// =====================================================================
// =====================================================================


void ModelWidget::moveModelItemUp(const QString& /*ID*/, int CurrentIndex)
{
  if (CurrentIndex < 0)
  {
    return;
  }

  unsigned int From = CurrentIndex;
  unsigned int To = (From == 0) ? m_Model.getItemsCount() - 1 : From - 1;

  m_Model.moveItem(From, To);

  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  mp_WaresManWidget->updateIndexesAndButtons();

  mp_ModelScene->refresh();

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELDEF);
}


// =====================================================================
// =====================================================================


void ModelWidget::moveModelItemDown(const QString& /*ID*/, int CurrentIndex)
{
  if (CurrentIndex < 0)
  {
    return;
  }

  unsigned int From = CurrentIndex;
  unsigned int To = (From == (unsigned int)m_Model.getItemsCount() - 1) ? 0 : From + 1;

  m_Model.moveItem(From,To);


  WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(From)->widget());
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->insertWidget(To,W);

  mp_WaresManWidget->updateIndexesAndButtons();

  mp_ModelScene->refresh();

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELDEF);

}


// =====================================================================
// =====================================================================


void ModelWidget::removeModelItem(const QString& /*ID*/, int CurrentIndex)
{
  if (CurrentIndex < 0)
  {
    return;
  }

  WareWidget* W =
      (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->takeAt(CurrentIndex)->widget());
  W->deleteLater();

  m_Model.removeItem(CurrentIndex);

  mp_WaresManWidget->updateIndexesAndButtons();

  mp_ModelScene->refresh();

  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELDEF);
}


// =====================================================================
// =====================================================================


void ModelWidget::refresh()
{
  updateCoupledModel();

  updateGlobalParams();

  mp_ModelScene->refresh();
}


// =====================================================================
// =====================================================================


void ModelWidget::updateCoupledModel()
{
  const std::list<openfluid::fluidx::ModelItemDescriptor*>& Items = m_Model.items();

  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itb = Items.begin();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator ite = Items.end();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator itl = Items.end().operator--();
  std::list<openfluid::fluidx::ModelItemDescriptor*>::const_iterator it;

  for (it = itb; it!= ite; ++it)
  {
    if ((*it)->getType() == openfluid::ware::WareType::SIMULATOR)
    {
      SimulatorWidget* SimWidget = new SimulatorWidget(this,*it,m_Model.getID(*it),0);
      mp_WaresManWidget->ui->WaresListAreaContents->layout()->addWidget(SimWidget);
      if (it == itb)
      {
        SimWidget->setUpButtonEnabled(false);
      }
      if (it == itl)
      {
        SimWidget->setDownButtonEnabled(false);
      }

      connect(SimWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
      connect(SimWidget,SIGNAL(srcEditAsked(const QString&,bool)),this,SLOT(notifySrcEditAsked(const QString&,bool)));
      connect(SimWidget,SIGNAL(srcGenerateAsked(const QString&)),this,SLOT(notifySrcGenerateAsked(const QString&)));
      connect(SimWidget,SIGNAL(upClicked(const QString&,int)),this,SLOT(moveModelItemUp(const QString&,int)));
      connect(SimWidget,SIGNAL(downClicked(const QString&,int)),this,SLOT(moveModelItemDown(const QString&,int)));
      connect(SimWidget,SIGNAL(removeClicked(const QString&,int)),this,SLOT(removeModelItem(const QString&,int)));
      connect(SimWidget,SIGNAL(styleChanged()),mp_ModelScene,SLOT(refresh()));
    }
    else if ((*it)->getType() == openfluid::ware::WareType::GENERATOR)
    {
      // TODO see if a more elegant method is possible for generators signature
      // than passing signature instance to constructor

      GeneratorWidget* GenWidget =
          new GeneratorWidget(this,*it,
                              m_Model.getID(*it),
                              0,
                              openfluid::machine::SimulatorSignatureRegistry::instance()->signature(*it));

      mp_WaresManWidget->ui->WaresListAreaContents->layout()->addWidget(GenWidget);
      if (it == itb)
      {
        GenWidget->setUpButtonEnabled(false);
      }
      if (it == itl)
      {
        GenWidget->setDownButtonEnabled(false);
      }

      connect(GenWidget,SIGNAL(changed()),this,SLOT(dispatchChangesFromChildren()));
      connect(GenWidget,SIGNAL(upClicked(const QString&,int)),this,SLOT(moveModelItemUp(const QString&,int)));
      connect(GenWidget,SIGNAL(downClicked(const QString&,int)),this,SLOT(moveModelItemDown(const QString&,int)));
      connect(GenWidget,SIGNAL(removeClicked(const QString&,int)),this,SLOT(removeModelItem(const QString&,int)));
      connect(GenWidget,SIGNAL(styleChanged()),mp_ModelScene,SLOT(refresh()));
    }
  }
  ((QBoxLayout*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()))->addStretch();

  mp_WaresManWidget->updateIndexesAndButtons();
}


// =====================================================================
// =====================================================================


void ModelWidget::dispatchChangesFromChildren()
{
  mp_ModelScene->refresh();
  emit changed(openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELDEF |
               openfluid::builderext::FluidXUpdateFlags::FLUIDX_MODELPARAMS);
}


// =====================================================================
// =====================================================================


void ModelWidget::notifySrcEditAsked(const QString& ID, bool Ghost)
{
  emit srcEditAsked(ID,openfluid::ware::WareType::SIMULATOR,Ghost);
}


// =====================================================================
// =====================================================================


void ModelWidget::notifySrcGenerateAsked(const QString& ID)
{
  emit srcGenerateAsked(ID);
}


// =====================================================================
// =====================================================================


void ModelWidget::prepareWaresUpdate()
{
  int LastIndex = mp_WaresManWidget->ui->WaresListAreaContents->layout()->count()-1;

  for (int i=0;i<=LastIndex;i++)
  {
    WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->itemAt(i)->widget());
    if (W != nullptr)
    {
      W->prepareWareUpdate();
    }
  }
}


// =====================================================================
// =====================================================================


void ModelWidget::updateWares()
{
  int LastIndex = mp_WaresManWidget->ui->WaresListAreaContents->layout()->count()-1;

  for (int i=0;i<=LastIndex;i++)
  {
    WareWidget* W = (WareWidget*)(mp_WaresManWidget->ui->WaresListAreaContents->layout()->itemAt(i)->widget());
    if (W != nullptr)
    {
      W->updateWare();
    }
  }

  mp_ModelScene->refresh();
}
