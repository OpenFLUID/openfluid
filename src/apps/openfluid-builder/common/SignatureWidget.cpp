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
  @file SignatureWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QDir>

#include <openfluid/ui/config.hpp>

#include "ui_SignatureWidget.h"
#include "SignatureWidget.hpp"


SignatureWidget::SignatureWidget(QWidget* Parent):
  QWidget(Parent), ui(new Ui::SignatureWidget)
{
  ui->setupUi(this);

  ui->InfosTabWidget->setCurrentIndex(0);
}


// =====================================================================
// =====================================================================


SignatureWidget::~SignatureWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


QString SignatureWidget::convertStdString(const std::string& Str, const QString& ReplaceStr)
{
  QString FinalStr = QString::fromStdString(Str);

  FinalStr.replace("&","&amp;").replace("<","&lt;").replace(">","&gt;").replace("\"","&quot;").replace("\n","<br/>");

  if (FinalStr.isNull() || FinalStr.isEmpty()) FinalStr = ReplaceStr;

  return FinalStr;
}


// =====================================================================
// =====================================================================


void SignatureWidget::clearAllInfos()
{
  while (ui->InfosTabWidget->count() > 1)
    ui->InfosTabWidget->removeTab(1);

  ui->GeneralLabel->setText("");

  while (ui->ParametersTableWidget->rowCount() > 0)
    ui->ParametersTableWidget->removeRow(0);

  while (ui->ExtrafilesTableWidget->rowCount() > 0)
    ui->ExtrafilesTableWidget->removeRow(0);

  while (ui->VariablesTableWidget->rowCount() > 0)
    ui->VariablesTableWidget->removeRow(0);

  while (ui->AttributesTableWidget->rowCount() > 0)
    ui->AttributesTableWidget->removeRow(0);

  while (ui->EventsTableWidget->rowCount() > 0)
    ui->EventsTableWidget->removeRow(0);

  while (ui->GraphTableWidget->rowCount() > 0)
    ui->GraphTableWidget->removeRow(0);
}


// =====================================================================
// =====================================================================


QString SignatureWidget::formatAuthors(const openfluid::ware::WareSignature::AuthorsList_t& AuthList)
{
  QString Str;

  openfluid::ware::WareSignature::AuthorsList_t::const_iterator itb = AuthList.begin();
  openfluid::ware::WareSignature::AuthorsList_t::const_iterator ite = AuthList.end();
  openfluid::ware::WareSignature::AuthorsList_t::const_iterator itl = AuthList.end().operator --();
  openfluid::ware::WareSignature::AuthorsList_t::const_iterator it;

  for (it=itb; it!= ite; ++it)
  {
    QString Name = convertStdString((*it).first,"unknown");
    QString Email = convertStdString((*it).second,"no email");
    Str += Name + " &lt;" + Email + "&gt;";
    if (it!=itl) Str+=", ";
  }

  return Str;
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateGeneral(const openfluid::machine::ModelItemSignatureInstance* Signature)
{
  QString Contents;

  QString PathLabelStr = tr("Plugin path");

  if (Signature->Ghost)
    PathLabelStr = tr("Ghost path");

  Contents += "<b>" + tr("ID") + ":</b> " + convertStdString(Signature->Signature->ID) + "<br/>";
  Contents += "<b>" + tr("Name") + ":</b> " + convertStdString(Signature->Signature->Name) + "<br/>";
  Contents += "<b>" + tr("Description") + ":</b> " + convertStdString(Signature->Signature->Description);

  if (Signature->ItemType == openfluid::ware::WareType::SIMULATOR)
  {
    Contents += "<hr>";
    Contents += "<b>" + tr("Author(s)") + ":</b> ";

    if (!Signature->Signature->Authors.empty())
      Contents += formatAuthors(Signature->Signature->Authors);
    else
      Contents += convertStdString("");

    Contents += "<hr>";
    Contents += "<b>" + PathLabelStr + ":</b> " +
                QDir::toNativeSeparators(convertStdString(Signature->FileFullPath));
    Contents += "<hr>";
    Contents += "<b>" + tr("Version") + ":</b> " + convertStdString(Signature->Signature->Version) + "<br/>";

    QString StatusStr = tr("experimental");
    if (Signature->Signature->Status == openfluid::ware::BETA) StatusStr = tr("beta");
    if (Signature->Signature->Status == openfluid::ware::STABLE) StatusStr = tr("stable");
    Contents += "<b>" + tr("Status") + ":</b> " + StatusStr + "<br/>";
    Contents += "<hr>";
    Contents += "<b>" + tr("Domain(s)") + ":</b> " + convertStdString(Signature->Signature->Domain) + "<br/>";
    Contents += "<b>" + tr("Process(es)") + ":</b> " + convertStdString(Signature->Signature->Process) + "<br/>";
    Contents += "<b>" + tr("Methods(s)") + ":</b> " + convertStdString(Signature->Signature->Method) + "<br/>";

  }

  ui->GeneralLabel->setText(Contents);
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateParametersCategory(const std::vector<openfluid::ware::SignatureDataItem>* Infos,
                                               const QString& CatStr, unsigned int BaseIndex)
{
  for (unsigned int i = 0; i < Infos->size(); i++)
  {
    QTableWidgetItem *Item;

    Item = new QTableWidgetItem(CatStr);
        ui->ParametersTableWidget->setItem(i+BaseIndex, 0, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).DataName));
    ui->ParametersTableWidget->setItem(i+BaseIndex, 1, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).DataUnit));
    ui->ParametersTableWidget->setItem(i+BaseIndex, 2, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).Description));
    ui->ParametersTableWidget->setItem(i+BaseIndex, 3, Item);
  }
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateParameters(const openfluid::machine::ModelItemSignatureInstance* Signature)
{
  const std::vector<openfluid::ware::SignatureDataItem>* ReqParams =
      &(Signature->Signature->HandledData.RequiredParams);
  const std::vector<openfluid::ware::SignatureDataItem>* UsParams =
      &(Signature->Signature->HandledData.UsedParams);

  ui->ParametersTableWidget->setRowCount(ReqParams->size()+UsParams->size());

  updateParametersCategory(ReqParams,tr("Required"),0);
  updateParametersCategory(UsParams,tr("Used"),ReqParams->size());

  if (ui->ParametersTableWidget->rowCount() > 0)
    ui->InfosTabWidget->addTab(ui->ParametersTab,tr("Parameters"));
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateExtrafilesCategory(const std::vector<std::string>* Infos,
                                               const QString& CatStr, unsigned int BaseIndex)
{
  QTableWidgetItem *Item;

  for (unsigned int i = 0; i < Infos->size(); i++)
  {
    Item = new QTableWidgetItem(CatStr);
    ui->ExtrafilesTableWidget->setItem(i+BaseIndex, 0, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i)));
    ui->ExtrafilesTableWidget->setItem(i+BaseIndex, 1, Item);
  }
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateExtrafiles(const openfluid::machine::ModelItemSignatureInstance* Signature)
{
  const std::vector<std::string>* ReqFiles = &(Signature->Signature->HandledData.RequiredExtraFiles);
  const std::vector<std::string>* UsFiles = &(Signature->Signature->HandledData.UsedExtraFiles);

  ui->ExtrafilesTableWidget->setRowCount(ReqFiles->size()+UsFiles->size());

  updateExtrafilesCategory(ReqFiles,tr("Required"),0);
  updateExtrafilesCategory(UsFiles,tr("Used"),ReqFiles->size());

  if (ui->ExtrafilesTableWidget->rowCount() > 0)
    ui->InfosTabWidget->addTab(ui->ExtrafilesTab,tr("Extra files"));
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateVariablesCategory(const std::vector<openfluid::ware::SignatureTypedSpatialDataItem>* Infos,
                                              const QString& CatStr, unsigned int BaseIndex)
{

  QTableWidgetItem *Item;

  for (unsigned int i = 0; i < Infos->size(); i++)
  {
    Item = new QTableWidgetItem(CatStr);
    ui->VariablesTableWidget->setItem(i+BaseIndex, 0, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).DataName));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 1, Item);

    Item = new QTableWidgetItem(QString::fromStdString(
        openfluid::core::Value::getStringFromValueType(Infos->at(i).DataType)));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 2, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).DataUnit));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 3, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).UnitsClass));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 4, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).Description));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 5, Item);
  }

}


// =====================================================================
// =====================================================================


void SignatureWidget::updateVariables(const openfluid::machine::ModelItemSignatureInstance* Signature)
{
  const std::vector<openfluid::ware::SignatureTypedSpatialDataItem>* ProdVars =
      &(Signature->Signature->HandledData.ProducedVars);
  const std::vector<openfluid::ware::SignatureTypedSpatialDataItem>* ReqVars =
      &(Signature->Signature->HandledData.RequiredVars);
  const std::vector<openfluid::ware::SignatureTypedSpatialDataItem>* UsVars =
      &(Signature->Signature->HandledData.UsedVars);
  const std::vector<openfluid::ware::SignatureTypedSpatialDataItem>* UpdVars =
      &(Signature->Signature->HandledData.UpdatedVars);


  ui->VariablesTableWidget->setRowCount(ProdVars->size()+ReqVars->size()+UsVars->size()+UpdVars->size());

  updateVariablesCategory(ProdVars,tr("Produced"),0);
  updateVariablesCategory(ReqVars,tr("Required"),ProdVars->size());
  updateVariablesCategory(UsVars,tr("Used"),ProdVars->size()+ReqVars->size());
  updateVariablesCategory(UpdVars,tr("Updated"),ProdVars->size()+ReqVars->size()+UsVars->size());

  if (ui->VariablesTableWidget->rowCount() > 0)
    ui->InfosTabWidget->addTab(ui->VariablesTab,tr("Variables"));
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateAttributesCategory(const std::vector<openfluid::ware::SignatureSpatialDataItem>* Infos,
                                               const QString& CatStr, unsigned int BaseIndex)
{

  QTableWidgetItem *Item;

  for (unsigned int i = 0; i < Infos->size(); i++)
  {
    Item = new QTableWidgetItem(CatStr);
    ui->AttributesTableWidget->setItem(i+BaseIndex, 0, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).DataName));
    ui->AttributesTableWidget->setItem(i+BaseIndex, 1, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).DataUnit));
    ui->AttributesTableWidget->setItem(i+BaseIndex, 2, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).UnitsClass));
    ui->AttributesTableWidget->setItem(i+BaseIndex, 3, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).Description));
    ui->AttributesTableWidget->setItem(i+BaseIndex, 4, Item);
  }

}


// =====================================================================
// =====================================================================


void SignatureWidget::updateAttributes(const openfluid::machine::ModelItemSignatureInstance* Signature)
{
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* ProdAttrs =
      &(Signature->Signature->HandledData.ProducedAttribute);
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* ReqAttrs =
      &(Signature->Signature->HandledData.RequiredAttribute);
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* UsAttrs =
      &(Signature->Signature->HandledData.UsedAttribute);


  ui->AttributesTableWidget->setRowCount(ProdAttrs->size()+ReqAttrs->size()+UsAttrs->size());

  updateAttributesCategory(ProdAttrs,tr("Produced"),0);
  updateAttributesCategory(ReqAttrs,tr("Required"),ProdAttrs->size());
  updateAttributesCategory(UsAttrs,tr("Used"),ProdAttrs->size()+ReqAttrs->size());

  if (ui->AttributesTableWidget->rowCount() > 0)
    ui->InfosTabWidget->addTab(ui->AttributesTab,tr("Attributes"));
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateEvents(const openfluid::machine::ModelItemSignatureInstance* Signature)
{
  const std::vector<std::string>* Events = &(Signature->Signature->HandledData.UsedEventsOnUnits);

  ui->EventsTableWidget->setRowCount(Events->size());

  QTableWidgetItem *Item;

  for (unsigned int i = 0; i < Events->size(); i++)
  {
    Item = new QTableWidgetItem(QString::fromStdString(Events->at(i)));
    ui->EventsTableWidget->setItem(i, 0, Item);
  }

  if (ui->EventsTableWidget->rowCount() > 0)
    ui->InfosTabWidget->addTab(ui->EventsTab,tr("Events"));
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateSpatialGraph(const openfluid::machine::ModelItemSignatureInstance* Signature)
{
  const std::string Desc = Signature->Signature->HandledUnitsGraph.UpdatedUnitsGraph;
  const std::vector<openfluid::ware::SignatureUnitsClassItem>* UnitsClasses =
      &(Signature->Signature->HandledUnitsGraph.UpdatedUnitsClass);

  if (!Desc.empty())
    ui->GraphDescriptionLabel->setText(tr("<b>Overall description</b>")+": "+convertStdString(Desc));

  ui->GraphTableWidget->setRowCount(UnitsClasses->size());

  QTableWidgetItem *Item;

  for (unsigned int i = 0; i < UnitsClasses->size(); i++)
  {
    Item = new QTableWidgetItem(QString::fromStdString(UnitsClasses->at(i).UnitsClass));
    ui->GraphTableWidget->setItem(i, 0, Item);

    Item = new QTableWidgetItem(QString::fromStdString(UnitsClasses->at(i).Description));
    ui->GraphTableWidget->setItem(i, 1, Item);
  }

  if (ui->GraphTableWidget->rowCount() > 0 || !Desc.empty())
    ui->InfosTabWidget->addTab(ui->SpatialGraphTab,tr("Spatial graph"));

}


// =====================================================================
// =====================================================================


void SignatureWidget::updateGeneral(const openfluid::machine::ObserverSignatureInstance* Signature)
{
  QString Contents;

  Contents += "<b>" + tr("ID") + ":</b> " + convertStdString(Signature->Signature->ID) + "<br/>";
  Contents += "<b>" + tr("Name") + ":</b> " + convertStdString(Signature->Signature->Name) + "<br/>";
  Contents += "<b>" + tr("Description") + ":</b> " + convertStdString(Signature->Signature->Description);

  Contents += "<hr>";
  Contents += "<b>" + tr("Author(s)") + ":</b> ";

  if (!Signature->Signature->Authors.empty())
    Contents += formatAuthors(Signature->Signature->Authors);
  else
    Contents += convertStdString("");

  Contents += "<hr>";
  Contents += "<b>" + tr("Plugin path") + ":</b> " + convertStdString(Signature->FileFullPath);
  Contents += "<hr>";
  Contents += "<b>" + tr("Version") + ":</b> " + convertStdString(Signature->Signature->Version) + "<br/>";

  QString StatusStr = tr("experimental");
  if (Signature->Signature->Status == openfluid::ware::BETA) StatusStr = tr("beta");
  if (Signature->Signature->Status == openfluid::ware::STABLE) StatusStr = tr("stable");
  Contents += "<b>" + tr("Status") + ":</b> " + StatusStr + "<br/>";


  ui->GeneralLabel->setText(Contents);
}


// =====================================================================
// =====================================================================


void SignatureWidget::mute()
{
  setEnabled(false);
  clearAllInfos();
}


// =====================================================================
// =====================================================================


void SignatureWidget::update(const openfluid::machine::ModelItemSignatureInstance* Signature)
{
  mute();

  if (Signature != nullptr)
  {
    setEnabled(true);
    updateGeneral(Signature);
    updateParameters(Signature);
    updateExtrafiles(Signature);
    updateVariables(Signature);
    updateAttributes(Signature);
    updateEvents(Signature);
    updateSpatialGraph(Signature);
  }
}


// =====================================================================
// =====================================================================


void SignatureWidget::update(const openfluid::machine::ObserverSignatureInstance* Signature)
{
  mute();

  if (Signature != nullptr)
  {
    setEnabled(true);
    updateGeneral(Signature);
  }
}

