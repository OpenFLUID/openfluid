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
  \file WareException.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_WARE_WAREEXCEPTION_HPP__
#define __OPENFLUID_WARE_WAREEXCEPTION_HPP__

#include <openfluid/base/Exception.hpp>
#include <openfluid/base/SimulationStatus.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/dllexport.hpp>

namespace openfluid { namespace ware {


class OPENFLUID_API WareException : public openfluid::base::Exception
{
  protected:

    PluggableWare::WareType m_WareType;

    openfluid::base::SimulationStatus::SimulationStage m_Stage;

    openfluid::core::TimeIndex_t m_TimeIndex;

    bool m_IsTimeIndexed;

    void buildFullMessage()
    {
      if (m_Source.empty())
        m_Source = "unknown source";

      m_FullMessage = m_Message;

      m_FullMessage += " (sent by " + m_Sender;
      if (m_WareType == PluggableWare::SIMULATOR)
        m_FullMessage += " simulator";
      if (m_WareType == PluggableWare::OBSERVER)
        m_FullMessage += " observer";

      if (m_Source != "")
        m_FullMessage += ", from " + m_Source;

      if (m_IsTimeIndexed)
      {
        std::stringstream StrStream;
        StrStream << m_TimeIndex;
        m_FullMessage += ", at time index #" + StrStream.str();
      }

      m_FullMessage += ")";
    }



  public:

    static void updateSourceInfo(const PluggableWare::WareType& WType,
                                 const openfluid::base::SimulationStatus::SimulationStage& Stage,
                                 std::string& SourceStr)
    {
      SourceStr = "unknown source";

      if (WType == PluggableWare::OBSERVER)
      {
        switch (Stage)
        {
          case openfluid::base::SimulationStatus::INITPARAMS : SourceStr = "initParams"; break;
          case openfluid::base::SimulationStatus::PREPAREDATA : SourceStr = "onPrepared"; break;
          case openfluid::base::SimulationStatus::CHECKCONSISTENCY : SourceStr = "onPrepared"; break;
          case openfluid::base::SimulationStatus::INITIALIZERUN : SourceStr = "onInitializedRun"; break;
          case openfluid::base::SimulationStatus::RUNSTEP : SourceStr = "onStepCompleted"; break;
          case openfluid::base::SimulationStatus::FINALIZERUN : SourceStr = "onFinalizedRun"; break;
          default : break;
        }
      }
      else if (WType == PluggableWare::SIMULATOR)
      {
        switch (Stage)
        {
          case openfluid::base::SimulationStatus::INITPARAMS : SourceStr = "initParams"; break;
          case openfluid::base::SimulationStatus::PREPAREDATA : SourceStr = "prepareData"; break;
          case openfluid::base::SimulationStatus::CHECKCONSISTENCY : SourceStr = "checkConsistency"; break;
          case openfluid::base::SimulationStatus::INITIALIZERUN : SourceStr = "initializeRun"; break;
          case openfluid::base::SimulationStatus::RUNSTEP : SourceStr = "runStep"; break;
          case openfluid::base::SimulationStatus::FINALIZERUN : SourceStr = "finalizeRun"; break;
          default : break;
        }
      }
    }


    WareException(const WareID_t& ID,
                  const PluggableWare::WareType& WType,
                  const std::string& Source,
                  const std::string& Msg) :
      openfluid::base::Exception(ID,Msg), m_WareType(WType), m_Stage(openfluid::base::SimulationStatus::UNKNOWN),
      m_TimeIndex(0), m_IsTimeIndexed(false)
    {
      m_Source = Source;
      buildFullMessage();
    }


    WareException(const WareID_t& ID,
                  const PluggableWare::WareType& WType,
                  const openfluid::base::SimulationStatus::SimulationStage& Stage,
                  const std::string& Msg) :
      openfluid::base::Exception(ID,Msg), m_WareType(WType), m_Stage(Stage), m_TimeIndex(0), m_IsTimeIndexed(false)
    {
      updateSourceInfo(WType,Stage,m_Source);
      buildFullMessage();
    }


    WareException(const WareID_t& ID,
                  const PluggableWare::WareType& WType,
                  const std::string& Source,
                  const openfluid::core::TimeIndex_t& TimeIndex,
                  const std::string& Msg) :
      openfluid::base::Exception(ID,Msg), m_WareType(WType), m_Stage(openfluid::base::SimulationStatus::UNKNOWN),
      m_TimeIndex(TimeIndex), m_IsTimeIndexed(true)
    {
      m_Source = Source;
      buildFullMessage();
    }


    WareException(const WareID_t& ID,
                  const PluggableWare::WareType& WType,
                  const openfluid::base::SimulationStatus::SimulationStage& Stage,
                  const openfluid::core::TimeIndex_t& TimeIndex,
                  const std::string& Msg) :
      openfluid::base::Exception(ID,Msg), m_WareType(WType), m_Stage(Stage),
      m_TimeIndex(TimeIndex), m_IsTimeIndexed(true)
    {
      updateSourceInfo(WType,Stage,m_Source);
      buildFullMessage();
    }

};


} } // namespaces




#endif /* __OPENFLUID_WARE_WAREEXCEPTION_HPP__ */
