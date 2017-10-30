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
  @file EditMarketplaceDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QPushButton>

#include <openfluid/ui/config.hpp>
#include <openfluid/ui/common/EditMarketplaceDialog.hpp>

#include "ui_EditMarketplaceDialog.h"


namespace openfluid { namespace ui { namespace common {


EditMarketplaceDialog::EditMarketplaceDialog(QWidget* Parent,
                                             const QString& Name, const QString& URL,
                                             const openfluid::base::PreferencesManager::MarketPlaces_t& MPlaces):
  OpenFLUIDDialog(Parent), ui(new Ui::EditMarketplaceDialog),
  m_IsEditMode(!Name.isEmpty() && !URL.isEmpty()), m_OriginalName(Name),
  m_MPlaces(MPlaces)
{
  ui->setupUi(this);

  ui->NameEdit->setText(Name);
  ui->URLEdit->setText(URL);

  // "required" placeholder
  QString PlaceholderStr = getPlaceholderRequired();
  ui->NameEdit->setPlaceholderText(PlaceholderStr);
  ui->URLEdit->setPlaceholderText(PlaceholderStr);

  connect(ui->NameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobally()));
  connect(ui->URLEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobally()));

  checkGlobally();

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


EditMarketplaceDialog::~EditMarketplaceDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void EditMarketplaceDialog::setMessage(const QString& Msg)
{
  if (Msg.isEmpty())
  {
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));
    if (m_IsEditMode)
      ui->MessageLabel->setText(tr("Edit a marketplace"));
    else
      ui->MessageLabel->setText(tr("Add a marketplace"));
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->MessageFrame->setStyleSheet(QString("background-color: %1;")
                                    .arg(openfluid::ui::config::DIALOGBANNER_WARNBGCOLOR));
    ui->MessageLabel->setText(Msg);
  }
}


// =====================================================================
// =====================================================================


void EditMarketplaceDialog::checkGlobally()
{

  if (ui->NameEdit->text().isEmpty())
  {
    setMessage(tr("Name of the marketplace cannot be empty"));
  }
  else if (!m_IsEditMode && m_MPlaces.find(ui->NameEdit->text()) != m_MPlaces.end())
  {
    setMessage(tr("Name of the marketplace already exists"));
  }
  else if (m_IsEditMode &&
           m_OriginalName != ui->NameEdit->text() &&
           m_MPlaces.find(ui->NameEdit->text()) != m_MPlaces.end())
  {
    setMessage(tr("Name of the marketplace already exists"));
  }
  else if (ui->URLEdit->text().isEmpty())
  {
    setMessage(tr("URL of the marketplace cannot be empty"));
  }
  else
    setMessage();
}


// =====================================================================
// =====================================================================


QString EditMarketplaceDialog::getURL() const
{
  return ui->URLEdit->text();
}


// =====================================================================
// =====================================================================


QString EditMarketplaceDialog::getName() const
{
  return ui->NameEdit->text();
}


} } } // namespaces

