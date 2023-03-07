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
  @file PluggableObserver.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_WARE_PLUGGABLEOBSERVER_HPP__
#define __OPENFLUID_WARE_PLUGGABLEOBSERVER_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulationInspectorWare.hpp>
#include <openfluid/ware/ObserverSignature.hpp>
#include <openfluid/ware/LoopMacros.hpp>


// =====================================================================
// =====================================================================


/**
  Macro for definition of observer class hook
  @param[in] pluginclassname The name of the class to instantiate
*/
#define DEFINE_OBSERVER_CLASS(pluginclassname) \
  extern "C" \
  { \
    OPENFLUID_PLUGIN const std::string* WAREABIVERSION_PROC_DECL() \
    { \
      return new std::string(openfluid::config::VERSION_FULL); \
    } \
    \
    OPENFLUID_PLUGIN openfluid::ware::PluggableObserver* WAREBODY_PROC_DECL() \
    { \
      return new pluginclassname(); \
    } \
  }


// =====================================================================
// =====================================================================


namespace openfluid { namespace ware {


class OPENFLUID_API PluggableObserver : public SimulationInspectorWare
{

  public:

    PluggableObserver();

    virtual ~PluggableObserver();

    using SimulationInspectorWare::initializeWare;

    /**
       Internally called by the framework.
     */
    void initializeWare(const WareID_t& SimID);

    /**
       Initializes simulator parameters of the simulator, given as a hash map. Internally called by the framework.
     */
    virtual void initParams(const openfluid::ware::WareParams_t& Params)=0;

    /**
       Prepares data. Internally called by the framework.
    */
    virtual void onPrepared()=0;


    /**
       Internally called by the framework.
    */
    virtual void onInitializedRun()=0;

    /**
       Internally called by the framework.
    */
    virtual void onStepCompleted()=0;

    /**
       Internally called by the framework.
    */
    virtual void onFinalizedRun()=0;

};


typedef PluggableObserver* (*GetPluggableObserverBodyProc)();

typedef ObserverSignature* (*GetPluggableObserverSignatureProc)();


} } // openfluid::ware


#endif /* __OPENFLUID_WARE_PLUGGABLEOBSERVER_HPP__ */
