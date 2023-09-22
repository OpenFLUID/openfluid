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
 @file WareSrcFiletypeManager_TEST.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_WareSrcFiletypeManager

#ifndef QT_VERSION_MAJOR
#pragma message "Qt version not found in source"
#else
#pragma message "Qt version found in source"
#endif

#if (QT_VERSION_MAJOR < 6)
#  pragma message("QT_VERSION_MAJOR < 6")


#  include <QRegExp>
#else
#  pragma message("QT_VERSION_MAJOR >= 6?")


#  include <QRegularExpression>  
#endif

#include <boost/test/unit_test.hpp>

#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(parseSyntaxFile)
{
  openfluid::ui::waresdev::WareSrcFiletypeManager* Manager =
      openfluid::ui::waresdev::WareSrcFiletypeManager::instance();

  BOOST_CHECK(Manager->getHighlightingRules("file.wrongtype").isEmpty());
  BOOST_CHECK(!Manager->getHighlightingRules("file.cpp").isEmpty());
}


// =====================================================================
// =====================================================================


bool hasMatch(QString Pattern, QString Content)
{
#if (QT_VERSION_MAJOR < 6)
  return QRegExp(Pattern).indexIn(Content) != -1;
#else
  return QRegularExpression(Pattern, QRegularExpression::DotMatchesEverythingOption).globalMatch(Content).hasNext();
#endif  
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(checkRegExp)
{
  BOOST_CHECK(hasMatch("\\bint\\b", "bla int bla"));
  BOOST_CHECK(!hasMatch("\\bit\\b", "bla int bla"));

  QString preproc("^\\s*#\\s*\\w+");  //<pattern value="^\s*#\s*\w+" />
  BOOST_CHECK(hasMatch(preproc, "#include bla"));
  BOOST_CHECK(hasMatch(preproc, "#include bla"));
  BOOST_CHECK(hasMatch(preproc, "# include bla"));

  // <pattern value="\bOPENFLUID_\w+\b" />  <pattern value="\bOpenFLUID_\w+\b" />
  BOOST_CHECK(hasMatch("\\bOPENFLUID_\\w+\\b", " OPENFLUID_GET(bla) "));

  QString simpleComment("//.*$");  // <pattern value="//.*$" />
  BOOST_CHECK(hasMatch(simpleComment, "// comment"));
  BOOST_CHECK(hasMatch(simpleComment, "bla //comment "));
  BOOST_CHECK(hasMatch(simpleComment, "//* comment"));
  BOOST_CHECK(!hasMatch(simpleComment, "bla / bla"));
  BOOST_CHECK(!hasMatch(simpleComment, "bla/bla"));

  QString multiComment("((^|[^/])/{1}\\*).*(\\*/)");  // <pattern start="(^|[^/])/{1}\*" end="\*/" />
  BOOST_CHECK(hasMatch(multiComment, "/* comment */"));
  BOOST_CHECK(hasMatch(multiComment, "/* comment\n next line */"));
  BOOST_CHECK(!hasMatch(multiComment, "//* comment */"));
  BOOST_CHECK(!hasMatch(multiComment, "// comment "));
  BOOST_CHECK(!hasMatch(multiComment, "/ bla "));

  BOOST_CHECK(hasMatch("\"[^\"]*\"", " bla \"text text \""));  // <pattern value='\"[^\"]*\"' />
  BOOST_CHECK(hasMatch("\'[^\']*\'", " bla 'text text '"));  // <pattern value="\'[^\']*\'" />

  QString functionRegex("\\w+\\s*(?=\\()");  //<pattern value="\w+\s*(?=\()" />
  BOOST_CHECK(hasMatch(functionRegex, "func()"));
  BOOST_CHECK(hasMatch(functionRegex, "func(param)"));
  BOOST_CHECK(hasMatch(functionRegex, "func ( param)"));
  BOOST_CHECK(!hasMatch(functionRegex, "2*(3+5)"));
}

