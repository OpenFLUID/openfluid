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
  \file AddGeneratorDialog.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __ADDGENERATORDIALOG_HPP__
#define __ADDGENERATORDIALOG_HPP__


#include <openfluid/fluidx/GeneratorDescriptor.hpp>

#include "OpenFLUIDDialog.hpp"

#include <QStringList>


namespace Ui
{
  class AddGeneratorDialog;
}


class AddGeneratorDialog : public OpenFLUIDDialog
{
  Q_OBJECT;

  private slots:

    void switchGeneratorOptions();

    void checkGlobal();

    void selectSourcesFile();

    void selectDistriFile();


  private:

    Ui::AddGeneratorDialog* ui;

    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod m_Method;

    void setMessage(const QString& Msg = "");


  public:

    AddGeneratorDialog(const QStringList& UnitsClasses, QWidget* Parent = 0);

    ~AddGeneratorDialog();

    openfluid::fluidx::GeneratorDescriptor::GeneratorMethod getMethod() const
    { return m_Method; }

    QString getVariableName() const;

    QString getUnitClass() const;

    unsigned int getVariableSize() const;

    openfluid::ware::WareParams_t getParams() const;

};




#endif /* __ADDGENERATORDIALOG_HPP__ */
