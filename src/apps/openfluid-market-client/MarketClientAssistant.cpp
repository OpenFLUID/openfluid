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



#include "MarketClientAssistant.hpp"

#include <iostream>
#include <boost/filesystem.hpp>

#include <openfluid/base.hpp>


MarketClientAssistant::MarketClientAssistant()
: m_SelectionPageBox(false,0),
  m_LicensesPageBox(false,0),
  m_InstallPageBox(false,0)
{

  openfluid::market::MarketPackage::initialize();

  set_title("OpenFLUID Market client");
  set_border_width(20);
  set_default_size(800, 600);
  set_position(Gtk::WIN_POS_CENTER);


  setupSelectionPage();
  setupLicensesPage();
  setupDownloadPage();


  append_page(m_SelectionPageBox);
  append_page(m_LicensesPageBox);
  append_page(m_InstallPageBox);


  set_page_title(*get_nth_page(0), "Packages selection");
  set_page_title(*get_nth_page(1), "Licenses");
  set_page_title(*get_nth_page(2), "Download and install");


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

  m_URLLabel.set_text("Marketplace: ");

  m_RefURLComboBoxModel = Gtk::ListStore::create(m_URLColumns);
  m_URLCombo.set_model(m_RefURLComboBoxModel);


  Gtk::TreeModel::Row TmpURLRow = *(m_RefURLComboBoxModel->append());
  TmpURLRow[m_URLColumns.m_Name] = "--- No marketplace selected ---";
  TmpURLRow[m_URLColumns.m_URL] = "";


  Glib::KeyFile MarketConf;



  if (boost::filesystem::exists(boost::filesystem::path(openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile())) &&
      MarketConf.load_from_file(openfluid::base::RuntimeEnvironment::getInstance()->getDefaultConfigFile()))
  {
    if (MarketConf.has_group("openfluid.market.marketplaces"))
    {
      std::vector<std::string> PlacesKeys = MarketConf.get_keys("openfluid.market.marketplaces");

      for (unsigned int i=0; i< PlacesKeys.size();i++)
      {
        std::vector<std::string> PlaceInfo = MarketConf.get_string_list("openfluid.market.marketplaces",PlacesKeys[i]);
        if (PlaceInfo.size() == 2 )
        {
          TmpURLRow = *(m_RefURLComboBoxModel->append());
          TmpURLRow[m_URLColumns.m_Name] = PlaceInfo[0];
          TmpURLRow[m_URLColumns.m_URL] = PlaceInfo[1];
        }
      }
    }
  }



  m_URLCombo.pack_start(m_URLColumns.m_Name);
  m_URLCombo.pack_start(m_URLColumns.m_URL);

  m_URLCombo.set_active(0);

  m_URLBox.pack_start(m_URLLabel,Gtk::PACK_SHRINK);
  m_URLBox.pack_start(m_URLCombo,Gtk::PACK_EXPAND_WIDGET);


  m_AvailPacksSWindow.add(m_AvailPacksBox);
  m_AvailPacksSWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);


  m_SelectAllButton.set_label("Select all");
  m_SelectAllButton.set_sensitive(false);
  m_SelectNoneButton.set_label("Select none");
  m_SelectNoneButton.set_sensitive(false);

  m_SelectionActionsBox.pack_start(m_SelectAllButton,Gtk::PACK_SHRINK,0);
  m_SelectionActionsBox.pack_start(m_SelectNoneButton,Gtk::PACK_SHRINK,12);


  m_SelectionPageBox.set_border_width(12);
  m_SelectionPageBox.pack_start(m_URLBox,Gtk::PACK_SHRINK,12);
  m_SelectionPageBox.pack_start(m_AvailPacksSWindow,Gtk::PACK_EXPAND_WIDGET,6);
  m_SelectionPageBox.pack_start(m_SelectionActionsBox,Gtk::PACK_SHRINK);


  m_URLCombo.signal_changed().connect(
    sigc::mem_fun(*this, &MarketClientAssistant::onURLComboChanged)
  );

  m_SelectAllButton.signal_clicked().connect(
      sigc::mem_fun(*this, &MarketClientAssistant::onSelectAllClicked)
    );

  m_SelectNoneButton.signal_clicked().connect(
      sigc::mem_fun(*this, &MarketClientAssistant::onSelectNoneClicked)
    );

}


// =====================================================================
// =====================================================================


void MarketClientAssistant::setupLicensesPage()
{

  m_LicensesLabel.set_text("Licenses:");
  m_LicensesLabel.set_alignment(0,0);

  m_RefLicenseTextBuffer = Gtk::TextBuffer::create();
  m_RefLicenseTextBuffer->set_text("");

  m_LicensesTextView.set_editable(false);
  m_LicensesTextView.set_buffer(m_RefLicenseTextBuffer);

  m_LicensesSWindow.add(m_LicensesTextView);
  m_LicensesSWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_RefLicenseTreeViewModel = Gtk::ListStore::create(m_LicensesColumns);
  m_LicensesTreeView.set_model(m_RefLicenseTreeViewModel);
  m_LicensesTreeView.set_headers_visible(false);

  m_RefLicensesTreeSelection = m_LicensesTreeView.get_selection();

  m_LicensesReviewBox.set_spacing(12);
  m_LicensesReviewBox.pack_start(m_LicensesTreeView,Gtk::PACK_SHRINK);
  m_LicensesReviewBox.pack_start(m_LicensesSWindow,Gtk::PACK_EXPAND_WIDGET);


  m_LicensesAcceptRadio.set_label("Accept packages licenses");
  m_LicensesDoNotRadio.set_label("Do not accept packages licenses");
  Gtk::RadioButton::Group Group = m_LicensesAcceptRadio.get_group();
  m_LicensesDoNotRadio.set_group(Group);
  m_LicensesDoNotRadio.set_active(true);

  m_LicensesPageBox.set_border_width(12);
  m_LicensesPageBox.pack_start(m_LicensesLabel,Gtk::PACK_SHRINK);
  m_LicensesPageBox.pack_start(m_LicensesReviewBox,Gtk::PACK_EXPAND_WIDGET);
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
  m_RefInstallTextBuffer = Gtk::TextBuffer::create();
  m_RefInstallTextBuffer->set_text("");

  m_InstallTextView.set_editable(false);
  m_InstallTextView.set_buffer(m_RefInstallTextBuffer);
  m_InstallTextView.set_wrap_mode(Gtk::WRAP_WORD);

  m_InstallSWindow.add(m_InstallTextView);
  m_InstallSWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

  m_InstallProgressBar.set_size_request(-1,30);

  m_InstallPageBox.set_border_width(12);
  m_InstallPageBox.pack_start(m_InstallProgressBar,Gtk::PACK_SHRINK,12);
  m_InstallPageBox.pack_start(m_InstallSWindow,Gtk::PACK_EXPAND_WIDGET,12);

}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onApply()
{
  std::cout << "Apply was clicked on page " << get_page_title(*get_nth_page(get_current_page()));
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onCancel()
{
  std::cout << "Cancel was clicked on page " << get_page_title(*get_nth_page(get_current_page()))  << std::endl;
  hide();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onClose()
{
  std::cout << "Assistant was closed" << std::endl;
  hide();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onPrepare(Gtk::Widget* /*Widget*/)
{
  set_title(Glib::ustring::compose("OpenFLUID Market client (Step %1 of %2)",get_current_page() + 1, get_n_pages()));



  // page : Licenses
  if (get_current_page() == 1)
  {
    initializeLicencesTreeView();
  }


  // page : Installation
  if (get_current_page() == 2)
  {
    set_page_complete(m_InstallPageBox, false);


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
    openfluid::market::MetaPackagesCatalog_t Catalog = m_MarketClient.getMetaPackagesCatalog();
    std::string LicenseID = Catalog[PackageID].AvailablePackages[Catalog[PackageID].Selected].License;

    std::map<std::string,std::string>::const_iterator LIter = m_MarketClient.getLicensesTexts().find(LicenseID);

    if (LIter != m_MarketClient.getLicensesTexts().end())
      m_RefLicenseTextBuffer->set_text(LIter->second);

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
        m_SelectAllButton.set_sensitive(!m_MarketClient.getMetaPackagesCatalog().empty());
        m_SelectNoneButton.set_sensitive(!m_MarketClient.getMetaPackagesCatalog().empty());
      }
    }
  }
  else
  {
    m_SelectAllButton.set_sensitive(false);
    m_SelectNoneButton.set_sensitive(false);
  }

  set_page_complete(m_SelectionPageBox,false);

  updateAvailPacksTreeview();
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onPackageInstallModified()
{
  bool Selection = false;

  std::list<MarketPackWidget*>::iterator APLiter;
  for (APLiter=mp_AvailPacksWidgets.begin();APLiter!=mp_AvailPacksWidgets.end();++APLiter)
  {
    MarketPackWidget* MPW;
    MPW = *APLiter;
    Selection = Selection || MPW->isInstall();
    if (MPW->isInstall())
      m_MarketClient.setSelectionFlag(MPW->getID(),MPW->getPackageFormat());
    else m_MarketClient.setSelectionFlag(MPW->getID(),openfluid::market::MetaPackageInfo::NONE);
  }

  set_page_complete(m_SelectionPageBox,Selection);
  m_LicensesDoNotRadio.set_active(true);

}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onSelectAllClicked()
{
  std::list<MarketPackWidget*>::iterator APLiter;

  for (APLiter=mp_AvailPacksWidgets.begin();APLiter!=mp_AvailPacksWidgets.end();++APLiter)
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

  for (APLiter=mp_AvailPacksWidgets.begin();APLiter!=mp_AvailPacksWidgets.end();++APLiter)
  {
    MarketPackWidget* MPW;
    MPW = *APLiter;
    MPW->setInstall(false);
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::onInstallTimeoutOnce()
{
  set_page_complete(m_InstallPageBox, false);

  m_MarketClient.preparePackagesInstallation();

  unsigned int PackToInstallCount = m_MarketClient.getCountOfPackagesToInstall();
  unsigned int InstalledCount = 0;

  m_InstallProgressBar.set_fraction(0.0);

  while( Gtk::Main::events_pending()) Gtk::Main::iteration();

  while (m_MarketClient.hasSelectedPackagesToInstall())
  {
    m_RefInstallTextBuffer->insert(m_RefInstallTextBuffer->end(),"\n");
    m_RefInstallTextBuffer->insert(m_RefInstallTextBuffer->end(),"Installing package "+m_MarketClient.getNextPackageToBeInstalled()->getID());
    m_InstallTextView.scroll_to_mark(m_RefInstallTextBuffer->get_insert(),0);

    while( Gtk::Main::events_pending()) Gtk::Main::iteration();

    try
    {
      m_MarketClient.installNextSelectedPackage();
      m_RefInstallTextBuffer->insert(m_RefInstallTextBuffer->end(),"  [Done]\n\n");

      while( Gtk::Main::events_pending()) Gtk::Main::iteration();
    }
    catch (openfluid::base::OFException& E)
    {
      std::string ErrorMsg(E.what());
      m_RefInstallTextBuffer->insert(m_RefInstallTextBuffer->end(),"  [Error]["+ErrorMsg+"]\n\n");

      while( Gtk::Main::events_pending()) Gtk::Main::iteration();
    }
    InstalledCount++;

    m_InstallProgressBar.set_fraction(float(InstalledCount)/float(PackToInstallCount));
    m_RefInstallTextBuffer->insert(m_RefInstallTextBuffer->end(),"********************************************************************************\n");
    m_InstallTextView.scroll_to_mark(m_RefInstallTextBuffer->get_insert(),0);
    while( Gtk::Main::events_pending()) Gtk::Main::iteration();
  }

  m_InstallProgressBar.set_fraction(1.0);
  m_RefInstallTextBuffer->insert(m_RefInstallTextBuffer->end(),"\n\nInstallation completed.");
  m_InstallTextView.scroll_to_mark(m_RefInstallTextBuffer->get_insert(),0);
  while( Gtk::Main::events_pending()) Gtk::Main::iteration();

  set_page_complete(m_InstallPageBox, true);
}

// =====================================================================
// =====================================================================


void MarketClientAssistant::updateAvailPacksTreeview()
{
  openfluid::market::MetaPackagesCatalog_t Catalog;
  openfluid::market::MetaPackagesCatalog_t::const_iterator CIter;


  std::list<MarketPackWidget*>::iterator APLiter;

  for (APLiter=mp_AvailPacksWidgets.begin();APLiter!=mp_AvailPacksWidgets.end();++APLiter)
  {
    MarketPackWidget* MPW;
    MPW = *APLiter;
    m_AvailPacksBox.remove(*MPW);
    delete MPW;
  }
  mp_AvailPacksWidgets.clear();



  Catalog = m_MarketClient.getMetaPackagesCatalog();

  for (CIter=Catalog.begin();CIter!=Catalog.end();++CIter)
  {

    std::map<openfluid::market::MetaPackageInfo::SelectionType,openfluid::market::PackageInfo>::const_iterator PIter;

    bool TmpBin = false;
    bool TmpSrc = false;

    for (PIter = CIter->second.AvailablePackages.begin();PIter!=CIter->second.AvailablePackages.end();++PIter)
    {
      if (PIter->first == openfluid::market::MetaPackageInfo::BIN) TmpBin = true;
      else TmpSrc = true;
    }


    mp_AvailPacksWidgets.push_back(new MarketPackWidget(CIter->first,TmpBin,TmpSrc));
    mp_AvailPacksWidgets.back()->signal_install_modified().connect(
        sigc::mem_fun(*this,&MarketClientAssistant::onPackageInstallModified)
    );

    m_AvailPacksBox.pack_start(*(mp_AvailPacksWidgets.back()),Gtk::PACK_SHRINK,10);

    m_AvailPacksBox.show_all_children();
  }
}


// =====================================================================
// =====================================================================


void MarketClientAssistant::initializeLicencesTreeView()
{
  openfluid::market::MetaPackagesCatalog_t Catalog;
  openfluid::market::MetaPackagesCatalog_t::const_iterator CIter;


  m_LicensesTreeView.remove_all_columns();
  m_RefLicenseTreeViewModel->clear();

  Catalog = m_MarketClient.getMetaPackagesCatalog();

  for (CIter=Catalog.begin();CIter!=Catalog.end();++CIter)
  {

    if (CIter->second.Selected != openfluid::market::MetaPackageInfo::NONE)
    {
      Gtk::TreeModel::Row TmpRow = *(m_RefLicenseTreeViewModel->append());
      TmpRow[m_LicensesColumns.m_ID] = CIter->first;
    }
  }

  m_LicensesTreeView.append_column("", m_LicensesColumns.m_ID);


  // select first package
  Gtk::TreeModel::Row TmpSelRow = m_RefLicenseTreeViewModel->children()[0];
  m_RefLicensesTreeSelection->select(TmpSelRow);

}


