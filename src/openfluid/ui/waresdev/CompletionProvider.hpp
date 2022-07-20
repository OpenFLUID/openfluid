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
  @file CompletionProvider.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_COMPLETIONPROVIDER_HPP__
#define __OPENFLUID_UIWARESDEV_COMPLETIONPROVIDER_HPP__


#include <QObject>
#include <QString>
#include <QMap>
#include <QVector>
#include <QFlags>
#include <QStringList>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/utils/SingletonMacros.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API CompletionProvider : public QObject
{
  Q_OBJECT

  OPENFLUID_SINGLETON_DEFINITION(CompletionProvider)


  public:

    enum class Origin
    {
      OPENFLUID,
      CPP,
      UNDEFINED
    };
    
    enum class ContextFlags
    {
       ANYWARE = 1 << 0,
       SIMULATOR = 1 << 2,
       OBSERVER = 1 << 3,
       BUILDEREXT = 1 << 4
    };
    Q_DECLARE_FLAGS(Context, ContextFlags)

    struct Rule
    {
      QString ProgLangCode;

      Origin Orig;

      Context DevContext;

      QStringList MenuPath;

      QString Title;

      QString Content;


      Rule() = default;

      Rule(QString LangCode, Origin O, Context Ctxt, 
           const QStringList& M, const QString& T, const QString& C) :
        ProgLangCode(LangCode), Orig(O), DevContext(Ctxt), MenuPath(M), Title(T), Content(C)
      { }

    };

    using Rules = QVector<Rule>;

    using RulesByWareType = QMap<openfluid::ware::WareType,Rules>;

    using RulesByLanguageByWareType = QMap<QString,RulesByWareType>;


  private:

    RulesByLanguageByWareType m_Rules;

    Rules m_NoRule;

    void addRule(const Rule& R);

    void initializeCppRules();


  public:

    CompletionProvider();

    virtual ~CompletionProvider()
    { }

    const Rules& getRules(QString LangCode, openfluid::ware::WareType Wtype);
};


Q_DECLARE_OPERATORS_FOR_FLAGS(CompletionProvider::Context)


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_COMPLETIONPROVIDER_HPP__ */
