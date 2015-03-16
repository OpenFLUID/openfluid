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
  @file WareException.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
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


    void buildFullMessage()
    {
      m_FullMessage = m_Message;
      m_FullMessage += " [" + m_Context.toString() + "]";
    }



  public:

    WareException(const openfluid::base::ExceptionContext& Context,
                  const std::string& Msg) :
      openfluid::base::Exception(Context,Msg)
    {
      buildFullMessage();
    }


    // =====================================================================
    // =====================================================================


    static openfluid::base::ExceptionContext computeContext(const openfluid::ware::PluggableWare::WareType WType,
                                                            const openfluid::ware::WareID_t& ID)
    {

      openfluid::base::ExceptionContext Context;

      Context["src"] = "ware";
      Context["waretype"] = "unknown";

      if (WType == PluggableWare::OBSERVER)
        Context["waretype"] = "observer";
      else if (WType == PluggableWare::SIMULATOR)
        Context["waretype"] = "simulator";

      Context["wareid"] = ID;

      return Context;
    }


    // =====================================================================
    // =====================================================================


    static openfluid::base::ExceptionContext computeContext(const openfluid::ware::PluggableWare::WareType WType,
                                                            const openfluid::ware::WareID_t& ID,
                                                            const openfluid::base::SimulationStatus::SimulationStage
                                                              Stage)
    {
      
      openfluid::base::ExceptionContext Context = computeContext(WType,ID);

      if (WType == PluggableWare::OBSERVER)
      {
        switch (Stage)
        {
          case openfluid::base::SimulationStatus::INITPARAMS : Context["stage"] = "initParams"; break;
          case openfluid::base::SimulationStatus::PREPAREDATA : Context["stage"] = "onPrepared"; break;
          case openfluid::base::SimulationStatus::CHECKCONSISTENCY : Context["stage"] = "onPrepared"; break;
          case openfluid::base::SimulationStatus::INITIALIZERUN : Context["stage"] = "onInitializedRun"; break;
          case openfluid::base::SimulationStatus::RUNSTEP : Context["stage"] = "onStepCompleted"; break;
          case openfluid::base::SimulationStatus::FINALIZERUN : Context["stage"] = "onFinalizedRun"; break;
          default : break;
        }
      }
      else if (WType == PluggableWare::SIMULATOR)
      {
        switch (Stage)
        {
          case openfluid::base::SimulationStatus::INITPARAMS : Context["stage"] = "initParams"; break;
          case openfluid::base::SimulationStatus::PREPAREDATA : Context["stage"] = "prepareData"; break;
          case openfluid::base::SimulationStatus::CHECKCONSISTENCY : Context["stage"] = "checkConsistency"; break;
          case openfluid::base::SimulationStatus::INITIALIZERUN : Context["stage"] = "initializeRun"; break;
          case openfluid::base::SimulationStatus::RUNSTEP : Context["stage"] = "runStep"; break;
          case openfluid::base::SimulationStatus::FINALIZERUN : Context["stage"] = "finalizeRun"; break;
          default : break;
        }
      }
      else
      {
        Context["stage"] = "unknown";
      }
      
      return Context;
    }


    // =====================================================================
    // =====================================================================


    static openfluid::base::ExceptionContext computeContext(const openfluid::ware::PluggableWare::WareType WType,
                                                            const openfluid::ware::WareID_t& ID,
                                                            const openfluid::base::SimulationStatus::SimulationStage
                                                              Stage,
                                                            openfluid::core::TimeIndex_t TimeIndex)
    {
      openfluid::base::ExceptionContext Context = computeContext(WType,ID,Stage);

      std::ostringstream IndexSStr;
      IndexSStr << TimeIndex;

      Context["timeindex"] = IndexSStr.str();

      return Context;
    }



};


} } // namespaces




#endif /* __OPENFLUID_WARE_WAREEXCEPTION_HPP__ */
