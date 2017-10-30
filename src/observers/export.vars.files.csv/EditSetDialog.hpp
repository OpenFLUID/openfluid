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
  @file EditSetDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/



#ifndef __EDITSETDIALOG_HPP__
#define __EDITSETDIALOG_HPP__


#include <QStringList>

#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ui/common/OpenFLUIDDialog.hpp>


namespace Ui
{
  class EditSetDialog;
}


class EditSetDialog : public openfluid::ui::common::OpenFLUIDDialog
{
  Q_OBJECT;


  private slots:

    void checkGlobal();


  private:

    Ui::EditSetDialog* ui;

    QStringList m_ExistingSetsNames;


  public:

    EditSetDialog(const QStringList& SetNames, const QStringList& FormatNames, const QStringList& ClassNames,
                 QWidget* Parent = nullptr);

    ~EditSetDialog();

    void initialize(const QString& Name, const QString& Format,
                    const QString& UnitsClass, const QString& UnitsIDs,
                    const QString& Vars);

    openfluid::ware::WareParams_t getSetParams();

};


#endif /* __EDITSETDIALOG_HPP__ */
