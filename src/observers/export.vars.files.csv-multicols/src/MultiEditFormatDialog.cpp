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
  @file MultiEditFormatDialog.cpp

  @author Armel THONI <armel.thoni@inra.fr>
*/


#include <QPushButton>


#include <openfluid/ui/config.hpp>
#include <openfluid/tools/IDHelpers.hpp>

#include "ui_MultiEditFormatDialog.h"
#include "MultiEditFormatDialog.hpp"


MultiEditFormatDialog::MultiEditFormatDialog(const QStringList& ExistingFormats,QWidget* Parent):
  openfluid::ui::common::OpenFLUIDDialog(Parent),
  ui(new Ui::MultiEditFormatDialog), m_ExistingFormatsNames(ExistingFormats)
{
  ui->setupUi(this);

  // "required" placeholder
  QString PlaceholderStr = getPlaceholderRequired();
  ui->FormatNameEdit->setPlaceholderText(PlaceholderStr);
  ui->ColSepEdit->setPlaceholderText(PlaceholderStr);
  ui->CommentCharEdit->setPlaceholderText(PlaceholderStr);

  ui->ColSepEdit->setText(QString::fromStdString(m_Format.ColSeparator).replace("\t","\\t"));
  ui->CommentCharEdit->setText(QString::fromStdString(m_Format.CommentChar));
  ui->MissingValueEdit->setText(QString::fromStdString(m_Format.MissingValueString));

  // Columns names as data
  m_HeaderLabels << tr("Columns names as comments") << tr("Columns names as data") << tr("Full") << tr("None");
  m_HeaderCodes << MultiCSVFormat::HeaderType::ColnamesAsComment << MultiCSVFormat::HeaderType::ColnamesAsData \
                << MultiCSVFormat::HeaderType::Full << MultiCSVFormat::HeaderType::None;

  m_DateLabels << "ISO" << tr("ISO extended") << tr("6 columns") << tr("Time index");
  m_DateCodes << "ISO" << "ISOEXT" << "6COLS" << "timeindex";
  m_FloatFormatsLabels << tr("Auto") << tr("Fixed") << tr("Scientific");
  m_FloatFormatsCodes  << "auto" << "fixed" << "scientific";

  ui->HeaderComboBox->addItems(m_HeaderLabels);
  ui->HeaderComboBox->setCurrentIndex(0);
  ui->DateComboBox->addItems(m_DateLabels);
  ui->DateComboBox->setCurrentIndex(0);
  ui->FloatFormatComboBox->addItems(m_FloatFormatsLabels);
  ui->FloatFormatComboBox->setCurrentIndex(0);

#if (QT_VERSION_MAJOR < 6)
  m_PreviewDateTimes = QList<openfluid::core::DateTime>::fromVector(
                                QVector<openfluid::core::DateTime>::fromStdVector(getPreviewDateTimes()));
  m_PreviewValues = QList<double>::fromVector(
                                QVector<double>::fromStdVector(getPreviewValues(true)));
#else
  const auto& PreviewDateTimes = getPreviewDateTimes();
  m_PreviewDateTimes = QList<openfluid::core::DateTime>(PreviewDateTimes.begin(), PreviewDateTimes.end());
  const auto& PreviewValues = getPreviewValues(true);
  m_PreviewValues = QList<double>(PreviewValues.begin(), PreviewValues.end());
#endif

  connect(ui->FormatNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->PredefDateRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->CustomDateRadioButton,SIGNAL(toggled(bool)),this,SLOT(checkGlobal()));
  connect(ui->CustomDateEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->HeaderComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(checkGlobal()));
  connect(ui->DateComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(checkGlobal()));
  connect(ui->FloatFormatComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(checkGlobal()));
  connect(ui->CommentCharEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->MissingValueEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->ColSepEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->PrecisionSpinBox,SIGNAL(valueChanged(int)),this,SLOT(checkGlobal()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

  checkGlobal();
}


// =====================================================================
// =====================================================================


MultiEditFormatDialog::~MultiEditFormatDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void MultiEditFormatDialog::checkGlobal()
{
  QString TmpColSep = ui->ColSepEdit->text().replace("\\t","\t");
  m_Format.ColSeparator = TmpColSep.toStdString();
  m_Format.CommentChar = ui->CommentCharEdit->text().toStdString();
  m_Format.MissingValueString = ui->MissingValueEdit->text().toStdString();
  m_Format.Header = m_HeaderCodes[ui->HeaderComboBox->currentIndex()];
  m_Format.Precision = ui->PrecisionSpinBox->value();
  m_Format.FloatFormat = m_FloatFormatsCodes[ui->FloatFormatComboBox->currentIndex()];
  if (ui->CustomDateRadioButton->isChecked())
  {
    m_Format.DateFormat = ui->CustomDateEdit->text().toStdString();
    m_Format.IsTimeIndexDateFormat = false;
  }
  else
  {
    m_Format.DateFormat = m_DateCodes[ui->DateComboBox->currentIndex()];
    m_Format.IsTimeIndexDateFormat = (m_Format.DateFormat == "timeindex");
  }

  updatePreview();

  ui->CustomDateEdit->setEnabled(ui->CustomDateRadioButton->isChecked());
  if (ui->CustomDateEdit->isEnabled())
  {
    ui->CustomDateEdit->setPlaceholderText(getPlaceholderRequired());
  }
  else
  {
    ui->CustomDateEdit->setPlaceholderText("");
  }

  bool InvalidName = ui->FormatNameEdit->text().isEmpty() ||
                    !openfluid::tools::isValidAlphaNumName(ui->FormatNameEdit->text().toStdString());

  bool ExistingFormat = m_ExistingFormatsNames.contains(ui->FormatNameEdit->text());

  if (InvalidName || ExistingFormat ||
      (ui->CustomDateEdit->isEnabled() && ui->CustomDateEdit->text().isEmpty()) ||
      ui->ColSepEdit->text().isEmpty() ||
      ui->CommentCharEdit->text().isEmpty())
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else
  {
    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
}


// =====================================================================
// =====================================================================


void MultiEditFormatDialog::initialize(const QString& Name, const QString& Header, const QString& ColSep,
                                 const QString& Date, const QString& Precision, const QString& FloatFormat, 
                                 const QString& CommentChar, const QString& MissingValueStr)
{
  ui->FormatNameEdit->setText(Name);
  ui->ColSepEdit->setText(ColSep);
  ui->PrecisionSpinBox->setValue(Precision.toLong());
  ui->CommentCharEdit->setText(CommentChar);
  ui->MissingValueEdit->setText(MissingValueStr);

  int FloatFormatIndex = m_FloatFormatsCodes.indexOf(FloatFormat.toStdString());
  if(FloatFormatIndex >= 0)
  {
    ui->FloatFormatComboBox->setCurrentIndex(FloatFormatIndex);
  }

  int HeaderIndex = m_HeaderCodes.indexOf(StrToHeaderType(Header.toStdString()));
  if (HeaderIndex >= 0)
  {
    ui->HeaderComboBox->setCurrentIndex(HeaderIndex);
  }

  int DateIndex = m_DateCodes.indexOf(Date.toStdString());
  if (DateIndex >= 0)
  {
    ui->PredefDateRadioButton->setChecked(true);
    ui->DateComboBox->setCurrentIndex(DateIndex);
  }
  else
  {
    ui->CustomDateRadioButton->setChecked(true);
    ui->CustomDateEdit->setText(Date);
  }

  checkGlobal();
}


// =====================================================================
// =====================================================================


void MultiEditFormatDialog::updatePreview()
{
  std::string FilePath = buildMultiColsFilename("/tmp/",CSV_FILES_EXT,"exset");
  std::ostringstream PreviewText;
  
  m_Format.adaptStreamFormat(PreviewText);

  PreviewText << buildMultiColsdHeader(m_Format,FilePath,"EU#76:exemple.var");

  for (int i=0; i<m_PreviewDateTimes.count();i++)
  {
    if (m_Format.IsTimeIndexDateFormat)
    {
      PreviewText << m_PreviewDateTimes[i].diffInSeconds(m_PreviewDateTimes[0]);
    }
    else
    {
      PreviewText << m_PreviewDateTimes[i].getAsString(StrToDateFormat(m_Format.DateFormat));
    }
    if (i < m_PreviewValues.size())
    {
      PreviewText << m_Format.ColSeparator << m_PreviewValues[i] << "\n";
    }
    else
    {
      PreviewText << m_Format.ColSeparator << m_Format.MissingValueString << "\n";
    }
  }

  ui->PreviewEdit->setText(QString::fromStdString(PreviewText.str()));
}


// =====================================================================
// =====================================================================


openfluid::ware::WareParams_t MultiEditFormatDialog::getFormatParams()
{
  std::string ParamsRoot = "format."+ui->FormatNameEdit->text().toStdString()+".";

  std::string DateStr;
  if (ui->PredefDateRadioButton->isChecked())
  {
    DateStr = m_DateCodes[ui->DateComboBox->currentIndex()];
  }
  else
  {
    DateStr = ui->CustomDateEdit->text().toStdString();
  }

  std::string PrecisionStr = ui->PrecisionSpinBox->cleanText().toStdString();
  std::string FloatFormatStr = m_FloatFormatsCodes[ui->FloatFormatComboBox->currentIndex()];
  std::string ColSepStr = ui->ColSepEdit->text().replace("\\t","\t").toStdString();
  std::string HeaderStr = HeaderTypeToStr(m_HeaderCodes[ui->HeaderComboBox->currentIndex()]);
  std::string CommentStr = ui->CommentCharEdit->text().toStdString();
  std::string MissingValueStr = ui->MissingValueEdit->text().toStdString();

  return openfluid::ware::WareParams_t({{ParamsRoot+"date",DateStr},
                                        {ParamsRoot+"precision",PrecisionStr},
                                        {ParamsRoot+"float-format",FloatFormatStr},
                                        {ParamsRoot+"colsep",ColSepStr},
                                        {ParamsRoot+"commentchar",CommentStr},
                                        {ParamsRoot+"header",HeaderStr},
                                        {ParamsRoot+"missingvalue",MissingValueStr}});
}
