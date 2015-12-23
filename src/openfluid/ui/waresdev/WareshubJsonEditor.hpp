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
 @file WareshubJsonEditor.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_WARESHUBJSONEDITOR_HPP__
#define __OPENFLUID_UIWARESDEV_WARESHUBJSONEDITOR_HPP__


#include <rapidjson/document.h>
#include <QLineEdit>
#include <QComboBox>
#include <openfluid/dllexport.hpp>
#include <openfluid/ui/waresdev/WareFileEditor.hpp>

namespace Ui { class WareshubJsonEditor;
}


namespace openfluid { namespace ui { namespace waresdev {

class OPENFLUID_API WareshubJsonEditor: public QWidget, public WareFileEditor
{
  Q_OBJECT

  private:

    Ui::WareshubJsonEditor* ui;

    rapidjson::Document Doc;

    bool m_IsModified = false;

    // SPDX License List Version: 2.2 Sep 30, 2015 https://spdx.org/licenses/
    QStringList m_Licenses = { "0BSD", "AAL", "Abstyles", "Adobe-2006", "Adobe-Glyph", "ADSL", "AFL-1.1", "AFL-1.2",
                               "AFL-2.0", "AFL-2.1", "AFL-3.0", "Afmparse", "AGPL-1.0", "AGPL-3.0", "Aladdin",
                               "AMDPLPA", "AML", "AMPAS", "ANTLR-PD", "Apache-1.0", "Apache-1.1", "Apache-2.0",
                               "APAFML", "APL-1.0", "APSL-1.0", "APSL-1.1", "APSL-1.2", "APSL-2.0", "Artistic-1.0",
                               "Artistic-1.0-cl8", "Artistic-1.0-Perl", "Artistic-2.0", "Bahyph", "Barr", "Beerware",
                               "BitTorrent-1.0", "BitTorrent-1.1", "Borceux", "BSD-2-Clause", "BSD-2-Clause-FreeBSD",
                               "BSD-2-Clause-NetBSD", "BSD-3-Clause", "BSD-3-Clause-Attribution", "BSD-3-Clause-Clear",
                               "BSD-3-Clause-LBNL", "BSD-4-Clause", "BSD-4-Clause-UC", "BSD-Protection", "BSL-1.0",
                               "bzip2-1.0.5", "bzip2-1.0.6", "Caldera", "CATOSL-1.1", "CC-BY-1.0", "CC-BY-2.0",
                               "CC-BY-2.5", "CC-BY-3.0", "CC-BY-4.0", "CC-BY-NC-1.0", "CC-BY-NC-2.0", "CC-BY-NC-2.5",
                               "CC-BY-NC-3.0", "CC-BY-NC-4.0", "CC-BY-NC-ND-1.0", "CC-BY-NC-ND-2.0", "CC-BY-NC-ND-2.5",
                               "CC-BY-NC-ND-3.0", "CC-BY-NC-ND-4.0", "CC-BY-NC-SA-1.0", "CC-BY-NC-SA-2.0",
                               "CC-BY-NC-SA-2.5", "CC-BY-NC-SA-3.0", "CC-BY-NC-SA-4.0", "CC-BY-ND-1.0", "CC-BY-ND-2.0",
                               "CC-BY-ND-2.5", "CC-BY-ND-3.0", "CC-BY-ND-4.0", "CC-BY-SA-1.0", "CC-BY-SA-2.0",
                               "CC-BY-SA-2.5", "CC-BY-SA-3.0", "CC-BY-SA-4.0", "CC0-1.0", "CDDL-1.0", "CDDL-1.1",
                               "CECILL-1.0", "CECILL-1.1", "CECILL-2.0", "CECILL-2.1", "CECILL-B", "CECILL-C",
                               "ClArtistic", "CNRI-Jython", "CNRI-Python", "CNRI-Python-GPL-Compatible", "Condor-1.1",
                               "CPAL-1.0", "CPL-1.0", "CPOL-1.02", "Crossword", "CrystalStacker", "CUA-OPL-1.0",
                               "Cube", "D-FSL-1.0", "diffmark", "DOC", "Dotseqn", "DSDP", "dvipdfm", "ECL-1.0",
                               "ECL-2.0", "EFL-1.0", "EFL-2.0", "eGenix", "Entessa", "EPL-1.0", "ErlPL-1.1",
                               "EUDatagrid", "EUPL-1.0", "EUPL-1.1", "Eurosym", "Fair", "Frameworx-1.0", "FreeImage",
                               "FSFUL", "FSFULLR", "FTL", "GFDL-1.1", "GFDL-1.2", "GFDL-1.3", "Giftware", "GL2PS",
                               "Glide", "Glulxe", "gnuplot", "GPL-1.0", "GPL-2.0", "GPL-3.0", "gSOAP-1.3b",
                               "HaskellReport", "HPND", "IBM-pibs", "ICU", "IJG", "ImageMagick", "iMatix", "Imlib2",
                               "Intel", "Intel-ACPI", "Interbase-1.0", "IPA", "IPL-1.0", "ISC", "JasPer-2.0", "JSON",
                               "Latex2e", "Leptonica", "LGPL-2.0", "LGPL-2.1", "LGPL-3.0", "LGPLLR", "Libpng",
                               "libtiff", "LPL-1.0", "LPL-1.02", "LPPL-1.0", "LPPL-1.1", "LPPL-1.2", "LPPL-1.3a",
                               "LPPL-1.3c", "MakeIndex", "MirOS", "MIT", "MIT-advertising", "MIT-CMU", "MIT-enna",
                               "MIT-feh", "MITNFA", "Motosoto", "mpich2", "MPL-1.0", "MPL-1.1", "MPL-2.0",
                               "MPL-2.0-no-copyleft-exception", "MS-PL", "MS-RL", "MTLL", "Multics", "Mup", "NASA-1.3",
                               "Naumen", "NBPL-1.0", "NCSA", "NetCDF", "Newsletr", "NGPL", "NLPL", "Nokia", "NOSL",
                               "Noweb", "NPL-1.0", "NPL-1.1", "NPOSL-3.0", "NRL", "NTP", "Nunit", "OCLC-2.0",
                               "ODbL-1.0", "OFL-1.0", "OFL-1.1", "OGTSL", "OLDAP-1.1", "OLDAP-1.2", "OLDAP-1.3",
                               "OLDAP-1.4", "OLDAP-2.0", "OLDAP-2.0.1", "OLDAP-2.1", "OLDAP-2.2", "OLDAP-2.2.1",
                               "OLDAP-2.2.2", "OLDAP-2.3", "OLDAP-2.4", "OLDAP-2.5", "OLDAP-2.6", "OLDAP-2.7",
                               "OLDAP-2.8", "OML", "OpenSSL", "OPL-1.0", "OSL-1.0", "OSL-1.1", "OSL-2.0", "OSL-2.1",
                               "OSL-3.0", "PDDL-1.0", "PHP-3.0", "PHP-3.01", "Plexus", "PostgreSQL", "psfrag",
                               "psutils", "Python-2.0", "Qhull", "QPL-1.0", "Rdisc", "RHeCos-1.1", "RPL-1.1",
                               "RPL-1.5", "RPSL-1.0", "RSA-MD", "RSCPL", "Ruby", "SAX-PD", "Saxpath", "SCEA",
                               "Sendmail", "SGI-B-1.0", "SGI-B-1.1", "SGI-B-2.0", "SimPL-2.0", "SISSL", "SISSL-1.2",
                               "Sleepycat", "SMLNJ", "SNIA", "Spencer-86", "Spencer-94", "Spencer-99", "SPL-1.0",
                               "SugarCRM-1.1.3", "SWL", "TCL", "TMate", "TORQUE-1.1", "TOSL", "Unicode-TOU",
                               "Unlicense", "UPL-1.0", "Vim", "VOSTROM", "VSL-1.0", "W3C", "W3C-19980720",
                               "Watcom-1.0", "Wsuipa", "WTFPL", "X11", "Xerox", "XFree86-1.1", "xinetd", "Xnet", "xpp",
                               "XSkat", "YPL-1.0", "YPL-1.1", "Zed", "Zend-2.0", "Zimbra-1.3", "Zimbra-1.4", "Zlib",
                               "zlib-acknowledgement", "ZPL-1.1", "ZPL-2.0", "ZPL-2.1" };

    QStringList m_Statuses = { "stable", "experimental", "beta" };

    void jsonStringArrayToLineEdit(const QString& Key, QLineEdit* LineEdit);
    void lineEditToJsonStringArray(const QString& Key, QLineEdit* LineEdit);

    void jsonStringToComboBox(const QString& Key, QComboBox* Combo);
    void comboBoxToJsonString(const QString& Key, QComboBox* Combo);

  public:

    WareshubJsonEditor(const QString& FilePath, QWidget* Parent = nullptr);

    ~WareshubJsonEditor();

    void saveContent();

    void saveContentToPath(const QString& Path);

    void updateContent();

    bool findReplace(FindReplaceDialog::FindReplaceAction Action, const QString& StringToFind,
      const QString& StringForReplace, QTextDocument::FindFlags Options, QString& Message);

    QString getSelectedText();

    bool isModified();

    QWidget* getWidget();

    void clearLineMessages()
    {
    }

    void addLineMessage(openfluid::waresdev::WareSrcMsgParser::WareSrcMsg /*Message*/)
    {
    }

    void updateLineNumberArea()
    {
    }

    void copy()
    {
    }

    void cut()
    {
    }

    void paste()
    {
    }

    void selectLine(int /*LineNumber*/)
    {
    }

    void goToLine()
    {
    }

    void updateSettings()
    {
    }

    void setFocus()
    {
    }

  private slots:

    void onChanged();

  signals :

    void editorChanged(WareFileEditor* Editor, bool Changed);

    void editorSaved();
};

} } } // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESHUBJSONEDITOR_HPP__ */
