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
  @file SignatureWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_SIGNATUREWIDGET_HPP__
#define __OPENFLUID_BUILDERAPP_SIGNATUREWIDGET_HPP__


#include <QWidget>

#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ObserverInstance.hpp>


namespace Ui
{
  class SignatureWidget;
}


class SignatureWidget : public QWidget
{
  Q_OBJECT;

  private:

    Ui::SignatureWidget* ui;

    static QString getGeneralInfoLine(const QString& Title, const QString& Text, bool WithPreBR = true);

    template<typename SignatureType>
    QString getCommonForGeneral(const openfluid::machine::WareContainer<SignatureType>& Container);

    void updateGeneral(const openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature>& Container);

    void updateGeneral(const openfluid::machine::WareContainer<openfluid::ware::ObserverSignature>& Container);

    void updateParametersCategory(const std::vector<openfluid::ware::SignatureDataItem>* Infos,
                                  const QString& CatStr, unsigned int BaseIndex);

    void updateParameters(const openfluid::ware::DataWareSignature* Signature);

    void updateExtrafilesCategory(const std::vector<std::string>* Infos,
                                  const QString& CatStr, unsigned int BaseIndex);

    void updateExtrafiles(const openfluid::ware::DataWareSignature* Signature);

    void updateVariablesCategory(const std::vector<openfluid::ware::SignatureSpatialDataItem>* Infos,
                                 const QString& CatStr,
                                 unsigned int BaseIndex);

    void updateReadVariables(const openfluid::ware::DataWareSignature* Signature);

    void updateVariables(const openfluid::ware::SimulatorSignature* Signature);

    void updateAttributesCategory(const std::vector<openfluid::ware::SignatureSpatialDataItem>* Infos,
                                  const QString& CatStr,
                                  unsigned int BaseIndex);

    
    void updateReadAttributes(const openfluid::ware::DataWareSignature* Signature);

    void updateAttributes(const openfluid::ware::SimulatorSignature* Signature);

    void updateEvents(const openfluid::ware::SimulatorSignature* Signature);

    void updateSpatialGraph(const openfluid::ware::SimulatorSignature* Signature);

    QString formatAuthors(const openfluid::ware::WareSignature::PeopleList_t& AuthList);

    void clearAllInfos();

    static QString convertStdString(const std::string& Str, const QString& ReplaceStr = "<i>"+tr("unknown")+"</i>");


  public:

    SignatureWidget(QWidget* Parent);

    ~SignatureWidget();

    void mute();

    void update(const openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature>& Container);

    void update(const openfluid::machine::WareContainer<openfluid::ware::ObserverSignature>& Container);
};


#endif /* __OPENFLUID_BUILDERAPP_SIGNATUREWIDGET_HPP__ */
