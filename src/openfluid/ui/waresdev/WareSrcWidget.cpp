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
 \file WareSrcWidget.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "WareSrcWidget.hpp"
#include "ui_WareSrcWidget.h"

#include <QList>

#include "WareSrcFileEditor.hpp"
#include "WareSrcToolbar.hpp"

namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcWidget::WareSrcWidget(
    const openfluid::waresdev::WareSrcManager::PathInfo& Info,
    bool IsStandalone, QWidget* Parent) :
    QWidget(Parent), ui(new Ui::WareSrcWidget), m_Container(Info.m_AbsolutePath,
                                                            Info.m_WareType,
                                                            Info.m_WareName)
{
  ui->setupUi(this);

  if (IsStandalone)
    ui->Toolbar_Layout->addWidget(new WareSrcToolbar(true, this));

  QList<int> Sizes;
  Sizes << 1000 << 180;
  ui->splitter->setSizes(Sizes);
}


// =====================================================================
// =====================================================================


WareSrcWidget::~WareSrcWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================

void WareSrcWidget::openFile(
    const openfluid::waresdev::WareSrcManager::PathInfo& Info)
{
  WareSrcFileEditor* Widget = m_WareSrcFilesByPath.value(
      Info.m_RelativePathToWareDir, 0);

  if (!Widget)
  {
    Widget = new WareSrcFileEditor(Info.m_AbsolutePath, this);

    int Pos = ui->WareSrcFileCollection->addTab(Widget, Info.m_FileName);
    ui->WareSrcFileCollection->setTabToolTip(Pos, Info.m_RelativePathToWareDir);

    m_WareSrcFilesByPath[Info.m_AbsolutePath] = Widget;
  }

  ui->WareSrcFileCollection->setCurrentWidget(Widget);
}


// =====================================================================
// =====================================================================


void WareSrcWidget::openDefaultFiles()
{
// TODO
}


// =====================================================================
// =====================================================================


} } }  // namespaces