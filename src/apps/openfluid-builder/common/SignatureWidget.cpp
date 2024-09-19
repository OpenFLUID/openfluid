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

  if (FinalStr.isNull() || FinalStr.isEmpty())
  {
    FinalStr = ReplaceStr;
  }

  return FinalStr;
}


// =====================================================================
// =====================================================================


void SignatureWidget::clearAllInfos()
{
  while (ui->InfosTabWidget->count() > 1)
  {
    ui->InfosTabWidget->removeTab(1);
  }

  ui->GeneralLabel->setText("");

  while (ui->ParametersTableWidget->rowCount() > 0)
  {
    ui->ParametersTableWidget->removeRow(0);
  }

  while (ui->ExtrafilesTableWidget->rowCount() > 0)
  {
    ui->ExtrafilesTableWidget->removeRow(0);
  }

  while (ui->VariablesTableWidget->rowCount() > 0)
  {
    ui->VariablesTableWidget->removeRow(0);
  }

  while (ui->AttributesTableWidget->rowCount() > 0)
  {
    ui->AttributesTableWidget->removeRow(0);
  }

  while (ui->EventsTableWidget->rowCount() > 0)
  {
    ui->EventsTableWidget->removeRow(0);
  }

  while (ui->GraphTableWidget->rowCount() > 0)
  {
    ui->GraphTableWidget->removeRow(0);
  }
}


// =====================================================================
// =====================================================================


QString SignatureWidget::formatAuthors(const openfluid::ware::WareSignature::PeopleList_t& AuthList)
{
  QString Str;

  openfluid::ware::WareSignature::PeopleList_t::const_iterator itb = AuthList.begin();
  openfluid::ware::WareSignature::PeopleList_t::const_iterator ite = AuthList.end();
  openfluid::ware::WareSignature::PeopleList_t::const_iterator itl = AuthList.end().operator--();
  openfluid::ware::WareSignature::PeopleList_t::const_iterator it;

  for (it=itb; it!= ite; ++it)
  {
    QString Name = convertStdString((*it).first,"unknown");
    QString Email = convertStdString((*it).second,"no email");
    Str += Name + " &lt;" + Email + "&gt;";
    if (it!=itl)
    {
      Str+=", ";
    }
  }

  return Str;
}


// =====================================================================
// =====================================================================


QString SignatureWidget::getGeneralInfoLine(const QString& Title, const QString& Text, bool WithPreBR)
{
  return QString("%3<b>%1</b>: %2").arg(Title).arg(Text).arg(WithPreBR ? "<br/>" : "");
}


// =====================================================================
// =====================================================================


template<typename SignatureType>
QString SignatureWidget::getCommonForGeneral(const openfluid::machine::WareContainer<SignatureType>& Container)
{
  QString Contents;

  Contents += getGeneralInfoLine(tr("ID"),convertStdString(Container.signature()->ID),false);
  Contents += getGeneralInfoLine(tr("Name"),convertStdString(Container.signature()->Name));
  Contents += getGeneralInfoLine(tr("Description"),convertStdString(Container.signature()->Description));


  if (Container.signature()->getType() == openfluid::ware::WareType::SIMULATOR ||
      Container.signature()->getType() == openfluid::ware::WareType::OBSERVER)
  {
    Contents += "<hr>";

    Contents += getGeneralInfoLine(tr("Version"),convertStdString(Container.signature()->Version),false);
    Contents += getGeneralInfoLine(tr("Author(s)"),
                                   Container.signature()->Authors.empty() ? 
                                   convertStdString("") : formatAuthors(Container.signature()->Authors));


    QString StatusStr = tr("experimental");
    if (Container.signature()->Status == openfluid::ware::BETA)
    {
      StatusStr = tr("beta");
    }
    if (Container.signature()->Status == openfluid::ware::STABLE)
    {
      StatusStr = tr("stable");
    }
    Contents += getGeneralInfoLine(tr("Status"),StatusStr);

    Contents += "<hr>";

    Contents += getGeneralInfoLine(tr("File path"),
                                   QDir::toNativeSeparators(convertStdString(Container.getPath())),false);

    auto BuildInfo = Container.signature()->BuildInfo;
    if (!BuildInfo.BuildType.empty())
    {
      Contents += getGeneralInfoLine(tr("Build type"),QString::fromStdString(BuildInfo.BuildType));
    }
    if (!BuildInfo.CompilerID.empty())
    {
      Contents += getGeneralInfoLine(tr("Compiler ID"),QString::fromStdString(BuildInfo.CompilerID));
    }
    if (!BuildInfo.CompilerVersion.empty())
    {
      Contents += getGeneralInfoLine(tr("Compiler version"),QString::fromStdString(BuildInfo.CompilerVersion));
    }
    if (!BuildInfo.CompilationFlags.empty())
    {
      Contents += getGeneralInfoLine(tr("Compilation flags"),QString::fromStdString(BuildInfo.CompilationFlags));
    }
  }

  return Contents;
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateGeneral(const openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature>& 
                                      Container)
{
  QString Contents = getCommonForGeneral(Container);

  if (Container.signature()->getType() == openfluid::ware::WareType::SIMULATOR)
  {
    Contents += "<hr>";

    Contents += getGeneralInfoLine(tr("Domain(s)"),convertStdString(Container.signature()->Domain),false);
    Contents += getGeneralInfoLine(tr("Process(es)"),convertStdString(Container.signature()->Process));
    Contents += getGeneralInfoLine(tr("Methods(s)"),convertStdString(Container.signature()->Method));
  }

  ui->GeneralLabel->setText(Contents);
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateGeneral(const openfluid::machine::WareContainer<openfluid::ware::ObserverSignature>& 
                                      Container)
{
  QString Contents = getCommonForGeneral(Container);

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

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).Name));
    ui->ParametersTableWidget->setItem(i+BaseIndex, 1, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).SIUnit));
    ui->ParametersTableWidget->setItem(i+BaseIndex, 2, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).Description));
    ui->ParametersTableWidget->setItem(i+BaseIndex, 3, Item);
  }
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateParameters(const openfluid::ware::SimulatorSignature* Signature)
{
  const std::vector<openfluid::ware::SignatureDataItem>* ReqParams = &(Signature->HandledData.RequiredParams);
  const std::vector<openfluid::ware::SignatureDataItem>* UsParams = &(Signature->HandledData.UsedParams);

  ui->ParametersTableWidget->setRowCount(ReqParams->size()+UsParams->size());

  updateParametersCategory(ReqParams,tr("Required"),0);
  updateParametersCategory(UsParams,tr("Used"),ReqParams->size());

  if (ui->ParametersTableWidget->rowCount() > 0)
  {
    ui->InfosTabWidget->addTab(ui->ParametersTab,tr("Parameters"));
  }
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


void SignatureWidget::updateExtrafiles(const openfluid::ware::SimulatorSignature* Signature)
{
  const std::vector<std::string>* ReqFiles = &(Signature->SimulatorHandledData.RequiredExtraFiles);
  const std::vector<std::string>* UsFiles = &(Signature->SimulatorHandledData.UsedExtraFiles);

  ui->ExtrafilesTableWidget->setRowCount(ReqFiles->size()+UsFiles->size());

  updateExtrafilesCategory(ReqFiles,tr("Required"),0);
  updateExtrafilesCategory(UsFiles,tr("Used"),ReqFiles->size());

  if (ui->ExtrafilesTableWidget->rowCount() > 0)
  {
    ui->InfosTabWidget->addTab(ui->ExtrafilesTab,tr("Extra files"));
  }
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateVariablesCategory(const std::vector<openfluid::ware::SignatureSpatialDataItem>* Infos,
                                              const QString& CatStr, unsigned int BaseIndex)
{

  QTableWidgetItem *Item;

  for (unsigned int i = 0; i < Infos->size(); i++)
  {
    Item = new QTableWidgetItem(CatStr);
    ui->VariablesTableWidget->setItem(i+BaseIndex, 0, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).Name));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 1, Item);

    Item = new QTableWidgetItem(QString::fromStdString(
        openfluid::core::Value::getStringFromValueType(Infos->at(i).DataType)));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 2, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).SIUnit));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 3, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).UnitsClass));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 4, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).Description));
    ui->VariablesTableWidget->setItem(i+BaseIndex, 5, Item);
  }

}


// =====================================================================
// =====================================================================


void SignatureWidget::updateVariables(const openfluid::ware::SimulatorSignature* Signature)
{
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* ProdVars =
      &(Signature->SimulatorHandledData.ProducedVars);
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* ReqVars =
      &(Signature->SimulatorHandledData.RequiredVars);
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* UsVars =
      &(Signature->SimulatorHandledData.UsedVars);
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* UpdVars =
      &(Signature->SimulatorHandledData.UpdatedVars);


  ui->VariablesTableWidget->setRowCount(ProdVars->size()+ReqVars->size()+UsVars->size()+UpdVars->size());

  updateVariablesCategory(ProdVars,tr("Produced"),0);
  updateVariablesCategory(ReqVars,tr("Required"),ProdVars->size());
  updateVariablesCategory(UsVars,tr("Used"),ProdVars->size()+ReqVars->size());
  updateVariablesCategory(UpdVars,tr("Updated"),ProdVars->size()+ReqVars->size()+UsVars->size());

  if (ui->VariablesTableWidget->rowCount() > 0)
  {
    ui->InfosTabWidget->addTab(ui->VariablesTab,tr("Variables"));
  }
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

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).Name));
    ui->AttributesTableWidget->setItem(i+BaseIndex, 1, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).SIUnit));
    ui->AttributesTableWidget->setItem(i+BaseIndex, 2, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).UnitsClass));
    ui->AttributesTableWidget->setItem(i+BaseIndex, 3, Item);

    Item = new QTableWidgetItem(QString::fromStdString(Infos->at(i).Description));
    ui->AttributesTableWidget->setItem(i+BaseIndex, 4, Item);
  }

}


// =====================================================================
// =====================================================================


void SignatureWidget::updateAttributes(const openfluid::ware::SimulatorSignature* Signature)
{
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* ProdAttrs =
      &(Signature->SimulatorHandledData.ProducedAttribute);
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* ReqAttrs =
      &(Signature->SimulatorHandledData.RequiredAttribute);
  const std::vector<openfluid::ware::SignatureSpatialDataItem>* UsAttrs =
      &(Signature->SimulatorHandledData.UsedAttribute);


  ui->AttributesTableWidget->setRowCount(ProdAttrs->size()+ReqAttrs->size()+UsAttrs->size());

  updateAttributesCategory(ProdAttrs,tr("Produced"),0);
  updateAttributesCategory(ReqAttrs,tr("Required"),ProdAttrs->size());
  updateAttributesCategory(UsAttrs,tr("Used"),ProdAttrs->size()+ReqAttrs->size());

  if (ui->AttributesTableWidget->rowCount() > 0)
  {
    ui->InfosTabWidget->addTab(ui->AttributesTab,tr("Attributes"));
  }
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateEvents(const openfluid::ware::SimulatorSignature* Signature)
{
  const std::vector<std::string>* Events = &(Signature->SimulatorHandledData.UsedEventsOnUnits);

  ui->EventsTableWidget->setRowCount(Events->size());

  QTableWidgetItem *Item;

  for (unsigned int i = 0; i < Events->size(); i++)
  {
    Item = new QTableWidgetItem(QString::fromStdString(Events->at(i)));
    ui->EventsTableWidget->setItem(i, 0, Item);
  }

  if (ui->EventsTableWidget->rowCount() > 0)
  {
    ui->InfosTabWidget->addTab(ui->EventsTab,tr("Events"));
  }
}


// =====================================================================
// =====================================================================


void SignatureWidget::updateSpatialGraph(const openfluid::ware::SimulatorSignature* Signature)
{
  const std::string Desc = Signature->HandledUnitsGraph.UpdatedUnitsGraph;
  const std::vector<openfluid::ware::SignatureUnitsClassItem>* UnitsClasses =
      &(Signature->HandledUnitsGraph.UpdatedUnitsClass);

  if (!Desc.empty())
  {
    ui->GraphDescriptionLabel->setText(tr("<b>Overall description</b>")+": "+convertStdString(Desc));
  }

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
  {
    ui->InfosTabWidget->addTab(ui->SpatialGraphTab,tr("Spatial graph"));
  }

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


void SignatureWidget::update(const openfluid::machine::WareContainer<openfluid::ware::SimulatorSignature>& Container)
{
  mute();

  if (Container.isValid() && Container.hasSignature())
  {
    setEnabled(true);
    updateGeneral(Container);
    updateParameters(Container.signature().get());
    updateExtrafiles(Container.signature().get());
    updateVariables(Container.signature().get());
    updateAttributes(Container.signature().get());
    updateEvents(Container.signature().get());
    updateSpatialGraph(Container.signature().get());
  }
}


// =====================================================================
// =====================================================================


void SignatureWidget::update(const openfluid::machine::WareContainer<openfluid::ware::ObserverSignature>& Container)
{
  mute();

  if (Container.isValid() && Container.hasSignature())
  {
    setEnabled(true);
    updateGeneral(Container);
  }
}

