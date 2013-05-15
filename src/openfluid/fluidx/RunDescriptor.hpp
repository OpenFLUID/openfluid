/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __RUNDESCRIPTOR_HPP__
#define __RUNDESCRIPTOR_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/base/SimulationStatus.hpp>


namespace openfluid { namespace fluidx {

class DLLEXPORT RunDescriptor
{
  private:

    int m_DeltaT;

    openfluid::base::SimulationStatus::SchedulingConstraint m_SchedConstraint;

    openfluid::core::DateTime m_BeginDate;
    openfluid::core::DateTime m_EndDate;

    bool m_IsUserValuesBufferSize;
    unsigned int m_ValuesBufferSize;

   bool m_Filled;

  public:

    RunDescriptor();

    RunDescriptor(int DeltaT,
                  openfluid::core::DateTime BeginDate,openfluid::core::DateTime EndDate);

    ~RunDescriptor();

    inline openfluid::core::DateTime getBeginDate() const {return m_BeginDate; };

    inline void setBeginDate(const openfluid::core::DateTime BeginDate) { m_BeginDate = BeginDate; };

    inline openfluid::core::DateTime getEndDate() const {return m_EndDate; };

    inline void setEndDate(const openfluid::core::DateTime EndDate) { m_EndDate = EndDate; };

    inline int getDeltaT() const {return m_DeltaT; };

    inline void setDeltaT(const int DeltaT) { m_DeltaT = DeltaT; };

    inline void setValuesBufferSize(const unsigned int StepsNbr)
    {
      m_ValuesBufferSize = StepsNbr;  m_IsUserValuesBufferSize = true;
    };

    inline bool isUserValuesBufferSize() const { return m_IsUserValuesBufferSize; };

    inline unsigned int getValuesBufferSize() const { return m_ValuesBufferSize; };

    inline openfluid::base::SimulationStatus::SchedulingConstraint getSchedulingConstraint() const { return m_SchedConstraint; };

    inline void setSchedulingConstraint(const openfluid::base::SimulationStatus::SchedulingConstraint& SConst) { m_SchedConstraint = SConst; };

    inline bool isFilled() const { return m_Filled; };

    inline void setFilled(const bool Filled) { m_Filled = Filled; };

    inline void unsetUserValuesBufferSize()
    {
      m_IsUserValuesBufferSize = false;
    }

};

} } // namespaces


#endif /* __RUNDESCRIPTOR_HPP__ */
