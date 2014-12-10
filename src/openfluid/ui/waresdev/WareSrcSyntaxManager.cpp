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
 \file WareSrcSyntaxManager.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcSyntaxManager.hpp>

#include <QDir>
#include <QFile>
#include <QDomDocument>

#include <openfluid/base/RuntimeEnv.hpp>

namespace openfluid { namespace ui { namespace waresdev {


WareSrcSyntaxManager* WareSrcSyntaxManager::mp_Instance = 0;


// =====================================================================
// =====================================================================


WareSrcSyntaxManager::WareSrcSyntaxManager()
{
  //TODO get formats from conf file

  m_Formats["keyword"].setForeground(QColor("#0000FF"));
  m_Formats["keyword"].setFontWeight(QFont::Bold);

  m_Formats["datatype"].setFontWeight(QFont::Bold);

  m_Formats["preprocessor"].setForeground(QColor("#FF00FF"));
  m_Formats["preprocessor"].setFontWeight(QFont::Bold);

  m_Formats["deprecated"].setForeground(QColor("#0000FF"));
  m_Formats["deprecated"].setFontWeight(QFont::Bold);
  m_Formats["deprecated"].setFontStrikeOut(true);

  m_Formats["quoted"].setForeground(QColor("#FF0000"));

  m_Formats["function"].setForeground(QColor("#0000FF"));

  m_Formats["comment"].setForeground(QColor("#00FF00"));
  m_Formats["comment"].setFontItalic(true);
}


// =====================================================================
// =====================================================================


WareSrcSyntaxManager::~WareSrcSyntaxManager()
{

}


// =====================================================================
// =====================================================================


QString WareSrcSyntaxManager::parseSyntaxFile(const QString& FileTypeName)
{
  QString SyntaxFile =
      QString("%1/share/openfluid/langspecs/%2.ofdev.xml").arg(
          QString::fromStdString(
              openfluid::base::RuntimeEnvironment::getInstance()
                  ->getInstallPrefix())).arg(FileTypeName);

  QDomDocument Doc;
  QFile File(SyntaxFile);

  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
    return QString("syntax file not found: %1").arg(SyntaxFile);

  QString Msg;
  int Line, Col;
  if (!Doc.setContent(&File, &Msg, &Line, &Col))
  {
    File.close();
    return QString("error in syntax file: %1 (line %2, column %3").arg(Msg).arg(
        Line).arg(Col);
  }
  File.close();

  QDomElement Elem = Doc.documentElement();

  if (Elem.tagName() != "openfluid")
    return "syntax file not well formed (missing 'openfluid' tag)";

  Elem = Elem.firstChildElement();
  if (Elem.tagName() != "waresdev")
    return "syntax file not well formed (missing 'waresdev' tag)";

  Elem = Elem.firstChildElement();
  if (Elem.tagName() != "language" || Elem.attribute("name") != FileTypeName)
    return QString(
        "syntax file not well formed (missing 'language' tag or language name is not '%1')")
        .arg(FileTypeName);

  Elem = Elem.firstChildElement();
  if (Elem.tagName() != "highlighting")
    return "no 'highlighting' tag in syntax file";

  HighlightingRules_t Rules;

  for (QDomElement E = Elem.firstChildElement(); !E.isNull();
      E = E.nextSiblingElement())
  {
    if (E.tagName() == "list")
    {
      QString StyleName = E.attribute("style");
      if (m_Formats.contains(StyleName))
      {
        QTextCharFormat Format = m_Formats.value(StyleName);

        QDomNodeList Items = E.elementsByTagName("item");
        for (int i = 0; i < Items.size(); i++)
        {
          Rules.append(
              HighlightingRule(
                  QRegExp(
                      QString("\\b%1\\b").arg(Items.at(i).toElement().text())),
                  Format));
        }
      }
    }
    else if (E.tagName() == "rule")
    {
      QString StyleName = E.attribute("style");
      if (m_Formats.contains(StyleName))
      {
        QTextCharFormat Format = m_Formats.value(StyleName);

        QDomNodeList Items = E.elementsByTagName("pattern");
        for (int i = 0; i < Items.size(); i++)
        {
          QDomElement Pattern = Items.at(i).toElement();

          // 'value' must be set and different of ''
          QString SimplePatternValue = Pattern.attribute("value", "");
          QString BeginPatternValue = Pattern.attribute("start", "");
          QString EndPatternValue = Pattern.attribute("end", "");
          if (!SimplePatternValue.isEmpty())
          {
            Rules.append(HighlightingRule(QRegExp(SimplePatternValue), Format));
          }
          else if (!BeginPatternValue.isEmpty() && !EndPatternValue.isEmpty())
          {
            Rules.append(
                HighlightingRule(QRegExp(BeginPatternValue),
                                 QRegExp(EndPatternValue), Format));
          }
        }
      }
    }
  }

  m_HighlightingRulesByFileType[FileTypeName] = Rules;

  return "";
}


// =====================================================================
// =====================================================================


WareSrcSyntaxManager* WareSrcSyntaxManager::getInstance()
{
  if (!mp_Instance)
    mp_Instance = new WareSrcSyntaxManager();

  return mp_Instance;
}


// =====================================================================
// =====================================================================


WareSrcSyntaxManager::HighlightingRules_t WareSrcSyntaxManager::getHighlightingRules(
    const QString& FileTypeName)
{
  if (!m_HighlightingRulesByFileType.contains(FileTypeName))
  {
    QString Msg = parseSyntaxFile(FileTypeName);

    if (!Msg.isEmpty())
      std::cout << qPrintable(Msg) << std::endl;
  }

  return m_HighlightingRulesByFileType.value(FileTypeName,
                                             HighlightingRules_t());
}


// =====================================================================
// =====================================================================


} } } // namespaces
