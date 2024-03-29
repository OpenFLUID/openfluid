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
  @file UnitsClassWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <memory>

#include <QColorDialog>
#include <QMessageBox>
#include <QDir>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/core/DatastoreItem.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/utils/GDALCompatibility.hpp>
#include <openfluid/utils/GDALHelpers.hpp>

#include "ui_UnitsClassWidget.h"
#include "UnitsClassWidget.hpp"
#include "AppTools.hpp"


QString UnitsClassWidget::m_ColorButtonStyleSheet =
    "QPushButton{ background-color : %1; border: 1px solid #8f8f91; border-radius: 3px;}";
/*    "QPushButton{ background-color : %1; border-radius : 6px; border: 1px solid #777777;}"
    "QPushButton:pressed{ background-color : %1; border-radius : 6px; border: 1px solid #777777;}"
    "QPushButton:default{ background-color : %1; border-radius : 2px; border: 1px solid #777777;}";*/


UnitsClassWidget::UnitsClassWidget(const QString& ClassName,
                                   const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& DSList,
                                   QWidget* Parent):
  QFrame(Parent),ui(new Ui::UnitsClassWidget),
  m_Selected(false), m_ClassName(ClassName), mp_LayerSource(nullptr)
{
  setObjectName("UnitsClassFrame");

  ui->setupUi(this);

  setSelected(false);

  ui->UnitClassLabel->setText(ClassName);

  ui->UpButton->setIcon(openfluid::ui::common::getIcon("go-up","/ui/common"));
  ui->UpButton->setIconSize(QSize(16,16));

  ui->DownButton->setIcon(openfluid::ui::common::getIcon("go-down","/ui/common"));
  ui->DownButton->setIconSize(QSize(16,16));

  ui->RemoveButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveButton->setIconSize(QSize(16,16));

  connect(ui->UpButton,SIGNAL(clicked()),this,SLOT(notifyUpClicked()));
  connect(ui->DownButton,SIGNAL(clicked()),this,SLOT(notifyDownClicked()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(notifyRemoveClicked()));

  ui->StyleWidget->setVisible(false);
  ui->ShowHideStyleLabel->setText(tr("Show map style"));
  connect(ui->ShowHideStyleLabel,SIGNAL(clicked()),this,SLOT(toggleShowHideStyle()));

  linkToDatastoreItem(DSList);

  // initialize line width

  int TmpLineWidth =
    openfluid::base::RunContextManager::instance()
      ->getProjectContextValue("/builder/spatial/unitsclasses/styles/"+m_ClassName.toStdString()+"/linewidth")
        .get<int>(0);

  m_LineWidth = TmpLineWidth;


  // initialize line color

  std::string TmpLineColor =
    openfluid::base::RunContextManager::instance()
      ->getProjectContextValue("/builder/spatial/unitsclasses/styles/"+m_ClassName.toStdString()+"/linecolor")
        .get<std::string>("");

  if (!TmpLineColor.empty())
  {
    m_LineColor = QColor(QString::fromStdString(TmpLineColor));
  }
  else
  {
    m_LineColor = openfluid::ui::common::getRandomColor();
    openfluid::base::RunContextManager::instance()
      ->setProjectContextValue("/builder/spatial/unitsclasses/styles/"+m_ClassName.toStdString(),"linecolor",
                               m_LineColor.name().toStdString());
  }


  // initialize fill color

  std::string TmpFillColor =
    openfluid::base::RunContextManager::instance()
      ->getProjectContextValue("/builder/spatial/unitsclasses/styles/"+m_ClassName.toStdString()+"/fillcolor")
        .get<std::string>("");

  if (!TmpFillColor.empty())
  {
    m_FillColor = QColor(QString::fromStdString(TmpFillColor));
  }
  else
  {
    m_FillColor = openfluid::ui::common::getRandomColor();
    openfluid::base::RunContextManager::instance()
      ->setProjectContextValue("/builder/spatial/unitsclasses/styles/"+m_ClassName.toStdString(),"fillcolor",
                               m_FillColor.name().toStdString());
  }


  ui->LineColorButton->setStyleSheet(QString(m_ColorButtonStyleSheet).arg(m_LineColor.name()));
  ui->FillColorButton->setStyleSheet(QString(m_ColorButtonStyleSheet).arg(m_FillColor.name()));
  ui->LineWidthSpinBox->setValue(m_LineWidth);

  connect(ui->VisibleCheckBox,SIGNAL(toggled(bool)),this,SLOT(changeVisible()));
  connect(ui->LineColorButton,SIGNAL(clicked()),this,SLOT(changeLineColor()));
  connect(ui->FillColorButton,SIGNAL(clicked()),this,SLOT(changeFillColor()));
  connect(ui->LineWidthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(changeLineWidth(int)));

}


// =====================================================================
// =====================================================================


UnitsClassWidget::~UnitsClassWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::toggleShowHideStyle()
{
  if (ui->StyleWidget->isVisible())
  {
    ui->StyleWidget->setVisible(false);
    ui->ShowHideStyleLabel->setText(tr("Show map style"));
  }
  else
  {
    ui->StyleWidget->setVisible(true);
    ui->ShowHideStyleLabel->setText(tr("Hide map style"));
  }
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::mousePressEvent(QMouseEvent* Event)
{
  if (!m_Selected)
  {
    emit selectionRequested(m_ClassName);
  }

  QWidget::mousePressEvent(Event);
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::setSelected(bool Selected)
{
  if (Selected)
  {
    m_Selected = true;
    setStyleSheet("QFrame {background-color : #FFFFFF;} QFrame#UnitsClassFrame {border : 1px solid #617487;}");
  }
  else
  {
    m_Selected = false;
    setStyleSheet("QFrame#UnitsClassFrame {border : 1px solid #CCCCCC;}");
  }
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::notifyUpClicked()
{
  emit upClicked(m_ClassName);
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::notifyDownClicked()
{
  emit downClicked(m_ClassName);
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::notifyRemoveClicked()
{
  bool OK = true;

  if (openfluid::base::PreferencesManager::instance()->isBuilderSpatialUnitsRemovalConfirm())
  {
    OK = (QMessageBox::question(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                tr("You are removing the %1 units class.\n"
                                   "All units of class %1 and associated attributes and connections will be lost.\n\n"
                                   "Proceed anyway?").arg(m_ClassName),
                                QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
  }

  if (OK)
  {
    emit removeClicked(m_ClassName);
  }
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::setUpButtonEnabled(bool Enabled)
{
  ui->UpButton->setEnabled(Enabled);
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::setDownButtonEnabled(bool Enabled)
{
  ui->DownButton->setEnabled(Enabled);
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::changeLineColor()
{
  QColor TmpColor = QColorDialog::getColor(m_LineColor,this);

  if (TmpColor.isValid())
  {
    m_LineColor = TmpColor;
    ui->LineColorButton->setStyleSheet(QString(m_ColorButtonStyleSheet).arg(m_LineColor.name()));

    openfluid::base::RunContextManager::instance()
      ->setProjectContextValue("/builder/spatial/unitsclasses/styles/"+m_ClassName.toStdString(),"linecolor",
        m_LineColor.name().toStdString());

    emit styleChanged(m_ClassName);
  }
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::changeFillColor()
{
  QColor TmpColor = QColorDialog::getColor(m_FillColor,this);

  if (TmpColor.isValid())
  {
    m_FillColor = TmpColor;
    ui->FillColorButton->setStyleSheet(QString(m_ColorButtonStyleSheet).arg(m_FillColor.name()));

    openfluid::base::RunContextManager::instance()
      ->setProjectContextValue("/builder/spatial/unitsclasses/styles/"+m_ClassName.toStdString(),"fillcolor",
        m_FillColor.name().toStdString());

    emit styleChanged(m_ClassName);
  }
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::changeLineWidth(int Width)
{
  m_LineWidth = Width;

  openfluid::base::RunContextManager::instance()
    ->setProjectContextValue("/builder/spatial/unitsclasses/styles/"+m_ClassName.toStdString(),"linewidth",m_LineWidth);

  emit styleChanged(m_ClassName);
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::changeVisible()
{
  emit styleChanged(m_ClassName);
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::linkToDatastoreItem(const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& DSList)
{
  
  bool LayerIsSurfacic = false;
  OGRwkbGeometryType LayerType = wkbUnknown;

  mp_LayerSource = nullptr;

  // disconnect signal to avoid multiple refresh of map,
  // due to changeVisible slot
  disconnect(ui->VisibleCheckBox,SIGNAL(toggled(bool)),this,SLOT(changeVisible()));

  ui->FillColorLabel->setVisible(false);
  ui->FillColorButton->setVisible(false);

  if (!DSList.empty())
  {
    mp_LayerSource = DSList.front();

    // retreive informations from layer
    if (mp_LayerSource->getType() == openfluid::core::UnstructuredValue::UnstructuredType::VECTOR)
    {
      std::unique_ptr<openfluid::core::DatastoreItem> DSItem = 
        std::make_unique<openfluid::core::DatastoreItem>(mp_LayerSource->getID(),
                                                         mp_LayerSource->getPrefixPath(),
                                                         mp_LayerSource->getRelativePath(),
                                                         mp_LayerSource->getType(),
                                                         mp_LayerSource->getUnitsClass());

      openfluid::core::GeoVectorValue* VectorData = dynamic_cast<openfluid::core::GeoVectorValue*>(DSItem->value());

      if (VectorData->data() != nullptr && VectorData->containsField("OFLD_ID",0))
      {
        // detect layer geometry type
        LayerType = VectorData->data()->GetLayer(0)->GetGeomType();
        // detect if layertype is a surface
        LayerIsSurfacic =  openfluid::utils::isOGRSurfacicType(VectorData->layer(0)->GetGeomType());
      }
    }

    ui->LayerSourceLabel->setText(QDir::toNativeSeparators(QString::fromStdString(mp_LayerSource->getRelativePath())));
    ui->StyleParamsWidget->setEnabled(true);

    std::string LayerTypeStr = OGRGeometryTypeToName(LayerType);
    if (LayerTypeStr.empty())
    {
      ui->LayerTypeLabel->setText(tr("Unknown geometry"));
    }
    else
    {
      ui->LayerTypeLabel->setText(QString::fromStdString(LayerTypeStr));
    }
     ui->LayerTypeLabel->setToolTip(QString("Geometry code: %1").arg(LayerType));

    if (LayerIsSurfacic)
    {
      ui->FillColorLabel->setVisible(true);
      ui->FillColorButton->setVisible(true);
    }
  }
  else
  {
    ui->LayerTypeLabel->setText(tr("No geometry available"));
    ui->ShowHideStyleLabel->setVisible(false);
    ui->LayerSourceLabel->setText("");
    ui->StyleParamsWidget->setEnabled(false);
    ui->VisibleCheckBox->setChecked(false);
  }

  connect(ui->VisibleCheckBox,SIGNAL(toggled(bool)),this,SLOT(changeVisible()));
}


// =====================================================================
// =====================================================================


void UnitsClassWidget::setLayerVisible()
{
  if (mp_LayerSource != nullptr)
  {
    ui->VisibleCheckBox->setChecked(true);
  }
}


// =====================================================================
// =====================================================================


bool UnitsClassWidget::isLayerVisible() const
{
  return ui->VisibleCheckBox->isChecked();
}
