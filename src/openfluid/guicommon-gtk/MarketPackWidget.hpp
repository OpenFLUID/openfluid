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
  \file MarketPackWidget.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MARKETPACKWIDGET_HPP__
#define __MARKETPACKWIDGET_HPP__


#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/combobox.h>
#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/liststore.h>

#include <openfluid/dllexport.hpp>
#include <openfluid/market/MarketInfos.hpp>


namespace openfluid { namespace guicommon {

// =====================================================================
// =====================================================================

class DLLEXPORT MarketPackWidget : public Gtk::EventBox
{

  public:
    typedef sigc::signal<void> signal_install_modified_t;

  private:
    Gtk::Image* m_EmptyCartImage;
    Gtk::Image* m_FullCartImage;

    Gtk::Label m_VersionLabel;

    bool onButtonRelease(GdkEventButton* Event);

  protected:
    openfluid::market::PackageInfo::PackageType m_PackageType;
    openfluid::market::MetaPackageInfo m_MetaPackInfo;

    Gtk::Label m_IDLabel;
    Gtk::Label m_LicenseLabel;

    Gtk::HBox m_MainHBox;

    Gtk::ToggleButton m_InstallToggle;

    Gtk::VBox m_DetailsLeftVBox;
    Gtk::VBox m_DetailsRightVBox;

    signal_install_modified_t m_signal_install_modified;

    void onInstallModified();

    static std::string replaceByUnknownIfEmpty(const std::string& Str);

    static std::string replaceByNoneIfEmpty(const std::string& Str);


  public:
    MarketPackWidget(const openfluid::market::PackageInfo::PackageType& Type,const openfluid::market::MetaPackageInfo& MetaPackInfo);

    ~MarketPackWidget();

    std::string getID() const { return m_MetaPackInfo.ID; };

    bool isInstall() const { return m_InstallToggle.get_active(); };

    void setInstall(bool Install) { m_InstallToggle.set_active(Install); };

    /**
     @return selected format in combobox
    */
    virtual openfluid::market::MetaPackageInfo::SelectionType getPackageFormat() const;

    signal_install_modified_t signal_install_modified();

    /**
     Creates tooltip for market pack widget
    */
    virtual void updateDisplayedInfos();


};


} } //namespaces


#endif /* __MARKETPACKWIDGET_HPP__ */
