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
  \file SpatialDomainWidget.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __SPATIALDOMAINWIDGET_HPP__
#define __SPATIALDOMAINWIDGET_HPP__


#include <QWidget>

#include "WorkspaceWidget.hpp"
#include "MapScene.hpp"

#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>
#include <openfluid/fluidx/AdvancedDatastoreDescriptor.hpp>


namespace Ui
{
  class SpatialDomainWidget;
}


class SpatialDomainWidget : public WorkspaceWidget
{
  Q_OBJECT

  private slots:

    void setSelectedClass(QString ClassName);

    void updateUnitSelection(int Row);

    void addUnitsClass();

    void moveUnitsClassUp(QString ClassName);

    void moveUnitsClassDown(QString ClassName);

    void removeUnitsClass(QString ClassName);

    void addUnit();

    void removeUnit();

    void addConnection();

    void removeConnection();

    void addAttribute();

    void editAttributesValues();

    void removeAttribute();

    void renameAttribute();

    void addEvent();

    void editEvent();

    void removeEvents();

    void refreshMap();

    void enableAutomaticView(bool Enabled);

    void updateSelectionFromMap();

    void updateFluidXAttributeFromCellValue(int Row, int Column);

    void updateFluidXProcessOrder(int PcsOrd);


  private:

    Ui::SpatialDomainWidget* ui;

    openfluid::fluidx::AdvancedDomainDescriptor& m_Domain;

    openfluid::fluidx::AdvancedDatastoreDescriptor& m_Datastore;

    QString m_ActiveClass;

    MapScene* mp_MapScene;

    void setActiveClass(const QString& ClassName);

    void refreshClassStructure();

    void refreshClassData();

    void refreshClassAttributes();

    void refreshClassEvents();

    void updateUpDownButtons();

    int getClassIndex(const QString& ClassName);

    QStringList getClassesOrderedStringList();


  public slots:

    void refresh();


  public:

    SpatialDomainWidget(QWidget* Parent,openfluid::fluidx::AdvancedFluidXDescriptor& AFXDesc);

    virtual ~SpatialDomainWidget();
};


#endif /* __SPATIALDOMAINWIDGET_HPP__ */
