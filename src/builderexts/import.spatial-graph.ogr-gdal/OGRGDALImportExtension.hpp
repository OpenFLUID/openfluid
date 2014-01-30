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
  \file OGRGDALImportExtension.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OGRGDALIMPORTEXTENSION_HPP__
#define __OGRGDALIMPORTEXTENSION_HPP__


#include <openfluid/builderext/PluggableModalExtension.hpp>
#include "PrecheckImportDialog.hpp"

DECLARE_BUILDEREXT_PLUGIN


// =====================================================================
// =====================================================================


namespace Ui
{
  class OGRGDALDialog;
}

#include "SourceInfos.hpp"

#include <QAbstractButton>
#include <QListWidgetItem>
#include <QCloseEvent>


class OGRGDALImportExtension : public openfluid::builderext::PluggableModalExtension
{
  Q_OBJECT;

  private slots:

    void addFileSource();

    void addWFSSource();

    void removeSource();

    void processButtonBoxClicked(QAbstractButton* Button);

    void updateUI();

    void runPrecheck();

    void updateUnitsClassInfos();

    void updateUnitsIDsInfos();

    void updateUnitsPcsOrdInfos();

    void updateUnitsToConnInfos();

    void updateUnitsChildofConnInfos();

    void updateImportedFieldsInfos(QListWidgetItem* Item);

    void updateIsAreaComputeInfos();

    void updateAreaComputeAttrInfos();

    void updateIsLengthComputeInfos();

    void updateLengthComputeAttrInfos();

    void updateIsXCentroidComputeInfos();

    void updateXCentroidComputeAttrInfos();

    void updateIsYCentroidComputeInfos();

    void updateYCentroidComputeAttrInfos();

    void updateIsZCentroidComputeInfos();

    void updateZCentroidComputeAttrInfos();


  private:

    Ui::OGRGDALDialog* ui;

    QString m_InputDir;

    QString m_TempDir;

    int m_CurrentSrcIndex;

    SourcesInfosList_t m_SourcesInfos;

    PrecheckImportDialog* mp_PrecheckImportDlg;

    void addSource(const SourceInfos& SrcInfos);

    void proceedToImport();

    void cancelImport();

    void closeEvent(QCloseEvent *Event);


  public slots:

    void update(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags);


  public:

    OGRGDALImportExtension();

    ~OGRGDALImportExtension();

    bool initialize();

};


#endif /* __OGRGDALIMPORTEXTENSION_HPP__ */
