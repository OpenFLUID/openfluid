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
#include <QUuid>

#include <openfluid/ui/config.hpp>
#include <openfluid/ui/waresdev/NewWareDialog.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>

#include "ui_NewWareDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


NewWareDialog::NewWareDialog(openfluid::ware::WareType Type, QWidget* Parent) :
    openfluid::ui::common::MessageDialog(Parent),
    ui(new Ui::NewWareDialog), m_WareType(Type), m_NewWarePath(""),
    m_UseSimSignature(false)
{
  ui->setupUi(this);

  m_WareTypeDir.setPath(openfluid::waresdev::WareSrcManager::instance()->getWareTypePath(m_WareType));

  ui->BextTypeComboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getBuilderExtTypeTexts());
  ui->BextCategoryComboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getBuilderExtCategoryTexts());

  QString WareId;
  QString SrcFilename;
  QString SrcClassname;

  switch (m_WareType)
  {
    case openfluid::ware::WareType::SIMULATOR:
      setupMessageUi(tr("Create a new simulator"));
      WareId = "sim.id";
      SrcFilename = "MySim.cpp";
      SrcClassname = "MySimulator";
      ui->BuilderExtWidget->setVisible(false);
      break;
    case openfluid::ware::WareType::OBSERVER:
      setupMessageUi(tr("Create a new observer"));
      WareId = "obs.id";
      SrcFilename = "MyObs.cpp";
      SrcClassname = "MyObserver";
      ui->Sim2docWidget->setVisible(false);
      ui->BuilderExtWidget->setVisible(false);
      break;
    case openfluid::ware::WareType::BUILDEREXT:
      setupMessageUi(tr("Create a new Builder extension"));
      WareId = "bext.id";
      SrcFilename = "MyExt.cpp";
      SrcClassname = "MyExtension";
      ui->UiWidget->setVisible(false);
      ui->Sim2docWidget->setVisible(false);
      break;
    default:
      break;
  }

  QString IDTooltip;
  QString CppTooltip;
  QString ClassTooltip;
  QRegExp CppRegExp = openfluid::waresdev::WareSrcFactory::getCppFilenameRegExp(CppTooltip);
  QRegExp ClassnameRegExp = openfluid::waresdev::WareSrcFactory::getClassnameRegExp(ClassTooltip);

  // "required" placeholder
  QString PlaceholderStr = getPlaceholderRequired();

  ui->IdEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getWareIdRegExp(IDTooltip), this));
  ui->IdEdit->setToolTip(IDTooltip);
  ui->IdEdit->setPlaceholderText(PlaceholderStr);
  ui->SourceFilenameEdit->setValidator(new QRegExpValidator(CppRegExp, this));
  ui->SourceFilenameEdit->setToolTip(CppTooltip);
  ui->SourceFilenameEdit->setPlaceholderText(PlaceholderStr);
  ui->UiParamSourceFilenameEdit->setValidator(new QRegExpValidator(CppRegExp, this));
  ui->UiParamSourceFilenameEdit->setToolTip(CppTooltip);
  ui->UiParamSourceFilenameEdit->setPlaceholderText(PlaceholderStr);
  ui->ClassNameEdit->setValidator(new QRegExpValidator(ClassnameRegExp, this));
  ui->ClassNameEdit->setToolTip(ClassTooltip);
  ui->ClassNameEdit->setPlaceholderText(PlaceholderStr);
  ui->UiParamClassNameEdit->setValidator(new QRegExpValidator(ClassnameRegExp, this));
  ui->UiParamClassNameEdit->setToolTip(ClassTooltip);
  ui->UiParamClassNameEdit->setPlaceholderText(PlaceholderStr);

  connect(ui->IdEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->SourceFilenameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->UiParamGroupBox, SIGNAL(toggled(bool)), this, SLOT(onInformationChanged()));
  connect(ui->UiParamSourceFilenameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->ClassNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->UiParamClassNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));

  ui->IdEdit->setText(WareId);
  ui->SourceFilenameEdit->setText(SrcFilename);
  ui->ClassNameEdit->setText(SrcClassname);
  ui->UiParamSourceFilenameEdit->setText("ParamsUiWidget.cpp");
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


void NewWareDialog::onInformationChanged()
{
  QString WarningMsg = "";

  if (!ui->IdEdit->hasAcceptableInput())
    WarningMsg = tr("Ware ID is empty");
  else if (m_WareTypeDir.exists(ui->IdEdit->text()))
    WarningMsg = tr("Ware ID already exists");
  else if (!ui->SourceFilenameEdit->hasAcceptableInput())
    WarningMsg = tr("Source file name must be of the form \"filexxx.cpp\"");
  else if (!ui->ClassNameEdit->hasAcceptableInput())
    WarningMsg = tr("Main class name is empty");
  else if (ui->UiParamGroupBox->isChecked() && !ui->UiParamSourceFilenameEdit->hasAcceptableInput())
    WarningMsg = tr("UI parameterization file name must be of the form \"filexxx.cpp\"");
  else if (ui->UiParamGroupBox->isChecked() && !ui->UiParamClassNameEdit->hasAcceptableInput())
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

  setMessage(Ok ? "" : WarningMsg);
}


// =====================================================================
// =====================================================================


void NewWareDialog::accept()
{
  QString WareId = ui->IdEdit->text();

  bool WithUiParam = ui->UiParamGroupBox->isChecked();
  bool WithHpp = false;

  openfluid::waresdev::WareSrcFactory Factory(m_WareType);
  Factory.setWareId(WareId);

  QString NewFilePath, ErrMsg;
  bool Ok;

  openfluid::waresdev::WareSrcFactory::Replacements Repl;

  Repl.ClassName = ui->ClassNameEdit->text();
  Repl.RootCppFilename = ui->SourceFilenameEdit->text();
  Repl.RootHppFilename = openfluid::waresdev::WareSrcFactory::getHppFilename(Repl.RootCppFilename);
  Repl.HppHeaderGuard = openfluid::waresdev::WareSrcFactory::getHeaderGuard(Repl.RootHppFilename);

  Repl.SignatureInfos = "  // Informations\n"
                     "  DECLARE_NAME(\"\")\n"
                     "  DECLARE_DESCRIPTION(\"\")\n"
                     "  DECLARE_VERSION(\"\")\n"
                     "  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL)\n";

  Repl.LinkUID = QUuid::createUuid().toString();

  if (m_WareType == openfluid::ware::WareType::SIMULATOR)
  {
    Repl.SimulatorSchedulingReturn = "DefaultDeltaT()";

    if (m_UseSimSignature)
    {
      Repl.SignatureInfos = openfluid::waresdev::WareSrcFactory::getSimulatorSignatureInfos(m_SimSignature);
      Repl.SimulatorSignatureData = openfluid::waresdev::WareSrcFactory::getSimulatorSignatureData(m_SimSignature);
      Repl.SimulatorInitCode = openfluid::waresdev::WareSrcFactory::getSimulatorInitCode(m_SimSignature);
      Repl.SimulatorRunCode = openfluid::waresdev::WareSrcFactory::getSimulatorRunCode(m_SimSignature);
      Repl.SimulatorSchedulingReturn =
          openfluid::waresdev::WareSrcFactory::getSimulatorSchedulingReturn(m_SimSignature);
    }
  }
  else if (m_WareType == openfluid::ware::WareType::OBSERVER)
  {

  }
  else if (m_WareType == openfluid::ware::WareType::BUILDEREXT)
  {
    Repl.BuilderExtModeIndex = ui->BextTypeComboBox->currentIndex();
    Repl.BuilderExtCategoryIndex = ui->BextCategoryComboBox->currentIndex();
    Repl.BuilderExtMenuText = ui->BextMenutextEdit->text();
    WithHpp = true;
    WithUiParam = false;
  }


  if (WithUiParam &&
      (m_WareType == openfluid::ware::WareType::SIMULATOR ||
      m_WareType == openfluid::ware::WareType::OBSERVER))
  {
    Repl.ParamsUiEnabled = true;
    Repl.ParamsUiClassname = ui->UiParamClassNameEdit->text();
    Repl.ParamsUiRootCppFilename = ui->UiParamSourceFilenameEdit->text();
    Repl.ParamsUiRootHppFilename = openfluid::waresdev::WareSrcFactory::getHppFilename(Repl.ParamsUiRootCppFilename);
    Repl.ParamsUiHeaderGuard = openfluid::waresdev::WareSrcFactory::getHeaderGuard(Repl.ParamsUiRootHppFilename);
    Repl.ParamsUiComment = "";
  }


  Ok = m_WareTypeDir.mkdir(WareId);
  if (!Ok)
    ErrMsg = tr("Unable to create the ware directory");
  else
    Ok = Factory.createCMakeListsFile(NewFilePath, ErrMsg);

  if (Ok)
    Ok = Factory.createCmakeConfigFile(Repl, NewFilePath, ErrMsg);

  if (Ok)
    Ok = Factory.createCppFile(Repl, NewFilePath, ErrMsg);

  if (Ok && WithHpp)
    Ok = Factory.createHppFile(Repl, NewFilePath, ErrMsg);

  if (Ok && WithUiParam)
    Ok = Factory.createParamUiCppFile(Repl, NewFilePath, ErrMsg);

  if (Ok && WithUiParam)
    Ok = Factory.createParamUiHppFile(Repl, NewFilePath, ErrMsg);

  if (Ok && ui->JsonCheckBox->isChecked())
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


} } }  // namespaces
