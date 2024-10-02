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
  @file ModelItemWidget.cpp

  @author Armel THONI <armel.thoni@inrae.fr>
 */


#include <QColorDialog>
#include <QDesktopServices>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/tools/Filesystem.hpp>

#include "ui_WareWidget.h"
#include "ModelItemWidget.hpp"
#include "ParameterWidget.hpp"
#include "AddParamDialog.hpp"
#include "builderconfig.hpp"
#include "ProjectCentral.hpp"
#include "ExtensionsRegistry.hpp"
#include "WaresTranslationsRegistry.hpp"


ModelItemWidget::ModelItemWidget(QWidget* Parent, openfluid::fluidx::ModelItemDescriptor* Desc,
                                 const openfluid::ware::WareID_t& ID, const QString& DisplayedText,
                                 const QString& BGColor, int Index):
  WareWidget(Parent,ID,DisplayedText,Desc->isEnabled(),BGColor,Index),
  mp_Desc(Desc)
{
  ui->ColorButton->setVisible(true);
  ui->CustomColor->setVisible(true);

  m_CustomColor = getCustomColor();
  applyCustomColor();
  
  connect(this, SIGNAL(styleChanged()), this, SLOT(applyCustomColor()));
  connect(ui->CustomColor, SIGNAL(clicked()), this, SLOT(changeCustomColor()));
}


// =====================================================================
// =====================================================================


ModelItemWidget::~ModelItemWidget()
{

}


// =====================================================================
// =====================================================================


void ModelItemWidget::setEnabledWare(bool Enabled)
{
  mp_Desc->setEnabled(Enabled);
  WareWidget::setEnabledWare(Enabled);
}


// =====================================================================
// =====================================================================


openfluid::fluidx::WareDescriptor* ModelItemWidget::getWareDescriptor()
{
  return mp_Desc;
}


// =====================================================================
// =====================================================================


QColor ModelItemWidget::getCustomColor()
{
  QColor CustomColor;
  std::string CustomColorStr =
    openfluid::base::RunContextManager::instance()
      ->getProjectContextValue("/builder/model/colors",m_ID).get<std::string>("");

  if (!CustomColorStr.empty())
  {
    CustomColor = QColor(QString::fromStdString(CustomColorStr));
  }

  return CustomColor;
}


// =====================================================================
// =====================================================================


void ModelItemWidget::applyCustomColor()
{
  QString CustomColorName;
  if (m_CustomColor.isValid())
  {
    CustomColorName = m_CustomColor.name();
      
    ui->ColorButton->setText("");
    ui->ColorButton->setIcon(openfluid::ui::common::getIcon("color-marker-cancel","/ui/common"));
    ui->ColorButton->setIconSize(QSize(16,16));
    
    disconnect(ui->ColorButton, SIGNAL(clicked()),this,SLOT(changeCustomColor()));
    connect(ui->ColorButton, SIGNAL(clicked()),this,SLOT(resetCustomColor()));
  }
  else
  {
    ui->ColorButton->setText("");
    ui->ColorButton->setIcon(openfluid::ui::common::getIcon("color-marker","/ui/common"));
    ui->ColorButton->setIconSize(QSize(16,16));
    
    disconnect(ui->ColorButton, SIGNAL(clicked()),this,SLOT(resetCustomColor()));
    connect(ui->ColorButton, SIGNAL(clicked()),this,SLOT(changeCustomColor()));
  }
  ui->CustomColor->setStyleSheet(QString("border-radius: 2px;border: 1px solid %1;background-color: %2;")
                                 .arg(BUILDER_WARE_BORDERCOLOR).arg(CustomColorName));
  
}


// =====================================================================
// =====================================================================


void ModelItemWidget::resetCustomColor()
{
  auto Path = "/builder/model/colors/"+m_ID;

  openfluid::base::RunContextManager::instance()->removeProjectContextValue(Path);
  m_CustomColor = QColor();
  emit styleChanged();
}


// =====================================================================
// =====================================================================


void ModelItemWidget::changeCustomColor()
{
  QColor TmpColor = QColorDialog::getColor(m_CustomColor,this);

  if (TmpColor.isValid())
  {
    m_CustomColor = TmpColor;
    
    openfluid::base::RunContextManager::instance()->setProjectContextValue("/builder/model/colors",m_ID,
                                                                          m_CustomColor.name().toStdString());

  }
  emit styleChanged();
}
