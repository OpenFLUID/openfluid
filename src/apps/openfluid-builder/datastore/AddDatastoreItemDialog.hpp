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
  @file AddDatastoreItemDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_ADDDATASTOREITEMDIALOG_HPP__
#define __OPENFLUID_BUILDERAPP_ADDDATASTOREITEMDIALOG_HPP__

#include <openfluid/ui/common/OpenFLUIDDialog.hpp>
#include <openfluid/core/UnstructuredValue.hpp>

#include <QCompleter>


namespace Ui
{
  class EditDatastoreItemDialog;
}


class AddDatastoreItemDialog : public openfluid::ui::common::OpenFLUIDDialog
{
  Q_OBJECT;

  private slots:

    void checkGlobal();

    void selectVectorFile();

    void selectRasterFile();


  private:

    Ui::EditDatastoreItemDialog* ui;

    const QStringList m_ExistingIDs;

    QString m_GDALFormatsStr;

    QString m_OGRFormatsStr;

    QCompleter* mp_Completer;

    void setMessage(const QString& Msg = "");


  public:

    AddDatastoreItemDialog(const QStringList& ExistingUnitsClass,const QStringList& ExistingIDs,
                           QWidget* Parent = 0);

    ~AddDatastoreItemDialog();

    QString getItemID() const;

    bool isUnitsClass() const;

    QString getUnitsClass() const;

    openfluid::core::UnstructuredValue::UnstructuredType getItemType() const;

    QString getSourceFilePath() const;

    bool isSourceFileInInputDataset() const;

    QString getCopySubdir() const;

};


#endif /* __OPENFLUID_BUILDERAPP_ADDDATASTOREITEMDIALOG_HPP__ */
