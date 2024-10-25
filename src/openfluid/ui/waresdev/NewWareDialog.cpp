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
  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#if (QT_VERSION_MAJOR < 6)
#include <QRegExp>
#else
#include <QRegularExpression>
#endif

#include <openfluid/ui/common/SignatureEditorWidget.hpp>
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

  QString WareId;
  QString SrcClassname;

  switch (m_WareType)
  {
    case openfluid::ware::WareType::SIMULATOR:
      setupMessageUi(tr("Create a new simulator"));
      WareId = "sim.id";
      SrcClassname = "Simulator";
      ui->BextSetupWidget->setVisible(false);
      break;
    case openfluid::ware::WareType::OBSERVER:
      setupMessageUi(tr("Create a new observer"));
      WareId = "obs.id";
      SrcClassname = "Observer";
      ui->BextSetupWidget->setVisible(false);
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
#if (QT_VERSION_MAJOR < 6)
  QRegExp ClassnameRegExp = getClassnameRegExp(ClassTooltip);
#else
  QRegularExpression ClassnameRegExp = getClassnameRegExp(ClassTooltip);
#endif

  // "required" placeholder
  QString PlaceholderStr = getPlaceholderRequired();

  ui->IdEdit->setValidator(
#if (QT_VERSION_MAJOR < 6)
      new QRegExpValidator(openfluid::ui::common::getWareIdRegExp(IDTooltip), this));
#else
      new QRegularExpressionValidator(openfluid::ui::common::getWareIdRegExp(IDTooltip), this));
#endif
  ui->IdEdit->setToolTip(IDTooltip);
  ui->IdEdit->setPlaceholderText(PlaceholderStr);
#if (QT_VERSION_MAJOR < 6)
  ui->ClassNameEdit->setValidator(new QRegExpValidator(ClassnameRegExp, this));
#else
  ui->ClassNameEdit->setValidator(new QRegularExpressionValidator(ClassnameRegExp, this));
#endif
  ui->ClassNameEdit->setToolTip(ClassTooltip);
  ui->ClassNameEdit->setPlaceholderText(PlaceholderStr);
#if (QT_VERSION_MAJOR < 6)
  ui->UiParamClassNameEdit->setValidator(new QRegExpValidator(ClassnameRegExp, this));
#else
  ui->UiParamClassNameEdit->setValidator(new QRegularExpressionValidator(ClassnameRegExp, this));
#endif
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


#if (QT_VERSION_MAJOR < 6)
QRegExp NewWareDialog::getClassnameRegExp(QString& Tooltip)
#else
QRegularExpression NewWareDialog::getClassnameRegExp(QString& Tooltip)
#endif
{
  Tooltip = QObject::tr("Accepts only letters, digits, underscores ('_'), and must begin with a letter.");
#if (QT_VERSION_MAJOR < 6)
  return QRegExp("[a-zA-Z]+[a-zA-Z0-9_]*");
#else
  return QRegularExpression("[a-zA-Z]+[a-zA-Z0-9_]*");
#endif
}


// =====================================================================
// =====================================================================


void NewWareDialog::onInformationChanged()
{
  QString WarningMsg = "";

  if (!ui->IdEdit->hasAcceptableInput())
  {
    WarningMsg = QApplication::translate("openfluid::ui::config", openfluid::ui::config::WAREID_MESSAGE_EMPTY);
  }
  else if (m_WareTypeDir.exists(ui->IdEdit->text()))
  {
    WarningMsg = QApplication::translate("openfluid::ui::config", openfluid::ui::config::WAREID_MESSAGE_EXISTING);
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
  return ui->BextSetupWidget->getBuilderextMode();
}


// =====================================================================
// =====================================================================


openfluid::builderext::ExtensionCategory  NewWareDialog::getBuilderextCategory() const
{
  return ui->BextSetupWidget->getBuilderextCategory();
}


// =====================================================================
// =====================================================================


QString NewWareDialog::getBuilderextMenuText() const
{
  return ui->BextSetupWidget->getBuilderextMenuText();
}


// =====================================================================
// =====================================================================


bool NewWareDialog::openSignature() const
{
  return ui->SignatureCheckBox->isChecked();
}


} } }  // namespaces
