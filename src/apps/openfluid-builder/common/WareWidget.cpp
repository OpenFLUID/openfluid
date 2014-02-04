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
  \file WareWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/guicommon/PreferencesManager.hpp>

#include "builderconfig.hpp"

#include "ui_WareWidget.h"
#include "WareWidget.hpp"
#include "ParameterWidget.hpp"

#include <QMessageBox>


WareWidget::WareWidget(QWidget* Parent, const openfluid::ware::WareID_t& ID, bool Enabled, const QString& BGColor):
  QWidget(Parent),ui(new Ui::WareWidget), m_ID(ID), m_EnabledBGColor(BGColor),
  m_Available(false),m_Enabled(Enabled), m_ParamsExpanded(false)
{

  ui->setupUi(this);
  ui->IDLabel->setText(QString::fromStdString(m_ID));

  ui->UpButton->setText("");
  ui->UpButton->setIcon(QIcon(":/icons/go-up.png"));
  ui->UpButton->setIconSize(QSize(16,16));

  ui->DownButton->setText("");
  ui->DownButton->setIcon(QIcon(":/icons/go-down.png"));
  ui->DownButton->setIconSize(QSize(16,16));

  ui->RemoveButton->setText("");
  ui->RemoveButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveButton->setIconSize(QSize(16,16));

  ui->AddParamButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddParamButton->setIconSize(QSize(16,16));


  setEnabledWare(m_Enabled);
  if (m_Enabled) ui->EnabledCheckBox->setCheckState(Qt::Checked);
  else ui->EnabledCheckBox->setCheckState(Qt::Unchecked);

  mp_ShowHideParamsLabel = new ActionLabel(tr("show parameters and informations"),ui->ParamInfoTitleWidget);
  ((QBoxLayout*)(ui->ParamInfoTitleWidget->layout()))->insertWidget(0,mp_ShowHideParamsLabel);

  mp_SignatureWidget = new SignatureWidget(this);
  ui->ParamInfoSplitter->addWidget(mp_SignatureWidget);

  QList<int> SplitSizes;
  SplitSizes << 300 << 300;
  ui->ParamInfoSplitter->setSizes(SplitSizes);

  connect(mp_ShowHideParamsLabel,SIGNAL(clicked()),this,SLOT(updateShowHideParams()));
  connect(ui->EnabledCheckBox,SIGNAL(toggled(bool)),this,SLOT(setEnabledWare(bool)));

  connect(ui->UpButton,SIGNAL(clicked()),this,SLOT(notifyUpClicked()));
  connect(ui->DownButton,SIGNAL(clicked()),this,SLOT(notifyDownClicked()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(notifyRemoveClicked()));

  ui->ParamInfoWidget->setVisible(false);
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
  ui->NameLabel->setVisible(Available);
  ui->ParamInfoTitleWidget->setVisible(Available);
  ui->ParamInfoWidget->setVisible(Available && m_ParamsExpanded);

  updateWidgetBackground();
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


void WareWidget::updateWidgetBackground()
{
  QString BGColor = m_EnabledBGColor;
  QString BGPattern = "";

  if (!m_Enabled) BGColor = BUILDER_DISABLEDWARE_BGCOLOR;
  if (!m_Available) BGPattern = " background-image:url(:/images/warn-pattern-lightgray.png);";

  setStyleSheet("QFrame#WareFrame { background-color:"+BGColor+"; "+BGPattern+"}");
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
    m_ParamsExpanded = false;
}



// =====================================================================
// =====================================================================


void WareWidget::displayParams()
{
  ui->ParamInfoWidget->setVisible(m_ParamsExpanded);
  if (m_ParamsExpanded)
    mp_ShowHideParamsLabel->setText(tr("hide parameters and informations"));
  else
    mp_ShowHideParamsLabel->setText(tr("show parameters and informations"));
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


void WareWidget::notifyUpClicked()
{
  emit upClicked(QString::fromStdString(m_ID));
}


// =====================================================================
// =====================================================================


void WareWidget::notifyDownClicked()
{
  emit downClicked(QString::fromStdString(m_ID));
}


// =====================================================================
// =====================================================================


void WareWidget::notifyRemoveClicked()
{
  bool OK = true;

  if (openfluid::guicommon::PreferencesManager::getInstance()->isItemRemovalConfirm())
  {
    QString TypeStr = tr("the generator");

    if (getType() == openfluid::fluidx::WareDescriptor::PluggedSimulator) TypeStr = tr("the simulator");
    else if (getType() == openfluid::fluidx::WareDescriptor::PluggedObserver) TypeStr = tr("the observer");


    OK = (QMessageBox::question(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                tr("You are removing %1 %2.\nAll parameters will be lost.\n\nProceed anyway?").arg(QString::fromStdString(getID())).arg(TypeStr),
                                QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
  }

  if (OK)
    emit removeClicked(QString::fromStdString(m_ID));
}


// =====================================================================
// =====================================================================


void WareWidget::updateParamValue(const QString& /*Name*/, const QString& /*Value*/)
{

}


// =====================================================================
// =====================================================================


void WareWidget::removeParam(const QString& /*Name*/)
{

}


// =====================================================================
// =====================================================================


bool WareWidget::addParameterWidget(const QString& Name, const QString& Value)
{
  ParameterWidget* ParamWidget = new ParameterWidget(this,
                                                     Name,Value,
                                                     QString::fromStdString(""),true);

  connect(ParamWidget,SIGNAL(valueChanged(const QString&, const QString&)),this, SLOT(updateParamValue(const QString&,const QString&)));
  connect(ParamWidget,SIGNAL(removeClicked(const QString&)),this, SLOT(removeParam(const QString&)));

  int Position = ui->ParamsAreaContents->layout()->count()-1;
  ((QBoxLayout*)(ui->ParamsAreaContents->layout()))->insertWidget(Position,ParamWidget);

  return true;
}


// =====================================================================
// =====================================================================


bool WareWidget::removeParameterWidget(const QString& Name, bool WithFinalStretch)
{
  int LastIndex = ui->ParamsAreaContents->layout()->count()-WithFinalStretch-1;

  for (int i=0;i<=LastIndex;i++)
  {
    ParameterWidget* W = (ParameterWidget*)(ui->ParamsAreaContents->layout()->itemAt(i)->widget());
    if (W != 0 && W->getName() == Name)
    {
      W = (ParameterWidget*)(ui->ParamsAreaContents->layout()->takeAt(i)->widget());
      W->deleteLater();
      return true;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


void WareWidget::updateWare()
{

}


// =====================================================================
// =====================================================================


void WareWidget::clearParameterWidgets()
{
  QBoxLayout* Layout = ((QBoxLayout*)(ui->ParamsAreaContents->layout()));
  QLayoutItem* Item;

  while ((Item = Layout->takeAt(0)) != NULL)
  {
    if (Item->widget() != NULL)
      Item->widget()->deleteLater();

    delete Item;
  }
}
