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
  @file FocusableLineEdit.hpp

  @author Armel Thoni <armel.thoni@inra.fr>
*/


#ifndef __OPENFLUID_UICOMMON_FOCUSABLELINEEDIT_HPP__
#define __OPENFLUID_UICOMMON_FOCUSABLELINEEDIT_HPP__


#include <QLineEdit>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API FocusableLineEdit : public QLineEdit
{
  Q_OBJECT;

  signals:
  
    void focusChanged();
    

  private:
    std::pair<size_t, size_t> m_TextSelection;
  

  private slots:
    void changeEditSelection();

  protected:

    void focusInEvent(QFocusEvent *e);


  public:

    FocusableLineEdit(QWidget* Parent = nullptr);

    virtual ~FocusableLineEdit()
    { }

    void modifyText(QString NewText);

};

} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_FOCUSABLELINEEDIT_HPP__ */
