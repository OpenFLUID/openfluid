/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file PreferencesDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <QFileDialog>

#include <openfluid/guicommon/PreferencesManager.hpp>
#include <openfluid/base/RuntimeEnv.hpp>


#include "AppTools.hpp"
#include "ExtensionPluginsManager.hpp"

#include "ui_PreferencesDialog.h"
#include "PreferencesDialog.hpp"
#include "ui_WaresSearchPathsWidget.h"
#include "WaresSearchPathsWidget.hpp"
#include "EditMarketplaceDialog.hpp"



PreferencesDialog::PreferencesDialog(QWidget* Parent):
  QDialog(Parent), ui(new Ui::PreferencesDialog),
  m_RecentsChanged(false),
  m_SimPathsChanged(false), m_ObsPathsChanged(false), m_WaresWatchingChanged(false)
{
  setWindowModality(Qt::ApplicationModal);

  ui->setupUi(this);

  ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                  .arg(BUILDER_DIALOGBANNER_BGCOLOR));

  ui->AddMarketPlaceButton->setText("");
  ui->AddMarketPlaceButton->setIcon(QIcon(":/icons/add.png"));
  ui->AddMarketPlaceButton->setIconSize(QSize(20,20));

  ui->EditMarketPlaceButton->setText("");
  ui->EditMarketPlaceButton->setIcon(QIcon(":/icons/modify.png"));
  ui->EditMarketPlaceButton->setIconSize(QSize(20,20));

  ui->RemoveMarketPlaceButton->setText("");
  ui->RemoveMarketPlaceButton->setIcon(QIcon(":/icons/remove.png"));
  ui->RemoveMarketPlaceButton->setIconSize(QSize(20,20));


  mp_SimSearchPaths = new WaresSearchPathsWidget();
  mp_ObsSearchPaths = new WaresSearchPathsWidget();
  mp_BExtSearchPaths = new WaresSearchPathsWidget();

  ui->PathsTabWidget->addTab(mp_SimSearchPaths,tr("Simulators"));
  ui->PathsTabWidget->addTab(mp_ObsSearchPaths,tr("Observers"));
  ui->PathsTabWidget->addTab(mp_BExtSearchPaths,tr("Builder-extensions"));


  QTreeWidgetItem *PrefItem;

  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0, tr("Interface"));
  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0, tr("Paths"));
  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0, tr("Simulations"));
  PrefItem = new QTreeWidgetItem(ui->PrefsTreeWidget);
  PrefItem->setText(0, tr("Market"));

  initialize();

  connect(ui->LangComboBox,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(updateLanguage(const QString&)));
  connect(ui->RecentMaxSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateRecentsMax(int)));
  connect(ui->ClearRecentsButton,SIGNAL(clicked()),this,SLOT(clearRecentsList()));
  connect(ui->AutoSaveCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableAutoSaveBeforeRun(bool)));
  connect(ui->ItemRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmItemRemoval(bool)));
  connect(ui->ParamRemovalCheckBox,SIGNAL(toggled(bool)),this,SLOT(confirmParamRemoval(bool)));
  connect(ui->WatchCheckBox,SIGNAL(toggled(bool)),this,SLOT(enableWatchers(bool)));

  connect(ui->WorkDirButton,SIGNAL(clicked()),this,SLOT(updateWorkDir()));

  connect(ui->DeltaTSpinBox,SIGNAL(valueChanged(int)),this,SLOT(updateDeltaT(int)));
  connect(ui->BeginDateEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(updatePeriodBegin(const QDateTime&)));
  connect(ui->EndDateEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(updatePeriodEnd(const QDateTime&)));

  connect(mp_SimSearchPaths->ui->AddButton,SIGNAL(clicked()),this,SLOT(addSimSearchPath()));
  connect(mp_SimSearchPaths->ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removeSimSearchPath()));
  connect(mp_SimSearchPaths->ui->UpButton,SIGNAL(clicked()),this,SLOT(moveupSimSearchPath()));
  connect(mp_SimSearchPaths->ui->DownButton,SIGNAL(clicked()),this,SLOT(movedownSimSearchPath()));
  connect(mp_ObsSearchPaths->ui->AddButton,SIGNAL(clicked()),this,SLOT(addObsSearchPath()));
  connect(mp_ObsSearchPaths->ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removeObsSearchPath()));
  connect(mp_ObsSearchPaths->ui->UpButton,SIGNAL(clicked()),this,SLOT(moveupObsSearchPath()));
  connect(mp_ObsSearchPaths->ui->DownButton,SIGNAL(clicked()),this,SLOT(movedownObsSearchPath()));
  connect(mp_BExtSearchPaths->ui->AddButton,SIGNAL(clicked()),this,SLOT(addBExtSearchPath()));
  connect(mp_BExtSearchPaths->ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removeBExtSearchPath()));
  connect(mp_BExtSearchPaths->ui->UpButton,SIGNAL(clicked()),this,SLOT(moveupBExtSearchPath()));
  connect(mp_BExtSearchPaths->ui->DownButton,SIGNAL(clicked()),this,SLOT(movedownBExtSearchPath()));


  connect(ui->AddMarketPlaceButton,SIGNAL(clicked()),this,SLOT(addMarketPlace()));
  connect(ui->EditMarketPlaceButton,SIGNAL(clicked()),this,SLOT(editMarketPlace()));
  connect(ui->RemoveMarketPlaceButton,SIGNAL(clicked()),this,SLOT(removeMarketPlace()));


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


void PreferencesDialog::initialize()
{

  openfluid::guicommon::PreferencesManager* PrefsMan =
    openfluid::guicommon::PreferencesManager::getInstance();

  openfluid::base::RuntimeEnvironment* RunEnv =
      openfluid::base::RuntimeEnvironment::getInstance();


  // Interface
  // TODO set up a fancier languages list
  ui->LangComboBox->addItem("default");
  ui->LangComboBox->addItems(openfluid::guicommon::PreferencesManager::getAvailableLangs());
  ui->LangComboBox->setCurrentIndex(ui->LangComboBox->findText(PrefsMan->getLang()));
  ui->RecentMaxSpinBox->setValue(PrefsMan->getRecentMax());

  ui->AutoSaveCheckBox->setChecked(PrefsMan->isAutomaticSaveBeforeRun());

  ui->ItemRemovalCheckBox->setChecked(PrefsMan->isItemRemovalConfirm());
  ui->ParamRemovalCheckBox->setChecked(PrefsMan->isParamRemovalConfirm());
  ui->WatchCheckBox->setChecked(PrefsMan->isWaresWatchersActive());

  // Paths
  ui->WorkDirEdit->setText(PrefsMan->getWorkdir());
  mp_SimSearchPaths->initialize(PrefsMan->getExtraSimulatorsPaths(),StringVectorToQStringList(RunEnv->getDefaultSimulatorsPluginsPaths()));
  mp_ObsSearchPaths->initialize(PrefsMan->getExtraObserversPaths(),StringVectorToQStringList(RunEnv->getDefaultObserversPluginsPaths()));
  mp_BExtSearchPaths->initialize(PrefsMan->getExtraExtensionsPaths(),
                                 StringVectorToQStringList(ExtensionPluginsManager::getInstance()->getPluginsStandardSearchPaths()));


  // Simulations
  ui->DeltaTSpinBox->setValue(PrefsMan->getDeltaT());

  ui->BeginDateEdit->setDateTime(QDateTime::fromString(PrefsMan->getBegin(),"yyyy-MM-dd HH:mm:ss"));
  ui->EndDateEdit->setDateTime(QDateTime::fromString(PrefsMan->getEnd(),"yyyy-MM-dd HH:mm:ss"));


  // Market
  updateMarketplacesList();
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateMarketplacesList()
{
  openfluid::guicommon::PreferencesManager* PrefsMan =
    openfluid::guicommon::PreferencesManager::getInstance();

  openfluid::guicommon::PreferencesManager::MarketPlaces_t MPlaces = PrefsMan->getMarketplaces();

  openfluid::guicommon::PreferencesManager::MarketPlaces_t::iterator MPit;
  openfluid::guicommon::PreferencesManager::MarketPlaces_t::iterator MPitb = MPlaces.begin();
  openfluid::guicommon::PreferencesManager::MarketPlaces_t::iterator MPite = MPlaces.end();


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


QStringList PreferencesDialog::extractSearchPath(WaresSearchPathsWidget* W)
{
  QStringList QSL;

  for (int i =0; i < W->ui->UserListWidget->count();++i)
  {
    QSL.append(W->ui->UserListWidget->item(i)->text());
  }

  return QSL;
}


// =====================================================================
// =====================================================================


bool PreferencesDialog::addSearchPath(WaresSearchPathsWidget* W)
{
  QString SelectedDir = QFileDialog::getExistingDirectory(this,tr("Select search path"));

  if (SelectedDir !=  "")
  {
    W->ui->UserListWidget->addItem(SelectedDir);
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool PreferencesDialog::removeSearchPath(WaresSearchPathsWidget* W)
{
  if (W->ui->UserListWidget->currentRow()>=0)
  {
    delete W->ui->UserListWidget->takeItem(W->ui->UserListWidget->currentRow());
    return true;
  }
  return false;
}


// =====================================================================
// =====================================================================


bool PreferencesDialog::moveupSearchPath(WaresSearchPathsWidget* W)
{
  int Index = W->ui->UserListWidget->currentRow();

  if (Index == 0) return false;

  QListWidgetItem *Item = W->ui->UserListWidget->takeItem(Index);
  W->ui->UserListWidget->insertItem(Index-1, Item);
  W->ui->UserListWidget->setCurrentRow(Index-1);

  return true;
}


// =====================================================================
// =====================================================================


bool PreferencesDialog::movedownSearchPath(WaresSearchPathsWidget* W)
{
  int Index = W->ui->UserListWidget->currentRow();

  if (Index == W->ui->UserListWidget->count()-1) return false;

  QListWidgetItem *Item = W->ui->UserListWidget->takeItem(Index);
  W->ui->UserListWidget->insertItem(Index+1, Item);
  W->ui->UserListWidget->setCurrentRow(Index+1);

  return true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::changePage(QTreeWidgetItem* Current, QTreeWidgetItem* Previous)
{
  if (!Current) Current = Previous;
  ui->PrefsStackedWidget->setCurrentIndex(ui->PrefsTreeWidget->indexOfTopLevelItem(Current));
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateLanguage(const QString& Lang)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setLang(Lang);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::clearRecentsList()
{
  openfluid::guicommon::PreferencesManager::getInstance()->clearRecentProjects();
  m_RecentsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateRecentsMax(int Val)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setRecentMax(Val);
  m_RecentsChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmItemRemoval(bool Confirm)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setItemRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::confirmParamRemoval(bool Confirm)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setParamRemovalConfirm(Confirm);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableWatchers(bool Active)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setWaresWatchersActive(Active);
  m_WaresWatchingChanged = true;
}


// =====================================================================
// =====================================================================


void PreferencesDialog::enableAutoSaveBeforeRun(bool AutoSave)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setAutomaticSaveBeforeRun(AutoSave);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateWorkDir()
{
  QString SelectedDir = QFileDialog::getExistingDirectory(this,tr("Select working directory"));

  if (SelectedDir !=  "")
  {
    ui->WorkDirEdit->setText(SelectedDir);
    openfluid::guicommon::PreferencesManager::getInstance()->setWorkdir(SelectedDir);
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updateDeltaT(int Val)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setDeltaT(Val);
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updatePeriodBegin(const QDateTime& DT)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setBegin(DT.toString("yyyy-MM-dd HH:mm:ss"));
  if (ui->EndDateEdit->dateTime() <= DT)
  {
    ui->EndDateEdit->setDateTime(DT.addSecs(1));
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::updatePeriodEnd(const QDateTime& DT)
{
  openfluid::guicommon::PreferencesManager::getInstance()->setEnd(DT.toString("yyyy-MM-dd HH:mm:ss"));
}


// =====================================================================
// =====================================================================


void PreferencesDialog::addMarketPlace()
{
  EditMarketplaceDialog MarketDlg(this,"","",
                                  openfluid::guicommon::PreferencesManager::getInstance()->getMarketplaces());

  if (MarketDlg.exec() == QDialog::Accepted)
  {
    openfluid::guicommon::PreferencesManager::getInstance()->addMarketplace(MarketDlg.getName(),MarketDlg.getURL());
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
                                    openfluid::guicommon::PreferencesManager::getInstance()->getMarketplaces());

    if (MarketDlg.exec() == QDialog::Accepted)
    {
      openfluid::guicommon::PreferencesManager::getInstance()->removeMarketplace(MarketDlg.getOriginalName());
      openfluid::guicommon::PreferencesManager::getInstance()->addMarketplace(MarketDlg.getName(),MarketDlg.getURL());
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

    openfluid::guicommon::PreferencesManager::getInstance()->removeMarketplace(AssociatedData[0]);

    updateMarketplacesList();
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::addSimSearchPath()
{
  if (addSearchPath(mp_SimSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraSimulatorsPaths(extractSearchPath(mp_SimSearchPaths));
    m_SimPathsChanged = true;
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::removeSimSearchPath()
{
  if (removeSearchPath(mp_SimSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraSimulatorsPaths(extractSearchPath(mp_SimSearchPaths));
    m_SimPathsChanged = true;
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::moveupSimSearchPath()
{
  if (moveupSearchPath(mp_SimSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraSimulatorsPaths(extractSearchPath(mp_SimSearchPaths));
    m_SimPathsChanged = true;
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::movedownSimSearchPath()
{
  if (movedownSearchPath(mp_SimSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraSimulatorsPaths(extractSearchPath(mp_SimSearchPaths));
    m_SimPathsChanged = true;
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::addObsSearchPath()
{
  if (addSearchPath(mp_ObsSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraObserversPaths(extractSearchPath(mp_ObsSearchPaths));
    m_ObsPathsChanged = true;
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::removeObsSearchPath()
{
  if (removeSearchPath(mp_ObsSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraObserversPaths(extractSearchPath(mp_ObsSearchPaths));
    m_ObsPathsChanged = true;
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::moveupObsSearchPath()
{
  if (moveupSearchPath(mp_ObsSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraObserversPaths(extractSearchPath(mp_ObsSearchPaths));
    m_ObsPathsChanged = true;
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::movedownObsSearchPath()
{
  if (movedownSearchPath(mp_ObsSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraObserversPaths(extractSearchPath(mp_ObsSearchPaths));
    m_ObsPathsChanged = true;
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::addBExtSearchPath()
{
  if (addSearchPath(mp_BExtSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraExtensionsPaths(extractSearchPath(mp_BExtSearchPaths));
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::removeBExtSearchPath()
{
  if (removeSearchPath(mp_BExtSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraExtensionsPaths(extractSearchPath(mp_BExtSearchPaths));
  }
}


// =====================================================================
// =====================================================================


void PreferencesDialog::moveupBExtSearchPath()
{
  if (moveupSearchPath(mp_BExtSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraExtensionsPaths(extractSearchPath(mp_BExtSearchPaths));
  }
}

// =====================================================================
// =====================================================================


void PreferencesDialog::movedownBExtSearchPath()
{
  if (movedownSearchPath(mp_BExtSearchPaths))
  {
    openfluid::guicommon::PreferencesManager::getInstance()->setExtraExtensionsPaths(extractSearchPath(mp_BExtSearchPaths));
  }
}



