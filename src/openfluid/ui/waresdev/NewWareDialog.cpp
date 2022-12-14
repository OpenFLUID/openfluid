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
  @file NewWareDialog.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPushButton>
#include <QMessageBox>

#include <openfluid/ui/config.hpp>
#include <openfluid/ui/waresdev/NewWareDialog.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>

#include "ui_NewWareDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


NewWareDialog::NewWareDialog(openfluid::ware::WareType Type, QWidget* Parent) :
    openfluid::ui::common::MessageDialog(Parent),
    ui(new Ui::NewWareDialog), m_WareType(Type), m_NewWarePath(""),
    m_UseSimSignature(false)
{
  ui->setupUi(this);

  m_WareTypeDir.setPath(QString::fromStdString(
                          openfluid::base::WorkspaceManager::instance()->getWaresPath(m_WareType))
                       );

  ui->BextTypeComboBox->addItems(getBuilderExtTypeTexts());
  ui->BextCategoryComboBox->addItems(getBuilderExtCategoryTexts());

  QString WareId;
  QString SrcClassname;

  switch (m_WareType)
  {
    case openfluid::ware::WareType::SIMULATOR:
      setupMessageUi(tr("Create a new simulator"));
      WareId = "sim.id";
      SrcClassname = "Simulator";
      ui->BuilderExtWidget->setVisible(false);
      break;
    case openfluid::ware::WareType::OBSERVER:
      setupMessageUi(tr("Create a new observer"));
      WareId = "obs.id";
      SrcClassname = "Observer";
      ui->BuilderExtWidget->setVisible(false);
      break;
    case openfluid::ware::WareType::BUILDEREXT:
      setupMessageUi(tr("Create a new Builder extension"));
      WareId = "bext.id";
      SrcClassname = "BuilderExtension";
      ui->UiWidget->setVisible(false);
      break;
    default:
      break;
  }

  QString IDTooltip;
  QString ClassTooltip;
  QRegExp ClassnameRegExp = getClassnameRegExp(ClassTooltip);

  // "required" placeholder
  QString PlaceholderStr = getPlaceholderRequired();

  ui->IdEdit->setValidator(
      new QRegExpValidator(getWareIdRegExp(IDTooltip), this));
  ui->IdEdit->setToolTip(IDTooltip);
  ui->IdEdit->setPlaceholderText(PlaceholderStr);
  ui->ClassNameEdit->setValidator(new QRegExpValidator(ClassnameRegExp, this));
  ui->ClassNameEdit->setToolTip(ClassTooltip);
  ui->ClassNameEdit->setPlaceholderText(PlaceholderStr);
  ui->UiParamClassNameEdit->setValidator(new QRegExpValidator(ClassnameRegExp, this));
  ui->UiParamClassNameEdit->setToolTip(ClassTooltip);
  ui->UiParamClassNameEdit->setPlaceholderText(PlaceholderStr);

  connect(ui->IdEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->UiParamGroupBox, SIGNAL(toggled(bool)), this, SLOT(onInformationChanged()));
  connect(ui->ClassNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->UiParamClassNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));

  ui->IdEdit->setText(WareId);
  ui->ClassNameEdit->setText(SrcClassname);
  ui->UiParamClassNameEdit->setText("ParamsUiWidget");
}


// =====================================================================
// =====================================================================


NewWareDialog::NewWareDialog(const openfluid::ware::SimulatorSignature& Signature, QWidget* Parent) :
  NewWareDialog(openfluid::ware::WareType::SIMULATOR,Parent)
{
  m_SimSignature = Signature;
  m_UseSimSignature = true;

  ui->IdEdit->setText(QString::fromStdString(Signature.ID));
  ui->IdEdit->setReadOnly(true);
}


// =====================================================================
// =====================================================================


NewWareDialog::~NewWareDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


QRegExp NewWareDialog::getClassnameRegExp(QString& Tooltip)
{
  Tooltip = QObject::tr("Accepts only letters, digits, underscores ('_'), and must begin with a letter.");
  return QRegExp("[a-zA-Z]+[a-zA-Z0-9_]*");
}


// =====================================================================
// =====================================================================


QRegExp NewWareDialog::getWareIdRegExp(QString& Tooltip)
{
  // TODO see openfluid::tools::isValidWareID() for refactoring

  Tooltip = QObject::tr("Accepts only letters, digits, dashes ('-'), underscores ('_') and dots ('.').");
  return QRegExp(QString::fromStdString(openfluid::tools::WareIDRuleString));
}


// =====================================================================
// =====================================================================


QStringList NewWareDialog::getBuilderExtTypeTexts()
{
  QStringList BextMode_Texts;
  BextMode_Texts << QObject::tr("Modal") << QObject::tr("Modeless") << QObject::tr("Workspace");
  return BextMode_Texts;
}


// =====================================================================
// =====================================================================


QStringList NewWareDialog::getBuilderExtCategoryTexts()
{
  QStringList BextCategory_Texts;
  BextCategory_Texts << QObject::tr("Spatial domain") << QObject::tr("Model") << QObject::tr("Results")
                      << QObject::tr("Other");
  return BextCategory_Texts;
}


// =====================================================================
// =====================================================================


void NewWareDialog::onInformationChanged()
{
  QString WarningMsg = "";

  if (!ui->IdEdit->hasAcceptableInput())
  {
    WarningMsg = tr("Ware ID is empty");
  }
  else if (m_WareTypeDir.exists(ui->IdEdit->text()))
  {
    WarningMsg = tr("Ware ID already exists");
  }
  else if (!ui->ClassNameEdit->hasAcceptableInput())
  {
    WarningMsg = tr("Main class name is empty");
  }
  else if (ui->UiParamGroupBox->isChecked() && !ui->UiParamClassNameEdit->hasAcceptableInput())
  {
    WarningMsg = tr("UI parameterization class name is empty");
  }  

  setStatus(WarningMsg);
}


// =====================================================================
// =====================================================================


void NewWareDialog::setStatus(const QString WarningMsg)
{
  bool Ok = WarningMsg.isEmpty();

  setMessage(Ok ? "" : WarningMsg);
}


// =====================================================================
// =====================================================================


openfluid::ware::WareID_t NewWareDialog::getID() const
{
  return  ui->IdEdit->text().toStdString();
}


// =====================================================================
// =====================================================================


std::string NewWareDialog::getClassName() const
{
  return  ui->ClassNameEdit->text().toStdString();
}


// =====================================================================
// =====================================================================


bool NewWareDialog::isWareUI() const
{
  return ui->UiParamGroupBox->isChecked();
}


// =====================================================================
// =====================================================================


openfluid::builderext::ExtensionMode  NewWareDialog::getBuilderextMode() const
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


openfluid::builderext::ExtensionCategory  NewWareDialog::getBuilderextCategory() const
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


QString NewWareDialog::getBuilderextMenuText() const
{
  return ui->BextMenutextEdit->text();
}


} } }  // namespaces
