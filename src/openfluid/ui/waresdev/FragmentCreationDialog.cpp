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
  @file FragmentCreationDialog.cpp

  @author Armel THÖNI <armel.thoni@inra.fr>
*/


#include <openfluid/config.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/ui/waresdev/FragmentCreationDialog.hpp>
#include <openfluid/ui/waresdev/AbstractSrcImportDialog.hpp>

#include "ui_FragmentCreationDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


FragmentCreationDialog::FragmentCreationDialog(QWidget* Parent, QString WarePath) :
    openfluid::ui::common::MessageDialog(Parent), ui(new Ui::FragmentCreationDialog),  m_WarePath(WarePath)
{
  ui->setupUi(this);

  setupMessageUi(tr("Create a fragment"));

  connect(ui->FragmentNameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(check()));

  ui->FragmentNameLineEdit->setToolTip(tr("Accepts only letters, digits, dashes ('-'), "
                                          "underscores ('_') and dots ('.'). Must begin by a letter."));
  
  connect(buttonBox(), SIGNAL(accepted()), this, SLOT(createFragment()));

  check();
}


// =====================================================================
// =====================================================================


QRegExp FragmentCreationDialog::getFragmentNamedRegExp()
{
  return QRegExp(QString::fromStdString(openfluid::tools::FragmentNameRuleString));
}


// =====================================================================
// =====================================================================


bool FragmentCreationDialog::check()
{
  if (ui->FragmentNameLineEdit->text().isEmpty())
  {
    setMessage(tr("Fragment name required"));
    return false;
  }
  else 
  {
    QRegExpValidator Validator(getFragmentNamedRegExp(), 0);
    QString FragmentName(ui->FragmentNameLineEdit->text());
    QString FirstLetter(FragmentName.front());
    int pos = 0;
    if (Validator.validate(FirstLetter, pos) != QValidator::Acceptable)
    {
      setMessage(tr("Must begin by a letter (without accent)."));
      return false;
    }
    if (Validator.validate(FragmentName, pos) != QValidator::Acceptable)
    {
      setMessage(tr("Accepts letters, digits, dashes, underscores and dots."));
      return false;
    }
  }
    setMessage();
    return true;
}


// =====================================================================
// =====================================================================


void FragmentCreationDialog::createFragment()
{
  std::string FragmentID = ui->FragmentNameLineEdit->text().toStdString();
  
  // Create directory
  openfluid::tools::FilesystemPath SrcFragmentsSubPath({openfluid::config::WARESDEV_SRC_DIR, 
                                                        openfluid::config::WARESDEV_FRAGMENTS_DIR});
  openfluid::tools::FilesystemPath FragmentsPath({m_WarePath.toStdString(), 
                                                  SrcFragmentsSubPath.toNative(), 
                                                  FragmentID});
  FragmentsPath.makeDirectory();
  
  // Create file if wanted
  if (ui->createFileCheckBox->isChecked())
  {
    FragmentsPath.makeFile(FragmentID+".hpp");
  }
}


} } } //namespaces
