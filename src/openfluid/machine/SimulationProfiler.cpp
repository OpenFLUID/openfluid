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
  @file SimulationProfiler.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <iomanip>

#include <openfluid/config.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/machine/SimulationProfiler.hpp>


namespace openfluid { namespace machine {


// =====================================================================
// =====================================================================


double SimulationProfiler::getDurationInDecimalSeconds(const TimeResolution_t& Duration)
{
/*  return (static_cast<double>(Duration.total_seconds()) +
         (static_cast<double>(Duration.fractional_seconds()) /
             static_cast<double>(boost::posix_time::time_duration::ticks_per_second())));*/

  return double(Duration.count()) / 1000000000;

}


// =====================================================================
// =====================================================================


SimulationProfiler::SimulationProfiler(const openfluid::base::SimulationStatus* SimStatus,
                                       const WareIDSequence_t& OrigModelSequence)
: mp_SimStatus(SimStatus), m_OriginalModelSequence(OrigModelSequence), m_CurrentTimeIndex(0)
{

  m_CurrentSequenceFile.open(openfluid::base::RunContextManager::instance()
    ->getOutputFullPath(openfluid::config::SCHEDULE_PROFILE_FILE).c_str(),std::ios::out);
  m_CurrentProfileFile.open(openfluid::base::RunContextManager::instance()
    ->getOutputFullPath(openfluid::config::TIMEINDEX_PROFILE_FILE).c_str(),std::ios::out);

  m_CurrentSequenceFile << "TIMEINDEX;<simulators call sequence>\n";


  m_CurrentProfileFile << std::fixed << std::setprecision(9);
  m_CurrentProfileFile << "TIMEINDEX";

  for (WareIDSequence_t::const_iterator It = m_OriginalModelSequence.begin();
      It != m_OriginalModelSequence.end();
      ++It)
  {
    m_CumulativeModelProfile[*It][openfluid::base::SimulationStatus::INITPARAMS] =
        TimeResolution_t(0);
    m_CumulativeModelProfile[*It][openfluid::base::SimulationStatus::PREPAREDATA] =
        TimeResolution_t(0);
    m_CumulativeModelProfile[*It][openfluid::base::SimulationStatus::CHECKCONSISTENCY] =
        TimeResolution_t(0);
    m_CumulativeModelProfile[*It][openfluid::base::SimulationStatus::INITIALIZERUN] =
        TimeResolution_t(0);
    m_CumulativeModelProfile[*It][openfluid::base::SimulationStatus::RUNSTEP] =
        TimeResolution_t(0);
    m_CumulativeModelProfile[*It][openfluid::base::SimulationStatus::FINALIZERUN] =
        TimeResolution_t(0);

    m_CurrentProfileFile << ";" << *It;
  }

  m_CurrentProfileFile << "\n";

}


// =====================================================================
// =====================================================================


SimulationProfiler::~SimulationProfiler()
{
  flushCurrentProfileToFiles();
  m_CurrentSequenceFile.close();
  m_CurrentProfileFile.close();

  // cumulative profile

  std::ofstream CumulativeFile;

  CumulativeFile.open(openfluid::base::RunContextManager::instance()
    ->getOutputFullPath(openfluid::config::CUMULATIVE_PROFILE_FILE).c_str(),std::ios::out);

  WareIDSequence_t::const_iterator ItPrf;
  WareIDSequence_t::const_iterator ItPrfB = m_OriginalModelSequence.begin();
  WareIDSequence_t::const_iterator ItPrfE = m_OriginalModelSequence.end();

  CumulativeFile << std::fixed << std::setprecision(9);
  CumulativeFile << "ID;INITPARAMS;PREPAREDATA;CHECKCONSISTENCY;INITIALIZERUN;RUNSTEP;FINALIZERUN\n";

  for (ItPrf=ItPrfB;ItPrf!=ItPrfE;++ItPrf)
  {
    CumulativeFile << *ItPrf;
    CumulativeFile << ";" <<
        getDurationInDecimalSeconds(m_CumulativeModelProfile[*ItPrf][openfluid::base::SimulationStatus::INITPARAMS]);
    CumulativeFile << ";" <<
        getDurationInDecimalSeconds(m_CumulativeModelProfile[*ItPrf][openfluid::base::SimulationStatus::PREPAREDATA]);
    CumulativeFile << ";" <<
        getDurationInDecimalSeconds(
            m_CumulativeModelProfile[*ItPrf][openfluid::base::SimulationStatus::CHECKCONSISTENCY]);
    CumulativeFile << ";" <<
        getDurationInDecimalSeconds(m_CumulativeModelProfile[*ItPrf][openfluid::base::SimulationStatus::INITIALIZERUN]);
    CumulativeFile << ";" <<
        getDurationInDecimalSeconds(m_CumulativeModelProfile[*ItPrf][openfluid::base::SimulationStatus::RUNSTEP]);
    CumulativeFile << ";" <<
        getDurationInDecimalSeconds(m_CumulativeModelProfile[*ItPrf][openfluid::base::SimulationStatus::FINALIZERUN]);
    CumulativeFile << "\n";
  }

  CumulativeFile.close();
}


// =====================================================================
// =====================================================================


void SimulationProfiler::flushCurrentProfileToFiles()
{
  // sequence file

  m_CurrentSequenceFile << m_CurrentTimeIndex;

  WareIDSequence_t::const_iterator ItSeq;
  WareIDSequence_t::const_iterator ItSeqB = m_CurrentTimeIndexModelSequence.begin();
  WareIDSequence_t::const_iterator ItSeqE = m_CurrentTimeIndexModelSequence.end();

  for (ItSeq=ItSeqB;ItSeq!=ItSeqE;++ItSeq)
    m_CurrentSequenceFile << ";" << *ItSeq;

  m_CurrentSequenceFile << "\n";


  // profile file

  m_CurrentProfileFile << m_CurrentTimeIndex;

  WareIDSequence_t::const_iterator ItPrf;
  WareIDSequence_t::const_iterator ItPrfB = m_OriginalModelSequence.begin();
  WareIDSequence_t::const_iterator ItPrfE = m_OriginalModelSequence.end();


  for (ItPrf=ItPrfB;ItPrf!=ItPrfE;++ItPrf)
  {
    CurrentTimeIndexModelProfile_t::const_iterator ItPrfT = m_CurrentTimeIndexModelProfile.find(*ItPrf);
    if (ItPrfT == m_CurrentTimeIndexModelProfile.end())
      m_CurrentProfileFile << ";NA";
    else
      m_CurrentProfileFile << ";" << getDurationInDecimalSeconds((*ItPrfT).second);
  }


  m_CurrentProfileFile << "\n";

  m_CurrentTimeIndexModelProfile.clear();
  m_CurrentTimeIndexModelSequence.clear();
}


// =====================================================================
// =====================================================================


void SimulationProfiler::addDuration(const openfluid::ware::WareID_t& SimID,
                                     openfluid::base::SimulationStatus::SimulationStage ProfilePart,
                                     const TimeResolution_t& Duration)
{
  if (ProfilePart == openfluid::base::SimulationStatus::INITIALIZERUN ||
      ProfilePart == openfluid::base::SimulationStatus::RUNSTEP)
  {
    if (mp_SimStatus->getCurrentTimeIndex() != m_CurrentTimeIndex)
    {
      flushCurrentProfileToFiles();
      m_CurrentTimeIndex = mp_SimStatus->getCurrentTimeIndex();
    }

    m_CurrentTimeIndexModelSequence.push_back(SimID);
    m_CurrentTimeIndexModelProfile[SimID] = Duration;
  }

  m_CumulativeModelProfile[SimID][ProfilePart] = m_CumulativeModelProfile[SimID][ProfilePart] + Duration;

}


} } //namespaces
