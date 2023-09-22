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
  @file DefaultAction.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_UICOMMON_DEFAULTACTION_HPP__
#define __OPENFLUID_UICOMMON_DEFAULTACTION_HPP__


#include <QObject>
#include <QAction>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API DefaultAction : public QAction
{

  Q_OBJECT

  public:

    DefaultAction(const QIcon& Icon, const QString& Text, QObject* Parent = nullptr) : QAction(Icon,Text,Parent)
    {
      setMenuRole(QAction::NoRole);
    }

    DefaultAction(const QString& Text, QObject* Parent = nullptr) : QAction(Text,Parent)
    {
      setMenuRole(QAction::NoRole);
    }

    DefaultAction(QObject* Parent = nullptr) : QAction(Parent)
    {
      setMenuRole(QAction::NoRole);
    }

    virtual ~DefaultAction()
    { }
};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_DEFAULTACTION_HPP__ */
