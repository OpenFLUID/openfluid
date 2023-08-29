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
  @file EditSignatureDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UICOMMON_EDITSIGNATUREDIALOG_HPP__
#define __OPENFLUID_UICOMMON_EDITSIGNATUREDIALOG_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>


namespace Ui
{
  class EditSignatureDialog;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API EditSignatureDialog : public MessageDialog
{
  Q_OBJECT;


  private:

    QStringList m_ExistingIDs;


  protected slots:

    void checkGlobally();


  private:

    Ui::EditSignatureDialog* ui;

    void initialize(const QStringList& ExistingIDs = QStringList());


  public:

    explicit EditSignatureDialog(QWidget* Parent = nullptr);

    virtual ~EditSignatureDialog();

    void initialize(const openfluid::ware::SimulatorSignature& Signature,
                        const QStringList& ExistingIDs = QStringList()); // used for ghosts

    void initialize(const QString& Path,
                    const QStringList& ExistingIDs = QStringList());

    openfluid::ware::SimulatorSignature getSignature() const;

    bool exportSignature(QString Path) const;
};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_EDITSIGNATUREDIALOG_HPP__ */
