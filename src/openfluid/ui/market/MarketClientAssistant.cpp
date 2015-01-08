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
  @file MarketClientAssistant.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/ui/market/MarketClientAssistant.hpp>

#include <iostream>
#include <QIcon>
#include <QTimer>
#include <QMessageBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QModelIndex>

#include <boost/foreach.hpp>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/ui/market/MarketBuildOptionsDialog.hpp>
#include <openfluid/ui/common/ViewLogFileWindow.hpp>
#include <openfluid/ui/market/MarketPackWidgetFormat.hpp>


namespace openfluid { namespace ui { namespace market {


MarketClientAssistant::MarketClientAssistant(QWidget* Parent):
  QWizard(Parent),m_SelectionPage(this),m_LicensesPage(this),m_InstallPage(this)
{

  m_MarketClient.enableLog(true);

  setWindowTitle(tr("OpenFLUID Market client"));
  resize(750,700);


  setupSelectionPage();
  setupLicensesPage();
  setupDownloadPage();


  addPage(&m_SelectionPage);
  addPage(&m_LicensesPage);
  addPage(&m_InstallPage);


  m_SelectionPage.setPageComplete(false);
  m_LicensesPage.setPageComplete(false);
  m_InstallPage.setPageComplete(true);


  setOptions(QWizard::NoDefaultButton | QWizard::NoBackButtonOnStartPage
      | QWizard::NoBackButtonOnLastPage | QWizard::CancelButtonOnLeft);

  m_InstallPage.setFinalPage(true);


  connect(button(QWizard::CancelButton), SIGNAL(clicked()), this, SLOT(onCancel()));
  connect(button(QWizard::FinishButton), SIGNAL(clicked()), this, SLOT(onClose()));
  connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(onPrepare(int)));
}


// =====================================================================
// =====================================================================


MarketClientAssistant::~MarketClientAssistant()
{
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::setupSelectionPage()
{
  m_SelectionPage.setTitle(tr("Packages selection"));

  m_URLLabel.setText(tr("Marketplace:")+" ");

  m_RefURLComboBoxModel.setRowCount(0);
  m_RefURLComboBoxModel.setColumnCount(2);
  m_URLCombo.setModel(&m_RefURLComboBoxModel);

  m_URLColumns.mp_Name = new QStandardItem(tr("--- No marketplace selected ---"));
  m_URLColumns.mp_URL = new QStandardItem("");

  m_URLColumns.appendItems();
  m_RefURLComboBoxModel.appendRow(m_URLColumns);


  openfluid::base::PreferencesManager::MarketPlaces_t MarketPlaces = openfluid::base::PreferencesManager::instance()->getMarketplaces();

  BOOST_FOREACH(openfluid::base::PreferencesManager::MarketPlaces_t::value_type &PlaceIt, MarketPlaces)
  {
    m_URLColumns.mp_Name = new QStandardItem(PlaceIt.first + " (" + PlaceIt.second + ")");
    m_URLColumns.mp_URL = new QStandardItem(PlaceIt.second);

    m_URLColumns.appendItems();
    m_RefURLComboBoxModel.appendRow(m_URLColumns);
  }

  m_URLCombo.setCurrentIndex(0);

  m_URLBox.addWidget(&m_URLLabel);
  m_URLBox.addWidget(&m_URLCombo, 1);


  m_SelectionPageBox.setSpacing(20);
  m_SelectionPageBox.addLayout(&m_URLBox);
  m_SelectionPageBox.addWidget(&m_TypesTabs);

  m_SelectionPage.setLayout(&m_SelectionPageBox);


  connect(&m_URLCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onURLComboChanged(int)));
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::setupLicensesPage()
{
  m_LicensesPage.setTitle(tr("Licenses"));

  m_LicensesLabel.setText(tr("Licenses:"));

  mp_LicensesTreeView = new QTreeWidget();
  mp_LicensesTreeView->setColumnCount(1);
  mp_LicensesTreeView->setHeaderHidden(true);

  m_LicensesListSWindow.setWidget(mp_LicensesTreeView);
  m_LicensesListSWindow.setWidgetResizable(true);

  mp_LicensesTextView = new QTextEdit();
  mp_LicensesTextView->setReadOnly(true);
  mp_LicensesTextView->setText("");

  m_LicensesReviewSWindow.setWidget(mp_LicensesTextView);
  m_LicensesReviewSWindow.setWidgetResizable(true);


  m_LicensesReviewPaned.setOrientation(Qt::Horizontal);
  m_LicensesReviewPaned.addWidget(&m_LicensesListSWindow);
  m_LicensesReviewPaned.addWidget(&m_LicensesReviewSWindow);


  m_LicensesAcceptRadio.setText(tr("I accept the terms of the license(s) agreement(s)"));
  m_LicensesDoNotRadio.setText(tr("I do not accept the terms of the license(s) agreement(s)"));

  QGroupBox *LicensesRadioGroup = new QGroupBox();
  QVBoxLayout *LicensesRadioLayout = new QVBoxLayout();

  LicensesRadioLayout->addWidget(&m_LicensesAcceptRadio);
  LicensesRadioLayout->addWidget(&m_LicensesDoNotRadio);
  LicensesRadioGroup->setLayout(LicensesRadioLayout);

  m_LicensesDoNotRadio.setChecked(true);


  m_LicensesPageBox.addWidget(&m_LicensesLabel);
  m_LicensesPageBox.addWidget(&m_LicensesReviewPaned, 1);
  m_LicensesPageBox.addWidget(LicensesRadioGroup);


  m_LicensesPage.setLayout(&m_LicensesPageBox);


  connect(mp_LicensesTreeView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
      this, SLOT(onLicensesTreeviewChanged(QTreeWidgetItem*)));

  connect(&m_LicensesAcceptRadio, SIGNAL(toggled(bool)), this, SLOT(onLicenseRadioClicked(bool)));

}


// =====================================================================
// =====================================================================


void MarketClientAssistant::setupDownloadPage()
{
  m_InstallPage.setTitle(tr("Download and install"));

  mp_InstallTable = new QTableWidget(0, 4);

  QStringList ColumnNames;
  ColumnNames << tr("Package ID") << tr("Type") << tr("Format") << tr("Status");
  mp_InstallTable->setHorizontalHeaderLabels(ColumnNames);
  mp_InstallTable->horizontalHeader()->setStretchLastSection(true);
  mp_InstallTable->verticalHeader()->setVisible(false);


  m_InstallSWindow.setWidget(mp_InstallTable);
  m_InstallSWindow.setWidgetResizable(true);
  m_InstallSWindow.setFrameShape(QFrame::NoFrame);

  m_ViewLogButton.setText(tr("View installation log"));

  QHBoxLayout *LogLayout = new QHBoxLayout();
  LogLayout->setAlignment(Qt::AlignRight);
  LogLayout->addWidget(&m_ViewLogButton);


  m_InstallProgressBar.setMinimumSize(0,30);
  m_InstallProgressBar.setRange(0, 100);
  m_InstallProgressBar.setTextVisible(true);


  m_InstallPageBox.setSpacing(10);
  m_InstallPageBox.addWidget(&m_InstallSWindow);
  m_InstallPageBox.addWidget(&m_InstallProgressBar);
  m_InstallPageBox.addLayout(LogLayout);


  m_InstallPage.setLayout(&m_InstallPageBox);


  connect(&m_ViewLogButton, SIGNAL(clicked()), this, SLOT(onViewLogClicked()));
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onApply()
{

}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onCancel()
{
  hide();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onClose()
{
  hide();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onPrepare(int Id)
{
  setWindowTitle(tr("OpenFLUID Market client (Step %1 of 3)").arg(QString::number(Id+1)));

  // page : Licenses
  if (Id == 1)
    initializeLicencesTreeView();


  // page : Installation
  if (Id == 2)
  {
    m_InstallPage.setPageComplete(true);
    m_ViewLogButton.setEnabled(false);

    // Remove of cancel button
    setOptions(QWizard::NoCancelButton | QWizard::NoBackButtonOnLastPage);

    QTimer::singleShot(250, this, SLOT(onInstallTimeoutOnce()));
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onURLComboChanged(int RowNumber)
{
  m_MarketClient.disconnect();
  clearAvailPacksTreeview();

  if (RowNumber > 0)
  {
    QString CurrURL = m_RefURLComboBoxModel.item(RowNumber, URLComboColumns::URL)->text();
    m_MarketClient.connect(CurrURL.toStdString());

    // if repository contains packages
    if (m_MarketClient.catalogsContainPackages())
      updateAvailPacksTreeview();
    else
      displayMarketplaceError();
  }
  else
  {
    // change mouse cursor to default
    unsetCursor();
  }

  m_SelectionPage.setPageComplete(false);
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onLicensesTreeviewChanged(QTreeWidgetItem *CurrentItem)
{
  mp_LicensesTextView->setText("");

  if (CurrentItem != 0)
  {
    QString PackageID = CurrentItem->text(0);

    openfluid::market::MetaPackagesCatalog_t::iterator PCit
      = m_MarketClient.findInTypesMetaPackagesCatalogs(PackageID.toStdString());

    // id package found ?
    if (PCit != m_MarketClient.typesMetaPackagesCatalogs().rbegin()->second.end())
    {
      // Getting license id
      std::string LicenseID = PCit->second.AvailablePackages[PCit->second.Selected].License;

      // Searching license
      std::map<std::string,std::string>::const_iterator LIter = m_MarketClient.licensesTexts().find(LicenseID);

      // Displaying license
      if (LIter != m_MarketClient.licensesTexts().end())
        mp_LicensesTextView->setText(QString::fromStdString(LIter->second));
      else
        mp_LicensesTextView->setText(tr("(License content not available)"));
    }
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onLicenseRadioClicked(bool Checked)
{
  m_LicensesPage.setPageComplete(Checked);
}


// =====================================================================
// =====================================================================


MarketPackWidget* MarketClientAssistant::availablePackWidget(const openfluid::ware::WareID_t& ID) const
{
  std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> >::const_iterator APMiter;
  std::list<MarketPackWidget*>::const_iterator APLiter;

  // Searching ID package in AvailPacksWidgets
  for (APMiter = mp_AvailPacksWidgets.begin(); APMiter != mp_AvailPacksWidgets.end(); ++APMiter)
  {
    for (APLiter = APMiter->second.begin(); APLiter != APMiter->second.end(); ++APLiter)
    {
      // package found
      if ((*APLiter)->getID() == QString::fromStdString(ID))
        return *APLiter;
    }
  }

  return 0;
}


// =====================================================================
// =====================================================================


bool MarketClientAssistant::hasParentSelected(const openfluid::ware::WareID_t& ID,
    const openfluid::market::PackageInfo::PackageType& Type)
{
  openfluid::market::MetaPackagesCatalog_t DataCatalog = m_MarketClient.typesMetaPackagesCatalogs().at(openfluid::market::PackageInfo::DATA);
  openfluid::market::MetaPackagesCatalog_t::iterator PCit;

  // for each dataset package
  for (PCit = DataCatalog.begin(); PCit != DataCatalog.end(); ++PCit)
  {
    std::list<openfluid::ware::WareID_t> Dependencies = PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::FLUIDX].Dependencies[Type];
    std::list<openfluid::ware::WareID_t>::iterator Dit = Dependencies.begin();

    // searching if ID package is a dependence of this dataset
    while (Dit != Dependencies.end() && !(*Dit == ID && availablePackWidget(PCit->second.ID)->isInstall()))
      ++Dit;

    if (Dit != Dependencies.end())
      return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool MarketClientAssistant::getUserChoice(const openfluid::ware::WareID_t& ID, const bool Select,
    const std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> >& PacksToSelect)
{
  QString Message;

  if (Select)
  {
    Message = tr("<b>%1</b> needs some dependencies to run.<br/>Do you want to select this packages ?<br/>")
        .arg(QString::fromStdString(ID));
  }
  else
  {
    Message = tr("<b>%1</b> has dependencies selected.<br/>Do you want to unselect this packages ?<br/>")
        .arg(QString::fromStdString(ID));
  }

  std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> >::const_iterator APMiter;
  std::list<MarketPackWidget*>::const_iterator APLiter;

  // display list of packs to select/unselect
  for (APMiter = PacksToSelect.begin(); APMiter != PacksToSelect.end(); ++APMiter)
  {
    Message += "<br/><u>"+getGraphicTypeName(APMiter->first, true, true)+":</u> ";
    for (APLiter = APMiter->second.begin(); APLiter != APMiter->second.end(); ++APLiter)
    {
      Message += "<br/> - " + (*APLiter)->getID();
    }
  }

  if (Select)
    Message += "<br/><br/><i>"+tr("Warning, the selected package cannot be used without them.")+"</i><br/>";

  int Answer = QMessageBox::question(this, tr("Dependencies"), Message, QMessageBox::Yes | QMessageBox::No);

  return Answer == QMessageBox::Yes;
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::selectDependencies(const openfluid::ware::WareID_t& ID)
{
  openfluid::market::MetaPackagesCatalog_t::iterator PCit = m_MarketClient.findInTypesMetaPackagesCatalogs(ID);

  // package found
  if (PCit != m_MarketClient.typesMetaPackagesCatalogs().rbegin()->second.end())
  {
    MarketPackWidget* MPW = availablePackWidget(ID);

    // dependencies of id package
    openfluid::market::PackageInfo::Dependencies_t Dependencies = PCit->second.AvailablePackages[MPW->getPackageFormat()].Dependencies;
    openfluid::market::PackageInfo::Dependencies_t::const_iterator DMit;
    std::list<openfluid::ware::WareID_t>::const_iterator DLit;

    // list of dependencies to select/unselect
    std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> > PacksToSelect;
    std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> >::iterator APMiter;
    std::list<MarketPackWidget*>::iterator APLiter;

    for (DMit = Dependencies.begin(); DMit != Dependencies.end(); ++DMit)
    {
      for (DLit = DMit->second.begin(); DLit != DMit->second.end(); ++DLit)
      {
        // For each dependence
        openfluid::ware::WareID_t DependenceID = *DLit;
        MarketPackWidget *DependencePack = availablePackWidget(DependenceID);

        // dependence pack widget found ?
        if (DependencePack != 0)
        {
          // dependence state different to selected package state
          if (DependencePack->isInstall() != MPW->isInstall())
          {
            // if select or if unselect + no parent selected
            if (MPW->isInstall() ||
                (!MPW->isInstall() && !hasParentSelected(DependencePack->getID().toStdString(), DMit->first)))
            {
              // Adding dependence to packs to select
              PacksToSelect[DMit->first].push_back(DependencePack);
            }
          }
        }

      }
    }

    // selected dependencies
    if (!PacksToSelect.empty())
    {
      if (getUserChoice(ID, MPW->isInstall(), PacksToSelect))
      {
        for (APMiter = PacksToSelect.begin(); APMiter != PacksToSelect.end(); ++APMiter)
        {
          for (APLiter = APMiter->second.begin(); APLiter != APMiter->second.end(); ++APLiter)
          {
            MarketPackWidget* Dependence = *APLiter;
            Dependence->setInstall(MPW->isInstall());
          }
        }

      }
    }

  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onPackageInstallModified()
{
  bool Selection = false;

  std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> >::iterator APMiter;
  std::list<MarketPackWidget*>::iterator APLiter;

  for (APMiter=mp_AvailPacksWidgets.begin();APMiter!=mp_AvailPacksWidgets.end();++APMiter)
  {
    for (APLiter=APMiter->second.begin();APLiter!=APMiter->second.end();++APLiter)
    {
      MarketPackWidget* MPW;
      MPW = *APLiter;
      Selection = Selection || MPW->isInstall();

      if (MPW->isInstall())
      {
        m_MarketClient.setSelectionFlag(MPW->getID().toStdString(),MPW->getPackageFormat());
      }
      else
      {
        m_MarketClient.setSelectionFlag(MPW->getID().toStdString(),openfluid::market::MetaPackageInfo::NONE);
      }

      if (APMiter->first != openfluid::market::PackageInfo::DATA)
        m_MarketClient.setSRCBuildOptions(MPW->getID().toStdString(),((MarketPackWidgetFormat*)MPW)->getEditedBuildOptions().toStdString());
    }
  }

  m_SelectionPage.setPageComplete(Selection);
  m_LicensesDoNotRadio.setChecked(true);

}


// =====================================================================
// =====================================================================


openfluid::market::PackageInfo::PackageType MarketClientAssistant::getCurrentTypeTab()
{
  QString CurrentTabName = m_TypesTabs.tabText(m_TypesTabs.currentIndex());
  std::map<openfluid::market::PackageInfo::PackageType,QWidget*>::const_iterator TabIt = mp_TabPage.begin();

  while (TabIt != mp_TabPage.end() && getGraphicTypeName(TabIt->first,true,true) != CurrentTabName)
    ++TabIt;

  return TabIt->first;
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onSelectAllClicked()
{
  std::list<MarketPackWidget*>::iterator APLiter;
  openfluid::market::PackageInfo::PackageType CurrentTab = getCurrentTypeTab();

  // Selecting of all packages of current tab
  for (APLiter = mp_AvailPacksWidgets[CurrentTab].begin(); APLiter != mp_AvailPacksWidgets[CurrentTab].end(); ++APLiter)
  {
    MarketPackWidget* MPW;
    MPW = *APLiter;
    MPW->setInstall(true);
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onSelectNoneClicked()
{
  std::list<MarketPackWidget*>::iterator APLiter;
  openfluid::market::PackageInfo::PackageType CurrentTab = getCurrentTypeTab();

  // Unselecting of all packages of current tab
  for (APLiter = mp_AvailPacksWidgets[CurrentTab].begin(); APLiter != mp_AvailPacksWidgets[CurrentTab].end(); ++APLiter)
  {
    MarketPackWidget* MPW;
    MPW = *APLiter;
    MPW->setInstall(false);
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onCommonBuildConfigClicked()
{
  openfluid::market::PackageInfo::PackageType CurrentTab = getCurrentTypeTab();
  MarketBuildOptionsDialog OptDialog(QString::fromStdString(openfluid::market::MarketPackage::getCommonBuildOptions(CurrentTab)),"");

  if (OptDialog.exec() == QDialog::Accepted)
  {
    // Setting common options of current tab
    openfluid::market::MarketPackage::setCommonBuildOptions(CurrentTab,OptDialog.getEditedOptions().toStdString());

    std::list<MarketPackWidget*>::iterator APLiter;

    for (APLiter = mp_AvailPacksWidgets[CurrentTab].begin(); APLiter != mp_AvailPacksWidgets[CurrentTab].end(); ++APLiter)
      ((MarketPackWidget*)(*APLiter))->updateDisplayedInfos();
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onInstallTimeoutOnce()
{
  m_InstallPage.setPageComplete(false);

  m_MarketClient.preparePackagesInstallation();

  updateInstallTreeview();

  unsigned int PackToInstallCount = m_MarketClient.getCountOfPackagesToInstall();
  unsigned int InstalledCount = 0;

  m_InstallProgressBar.setValue(0);
  m_InstallProgressBar.setFormat(tr("0% complete"));

  while (m_MarketClient.hasSelectedPackagesToInstall())
  {
    QTableWidgetItem *StatusItem = mp_InstallTable->item(InstalledCount,
        InstallTableColumns::STATUS);
    mp_InstallTable->scrollToItem(StatusItem);

    StatusItem->setText(tr("Installation in progress..."));

    try
    {
      m_MarketClient.installNextSelectedPackage();
      StatusItem->setText(tr("Success"));
    }
    catch (openfluid::base::FrameworkException& E)
    {
      std::string ErrorMsg(E.what());
      StatusItem->setText(tr("Failed"));
    }
    InstalledCount++;

    int Percent = int(float(InstalledCount)/float(PackToInstallCount) * 100);
    m_InstallProgressBar.setValue(Percent);
    m_InstallProgressBar.setFormat(tr("%1\% complete").arg(QString::number(Percent)));
  }

  m_InstallProgressBar.setValue(100);
  m_InstallProgressBar.setFormat(tr("100% complete"));

  m_ViewLogButton.setEnabled(true);
  m_InstallPage.setPageComplete(true);
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::clearAvailPacksTreeview()
{
  // change mouse cursor to watch
  setCursor(Qt::WaitCursor);

  std::map<openfluid::market::PackageInfo::PackageType,QWidget*>::iterator TabIt;

  // for each tab
  for (TabIt = mp_TabPage.begin(); TabIt != mp_TabPage.end(); ++TabIt)
  {
    // Tab VBox exists ?
    if (TabIt->second != 0)
    {
      // removing widgets from available packages display
      if (mp_AvailTypesPacksBox[TabIt->first]->layout() != 0)
      {
        QLayoutItem* Item;
        while ((Item = mp_AvailTypesPacksBox[TabIt->first]->layout()->takeAt(0)) != 0)
        {
          delete Item->widget();
          delete Item;
        }
        delete mp_AvailTypesPacksBox[TabIt->first]->layout();
      }

      delete mp_SelectAllButton[TabIt->first];
      delete mp_SelectNoneButton[TabIt->first];
      delete mp_CommonBuildConfigButton[TabIt->first];
      delete mp_ActionButtonsBox[TabIt->first];

      delete mp_AvailTypesPacksSWindow[TabIt->first];

      delete mp_TabBox[TabIt->first];
      delete TabIt->second;

      mp_SelectAllButton[TabIt->first] = 0;
      mp_SelectNoneButton[TabIt->first] = 0;
      mp_CommonBuildConfigButton[TabIt->first] = 0;
      mp_ActionButtonsBox[TabIt->first] = 0;

      mp_AvailTypesPacksBox[TabIt->first] = 0;
      mp_AvailTypesPacksSWindow[TabIt->first] = 0;

      mp_TabBox[TabIt->first] = 0;
      TabIt->second = 0;
    }
  }

  mp_AvailPacksWidgets.clear();

  // removing tabs
  while (m_TypesTabs.count() > 0)
  {
    m_TypesTabs.removeTab(0);
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::updateAvailPacksTreeview()
{
  openfluid::market::TypesMetaPackagesCatalogs_t Catalogs;
  openfluid::market::TypesMetaPackagesCatalogs_t::const_iterator TCIter;
  openfluid::market::MetaPackagesCatalog_t::const_iterator CIter;


  Catalogs = m_MarketClient.typesMetaPackagesCatalogs();

  for (TCIter = Catalogs.begin(); TCIter != Catalogs.end(); ++TCIter)
  {
    // New tab
    if (!TCIter->second.empty())
    {
      // Creating ScrolledWindow and VBox containers
      mp_AvailTypesPacksSWindow[TCIter->first] = new QScrollArea();
      mp_AvailTypesPacksBox[TCIter->first] = new QVBoxLayout();

      // removing ScrolledWindow border
      mp_AvailTypesPacksSWindow[TCIter->first]->setFrameShape(QFrame::NoFrame);
      mp_AvailTypesPacksSWindow[TCIter->first]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      mp_AvailTypesPacksSWindow[TCIter->first]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


      QWidget *scrolledWidget = new QWidget();
      scrolledWidget->setLayout(mp_AvailTypesPacksBox[TCIter->first]);
      mp_AvailTypesPacksSWindow[TCIter->first]->setWidget(scrolledWidget);
      mp_AvailTypesPacksSWindow[TCIter->first]->setWidgetResizable(true);


      mp_AvailTypesPacksBox[TCIter->first]->setSpacing(0);
      mp_AvailTypesPacksBox[TCIter->first]->setAlignment(Qt::AlignTop);


      // Adding packages in VBox
      for (CIter = TCIter->second.begin(); CIter != TCIter->second.end(); ++CIter)
      {
        // Creating MarketPackWidget
        if (TCIter->first == openfluid::market::PackageInfo::DATA)
          mp_AvailPacksWidgets[TCIter->first].push_back(new MarketPackWidget(TCIter->first,CIter->second));
        else
          mp_AvailPacksWidgets[TCIter->first].push_back(new MarketPackWidgetFormat(TCIter->first,CIter->second));


        connect(mp_AvailPacksWidgets[TCIter->first].back(), SIGNAL(installModified(openfluid::ware::WareID_t)),
            this, SLOT(selectDependencies(openfluid::ware::WareID_t)));

        connect(mp_AvailPacksWidgets[TCIter->first].back(), SIGNAL(installModified(openfluid::ware::WareID_t)),
            this, SLOT(onPackageInstallModified()));


        QFrame* Line = new QFrame();
        Line->setFrameShape(QFrame::HLine);
        Line->setFrameShadow(QFrame::Sunken);

        mp_AvailTypesPacksBox[TCIter->first]->addWidget(Line);
        mp_AvailTypesPacksBox[TCIter->first]->addWidget(mp_AvailPacksWidgets[TCIter->first].back());
      }


      // Creating buttons
      mp_ActionButtonsBox[TCIter->first] = new QHBoxLayout();
      mp_SelectAllButton[TCIter->first] = new QPushButton(tr("Select all"));
      mp_SelectNoneButton[TCIter->first] = new QPushButton(tr("Select none"));
      mp_CommonBuildConfigButton[TCIter->first] = new QPushButton(tr("Edit build options"));
      mp_CommonBuildConfigButton[TCIter->first]->setIcon(QIcon(":/ui/market/icons/preferences.png"));


      // Disable build options button for datasets
      if (TCIter->first == openfluid::market::PackageInfo::DATA)
        mp_CommonBuildConfigButton[TCIter->first]->setEnabled(false);


      connect(mp_SelectAllButton[TCIter->first], SIGNAL(clicked()), this, SLOT(onSelectAllClicked()));
      connect(mp_SelectNoneButton[TCIter->first], SIGNAL(clicked()), this, SLOT(onSelectNoneClicked()));
      connect(mp_CommonBuildConfigButton[TCIter->first], SIGNAL(clicked()), this, SLOT(onCommonBuildConfigClicked()));


      mp_ActionButtonsBox[TCIter->first]->setSpacing(10);
      mp_ActionButtonsBox[TCIter->first]->setAlignment(Qt::AlignLeft);
      mp_ActionButtonsBox[TCIter->first]->addWidget(mp_SelectAllButton[TCIter->first]);
      mp_ActionButtonsBox[TCIter->first]->addWidget(mp_SelectNoneButton[TCIter->first]);
      mp_ActionButtonsBox[TCIter->first]->addSpacing(330);
      mp_ActionButtonsBox[TCIter->first]->addWidget(mp_CommonBuildConfigButton[TCIter->first]);


      mp_TabPage[TCIter->first] = new QWidget();
      mp_TabBox[TCIter->first] = new QVBoxLayout();

      mp_TabBox[TCIter->first]->addWidget(mp_AvailTypesPacksSWindow[TCIter->first]);

      QFrame* Line = new QFrame();
      Line->setFrameShape(QFrame::HLine);
      Line->setFrameShadow(QFrame::Sunken);
      mp_TabBox[TCIter->first]->addWidget(Line);
      mp_TabBox[TCIter->first]->addLayout(mp_ActionButtonsBox[TCIter->first]);


      // Create tab
      mp_TabPage[TCIter->first]->setLayout(mp_TabBox[TCIter->first]);
      m_TypesTabs.addTab(mp_TabPage[TCIter->first], getGraphicTypeName(TCIter->first, true,true));

    }
  }

  // change mouse cursor to default
  unsetCursor();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::displayMarketplaceError()
{
  // change mouse cursor to default
  unsetCursor();

  // get current marketplace name
  QString MarketplaceName = m_URLCombo.currentText();

  // display error message
  QMessageBox::critical(this, tr("Connection error"),
      tr("Unable to connect to \"%1\" marketplace.\nThis marketplace is not available, does not contain any catalog files or catalogs are empty.")
      .arg(MarketplaceName));

  m_URLCombo.setCurrentIndex(0);
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::initializeLicencesTreeView()
{
  openfluid::market::TypesMetaPackagesCatalogs_t Catalogs;
  openfluid::market::TypesMetaPackagesCatalogs_t::const_iterator TCIter;
  openfluid::market::MetaPackagesCatalog_t::const_iterator CIter;

  mp_LicensesTreeView->clear();

  std::map<openfluid::market::PackageInfo::PackageType,bool> RowCreated;
  Catalogs = m_MarketClient.typesMetaPackagesCatalogs();

  for (TCIter = Catalogs.begin(); TCIter != Catalogs.end(); ++TCIter)
  {
    // Creation of type row
    QTreeWidgetItem *Item = new QTreeWidgetItem();

    for (CIter = TCIter->second.begin(); CIter != TCIter->second.end(); ++CIter)
    {
      // If package is selected
      if (CIter->second.Selected != openfluid::market::MetaPackageInfo::NONE)
      {
        // If TypeRow not created
        if (!RowCreated[TCIter->first])
        {
          // Add in licenses treeview
          mp_LicensesTreeView->addTopLevelItem(Item);
          Item->setText(0, getGraphicTypeName(TCIter->first,true,true));

          RowCreated[TCIter->first] = true;
        }

        // Creation of sub-row in type row (item)
        QTreeWidgetItem *PackItem = new QTreeWidgetItem(Item);
        PackItem->setText(0, QString::fromStdString(CIter->first));
      }
    }
  }


  // open first type row
  QTreeWidgetItem *TypeItem = mp_LicensesTreeView->itemAt(0, 0);
  TypeItem->setExpanded(true);

  // select first package
  mp_LicensesTreeView->setCurrentItem(mp_LicensesTreeView->itemAt(0, 0)->child(0));
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::updateInstallTreeview()
{
  std::list<openfluid::market::MarketPackage*> PacksList = m_MarketClient.selectionToInstall();
  std::list<openfluid::market::MarketPackage*>::const_iterator PLiter;

  for (PLiter = PacksList.begin();PLiter != PacksList.end();++PLiter )
  {
    int TmpRow = mp_InstallTable->rowCount();
    mp_InstallTable->insertRow(TmpRow);

    m_InstallColumns.mp_ID = new QTableWidgetItem(QString::fromStdString((*PLiter)->getID()));
    m_InstallColumns.mp_Type = new QTableWidgetItem(getGraphicTypeName((*PLiter)->getPackageType(),true,false));
    m_InstallColumns.mp_Format = new QTableWidgetItem();

    if ((*PLiter)->getFormat() == openfluid::market::MetaPackageInfo::BIN)
      m_InstallColumns.mp_Format = new QTableWidgetItem(tr("binary"));
    else if ((*PLiter)->getFormat() == openfluid::market::MetaPackageInfo::SRC)
      m_InstallColumns.mp_Format = new QTableWidgetItem(tr("source"));
    else
      m_InstallColumns.mp_Format = new QTableWidgetItem(tr("dataset"));

    m_InstallColumns.mp_Status = new QTableWidgetItem(tr("Pending"));


    m_InstallColumns.setFlags();

    mp_InstallTable->setItem(TmpRow, 0, m_InstallColumns.mp_ID);
    mp_InstallTable->setItem(TmpRow, 1, m_InstallColumns.mp_Type);
    mp_InstallTable->setItem(TmpRow, 2, m_InstallColumns.mp_Format);
    mp_InstallTable->setItem(TmpRow, 3, m_InstallColumns.mp_Status);
  }

  mp_InstallTable->resizeColumnsToContents();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onViewLogClicked()
{
  openfluid::ui::common::ViewLogFileWindow LogWindow(QString::fromStdString(openfluid::market::MarketPackage::getLogFile()));

  LogWindow.exec();
}


// =====================================================================
// =====================================================================


QString MarketClientAssistant::getGraphicTypeName(const openfluid::market::PackageInfo::PackageType& Type, const bool Maj, const bool Plural)
{
  QString TypeNames[] = { tr("simulator"), tr("observer"), tr("builder extension"), tr("dataset")};
  QString PluralTypeNames[] = { tr("simulators"), tr("observers"), tr("builder extensions"), tr("datasets")};
  QString Name;

  if (Plural)
    Name = PluralTypeNames[Type];
  else
    Name = TypeNames[Type];

  if (Maj)
    Name.replace(0, 1, Name[0].toUpper());

  return Name;
}


} } } // namespaces

