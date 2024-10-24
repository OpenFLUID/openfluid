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
  @file SignatureDataEditorWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#ifndef __OPENFLUID_UICOMMON_SIGNATUREDATAEDITORWIDGET_HPP__
#define __OPENFLUID_UICOMMON_SIGNATUREDATAEDITORWIDGET_HPP__


#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QList>
#include <QSet>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/common/SignatureDataEditDefs.hpp>


namespace Ui
{
  class SignatureDataEditorWidget;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API SignatureDataEditorWidget : public QWidget
{
  Q_OBJECT;

  public:

     enum class DataColumns { DATAID, DATATYPE,
                        UNITSCLASS, SIUNIT,
                        ROCONDITION, RWCONDITION, RWUCONDITION,
                        DESCRIPTION
                      };

     enum class DataConditionsIndices { USED = 0, REQUIRED = 1,
                                  PRODUCED = 2, UPDATED = 3,
                                  UNKNOWN = 99};

    bool areAllCellsValid(const QString& Header, const DataTableType& HandledDataType);

    bool areAllCellsEmpty(const QString& Header);

  private slots:

    void removeDataLine();

    void addDataLine();

    void onItemChanged(QTableWidgetItem*);


  signals:

    void dataTableChanged();


  private:

    Ui::SignatureDataEditorWidget* ui;

    QList<DataColumns> m_Columns;

    QMap<QString, int> m_ColHeadersValuesMap;
    QSet<QString> m_AvailableHeaders;

    QString m_DataIDHeader;
    QString m_UnitsClassHeader;
    QString m_IOConditionHeader;
    QString m_TypeHeader;
    QString m_DescriptionHeader;
    QString m_SIUnitHeader;

  public:

    explicit SignatureDataEditorWidget(QWidget* Parent = nullptr);

    virtual ~SignatureDataEditorWidget();

    void initialize(const QList<DataColumns>& Columns);

    const QTableWidget* dataTableWidget() const;

    void addDataLine(const QMap<int,QVariant>& DataLine);

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_SIGNATUREDATAEDITORWIDGET_HPP__ */
