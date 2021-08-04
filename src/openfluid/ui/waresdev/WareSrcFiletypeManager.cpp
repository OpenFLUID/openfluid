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
 @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <QDir>
#include <QFileInfo>


#include <openfluid/tools/TinyXML2Helpers.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace ui { namespace waresdev {


OPENFLUID_SINGLETON_INITIALIZATION(WareSrcFiletypeManager)


WareSrcFiletypeManager::WareSrcFiletypeManager()
{
  openfluid::base::Environment::init();

  updateStyles();

  initializeFileTypes();

  QDir WaresdevDir(
      QString("%1/%2").arg(QString::fromStdString(openfluid::base::Environment::getInstallPrefix()))
          .arg(QString::fromStdString(openfluid::config::SHARE_WARESDEV_INSTALL_PATH)));

  /*
   * TODO Avoid the multiple parsing of the same file,
   * that happens if several file types have the same syntax file and/or same completion file
   */
  for (QMap<QString, WareSrcFiletype>::iterator it = m_WareSrcFiletypes.begin(); it != m_WareSrcFiletypes.end(); ++it)
  {
    if (!it.value().m_HlFilename.isEmpty())
    {
      it.value().m_HlRules = parseSyntaxFile(WaresdevDir.filePath(it.value().m_HlFilename));
    }
  }
}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::~WareSrcFiletypeManager()
{

}


// =====================================================================
// =====================================================================


void WareSrcFiletypeManager::updateStyles()
{
  m_Formats.clear();

  auto Rules = openfluid::base::PreferencesManager::instance()->getWaresdevSyntaxHighlightingRules();

  for (const auto& R : Rules)
  {
    QString StyleName = QString::fromStdString(R.first);
    QColor Color = QString::fromStdString(R.second.Color);

    if (Color.isValid())
    {
      m_Formats[StyleName].setForeground(Color);
    }

    for (const auto& D : R.second.Decoration)
    {
      if (D == "bold")
      {
        m_Formats[StyleName].setFontWeight(QFont::Bold);
      }
      else if (D == "italic")
      {
        m_Formats[StyleName].setFontItalic(true);
      }
      else if (D == "underline")
      {
        m_Formats[StyleName].setFontUnderline(true);
      }
      else if (D == "strike-through")
      {
        m_Formats[StyleName].setFontStrikeOut(true);
      }
    }
  }

  for (QMap<QString, WareSrcFiletype>::iterator it = m_WareSrcFiletypes.begin() ; it != m_WareSrcFiletypes.end() ; ++it)
  {
    for (WareSrcFiletypeManager::HighlightingRules_t::iterator itt = it->m_HlRules.begin() ;
        itt != it->m_HlRules.end(); ++ itt)
      itt->Format = m_Formats.value(itt->StyleName, QTextCharFormat());
  }

}


// =====================================================================
// =====================================================================


void WareSrcFiletypeManager::initializeFileTypes()
{
  WareSrcFiletype FileType;

  FileType = WareSrcFiletype();  
  FileType.m_Extensions = "*.c++;*.cxx;*.cpp;*.cc;*.C;*.moc;*.c";
  FileType.m_LangCode = "cpp";
  FileType.m_IconPath = ":/ui/common/filetypes/cpp.png";
  FileType.m_HlFilename = "cpp.syntaxhl.ofdev.xml";
  m_WareSrcFiletypes["cpp"] = FileType;

  FileType = WareSrcFiletype();  
  FileType.m_Extensions = "*.h;*.hh;*.H;*.h++;*.hxx;*.hpp;*.hcc";
  FileType.m_LangCode = "cpp";
  FileType.m_IconPath = ":/ui/common/filetypes/hpp.png";
  FileType.m_HlFilename = "cpp.syntaxhl.ofdev.xml";
  m_WareSrcFiletypes["hpp"] = FileType;

  FileType = WareSrcFiletype();  
  FileType.m_Extensions = "CMakeLists.txt";
  FileType.m_LangCode = "cmake";
  FileType.m_IconPath = ":/ui/common/filetypes/cmakelists.png";
  FileType.m_HlFilename = "cmake.syntaxhl.ofdev.xml";
  m_WareSrcFiletypes["cmakelists"] = FileType;

  FileType = WareSrcFiletype();  
  FileType.m_Extensions = "CMake.in.config;*.cmake";
  FileType.m_LangCode = "cmake";
  FileType.m_IconPath = ":/ui/common/filetypes/cmake.png";
  FileType.m_HlFilename = "cmake.syntaxhl.ofdev.xml";
  m_WareSrcFiletypes["cmake"] = FileType;

  FileType = WareSrcFiletype();  
  FileType.m_Extensions = "*.f;*.F;*.for;*.FOR;*.f90;*.F90;*.fpp;*.FPP;*.f95;*.F95";
  FileType.m_LangCode = "fortran";
  FileType.m_IconPath = ":/ui/common/filetypes/fortran.png";
  FileType.m_HlFilename = "";
  m_WareSrcFiletypes["fortran"] = FileType;

  FileType = WareSrcFiletype();  
  FileType.m_Extensions = "*.tex;*.ltx;*.dtx;*.sty;*.cls;*.bbx;*.cbx;*.lbx;*.bib";
  FileType.m_LangCode = "latex";
  FileType.m_IconPath = "";
  FileType.m_HlFilename = "";
  m_WareSrcFiletypes["latex"] = FileType;

  FileType = WareSrcFiletype();  
  FileType.m_Extensions = "*.png;*.jpg;*.tif";
  FileType.m_LangCode = "";
  FileType.m_IconPath = "";
  FileType.m_HlFilename = "";
  m_WareSrcFiletypes["image"] = FileType;

  FileType = WareSrcFiletype();  
  FileType.m_Extensions = "*.ui;*.ts";
  FileType.m_LangCode = "";
  FileType.m_IconPath = ":/ui/common/filetypes/qt-ui.png";
  FileType.m_HlFilename = "";
  m_WareSrcFiletypes["qt-ui"] = FileType;

  FileType = WareSrcFiletype();  
  FileType.m_Extensions = "wareshub.json";
  FileType.m_LangCode = "json";
  FileType.m_IconPath = ":/ui/common/filetypes/wareshub.png";
  FileType.m_HlFilename = "";
  m_WareSrcFiletypes["wareshub"] = FileType;


  // registration of icons by file extensions
  for (auto it = m_WareSrcFiletypes.constBegin(); it != m_WareSrcFiletypes.constEnd(); ++it)
  {
    if (!it.value().m_IconPath.isEmpty())
    {
      m_IconsByFileExtensionList[it.value().m_Extensions] = it.value().m_IconPath;
    }
  }
}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::HighlightingRules_t WareSrcFiletypeManager::parseSyntaxFile(const QString& FilePath)
{
  tinyxml2::XMLDocument Doc;

  if (Doc.LoadFile(FilePath.toStdString().c_str()) != tinyxml2::XML_SUCCESS)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "file cannot be found: " + FilePath.toStdString());
  }


  HighlightingRules_t Rules;

  const auto Root = Doc.RootElement();
  if (Root != nullptr && std::string(Root->Name()) == "openfluid")
  {
    const auto WaresDevElt = Root->FirstChildElement("waresdev");

    if (WaresDevElt != nullptr)
    {
      const auto LangElt = WaresDevElt->FirstChildElement("language");

      if (LangElt != nullptr)
      {
        const auto HlhtElt = LangElt->FirstChildElement("highlighting");

        if (HlhtElt != nullptr)
        {
          for (auto Elt = HlhtElt->FirstChildElement(); Elt != nullptr; Elt = Elt->NextSiblingElement())
          {
            std::string TagName(Elt->Name());

            if (TagName == "list")
            {
              QString StyleName = QString::fromStdString(openfluid::tools::getXMLAttribute(Elt,"style"));

              if (m_Formats.contains(StyleName))
              {
                QTextCharFormat Format = m_Formats.value(StyleName);

                for (auto ItemElt = Elt->FirstChildElement("item"); ItemElt != nullptr; 
                    ItemElt = ItemElt->NextSiblingElement("item"))
                {
                  QString ItemText = QString::fromStdString(openfluid::tools::getXMLText(ItemElt));
                  Rules.append(HighlightingRule(StyleName, QRegExp(QString("\\b%1\\b").arg(ItemText)),Format));
                }
              }
            }
            else if (TagName == "rule")
            {
              QString StyleName = QString::fromStdString(openfluid::tools::getXMLAttribute(Elt,"style"));

              if (m_Formats.contains(StyleName))
              {
                QTextCharFormat Format = m_Formats.value(StyleName);

                for (auto PttrnElt = Elt->FirstChildElement("pattern"); PttrnElt != nullptr; 
                     PttrnElt = PttrnElt->NextSiblingElement("pattern"))
                {
                  QString SimplePatternValue = 
                    QString::fromStdString(openfluid::tools::getXMLAttribute(PttrnElt,"value"));
                  QString BeginPatternValue = 
                    QString::fromStdString(openfluid::tools::getXMLAttribute(PttrnElt,"start"));
                  QString EndPatternValue = 
                    QString::fromStdString(openfluid::tools::getXMLAttribute(PttrnElt,"end"));
                  
                  if (!SimplePatternValue.isEmpty())
                  {
                    Rules.append(HighlightingRule(StyleName, QRegExp(SimplePatternValue), Format));
                  }
                  else if (!BeginPatternValue.isEmpty() && !EndPatternValue.isEmpty())
                  {
                    Rules.append(HighlightingRule(StyleName,
                                                  QRegExp(BeginPatternValue),QRegExp(EndPatternValue),
                                                  Format));
                  }
                }
              }
            }
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "file is not well formed ('highlighting' tag is missing): "+
                                                    FilePath.toStdString());
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "file is not well formed ('language' tag is missing): "+
                                                  FilePath.toStdString());
      }
    }
    else
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "file is not well formed ('waresdev' tag is missing): "+
                                                FilePath.toStdString());
    }
  }
  else
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "file is not well formed ('openfluid' tag is missing): "+
                                              FilePath.toStdString());
  }

  return Rules;
}


// =====================================================================
// =====================================================================


QString WareSrcFiletypeManager::getFileType(const QString& FilePath) const
{
  for (QMap<QString, WareSrcFiletype>::const_iterator it = m_WareSrcFiletypes.begin();
       it != m_WareSrcFiletypes.end(); ++it)
  {
    if (QDir::match(it.value().m_Extensions,QFileInfo(FilePath).fileName()))
    {
      return it.key();
    }
  }

  return "";
}


// =====================================================================
// =====================================================================


QString WareSrcFiletypeManager::getFileLanguage(const QString& FilePath) const
{ 
  for (QMap<QString, WareSrcFiletype>::const_iterator it = m_WareSrcFiletypes.begin();
       it != m_WareSrcFiletypes.end(); ++it)
  {
    if (QDir::match(it.value().m_Extensions, QFileInfo(FilePath).fileName()))
    {
      return it.value().m_LangCode;
    }
  }

  return "";
}


// =====================================================================
// =====================================================================


QMap<QString, QString> WareSrcFiletypeManager::getIconsByFileExtensionList() const
{
  return m_IconsByFileExtensionList;
}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::HighlightingRules_t WareSrcFiletypeManager::getHighlightingRules(const QString& FilePath) const
{
  QString FileType = getFileType(FilePath);

  return m_WareSrcFiletypes.value(FileType, WareSrcFiletype()).m_HlRules;
}

} } }  // namespaces
