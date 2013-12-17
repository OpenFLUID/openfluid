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
  \file SignatureWidget.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __SIGNATUREWIDGET_HPP__
#define __SIGNATUREWIDGET_HPP__


#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>

#include <QWidget>

namespace Ui
{
  class SignatureWidget;
}


class SignatureWidget : public QWidget
{
  private:

    Ui::SignatureWidget* ui;

    void updateGeneral(const openfluid::machine::ModelItemSignatureInstance* Signature);

    void updateParameters(const openfluid::machine::ModelItemSignatureInstance* Signature);

    void updateExtrafilesCategory(const std::vector<std::string>* Infos,
                                  const QString& CatStr, unsigned int BaseIndex);

    void updateExtrafiles(const openfluid::machine::ModelItemSignatureInstance* Signature);

    void updateVariablesCategory(const std::vector<openfluid::ware::SignatureHandledTypedDataItem>* Infos,
                                 const QString& CatStr,
                                 unsigned int BaseIndex);

    void updateVariables(const openfluid::machine::ModelItemSignatureInstance* Signature);

    void updateAttributesCategory(const std::vector<openfluid::ware::SignatureHandledDataItem>* Infos,
                                  const QString& CatStr,
                                  unsigned int BaseIndex);

    void updateAttributes(const openfluid::machine::ModelItemSignatureInstance* Signature);

    void updateEvents(const openfluid::machine::ModelItemSignatureInstance* Signature);

    void updateSpatialGraph(const openfluid::machine::ModelItemSignatureInstance* Signature);

    void updateGeneral(const openfluid::machine::ObserverSignatureInstance* Signature);

    QString formatAuthors(const openfluid::ware::WareSignature::AuthorsList_t& AuthList);

    void clearAllInfos();

    static QString convertStdString(const std::string& Str, const QString& ReplaceStr = "<i>unknown</i>");

  public:

    SignatureWidget(QWidget* Parent);

    ~SignatureWidget();

    void update(const openfluid::machine::ModelItemSignatureInstance* Signature);

    void update(const openfluid::machine::ObserverSignatureInstance* Signature);
};


#endif /* __SIGNATUREWIDGET_HPP__ */
