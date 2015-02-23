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
 */


#ifndef __OPENFLUID_UIWARESDEV_WARESRCFILETYPEMANAGER_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCFILETYPEMANAGER_HPP__

#include <openfluid/dllexport.hpp>

#include <QTextCharFormat>
#include <QMap>
#include <QRegExp>
#include <QDomElement>
#include <QFile>

#include <openfluid/waresdev/WareSrcManager.hpp>


namespace openfluid { namespace ui { namespace waresdev {

class WareSrcFiletype;

class OPENFLUID_API WareSrcFiletypeManager
{
  public:

    struct HighlightingRule
    {
        QRegExp Pattern;
        QRegExp EndPattern;
        QTextCharFormat Format;
        HighlightingRule()
        {
        }
        HighlightingRule(QRegExp APattern, QTextCharFormat AFormat) :
            Pattern(APattern), EndPattern(QRegExp()), Format(AFormat)
        {
        }
        HighlightingRule(QRegExp ABeginPattern, QRegExp AnEndPattern, QTextCharFormat AFormat) :
            Pattern(ABeginPattern), EndPattern(AnEndPattern), Format(AFormat)
        {
        }
    };

    typedef QVector<HighlightingRule> HighlightingRules_t;

    struct CompletionRule
    {
        QString MenuPath;
        QString Title;
        QString Content;
        bool IsForCompletion;
        QString IconPath;
        CompletionRule() :
            IsForCompletion(false)
        {
        }
        CompletionRule(const QString& AMenuPath, const QString& ATitle, const QString& AContent, bool AIsForCompletion,
                       const QString& AIconPath) :
            MenuPath(AMenuPath), Title(ATitle), Content(AContent), IsForCompletion(AIsForCompletion),
            IconPath(AIconPath)
        {
        }
    };

    typedef QVector<CompletionRule> CompletionRules_t;
    typedef QMap<openfluid::waresdev::WareSrcManager::WareType, CompletionRules_t> CompletionRulesByWareType_t;

  private:

    struct WareSrcFiletype
    {
        QString m_Extensions;

        QString m_IconPath;

        QString m_HlFilename;
        QString m_CompFilename;

        WareSrcFiletypeManager::HighlightingRules_t m_HlRules;
        WareSrcFiletypeManager::CompletionRulesByWareType_t m_CompRules;
    };

    static WareSrcFiletypeManager* mp_Instance;

    QMap<QString, QTextCharFormat> m_Formats;

    QMap<QString, QString> m_IconsByFileExtensionList;

    QMap<QString, WareSrcFiletype> m_WareSrcFiletypes;

    WareSrcFiletypeManager();

    /**
     * @throw openfluid::base::FrameworkException
     */
    QDomElement openWaresdevFile(const QString& FilePath);

    /**
     * @throw openfluid::base::FrameworkException
     */
    void parseFiletypeFile(const QString& FilePath);

    /**
     * @throw openfluid::base::FrameworkException
     */
    HighlightingRules_t parseSyntaxFile(const QString& FilePath);

    /**
     * @throw openfluid::base::FrameworkException
     */
    CompletionRulesByWareType_t parseCompletionFile(const QString& FilePath);

    QString getFileType(const QString& FileName);

  public:

    ~WareSrcFiletypeManager();

    static WareSrcFiletypeManager* instance();

    QMap<QString, QString> getIconsByFileExtensionList();

    HighlightingRules_t getHighlightingRules(const QString& FilePath);

    CompletionRules_t getCompletionRules(const QString& FilePath);
};


// =====================================================================
// =====================================================================


} } }  // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESRCFILETYPEMANAGER_HPP__ */
