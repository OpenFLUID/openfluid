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


#include "NewSrcFileAssistant.hpp"
#include "ui_NewSrcFileAssistant.h"
#include "ui_CppPage.h"
#include "ui_CMakeConfigPage.h"

#include <QMessageBox>
#include <QUuid>

#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>
#include <openfluid/ui/config.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


CppPage::CppPage(const QDir& WareDir, QWidget* Parent) :
    QWizardPage(Parent), ui(new Ui::CppPage), m_WareDir(WareDir), m_IsHpp(false)
{
  ui->setupUi(this);

  ui->BextType_comboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getBuilderExtTypeTexts());
  ui->BextCategory_comboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getBuilderExtCategoryTexts());

  QString Tooltip;
  ui->ClassName_lineEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getClassnameRegExp(Tooltip), this));
  ui->ClassName_lineEdit->setToolTip(Tooltip);
  ui->ClassName_lineEdit->setPlaceholderText(openfluid::ui::config::PLACEHOLDER_REQUIRED);
  ui->SourceFilename_lineEdit->setPlaceholderText(openfluid::ui::config::PLACEHOLDER_REQUIRED);

  registerField("SourceFilename", ui->SourceFilename_lineEdit);
  registerField("Classname", ui->ClassName_lineEdit);
  registerField("BextType", ui->BextType_comboBox);
  registerField("BextCategory", ui->BextCategory_comboBox);
  registerField("BextMenutext", ui->BextMenutext_lineEdit);

  connect(ui->SourceFilename_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->ClassName_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
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
  ui->SourceFilename_lineEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getCppFilenameRegExp(Tooltip, IsHpp), this));
  ui->SourceFilename_lineEdit->setToolTip(Tooltip);

  QString ClassName;
  QString SourceFilename;
  QString SourceFilename_label;

  if (IsUi)
  {
    ClassName = "ParamsUiWidget";
    SourceFilename = "ParamsUiWidget";

    if (IsHpp)  // UiHpp
    {
      m_DefaultMsg = tr("Create the parameterization UI header file (.hpp)");
      SourceFilename_label = tr("Widget header file name (.hpp)");
    }
    else  // UiCpp
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

    if (IsHpp)  // MainHpp
    {
      m_DefaultMsg = tr("Create the main C++ header file (.hpp)");
      SourceFilename_label = tr("Header file name (.hpp)");
    }
    else  // MainCpp
    {
      m_DefaultMsg = tr("Create the main C++ source file (.cpp)");
      SourceFilename_label = tr("Source file name (.cpp)");
    }
  }

  SourceFilename.append(IsHpp ? ".hpp" : ".cpp");

  ui->SourceFilename_label->setText(SourceFilename_label);
  ui->SourceFilename_lineEdit->setText(SourceFilename);
  ui->ClassName_lineEdit->setText(ClassName);

  bool IsBuilderExt = (Type == openfluid::ware::WareType::BUILDEREXT);
  ui->BuilderExt_widget->setVisible(IsBuilderExt);
  ui->BuilderExt_groupBox->setVisible(IsBuilderExt && !IsHpp);
}


// =====================================================================
// =====================================================================


void CppPage::onInformationChanged()
{
  QString WarningMsg = "";

  if (!ui->SourceFilename_lineEdit->hasAcceptableInput())
    WarningMsg = QString(tr("File name must be of the form \"filexxx.%1\"")).arg(m_IsHpp ? "hpp" : "cpp");
  else if (m_WareDir.exists(ui->SourceFilename_lineEdit->text()))
    WarningMsg = tr("File already exists");
  else if (!ui->ClassName_lineEdit->hasAcceptableInput())
    WarningMsg = tr("Class name is empty");

  NewSrcFileAssistant::setStatus(m_DefaultMsg, WarningMsg, ui->MessageLabel, ui->MessageFrame);

  emit QWizardPage::completeChanged();
}


// =====================================================================
// =====================================================================


bool CppPage::isComplete() const
{
  return (ui->SourceFilename_lineEdit->hasAcceptableInput() && !m_WareDir.exists(ui->SourceFilename_lineEdit->text())
          && ui->ClassName_lineEdit->hasAcceptableInput());
}


// =====================================================================
// =====================================================================

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

  ui->RootFilename_lineEdit->setText(RootFilename);

  ui->Sim2doc_comboBox->addItems(openfluid::waresdev::WareSrcFactory::Replacements::getSim2docModeTexts());
  ui->Sim2doc_widget->setVisible(IsSim);

  QString Tooltip;
  ui->RootFilename_lineEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getCppFilenameRegExp(Tooltip), this));
  ui->RootFilename_lineEdit->setToolTip(Tooltip);
  ui->RootFilename_lineEdit->setPlaceholderText(openfluid::ui::config::PLACEHOLDER_REQUIRED);

  m_DefaultMsg = tr("Create the \"CMake.in.config\" file");
  NewSrcFileAssistant::setStatus(m_DefaultMsg, "", ui->MessageLabel, ui->MessageFrame);

  registerField("RootFilename", ui->RootFilename_lineEdit);
  registerField("Sim2docMode", ui->Sim2doc_comboBox);
  registerField("Sim2docInstall", ui->Sim2doc_checkBox);

  connect(ui->RootFilename_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onRootfilenameTextChanged()));
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
  return ui->RootFilename_lineEdit->hasAcceptableInput();
}


// =====================================================================
// =====================================================================

void CMakeConfigPage::onRootfilenameTextChanged()
{
  NewSrcFileAssistant::setStatus(
      m_DefaultMsg,
      ui->RootFilename_lineEdit->hasAcceptableInput() ? "" : tr("File name must be of the form \"filexxx.cpp\""),
      ui->MessageLabel, ui->MessageFrame);

  emit QWizardPage::completeChanged();
}


// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


NewSrcFileAssistant::NewSrcFileAssistant(const openfluid::waresdev::WareSrcContainer& Container, QWidget* Parent) :
    QWizard(Parent), ui(new Ui::NewSrcFileAssistant), mref_Container(Container)
{
  ui->setupUi(this);

  QString BgColorCss = QString("background-color: %1;").arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR);
  ui->MessageFrame->setStyleSheet(BgColorCss);
  ui->MessageFrame_2->setStyleSheet(BgColorCss);

  ui->buttonGroup->setId(ui->Wareshub_radioButton, -1);
  ui->buttonGroup->setId(ui->CMakeLists_radioButton, -1);
  ui->buttonGroup->setId(ui->Empty_radioButton, EMPTY_PAGE);
  ui->buttonGroup->setId(ui->CMakeConfig_radioButton, CMAKECONFIG_PAGE);
  ui->buttonGroup->setId(ui->Cpp_radioButton, CPP_PAGE);
  ui->buttonGroup->setId(ui->Hpp_radioButton, HPP_PAGE);
  ui->buttonGroup->setId(ui->CppUi_radioButton, CPP_UI_PAGE);
  ui->buttonGroup->setId(ui->HppUi_radioButton, HPP_UI_PAGE);

  QString MainCpp = mref_Container.getMainCppPath();
  QString UiParamCpp = mref_Container.getUiParamCppPath();

  ui->Wareshub_radioButton->setEnabled(mref_Container.getJsonPath().isEmpty());
  ui->CMakeLists_radioButton->setEnabled(mref_Container.getCMakeListsPath().isEmpty());
  ui->CMakeConfig_radioButton->setEnabled(mref_Container.getCMakeConfigPath().isEmpty());
  ui->Cpp_radioButton->setEnabled(MainCpp.isEmpty());
  ui->Hpp_radioButton->setEnabled(!QFile::exists(openfluid::waresdev::WareSrcFactory::getHppFilename(MainCpp)));
  ui->CppUi_radioButton->setEnabled(UiParamCpp.isEmpty());
  ui->HppUi_radioButton->setEnabled(!QFile::exists(openfluid::waresdev::WareSrcFactory::getHppFilename(UiParamCpp)));

  foreach(QRadioButton* Bt,findChildren<QRadioButton*>())
  {
    if(!Bt->isEnabled())
      Bt->setToolTip(tr("This file already exists"));
  }

  openfluid::ware::WareType Type = mref_Container.getType();

  if (Type == openfluid::ware::WareType::BUILDEREXT)
  {
    ui->CppUi_radioButton->setVisible(false);
    ui->HppUi_radioButton->setVisible(false);
    ui->Ui_line->setVisible(false);
  }
  else
  {
    ui->Hpp_radioButton->setVisible(false);
  }

// existing pages : IntroPage (index 0), EmptyPage (index 1)
  addPage(new CMakeConfigPage(Type, this));  // index 2
  addPage(new CppPage(QDir(mref_Container.getAbsolutePath()), this));  // index 3

  mp_Factory = new openfluid::waresdev::WareSrcFactory(Type);
  mp_Factory->setWareId(Container.getName());

  connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onFileTypeButtonClicked(int)));
  connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(onEmptyBrowseToolButtonClicked()));
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

  if (currentId() != INTRO_PAGE || CheckedID < 0)
    return -1;

  if (CheckedID >= CPP_PAGE)
    return CPP_PAGE;

  return CheckedID;
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::initializePage(int Id)
{
  if (Id == CPP_PAGE)
  {
    int CheckedId = ui->buttonGroup->checkedId();

    bool IsHpp = (CheckedId == HPP_PAGE || CheckedId == HPP_UI_PAGE);
    bool IsUi = (CheckedId == CPP_UI_PAGE || CheckedId == HPP_UI_PAGE);

    qobject_cast<CppPage*>(page(CPP_PAGE))->initialize(IsHpp, IsUi, mref_Container.getType());
  }
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::onFileTypeButtonClicked(int Id)
{
  currentPage()->setFinalPage(Id < 0);
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::onEmptyBrowseToolButtonClicked()
{
  QString WarePath = mref_Container.getAbsolutePath();

  QString NewPath = WareExplorerDialog::getCreateFilePath(this, WarePath, "");

  if (NewPath.isEmpty())
    return;

  ui->EmptyFile_label->setText(QDir(WarePath).relativeFilePath(NewPath));

  m_NewFilePath = NewPath;
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::accept()
{
  QString NewFilePath, ErrMsg;
  bool Ok = false;

  switch (currentId())
  {
    case INTRO_PAGE:
      if (ui->Wareshub_radioButton->isChecked())
        Ok = mp_Factory->createJsonFile(NewFilePath, ErrMsg);
      else if (ui->CMakeLists_radioButton->isChecked())
        Ok = mp_Factory->createCMakeListsFile(NewFilePath, ErrMsg);
      break;
    case EMPTY_PAGE:
    {
      NewFilePath = m_NewFilePath;

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
    case CPP_PAGE:
    {
      openfluid::waresdev::WareSrcFactory::Replacements R;

      bool IsBuilderExt = (mref_Container.getType() == openfluid::ware::WareType::BUILDEREXT);

      switch (ui->buttonGroup->checkedId())
      {
        case CPP_PAGE:
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
        case HPP_PAGE:
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
        case CPP_UI_PAGE:
          R.ParamsUiClassname = field("Classname").toString();
          R.ParamsUiRootCppFilename = field("SourceFilename").toString();
          R.ParamsUiRootHppFilename = openfluid::waresdev::WareSrcFactory::getHppFilename(R.ParamsUiRootCppFilename);

          Ok = mp_Factory->createParamUiCppFile(R, NewFilePath, ErrMsg);
          break;
        case HPP_UI_PAGE:
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
    case CMAKECONFIG_PAGE:
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


// =====================================================================
// =====================================================================


} } }  // namespaces
