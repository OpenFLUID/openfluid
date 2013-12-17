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
  \file SimulationProfiler.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __SIMULATIONPROFILER_HPP___
#define __SIMULATIONPROFILER_HPP___

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/base/SimulationStatus.hpp>
#include <openfluid/dllexport.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <map>
#include <list>


namespace openfluid { namespace machine {


// =====================================================================
// =====================================================================



class DLLEXPORT SimulationProfiler
{
  public:

    typedef std::list<openfluid::ware::WareID_t> WareIDSequence_t;

  private:

    typedef std::map<openfluid::base::SimulationStatus::SimulationStage,boost::posix_time::time_duration> CumulativeSimulatorProfile_t;

    typedef std::map<openfluid::ware::WareID_t,CumulativeSimulatorProfile_t> CumulativeModelProfile_t;

    typedef std::map<openfluid::ware::WareID_t,boost::posix_time::time_duration> CurrentTimeIndexModelProfile_t;

    typedef WareIDSequence_t CurrentTimeIndexModelSequence_t;

    CumulativeModelProfile_t m_CumulativeModelProfile;

    CurrentTimeIndexModelProfile_t m_CurrentTimeIndexModelProfile;
    CurrentTimeIndexModelSequence_t m_CurrentTimeIndexModelSequence;


    const openfluid::base::SimulationStatus* mp_SimStatus;

    const WareIDSequence_t m_OriginalModelSequence;

    openfluid::core::TimeIndex_t m_CurrentTimeIndex;

    std::ofstream m_CurrentSequenceFile;

    std::ofstream m_CurrentProfileFile;

    static double getDurationInDecimalSeconds(const boost::posix_time::time_duration& Duration);

    void flushCurrentProfileToFiles();

  public:

    SimulationProfiler(const openfluid::base::SimulationStatus* SimStatus, const WareIDSequence_t& OrigModelSequence);

    ~SimulationProfiler();

    void addDuration(const openfluid::ware::WareID_t& SimID,
                     openfluid::base::SimulationStatus::SimulationStage ProfilePart,
                     const boost::posix_time::time_duration& Duration);

};



} } //namespaces


#endif /* __SIMULATIONPROFILER_HPP___ */
