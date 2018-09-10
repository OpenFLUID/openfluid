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
 @file NewSrcFileAssistant.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <QMessageBox>
#include <QUuid>

#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>
#include <openfluid/ui/config.hpp>

#include "NewSrcFileAssistant.hpp"
#include "ui_NewSrcFileAssistant.h"
#include "ui_EmptyPage.h"
#include "ui_CppPage.h"
#include "ui_CMakeConfigPage.h"


namespace openfluid { namespace ui { namespace waresdev {


EmptyPage::EmptyPage(const QString& WarePath, QWidget* Parent) :
    QWizardPage(Parent), ui(new Ui::EmptyPage), m_WarePath(WarePath)
{
  ui->setupUi(this);

  ui->EmptyFileLabel->setText(m_FilePathPlaceholder);

  ui->EmptyFileLineEdit->setVisible(false);
  registerField("EmptyFilepath", ui->EmptyFileLineEdit);

  NewSrcFileAssistant::setStatus(m_DefaultMsg, tr("No file selected"), ui->MessageLabel, ui->MessageFrame);

  connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(onEmptyBrowseToolButtonClicked()));
}


// =====================================================================
// =====================================================================


EmptyPage::~EmptyPage()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EmptyPage::initialize()
{
  QString FilePathLabel = ui->EmptyFileLabel->text();

  if (FilePathLabel != m_FilePathPlaceholder)
    ui->EmptyFileLineEdit->setText(QDir(m_WarePath).absoluteFilePath(FilePathLabel));
}


// =====================================================================
// =====================================================================


bool EmptyPage::isComplete() const
{
  return !ui->EmptyFileLineEdit->text().isEmpty();
}


// =====================================================================
// =====================================================================


void EmptyPage::onEmptyBrowseToolButtonClicked()
{
  QString LineEditPath = ui->EmptyFileLineEdit->text();

  QString NewPath = WareExplorerDialog::getCreateFilePath(
      this, m_WarePath, LineEditPath.isEmpty() ? QDir(m_WarePath).relativeFilePath("file.txt") : LineEditPath);

  if (!NewPath.isEmpty())
  {
    ui->EmptyFileLabel->setText(QDir(m_WarePath).relativeFilePath(NewPath));
    ui->EmptyFileLineEdit->setText(NewPath);
    NewSrcFileAssistant::setStatus(m_DefaultMsg, "", ui->MessageLabel, ui->MessageFrame);
  }

  emit QWizardPage::completeChanged();
}


// =====================================================================
// =====================================================================


CppPage::CppPage(const QDir& WareDir, QWidget* Parent) :
    QWizardPage(Parent), ui(new Ui::CppPage), m_WareDir(WareDir), m_IsHpp(false)
{
  ui->setupUi(this);

  ui->BextTypeComboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getBuilderExtTypeTexts());
  ui->BextCategoryComboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getBuilderExtCategoryTexts());

  QString Tooltip;
  ui->ClassNameEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getClassnameRegExp(Tooltip), this));
  ui->ClassNameEdit->setToolTip(Tooltip);

  // "required" placeholder
  QString PlaceholderStr = QApplication::translate("openfluid::ui::config",openfluid::ui::config::PLACEHOLDER_REQUIRED);
  ui->ClassNameEdit->setPlaceholderText(PlaceholderStr);
  ui->SourceFilenameEdit->setPlaceholderText(PlaceholderStr);

  registerField("SourceFilename", ui->SourceFilenameEdit);
  registerField("Classname", ui->ClassNameEdit);
  registerField("BextType", ui->BextTypeComboBox);
  registerField("BextCategory", ui->BextCategoryComboBox);
  registerField("BextMenutext", ui->BextMenutextEdit);

  connect(ui->SourceFilenameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->ClassNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
}


// =====================================================================
// =====================================================================


CppPage::~CppPage()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void CppPage::initialize(bool IsHpp, bool IsUi, openfluid::ware::WareType Type)
{
  m_IsHpp = IsHpp;

  QString Tooltip;
  ui->SourceFilenameEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getCppFilenameRegExp(Tooltip, IsHpp), this));
  ui->SourceFilenameEdit->setToolTip(Tooltip);

  QString ClassName;
  QString SourceFilename;
  QString SourceFilename_label;

  if (IsUi)
  {
    ClassName = "ParamsUiWidget";
    SourceFilename = "ParamsUiWidget";

    if (IsHpp) // UiHpp
    {
      m_DefaultMsg = tr("Create the parameterization UI header file (.hpp)");
      SourceFilename_label = tr("Widget header file name (.hpp)");
    }
    else // UiCpp
    {
      m_DefaultMsg = tr("Create the parameterization UI source file (.cpp)");
      SourceFilename_label = tr("Widget source file name (.cpp)");
    }
  }
  else
  {
    switch (Type)
    {
      case openfluid::ware::WareType::SIMULATOR:
        ClassName = "MySimulator";
        SourceFilename = "MySim";
        break;
      case openfluid::ware::WareType::OBSERVER:
        ClassName = "MyObserver";
        SourceFilename = "MyObs";
        break;
      case openfluid::ware::WareType::BUILDEREXT:
        ClassName = "MyExtension";
        SourceFilename = "MyExt";
        break;
      default:
        break;
    }

    if (IsHpp) // MainHpp
    {
      m_DefaultMsg = tr("Create the main C++ header file (.hpp)");
      SourceFilename_label = tr("Header file name (.hpp)");
    }
    else // MainCpp
    {
      m_DefaultMsg = tr("Create the main C++ source file (.cpp)");
      SourceFilename_label = tr("Source file name (.cpp)");
    }
  }

  SourceFilename.append(IsHpp ? ".hpp" : ".cpp");

  ui->SourceFilename_label->setText(SourceFilename_label);
  ui->SourceFilenameEdit->setText(SourceFilename);
  ui->ClassNameEdit->setText(ClassName);

  bool IsBuilderExt = (Type == openfluid::ware::WareType::BUILDEREXT);
  ui->BuilderExtWidget->setVisible(IsBuilderExt);
  ui->BuilderExtGroupBox->setVisible(IsBuilderExt && !IsHpp);
}


// =====================================================================
// =====================================================================


void CppPage::onInformationChanged()
{
  QString WarningMsg = "";

  if (!ui->SourceFilenameEdit->hasAcceptableInput())
    WarningMsg = QString(tr("File name must be of the form \"filexxx.%1\"")).arg(m_IsHpp ? "hpp" : "cpp");
  else if (m_WareDir.exists(ui->SourceFilenameEdit->text()))
    WarningMsg = tr("File already exists");
  else if (!ui->ClassNameEdit->hasAcceptableInput())
    WarningMsg = tr("Class name is empty");

  NewSrcFileAssistant::setStatus(m_DefaultMsg, WarningMsg, ui->MessageLabel, ui->MessageFrame);

  emit QWizardPage::completeChanged();
}


// =====================================================================
// =====================================================================


bool CppPage::isComplete() const
{
  return (ui->SourceFilenameEdit->hasAcceptableInput() && !m_WareDir.exists(ui->SourceFilenameEdit->text())
          && ui->ClassNameEdit->hasAcceptableInput());
}


// =====================================================================
// =====================================================================


CMakeConfigPage::CMakeConfigPage(openfluid::ware::WareType Type, QWidget* Parent) :
    QWizardPage(Parent), ui(new Ui::CMakeConfigPage)
{
  ui->setupUi(this);

  bool IsSim = false;
  QString RootFilename;

  switch (Type)
  {
    case openfluid::ware::WareType::SIMULATOR:
      IsSim = true;
      RootFilename = "MySim.cpp";
      break;
    case openfluid::ware::WareType::OBSERVER:
      RootFilename = "MyObs.cpp";
      break;
    case openfluid::ware::WareType::BUILDEREXT:
      RootFilename = "MyExt.cpp";
      break;
    default:
      break;
  }

  ui->RootFilenameEdit->setText(RootFilename);

  ui->Sim2docComboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getSim2docModeTexts());
  ui->Sim2docWidget->setVisible(IsSim);

  QString Tooltip;
  ui->RootFilenameEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getCppFilenameRegExp(Tooltip), this));
  ui->RootFilenameEdit->setToolTip(Tooltip);
  ui->RootFilenameEdit->setPlaceholderText(QApplication::translate("openfluid::ui::config",
                                                                        openfluid::ui::config::PLACEHOLDER_REQUIRED));

  m_DefaultMsg = tr("Create the \"CMake.in.config\" file");
  NewSrcFileAssistant::setStatus(m_DefaultMsg, "", ui->MessageLabel, ui->MessageFrame);

  registerField("RootFilename", ui->RootFilenameEdit);
  registerField("Sim2docMode", ui->Sim2docComboBox);
  registerField("Sim2docInstall", ui->Sim2docCheckBox);

  connect(ui->RootFilenameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onRootfilenameTextChanged()));
}


// =====================================================================
// =====================================================================


CMakeConfigPage::~CMakeConfigPage()
{
  delete ui;
}


// =====================================================================
// =====================================================================


bool CMakeConfigPage::isComplete() const
{
  return ui->RootFilenameEdit->hasAcceptableInput();
}


// =====================================================================
// =====================================================================


void CMakeConfigPage::onRootfilenameTextChanged()
{
  NewSrcFileAssistant::setStatus(
      m_DefaultMsg,
      ui->RootFilenameEdit->hasAcceptableInput() ? "" : tr("File name must be of the form \"filexxx.cpp\""),
      ui->MessageLabel, ui->MessageFrame);

  emit QWizardPage::completeChanged();
}


// =====================================================================
// =====================================================================


NewSrcFileAssistant::NewSrcFileAssistant(const openfluid::waresdev::WareSrcContainer& Container, QWidget* Parent) :
    QWizard(Parent), ui(new Ui::NewSrcFileAssistant), mref_Container(Container)
{
  ui->setupUi(this);

  QString BgColorCss = QString("background-color: %1;").arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR);
  ui->MessageFrame->setStyleSheet(BgColorCss);

  ui->buttonGroup->setId(ui->WareshubRadioButton, -1);
  ui->buttonGroup->setId(ui->CMakeListsRadioButton, -1);
  ui->buttonGroup->setId(ui->EmptyRadioButton, static_cast<int>(PageType::EMPTY_PAGE));
  ui->buttonGroup->setId(ui->CMakeConfigRadioButton, static_cast<int>(PageType::CMAKECONFIG_PAGE));
  ui->buttonGroup->setId(ui->CppRadioButton, static_cast<int>(PageType::CPP_PAGE));
  ui->buttonGroup->setId(ui->HppRadioButton, static_cast<int>(PageType::HPP_PAGE));
  ui->buttonGroup->setId(ui->CppUiRadioButton, static_cast<int>(PageType::CPP_UI_PAGE));
  ui->buttonGroup->setId(ui->HppUiRadioButton, static_cast<int>(PageType::HPP_UI_PAGE));

  QString MainCpp = mref_Container.getMainCppPath();
  QString UiParamCpp = mref_Container.getUiParamCppPath();

  ui->WareshubRadioButton->setEnabled(mref_Container.getJsonPath().isEmpty());
  ui->CMakeListsRadioButton->setEnabled(mref_Container.getCMakeListsPath().isEmpty());
  ui->CMakeConfigRadioButton->setEnabled(mref_Container.getCMakeConfigPath().isEmpty());
  ui->CppRadioButton->setEnabled(MainCpp.isEmpty());
  ui->HppRadioButton->setEnabled(!QFile::exists(openfluid::waresdev::WareSrcFactory::getHppFilename(MainCpp)));
  ui->CppUiRadioButton->setEnabled(UiParamCpp.isEmpty());
  ui->HppUiRadioButton->setEnabled(!QFile::exists(openfluid::waresdev::WareSrcFactory::getHppFilename(UiParamCpp)));

  for (QRadioButton* Bt : findChildren<QRadioButton*>())
  {
    if (!Bt->isEnabled())
      Bt->setToolTip(tr("This file already exists"));
  }

  openfluid::ware::WareType Type = mref_Container.getType();

  if (Type == openfluid::ware::WareType::BUILDEREXT)
  {
    ui->CppUiRadioButton->setVisible(false);
    ui->HppUiRadioButton->setVisible(false);
    ui->UiLine->setVisible(false);
  }
  else
  {
    ui->HppRadioButton->setVisible(false);
  }

// existing page : IntroPage (index 0)
  addPage(new EmptyPage(mref_Container.getAbsolutePath(), this)); // index 1
  addPage(new CMakeConfigPage(Type, this)); // index 2
  addPage(new CppPage(QDir(mref_Container.getAbsolutePath()), this)); // index 3

  mp_Factory = new openfluid::waresdev::WareSrcFactory(Type);
  mp_Factory->setWareId(Container.getID());

  connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onFileTypeButtonClicked(int)));
}


// =====================================================================
// =====================================================================


NewSrcFileAssistant::~NewSrcFileAssistant()
{
  delete ui;
  delete mp_Factory;
}


// =====================================================================
// =====================================================================


int NewSrcFileAssistant::nextId() const
{
  int CheckedID = ui->buttonGroup->checkedId();

  if (currentId() != static_cast<int>(PageType::INTRO_PAGE) || CheckedID < 0)
    return -1;

  if (CheckedID >= static_cast<int>(PageType::CPP_PAGE))
    return static_cast<int>(PageType::CPP_PAGE);

  return CheckedID;
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::initializePage(int Id)
{
  if (Id == static_cast<int>(PageType::CPP_PAGE))
  {
    int CheckedId = ui->buttonGroup->checkedId();

    bool IsHpp = (CheckedId == static_cast<int>(PageType::HPP_PAGE)
                  || CheckedId == static_cast<int>(PageType::HPP_UI_PAGE));
    bool IsUi = (CheckedId == static_cast<int>(PageType::CPP_UI_PAGE)
                 || CheckedId == static_cast<int>(PageType::HPP_UI_PAGE));

    qobject_cast<CppPage*>(page(static_cast<int>(PageType::CPP_PAGE)))->initialize(IsHpp, IsUi,
                                                                                   mref_Container.getType());
  }
  else if (Id == static_cast<int>(PageType::EMPTY_PAGE))
    qobject_cast<EmptyPage*>(page(static_cast<int>(PageType::EMPTY_PAGE)))->initialize();
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::onFileTypeButtonClicked(int Id)
{
  currentPage()->setFinalPage(Id < 0);
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::accept()
{
  QString NewFilePath, ErrMsg;
  bool Ok = false;

  switch (currentId())
  {
    case static_cast<int>(PageType::INTRO_PAGE):
      if (ui->WareshubRadioButton->isChecked())
        Ok = mp_Factory->createJsonFile(NewFilePath, ErrMsg);
      else if (ui->CMakeListsRadioButton->isChecked())
        Ok = mp_Factory->createCMakeListsFile(NewFilePath, ErrMsg);
      break;
    case static_cast<int>(PageType::EMPTY_PAGE):
      {
      NewFilePath = field("EmptyFilepath").toString();

      Ok = QDir().mkpath(QFileInfo(NewFilePath).absolutePath());
      if (!Ok)
        ErrMsg = tr("Unable to create the path \"%1\"").arg(NewFilePath);
      else
      {
        Ok = QFile(NewFilePath).open(QIODevice::ReadWrite | QIODevice::Text);
        if (!Ok)
          ErrMsg = tr("Unable to create the file \"%1\"").arg(NewFilePath);
      }
      break;
    }
    case static_cast<int>(PageType::CPP_PAGE):
      {
      openfluid::waresdev::WareSrcFactory::Replacements R;

      bool IsBuilderExt = (mref_Container.getType() == openfluid::ware::WareType::BUILDEREXT);

      switch (ui->buttonGroup->checkedId())
      {
        case static_cast<int>(PageType::CPP_PAGE):
          {
          R.ClassName = field("Classname").toString();
          R.RootCppFilename = field("SourceFilename").toString();
          if (IsBuilderExt)
          {
            R.RootHppFilename = openfluid::waresdev::WareSrcFactory::getHppFilename(R.RootCppFilename);
            R.BuilderExtModeIndex = field("BextType").toInt();
            R.BuilderExtCategoryIndex = field("BextCategory").toInt();
            R.BuilderExtMenuText = field("BextMenutext").toString();
          }

          Ok = mp_Factory->createCppFile(R, NewFilePath, ErrMsg);
          break;
        }
        case static_cast<int>(PageType::HPP_PAGE):
          {
          R.ClassName = field("Classname").toString();
          R.RootHppFilename = field("SourceFilename").toString();
          if (IsBuilderExt)
          {
            R.BuilderExtModeIndex = field("BextType").toInt();
            R.HppHeaderGuard = openfluid::waresdev::WareSrcFactory::getHeaderGuard(R.RootHppFilename);
          }

          Ok = mp_Factory->createHppFile(R, NewFilePath, ErrMsg);
          break;
        }
        case static_cast<int>(PageType::CPP_UI_PAGE):
          R.ParamsUiClassname = field("Classname").toString();
          R.ParamsUiRootCppFilename = field("SourceFilename").toString();
          R.ParamsUiRootHppFilename = openfluid::waresdev::WareSrcFactory::getHppFilename(R.ParamsUiRootCppFilename);

          Ok = mp_Factory->createParamUiCppFile(R, NewFilePath, ErrMsg);
          break;
        case static_cast<int>(PageType::HPP_UI_PAGE):
          R.ParamsUiClassname = field("Classname").toString();
          R.ParamsUiRootHppFilename = field("SourceFilename").toString();
          R.ParamsUiHeaderGuard = openfluid::waresdev::WareSrcFactory::getHeaderGuard(R.ParamsUiRootHppFilename);

          Ok = mp_Factory->createParamUiHppFile(R, NewFilePath, ErrMsg);

          break;
        default:
          break;
      }
      break;
    }
    case static_cast<int>(PageType::CMAKECONFIG_PAGE):
      {
      openfluid::waresdev::WareSrcFactory::Replacements R;
      R.RootCppFilename = field("RootFilename").toString();
      R.LinkUID = QUuid::createUuid().toString();
      R.Sim2docModeIndex = field("Sim2docMode").toInt();
      R.Sim2docInstall = field("Sim2docInstall").toBool();

      Ok = mp_Factory->createCmakeConfigFile(R, NewFilePath, ErrMsg);
      break;
    }
    default:
      break;
  }

  if (Ok)
  {
    m_NewFilePath = NewFilePath;
    QDialog::accept();
  }
  else
    QMessageBox::warning(this, tr("Error"), tr("Unable to create the file \"%1\"").arg(ErrMsg));
}


// =====================================================================
// =====================================================================


QString NewSrcFileAssistant::getNewFilePath()
{
  return m_NewFilePath;
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::setStatus(const QString DefaultMsg, const QString WarningMsg, QLabel* MsgLabel,
  QFrame* MsgFrame)
{
  bool Ok = WarningMsg.isEmpty();

  MsgLabel->setText(Ok ? DefaultMsg : WarningMsg);

  MsgFrame->setStyleSheet(
      QString("background-color: %1;").arg(
          Ok ? openfluid::ui::config::DIALOGBANNER_BGCOLOR : openfluid::ui::config::DIALOGBANNER_WARNBGCOLOR));
}


} } } // namespaces
