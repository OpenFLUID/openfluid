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
  @file SourceAddDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __SOURCEADDDIALOG_HPP__
#define __SOURCEADDDIALOG_HPP__

namespace Ui
{
  class SourceAddDialog;
}

#include "SourceInfos.hpp"

#include <QDialog>


class SourceAddDialog : public QDialog
{
  Q_OBJECT;

  private:

    bool checkRequiredFields();


  private slots:

    void proceedToImport();

    void handleSourceLinked(void* Src);

    void handleLayerCounted(int Count);

    void handleLayerFetched(int Index, QString Name, QString GeomStr);

    void handleSourceFinished();

    void handleSourceError(QString Message);


  protected slots:

    void globalCheck();


  protected:

    Ui::SourceAddDialog* ui;

    OGRDataSource* mp_DataSource;

    QString m_CurrentSourceURI;

    QString m_CurrentSourceDisplay;

    SourceInfos m_SrcInfos;

    void openDataSource();

    virtual void updateAfterOpen() = 0;

    virtual bool prepareToImport() = 0;


  public:

    SourceAddDialog(QWidget* Parent = NULL);

    ~SourceAddDialog();

    const SourceInfos& sourceInfos()
    { return m_SrcInfos; }
};


#endif /* __SOURCEADDDIALOG_HPP__ */
