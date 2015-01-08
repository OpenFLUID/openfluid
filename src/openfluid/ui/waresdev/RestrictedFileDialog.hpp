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
 \file RestrictedFileDialog.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV__RESTRICTEDFILEDIALOG_HPP_
#define __OPENFLUID_UIWARESDEV__RESTRICTEDFILEDIALOG_HPP_

#include <openfluid/dllexport.hpp>

#include <QFileDialog>
#include <QSortFilterProxyModel>
#include <QComboBox>


namespace openfluid { namespace ui { namespace waresdev {

class FileFilterProxyModel;


// =====================================================================
// =====================================================================


class OPENFLUID_API RestrictedFileDialog: public QFileDialog
{
  Q_OBJECT

  private:

    QString m_TopDirectory;

    FileFilterProxyModel* mp_ProxyModel;

    QComboBox* mp_LookInCombo;

    QWidget* mp_ToParentButton;

    RestrictedFileDialog(QWidget* Parent, const QString& CurrentDirectory,
                         const QString& TopDirectory);

  public:

    static QString getSaveFileName(QWidget* Parent, const QString& CurrentPath,
                                   const QString& TopDirectory);

    static QString getOpenFileName(QWidget* Parent, const QString& CurrentPath,
                                   const QString& TopDirectory);

    static QStringList getOpenFileNames(QWidget* Parent, const QString& CurrentPath,
                                       const QString& TopDirectory);

    ~RestrictedFileDialog();

  private slots:

    void onComboIndexChanged(int Index);
};


// =====================================================================
// =====================================================================


class OPENFLUID_API FileFilterProxyModel: public QSortFilterProxyModel
{
  private:

    QString m_TopDirectory;

  protected:

    virtual bool filterAcceptsRow(int SourceRow,
                                  const QModelIndex& SourceParent) const;

  public:

    FileFilterProxyModel(const QString& TopDirectory);
};


// =====================================================================
// =====================================================================


} } } // namespaces

#endif /* __OPENFLUID_UIWARESDEV__RESTRICTEDFILEDIALOG_HPP_ */
