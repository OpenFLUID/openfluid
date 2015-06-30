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
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/NewWareDialog.hpp>

#include "ui_NewWareDialog.h"

#include <openfluid/ui/config.hpp>

#include <QPushButton>
#include <QMessageBox>
#include <QUuid>

#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


NewWareDialog::NewWareDialog(openfluid::waresdev::WareSrcManager::WareType Type, QWidget* Parent) :
    QDialog(Parent), ui(new Ui::NewWareDialog), m_WareType(Type), m_NewWarePath(""),
    m_UseSimSignature(false)
{
  ui->setupUi(this);

  m_WareTypeDir.setPath(openfluid::waresdev::WareSrcManager::instance()->getWareTypePath(m_WareType));

  ui->Sim2doc_comboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getSim2docModeTexts());
  ui->BextType_comboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getBuilderExtTypeTexts());
  ui->BextCategory_comboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getBuilderExtCategoryTexts());

  QString WareId;
  QString SrcFilename;
  QString SrcClassname;

  switch (m_WareType)
  {
    case openfluid::waresdev::WareSrcManager::SIMULATOR:
      m_DefaultMsg = tr("Create a new simulator");
      WareId = "sim.id";
      SrcFilename = "MySim.cpp";
      SrcClassname = "MySimulator";
      ui->BuilderExt_widget->setVisible(false);
      break;
    case openfluid::waresdev::WareSrcManager::OBSERVER:
      m_DefaultMsg = tr("Create a new observer");
      WareId = "obs.id";
      SrcFilename = "MyObs.cpp";
      SrcClassname = "MyObserver";
      ui->Sim2doc_widget->setVisible(false);
      ui->BuilderExt_widget->setVisible(false);
      break;
    case openfluid::waresdev::WareSrcManager::BUILDEREXT:
      m_DefaultMsg = tr("Create a new Builder extension");
      WareId = "bext.id";
      SrcFilename = "MyExt.cpp";
      SrcClassname = "MyExtension";
      ui->Ui_widget->setVisible(false);
      ui->Sim2doc_widget->setVisible(false);
      break;
    default:
      break;
  }

  QString IDTooltip;
  QString CppTooltip;
  QString ClassTooltip;
  QRegExp CppRegExp = openfluid::waresdev::WareSrcFactory::getCppFilenameRegExp(CppTooltip);
  QRegExp ClassnameRegExp = openfluid::waresdev::WareSrcFactory::getClassnameRegExp(ClassTooltip);
  ui->Id_lineEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getWareIdRegExp(IDTooltip), this));
  ui->Id_lineEdit->setToolTip(IDTooltip);
  ui->Id_lineEdit->setPlaceholderText(openfluid::ui::config::PLACEHOLDER_REQUIRED);
  ui->SourceFilename_lineEdit->setValidator(new QRegExpValidator(CppRegExp, this));
  ui->SourceFilename_lineEdit->setToolTip(CppTooltip);
  ui->SourceFilename_lineEdit->setPlaceholderText(openfluid::ui::config::PLACEHOLDER_REQUIRED);
  ui->UiParamSourceFilename_lineEdit->setValidator(new QRegExpValidator(CppRegExp, this));
  ui->UiParamSourceFilename_lineEdit->setToolTip(CppTooltip);
  ui->UiParamSourceFilename_lineEdit->setPlaceholderText(openfluid::ui::config::PLACEHOLDER_REQUIRED);
  ui->ClassName_lineEdit->setValidator(new QRegExpValidator(ClassnameRegExp, this));
  ui->ClassName_lineEdit->setToolTip(ClassTooltip);
  ui->ClassName_lineEdit->setPlaceholderText(openfluid::ui::config::PLACEHOLDER_REQUIRED);
  ui->UiParamClassName_lineEdit->setValidator(new QRegExpValidator(ClassnameRegExp, this));
  ui->UiParamClassName_lineEdit->setToolTip(ClassTooltip);
  ui->UiParamClassName_lineEdit->setPlaceholderText(openfluid::ui::config::PLACEHOLDER_REQUIRED);

  connect(ui->Id_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->SourceFilename_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->UiParam_groupBox, SIGNAL(toggled(bool)), this, SLOT(onInformationChanged()));
  connect(ui->UiParamSourceFilename_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->ClassName_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->UiParamClassName_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));

  ui->Id_lineEdit->setText(WareId);
  ui->SourceFilename_lineEdit->setText(SrcFilename);
  ui->ClassName_lineEdit->setText(SrcClassname);
  ui->UiParamSourceFilename_lineEdit->setText("ParamsUiWidget.cpp");
  ui->UiParamClassName_lineEdit->setText("ParamsUiWidget");
}


// =====================================================================
// =====================================================================


NewWareDialog::NewWareDialog(const openfluid::ware::SimulatorSignature& Signature, QWidget* Parent) :
  NewWareDialog(openfluid::waresdev::WareSrcManager::SIMULATOR,Parent)
{
  m_SimSignature = Signature;
  m_UseSimSignature = true;

  ui->Id_lineEdit->setText(QString::fromStdString(Signature.ID));
  ui->Id_lineEdit->setReadOnly(true);
}


// =====================================================================
// =====================================================================


NewWareDialog::~NewWareDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void NewWareDialog::onInformationChanged()
{
  QString WarningMsg = "";

  if (!ui->Id_lineEdit->hasAcceptableInput())
    WarningMsg = tr("Ware ID is empty");
  else if (m_WareTypeDir.exists(ui->Id_lineEdit->text()))
    WarningMsg = tr("Ware ID already exists");
  else if (!ui->SourceFilename_lineEdit->hasAcceptableInput())
    WarningMsg = tr("Source file name must be of the form \"filexxx.cpp\"");
  else if (!ui->ClassName_lineEdit->hasAcceptableInput())
    WarningMsg = tr("Main class name is empty");
  else if (ui->UiParam_groupBox->isChecked() && !ui->UiParamSourceFilename_lineEdit->hasAcceptableInput())
    WarningMsg = tr("UI parameterization file name must be of the form \"filexxx.cpp\"");
  else if (ui->UiParam_groupBox->isChecked() && !ui->UiParamClassName_lineEdit->hasAcceptableInput())
    WarningMsg = tr("UI parameterization class name is empty");

  setStatus(WarningMsg);
}


// =====================================================================
// =====================================================================


QString NewWareDialog::getNewWarePath()
{
  return m_NewWarePath;
}


// =====================================================================
// =====================================================================


void NewWareDialog::setStatus(const QString WarningMsg)
{
  bool Ok = WarningMsg.isEmpty();

  ui->MessageLabel->setText(Ok ? m_DefaultMsg : WarningMsg);

  ui->MessageFrame->setStyleSheet(
      QString("background-color: %1;").arg(
          Ok ? openfluid::ui::config::DIALOGBANNER_BGCOLOR : openfluid::ui::config::DIALOGBANNER_WARNBGCOLOR));

  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(Ok);
}

// =====================================================================
// =====================================================================


void NewWareDialog::accept()
{
  QString WareId = ui->Id_lineEdit->text();

  bool WithUiParam = ui->UiParam_groupBox->isChecked();
  bool WithHpp = false;

  openfluid::waresdev::WareSrcFactory Factory(m_WareType);
  Factory.setWareId(WareId);

  QString NewFilePath, ErrMsg;
  bool Ok;

  openfluid::waresdev::WareSrcFactory::Replacements R;

  R.ClassName = ui->ClassName_lineEdit->text();
  R.RootCppFilename = ui->SourceFilename_lineEdit->text();
  R.RootHppFilename = openfluid::waresdev::WareSrcFactory::getHppFilename(R.RootCppFilename);
  R.HppHeaderGuard = openfluid::waresdev::WareSrcFactory::getHeaderGuard(R.RootHppFilename);

  R.SignatureInfos = "  // Informations\n"
                     "  DECLARE_NAME(\"\")\n"
                     "  DECLARE_DESCRIPTION(\"\")\n"
                     "  DECLARE_VERSION(\"\")\n"
                     "  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL)\n";

  R.LinkUID = QUuid::createUuid();

  if (m_WareType == openfluid::waresdev::WareSrcManager::SIMULATOR)
  {
    R.Sim2docModeIndex = ui->Sim2doc_comboBox->currentIndex();
    R.Sim2docInstall = ui->Sim2doc_checkBox->isChecked();

    R.SimulatorSchedulingReturn = "DefaultDeltaT()";

    if (m_UseSimSignature)
    {
      R.SignatureInfos = openfluid::waresdev::WareSrcFactory::getSimulatorSignatureInfos(m_SimSignature);
      R.SimulatorSignatureData = openfluid::waresdev::WareSrcFactory::getSimulatorSignatureData(m_SimSignature);
      R.SimulatorInitCode = openfluid::waresdev::WareSrcFactory::getSimulatorInitCode(m_SimSignature);
      R.SimulatorRunCode = openfluid::waresdev::WareSrcFactory::getSimulatorRunCode(m_SimSignature);
      R.SimulatorSchedulingReturn = openfluid::waresdev::WareSrcFactory::getSimulatorSchedulingReturn(m_SimSignature);
    }
  }
  else if (m_WareType == openfluid::waresdev::WareSrcManager::OBSERVER)
  {

  }
  else if (m_WareType == openfluid::waresdev::WareSrcManager::BUILDEREXT)
  {
    R.BuilderExtModeIndex = ui->BextType_comboBox->currentIndex();
    R.BuilderExtCategoryIndex = ui->BextCategory_comboBox->currentIndex();
    R.BuilderExtMenuText = ui->BextMenutext_lineEdit->text();
    WithHpp = true;
    WithUiParam = false;
  }


  if (WithUiParam &&
      (m_WareType == openfluid::waresdev::WareSrcManager::SIMULATOR ||
      m_WareType == openfluid::waresdev::WareSrcManager::OBSERVER))
  {
    R.ParamsUiEnabled = true;
    R.ParamsUiClassname = ui->UiParamClassName_lineEdit->text();
    R.ParamsUiRootCppFilename = ui->UiParamSourceFilename_lineEdit->text();
    R.ParamsUiRootHppFilename = openfluid::waresdev::WareSrcFactory::getHppFilename(R.ParamsUiRootCppFilename);
    R.ParamsUiHeaderGuard = openfluid::waresdev::WareSrcFactory::getHeaderGuard(R.ParamsUiRootHppFilename);
    R.ParamsUiComment = "";
  }


  Ok = m_WareTypeDir.mkdir(WareId);
  if (!Ok)
    ErrMsg = tr("Unable to create the ware directory");
  else
    Ok = Factory.createCMakeListsFile(NewFilePath, ErrMsg);

  if (Ok)
    Ok = Factory.createCmakeConfigFile(R, NewFilePath, ErrMsg);

  if (Ok)
    Ok = Factory.createCppFile(R, NewFilePath, ErrMsg);

  if (Ok && WithHpp)
    Ok = Factory.createHppFile(R, NewFilePath, ErrMsg);

  if (Ok && WithUiParam)
    Ok = Factory.createParamUiCppFile(R, NewFilePath, ErrMsg);

  if (Ok && WithUiParam)
    Ok = Factory.createParamUiHppFile(R, NewFilePath, ErrMsg);

  if (Ok && ui->Json_checkBox->isChecked())
    Ok = Factory.createJsonFile(NewFilePath, ErrMsg);

  if (Ok)
  {
    m_NewWarePath = m_WareTypeDir.filePath(WareId);
    QDialog::accept();
  }
  else
  {
    if (m_WareTypeDir.exists(WareId))
      openfluid::tools::emptyDirectoryRecursively(m_WareTypeDir.absoluteFilePath(WareId).toStdString());
    QMessageBox::warning(this, tr("Error"), tr("Unable to create the ware \"%1\"").arg(ErrMsg));
  }
}


// =====================================================================
// =====================================================================


} } }  // namespaces
