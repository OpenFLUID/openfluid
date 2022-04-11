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
  @file WaresSrcExportDialog.hpp
  @brief Header of ...

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WARESSRCEXPORTDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_WARESSRCEXPORTDIALOG_HPP__


#include <QDialog>
#include <QListWidget>

#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/dllexport.hpp>


namespace Ui {
  class WaresSrcExportDialog;
}


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WaresSrcExportDialog: public openfluid::ui::common::MessageDialog
{
  Q_OBJECT


  private slots :

    bool check();

    void onPackagePathButtonClicked();

    void exportToPackage();


  private:

    Ui::WaresSrcExportDialog* ui;

    typedef QMap<openfluid::ware::WareType, QListWidget*> ListWidgetsByWareType_t;

    ListWidgetsByWareType_t m_ListWidgetsByWareType;

    void initWaresLists();


  public:

    explicit WaresSrcExportDialog(QWidget* Parent);

    QString getPackageFilePath();

    QString getPackagers();

    QString getPackageDesciption();

    QStringList getSelectedWares();

};


} } } //namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESSRCEXPORTDIALOG_HPP__ */
