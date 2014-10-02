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
  \file PluggableWare.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __PLUGGABLEWARE_HPP__
#define __PLUGGABLEWARE_HPP__

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/base/EnvProperties.hpp>
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


// =====================================================================
// =====================================================================


typedef std::string WareParamKey_t;
typedef openfluid::core::StringValue WareParamValue_t;

typedef std::map<WareParamKey_t, WareParamValue_t> WareParams_t;


// =====================================================================
// =====================================================================


class DLLEXPORT PluggableWare
{
  public:

    enum WareType { UNDEFINED, OBSERVER, SIMULATOR, OTHER };


  private:

    /**
      Ware execution environment
    */
    const openfluid::base::EnvironmentProperties* mp_WareEnv;

    /**
      Simulator ID
    */
    WareID_t m_WareID;


  protected:

    virtual bool isLinked() const { return mp_WareEnv != NULL; };

    bool m_Initialized;

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseError(const std::string& Msg);

    /**
      Gets an environment string value associated to a Key
      @param[in] Key the sender of the message
      @param[out] Val the value associated with the environment key
    */
    bool OPENFLUID_GetRunEnvironment(std::string Key, std::string& Val);

    /**
      Gets an environment boolean value associated to a Key
      @param[in] Key the sender of the message
      @param[out] Val the value associated with the environment key
    */
    bool OPENFLUID_GetRunEnvironment(std::string Key, bool& Val);

    /**
      Returns the ID of the ware (itself)
      @return the ID
    */
    WareID_t OPENFLUID_GetWareID() const { return m_WareID; };

    WareType m_WareType;


    PluggableWare(WareType WType);

  public:

    virtual ~PluggableWare();

    void linkToRunEnvironment(const openfluid::base::EnvironmentProperties* Env)
    {
      mp_WareEnv = Env;
    };

    virtual void initializeWare(const WareID_t& ID);

    virtual void finalizeWare();

    /**
     * Return false if ParameterKey starts nor ends with a dot, true otherwise
     */
    static bool isWellFormated(const openfluid::ware::WareParamKey_t& ParameterKey);

    static boost::property_tree::ptree getParamsAsPropertyTree(
        const WareParams_t& Params);

};



} } // openfluid::ware



#endif /* __PLUGGABLEWARE_HPP__ */
