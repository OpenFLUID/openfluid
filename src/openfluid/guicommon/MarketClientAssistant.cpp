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
  \file MarketClientAssistant.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/guicommon/MarketClientAssistant.hpp>

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <glibmm/i18n.h>

#include <gtkmm/image.h>
#include <gtkmm/stock.h>
#include <gtkmm/main.h>
#include <gtkmm/separator.h>

#include <gdkmm/cursor.h>

#include <openfluid/guicommon/MarketBuildOptionsDialog.hpp>
#include <openfluid/guicommon/ViewLogFileWindow.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>
#include <openfluid/guicommon/MarketPackWidgetFormat.hpp>
#include <openfluid/tools/SwissTools.hpp>


namespace openfluid { namespace guicommon {


MarketClientAssistant::MarketClientAssistant()
: m_SelectionPageBox(false,0),
  m_LicensesPageBox(false,0),
  m_InstallPageBox(false,0)
{

  m_MarketClient.enableLog(true);

  set_title("OpenFLUID Market client");
  set_border_width(20);
  set_default_size(750, 700);
  set_position(Gtk::WIN_POS_CENTER);


  Glib::RefPtr<Gdk::Pixbuf> IconPixbuf = Gdk::Pixbuf::create_from_file(openfluid::base::RuntimeEnvironment::getInstance()->getCommonResourceFilePath("openfluid_icon.png"));

  set_default_icon(IconPixbuf);


  setupSelectionPage();
  setupLicensesPage();
  setupDownloadPage();


  append_page(m_SelectionPageBox);
  append_page(m_LicensesPageBox);
  append_page(m_InstallPageBox);


  set_page_title(*get_nth_page(0), _("Packages selection"));
  set_page_title(*get_nth_page(1), _("Licenses"));
  set_page_title(*get_nth_page(2), _("Download and install"));


  set_page_complete(m_SelectionPageBox, false);
  set_page_complete(m_LicensesPageBox, false);
  set_page_complete(m_InstallPageBox, true);


  set_page_type(m_SelectionPageBox, Gtk::ASSISTANT_PAGE_INTRO);
  set_page_type(m_LicensesPageBox, Gtk::ASSISTANT_PAGE_CONTENT);
  set_page_type(m_InstallPageBox, Gtk::ASSISTANT_PAGE_SUMMARY);


  signal_apply().connect(sigc::mem_fun(*this,
      &MarketClientAssistant::onApply));

  signal_cancel().connect(sigc::mem_fun(*this,
      &MarketClientAssistant::onCancel));

  signal_close().connect(sigc::mem_fun(*this,
      &MarketClientAssistant::onClose));

  signal_prepare().connect(sigc::mem_fun(*this,
      &MarketClientAssistant::onPrepare));

  show_all_children();
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

  m_URLLabel.set_text(_("Marketplace:")+std::string(" "));

  m_RefURLComboBoxModel = Gtk::ListStore::create(m_URLColumns);
  m_URLCombo.set_model(m_RefURLComboBoxModel);


  Gtk::TreeModel::Row TmpURLRow = *(m_RefURLComboBoxModel->append());
  TmpURLRow[m_URLColumns.m_Name] = _("--- No marketplace selected ---");
  TmpURLRow[m_URLColumns.m_URL] = "";


  openfluid::guicommon::PreferencesManager::MarketPlaces_t MarketPlaces = openfluid::guicommon::PreferencesManager::getInstance()->getMarketplaces();

  BOOST_FOREACH(openfluid::guicommon::PreferencesManager::MarketPlaces_t::value_type &PlaceIt, MarketPlaces)
  {
    TmpURLRow = *(m_RefURLComboBoxModel->append());
    TmpURLRow[m_URLColumns.m_Name] = PlaceIt.first;
    TmpURLRow[m_URLColumns.m_URL] = PlaceIt.second;
  }



  m_URLCombo.pack_start(m_URLColumns.m_Name);
  m_URLCombo.pack_start(m_URLColumns.m_URL);

  m_URLCombo.set_active(0);

  m_URLBox.pack_start(m_URLLabel,Gtk::PACK_SHRINK);
  m_URLBox.pack_start(m_URLCombo,Gtk::PACK_EXPAND_WIDGET);


  m_SelectionPageBox.set_border_width(12);
  m_SelectionPageBox.pack_start(m_URLBox,Gtk::PACK_SHRINK,12);
  m_SelectionPageBox.pack_start(m_TypesTabs,Gtk::PACK_EXPAND_WIDGET,6);


  m_URLCombo.signal_changed().connect(
    sigc::mem_fun(*this, &MarketClientAssistant::onURLComboChanged)
  );

}


// =====================================================================
// =====================================================================


void MarketClientAssistant::setupLicensesPage()
{
  m_LicensesLabel.set_text(_("Licenses:"));
  m_LicensesLabel.set_alignment(0,0);

  m_RefLicenseTextBuffer = Gtk::TextBuffer::create();
  m_RefLicenseTextBuffer->set_text("");

  m_LicensesTextView.set_editable(false);
  m_LicensesTextView.set_buffer(m_RefLicenseTextBuffer);

  m_LicensesReviewSWindow.add(m_LicensesTextView);
  m_LicensesReviewSWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);

  m_RefLicenseTreeViewModel = Gtk::TreeStore::create(m_LicensesColumns);
  m_LicensesTreeView.set_model(m_RefLicenseTreeViewModel);
  m_LicensesTreeView.set_headers_visible(false);

  m_LicensesListSWindow.add(m_LicensesTreeView);
  m_LicensesListSWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);
  m_LicensesListSWindow.set_size_request(180);

  m_RefLicensesTreeSelection = m_LicensesTreeView.get_selection();

  // adding widgets in hpaned
  m_LicensesReviewPaned.pack1(m_LicensesListSWindow,false,false);
  m_LicensesReviewPaned.pack2(m_LicensesReviewSWindow,true,false);


  m_LicensesAcceptRadio.set_label(_("I accept the terms of the license(s) agreement(s)"));
  m_LicensesDoNotRadio.set_label(_("I do not accept the terms of the license(s) agreement(s)"));
  Gtk::RadioButton::Group Group = m_LicensesAcceptRadio.get_group();
  m_LicensesDoNotRadio.set_group(Group);
  m_LicensesDoNotRadio.set_active(true);

  m_LicensesPageBox.set_border_width(12);
  m_LicensesPageBox.pack_start(m_LicensesLabel,Gtk::PACK_SHRINK);
  m_LicensesPageBox.pack_start(m_LicensesReviewPaned,Gtk::PACK_EXPAND_WIDGET);
  m_LicensesPageBox.pack_start(m_LicensesAcceptRadio,Gtk::PACK_SHRINK);
  m_LicensesPageBox.pack_start(m_LicensesDoNotRadio,Gtk::PACK_SHRINK);

  m_RefLicensesTreeSelection->signal_changed().connect(
      sigc::mem_fun(*this,&MarketClientAssistant::onLicensesTreeviewChanged)
  );


  m_LicensesAcceptRadio.signal_clicked().connect(
      sigc::mem_fun(*this,&MarketClientAssistant::onLicenseRadioClicked)
  );
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::setupDownloadPage()
{

  m_RefInstallTreeViewModel = Gtk::ListStore::create(m_InstallColumns);
  m_InstallTreeView.set_model(m_RefInstallTreeViewModel);


  m_InstallTreeView.append_column(_("Package ID"),m_InstallColumns.m_ID);
  m_InstallTreeView.append_column(_("Type"),m_InstallColumns.m_Type);
  m_InstallTreeView.append_column(_("Format"),m_InstallColumns.m_Format);
  m_InstallTreeView.append_column(_("Status"),m_InstallColumns.m_Status);
  m_InstallSWindow.add(m_InstallTreeView);
  m_InstallSWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_LogButtonAlign.set(1.0,0.5,0.0,1.0);

  m_LogButtonAlign.add(m_ViewLogButton);

  m_ViewLogButton.set_label(_("View installation log"));

  m_InstallProgressBar.set_size_request(-1,30);

  m_InstallPageBox.set_border_width(12);
  m_InstallPageBox.pack_start(m_InstallSWindow,Gtk::PACK_EXPAND_WIDGET,12);
  m_InstallPageBox.pack_start(m_InstallProgressBar,Gtk::PACK_SHRINK,12);
  m_InstallPageBox.pack_start(m_LogButtonAlign,Gtk::PACK_SHRINK,0);


  m_ViewLogButton.signal_clicked().connect(
      sigc::mem_fun(*this, &MarketClientAssistant::onViewLogClicked)
    );


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


void MarketClientAssistant::onPrepare(Gtk::Widget* /*Widget*/)
{
  set_title(Glib::ustring::compose(_("OpenFLUID Market client (Step %1 of %2)"),get_current_page() + 1, get_n_pages()));



  // page : Licenses
  if (get_current_page() == 1)
  {
    initializeLicencesTreeView();
  }


  // page : Installation
  if (get_current_page() == 2)
  {
    set_page_complete(m_InstallPageBox, false);
    m_ViewLogButton.set_sensitive(false);

    Glib::signal_timeout().connect_once(sigc::mem_fun(*this,
      &MarketClientAssistant::onInstallTimeoutOnce), 250 );
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onLicensesTreeviewChanged()
{
  Gtk::TreeModel::iterator Iter = m_RefLicensesTreeSelection->get_selected();

  m_RefLicenseTextBuffer->set_text("");

  if(Iter)
  {
    Gtk::TreeModel::Row Row = *Iter;
    std::string PackageID = Row.get_value(m_LicensesColumns.m_ID);

    openfluid::market::MetaPackagesCatalog_t::iterator PCit = m_MarketClient.findInTypesMetaPackagesCatalogs(PackageID);

    // id package found ?
    if (PCit != m_MarketClient.getTypesMetaPackagesCatalogs().rbegin()->second.end())
    {
      // Getting license id
      std::string LicenseID = PCit->second.AvailablePackages[PCit->second.Selected].License;

      // Searching license
      std::map<std::string,std::string>::const_iterator LIter = m_MarketClient.getLicensesTexts().find(LicenseID);

      // Displaying license
      if (LIter != m_MarketClient.getLicensesTexts().end())
        m_RefLicenseTextBuffer->set_text(LIter->second);
      else
        m_RefLicenseTextBuffer->set_text(_("(License content not available)"));
    }
  }


}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onLicenseRadioClicked()
{
  if (m_LicensesAcceptRadio.get_active())
    set_page_complete(m_LicensesPageBox, true);
  else
    set_page_complete(m_LicensesPageBox, false);
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onURLComboChanged()
{
  m_MarketClient.disconnect();

  if (m_URLCombo.get_active_row_number() > 0)
  {
    Gtk::TreeModel::iterator TmpIter = m_URLCombo.get_active();
    if (TmpIter)
    {
      Gtk::TreeModel::Row TmpRow = *TmpIter;
      if (TmpRow)
      {
        Glib::ustring TmpURL = TmpRow[m_URLColumns.m_URL];
        m_MarketClient.connect(TmpURL);
      }
    }
  }

  set_page_complete(m_SelectionPageBox,false);

  updateAvailPacksTreeview();
}


// =====================================================================
// =====================================================================


MarketPackWidget* MarketClientAssistant::getAvailPackWidget(const openfluid::ware::WareID_t& ID) const
{
  std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> >::const_iterator APMiter;
  std::list<MarketPackWidget*>::const_iterator APLiter;

  // Searching ID package in AvailPacksWidgets
  for (APMiter = mp_AvailPacksWidgets.begin(); APMiter != mp_AvailPacksWidgets.end(); ++APMiter)
  {
    for (APLiter = APMiter->second.begin(); APLiter != APMiter->second.end(); ++APLiter)
    {
      // package found
      if ((*APLiter)->getID() == ID)
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
  openfluid::market::MetaPackagesCatalog_t DataCatalog = m_MarketClient.getTypesMetaPackagesCatalogs().at(openfluid::market::PackageInfo::DATA);
  openfluid::market::MetaPackagesCatalog_t::iterator PCit;

  // for each dataset package
  for (PCit = DataCatalog.begin(); PCit != DataCatalog.end(); ++PCit)
  {
    std::list<openfluid::ware::WareID_t> Dependencies = PCit->second.AvailablePackages[openfluid::market::MetaPackageInfo::FLUIDX].Dependencies[Type];
    std::list<openfluid::ware::WareID_t>::iterator Dit = Dependencies.begin();

    // searching if ID package is a dependence of this dataset
    while (Dit != Dependencies.end() && !(*Dit == ID && getAvailPackWidget(PCit->second.ID)->isInstall()))
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
  std::string Message = "<b>" + ID + "</b>";
  if (Select)
  {
    Message += _(" needs some dependencies to run.\nDo you want to select this packages ?\n");
  }
  else
  {
    Message += _(" has dependencies selected.\nDo you want to unselect this packages ?\n");
  }

  std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> >::const_iterator APMiter;
  std::list<MarketPackWidget*>::const_iterator APLiter;

  // display list of packs to select/unselect
  for (APMiter = PacksToSelect.begin(); APMiter != PacksToSelect.end(); ++APMiter)
  {
    Message += "\n<u>"+getGraphicTypeName(APMiter->first, true, true)+":</u> ";
    for (APLiter = APMiter->second.begin(); APLiter != APMiter->second.end(); ++APLiter)
    {
      Message += "\n - " + (*APLiter)->getID();
    }
  }

  // creating message window
  Gtk::MessageDialog Dialog(*this, Message, true, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
  Dialog.set_title(_("Dependencies"));
  if (Select)
    Dialog.set_secondary_text(_("Warning, the selected package cannot be used without them."), true);
  int Answer = Dialog.run();

  return Answer == Gtk::RESPONSE_YES;
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::selectDependencies(const openfluid::ware::WareID_t& ID)
{
  openfluid::market::MetaPackagesCatalog_t::iterator PCit = m_MarketClient.findInTypesMetaPackagesCatalogs(ID);

  // package found
  if (PCit != m_MarketClient.getTypesMetaPackagesCatalogs().rbegin()->second.end())
  {
    MarketPackWidget* MPW = getAvailPackWidget(ID);

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
        MarketPackWidget *DependencePack = getAvailPackWidget(DependenceID);

        // dependence pack widget found ?
        if (DependencePack != 0)
        {
          // dependence state different to selected package state
          if (DependencePack->isInstall() != MPW->isInstall())
          {
            // if select or if unselect + no parent selected
            if (MPW->isInstall() ||
                (!MPW->isInstall() && !hasParentSelected(DependencePack->getID(), DMit->first)))
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
        m_MarketClient.setSelectionFlag(MPW->getID(),MPW->getPackageFormat());
      }
      else
      {
        m_MarketClient.setSelectionFlag(MPW->getID(),openfluid::market::MetaPackageInfo::NONE);
      }

      if (APMiter->first != openfluid::market::PackageInfo::DATA)
        m_MarketClient.setSRCBuildOptions(MPW->getID(),((MarketPackWidgetFormat*)MPW)->getEditedBuildOptions());
    }
  }

  set_page_complete(m_SelectionPageBox,Selection);
  m_LicensesDoNotRadio.set_active(true);

}


// =====================================================================
// =====================================================================


openfluid::market::PackageInfo::PackageType MarketClientAssistant::getCurrentTypeTab()
{
  std::string CurrentTabName = m_TypesTabs.get_tab_label_text(*m_TypesTabs.get_nth_page(m_TypesTabs.get_current_page()));
  std::map<openfluid::market::PackageInfo::PackageType,Gtk::VBox*>::const_iterator ATPBiter = mp_AvailTypesPacksBox.begin();

  while (ATPBiter != mp_AvailTypesPacksBox.end() && getGraphicTypeName(ATPBiter->first,true,true) != CurrentTabName)
    ++ATPBiter;

  return ATPBiter->first;
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
  MarketBuildOptionsDialog OptDialog(openfluid::market::MarketPackage::getCommonBuildOptions(CurrentTab),"");

  if (OptDialog.run() == Gtk::RESPONSE_OK)
  {
    // Setting common options of current tab
    openfluid::market::MarketPackage::setCommonBuildOptions(CurrentTab,OptDialog.getEditedOptions());

    std::list<MarketPackWidget*>::iterator APLiter;

    for (APLiter = mp_AvailPacksWidgets[CurrentTab].begin(); APLiter != mp_AvailPacksWidgets[CurrentTab].end(); ++APLiter)
      ((MarketPackWidget*)(*APLiter))->updateDisplayedInfos();
  }
}

// =====================================================================
// =====================================================================


void MarketClientAssistant::onInstallTimeoutOnce()
{
  std::string StrPercent;

  set_page_complete(m_InstallPageBox, false);

  m_MarketClient.preparePackagesInstallation();

  updateInstallTreeview();

  unsigned int PackToInstallCount = m_MarketClient.getCountOfPackagesToInstall();
  unsigned int InstalledCount = 0;

  m_InstallProgressBar.set_fraction(0.0);
  m_InstallProgressBar.set_text(_("0% complete"));

  while( Gtk::Main::events_pending()) Gtk::Main::iteration();

  while (m_MarketClient.hasSelectedPackagesToInstall())
  {

    Gtk::TreeModel::Row TmpRow = m_RefInstallTreeViewModel->children()[InstalledCount];

    m_InstallTreeView.scroll_to_row(Gtk::TreePath(*TmpRow),0.7);

    TmpRow[m_InstallColumns.m_Status] = _("Installation in progress...");
    while( Gtk::Main::events_pending()) Gtk::Main::iteration();

    try
    {
      m_MarketClient.installNextSelectedPackage();
      TmpRow[m_InstallColumns.m_Status] = _("Success");
      while( Gtk::Main::events_pending()) Gtk::Main::iteration();
    }
    catch (openfluid::base::OFException& E)
    {
      std::string ErrorMsg(E.what());
      TmpRow[m_InstallColumns.m_Status] = _("Failed");
      while( Gtk::Main::events_pending()) Gtk::Main::iteration();
    }
    InstalledCount++;

    m_InstallProgressBar.set_fraction(float(InstalledCount)/float(PackToInstallCount));
    openfluid::tools::ConvertValue(int(m_InstallProgressBar.get_fraction()*100),&StrPercent);
    m_InstallProgressBar.set_text(StrPercent+_("% complete"));

    while( Gtk::Main::events_pending()) Gtk::Main::iteration();
  }

  m_InstallProgressBar.set_fraction(1.0);
  m_InstallProgressBar.set_text(_("100% complete"));
  while( Gtk::Main::events_pending()) Gtk::Main::iteration();

  m_ViewLogButton.set_sensitive(true);

  set_page_complete(m_InstallPageBox, true);
}

// =====================================================================
// =====================================================================


void MarketClientAssistant::updateAvailPacksTreeview()
{
  // change mouse cursor to watch
  get_window()->set_cursor(Gdk::Cursor(Gdk::WATCH));
  while (Gtk::Main::events_pending ()) Gtk::Main::iteration ();

  openfluid::market::TypesMetaPackagesCatalogs_t Catalogs;
  openfluid::market::TypesMetaPackagesCatalogs_t::const_iterator TCIter;
  openfluid::market::MetaPackagesCatalog_t::const_iterator CIter;

  std::map<openfluid::market::PackageInfo::PackageType,Gtk::VBox*>::iterator ATPBiter;

  for (ATPBiter = mp_AvailTypesPacksBox.begin(); ATPBiter != mp_AvailTypesPacksBox.end(); ++ATPBiter)
  {
    // Tab VBox exists ?
    if (ATPBiter->second != 0)
    {
      std::vector<Gtk::Widget*> AvailPacksBoxChildren = ATPBiter->second->get_children();
      std::vector<Gtk::Widget*>::iterator APBCiter;

      // removing widgets from available packages display
      for (APBCiter=AvailPacksBoxChildren.begin();APBCiter!=AvailPacksBoxChildren.end();++APBCiter)
        ATPBiter->second->remove(**APBCiter);
    }
  }


  std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> >::iterator APMiter;
  std::list<MarketPackWidget*>::iterator APLiter;

  // destroying widgets from available packages list
  for (APMiter = mp_AvailPacksWidgets.begin(); APMiter != mp_AvailPacksWidgets.end(); ++APMiter)
  {
    for (APLiter = APMiter->second.begin(); APLiter != APMiter->second.end(); ++APLiter)
    {
      MarketPackWidget* MPW;
      MPW = *APLiter;
      delete MPW;
    }
  }

  mp_AvailPacksWidgets.clear();


  // removing tabs
  while (m_TypesTabs.get_n_pages() > 0)
  {
    m_TypesTabs.remove_page();
  }


  // initializing pointers
  for (ATPBiter = mp_AvailTypesPacksBox.begin(); ATPBiter != mp_AvailTypesPacksBox.end(); ++ATPBiter)
  {
    ATPBiter->second = 0;
    mp_AvailTypesPacksSWindow[ATPBiter->first] = 0;
    mp_TabBox[ATPBiter->first] = 0;

    mp_ActionButtonsBox[ATPBiter->first] = 0;
    mp_SelectAllButton[ATPBiter->first] = 0;
    mp_SelectNoneButton[ATPBiter->first] = 0;
    mp_CommonBuildConfigButton[ATPBiter->first] = 0;
  }


  Catalogs = m_MarketClient.getTypesMetaPackagesCatalogs();

  for (TCIter = Catalogs.begin(); TCIter != Catalogs.end(); ++TCIter)
  {
    // New tab
    if (!TCIter->second.empty())
    {
      // Creating ScrolledWindow and VBox containers
      mp_AvailTypesPacksSWindow[TCIter->first] = Gtk::manage(new Gtk::ScrolledWindow());
      mp_AvailTypesPacksBox[TCIter->first] = Gtk::manage(new Gtk::VBox());

      mp_AvailTypesPacksSWindow[TCIter->first]->add(*mp_AvailTypesPacksBox[TCIter->first]);
      mp_AvailTypesPacksSWindow[TCIter->first]->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);

      // removing ScrolledWindow border
      Gtk::Viewport* pViewport=(Gtk::Viewport*) mp_AvailTypesPacksSWindow[TCIter->first]->get_child();
      pViewport->set_shadow_type(Gtk::SHADOW_NONE);


      // Adding packages in VBox
      for (CIter = TCIter->second.begin(); CIter != TCIter->second.end(); ++CIter)
      {
        // Creating MarketPackWidget
        if (TCIter->first == openfluid::market::PackageInfo::DATA)
          mp_AvailPacksWidgets[TCIter->first].push_back(new MarketPackWidget(TCIter->first,CIter->second));
        else
          mp_AvailPacksWidgets[TCIter->first].push_back(new MarketPackWidgetFormat(TCIter->first,CIter->second));


        mp_AvailPacksWidgets[TCIter->first].back()->signal_install_modified().connect(
          sigc::bind<openfluid::ware::WareID_t>(sigc::mem_fun(*this,&MarketClientAssistant::selectDependencies),CIter->first)
        );

        mp_AvailPacksWidgets[TCIter->first].back()->signal_install_modified().connect(
            sigc::mem_fun(*this,&MarketClientAssistant::onPackageInstallModified)
        );

        mp_AvailTypesPacksBox[TCIter->first]->pack_start(*(new Gtk::HSeparator()),Gtk::PACK_SHRINK,0);
        mp_AvailTypesPacksBox[TCIter->first]->pack_start(*(mp_AvailPacksWidgets[TCIter->first].back()),Gtk::PACK_SHRINK,0);
      }

      // Creating buttons
      mp_ActionButtonsBox[TCIter->first] = Gtk::manage(new Gtk::HBox());
      mp_SelectAllButton[TCIter->first] = Gtk::manage(new Gtk::Button(_("Select all")));
      mp_SelectNoneButton[TCIter->first] = Gtk::manage(new Gtk::Button(_("Select none")));
      mp_CommonBuildConfigButton[TCIter->first] = Gtk::manage(new Gtk::Button());

      //m_CommonBuildConfigButton.set_label(_("Configure source builds"));
      Gtk::Image* Img = Gtk::manage(new Gtk::Image(Gtk::Stock::PREFERENCES, Gtk::ICON_SIZE_BUTTON));
      Gtk::Label* Lbl = Gtk::manage(new Gtk::Label(_("Edit build options")));
      Gtk::HBox* Box = Gtk::manage(new Gtk::HBox());
      Box->pack_start(*Img,Gtk::PACK_SHRINK,3);
      Box->pack_end(*Lbl,Gtk::PACK_SHRINK,3);
      mp_CommonBuildConfigButton[TCIter->first]->add(*Box);
      mp_CommonBuildConfigButton[TCIter->first]->show_all_children(true);

      // Disable build options button for datasets
      if (TCIter->first == openfluid::market::PackageInfo::DATA)
        mp_CommonBuildConfigButton[TCIter->first]->set_sensitive(false);


      mp_ActionButtonsBox[TCIter->first]->pack_start(*mp_SelectAllButton[TCIter->first],Gtk::PACK_SHRINK,0);
      mp_ActionButtonsBox[TCIter->first]->pack_start(*mp_SelectNoneButton[TCIter->first],Gtk::PACK_SHRINK,12);
      mp_ActionButtonsBox[TCIter->first]->pack_start(*Gtk::manage(new Gtk::EventBox()),Gtk::PACK_EXPAND_WIDGET,0);
      mp_ActionButtonsBox[TCIter->first]->pack_start(*mp_CommonBuildConfigButton[TCIter->first],Gtk::PACK_SHRINK,0);


      mp_TabBox[TCIter->first] = Gtk::manage(new Gtk::VBox());
      mp_TabBox[TCIter->first]->pack_start(*mp_AvailTypesPacksSWindow[TCIter->first],Gtk::PACK_EXPAND_WIDGET,6);
      mp_TabBox[TCIter->first]->pack_start(*(Gtk::manage(new Gtk::HSeparator())),Gtk::PACK_SHRINK,5);
      mp_TabBox[TCIter->first]->pack_start(*mp_ActionButtonsBox[TCIter->first],Gtk::PACK_SHRINK,4);


      mp_SelectAllButton[TCIter->first]->signal_clicked().connect(
          sigc::mem_fun(*this, &MarketClientAssistant::onSelectAllClicked)
        );

      mp_SelectNoneButton[TCIter->first]->signal_clicked().connect(
          sigc::mem_fun(*this, &MarketClientAssistant::onSelectNoneClicked)
        );

      mp_CommonBuildConfigButton[TCIter->first]->signal_clicked().connect(
          sigc::mem_fun(*this, &MarketClientAssistant::onCommonBuildConfigClicked)
        );


      // Create tab
      m_TypesTabs.append_page(*mp_TabBox[TCIter->first], getGraphicTypeName(TCIter->first, true,true));

      // Load tab content
      int LastPage = m_TypesTabs.get_n_pages() - 1;
      m_TypesTabs.get_nth_page(LastPage)->show_all();
      m_TypesTabs.set_current_page(LastPage);
      m_TypesTabs.set_current_page(0);
    }
  }


  // change mouse cursor to default
  get_window()->set_cursor();
  while (Gtk::Main::events_pending ()) Gtk::Main::iteration ();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::initializeLicencesTreeView()
{
  openfluid::market::TypesMetaPackagesCatalogs_t Catalogs;
  openfluid::market::TypesMetaPackagesCatalogs_t::const_iterator TCIter;
  openfluid::market::MetaPackagesCatalog_t::const_iterator CIter;

  m_LicensesTreeView.remove_all_columns();
  m_RefLicenseTreeViewModel->clear();

  std::map<openfluid::market::PackageInfo::PackageType,bool> RowCreated;
  Catalogs = m_MarketClient.getTypesMetaPackagesCatalogs();

  for (TCIter = Catalogs.begin(); TCIter != Catalogs.end(); ++TCIter)
  {
    Gtk::TreeModel::Row TypeRow;

    for (CIter = TCIter->second.begin(); CIter != TCIter->second.end(); ++CIter)
    {
      // If package is selected
      if (CIter->second.Selected != openfluid::market::MetaPackageInfo::NONE)
      {
        // If TypeRow not created
        if (!RowCreated[TCIter->first])
        {
          TypeRow = *(m_RefLicenseTreeViewModel->append());
          TypeRow[m_LicensesColumns.m_ID] = getGraphicTypeName(TCIter->first,true,true);

          RowCreated[TCIter->first] = true;
        }

        // Creation of sub-row
        Gtk::TreeModel::Row TmpRow = *(m_RefLicenseTreeViewModel->append(TypeRow.children()));
        TmpRow[m_LicensesColumns.m_ID] = CIter->first;
      }
    }
  }

  m_LicensesTreeView.append_column("", m_LicensesColumns.m_ID);


  // open first type row
  Gtk::TreeModel::Path RowPath = m_RefLicenseTreeViewModel->get_path(m_RefLicenseTreeViewModel->children().begin());
  m_LicensesTreeView.expand_row(RowPath, false);

  // select first package
  Gtk::TreeModel::Row TmpSelRow = m_RefLicenseTreeViewModel->children()[0]->children()[0];
  m_RefLicensesTreeSelection->select(TmpSelRow);
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::updateInstallTreeview()
{
  std::list<openfluid::market::MarketPackage*> PacksList = m_MarketClient.getSelectionToInstall();
  std::list<openfluid::market::MarketPackage*>::const_iterator PLiter;

  for (PLiter = PacksList.begin();PLiter != PacksList.end();++PLiter )
  {
    Gtk::TreeModel::Row TmpRow = *(m_RefInstallTreeViewModel->append());
    TmpRow[m_InstallColumns.m_ID] = (*PLiter)->getID();
    TmpRow[m_InstallColumns.m_Type] = getGraphicTypeName((*PLiter)->getPackageType(),true,false);
    if ((*PLiter)->getFormat() == openfluid::market::MetaPackageInfo::BIN) TmpRow[m_InstallColumns.m_Format] = _("binary");
    else if ((*PLiter)->getFormat() == openfluid::market::MetaPackageInfo::SRC) TmpRow[m_InstallColumns.m_Format] = _("source");
    else TmpRow[m_InstallColumns.m_Format] = _("dataset");
    TmpRow[m_InstallColumns.m_Status] = _("Pending");
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onViewLogClicked()
{

  ViewLogFileWindow LogWindow(openfluid::market::MarketPackage::getLogFile());

  Gtk::Main::run(LogWindow);
}


// =====================================================================
// =====================================================================


std::string MarketClientAssistant::getGraphicTypeName(const openfluid::market::PackageInfo::PackageType& Type, const bool Maj, const bool Plural)
{
  std::string TypeNames[] = { _("simulator"), _("observer"), _("builder extension"), _("dataset")};
  std::string PluralTypeNames[] = { _("simulators"), _("observers"), _("builder extensions"), _("datasets")};
  std::string Name;

  if (Plural)
    Name = PluralTypeNames[Type];
  else
    Name = TypeNames[Type];

  if (Maj)
    Name[0] = toupper(Name[0]);

  return Name;
}


} } //namespaces

