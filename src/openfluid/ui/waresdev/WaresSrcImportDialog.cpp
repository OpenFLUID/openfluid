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
 @file WaresSrcImportDialog.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <openfluid/ui/waresdev/WaresSrcImportDialog.hpp>

#include "ui_WaresSrcImportDialog.h"

#include <QFileDialog>


namespace openfluid { namespace ui { namespace waresdev {

// =====================================================================
// =====================================================================


WaresSrcImportDialog::WaresSrcImportDialog(QWidget* Parent) :
    QDialog(Parent), ui(new Ui::WaresSrcImportDialog)
{
  ui->setupUi(this);

  ui->WareshubWidget->setVisible(false);

  onPackageSourceChoosen(true);

  connect(ui->PackageRadioButton, SIGNAL(toggled(bool)), this, SLOT(onPackageSourceChoosen(bool)));
  connect(ui->WareshubRadioButton, SIGNAL(toggled(bool)), this, SLOT(onGitSourceChoosen(bool)));

  connect(ui->PackagePathButton, SIGNAL(clicked()), this, SLOT(onPackagePathButtonClicked()));
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onPackageSourceChoosen(bool Checked)
{
  if (!Checked)
    return;

  ui->WaresGroupBox->setTitle("Available wares in package");
}


// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onGitSourceChoosen(bool Checked)
{
  if (!Checked)
    return;

  ui->WaresGroupBox->setTitle("Available wares on wareshub site");
}

// =====================================================================
// =====================================================================


void WaresSrcImportDialog::onPackagePathButtonClicked()
{
  QString PackageFilePath = QFileDialog::getOpenFileName(this, tr("Choose the package to open"),
                                                         QDir::homePath(),
                                                         tr("OpenFLUID wares packages (*.ofwdp)"));

  if (PackageFilePath.isEmpty())
    return;

  ui->PackagePathLineEdit->setText(PackageFilePath);
}


// =====================================================================
// =====================================================================


} } } //namespaces
