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
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
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
    return;

  if (!QDir().mkpath(QFileInfo(Path).absolutePath()))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              QString("Unable to create the path \"%1\"").arg(Path).toStdString());

  QFile File(Path);

  if (!File.open(QIODevice::WriteOnly | QIODevice::Text))
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION, QString("Unable to open the file \"%1\" in write mode").arg(Path).toStdString());

  lineEditToJsonStringArray("tags", ui->TagsLineEdit);
  lineEditToJsonStringArray("contacts", ui->ContactsLineEdit);
  lineEditToJsonStringArray("external-deps", ui->DependenciesLineEdit);

  comboBoxToJsonString("license", ui->LicenseComboBox);
  comboBoxToJsonString("status", ui->DevStatusComboBox);

  issuesMapToJsonIssues();

  rapidjson::StringBuffer Buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> Writer(Buffer);
  Doc.Accept(Writer);

  QTextStream Str(&File);
  Str << QString::fromUtf8(Buffer.GetString());
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::lineEditToJsonStringArray(const QString& Key, QLineEdit* LineEdit)
{
  QStringList Values = LineEdit->text().split(",");

  rapidjson::Value JsonValue(rapidjson::kArrayType);
  rapidjson::Document::AllocatorType& Alloc = Doc.GetAllocator();

  for (const auto& Val : Values)
  {
    rapidjson::Value v(Val.toStdString().c_str(), Alloc);
    JsonValue.PushBack(v, Alloc);
  }

  rapidjson::Value::MemberIterator it = Doc.FindMember(Key.toStdString().c_str());

  if (it == Doc.MemberEnd())
  {
    Doc.AddMember(rapidjson::Value(Key.toStdString().c_str(), Alloc), JsonValue, Alloc);
  }
  else
  {
    rapidjson::Value& v = it->value;
    if (v.IsArray())
    {
      v.Clear();
      v = JsonValue;
    }
  }
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::comboBoxToJsonString(const QString& Key, QComboBox* Combo)
{
  QString Value = Combo->currentText();

  rapidjson::Document::AllocatorType& Alloc = Doc.GetAllocator();

  rapidjson::Value JsonValue(Value.toStdString().c_str(), Alloc);

  rapidjson::Value::MemberIterator it = Doc.FindMember(Key.toStdString().c_str());

  if (it == Doc.MemberEnd())
    Doc.AddMember(rapidjson::Value(Key.toStdString().c_str(), Alloc), JsonValue, Alloc);
  else
    it->value = JsonValue;
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::issuesMapToJsonIssues()
{
  rapidjson::Value JsonIssuesObject(rapidjson::kObjectType);
  rapidjson::Document::AllocatorType& Alloc = Doc.GetAllocator();

  for (const auto& Issue : m_IssuesByID.values())
  {
    rapidjson::Value JsonIssueObject(rapidjson::kObjectType);


    rapidjson::Value TitleValue(Issue.m_Title.toStdString().c_str(), Alloc);
    JsonIssueObject.AddMember(rapidjson::Value("title", Alloc), TitleValue, Alloc);

    rapidjson::Value CreatorValue(Issue.m_Creator.toStdString().c_str(), Alloc);
    JsonIssueObject.AddMember(rapidjson::Value("creator", Alloc), CreatorValue, Alloc);

    rapidjson::Value DateValue(Issue.m_Date.toString("yyyy-MM-dd").toStdString().c_str(), Alloc);
    JsonIssueObject.AddMember(rapidjson::Value("date", Alloc), DateValue, Alloc);

    rapidjson::Value TypeValue(Issue.m_Type.toStdString().c_str(), Alloc);
    JsonIssueObject.AddMember(rapidjson::Value("type", Alloc), TypeValue, Alloc);

    rapidjson::Value StateValue(Issue.m_State.toStdString().c_str(), Alloc);
    JsonIssueObject.AddMember(rapidjson::Value("state", Alloc), StateValue, Alloc);

    rapidjson::Value DescValue(Issue.m_Description.toStdString().c_str(), Alloc);
    JsonIssueObject.AddMember(rapidjson::Value("description", Alloc), DescValue, Alloc);

    rapidjson::Value UrgValue(Issue.m_Urgency.toStdString().c_str(), Alloc);
    JsonIssueObject.AddMember(rapidjson::Value("urgency", Alloc), UrgValue, Alloc);


    JsonIssuesObject.AddMember(rapidjson::Value(Issue.m_ID.toStdString().c_str(), Alloc), JsonIssueObject, Alloc);
  }


  rapidjson::Value::MemberIterator it = Doc.FindMember("issues");

  if (it == Doc.MemberEnd())
    Doc.AddMember(rapidjson::Value("issues", Alloc), JsonIssuesObject, Alloc);
  else
    it->value = JsonIssuesObject;
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::updateContent()
{
  QFile File(m_FilePath);

  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              QString("Cannot open file %1").arg(m_FilePath).toStdString());

  Doc.Parse(QTextStream(&File).readAll().toStdString().c_str());

  if (Doc.HasParseError())
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION,
        QString("Error while parsing json file %1").arg(m_FilePath).toStdString());

  if (!Doc.IsObject())
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              QString("Error in json file %1").arg(m_FilePath).toStdString());


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

  rapidjson::Value::MemberIterator it = Doc.FindMember(Key.toStdString().c_str());

  if (it != Doc.MemberEnd())
  {
    rapidjson::Value& JsonValue = it->value;
    if (JsonValue.IsArray())
    {
      for (unsigned int i = 0; i < JsonValue.Size(); i++)
      {
        if (JsonValue[i].IsString())
          Values << JsonValue[i].GetString();
      }
    }
  }

  LineEdit->setText(Values.join(","));
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::jsonStringToComboBox(const QString& Key, QComboBox* Combo)
{
  rapidjson::Value::MemberIterator it = Doc.FindMember(Key.toStdString().c_str());

  if (it != Doc.MemberEnd())
  {
    rapidjson::Value& JsonValue = it->value;
    if (JsonValue.IsString())
    {
      QString Str = QString::fromUtf8(JsonValue.GetString());
      int Index = Combo->findText(Str, Qt::MatchFixedString);
      if (Index > -1)
        Combo->setCurrentIndex(Index);
      else if (Combo->isEditable())
        Combo->setEditText(Str);
    }
  }
}


// =====================================================================
// =====================================================================


void WareshubJsonEditor::jsonIssuesToIssuesMap()
{
  QStringList Values;

  rapidjson::Value::MemberIterator it = Doc.FindMember("issues");

  if (it != Doc.MemberEnd())
  {
    rapidjson::Value& IssuesValue = it->value;

    if (IssuesValue.IsObject())
    {
      for (rapidjson::Value::MemberIterator itr = IssuesValue.MemberBegin();
          itr != IssuesValue.MemberEnd(); ++itr)
      {
        QString ID = QString::fromUtf8(itr->name.GetString());

        rapidjson::Value& IssueContentValue = itr->value;

        if (IssueContentValue.IsObject())
        {
          WareshubIssueDialog::Issue I;

          rapidjson::Value::MemberIterator itI;

          itI = IssueContentValue.FindMember("title");
          if (itI != IssueContentValue.MemberEnd())
            I.m_Title = itI->value.GetString();

          itI = IssueContentValue.FindMember("creator");
          if (itI != IssueContentValue.MemberEnd())
            I.m_Creator = itI->value.GetString();

          itI = IssueContentValue.FindMember("date");
          if (itI != IssueContentValue.MemberEnd())
            I.m_Date = QDate::fromString(itI->value.GetString(), "yyyy-MM-dd");

          itI = IssueContentValue.FindMember("type");
          if (itI != IssueContentValue.MemberEnd())
            I.m_Type = itI->value.GetString();

          itI = IssueContentValue.FindMember("state");
          if (itI != IssueContentValue.MemberEnd())
            I.m_State = itI->value.GetString();

          itI = IssueContentValue.FindMember("description");
          if (itI != IssueContentValue.MemberEnd())
            I.m_Description = itI->value.GetString();

          itI = IssueContentValue.FindMember("urgency");
          if (itI != IssueContentValue.MemberEnd())
          {
            I.m_Urgency = itI->value.GetString();
            I.m_Urgency.replace("normal", "medium");
          }

          I.m_ID = ID;

          m_IssuesByID[ID] = I;
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
      IDItem->setData(Qt::DisplayRole, IDInt);
    else
      IDItem->setData(Qt::DisplayRole, ID);
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
          IDItem->setData(Qt::DisplayRole, IDInt);
        else
          IDItem->setData(Qt::DisplayRole, I.m_ID);

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
      IDItem->setData(Qt::DisplayRole, IDInt);
    else
      IDItem->setData(Qt::DisplayRole, I.m_ID);
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
