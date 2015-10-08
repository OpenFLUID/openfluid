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
 @file WaresSrcImportDialog.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__


#include <QDialog>
#include <QListWidget>

#include <openfluid/dllexport.hpp>

namespace Ui {
class WaresSrcImportDialog;
}

namespace openfluid { namespace waresdev {
class WaresDevImportPackage;
} }

namespace openfluid { namespace ui { namespace waresdev {

class OPENFLUID_API WaresSrcImportDialog: public QDialog
{
  Q_OBJECT

  private:

    Ui::WaresSrcImportDialog* ui;

    QMap<std::string, QListWidget*> m_ListWidgetsByWareTypeName;

    openfluid::waresdev::WaresDevImportPackage* mp_ImportFilePkg = 0;

    openfluid::waresdev::WaresDevImportPackage* mp_ImportGitPkg = 0;

    void setMessage(const QString& Msg = "");

    void updatePackageInfo(openfluid::waresdev::WaresDevImportPackage* ImportPackage);

    void updateWaresList(openfluid::waresdev::WaresDevImportPackage* ImportPackage);

    QStringList getSelectedWares();

  private slots :

    bool check();

    void onPackageSourceChoosen(bool Checked);

    void onGitSourceChoosen(bool Checked);

    void onPackagePathButtonClicked();

    void importPackage();

  public:

    WaresSrcImportDialog(QWidget* Parent);
};

} } } //namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESSRCIMPORTDIALOG_HPP__ */
