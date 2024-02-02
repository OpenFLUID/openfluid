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
  @file EditClassIDVarDialog.hpp

  @author Armel THONI <armel.thoni@inra.fr>
*/

#ifndef __EDITTRIPLETDIALOG_H__
#define __EDITTRIPLETDIALOG_H__


#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>

#include "MultiCSVObsTools.hpp"


namespace Ui
{
  class EditClassIDVarDialog;
}


class EditClassIDVarDialog : public openfluid::ui::common::MessageDialog
{
    Q_OBJECT;


  private slots:

    void checkGlobal();

  private:

    Ui::EditClassIDVarDialog *ui;
  
  public:
      
    EditClassIDVarDialog(const QStringList& ClassNames, QWidget* Parent = nullptr);
    
    ~EditClassIDVarDialog();
    
    void initialize(const QString& UnitsClasses, const QString& UnitsIDs, const QString& Variables, 
                    const bool HasPrecision, const std::size_t Precision);
      
    void update();

    std::vector<openfluid::tools::ClassIDVarPrecision> getClassIDVarSettings();
  
};

#endif // __EDITTRIPLETDIALOG_H__
