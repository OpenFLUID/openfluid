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
  @file SignatureEditorWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_UICOMMON_SIGNATUREEDITORWIDGET_HPP__
#define __OPENFLUID_UICOMMON_SIGNATUREEDITORWIDGET_HPP__


#include <QTabWidget>
#include <QTableWidget>

#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/ware/ObserverSignature.hpp>
#include <openfluid/ui/common/WareIssuesManagerWidget.hpp>
#include <openfluid/waresdev/WareSignatureSerializer.hpp>


namespace Ui
{
  class SignatureEditorWidget;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API SignatureEditorWidget : public QTabWidget
{
  Q_OBJECT;

  private slots:

    void notifyChanged();

    void addAuthorLine();

    void removeAuthorLine();

    void addContactLine();

    void removeContactLine();

  private:

    Ui::SignatureEditorWidget* ui;

    bool m_StaticID;

    openfluid::ui::common::WareIssuesManagerWidget* mp_IssuesManager;

    void initializeCommon(const openfluid::ware::WareSignature* Signature);

    void initializeID(const QString& ID);

    void initializeIssuesUIFromSignature(const openfluid::ware::WareSignature& Signature);

    void initializeParametersUIFromSignature(const openfluid::ware::DataWareSignature& Signature);

    void initializeExtrafilesUIFromSignature(const openfluid::ware::DataWareSignature& Signature);

    void initializeInputAttributesUIFromSignature(const openfluid::ware::DataWareSignature& Signature, bool IsRW=true);
    
    void initializeAttributesUIFromSignature(const openfluid::ware::SimulatorSignature& Signature, bool IsRW=true);

    void initializeEventsUIFromSignature(const openfluid::ware::SimulatorSignature& Signature);

    void initializeInputVariablesUIFromSignature(const openfluid::ware::DataWareSignature& Signature, bool IsRW=true);

    void initializeVariablesUIFromSignature(const openfluid::ware::SimulatorSignature& Signature, bool IsRW=true);

    void initializeDynamicsUIFromSignature(const openfluid::ware::SimulatorSignature& Signature);

    void initializeIntegrationUIFromSignature(const openfluid::builderext::BuilderExtensionSignature& Signature);

    void updateSignatureFromCommonsUI(openfluid::ware::WareSignature& Signature) const;

    void updateSignatureFromParametersUI(openfluid::ware::DataWareSignature& Signature) const;

    void updateSignatureFromExtrafilesUI(openfluid::ware::DataWareSignature& Signature) const;

    void updateSignatureFromReadAttributesUI(openfluid::ware::DataWareSignature& Signature) const;

    void updateSignatureFromAttributesUI(openfluid::ware::SimulatorSignature& Signature) const;

    void updateSignatureFromEventsUI(openfluid::ware::SimulatorSignature& Signature) const;

    void updateSignatureFromReadVariablesUI(openfluid::ware::DataWareSignature& Signature) const;

    void updateSignatureFromVariablesUI(openfluid::ware::SimulatorSignature& Signature) const;

    void updateSignatureFromDynamicsUI(openfluid::ware::SimulatorSignature& Signature) const;
    
    void updateSignatureFromIntegrationUI(openfluid::builderext::BuilderExtensionSignature& Signature) const;


  signals:

    void changed();


  public:

    explicit SignatureEditorWidget(QWidget* Parent = nullptr);

    virtual ~SignatureEditorWidget();

    void initializeSimulator(const openfluid::ware::SimulatorSignature& Signature);  // used by ghosts

    void initializeObserver(const openfluid::ware::ObserverSignature& Observer);

    void initializeBuilderext(const openfluid::builderext::BuilderExtensionSignature& Signature);
    
    void initialize(const QString& SignaturePath);

    openfluid::ware::SimulatorSignature getSignature() const; // used for ghosts

    bool exportSignature(const QString& SignaturePath) const;

    bool isValidID() const;

    QString getEditedID() const;

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_SIGNATUREEDITORWIDGET_HPP__ */
