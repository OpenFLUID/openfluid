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
  @file WareshubJsonEditor.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <fstream>

#include <QTextStream>
#include <QMessageBox>

#include <openfluid/ui/waresdev/WareshubJsonEditor.hpp>
#include <openfluid/base/FrameworkException.hpp>

#include "ui_WareshubJsonEditor.h"


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareshubJsonEditor::WareshubJsonEditor(const QString& FilePath, QWidget* Parent) :
    QWidget(Parent), WareFileEditor(FilePath), ui(new Ui::WareshubJsonEditor)
{
  ui->setupUi(this);

  ui->MessageFrame->updateDefaultMessage("wareshub.json");
  ui->MessageFrame->setMessage("");

  ui->FilePathLabel->setText(FilePath);

  ui->TagsLineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9a-zA-Z_,-]*$"), this));
  ui->DependenciesLineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9a-zA-Z_,-]*$"), this));

  ui->LicenseComboBox->addItems(m_Licenses);
  ui->LicenseComboBox->setEditText("");

  ui->DevStatusComboBox->addItems(m_Statuses);
  ui->DevStatusComboBox->setCurrentIndex(1);

  connect(ui->TagsLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(onChanged()));
  connect(ui->ContactsLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(onChanged()));
  connect(ui->DependenciesLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(onChanged()));

  connect(ui->LicenseComboBox, SIGNAL(editTextChanged(const QString &)), this, SLOT(onChanged()));
  connect(ui->DevStatusComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onChanged()));

  connect(ui->RemoveButton, SIGNAL(clicked()), this, SLOT(onRemoveIssueClicked()));
  connect(ui->EditButton, SIGNAL(clicked()), this, SLOT(onEditIssueClicked()));
  connect(ui->AddButton, SIGNAL(clicked()), this, SLOT(onAddIssueClicked()));

  connect(ui->IssuesTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(onEditIssueClicked()));

  updateContent();
}


// =====================================================================
// =====================================================================


WareshubJsonEditor::~WareshubJsonEditor()
{

}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::onChanged()
{
  m_IsModified = true;

  emit editorChanged(this, m_IsModified);
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::saveContent()
{
  saveContentToPath(m_FilePath);

  m_IsModified = false;

  emit editorChanged(this, m_IsModified);
  emit editorSaved();
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::saveContentToPath(const QString& Path)
{
  if (Path.isEmpty())
  {
    return;
  }

  if (!QDir().mkpath(QFileInfo(Path).absolutePath()))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              QString("Unable to create the path \"%1\"").arg(Path).toStdString());
  }

  std::ofstream File(Path.toStdString(),std::ofstream::out);
  if (!File.is_open())
  {
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION, QString("Unable to open the file \"%1\" in write mode").arg(Path).toStdString());
  }

  lineEditToJsonStringArray("tags", ui->TagsLineEdit);
  lineEditToJsonStringArray("contacts", ui->ContactsLineEdit);
  lineEditToJsonStringArray("external-deps", ui->DependenciesLineEdit);

  comboBoxToJsonString("license", ui->LicenseComboBox);
  comboBoxToJsonString("status", ui->DevStatusComboBox);

  issuesMapToJsonIssues();

  File << m_Doc.dump(2);
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::lineEditToJsonStringArray(const QString& Key, QLineEdit* LineEdit)
{
  QStringList Values = LineEdit->text().split(",");

  openfluid::tools::json JSONValue(openfluid::tools::json::value_t::array);

  for (const auto& Val : Values)
  {
    openfluid::tools::json v(Val.toStdString());
    JSONValue.push_back(v);
  }

  m_Doc[Key.toStdString()] = JSONValue;
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::comboBoxToJsonString(const QString& Key, QComboBox* Combo)
{
  QString Value = Combo->currentText();

  m_Doc[Key.toStdString()] = Value.toStdString();
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::issuesMapToJsonIssues()
{
  openfluid::tools::json JSONIssuesObject(openfluid::tools::json::value_t::object);

  for (const auto& Issue : m_IssuesByID.values())
  {
    openfluid::tools::json JSONIssueObject(openfluid::tools::json::value_t::object);

    JSONIssueObject["title"] = Issue.m_Title.toStdString();
    JSONIssueObject["creator"] = Issue.m_Creator.toStdString();
    JSONIssueObject["date"] = Issue.m_Date.toString("yyyy-MM-dd").toStdString();
    JSONIssueObject["type"] = Issue.m_Type.toStdString();
    JSONIssueObject["state"] = Issue.m_State.toStdString();
    JSONIssueObject["description"] = Issue.m_Description.toStdString();
    JSONIssueObject["urgency"] = Issue.m_Urgency.toStdString();
    
    JSONIssuesObject[Issue.m_ID.toStdString()] = JSONIssueObject;
  }

  m_Doc["issues"] = JSONIssuesObject;
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::updateContent()
{
  std::ifstream FileStream;

  FileStream.open(m_FilePath.toStdString().c_str(),std::ifstream::in);
  if (!FileStream.is_open())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              QString("Cannot open file %1").arg(m_FilePath).toStdString());
  }

  try
  {
    m_Doc = openfluid::tools::json::parse(FileStream);
  }
  catch (openfluid::tools::json::parse_error&)
  {
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        QString("Error while parsing json file %1").arg(m_FilePath).toStdString());
  }

  if (!m_Doc.is_object())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              QString("Error in json file %1").arg(m_FilePath).toStdString());
  }


  jsonStringArrayToLineEdit("tags", ui->TagsLineEdit);
  jsonStringArrayToLineEdit("contacts", ui->ContactsLineEdit);
  jsonStringArrayToLineEdit("external-deps", ui->DependenciesLineEdit);

  jsonStringToComboBox("license", ui->LicenseComboBox);
  jsonStringToComboBox("status", ui->DevStatusComboBox);

  jsonIssuesToIssuesMap();
  updateIssuesTable();

  m_IsModified = false;
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::jsonStringArrayToLineEdit(const QString& Key, QLineEdit* LineEdit)
{
  QStringList Values;

  if (m_Doc.contains(Key.toStdString()))
  {
    const openfluid::tools::json& JSONValue = m_Doc[Key.toStdString()];
    if (JSONValue.is_array())
    {
      for (const auto& v : JSONValue)
      {
        if (v.is_string())
        {
          Values << QString::fromStdString(v.get<std::string>());
        }
      }
    }
  }

  LineEdit->setText(Values.join(","));
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::jsonStringToComboBox(const QString& Key, QComboBox* Combo)
{
  if (m_Doc.contains(Key.toStdString()))
  {
    const openfluid::tools::json& JSONValue = m_Doc[Key.toStdString()];
    if (JSONValue.is_string())
    {
      QString Str = QString::fromStdString(JSONValue.get<std::string>());
      int Index = Combo->findText(Str, Qt::MatchFixedString);
      if (Index > -1)
      {
        Combo->setCurrentIndex(Index);
      }
      else if (Combo->isEditable())
      {
        Combo->setEditText(Str);
      }
    }
  }
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::jsonIssuesToIssuesMap()
{
  QStringList Values;

  if (m_Doc.contains("issues"))
  {
    const openfluid::tools::json& IssuesValue = m_Doc["issues"];

    if (IssuesValue.is_object())
    {
      for (const auto& [ID,Info] : IssuesValue.items())
      {

        if (Info.is_object())
        {
          WareshubIssueDialog::Issue I;

          if (Info.contains("title"))
          {
            I.m_Title = QString::fromStdString(Info["title"].get<std::string>());
          }

          if (Info.contains("creator"))
          {
            I.m_Creator = QString::fromStdString(Info["creator"].get<std::string>());
          }

          if (Info.contains("date"))
          {
            I.m_Date = QDate::fromString(QString::fromStdString(Info["date"].get<std::string>()), "yyyy-MM-dd");
          }

          if (Info.contains("type"))
          {
            I.m_Type = QString::fromStdString(Info["type"].get<std::string>());
          }

          if (Info.contains("state"))
          {
            I.m_State = QString::fromStdString(Info["state"].get<std::string>());
          }

          if (Info.contains("description"))
          {
            I.m_Description = QString::fromStdString(Info["description"].get<std::string>());
          }

          if (Info.contains("urgency"))
          {
            I.m_Urgency = QString::fromStdString(Info["urgency"].get<std::string>());
            I.m_Urgency.replace("normal", "medium");
          }

          I.m_ID = QString::fromStdString(ID);

          m_IssuesByID[I.m_ID] = I;
        }

      }

    }
  }

}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::updateIssuesTable()
{
  ui->IssuesTable->clear();

  ui->IssuesTable->setHorizontalHeaderLabels(QStringList( { tr("ID"), tr("Title"), tr("Type"), tr("State") }));
  ui->IssuesTable->verticalHeader()->setVisible(false);

  ui->IssuesTable->setRowCount(m_IssuesByID.size());

  int row = 0;
  bool Ok;
  for (const auto& ID : m_IssuesByID.keys())
  {
    QTableWidgetItem* IDItem = new QTableWidgetItem("");
    int IDInt = ID.toInt(&Ok);
    if (Ok)
    {
      IDItem->setData(Qt::DisplayRole, IDInt);
    }
    else
    {
      IDItem->setData(Qt::DisplayRole, ID);
    }
    ui->IssuesTable->setItem(row, 0, IDItem);

    ui->IssuesTable->setItem(row, 1, new QTableWidgetItem(m_IssuesByID[ID].m_Title));
    ui->IssuesTable->setItem(row, 2, new QTableWidgetItem(m_IssuesByID[ID].m_Type));
    ui->IssuesTable->setItem(row, 3, new QTableWidgetItem(m_IssuesByID[ID].m_State));

    row++;
  }

  ui->IssuesTable->resizeColumnsToContents();
}


// =====================================================================
// =====================================================================


bool WareshubJsonEditor::findReplace(FindReplaceDialog::FindReplaceAction /*Action*/, const QString& /*StringToFind*/,
  const QString& /*StringForReplace*/, QTextDocument::FindFlags /*Options*/, QString& /*Message*/)
{
  return false;
}


// =====================================================================
// =====================================================================


QString WareshubJsonEditor::getSelectedText()
{
  return "";
}


// =====================================================================
// =====================================================================


bool WareshubJsonEditor::isModified()
{
  return m_IsModified;
}


// =====================================================================
// =====================================================================


QWidget* WareshubJsonEditor::getWidget()
{
  return this;
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::onRemoveIssueClicked()
{
  QModelIndexList selectedList = ui->IssuesTable->selectionModel()->selectedRows();

  if (!selectedList.isEmpty())
  {
    QModelIndex Selected = selectedList.at(0);
    QString ID = Selected.data(Qt::DisplayRole).toString();

    if (QMessageBox::question(
                              this,
                              tr("Remove an issue"),
                              tr("Are you sure you want to remove issue \"%1\" ?").arg(ID),
                              QMessageBox::Ok | QMessageBox::Cancel)
        == QMessageBox::Ok)
    {
      ui->IssuesTable->removeRow(Selected.row());

      m_IssuesByID.remove(ID);

      onChanged();
    }
  }
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::onEditIssueClicked()
{
  QModelIndexList selectedList = ui->IssuesTable->selectionModel()->selectedRows();

  if (!selectedList.isEmpty())
  {
    QModelIndex Selected = selectedList.at(0);
    QString ID = Selected.data(Qt::DisplayRole).toString();

    WareshubIssueDialog Dialog(m_IssuesByID.keys(), this, m_IssuesByID[ID]);

    if (Dialog.exec() == QDialog::Accepted)
    {
      int Row = Selected.row();

      WareshubIssueDialog::Issue I = Dialog.getIssue();

      if (ID != I.m_ID)
      {
        QTableWidgetItem* IDItem = ui->IssuesTable->item(Row, 0);
        bool Ok;
        int IDInt = I.m_ID.toInt(&Ok);
        if (Ok)
        {
          IDItem->setData(Qt::DisplayRole, IDInt);
        }
        else
        {
          IDItem->setData(Qt::DisplayRole, I.m_ID);
        }

        Row = IDItem->row();

        m_IssuesByID.remove(ID);
      }

      ui->IssuesTable->item(Row, 1)->setData(Qt::DisplayRole, I.m_Title);
      ui->IssuesTable->item(Row, 2)->setData(Qt::DisplayRole, I.m_Type);
      ui->IssuesTable->item(Row, 3)->setData(Qt::DisplayRole, I.m_State);

      m_IssuesByID[I.m_ID] = I;

      ui->IssuesTable->selectRow(Row);

      onChanged();
    }
  }

}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::onAddIssueClicked()
{
  WareshubIssueDialog Dialog(m_IssuesByID.keys(), this);

  if (Dialog.exec() == QDialog::Accepted)
  {
    WareshubIssueDialog::Issue I = Dialog.getIssue();

    int RowCountBefore = ui->IssuesTable->rowCount();

    ui->IssuesTable->setRowCount(RowCountBefore + 1);

    QTableWidgetItem* IDItem = new QTableWidgetItem("");
    bool Ok;
    int IDInt = I.m_ID.toInt(&Ok);
    if (Ok)
    {
      IDItem->setData(Qt::DisplayRole, IDInt);
    }
    else
    {
      IDItem->setData(Qt::DisplayRole, I.m_ID);
    }
    ui->IssuesTable->setItem(RowCountBefore, 0, IDItem);

    int NewRow = IDItem->row();

    ui->IssuesTable->setItem(NewRow, 1, new QTableWidgetItem(I.m_Title));
    ui->IssuesTable->setItem(NewRow, 2, new QTableWidgetItem(I.m_Type));
    ui->IssuesTable->setItem(NewRow, 3, new QTableWidgetItem(I.m_State));

    m_IssuesByID[I.m_ID] = I;

    ui->IssuesTable->selectRow(NewRow);

    onChanged();
  }
}


// =====================================================================
// =====================================================================


} } } // namespaces
