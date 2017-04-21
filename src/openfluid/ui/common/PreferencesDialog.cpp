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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <QLocale>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QSignalMapper>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/common/PreferencesDialog.hpp>
#include <openfluid/ui/common/WaresSearchPathsWidget.hpp>
#include <openfluid/ui/common/EditMarketplaceDialog.hpp>
#include <openfluid/ui/common/DetectQtDevToolsDialog.hpp>

#include "ui_PreferencesDialog.h"
#include "ui_WaresSearchPathsWidget.h"


namespace openfluid { namespace ui { namespace common {


PreferencesDialog::PreferencesDialog(QWidget* Parent, DisplayMode Mode):
  OpenFLUIDDialog(Parent), ui(new Ui::PreferencesDialog),
  m_RecentsChanged(false),
  m_SimPathsChanged(false), m_ObsPathsChanged(false), m_WaresWatchingChanged(false), m_TextEditorSettingsChanged(false),
  m_RestartRequired(false),
  m_OriginalLangIndex(0), m_Mode(Mode)
{
  setWindowModality(Qt::ApplicationModal);

  ui->setupUi(this);

  ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                  .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));

  ui->AddMarketPlaceButton->setText("");
  ui->AddMarketPlaceButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddMarketPlaceButton->setIconSize(QSize(20,20));

  ui->EditMarketPlaceButton->setText("");
  ui->EditMarketPlaceButton->setIcon(openfluid::ui::common::getIcon("modify","/ui/common"));
  ui->EditMarketPlaceButton->setIconSize(QSize(20,20));

  ui->RemoveMarketPlaceButton->setText("");
  ui->RemoveMarketPlaceButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveMarketPlaceButton->setIconSize(QSize(20,20));

  QTreeWidgetItem *PrefItem;

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Environment"));
  PrefItem->setData(0,Qt::UserRole,ENVIRONMENT_PAGE);

  if (m_Mode == MODE_BUILDER || m_Mode == MODE_FULL)
  {
    PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
    PrefItem->setText(0,tr("Interface"));
    PrefItem->setData(0,Qt::UserRole,BUILDER_PAGE);

    PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
    PrefItem->setText(0,tr("Simulations"));
    PrefItem->setData(0,Qt::UserRole,SIMULATION_PAGE);
  }

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Development tools"));
  PrefItem->setData(0,Qt::UserRole,DEVENV_PAGE);

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Code editor"));
  PrefItem->setData(0,Qt::UserRole,DEVEDITOR_PAGE);

#if OPENFLUID_MARKET_ENABLED
  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0,tr("Market"));
  PrefItem->setData(0,Qt::UserRole,MARKET_PAGE);
#endif


  ui->WorkspacesPathsWidget->setAllowEmpty(false);
  ui->WaresPathsTabWidget->setCurrentIndex(0);

  if (!(m_Mode == MODE_BUILDER || m_Mode == MODE_FULL))
    ui->WaresPathsTabWidget->removeTab(2);


  initialize();

  connect(ui->LangComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateLanguage(int)));
  connect(ui->RecentMaxSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateRecentsMax(int)));
  connect(ui->ClearRecentsButton,SIGNAL(clicked()),this,SLOT(clearRecentsList()));
  connect(ui->AutoSaveCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableAutoSaveBeforeRun(bool)));
  connect(ui->ItemRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmItemRemoval(bool)));
  connect(ui->ParamRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmParamRemoval(bool)));
  connect(ui->WatchCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableWatchers(bool)));

  connect(ui->UnitsRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmUnitsRemoval(bool)));
  connect(ui->ConnectionsRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmConnectionsRemoval(bool)));
  connect(ui->AttributesRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmAttributesRemoval(bool)));


  connect(ui->WorkspacesPathsWidget,SIGNAL(pathsUpdated()),this,SLOT(processWorkspacesPathsUpdate()));

  connect(ui->SimulatorsSearchPathsWidget,SIGNAL(userPathsUpdated()),this,SLOT(processSimUserPathsUpdate()));
  connect(ui->ObserversSearchPathsWidget,SIGNAL(userPathsUpdated()),this,SLOT(processObsUserPathsUpdate()));
  connect(ui->BuilderextsSearchPathsWidget,SIGNAL(userPathsUpdated()),this,SLOT(processBextUserPathsUpdate()));


  connect(ui->DeltaTSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateDeltaT(int)));
  connect(ui->BeginDateEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(updatePeriodBegin(const QDateTime&)));
  connect(ui->EndDateEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(updatePeriodEnd(const QDateTime&)));

  connect(ui->SyntaxHLCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableSyntaxHighlighting(bool)));
  connect(ui->CurrentLineHLCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableCurrentLineHighlighting(bool)));
  connect(ui->CurrentLineColorButton, SIGNAL(clicked()), this, SLOT(changeCurrentLineColor()));
  connect(ui->FontComboBox, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(changeCurrentFont(const QFont&)));
  connect(ui->LineWrappingCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableLineWrapping(bool)));
  connect(ui->IndentSpaceNbSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeIndentSpaceNumber(int)));
  connect(ui->TextEditorApplyButton, SIGNAL(clicked()), this, SLOT(applyTextEditorSettings()));
  connect(ui->TextEditorRestoreDefaultsButton, SIGNAL(clicked()), this, SLOT(restoreDefaultsTextEditorSettings()));

  connect(ui->AddMarketPlaceButton,SIGNAL(clicked()),this,SLOT(addMarketPlace()));
  connect(ui->EditMarketPlaceButton,SIGNAL(clicked()),this,SLOT(editMarketPlace()));
  connect(ui->RemoveMarketPlaceButton,SIGNAL(clicked()),this,SLOT(removeMarketPlace()));


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

  if(Mode == MODE_DEVSTUDIO || Mode == MODE_FULL)
    connect(ui->SslNoVerifyCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateDevSslNoVerify(bool)));
  else
    ui->GitGroupBox->setVisible(false);

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
    LangName = QLocale::languageToString(Loc.language());
  else
    LangName = Loc.nativeLanguageName();

  LangName[0] = LangName[0].toUpper();
  QString CountryName = Loc.nativeCountryName();

  return QString("%1 (%2)").arg(LangName).arg(Loc.nativeCountryName());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::initialize()
{
  openfluid::base::PreferencesManager* PrefsMan = openfluid::base::PreferencesManager::instance();

  // Interface language
  QStringList AvailLangCodes = openfluid::base::PreferencesManager::getAvailableLangs();

  ui->LangComboBox->addItem(getLanguageAsPrettyString("en_GB"),"default");
  for (auto Code : AvailLangCodes)
  {
    ui->LangComboBox->addItem(getLanguageAsPrettyString(Code),Code);
  }
  ui->LangComboBox->setCurrentIndex(ui->LangComboBox->findData(PrefsMan->getLang()));

  m_OriginalLangIndex = ui->LangComboBox->currentIndex();

  ui->RestartLabel->setText("");


  // Workspaces paths
  ui->WorkspacesPathsWidget->setPathsList(PrefsMan->getBuilderWorkspacesPaths());

  m_OriginalActiveWorkspace = PrefsMan->getBuilderWorkspacePath();


  if (m_Mode == MODE_BUILDER || m_Mode == MODE_FULL)
  {
    // Wares search paths
    ui->SimulatorsSearchPathsWidget->initialize(PrefsMan->getBuilderExtraSimulatorsPaths(),
                                                openfluid::tools::toQStringList(
                                                    openfluid::base::Environment::getDefaultSimulatorsDirs()));
    ui->ObserversSearchPathsWidget->initialize(PrefsMan->getBuilderExtraObserversPaths(),
                                               openfluid::tools::toQStringList(
                                                   openfluid::base::Environment::getDefaultObserversDirs()));
    ui->BuilderextsSearchPathsWidget->initialize(PrefsMan->getBuilderExtraExtensionsPaths(),
                                                 openfluid::tools::toQStringList(
                                                     openfluid::base::Environment::getDefaultBuilderextsDirs()));

    // interface
    ui->RecentMaxSpinBox->setValue(PrefsMan->getBuilderRecentMax());

    ui->AutoSaveCheckBox->setChecked(PrefsMan->isBuilderAutomaticSaveBeforeRun());

    ui->ItemRemovalCheckBox->setChecked(PrefsMan->isBuilderItemRemovalConfirm());
    ui->ParamRemovalCheckBox->setChecked(PrefsMan->isBuilderParamRemovalConfirm());
    ui->WatchCheckBox->setChecked(PrefsMan->isBuilderWaresWatchersActive());

    ui->UnitsRemovalCheckBox->setChecked(PrefsMan->isBuilderSpatialUnitsRemovalConfirm());
    ui->ConnectionsRemovalCheckBox->setChecked(PrefsMan->isBuilderSpatialConnsRemovalConfirm());
    ui->AttributesRemovalCheckBox->setChecked(PrefsMan->isBuilderSpatialAttrsRemovalConfirm());


    // Simulations
    ui->DeltaTSpinBox->setValue(PrefsMan->getBuilderDeltaT());

    ui->BeginDateEdit->setDateTime(QDateTime::fromString(PrefsMan->getBuilderBegin(),"yyyy-MM-dd HH:mm:ss"));
    ui->EndDateEdit->setDateTime(QDateTime::fromString(PrefsMan->getBuilderEnd(),"yyyy-MM-dd HH:mm:ss"));

  }

  // Development tools
  ui->ConfigPathEnvEdit->setText(PrefsMan->getWaresdevConfigEnv("PATH"));
  ui->ConfigGeneratorEdit->setText(PrefsMan->getWaresdevConfigGenerator());
  ui->ConfigOptionsEdit->setText(PrefsMan->getWaresdevConfigOptions());
  ui->BuildPathEnvEdit->setText(PrefsMan->getWaresdevBuildEnv("PATH"));
  ui->ShowPathCheckBox->setChecked(PrefsMan->isWaresdevShowCommandEnv("PATH"));
  ui->SslNoVerifyCheckBox->setChecked(PrefsMan->isWaresdevGitSslNoVerify());

  // Code editor
  intializeTextEditorSettings();


#if OPENFLUID_MARKET_ENABLED
  // Market
  updateMarketplacesList();
#endif
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changePage(QTreeWidgetItem* Current, QTreeWidgetItem* Previous)
{
  if (!Current) Current = Previous;
  ui->PrefsStackedWidget->setCurrentIndex(Current->data(0,Qt::UserRole).toInt());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateLanguage(int Index)
{
  QString Lang = ui->LangComboBox->itemData(Index).toString();

  openfluid::base::PreferencesManager::instance()->setLang(Lang);

  updateRestartStatus();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::clearRecentsList()
{
  openfluid::base::PreferencesManager::instance()->clearBuilderRecentProjects();
  m_RecentsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateRecentsMax(int Val)
{
  openfluid::base::PreferencesManager::instance()->setBuilderRecentMax(Val);
  m_RecentsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmItemRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setBuilderItemRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmParamRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setBuilderParamRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmUnitsRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setBuilderSpatialUnitsRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmConnectionsRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setBuilderSpatialConnsRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmAttributesRemoval(bool Confirm)
{
  openfluid::base::PreferencesManager::instance()->setBuilderSpatialAttrsRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableWatchers(bool Active)
{
  openfluid::base::PreferencesManager::instance()->setBuilderWaresWatchersActive(Active);
  m_WaresWatchingChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableAutoSaveBeforeRun(bool AutoSave)
{
  openfluid::base::PreferencesManager::instance()->setBuilderAutomaticSaveBeforeRun(AutoSave);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDeltaT(int Val)
{
  openfluid::base::PreferencesManager::instance()->setBuilderDeltaT(Val);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updatePeriodBegin(const QDateTime& DT)
{
  openfluid::base::PreferencesManager::instance()->setBuilderBegin(DT.toString("yyyy-MM-dd HH:mm:ss"));
  if (ui->EndDateEdit->dateTime() <= DT)
  {
    ui->EndDateEdit->setDateTime(DT.addSecs(1));
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updatePeriodEnd(const QDateTime& DT)
{
  openfluid::base::PreferencesManager::instance()->setBuilderEnd(DT.toString("yyyy-MM-dd HH:mm:ss"));
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableSyntaxHighlighting(bool Enable)
{
  openfluid::base::PreferencesManager::instance()->setWaresdevSyntaxHighlightingEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}

// =====================================================================
// =====================================================================


void PreferencesDialog::enableCurrentLineHighlighting(bool Enable)
{
  openfluid::base::PreferencesManager::instance()->setWaresdevCurrentlineHighlightingEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeCurrentLineColor()
{
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();

  QColor Color = QColorDialog::getColor(PrefMgr->getWaresdevCurrentlineColor(),this);

  // Cancel on QColorDialog returns an invalid Color
  if(Color.isValid())
  {
    QString ColorName = Color.name();

    ui->CurrentLineColorButton->setStyleSheet(
        QString("border: 2px solid grey; border-radius: 4px; background-color: %1").arg(ColorName));

    PrefMgr->setWaresdevCurrentlineColor(ColorName);

    m_TextEditorSettingsChanged = true;
  }

}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeCurrentFont(const QFont& Font)
{
  QString FontName = Font.family();

  ui->FontComboBox->setFont(Font);

  openfluid::base::PreferencesManager::instance()->setWaresdevFontName(FontName);

  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableLineWrapping(bool Enable)
{
  openfluid::base::PreferencesManager::instance()->setWaresdevLineWrappingEnabled(Enable);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changeIndentSpaceNumber(int SpaceNb)
{
  openfluid::base::PreferencesManager::instance()->setWaresdevIndentSpaceNb(SpaceNb);
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::addMarketPlace()
{
  EditMarketplaceDialog MarketDlg(this,"","",
                                  openfluid::base::PreferencesManager::instance()->getMarketplaces());

  if (MarketDlg.exec() == QDialog::Accepted)
  {
    openfluid::base::PreferencesManager::instance()->addMarketplace(MarketDlg.getName(),MarketDlg.getURL());
    updateMarketplacesList();
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::editMarketPlace()
{
  if (ui->MarketPlacesListWidget->currentRow()>=0)
  {
    QListWidgetItem* Item = ui->MarketPlacesListWidget->currentItem();
    QStringList AssociatedData = Item->data(Qt::UserRole).toStringList();

    EditMarketplaceDialog MarketDlg(this,AssociatedData[0],AssociatedData[1],
                                    openfluid::base::PreferencesManager::instance()->getMarketplaces());

    if (MarketDlg.exec() == QDialog::Accepted)
    {
      openfluid::base::PreferencesManager::instance()->removeMarketplace(MarketDlg.getOriginalName());
      openfluid::base::PreferencesManager::instance()->addMarketplace(MarketDlg.getName(),MarketDlg.getURL());
      updateMarketplacesList();
    }
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::removeMarketPlace()
{
  if (ui->MarketPlacesListWidget->currentRow()>=0)
  {
    QListWidgetItem* Item = ui->MarketPlacesListWidget->currentItem();
    QStringList AssociatedData = Item->data(Qt::UserRole).toStringList();

    openfluid::base::PreferencesManager::instance()->removeMarketplace(AssociatedData[0]);

    updateMarketplacesList();
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateMarketplacesList()
{
  openfluid::base::PreferencesManager* PrefsMan =
    openfluid::base::PreferencesManager::instance();

  openfluid::base::PreferencesManager::MarketPlaces_t MPlaces = PrefsMan->getMarketplaces();

  openfluid::base::PreferencesManager::MarketPlaces_t::iterator MPit;
  openfluid::base::PreferencesManager::MarketPlaces_t::iterator MPitb = MPlaces.begin();
  openfluid::base::PreferencesManager::MarketPlaces_t::iterator MPite = MPlaces.end();


  // clear the list
  while(ui->MarketPlacesListWidget->count()>0)
  {
    delete ui->MarketPlacesListWidget->takeItem(0);
  }

  // populate the list
  for (MPit=MPitb;MPit!=MPite;++MPit)
  {
    QListWidgetItem* Item = new QListWidgetItem((*MPit).first+ " (" + (*MPit).second+")",ui->MarketPlacesListWidget);
    QStringList AssociatedData;
    AssociatedData << (*MPit).first << (*MPit).second;
    Item->setData(Qt::UserRole,AssociatedData);
  }

  ui->MarketPlacesListWidget->sortItems();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::intializeTextEditorSettings()
{
  openfluid::base::PreferencesManager* PrefsMan = openfluid::base::PreferencesManager::instance();


  // Syntax highlighting

  bool IsHLEnabled = PrefsMan->isWaresdevSyntaxHighlightingEnabled();
  ui->SyntaxHLCheckBox->setChecked(IsHLEnabled);

  openfluid::base::PreferencesManager::SyntaxHighlightingRules_t Rules =
      PrefsMan->getWaresdevSyntaxHighlightingRules();

  QSignalMapper* SignalMapperCB = new QSignalMapper(this);
  QSignalMapper* SignalMapperButton = new QSignalMapper(this);

  for(int row = ui->SyntaxGridLayout->rowCount() - 1 ; row > 0 ; row--)
  {
    for(int col = ui->SyntaxGridLayout->columnCount() - 1; col >= 0 ; col--)
      delete ui->SyntaxGridLayout->itemAtPosition(row,col)->widget();
  }

  int row = 1;
  for(openfluid::base::PreferencesManager::SyntaxHighlightingRules_t::iterator it = Rules.begin() ;
      it != Rules.end() ; ++ it)
  {
    QString StyleName = it.key();
    QStringList Decorations = it.value().m_Decoration;
    QString ColorName = it.value().m_Color;

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

      col ++;
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

  ui->CurrentLineHLCheckBox->setChecked(PrefsMan->isWaresdevCurrentlineHighlightingEnabled());

  QString Color = PrefsMan->getWaresdevCurrentlineColor();
  if(QColor::isValidColor(Color))
    ui->CurrentLineColorButton->setStyleSheet(
        QString("border: 1px solid grey; border-radius: 4px; background-color: %1").arg(Color));


  // Font

  QString FontName = PrefsMan->getWaresdevFontName();
  ui->FontComboBox->setFont(QFont(FontName));
  ui->FontComboBox->setCurrentFont(QFont(FontName));


  // Others

  ui->LineWrappingCheckBox->setChecked(PrefsMan->isWaresdevLineWrappingEnabled());
  ui->IndentSpaceNbSpinBox->setValue(PrefsMan->getWaresdevIndentSpaceNb());
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
  QString StyleName = StyleNameLabel->text();

  QStringList Decorations;
  int col = 1;
  for(const QString& Format : m_Formats)
  {
    if(qobject_cast<QCheckBox*>(ui->SyntaxGridLayout->itemAtPosition(ElementRow,col)->widget())->isChecked())
      Decorations << Format;

    col ++;
  }
  QString ColorName = StyleNameLabel->property("ColorName").toString();

  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();
  openfluid::base::PreferencesManager::SyntaxHighlightingRules_t Rules = PrefMgr->getWaresdevSyntaxHighlightingRules();
  openfluid::base::PreferencesManager::SyntaxHighlightingRules_t::iterator it = Rules.find(StyleName);
  if(it != Rules.end())
  {
    it->m_Decoration = Decorations;
    it->m_Color = ColorName;
  }
  else
    Rules.insert(StyleName, openfluid::base::PreferencesManager::SyntaxHighlightingRule_t(ColorName,Decorations));
  openfluid::base::PreferencesManager::instance()->setWaresdevSyntaxHighlightingRules(Rules);

  updateSyntaxElementLabel(StyleNameLabel, Decorations,ColorName);

  QToolButton* ColorButton = qobject_cast<QToolButton*>(
      ui->SyntaxGridLayout->itemAtPosition(ElementRow, m_Formats.size() + 1)->widget());
  updateSyntaxElementColorButton(ColorButton, ColorName);

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
  openfluid::base::PreferencesManager::instance()
      ->setBuilderExtraSimulatorsPaths(ui->SimulatorsSearchPathsWidget->getUserPaths());
  m_SimPathsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processObsUserPathsUpdate()
{
  openfluid::base::PreferencesManager::instance()
      ->setBuilderExtraObserversPaths(ui->ObserversSearchPathsWidget->getUserPaths());
  m_ObsPathsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processBextUserPathsUpdate()
{
  openfluid::base::PreferencesManager::instance()
      ->setBuilderExtraExtensionsPaths(ui->BuilderextsSearchPathsWidget->getUserPaths());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::processWorkspacesPathsUpdate()
{
  openfluid::base::PreferencesManager::instance()->setBuilderWorkspacesPaths(ui->WorkspacesPathsWidget->getPathsList());
  updateRestartStatus();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevConfigPATH()
{
  openfluid::base::PreferencesManager::instance()->setWaresdevConfigEnv("PATH",ui->ConfigPathEnvEdit->text());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevConfigGenerator()
{
  openfluid::base::PreferencesManager::instance()->setWaresdevConfigGenerator(ui->ConfigGeneratorEdit->text());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevConfigOptions()
{
  openfluid::base::PreferencesManager::instance()->setWaresdevConfigOptions(ui->ConfigOptionsEdit->text());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevBuildPATH()
{
  openfluid::base::PreferencesManager::instance()->setWaresdevBuildEnv("PATH",ui->BuildPathEnvEdit->text());
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevShowPATH(bool Enabled)
{
  openfluid::base::PreferencesManager::instance()->setWaresdevShowCommandEnv("PATH",Enabled);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDevSslNoVerify(bool NoVerify)
{
  openfluid::base::PreferencesManager::instance()->setWaresdevGitSslNoVerify(NoVerify);
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
    emit applyTextEditorSettingsAsked();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::restoreDefaultsTextEditorSettings()
{
  openfluid::base::PreferencesManager::instance()->setWaresdevTextEditorDefaults(true);
  intializeTextEditorSettings();
  m_TextEditorSettingsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateRestartStatus()
{
  QString CurrentWorkspace = ui->WorkspacesPathsWidget->getPathsList().value(0,"");


  if (ui->LangComboBox->currentIndex() != m_OriginalLangIndex ||
      CurrentWorkspace != m_OriginalActiveWorkspace)
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
