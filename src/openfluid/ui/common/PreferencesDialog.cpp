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
  @file PreferencesDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QLocale>
#include <QMessageBox>
#include <QColorDialog>
#include <QSignalMapper>

#include <openfluid/config.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/ui/QtHelpers.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/common/PreferencesDialog.hpp>
#include <openfluid/ui/common/WaresSearchPathsWidget.hpp>
#include <openfluid/ui/common/DetectQtDevToolsDialog.hpp>

#include "ui_PreferencesDialog.h"
#include "ui_WaresSearchPathsWidget.h"


namespace openfluid { namespace ui { namespace common {


PreferencesDialog::PreferencesDialog(QWidget* Parent, DisplayMode Mode):
  MessageDialog(Parent), ui(new Ui::PreferencesDialog),
  m_RecentsChanged(false),
  m_SimPathsChanged(false), m_ObsPathsChanged(false), m_WaresWatchingChanged(false), m_TextEditorSettingsChanged(false),
  m_RestartRequired(false),
  m_OriginalLangIndex(0), m_Mode(Mode)
{
  mp_PrefsMan = openfluid::base::PreferencesManager::instance();
  mp_WorksMan = openfluid::base::WorkspaceManager::instance();

  setWindowModality(Qt::ApplicationModal);

  ui->setupUi(this);

  setupMessageUi(tr("Preferences"),QDialogButtonBox::NoButton);


  QTreeWidgetItem *PrefItem;

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("General"));
  PrefItem->setData(0,Qt::UserRole,static_cast<int>(PagesIndexes::GENERAL_PAGE));

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Workspaces"));
  PrefItem->setData(0,Qt::UserRole,static_cast<int>(PagesIndexes::WORKSPACES_PAGE));

  if (m_Mode == DisplayMode::BUILDER || m_Mode == DisplayMode::FULL)
  {
    PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
    PrefItem->setText(0,tr("Interface"));
    PrefItem->setData(0,Qt::UserRole,static_cast<int>(PagesIndexes::BUILDER_PAGE));

    PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
    PrefItem->setText(0,tr("Simulations"));
    PrefItem->setData(0,Qt::UserRole,static_cast<int>(PagesIndexes::SIMULATION_PAGE));
  }

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Development tools"));
  PrefItem->setData(0,Qt::UserRole,static_cast<int>(PagesIndexes::DEVENV_PAGE));

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Code editor"));
  PrefItem->setData(0,Qt::UserRole,static_cast<int>(PagesIndexes::DEVEDITOR_PAGE));


  ui->WorkspacesPathsWidget->setAllowEmpty(false);
  ui->WaresPathsTabWidget->setCurrentIndex(0);

  if (!(m_Mode == DisplayMode::BUILDER || m_Mode == DisplayMode::FULL))
  {
    ui->WaresPathsTabWidget->removeTab(2);
  }


  initialize();

  connect(ui->LangComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateLanguage(int)));
  connect(ui->ClearRecentsButton,SIGNAL(clicked()),this,SLOT(clearRecentsList()));
  connect(ui->RunAutoSaveCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableAutoSaveBeforeRun(bool)));
  connect(ui->ItemRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmItemRemoval(bool)));
  connect(ui->ParamRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmParamRemoval(bool)));
  connect(ui->WatchCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableWatchers(bool)));

  connect(ui->UnitsRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmUnitsRemoval(bool)));
  connect(ui->ConnectionsRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmConnectionsRemoval(bool)));
  connect(ui->AttributesRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmAttributesRemoval(bool)));


  connect(ui->WorkspacesPathsWidget,SIGNAL(pathsUpdated()),this,SLOT(processWorkspacesPathsUpdate()));
  connect(ui->ExtToolsWidget,SIGNAL(toolsUpdated()),this,SLOT(processExtToolsUpdate()));

  connect(ui->SimulatorsSearchPathsWidget,SIGNAL(userPathsUpdated()),this,SLOT(processSimUserPathsUpdate()));
  connect(ui->ObserversSearchPathsWidget,SIGNAL(userPathsUpdated()),this,SLOT(processObsUserPathsUpdate()));
  connect(ui->BuilderextsSearchPathsWidget,SIGNAL(userPathsUpdated()),this,SLOT(processBextUserPathsUpdate()));


  connect(ui->DeltaTSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateDeltaT(int)));
  connect(ui->PeriodWidget,SIGNAL(beginChanged(const QDateTime&)),this,SLOT(updatePeriodBegin(const QDateTime&)));
  connect(ui->PeriodWidget,SIGNAL(endChanged(const QDateTime&)),this,SLOT(updatePeriodEnd(const QDateTime&)));

  connect(ui->SyntaxHLCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableSyntaxHighlighting(bool)));
  connect(ui->CurrentLineHLCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableCurrentLineHighlighting(bool)));
  connect(ui->CurrentLineColorButton, SIGNAL(clicked()), this, SLOT(changeCurrentLineColor()));
  connect(ui->FontComboBox, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(changeCurrentFont(const QFont&)));
  connect(ui->LineWrappingCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableLineWrapping(bool)));
  connect(ui->IndentSpaceNbSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeIndentSpaceNumber(int)));
  connect(ui->ShowInvisibleCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableInvisibleCharsDisplay(bool)));
  connect(ui->ShowCarriageReturnCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableCarriageReturnDisplay(bool)));
  connect(ui->TextEditorApplyButton, SIGNAL(clicked()), this, SLOT(applyTextEditorSettings()));
  connect(ui->TextEditorRestoreDefaultsButton, SIGNAL(clicked()), this, SLOT(restoreDefaultsTextEditorSettings()));

  connect(ui->BuildAutoSaveCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableAutosaveBeforeBuild(bool)));
  connect(ui->ConfigPathEnvEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateDevConfigPATH()));
  connect(ui->ConfigGeneratorEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateDevConfigGenerator()));
  connect(ui->ConfigOptionsEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateDevConfigOptions()));
  connect(ui->BuildPathEnvEdit,SIGNAL(textEdited(const QString&)),this,SLOT(updateDevBuildPATH()));
  connect(ui->ShowPathCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateDevShowPATH(bool)));

#if !defined(OPENFLUID_OS_WINDOWS)
  ui->DetectQtDevWidget->setVisible(false);
#else
  connect(ui->DetectQtDevButton,SIGNAL(clicked()),this,SLOT(detectQtDevToolsMinGW()));
#endif

  if(Mode == DisplayMode::DEVSTUDIO || Mode == DisplayMode::FULL)
  {
    connect(ui->SslNoVerifyCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateDevSslNoVerify(bool)));
    connect(ui->CheckoutCurrentOFCheckBox,SIGNAL(toggled(bool)),
            this,SLOT(updateDevCheckoutCurrentOpenFLUIDVersion(bool)));
  }
  else
  {
    ui->GitGroupBox->setVisible(false);
  }

  connect(ui->PrefsTreeWidget,
          SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
          this, SLOT(changePage(QTreeWidgetItem*,QTreeWidgetItem*)));

  ui->PrefsTreeWidget->setCurrentItem(ui->PrefsTreeWidget->topLevelItem(0));


  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


PreferencesDialog::~PreferencesDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


QString PreferencesDialog::getLanguageAsPrettyString(const QString& LangCode)
{
  QLocale Loc(LangCode);
  QString LangName;

  if (Loc.language() == QLocale::English)
  {
    LangName = QLocale::languageToString(Loc.language());
  }
  else
  {
    LangName = Loc.nativeLanguageName();
  }

  LangName[0] = LangName[0].toUpper();
  QString CountryName = Loc.nativeCountryName();

  return QString("%1 (%2)").arg(LangName).arg(Loc.nativeCountryName());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::initialize()
{
  // Interface language
  QStringList AvailLangCodes = 
    openfluid::ui::toQStringList(openfluid::base::PreferencesManager::getAvailableUILanguages());

  ui->LangComboBox->addItem(getLanguageAsPrettyString("en_GB"),"default");
  for (auto Code : AvailLangCodes)
  {
    ui->LangComboBox->addItem(getLanguageAsPrettyString(Code),Code);
  }
  ui->LangComboBox->setCurrentIndex(ui->LangComboBox->findData(QString::fromStdString(mp_PrefsMan->getUILanguage())));

  m_OriginalLangIndex = ui->LangComboBox->currentIndex();

  ui->RestartLabel->setText("");
 

  // Workspaces paths
  ui->WorkspacesPathsWidget->setPathsList(openfluid::ui::toQStringList(mp_PrefsMan->getWorkspacesPaths()));

  m_OriginalActiveWorkspace = QString::fromStdString(mp_PrefsMan->getCurrentWorkspacePath());


  if (m_Mode == DisplayMode::BUILDER || m_Mode == DisplayMode::FULL)
  {
    // Wares search paths
    ui->SimulatorsSearchPathsWidget->initialize(
      openfluid::ui::toQStringList(mp_PrefsMan->getBuilderExtraSimulatorsPaths()),
      openfluid::ui::toQStringList(openfluid::base::Environment::getDefaultSimulatorsDirs())
    );
    ui->ObserversSearchPathsWidget->initialize(
      openfluid::ui::toQStringList(mp_PrefsMan->getBuilderExtraObserversPaths()),
      openfluid::ui::toQStringList(openfluid::base::Environment::getDefaultObserversDirs())
    );
    ui->BuilderextsSearchPathsWidget->initialize(
      openfluid::ui::toQStringList(mp_PrefsMan->getBuilderExtraExtensionsPaths()),
      openfluid::ui::toQStringList(openfluid::base::Environment::getDefaultBuilderextsDirs())
    );

    // interface
    ui->RunAutoSaveCheckBox->setChecked(mp_PrefsMan->isBuilderAutomaticSaveBeforeRun());

    ui->ItemRemovalCheckBox->setChecked(mp_PrefsMan->isBuilderWareRemovalConfirm());
    ui->ParamRemovalCheckBox->setChecked(mp_PrefsMan->isBuilderParamRemovalConfirm());
    ui->WatchCheckBox->setChecked(mp_PrefsMan->isBuilderWaresWatchersActive());

    ui->UnitsRemovalCheckBox->setChecked(mp_PrefsMan->isBuilderSpatialUnitsRemovalConfirm());
    ui->ConnectionsRemovalCheckBox->setChecked(mp_PrefsMan->isBuilderSpatialConnsRemovalConfirm());
    ui->AttributesRemovalCheckBox->setChecked(mp_PrefsMan->isBuilderSpatialAttrsRemovalConfirm());


    // Simulations
    ui->DeltaTSpinBox->setValue(mp_PrefsMan->getBuilderDeltaT());

    ui->PeriodWidget->initialize(
      QDateTime::fromString(QString::fromStdString(mp_PrefsMan->getBuilderBeginDate()),"yyyy-MM-dd HH:mm:ss"),
      QDateTime::fromString(QString::fromStdString(mp_PrefsMan->getBuilderEndDate()),"yyyy-MM-dd HH:mm:ss")
    );

  }

  // Development tools
  ui->BuildAutoSaveCheckBox->setChecked(mp_PrefsMan->isWaresdevAutomaticSaveBeforeBuild());
  ui->ConfigPathEnvEdit->setText(QString::fromStdString(mp_PrefsMan->getWaresdevConfigureEnv("PATH")));
  ui->ConfigGeneratorEdit->setText(QString::fromStdString(mp_PrefsMan->getWaresdevConfigureGenerator()));
  ui->ConfigOptionsEdit->setText(QString::fromStdString(mp_PrefsMan->getWaresdevConfigureOptions()));
  ui->BuildPathEnvEdit->setText(QString::fromStdString(mp_PrefsMan->getWaresdevBuildEnv("PATH")));
  ui->ShowPathCheckBox->setChecked(mp_PrefsMan->isWaresdevShowCommandEnv("PATH"));
  ui->SslNoVerifyCheckBox->setChecked(mp_PrefsMan->isWaresdevGitSslNoVerify());
  ui->CheckoutCurrentOFCheckBox->setChecked(mp_PrefsMan->isWaresdevCheckoutCurrentVersion());

  // Code editor
  intializeTextEditorSettings();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changePage(QTreeWidgetItem* Current, QTreeWidgetItem* Previous)
{
  if (!Current)
  {
    Current = Previous;
  }
  ui->PrefsStackedWidget->setCurrentIndex(Current->data(0,Qt::UserRole).toInt());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateLanguage(int Index)
{
  QString Lang = ui->LangComboBox->itemData(Index).toString();

  mp_PrefsMan->setUILanguage(Lang.toStdString());

  updateRestartStatus();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::clearRecentsList()
{
  mp_WorksMan->clearRecentProjects();

  m_RecentsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmItemRemoval(bool Confirm)
{
  mp_PrefsMan->setBuilderWareRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmParamRemoval(bool Confirm)
{
  mp_PrefsMan->setBuilderParamRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmUnitsRemoval(bool Confirm)
{
  mp_PrefsMan->setBuilderSpatialUnitsRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmConnectionsRemoval(bool Confirm)
{
  mp_PrefsMan->setBuilderSpatialConnsRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmAttributesRemoval(bool Confirm)
{
  mp_PrefsMan->setBuilderSpatialAttrsRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableWatchers(bool Active)
{
  mp_PrefsMan->setBuilderWaresWatchersActive(Active);
  m_WaresWatchingChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableAutoSaveBeforeRun(bool AutoSave)
{
  mp_PrefsMan->setBuilderAutomaticSaveBeforeRun(AutoSave);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDeltaT(int Val)
{
  mp_PrefsMan->setBuilderDeltaT(Val);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updatePeriodBegin(const QDateTime& DT)
{
  mp_PrefsMan->setBuilderBeginDate(DT.toString("yyyy-MM-dd HH:mm:ss").toStdString());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updatePeriodEnd(const QDateTime& DT)
{
  mp_PrefsMan->setBuilderEndDate(DT.toString("yyyy-MM-dd HH:mm:ss").toStdString());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableSyntaxHighlighting(bool Enable)
{
  mp_PrefsMan->setWaresdevSyntaxHighlightingEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableCurrentLineHighlighting(bool Enable)
{
  mp_PrefsMan->setWaresdevCurrentlineHighlightingEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeCurrentLineColor()
{
  QColor Color = QColorDialog::getColor(QString::fromStdString(mp_PrefsMan->getWaresdevCurrentlineColor()),this);

  // Cancel on QColorDialog returns an invalid Color
  if(Color.isValid())
  {
    QString ColorName = Color.name();

    ui->CurrentLineColorButton->setStyleSheet(
        QString("border: 2px solid grey; border-radius: 4px; background-color: %1").arg(ColorName));

    mp_PrefsMan->setWaresdevCurrentlineColor(ColorName.toStdString());

    m_TextEditorSettingsChanged = true;
  }

}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeCurrentFont(const QFont& Font)
{
  QString FontName = Font.family();

  ui->FontComboBox->setFont(Font);

  mp_PrefsMan->setWaresdevFontName(FontName.toStdString());

  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableLineWrapping(bool Enable)
{
  mp_PrefsMan->setWaresdevLineWrappingEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeIndentSpaceNumber(int SpaceNb)
{
  mp_PrefsMan->setWaresdevIndentSpaceNb(SpaceNb);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableInvisibleCharsDisplay(bool Enable)
{
  mp_PrefsMan->setWaresdevSpaceTabDisplayEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableCarriageReturnDisplay(bool Enable)
{
  mp_PrefsMan->setWaresdevCarriageReturnDisplayEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::intializeTextEditorSettings()
{
  // Syntax highlighting

  bool IsHLEnabled = mp_PrefsMan->isWaresdevSyntaxHighlightingEnabled();
  ui->SyntaxHLCheckBox->setChecked(IsHLEnabled);

  openfluid::base::PreferencesManager::SyntaxHighlightingRules_t Rules =
      mp_PrefsMan->getWaresdevSyntaxHighlightingRules();

  QSignalMapper* SignalMapperCB = new QSignalMapper(this);
  QSignalMapper* SignalMapperButton = new QSignalMapper(this);

  for(int row = ui->SyntaxGridLayout->rowCount() - 1 ; row > 0 ; row--)
  {
    for(int col = ui->SyntaxGridLayout->columnCount() - 1; col >= 0 ; col--)
    {
      delete ui->SyntaxGridLayout->itemAtPosition(row,col)->widget();
    }  
  }

  int row = 1;
  for(const auto& R: Rules)
  {
    QString StyleName = QString::fromStdString(R.first);
    QStringList Decorations = openfluid::ui::toQStringList(R.second.Decoration);
    QString ColorName = QString::fromStdString(R.second.Color);

    QLabel* Label = new QLabel(StyleName,this);
    ui->SyntaxGridLayout->addWidget(Label,row,0);

    int col = 1;
    for(const QString& Format : m_Formats)
    {
      QCheckBox* CB =  new QCheckBox(this);

      CB->setChecked(Decorations.contains(Format));
      SignalMapperCB->setMapping(CB, row);
      connect(CB, SIGNAL(toggled(bool)), SignalMapperCB, SLOT(map()));

      ui->SyntaxGridLayout->addWidget(CB, row, col, Qt::AlignHCenter);

      col++;
    }

    QToolButton* ChangeColorButton = new QToolButton(this);
    ChangeColorButton->setText("...");
    SignalMapperButton->setMapping(ChangeColorButton, row);
    connect(ChangeColorButton, SIGNAL(clicked()), SignalMapperButton, SLOT(map()));
    ui->SyntaxGridLayout->addWidget(ChangeColorButton, row, col, Qt::AlignHCenter);

    updateSyntaxElementLabel(Label, Decorations, ColorName);
    updateSyntaxElementColorButton(ChangeColorButton,ColorName);

    row ++;
  }

  connect(SignalMapperCB,SIGNAL(mapped(int)), this,SLOT(changeSyntaxElementDecoration(int)));
  connect(SignalMapperButton,SIGNAL(mapped(int)), this,SLOT(changeSyntaxElementColor(int)));

  ui->scrollArea->setEnabled(IsHLEnabled);


  // Current line highlighting

  ui->CurrentLineHLCheckBox->setChecked(mp_PrefsMan->isWaresdevCurrentlineHighlightingEnabled());

  QString Color = QString::fromStdString(mp_PrefsMan->getWaresdevCurrentlineColor());
  if(QColor::isValidColor(Color))
  {
    ui->CurrentLineColorButton->setStyleSheet(
        QString("border: 1px solid grey; border-radius: 4px; background-color: %1").arg(Color));
  }


  // Font

  QString FontName = QString::fromStdString(mp_PrefsMan->getWaresdevFontName());
  ui->FontComboBox->setFont(QFont(FontName));
  ui->FontComboBox->setCurrentFont(QFont(FontName));


  // Others

  ui->LineWrappingCheckBox->setChecked(mp_PrefsMan->isWaresdevLineWrappingEnabled());
  ui->IndentSpaceNbSpinBox->setValue(mp_PrefsMan->getWaresdevIndentSpaceNb());

  ui->ShowInvisibleCheckBox->setChecked(mp_PrefsMan->isWaresdevSpaceTabDisplayEnabled());
  ui->ShowCarriageReturnCheckBox->setChecked(mp_PrefsMan->isWaresdevCarriageReturnDisplayEnabled());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateSyntaxElementLabel(QLabel* Label, const QStringList& Decorations,
  const QString& ColorName)
{
  QStringList Font = Decorations.filter(QRegExp("bold|italic"));
  QStringList TextDecoration = Decorations.filter(QRegExp("underline|strike-through"));

  QString Stylesheet = QString("QLabel {font: %1;text-decoration: %2;color: %3}")
          .arg(Font.isEmpty() ? "none" : Font.join(" "))
          .arg(TextDecoration.isEmpty() ? "none" : TextDecoration.join(" ").replace("strike-through","line-through"))
          .arg(ColorName);

  Label->setStyleSheet(Stylesheet);

  Label->setProperty("ColorName",ColorName);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateSyntaxElementColorButton(QToolButton* Button, const QString& ColorName)
{
  Button->setStyleSheet(QString("border: 1px solid grey; border-radius: 4px; color: white; background-color: %1")
                        .arg(ColorName));
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeSyntaxElementDecoration(int ElementRow)
{
  QLabel* StyleNameLabel = qobject_cast<QLabel*>(ui->SyntaxGridLayout->itemAtPosition(ElementRow,0)->widget());
  std::string StyleName = StyleNameLabel->text().toStdString();

  std::vector<std::string> Decorations;
  int col = 1;
  for(const QString& Format : m_Formats)
  {
    if(qobject_cast<QCheckBox*>(ui->SyntaxGridLayout->itemAtPosition(ElementRow,col)->widget())->isChecked())
    {
      Decorations.push_back(Format.toStdString());
    }

    col++;
  }
  std::string ColorName = StyleNameLabel->property("ColorName").toString().toStdString();

  auto Rules = mp_PrefsMan->getWaresdevSyntaxHighlightingRules();
  openfluid::base::PreferencesManager::SyntaxHighlightingRules_t::iterator it = Rules.find(StyleName);
  if(it != Rules.end())
  {
    it->second.Decoration = Decorations;
    it->second.Color = ColorName;
  }
  else
  {
    Rules[StyleName] = openfluid::base::PreferencesManager::SyntaxHighlightingRule_t{ColorName,Decorations};
  }
  mp_PrefsMan->setWaresdevSyntaxHighlightingRules(Rules);

  updateSyntaxElementLabel(StyleNameLabel,
                           openfluid::ui::toQStringList(Decorations),QString::fromStdString(ColorName));

  QToolButton* ColorButton = qobject_cast<QToolButton*>(
      ui->SyntaxGridLayout->itemAtPosition(ElementRow, m_Formats.size() + 1)->widget());
  updateSyntaxElementColorButton(ColorButton,QString::fromStdString(ColorName));

  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeSyntaxElementColor(int ElementRow)
{
  QLabel* StyleNameLabel = qobject_cast<QLabel*>(ui->SyntaxGridLayout->itemAtPosition(ElementRow,0)->widget());
  QString ColorName = StyleNameLabel->property("ColorName").toString();

  QColor Color = QColorDialog::getColor(QColor(ColorName),this);

  // Cancel on QColorDialog returns an invalid Color
  if(Color.isValid())
  {
    QString NewColorName = Color.name();

    StyleNameLabel->setProperty("ColorName",NewColorName);

    changeSyntaxElementDecoration(ElementRow);

    m_TextEditorSettingsChanged = true;
  }

}


// =====================================================================
// =====================================================================


void PreferencesDialog::processSimUserPathsUpdate()
{
  mp_PrefsMan->setBuilderExtraSimulatorsPaths(
    openfluid::ui::toStdStringVector(ui->SimulatorsSearchPathsWidget->getUserPaths())
  );
  m_SimPathsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processObsUserPathsUpdate()
{
  mp_PrefsMan->setBuilderExtraObserversPaths(
    openfluid::ui::toStdStringVector(ui->ObserversSearchPathsWidget->getUserPaths())
  );
  m_ObsPathsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processBextUserPathsUpdate()
{
  mp_PrefsMan->setBuilderExtraExtensionsPaths(
    openfluid::ui::toStdStringVector(ui->BuilderextsSearchPathsWidget->getUserPaths())
  );
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processWorkspacesPathsUpdate()
{
  mp_PrefsMan->setWorkspacesPaths(openfluid::ui::toStdStringVector(ui->WorkspacesPathsWidget->getPathsList()));
  updateRestartStatus();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processExtToolsUpdate()
{
  for (auto T : ui->ExtToolsWidget->getToolsList())
  {
    std::cout << T.Name << std::endl;
  }
  
  mp_PrefsMan->setWaresdevExternalTools(ui->ExtToolsWidget->getToolsList());
  updateRestartStatus();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevConfigPATH()
{
  mp_PrefsMan->setWaresdevConfigureEnv("PATH",ui->ConfigPathEnvEdit->text().toStdString());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableAutosaveBeforeBuild(bool Enabled)
{
  mp_PrefsMan->setWaresdevAutomaticSaveBeforeBuild(Enabled);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevConfigGenerator()
{
  mp_PrefsMan->setWaresdevConfigureGenerator(ui->ConfigGeneratorEdit->text().toStdString());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevConfigOptions()
{
  mp_PrefsMan->setWaresdevConfigureOptions(ui->ConfigOptionsEdit->text().toStdString());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevBuildPATH()
{
  mp_PrefsMan->setWaresdevBuildEnv("PATH",ui->BuildPathEnvEdit->text().toStdString());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevShowPATH(bool Enabled)
{
  mp_PrefsMan->setWaresdevShowCommandEnv("PATH",Enabled);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevSslNoVerify(bool NoVerify)
{
  mp_PrefsMan->setWaresdevGitSslNoVerify(NoVerify);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevCheckoutCurrentOpenFLUIDVersion(bool CheckoutCurrentVersion)
{
  mp_PrefsMan->setWaresdevCheckoutCurrentVersion(CheckoutCurrentVersion);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::detectQtDevToolsMinGW()
{
#if defined(OPENFLUID_OS_WINDOWS)
  DetectQtDevToolsDialog Dlg(this);

  if (Dlg.exec() == QDialog::Accepted)
  {
    ui->ConfigPathEnvEdit->setText(QDir::toNativeSeparators(Dlg.getFrameworkPath())+";"+
                                   QDir::toNativeSeparators(Dlg.getDevToolsPath())+";%%PATH%%");
    ui->ConfigGeneratorEdit->setText("MinGW Makefiles");

    updateDevConfigGenerator();
    updateDevConfigPATH();
  }
#endif
}


// =====================================================================
// =====================================================================


void PreferencesDialog::applyTextEditorSettings()
{
  if(m_TextEditorSettingsChanged)
  {
    emit applyTextEditorSettingsAsked();
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::restoreDefaultsTextEditorSettings()
{
  mp_PrefsMan->setWaresdevTextEditorDefaults(true);
  intializeTextEditorSettings();
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateRestartStatus()
{
  QString CurrentWorkspace = ui->WorkspacesPathsWidget->getPathsList().value(0,"");


  if (ui->LangComboBox->currentIndex() != m_OriginalLangIndex ||
      CurrentWorkspace != m_OriginalActiveWorkspace ||
      ui->ExtToolsWidget->getNeedRestart())
  {
    ui->RestartLabel->setText(tr("Restart required"));
    m_RestartRequired = true;
  }
  else
  {
    ui->RestartLabel->setText("");
    m_RestartRequired = false;
  }
}


} } } // namespaces
