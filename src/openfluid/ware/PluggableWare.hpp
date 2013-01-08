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
  \file PluggableWare.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __PLUGGABLEWARE_HPP__
#define __PLUGGABLEWARE_HPP__

#include <boost/filesystem/path.hpp>
#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/base/ExecMsgs.hpp>
#include <openfluid/base/EnvProperties.hpp>
#include <openfluid/ware/FunctionSignature.hpp>
#include <openfluid/base/StdoutFileOStream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>

namespace openfluid { namespace ware {


/**
  Hook function name of ware plugin body
*/
#define WAREBODY_PROC_NAME "GetWareBody"

/**
  Hook function name of ware plugin signature
*/
#define WARESIGNATURE_PROC_NAME "GetWareSignature"

/**
  Hook function name of ware ABI version
*/
#define WAREABIVERSION_PROC_NAME "GetWareABIVersion"

// =====================================================================
// =====================================================================

typedef std::string WareParamKey_t;
typedef std::string WareParamValue_t;

typedef boost::property_tree::ptree WareParams_t;


// =====================================================================
// =====================================================================


class DLLEXPORT PluggableWare
{
  private:

    /**
      Ware execution environment
    */
    const openfluid::base::EnvironmentProperties* mp_WareEnv;

    /**
      Function ID
    */
    WareID_t m_WareID;


  protected:

    bool isLinked() { return (mp_WareEnv != NULL && mp_ExecMsgs != NULL); };

    /**
      Pointer to the execution messages repository
     */
    openfluid::base::ExecutionMessages* mp_ExecMsgs;


    /**
      Raises a warning message to the kernel. This do not stops the simulation
      @param[in] Sender the sender of the message
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseWarning(std::string Sender, std::string Msg);

    /**
      Raises a warning message to the kernel. This do not stops the simulation
      @param[in] Sender the sender of the message
      @param[in] Source the source of the message
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseWarning(std::string Sender, std::string Source, std::string Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Sender the sender of the message
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseError(std::string Sender, std::string Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Sender the sender of the message
      @param[in] Source of the message (location in the sender)
      @param[in] Msg the content of the message
    */
    virtual void OPENFLUID_RaiseError(std::string Sender, std::string Source, std::string Msg);

    /**
      Gets an environment string value associated to a Key
      @param[in] Key the sender of the message
      @param[out] Val the value associated with the environment key
    */
    bool OPENFLUID_GetRunEnvironment(std::string Key, std::string *Val);

    /**
      Gets an environment boolean value associated to a Key
      @param[in] Key the sender of the message
      @param[out] Val the value associated with the environment key
    */
    bool OPENFLUID_GetRunEnvironment(std::string Key, bool *Val);

    WareID_t OPENFLUID_GetWareID() { return m_WareID; };

    openfluid::base::StdoutAndFileOutputStream OPENFLUID_Logger;


  public:

    PluggableWare()
    : mp_ExecMsgs(NULL),mp_WareEnv(NULL),m_WareID("")
    {};

    virtual ~PluggableWare() {};

    void linkToExecutionMessages(openfluid::base::ExecutionMessages* ExecMsgs)
    {
      mp_ExecMsgs = ExecMsgs;
    };

    void linkToRunEnvironment(const openfluid::base::EnvironmentProperties* Env)
    {
      mp_WareEnv = Env;
    };

    void initializeWare(const WareID_t& ID);

    void finalizeWare();

};



} } // openfluid::ware



#endif /* __PLUGGABLEWARE_HPP__ */
