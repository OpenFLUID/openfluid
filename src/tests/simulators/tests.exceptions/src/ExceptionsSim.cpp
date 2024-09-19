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
  @file ExceptionsSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/debug.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>


class ExceptionsSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    std::string m_PartStr;


  public:


    ExceptionsSimulator() : PluggableSimulator()
    {

    }


    // =====================================================================
    // =====================================================================


    ~ExceptionsSimulator()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      OPENFLUID_GetWareParameter(Params,"part",m_PartStr);

      if (m_PartStr == "initparams")
      {
        OPENFLUID_RaiseError(m_PartStr);
      }
    }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {
      if (m_PartStr == "preparedata")
      {
        OPENFLUID_RaiseError(m_PartStr);
      }
    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {
      if (m_PartStr == "checkconsistency")
      {
        OPENFLUID_RaiseError(m_PartStr);
      }
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      if (m_PartStr == "initializerun")
      {
        OPENFLUID_RaiseError(m_PartStr);
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      if (m_PartStr == "runstep")
      {
        OPENFLUID_RaiseError(m_PartStr);
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {
      if (m_PartStr  == "finalizerun")
      {
        OPENFLUID_RaiseError(m_PartStr);
      }
    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(ExceptionsSimulator)

