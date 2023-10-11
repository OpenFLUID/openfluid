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
  @file ActionLabel.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_ACTIONLABEL_HPP__
#define __OPENFLUID_UICOMMON_ACTIONLABEL_HPP__


#include <QObject>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/common/ClickableLabel.hpp>


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API ActionLabel : public ClickableLabel
{

  Q_OBJECT

  public:

    explicit ActionLabel(QWidget* Parent = nullptr);

    ActionLabel(const QString& Text, QWidget* Parent = nullptr);

    virtual ~ActionLabel()
    { }

    void setColor(const QString& Color);

};

} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_ACTIONLABEL_HPP__ */
