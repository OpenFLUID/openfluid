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
  @file RunDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_FLUIDX_RUNDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_RUNDESCRIPTOR_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/base/SimulationStatus.hpp>


namespace openfluid { namespace fluidx {

class OPENFLUID_API RunDescriptor
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

    inline openfluid::core::DateTime getBeginDate() const
    {return m_BeginDate; };

    inline void setBeginDate(const openfluid::core::DateTime BeginDate)
    { m_BeginDate = BeginDate; };

    inline openfluid::core::DateTime getEndDate() const
    {return m_EndDate; };

    inline void setEndDate(const openfluid::core::DateTime EndDate)
    { m_EndDate = EndDate; };

    inline int getDeltaT() const
    {return m_DeltaT; };

    inline void setDeltaT(const int DeltaT)
    { m_DeltaT = DeltaT; };

    inline void setValuesBufferSize(const unsigned int StepsNbr)
    {
      m_ValuesBufferSize = StepsNbr;  m_IsUserValuesBufferSize = true;
    };

    inline bool isUserValuesBufferSize() const
    { return m_IsUserValuesBufferSize; };

    inline unsigned int getValuesBufferSize() const
    { return m_ValuesBufferSize; };

    inline openfluid::base::SimulationStatus::SchedulingConstraint getSchedulingConstraint() const
    { return m_SchedConstraint; };

    inline void setSchedulingConstraint(const openfluid::base::SimulationStatus::SchedulingConstraint& SConst)
    { m_SchedConstraint = SConst; };

    inline bool isFilled() const
    { return m_Filled; };

    inline void setFilled(const bool Filled)
    { m_Filled = Filled; };

    inline void unsetUserValuesBufferSize()
    {
      m_IsUserValuesBufferSize = false;
    }

};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_RUNDESCRIPTOR_HPP__ */
