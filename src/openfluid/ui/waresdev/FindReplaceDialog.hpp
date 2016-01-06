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
 @file FindReplaceDialog.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_FINDREPLACEDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_FINDREPLACEDIALOG_HPP__

#include <openfluid/dllexport.hpp>

#include <QDialog>
#include <QTextDocument>

namespace Ui { class FindReplaceDialog;
}

namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API FindReplaceDialog: public QDialog
{
  Q_OBJECT

  public:

    enum class FindReplaceAction
    {
      FindOnly, ReplaceOnly, ReplaceFind, ReplaceAll
    };

  private:

    Ui::FindReplaceDialog* ui;

    QTextDocument::FindFlags getFindOptions();

  public:

    FindReplaceDialog(QWidget* Parent);

    ~FindReplaceDialog();

    void setMessage(const QString& Message, bool TextFound);

  signals :

    void findReplaceRequested(FindReplaceDialog::FindReplaceAction Action, const QString& StringToFind,
                              const QString& StringForReplace, QTextDocument::FindFlags Flags);

  public slots:

    void show(const QString& SelectedText);

  private slots:

    void onTextChanged(const QString& Text);

    void onFindClicked();

    void onReplaceClicked();

    void onReplaceFindClicked();

    void onReplaceAllClicked();
};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_FINDREPLACEDIALOG_HPP__ */
