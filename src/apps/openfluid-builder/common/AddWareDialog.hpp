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
  @file AddWareDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_ADDWAREDIALOG_HPP__
#define __OPENFLUID_BUILDERAPP_ADDWAREDIALOG_HPP__


#include <openfluid/ui/common/OpenFLUIDDialog.hpp>


namespace Ui
{
  class AddWareDialog;
}


class SignatureWidget;


class AddWareDialog : public openfluid::ui::common::OpenFLUIDDialog
{
  Q_OBJECT;

  protected slots:

    /**
      Labelizes items in wares list using IDs or Names. If the name is not available, the ID is used as a fallback.
      A QStringList is associated to each item, with the ID at index 0 and the name at index 1
      @param[in] NameFirst if true, the name is preferably used as the ware label
    */
    void labelizeItems(bool NameFirst);


  protected:

    Ui::AddWareDialog* ui;


  public:

    AddWareDialog(QWidget* Parent = nullptr);

    ~AddWareDialog();

    QString getSelectedID() const;

};


#endif /* __OPENFLUID_BUILDERAPP_ADDWAREDIALOG_HPP__ */
