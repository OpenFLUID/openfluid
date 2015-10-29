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
  @file WaresTranslationsRegistry.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <QApplication>
#include <QFileInfo>

#include <openfluid/base/PreferencesManager.hpp>

#include "WaresTranslationsRegistry.hpp"


// =====================================================================
// =====================================================================


WaresTranslationsRegistry::WaresTranslationsRegistry()
{
  m_LangCode = openfluid::base::PreferencesManager::instance()->getLang();
}


// =====================================================================
// =====================================================================


WaresTranslationsRegistry::~WaresTranslationsRegistry()
{

}


// =====================================================================
// =====================================================================


QTranslator* WaresTranslationsRegistry::tryLoadWareTranslation(const QString& WareFullPath)
{
  if (m_LangCode.isEmpty() || m_LangCode == "default")
    return nullptr;


  QString LangFile;

  QFileInfo WareFileInfo(WareFullPath);
  LangFile = WareFileInfo.absolutePath()+"/"+WareFileInfo.completeBaseName()+"-"+m_LangCode+".qm";


  if (!LangFile.isEmpty() && QFile::exists(LangFile))
  {
    if (m_TranslatorsByFile.find(LangFile) == m_TranslatorsByFile.end())
    {
      QTranslator* Translator = new QTranslator(QApplication::instance());
      Translator->load(LangFile);
      QApplication::installTranslator(Translator);
      m_TranslatorsByFile[LangFile].reset(Translator);
    }

    return m_TranslatorsByFile[LangFile].get();
  }

  return nullptr;
}


// =====================================================================
// =====================================================================


QString WaresTranslationsRegistry::tryTranslate(const QString& WareFullPath,
                                                const QString& Context, const QString& Text)
{
  QTranslator* ExtTranslator = nullptr;

  ExtTranslator = tryLoadWareTranslation(WareFullPath);

  if (ExtTranslator)
  {
    QString TSText = ExtTranslator->translate(Context.toStdString().c_str(),Text.toStdString().c_str());
    if (!TSText.isEmpty())
      return TSText;
  }
  return Text;
}
