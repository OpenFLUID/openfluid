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
  @file AddParamDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_ADDPARAMDIALOG_HPP__
#define __OPENFLUID_BUILDERAPP_ADDPARAMDIALOG_HPP__


#include <openfluid/ui/common/OpenFLUIDDialog.hpp>

#include <QCompleter>
#include <QStringList>


namespace Ui
{
  class AddParamDialog;
}


class AddParamDialog : public openfluid::ui::common::OpenFLUIDDialog
{
  Q_OBJECT;

  private slots:

    void checkGlobal();


  private:

    Ui::AddParamDialog* ui;

    QCompleter* mp_Completer;

    const QStringList m_ExistingParams;

    const QStringList m_CompletingParams;

    void setMessage(const QString& Msg = "");


  public:

    AddParamDialog(const QStringList& ExistingParams,const QStringList& CompletingParams,
                   QWidget* Parent = nullptr);

    ~AddParamDialog();

    QString getParamName() const;

    QString getParamValue() const;


};



#endif /* __OPENFLUID_BUILDERAPP_ADDPARAMDIALOG_HPP__ */
