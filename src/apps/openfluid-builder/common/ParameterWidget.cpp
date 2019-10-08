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
  @file ParameterWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QMessageBox>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/ui/common/ShortcutCompleter.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_ParameterWidget.h"
#include "ParameterWidget.hpp"
#include "ProjectCentral.hpp"


ParameterWidget::ParameterWidget(QWidget* Parent,
                                 const QString& Name, const QString& Value,
                                 const QString& SIUnit,
                                 bool Required,
                                 bool Removable):
  QWidget(Parent),ui(new Ui::ParameterWidget)
{
  ui->setupUi(this);

  ui->NameLabel->setText(Name);
  ui->ValueEdit->setText(Value);
  ui->SIUnitLabel->setText(SIUnit);
  ui->GlobalValueLabel->setText("");

  if (Required)
  {
    ui->ValueEdit->setPlaceholderText(QApplication::translate("openfluid::ui::config",
                                                              openfluid::ui::config::PLACEHOLDER_REQUIRED));
  }


  // Completer
  openfluid::ui::common::ShortcutCompleter* Completer =
      new openfluid::ui::common::ShortcutCompleter(ProjectCentral::instance()->allNamesListModel(),this);
  Completer->linkToLineEdit(ui->ValueEdit);
  // TODO FIXME notifyValueChanged() not activated when hitting escape key!


  connect(Completer,SIGNAL(activated(const QString&)),this,SLOT(notifyValueChanged()));
  connect(ui->ValueEdit,SIGNAL(textEdited(const QString&)),this,SLOT(notifyValueChanged()));


  if (Removable)
  {
    ui->RemoveButton->setText("");
    ui->RemoveButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
    ui->RemoveButton->setIconSize(QSize(16,16));

    connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(notifyRemoveClicked()));
  }

  ui->RemoveButton->setVisible(Removable);
}


// =====================================================================
// =====================================================================


ParameterWidget::~ParameterWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void ParameterWidget::notifyValueChanged()
{
  emit valueChanged(ui->NameLabel->text(),ui->ValueEdit->text());
}


// =====================================================================
// =====================================================================


void ParameterWidget::notifyRemoveClicked()
{
  bool OK = true;

  if (openfluid::base::PreferencesManager::instance()->isBuilderParamRemovalConfirm())
  {
    OK = (QMessageBox::question(QApplication::activeWindow(),
                                "OpenFLUID-Builder",
                                tr("You are removing the %1 parameter.\n"
                                   "Its value will be lost.\nProceed anyway?").arg(getName()),
                                QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
  }

  if (OK)
  {
    emit removeClicked(ui->NameLabel->text());
  }
}


// =====================================================================
// =====================================================================


QString ParameterWidget::getName()
{
  return ui->NameLabel->text();
}
