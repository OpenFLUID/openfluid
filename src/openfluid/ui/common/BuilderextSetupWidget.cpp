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
 @file BuilderextSetupWidget.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/ui/common/BuilderextSetupWidget.hpp>
#include "ui_BuilderextSetupWidget.h"


namespace openfluid { namespace ui { namespace common {

BuilderextSetupWidget::BuilderextSetupWidget(QWidget* Parent) :
  QWidget(Parent), ui(new Ui::BuilderextSetupWidget)
{
  ui->setupUi(this); 
  

  ui->BextTypeComboBox->addItems(getBuilderExtTypeTexts());
  ui->BextCategoryComboBox->addItems(getBuilderExtCategoryTexts());
}


// =====================================================================
// =====================================================================


BuilderextSetupWidget::~BuilderextSetupWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


QStringList BuilderextSetupWidget::getBuilderExtTypeTexts()
{
  QStringList BextMode_Texts;
  BextMode_Texts << QObject::tr("Modal") << QObject::tr("Modeless") << QObject::tr("Workspace");
  return BextMode_Texts;
}


// =====================================================================
// =====================================================================


QStringList BuilderextSetupWidget::getBuilderExtCategoryTexts()
{
  QStringList BextCategory_Texts;
  BextCategory_Texts << QObject::tr("Spatial domain") << QObject::tr("Model") << QObject::tr("Results")
                      << QObject::tr("Other");
  return BextCategory_Texts;
}


// =====================================================================
// =====================================================================


void BuilderextSetupWidget::initializeFromSignature(const openfluid::builderext::BuilderExtensionSignature& Signature)
{
  // TOIMPL find better way of applying index based on value for both comboboxes
  if (Signature.Mode == openfluid::builderext::ExtensionMode::MODAL)
  {
    ui->BextTypeComboBox->setCurrentIndex(0);
  }
  else if (Signature.Mode == openfluid::builderext::ExtensionMode::MODELESS)
  {
    ui->BextTypeComboBox->setCurrentIndex(1);
  }
  else
  {
    ui->BextTypeComboBox->setCurrentIndex(2);
  }
  

  if (Signature.Category == openfluid::builderext::ExtensionCategory::SPATIAL)
  {
    ui->BextCategoryComboBox->setCurrentIndex(0);
  }
  else if (Signature.Category == openfluid::builderext::ExtensionCategory::MODEL)
  {
    ui->BextCategoryComboBox->setCurrentIndex(1);
  }
  else if (Signature.Category == openfluid::builderext::ExtensionCategory::RESULTS)
  {
    ui->BextCategoryComboBox->setCurrentIndex(2);
  }
  else
  {
    ui->BextCategoryComboBox->setCurrentIndex(3);
  }

  ui->BextMenutextEdit->setText(QString::fromStdString(Signature.MenuText));
}


// =====================================================================
// =====================================================================


openfluid::builderext::ExtensionMode BuilderextSetupWidget::getBuilderextMode() const
{
  if (ui->BextTypeComboBox->currentIndex() == 0)
  {
    return openfluid::builderext::ExtensionMode::MODAL;
  }

  if (ui->BextTypeComboBox->currentIndex() == 1)
  {
    return openfluid::builderext::ExtensionMode::MODELESS;
  }

  return openfluid::builderext::ExtensionMode::WORKSPACE;
}


// =====================================================================
// =====================================================================


openfluid::builderext::ExtensionCategory BuilderextSetupWidget::getBuilderextCategory() const
{
  if (ui->BextCategoryComboBox->currentIndex() == 0)
  {
    return openfluid::builderext::ExtensionCategory::SPATIAL;
  }

  if (ui->BextCategoryComboBox->currentIndex() == 1)
  {
    return openfluid::builderext::ExtensionCategory::MODEL;
  }

  if (ui->BextCategoryComboBox->currentIndex() == 2)
  {
    return openfluid::builderext::ExtensionCategory::RESULTS;
  }

  return openfluid::builderext::ExtensionCategory::OTHER;
}


// =====================================================================
// =====================================================================


QString BuilderextSetupWidget::getBuilderextMenuText() const
{
  return ui->BextMenutextEdit->text();
}


} } }  // namespaces
