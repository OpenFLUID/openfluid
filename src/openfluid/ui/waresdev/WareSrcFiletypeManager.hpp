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
 @file WareSrcFiletypeManager.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WARESRCFILETYPEMANAGER_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCFILETYPEMANAGER_HPP__


#include <QTextCharFormat>
#include <QMap>
#if (QT_VERSION_MAJOR < 6)
#include <QRegExp>
#else
#include <QRegularExpression>
#endif

#include <openfluid/utils/SingletonMacros.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class WareSrcFiletype;


class OPENFLUID_API WareSrcFiletypeManager
{

  OPENFLUID_SINGLETON_DEFINITION(WareSrcFiletypeManager)

  public:

    struct HighlightingRule
    {
      QString StyleName;

#if (QT_VERSION_MAJOR < 6)
      QRegExp Pattern;

      QRegExp EndPattern;
#else
      QRegularExpression Pattern;

      QRegularExpression EndPattern;
#endif

      QTextCharFormat Format;

      HighlightingRule()
      { }

#if (QT_VERSION_MAJOR < 6)
      HighlightingRule(const QString& AStyleName, QRegExp APattern, QTextCharFormat AFormat) :
        StyleName(AStyleName), Pattern(APattern), EndPattern(QRegExp()), Format(AFormat)
      { }

      HighlightingRule(const QString& AStyleName, QRegExp ABeginPattern, QRegExp AnEndPattern,
                       QTextCharFormat AFormat) :
        StyleName(AStyleName), Pattern(ABeginPattern), EndPattern(AnEndPattern), Format(AFormat)
      { }
#else
      HighlightingRule(const QString& AStyleName, QRegularExpression APattern, QTextCharFormat AFormat) :
        StyleName(AStyleName), Pattern(APattern), EndPattern(QRegularExpression()), Format(AFormat)
      { }

      HighlightingRule(const QString& AStyleName, QRegularExpression ABeginPattern, QRegularExpression AnEndPattern,
                       QTextCharFormat AFormat) :
        StyleName(AStyleName), Pattern(ABeginPattern), EndPattern(AnEndPattern), Format(AFormat)
      { }
#endif
    };

    typedef QVector<HighlightingRule> HighlightingRules_t;


  private:

    struct WareSrcFiletype
    {
      QString m_Extensions;

      QString m_LangCode;

      QString m_IconPath;

      QString m_HlFilename;

      QString m_CompFilename;

      WareSrcFiletypeManager::HighlightingRules_t m_HlRules;
    };

    QMap<QString, QTextCharFormat> m_Formats;

    QMap<QString, QString> m_IconsByFileExtensionList;

    QMap<QString, WareSrcFiletype> m_WareSrcFiletypes;

    WareSrcFiletypeManager();

    ~WareSrcFiletypeManager();

    void initializeFileTypes();

    /**
      @throw openfluid::base::FrameworkException
    */
    HighlightingRules_t parseSyntaxFile(const QString& FilePath);


  public:

    void updateStyles();

    QMap<QString, QString> getIconsByFileExtensionList() const;

    HighlightingRules_t getHighlightingRules(const QString& FilePath) const;

    QString getFileType(const QString& FilePath) const;

    QString getFileLanguage(const QString& FilePath) const;

};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESRCFILETYPEMANAGER_HPP__ */
