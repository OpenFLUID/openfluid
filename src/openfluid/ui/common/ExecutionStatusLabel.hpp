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
  @file ExecutionStatusLabel.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_EXECUTIONSTATUSLABEL_HPP__
#define __OPENFLUID_UICOMMON_EXECUTIONSTATUSLABEL_HPP__


#include <QLabel>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API ExecutionStatusLabel : public QLabel
{
  Q_OBJECT;

  public:

    enum class Status
    {
      NOTSTARTED,
      RUNNING,
      REQUESTING_PAUSE,
      PAUSED,
      REQUESTING_ABORT,
      ABORTED,
      FAILED,
      SUCCEEDED,
      USERKILLED,
      NONE
    };

    explicit ExecutionStatusLabel(QWidget* Parent = nullptr);

    ExecutionStatusLabel(const QString& Text, QWidget* Parent = nullptr);

    virtual ~ExecutionStatusLabel()
    { 

    }

    void setStatus(Status S);

    void setStatusTextOnly(Status S);

    void setStatusStyleOnly(Status S);
};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_EXECUTIONSTATUSLABEL_HPP__ */
