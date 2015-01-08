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
 \file RestrictedFileDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */


#include <openfluid/ui/waresdev/RestrictedFileDialog.hpp>

#include <QFileSystemModel>
#include <QUrl>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


RestrictedFileDialog::RestrictedFileDialog(QWidget* Parent,
                                           const QString& CurrentDirectory,
                                           const QString& TopDirectory) :
    QFileDialog(Parent, QString(), CurrentDirectory), m_TopDirectory(
        TopDirectory), mp_ProxyModel(new FileFilterProxyModel(TopDirectory))
{
  setOption(QFileDialog::HideNameFilterDetails);

  setViewMode(QFileDialog::List);

  // proxyModel should be useless if lookInCombo, history and sidebarUrls are correctly set and updated, but it's a security
  setProxyModel(mp_ProxyModel);

  setHistory(QStringList());

  QList<QUrl> Urls;
  Urls << QUrl::fromLocalFile(m_TopDirectory);
  setSidebarUrls(Urls);

  mp_LookInCombo = qobject_cast<QComboBox*>(
      findChild<QWidget *>("lookInCombo"));

  mp_ToParentButton = findChild<QWidget *>("toParentButton");

  if (mp_LookInCombo)
  {
    connect(mp_LookInCombo, SIGNAL(currentIndexChanged(int)), this,
            SLOT(onComboIndexChanged(int)));

    onComboIndexChanged(mp_LookInCombo->currentIndex());
  }
}


// =====================================================================
// =====================================================================


RestrictedFileDialog::~RestrictedFileDialog()
{
//  "The file dialog will take ownership of the proxyModel."
//  delete mp_ProxyModel;
}


// =====================================================================
// =====================================================================


void RestrictedFileDialog::onComboIndexChanged(int Index)
{
  if (Index < 0)
    return;

  // must be done here and not on directoryEntered() signal because it seems not sent when using prev/next buttons !
  if (mp_ToParentButton)
    mp_ToParentButton->setEnabled(directory().path() != m_TopDirectory);

  if (mp_LookInCombo)
  {
    int n = mp_LookInCombo->findText(m_TopDirectory);

    if (n > -1)
    {
      int t = mp_LookInCombo->count();

      for (int i = t - 1; i > n; i--)
        mp_LookInCombo->removeItem(i);
    }
  }

}


// =====================================================================
// =====================================================================


QString RestrictedFileDialog::getSaveFileName(QWidget* Parent,
                                              const QString& CurrentPath,
                                              const QString& TopDirectory)
{
  RestrictedFileDialog Dialog(Parent, CurrentPath, TopDirectory);

  Dialog.setAcceptMode(QFileDialog::AcceptSave);

  if (Dialog.exec())
    return Dialog.selectedFiles().value(0, "");

  return "";
}


// =====================================================================
// =====================================================================


QString RestrictedFileDialog::getOpenFileName(QWidget* Parent,
                                              const QString& CurrentPath,
                                              const QString& TopDirectory)
{
  RestrictedFileDialog Dialog(Parent, CurrentPath, TopDirectory);

  Dialog.setAcceptMode(QFileDialog::AcceptOpen);
  Dialog.setFileMode(QFileDialog::ExistingFile);

  if (Dialog.exec())
    return Dialog.selectedFiles().value(0, "");

  return "";
}



// =====================================================================
// =====================================================================


// TODO add a filter on file type ?
QStringList RestrictedFileDialog::getOpenFileNames(QWidget* Parent,
                                              const QString& CurrentPath,
                                              const QString& TopDirectory)
{
  RestrictedFileDialog Dialog(Parent, CurrentPath, TopDirectory);

  Dialog.setAcceptMode(QFileDialog::AcceptOpen);
  Dialog.setFileMode(QFileDialog::ExistingFiles);

  if (Dialog.exec())
    return Dialog.selectedFiles();

  return QStringList();
}


// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


FileFilterProxyModel::FileFilterProxyModel(const QString& TopDirectory) :
    m_TopDirectory(TopDirectory)
{
}


// =====================================================================
// =====================================================================


bool FileFilterProxyModel::filterAcceptsRow(
    int SourceRow, const QModelIndex& SourceParent) const
{
  QModelIndex index0 = sourceModel()->index(SourceRow, 0, SourceParent);

  QFileSystemModel* FileModel = qobject_cast<QFileSystemModel*>(sourceModel());

  if (FileModel)
    return FileModel->filePath(index0).indexOf(m_TopDirectory) >= 0;

  return false;
}


// =====================================================================
// =====================================================================


} } } // namespaces
