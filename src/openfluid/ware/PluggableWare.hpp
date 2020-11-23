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
  @file PluggableWare.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_WARE_PLUGGABLEWARE_HPP__
#define __OPENFLUID_WARE_PLUGGABLEWARE_HPP__


#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/WareSignature.hpp>


namespace openfluid { namespace ware {


/**
  Hook function name of ware plugin body
*/
#define WAREBODY_PROC_NAME "GetWareBody"
#define WAREBODY_PROC_DECL GetWareBody

/**
  Hook function name of ware plugin signature
*/
#define WARESIGNATURE_PROC_NAME "GetWareSignature"
#define WARESIGNATURE_PROC_DECL GetWareSignature

/**
  Hook function name of ware ABI version
*/
#define WAREABIVERSION_PROC_NAME "GetWareABIVersion"
#define WAREABIVERSION_PROC_DECL GetWareABIVersion


/**
  Hook function name of ware ABI version
*/
#define WARELINKUID_PROC_NAME "GetWareLinkUID"
#define WARELINKUID_PROC_DECL GetWareLinkUID


// =====================================================================
// =====================================================================


#define DEFINE_WARE_LINKUID(linkuid) \
  extern "C" \
  { \
    OPENFLUID_PLUGIN const std::string* WARELINKUID_PROC_DECL() \
    { \
      return new std::string(linkuid); \
    } \
  }


// =====================================================================
// =====================================================================


class OPENFLUID_API PluggableWare
{
  private:

    /**
      Ware execution environment
    */
    const openfluid::core::MapValue* mp_WareEnv = nullptr;


    /**
      Ware Type
    */
    WareType m_WareType = WareType::UNDEFINED;


    /**
      Ware ID
    */
    WareID_t m_WareID;


  protected:

    bool m_Initialized = false;

    virtual bool isLinked() const
    { 
      return mp_WareEnv != nullptr;
    }

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Msg the content of the message

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR","OBSERVER"],
        "menupath" : ["Compute code","Messages"],
        "title" : "Raise fatal error",
        "text" : "OPENFLUID_RaiseError(%%SEL_START%%\"message\"%%SEL_END%%)"
      }
      @endcond
    */
    virtual void OPENFLUID_RaiseError(const std::string& Msg);

    /**
      Gets an environment string value associated to a Key
      @param[in] Key the requested environment key
      @param[out] Val the value associated with the environment key
    */
    bool OPENFLUID_GetRunEnvironment(const std::string& Key, std::string& Val);

    /**
      Gets an environment boolean value associated to a Key
      @param[in] Key the requested environment key
      @param[out] Val the value associated with the environment key

      @cond OpenFLUID:completion
      {
        "contexts" : ["ANYWARE"],
        "menupath" : ["Compute code", "Run environment"],
        "title" : "Get information about run environment",
        "text" : "OPENFLUID_GetRunEnvironment(%%SEL_START%%\"key\"%%SEL_END%%,Value)"
      }
      @endcond

      @cond OpenFLUID:completion
      {
        "contexts" : ["ANYWARE"],
        "menupath" : ["Compute code", "Run environment"],
        "title" : "Get current input dataset directory",
        "text" : "OPENFLUID_GetRunEnvironment(\"dir.input\",%%SEL_START%%Value%%SEL_END%%)"
      }
      @endcond

      @cond OpenFLUID:completion
      {
        "contexts" : ["ANYWARE"],
        "menupath" : ["Compute code", "Run environment"],
        "title" : "Get current output results directory",
        "text" : "OPENFLUID_GetRunEnvironment(\"dir.output\",%%SEL_START%%Value%%SEL_END%%)"
      }
      @endcond

      @cond OpenFLUID:completion
      {
        "contexts" : ["ANYWARE"],
        "menupath" : ["Compute code", "Run environment"],
        "title" : "Get current directory for temporary data",
        "text" : "OPENFLUID_GetRunEnvironment(\"dir.temp\",%%SEL_START%%Value%%SEL_END%%)"
      }
      @endcond
    */
    bool OPENFLUID_GetRunEnvironment(const std::string& Key, bool& Val);

    /**
      Returns the ID of the ware (itself)
      @return the ID

      @cond OpenFLUID:completion
      {
        "contexts" : ["ANYWARE"],
        "menupath" : ["Compute code", "Run environment"],
        "title" : "Get current ware ID",
        "text" : "OPENFLUID_GetWareID()"
      }
      @endcond
    */
    WareID_t OPENFLUID_GetWareID() const
    { 
      return m_WareID;
    }


    /**
      Returns the type of the ware (itself)
      @return the type
    */
    WareType OPENFLUID_GetWareType() const
    { 
      return m_WareType;
    }


    PluggableWare(WareType WType);


  public:

    PluggableWare() = default;

    PluggableWare(const PluggableWare&) = delete;
    
    PluggableWare(PluggableWare&&) = default;

    PluggableWare& operator=(const PluggableWare&) = delete;

    PluggableWare& operator=(PluggableWare&&) = default;

    virtual ~PluggableWare() = default;

    void linkToRunEnvironment(const openfluid::core::MapValue* Env)
    {
      mp_WareEnv = Env;
    }

    virtual void initializeWare(const WareID_t& ID);

    virtual void finalizeWare();

    /**
     * Return false if ParameterKey starts nor ends with a dot, true otherwise
     */
    static bool isWellFormatted(const openfluid::ware::WareParamKey_t& ParameterKey);

};


} }  // namespaces


#endif /* __OPENFLUID_WARE_PLUGGABLEWARE_HPP__ */
