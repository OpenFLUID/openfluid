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
  @file WareWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QMessageBox>
#include <QDesktopServices>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "builderconfig.hpp"

#include "ui_WareWidget.h"
#include "WareWidget.hpp"
#include "ParameterWidget.hpp"
#include "ExtensionsRegistry.hpp"
#include "WaresTranslationsRegistry.hpp"
#include "ProjectCentral.hpp"
#include "AddParamDialog.hpp"


WareWidget::WareWidget(QWidget* Parent,
                       const openfluid::ware::WareID_t& ID,
                       const QString& DisplayedText,
                       bool Enabled, const QString& BGColor, int Index):
  QWidget(Parent),ui(new Ui::WareWidget), m_ID(ID), m_EnabledBGColor(BGColor),
  m_Available(false),m_IsTranslated(false), m_Ghost(false),m_Enabled(Enabled), m_CurrentIndex(Index),
  m_ParamsExpanded(false), mp_ParamsWidget(nullptr)
{
  ui->setupUi(this);
  ui->IDLabel->setText(DisplayedText);
  ui->IDLabel->setToolTip(DisplayedText);
  ui->IDLabel->setElideMode(Qt::ElideRight);

  ui->OrderLabel->setText(QString("#%1").arg(Index));

  ui->NameLabel->setElideMode(Qt::ElideRight);

  ui->DebugIconLabel->setText("");
  ui->DebugIconLabel->setPixmap(openfluid::ui::common::getImage("ware-debug","builder"));
  ui->SpeedIconLabel->setText("");
  ui->SpeedIconLabel->setPixmap(openfluid::ui::common::getImage("ware-speed","builder"));
  updateBuildInfoIcons(false,false);


  ui->DocButton->setVisible(false);
  ui->DocButton->setText("");
  ui->DocButton->setIcon(openfluid::ui::common::getIcon("view-doc","/builder"));
  ui->DocButton->setIconSize(QSize(16,16));

  ui->UpButton->setText("");
  ui->UpButton->setIcon(openfluid::ui::common::getIcon("go-up","/ui/common"));
  ui->UpButton->setIconSize(QSize(16,16));

  ui->DownButton->setText("");
  ui->DownButton->setIcon(openfluid::ui::common::getIcon("go-down","/ui/common"));
  ui->DownButton->setIconSize(QSize(16,16));

  ui->RemoveButton->setText("");
  ui->RemoveButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveButton->setIconSize(QSize(16,16));

  ui->AddParamButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddParamButton->setIconSize(QSize(16,16));


  setEnabledWare(m_Enabled);
  if (m_Enabled)
  {
    ui->EnabledCheckBox->setCheckState(Qt::Checked);
  }
  else
  {
    ui->EnabledCheckBox->setCheckState(Qt::Unchecked);
  }

  ui->ShowHideParamsLabel->setText(tr("show parameters and informations"));


  QList<int> SplitSizes;
  SplitSizes << 300 << 300;
  ui->ParamInfoSplitter->setSizes(SplitSizes);

  connect(ui->ShowHideParamsLabel,SIGNAL(clicked()),this,SLOT(updateShowHideParams()));
  connect(ui->EnabledCheckBox,SIGNAL(toggled(bool)),this,SLOT(setEnabledWare(bool)));

  connect(ui->DocButton,SIGNAL(clicked()),this,SLOT(openDocFile()));

  connect(ui->UpButton,SIGNAL(clicked()),this,SLOT(notifyUpClicked()));
  connect(ui->DownButton,SIGNAL(clicked()),this,SLOT(notifyDownClicked()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(notifyRemoveClicked()));

  connect(ui->ParameterizationSwitchLabel,SIGNAL(clicked()),this,SLOT(switchParameterizationMode()));

  ui->ParamInfoWidget->setVisible(false);
  ui->GenerateSrcButton->setVisible(false);
  ui->ColorButton->setVisible(false);
  ui->CustomColor->setVisible(false);
}


// =====================================================================
// =====================================================================


WareWidget::~WareWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void WareWidget::setAvailableWare(bool Available)
{
  m_Available = Available;
  ui->DebugIconLabel->setVisible(Available);
  ui->SpeedIconLabel->setVisible(Available);
  ui->NameLabel->setVisible(Available);
  ui->ParamInfoTitleWidget->setVisible(Available);
  ui->ShowHideParamsLabel->setVisible(Available);
  ui->ParamInfoWidget->setVisible(Available && m_ParamsExpanded);

  updateWidgetBackground();
}


// =====================================================================
// =====================================================================


void WareWidget::updateWidgetBackground()
{
  QString BGColor = m_EnabledBGColor;
  QString BGPattern = "";

  if (m_Ghost)
  {
    BGColor = BUILDER_GHOST_BGCOLOR;
  }

  if (!m_Enabled)
  {
    BGColor = BUILDER_DISABLEDWARE_BGCOLOR;
  }

  if (!m_Available)
  {
    BGPattern = " background-image:url(:/builder/images/warn-pattern-lightgray.png); ";
  }

  setStyleSheet(QString("#WareFrame { background-color: %1; %2border: 1px solid %3}")
                .arg(BGColor).arg(BGPattern).arg(BUILDER_WARE_BORDERCOLOR));
}


// =====================================================================
// =====================================================================


void WareWidget::updateBuildInfoIcons(bool Debug,bool Speed)
{
  ui->DebugIconLabel->setEnabled(Debug);
  ui->DebugIconLabel->setToolTip(QString("Built with debug information : %1").arg(Debug ? "yes" : "no"));
  ui->SpeedIconLabel->setEnabled(Speed);
  ui->SpeedIconLabel->setToolTip(QString("Optimized for speed : %1").arg(Speed ? "yes" : "no"));
}


// =====================================================================
// =====================================================================


void WareWidget::findDocFile(const std::string& WarePath, const openfluid::ware::WareID_t& WareID)
{
  m_DocFilePath.clear();

  std::string BasePath = openfluid::tools::FilesystemPath(WarePath).dirname();
  std::string ExpectedFilePath = openfluid::tools::Filesystem::joinPath({BasePath,WareID+".pdf"});

  if (openfluid::tools::FilesystemPath(ExpectedFilePath).isFile())
  {
    m_DocFilePath = ExpectedFilePath;
  }
}


// =====================================================================
// =====================================================================


void WareWidget::updateShowHideParams()
{
  setExpanded(!m_ParamsExpanded);
  displayParams();
}


// =====================================================================
// =====================================================================


void WareWidget::setExpanded(bool Expand)
{
  if (m_Available)
  {
    m_ParamsExpanded = Expand;
  }
  else
  {
    m_ParamsExpanded = false;
  }
}


// =====================================================================
// =====================================================================


void WareWidget::displayParams()
{
  ui->ParamInfoWidget->setVisible(m_ParamsExpanded);

  if (m_ParamsExpanded)
  {
    ui->ShowHideParamsLabel->setText(tr("hide parameters and informations"));
  }
  else
  {
    ui->ShowHideParamsLabel->setText(tr("show parameters and informations"));
  }
}


// =====================================================================
// =====================================================================


void WareWidget::setUpButtonEnabled(bool Enabled)
{
  ui->UpButton->setEnabled(Enabled);
}


// =====================================================================
// =====================================================================


void WareWidget::setDownButtonEnabled(bool Enabled)
{
  ui->DownButton->setEnabled(Enabled);
}


// =====================================================================
// =====================================================================


void WareWidget::setCurrentIndex(int Index)
{
  m_CurrentIndex = Index;
  ui->OrderLabel->setText(QString("#%1").arg(m_CurrentIndex+1));
}


// =====================================================================
// =====================================================================


void WareWidget::notifyUpClicked()
{
  emit upClicked(QString::fromStdString(m_ID),m_CurrentIndex);
}


// =====================================================================
// =====================================================================


void WareWidget::notifyDownClicked()
{
  emit downClicked(QString::fromStdString(m_ID),m_CurrentIndex);
}


// =====================================================================
// =====================================================================


void WareWidget::notifyRemoveClicked()
{
  bool OK = true;

  if (openfluid::base::PreferencesManager::instance()->isBuilderWareRemovalConfirm())
  {
    QString IDStr = QString::fromStdString(getID());

    QString TypeIDStr = tr("the %1 generator").arg(IDStr);

    if (getType() == openfluid::ware::WareType::SIMULATOR)
    {
      TypeIDStr = tr("the %1 simulator").arg(IDStr);
    }
    else if (getType() == openfluid::ware::WareType::OBSERVER)
    {
      TypeIDStr = tr("the %1 observer").arg(IDStr);
    }

    OK = (QMessageBox::question(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                tr("You are removing %1.\n"
                                   "All parameters will be lost.\n\nProceed anyway?")
                                .arg(TypeIDStr),
                                QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
  }

  if (OK)
  {
    emit removeClicked(QString::fromStdString(m_ID),m_CurrentIndex);
  }
}


// =====================================================================
// =====================================================================


bool WareWidget::addParameterWidget(const QString& Name, const QString& Value)
{
  ParameterWidget* ParamWidget = new ParameterWidget(this,
                                                     Name,Value,
                                                     QString::fromStdString(""),false,true);

  connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),
          this, SLOT(updateParameterValue(const QString&,const QString&)));
  connect(ParamWidget,SIGNAL(removeClicked(const QString&)),
          this, SLOT(removeParameterFromList(const QString&)));

  ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()))->addWidget(ParamWidget);

  return true;
}


// =====================================================================
// =====================================================================


bool WareWidget::removeParameterWidget(const QString& Name)
{
  int LastIndex = ui->ParamsListZoneWidget->layout()->count()-1;

  for (int i=0;i<=LastIndex;i++)
  {
    ParameterWidget* W = static_cast<ParameterWidget*>(ui->ParamsListZoneWidget->layout()->itemAt(i)->widget());
    if (W != 0 && W->getName() == Name)
    {
      W = static_cast<ParameterWidget*>(ui->ParamsListZoneWidget->layout()->takeAt(i)->widget());
      W->deleteLater();
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


void WareWidget::openDocFile()
{
  if (!m_DocFilePath.empty())
  {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(m_DocFilePath)));
    QApplication::restoreOverrideCursor();
  }
}


// =====================================================================
// =====================================================================


void WareWidget::clearParameterWidgets()
{
  QBoxLayout* Layout = ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()));
  QLayoutItem* Item;

  while ((Item = Layout->takeAt(0)) != nullptr)
  {
    if (Item->widget() != nullptr)
    {
      Item->widget()->deleteLater();
    }

    delete Item;
  }
}


// =====================================================================
// =====================================================================


std::string WareWidget::getParamValue(const std::string& ParamName, openfluid::ware::WareParams_t& DescParams)
{
  std::string ParamValue;
  if (DescParams.find(ParamName) != DescParams.end())
  {
    ParamValue = DescParams[ParamName];
  }
  return ParamValue;
}


// =====================================================================
// =====================================================================


void WareWidget::addParam(const std::string& ParamName, const std::string& ParamValue, const std::string& ParamUnit, 
                  QStringList& ParamsInSign, const bool Required, const bool Removable)
{
  ParameterWidget* ParamWidget = new ParameterWidget(this,
                                                     QString::fromStdString(ParamName),
                                                     QString::fromStdString(ParamValue),
                                                     QString::fromStdString(ParamUnit),
                                                     Required, Removable);

  if (Removable)
  {
    connect(ParamWidget,SIGNAL(removeClicked(const QString&)),
            this, SLOT(removeParameterFromList(const QString&)));
  }
  else
  {
    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),
            this, SLOT(updateParameterValue(const QString&,const QString&)));
    ParamsInSign << QString::fromStdString(ParamName);
  }

  ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()))->addWidget(ParamWidget);
}


// =====================================================================
// =====================================================================


QStringList WareWidget::createParamWidgetsFromSignature(const openfluid::ware::DataWareSignature* Signature)
{
  const auto& UsedParams = Signature->HandledData.UsedParams;
  const auto&  RequiredParams = Signature->HandledData.RequiredParams;
    
  openfluid::ware::WareParams_t DescParams = getWareDescriptor()->getParameters();
  QStringList ParamsInSign;
  
  // Required params

  for (const auto& Param : RequiredParams)
  {
    std::string ParamName = Param.Name;
    addParam(ParamName, getParamValue(ParamName, DescParams), Param.SIUnit, ParamsInSign, true, false);
  }

  // Used params

  for (const auto& Param : UsedParams)
  {
    std::string ParamName = Param.Name;
    addParam(ParamName, getParamValue(ParamName, DescParams), Param.SIUnit, ParamsInSign, false, false);
  }
  return ParamsInSign;
}


// =====================================================================
// =====================================================================


void WareWidget::updateParametersList()
{
  clearParameterWidgets();

  openfluid::ware::WareParams_t DescParams = getWareDescriptor()->getParameters();

  for (auto it = DescParams.begin();it != DescParams.end(); ++it)
  {
    ParameterWidget* ParamWidget =
        new ParameterWidget(this,
                            QString::fromStdString((*it).first),QString::fromStdString((*it).second),
                            QString::fromStdString(""),
                            false,true);

    connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),
            this, SLOT(updateParameterValue(const QString&,const QString&)));
    connect(ParamWidget,SIGNAL(removeClicked(const QString&)),
            this, SLOT(removeParameterFromList(const QString&)));

    ((QBoxLayout*)(ui->ParamsListZoneWidget->layout()))->addWidget(ParamWidget);
  }
  applyContainer();
}


// =====================================================================
// =====================================================================


void WareWidget::updateParametersListWithSignature(const openfluid::ware::DataWareSignature* Signature)
{
  clearParameterWidgets();

  QStringList ParamsInSign = createParamWidgetsFromSignature(Signature);

  // Other params not in signature
  openfluid::ware::WareParams_t DescParams = getWareDescriptor()->getParameters();
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


void WareWidget::addParameterToList()
{
  QStringList ExistPList;

  openfluid::ware::WareParams_t Params = getWareDescriptor()->getParameters();

  for (openfluid::ware::WareParams_t::iterator it = Params.begin();it != Params.end(); ++it)
  {
    ExistPList.append(QString::fromStdString((*it).first));
  }

  // set existing parameters list as completion list
  // (for easy access to series of similar parameters)

  AddParamDialog AddPDlg(ExistPList,ExistPList,this);

  if (AddPDlg.exec() == QDialog::Accepted)
  {
    if (addParameterWidget(AddPDlg.getParamName(),AddPDlg.getParamValue()))
    {
      getWareDescriptor()->setParameter(AddPDlg.getParamName().toStdString(),AddPDlg.getParamValue().toStdString());
      emit changed();
    }
  }
}


// =====================================================================
// =====================================================================


void WareWidget::updateParameterValue(const QString& Name, const QString& Value)
{
  getWareDescriptor()->setParameter(Name.toStdString(),Value.toStdString());
  emit changed();
}


// =====================================================================
// =====================================================================


void WareWidget::removeParameterFromList(const QString& Name)
{
  if (removeParameterWidget(Name))
  {
    getWareDescriptor()->eraseParameter(Name.toStdString());
    emit changed();
  }
}


// =====================================================================
// =====================================================================


void WareWidget::prepareWareUpdate()
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


void WareWidget::setupParamExtension(bool IsParameterization, openfluid::machine::UUID_t LinkUID)
{
  mp_ParamsWidget = nullptr;
  if (IsParameterization)
  {
    if (!m_IsTranslated)
    {
      auto ParamsUIWarePath = QString::fromStdString(
        ExtensionsRegistry::instance()->getParameterizationExtensionPath(LinkUID)
      );
      WaresTranslationsRegistry::instance()->tryLoadWareTranslation(ParamsUIWarePath);
      m_IsTranslated = true;
    }

    mp_ParamsWidget = static_cast<openfluid::ui::builderext::PluggableParameterizationExtension*>(
        ExtensionsRegistry::instance()->instanciateParameterizationExtension(LinkUID));
    mp_ParamsWidget->setParent(this);
    mp_ParamsWidget->linkParams(&(getWareDescriptor()->parameters()));
    mp_ParamsWidget->setFluidXDescriptor(&(ProjectCentral::instance()->descriptors()));

    connect(mp_ParamsWidget,SIGNAL(changed()),this,SLOT(notifyChangedFromParameterizationWidget()));

    int Position = ui->ParameterizationStackWidget->addWidget(mp_ParamsWidget);

    mp_ParamsWidget->update();

    ui->ParameterizationStackWidget->setCurrentIndex(Position);
  }

  updateParameterizationSwitch();
}


// =====================================================================
// =====================================================================
    

void WareWidget::updateWare()
{
  refresh();
}


// =====================================================================
// =====================================================================


void WareWidget::setEnabledWare(bool Enabled)
{
  m_Enabled = Enabled;
  updateWidgetBackground();
}


// =====================================================================
// =====================================================================


void WareWidget::updateParameterizationSwitch()
{
  ui->ParameterizationSwitchLabel->setVisible(mp_ParamsWidget);

  if (mp_ParamsWidget)
  {
    if (ui->ParameterizationStackWidget->currentIndex() == 0)
    {
      ui->ParameterizationSwitchLabel->setText(tr("switch to assistant"));
    }
    else
    {
      ui->ParameterizationSwitchLabel->setText(tr("switch to list"));
    }
  }
}


// =====================================================================
// =====================================================================


void WareWidget::switchParameterizationMode()
{
  if (mp_ParamsWidget && ui->ParameterizationStackWidget->count() > 1)
  {

    if (ui->ParameterizationStackWidget->currentWidget() == mp_ParamsWidget)
    {
      updateParametersList();
      ui->ParameterizationStackWidget->setCurrentWidget(ui->ParamsListPage);
    }
    else
    {
      mp_ParamsWidget->update();
      ui->ParameterizationStackWidget->setCurrentWidget(mp_ParamsWidget);
    }

    updateParameterizationSwitch();
  }
}


// =====================================================================
// =====================================================================


void WareWidget::notifyChangedFromParameterizationWidget()
{
  emit changed();
}


// =====================================================================
// =====================================================================


void WareWidget::mouseDoubleClickEvent(QMouseEvent* Event)
{
  if (isClickable())
  {
    if (m_Ghost)
    {
      emit srcEditAsked(QString::fromStdString(m_ID),m_Ghost);
    }
#ifdef ENABLE_WARESDEV_INTEGRATION
    else
    {
      emit srcEditAsked(QString::fromStdString(m_ID),m_Ghost);
    }
#endif

    QWidget::mousePressEvent(Event);
  }
}
