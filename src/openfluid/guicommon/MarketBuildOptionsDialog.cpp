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
  \file MarketBuildOptionsDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/guicommon/MarketBuildOptionsDialog.hpp>
#include <openfluid/market/MarketPackage.hpp>
#include <openfluid/tools/SwissTools.hpp>

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>


namespace openfluid { namespace guicommon {

// =====================================================================
// =====================================================================


MarketBuildOptionsDialog::MarketBuildOptionsDialog(const QString& CommonBuildOptions, const QString& BuildOptions, const QString& SimID)
: QDialog(), m_CommonBuildOptions(CommonBuildOptions),m_BuildOptions(BuildOptions),m_SimID(SimID)
{

  setMinimumSize(450,0);

  QLabel *InfoLabel = new QLabel();
  InfoLabel->setText("<i>"+tr("These options control the builds of source packages.<br/>Changing this is at your own risk.")+"</i>");
  InfoLabel->setAlignment(Qt::AlignHCenter);

  QLabel *CommonOptsLabel = new QLabel();

  if (!SimID.isEmpty())
  {
    QString Options = QString::fromStdString(openfluid::tools::ReplaceEmptyString(CommonBuildOptions.toStdString(),tr("<i>none</i>").toStdString()));
    CommonOptsLabel->setText("<u>"+tr("Common source build options:")+"</u><br/>" + Options);
  }
  else
    CommonOptsLabel->setText("");



  QLabel *EditLabel = new QLabel();
  if (!SimID.isEmpty())
  {
    EditLabel->setText(tr("Specific build options for %1:").arg(SimID));
  }
  else
  {
    EditLabel->setText(tr("Common source build options:"));
  }


  if (SimID.isEmpty()) m_OptionsEntry.setText(CommonBuildOptions);
  else m_OptionsEntry.setText(BuildOptions);


  QDialogButtonBox *ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                      | QDialogButtonBox::Cancel);

  connect(ButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));


  QVBoxLayout *MainLayout = new QVBoxLayout();

  MainLayout->addWidget(InfoLabel);
  MainLayout->addSpacing(10);
  MainLayout->addWidget(CommonOptsLabel);
  MainLayout->addSpacing(10);
  MainLayout->addWidget(EditLabel);
  MainLayout->addWidget(&m_OptionsEntry);
  MainLayout->addWidget(ButtonBox);


  if(m_SimID.isEmpty())
    setWindowTitle(tr("Common build options for all source packages"));
  else
    setWindowTitle(tr("Build options for %1").arg(m_SimID));

  setLayout(MainLayout);
}


// =====================================================================
// =====================================================================

} } //namespaces

