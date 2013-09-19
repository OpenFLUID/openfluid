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
  \file MarketClientAssistant.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MARKETCLIENTASSISTANT_HPP__
#define __MARKETCLIENTASSISTANT_HPP__

#include <QWizard>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QTabWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QTreeWidget>
#include <QRadioButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QProgressBar>
#include <QSplitter>
#include <QStandardItemModel>

#include <openfluid/dllexport.hpp>
#include <openfluid/market/MarketClient.hpp>
#include <openfluid/guicommon/MarketWizardPage.hpp>


namespace openfluid { namespace guicommon {

class MarketPackWidget;

// TODO review parenting of all widgets
class DLLEXPORT MarketClientAssistant : public QWizard
{
  Q_OBJECT

  private:

    // ===== Package selection =====//
    MarketWizardPage m_SelectionPage;
    QVBoxLayout m_SelectionPageBox;

    QHBoxLayout m_URLBox;
    QLabel m_URLLabel;
    QComboBox m_URLCombo;
    QStandardItemModel m_RefURLComboBoxModel;

    class URLComboColumns : public QList<QStandardItem*>
    {
      public:

        enum { NAME, URL};

        QStandardItem *mp_Name;
        QStandardItem *mp_URL;

        URLComboColumns() {}

        void appendItems() { clear(); append(mp_Name); append(mp_URL); }
    };

    URLComboColumns m_URLColumns;


    // Tabs
    QTabWidget m_TypesTabs;

    // Pages and main box of tab
    std::map<openfluid::market::PackageInfo::PackageType,QWidget*> mp_TabPage;
    std::map<openfluid::market::PackageInfo::PackageType,QVBoxLayout*> mp_TabBox;

    // Box and ScrolledWindow for packages list
    std::map<openfluid::market::PackageInfo::PackageType,QVBoxLayout*> mp_AvailTypesPacksBox;
    std::map<openfluid::market::PackageInfo::PackageType,QScrollArea*> mp_AvailTypesPacksSWindow;

    // Buttons of tab
    std::map<openfluid::market::PackageInfo::PackageType,QHBoxLayout*> mp_ActionButtonsBox;
    std::map<openfluid::market::PackageInfo::PackageType,QPushButton*> mp_SelectAllButton;
    std::map<openfluid::market::PackageInfo::PackageType,QPushButton*> mp_SelectNoneButton;
    std::map<openfluid::market::PackageInfo::PackageType,QPushButton*> mp_CommonBuildConfigButton;

    // List of MarketPackWidget
    std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> > mp_AvailPacksWidgets;


    /**
     @return Type of current tab
    */
    openfluid::market::PackageInfo::PackageType getCurrentTypeTab();


    /**
     @return MarketPackWidget of ID package
     @param ID of package
    */
    MarketPackWidget* getAvailPackWidget(const openfluid::ware::WareID_t& ID) const;

    /**
     @return true if ID package passed as parameter has datasets selected which need it
     @param ID of dependence
     @param Type Type of dependence
    */
    bool hasParentSelected(const openfluid::ware::WareID_t& ID,
        const openfluid::market::PackageInfo::PackageType& Type);

    /**
     @return choice of user to apply action of selected dataset to this dependencies
     @param ID of package selected
     @param Select action applied on the package
     @param List of dependencies to select
    */
    bool getUserChoice(const openfluid::ware::WareID_t& ID, const bool Select,
        const std::map<openfluid::market::PackageInfo::PackageType,std::list<MarketPackWidget*> >& PacksToSelect);


    // ===== Licenses =====//
    MarketWizardPage m_LicensesPage;
    QVBoxLayout m_LicensesPageBox;

    QLabel m_LicensesLabel;

    // Adjustable container for packages treeview and license text view
    QSplitter m_LicensesReviewPaned;

    QTreeWidget *mp_LicensesTreeView;

    QTextEdit *mp_LicensesTextView;


    QScrollArea m_LicensesListSWindow;
    QScrollArea m_LicensesReviewSWindow;

    QRadioButton m_LicensesAcceptRadio;
    QRadioButton m_LicensesDoNotRadio;




    // ===== Download and install =====//
    MarketWizardPage m_InstallPage;

    QVBoxLayout m_InstallPageBox;

    QTableWidget *mp_InstallTable;


    class InstallTableColumns
    {
      public:

        enum { ID, TYPE, FORMAT, STATUS};

        QTableWidgetItem *mp_ID;
        QTableWidgetItem *mp_Type;
        QTableWidgetItem *mp_Format;
        QTableWidgetItem *mp_Status;

        InstallTableColumns() {}

        void setFlags()
        {
          mp_ID->setFlags(mp_ID->flags() ^ Qt::ItemIsEditable);
          mp_Type->setFlags(mp_Type->flags() ^ Qt::ItemIsEditable);
          mp_Format->setFlags(mp_Format->flags() ^ Qt::ItemIsEditable);
          mp_Status->setFlags(mp_Status->flags() ^ Qt::ItemIsEditable);
        }
    };

    InstallTableColumns m_InstallColumns;


    QScrollArea m_InstallSWindow;

    QProgressBar m_InstallProgressBar;
    QPushButton m_ViewLogButton;


    void setupSelectionPage();
    void setupLicensesPage();
    void setupDownloadPage();


    /**
     * Removes tabs, destroys content and clear screen
    */
    void clearAvailPacksTreeview();

    /**
     * Creates tabs and display all available packages in Market-place selected
    */
    void updateAvailPacksTreeview();

    /**
     * Indicates that no marketplace reached and unselects repository
    */
    void displayMarketplaceError();


    void initializeLicencesTreeView();

    void updateInstallTreeview();

    openfluid::market::MarketClient m_MarketClient;

    int m_InstallationTimeout;


  private slots:

    void onApply();
    void onCancel();
    void onClose();
    void onPrepare(int Id);

    void onURLComboChanged(int RowNumber);

    /**
     * Selects all packages of current tab
    */
    void onSelectAllClicked();

    /**
     * Unselects all packages of current tab
    */
    void onSelectNoneClicked();

    /**
     * Edit common options of packages of the current tab
    */
    void onCommonBuildConfigClicked();


    /**
     Selects dependencies of ID package
     @param ID of package selected
    */
    void selectDependencies(const openfluid::ware::WareID_t& ID);


    void onPackageInstallModified();


    void onLicenseRadioClicked(bool Checked);


    void onLicensesTreeviewChanged(QTreeWidgetItem *CurrentItem);


    void onInstallTimeoutOnce();

    void onViewLogClicked();


  public:

    MarketClientAssistant(QWidget* Parent);

    virtual ~MarketClientAssistant();

    /**
     @return string name of type passed as parameter
     @param Type Type of package
     @param Maj First letter in maj
     @param Plural Return plural name
    */
    static QString getGraphicTypeName(const openfluid::market::PackageInfo::PackageType& Type, const bool Maj, const bool Plural);
};

} } //namespaces


#endif /* __MARKETCLIENTASSISTANT_HPP__ */
