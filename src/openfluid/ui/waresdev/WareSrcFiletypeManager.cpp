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
 @file WareSrcFiletypeManager.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <QDir>
#include <QFile>
#include <QDomDocument>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcFiletypeManager* WareSrcFiletypeManager::mp_Instance = 0;

// =====================================================================
// =====================================================================


WareSrcFiletypeManager::WareSrcFiletypeManager()
{
  openfluid::base::PreferencesManager::SyntaxHighlightingRules_t Rules = openfluid::base::PreferencesManager::instance()
      ->getSyntaxHighlightingRules();

  for (openfluid::base::PreferencesManager::SyntaxHighlightingRules_t::iterator it = Rules.begin(); it != Rules.end();
      ++it)
  {
    QString StyleName = it.key();

    QColor Color(it.value().m_Color);
    if (Color.isValid())
      m_Formats[StyleName].setForeground(Color);

    foreach(QString Decoration,it.value().m_Decoration){
    if(Decoration == "bold")
    m_Formats[StyleName].setFontWeight(QFont::Bold);
    else if(Decoration == "italic")
    m_Formats[StyleName].setFontItalic(true);
    else if(Decoration == "underline")
    m_Formats[StyleName].setFontUnderline(true);
    else if(Decoration == "strike-through")
    m_Formats[StyleName].setFontStrikeOut(true);
  }
}

  QDir WaresdevDir(
      QString("%1/%2").arg(QString::fromStdString(openfluid::base::RuntimeEnvironment::instance()->getInstallPrefix()))
          .arg(QString::fromStdString(openfluid::config::SHARE_WARESDEV_INSTALL_PATH)));

  parseFiletypeFile(WaresdevDir.filePath("filetypes.ofdev.xml"));

  /*
   * TODO Avoid the multiple parsing of the same file,
   * that happens if several file types have the same syntax file and/or same completion file
   */
  for (QMap<QString, WareSrcFiletype>::iterator it = m_WareSrcFiletypes.begin(); it != m_WareSrcFiletypes.end(); ++it)
  {
    if (!it.value().m_HlFilename.isEmpty())
      it.value().m_HlRules = parseSyntaxFile(WaresdevDir.filePath(it.value().m_HlFilename));
    if (!it.value().m_CompFilename.isEmpty())
      it.value().m_CompRules = parseCompletionFile(WaresdevDir.filePath(it.value().m_CompFilename));
  }

}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::~WareSrcFiletypeManager()
{

}


// =====================================================================
// =====================================================================


QDomElement WareSrcFiletypeManager::openWaresdevFile(const QString& FilePath)
{
  QDomDocument Doc;
  QFile File(FilePath);

  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
    throw openfluid::base::FrameworkException("WareSrcFiletypeManager::openWaresdevFile",
                                              QString("file not found: %1").arg(FilePath).toStdString());

  QString Msg;
  int Line, Col;
  if (!Doc.setContent(&File, &Msg, &Line, &Col))
  {
    File.close();
    throw openfluid::base::FrameworkException(
        "WareSrcFiletypeManager::openWaresdevFile",
        QString("error in file: %1 (line %2, column %3").arg(Msg).arg(Line).arg(Col).toStdString());
  }
  File.close();

  QDomElement Elem = Doc.documentElement();

  if (Elem.tagName() != "openfluid")
    throw openfluid::base::FrameworkException("WareSrcFiletypeManager::openWaresdevFile",
                                              "file not well formed (missing 'openfluid' tag)");

  Elem = Elem.firstChildElement();
  if (Elem.tagName() != "waresdev")
    throw openfluid::base::FrameworkException("WareSrcFiletypeManager::openWaresdevFile",
                                              "file not well formed (missing 'waresdev' tag)");

  return Elem;
}


// =====================================================================
// =====================================================================


void WareSrcFiletypeManager::parseFiletypeFile(const QString& FilePath)
{
  QDomElement Elem = openWaresdevFile(FilePath);

  Elem = Elem.firstChildElement();
  if (Elem.tagName() != "filetypes")
    throw openfluid::base::FrameworkException("WareSrcFiletypeManager::parseFiletypeFile",
                                              "language file not well formed (missing 'filetypes' tag)");

  for (QDomElement TypeElem = Elem.firstChildElement(); !TypeElem.isNull() && TypeElem.tagName() == "filetype";
      TypeElem = TypeElem.nextSiblingElement())
  {
    QString TypeName = TypeElem.attribute("name");
    if (TypeName.isEmpty())
      throw openfluid::base::FrameworkException(
          "WareSrcFiletypeManager::parseFiletypeFile",
          "language file not well formed ('filetype' tag has no 'name' attribute)");

    WareSrcFiletype Type;

    QDomNode ExtNode = TypeElem.namedItem("extensions");
    if (ExtNode.isNull() || !ExtNode.isElement())
      throw openfluid::base::FrameworkException("WareSrcFiletypeManager::parseFiletypeFile",
                                                "language file not well formed (missing 'extensions' tag)");

    Type.m_Extensions = ExtNode.toElement().text();

    QString IconPath = TypeElem.attribute("icon");
    if (!IconPath.isEmpty())
    {
      Type.m_IconPath = IconPath;
      m_IconsByFileExtensionList[Type.m_Extensions] = IconPath;
    }

    for (QDomElement TypeInfoElem = TypeElem.firstChildElement(); !TypeInfoElem.isNull();
        TypeInfoElem = TypeInfoElem.nextSiblingElement())
    {
      if (TypeInfoElem.tagName() == "edition" && TypeInfoElem.attribute("internal") == "yes")
      {
        for (QDomElement EditionElem = TypeInfoElem.firstChildElement(); !EditionElem.isNull(); EditionElem =
            EditionElem.nextSiblingElement())
        {
          if (EditionElem.tagName() == "highlighting")
            Type.m_HlFilename = EditionElem.attribute("specsfile");
          else if (EditionElem.tagName() == "completion")
            Type.m_CompFilename = EditionElem.attribute("specsfile");
        }

      }
    }

    m_WareSrcFiletypes[TypeName] = Type;
  }

}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::HighlightingRules_t WareSrcFiletypeManager::parseSyntaxFile(const QString& FilePath)
{
  QDomElement Elem = openWaresdevFile(FilePath);

  // TODO remove useless 'language' tag
  Elem = Elem.firstChildElement();
//  if (Elem.tagName() != "language" || Elem.attribute("name") != FileTypeName)
//    throw openfluid::base::FrameworkException(
//        "WareSrcFiletypeManager::parseSyntaxFile",
//        QString(
//            "syntax file not well formed (missing 'language' tag or language name is not '%1')")
//            .arg(FileTypeName).toStdString());

  Elem = Elem.firstChildElement();
  if (Elem.tagName() != "highlighting")
    throw openfluid::base::FrameworkException("WareSrcFiletypeManager::parseSyntaxFile",
                                              "no 'highlighting' tag in syntax file");

  HighlightingRules_t Rules;

  for (QDomElement HlElem = Elem.firstChildElement(); !HlElem.isNull(); HlElem = HlElem.nextSiblingElement())
  {
    if (HlElem.tagName() == "list")
    {
      QString StyleName = HlElem.attribute("style");
      if (m_Formats.contains(StyleName))
      {
        QTextCharFormat Format = m_Formats.value(StyleName);

        QDomNodeList Items = HlElem.elementsByTagName("item");
        for (int i = 0; i < Items.size(); i++)
        {
          Rules.append(HighlightingRule(QRegExp(QString("\\b%1\\b").arg(Items.at(i).toElement().text())), Format));
        }
      }
    }
    else if (HlElem.tagName() == "rule")
    {
      QString StyleName = HlElem.attribute("style");
      if (m_Formats.contains(StyleName))
      {
        QTextCharFormat Format = m_Formats.value(StyleName);

        QDomNodeList Items = HlElem.elementsByTagName("pattern");
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
            Rules.append(HighlightingRule(QRegExp(BeginPatternValue), QRegExp(EndPatternValue), Format));
          }
        }
      }
    }
  }

  return Rules;
}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::CompletionRulesByWareType_t WareSrcFiletypeManager::parseCompletionFile(const QString& FilePath)
{

  QDomElement Elem = openWaresdevFile(FilePath);

  // TODO remove useless 'language' tag
  Elem = Elem.firstChildElement();
  //  if (Elem.tagName() != "language" || Elem.attribute("name") != FileTypeName)
  //    throw openfluid::base::FrameworkException(
  //        "WareSrcFiletypeManager::parseCompletionFile",
  //        QString(
  //            "syntax file not well formed (missing 'language' tag or language name is not '%1')")
  //            .arg(FileTypeName).toStdString());


  CompletionRulesByWareType_t Rules;

  QDomNodeList Snippets = Elem.elementsByTagName("snippets");

  for (int i = 0; i < Snippets.size(); i++)
  {

    QDomElement SnippetElem = Snippets.at(i).toElement();

    QString Category = SnippetElem.attribute("category");
    QString Icon = SnippetElem.attribute("icon");

    if (Category.isEmpty())
      throw openfluid::base::FrameworkException("WareSrcFiletypeManager::parseCompletionFile",
                                                "missing category attribute in  <snippets> tag");

    QDomNodeList Items = SnippetElem.elementsByTagName("item");

    for (int j = 0; j < Items.size(); j++)
    {
      QDomElement ItemElem = Items.at(j).toElement();

      QString MenuPath = ItemElem.attribute("menupath");
      int LastSepPosition = MenuPath.lastIndexOf('/');

      QString MenuTree = Category+"/"+MenuPath.left(LastSepPosition);
      QString MenuTitle = MenuPath.section('/',-1);

      bool ForCompletion = ItemElem.attribute("completion") == "true";

      QString Content = ItemElem.text().trimmed().replace("\\n", "\n");


      CompletionRule Rule(MenuTree, MenuTitle, Content, ForCompletion, Icon);

      QString Context = ItemElem.attribute("context");
      if (Context.contains("anyware") || Context.isEmpty())
      {
        Rules[openfluid::waresdev::WareSrcManager::SIMULATOR].append(Rule);
        Rules[openfluid::waresdev::WareSrcManager::OBSERVER].append(Rule);
        Rules[openfluid::waresdev::WareSrcManager::BUILDEREXT].append(Rule);
      }
      else
      {
        if (Context.contains("simulator"))
          Rules[openfluid::waresdev::WareSrcManager::SIMULATOR].append(Rule);
        if (Context.contains("observer"))
          Rules[openfluid::waresdev::WareSrcManager::OBSERVER].append(Rule);
        if (Context.contains("builderext"))
          Rules[openfluid::waresdev::WareSrcManager::BUILDEREXT].append(Rule);
      }
    }
  }

  return Rules;
}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager* WareSrcFiletypeManager::instance()
{
  if (!mp_Instance)
    mp_Instance = new WareSrcFiletypeManager();

  return mp_Instance;
}


// =====================================================================
// =====================================================================


QString WareSrcFiletypeManager::getFileType(const QString& FileName)
{
  for (QMap<QString, WareSrcFiletype>::iterator it = m_WareSrcFiletypes.begin(); it != m_WareSrcFiletypes.end(); ++it)
  {
    if (QDir::match(it.value().m_Extensions, FileName))
      return it.key();
  }

  return "";
}


// =====================================================================
// =====================================================================


QMap<QString, QString> WareSrcFiletypeManager::getIconsByFileExtensionList()
{
  return m_IconsByFileExtensionList;
}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::HighlightingRules_t WareSrcFiletypeManager::getHighlightingRules(const QString& FilePath)
{
  QString FileType = getFileType(QFileInfo(FilePath).fileName());

  return m_WareSrcFiletypes.value(FileType, WareSrcFiletype()).m_HlRules;
}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::CompletionRules_t WareSrcFiletypeManager::getCompletionRules(const QString& FilePath)
{
  QString FileType = getFileType(QFileInfo(FilePath).fileName());

  openfluid::waresdev::WareSrcManager::WareType WareType = openfluid::waresdev::WareSrcManager::instance()->getPathInfo(
      FilePath).m_WareType;

  return m_WareSrcFiletypes.value(FileType, WareSrcFiletype()).m_CompRules.value(WareType, QVector<CompletionRule>());
}


// =====================================================================
// =====================================================================


} } }  // namespaces
