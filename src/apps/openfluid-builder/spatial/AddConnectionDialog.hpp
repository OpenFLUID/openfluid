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
  @file AddConnectionDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_BUILDERAPP_ADDCONNECTIONDIALOG_HPP__
#define __OPENFLUID_BUILDERAPP_ADDCONNECTIONDIALOG_HPP__



enum ConnectionCode { BUILDER_CONNCODE_TO = 0,
                      BUILDER_CONNCODE_FROM = 1,
                      BUILDER_CONNCODE_PARENTOF = 2,
                      BUILDER_CONNCODE_CHILDOF = 3 };


namespace Ui
{
  class AddConnectionDialog;
}


#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>


class AddConnectionDialog : public openfluid::ui::common::MessageDialog
{
  Q_OBJECT;

  private slots:

    void checkGlobal();

    void updateDestIDs();


  private:

    Ui::AddConnectionDialog* ui;

    QMap<QString,QStringList> m_ClassID;

    const openfluid::fluidx::AdvancedDomainDescriptor* mp_Domain;


  public:

    AddConnectionDialog(const QString& SrcClass, const QString& SrcID,
                        const openfluid::fluidx::AdvancedDomainDescriptor* Domain,
                        QWidget* Parent = nullptr);

    ~AddConnectionDialog();

    ConnectionCode getConnectionCode() const;

    QString getDestinationClass() const;

    QString getDestinationID() const;
};


#endif /* __OPENFLUID_BUILDERAPP_ADDCONNECTIONDIALOG_HPP__ */
