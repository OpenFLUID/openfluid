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
  \file PluggableBuilderExtension.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#ifndef __PLUGGABLEBUILDEREXTENSION_HPP__
#define __PLUGGABLEBUILDEREXTENSION_HPP__



#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/builderext/FluidXUpdateFlags.hpp>
#include <openfluid/dllexport.hpp>


/**
  Macro for declaration of builder extension and signature hooks
*/
#define DECLARE_BUILDEREXT_PLUGIN \
  extern "C" \
  { \
    DLLEXPORT std::string GetWareABIVersion(); \
    DLLEXPORT openfluid::builderext::PluggableBuilderExtension* GetWareBody(); \
    DLLEXPORT openfluid::builderext::BuilderExtensionSignature* GetWareSignature(); \
  }




// =====================================================================
// =====================================================================


/**
  Macro for definition of builder extension class hook
  @param[in] pluginclassname The name of the class to instantiate
*/
#define DEFINE_BUILDEREXT_CLASS(pluginclassname) \
  std::string GetWareABIVersion() \
  { \
    return std::string(openfluid::config::FULL_VERSION); \
  } \
  \
  openfluid::builderext::PluggableBuilderExtension* GetWareBody() \
  { \
    return new pluginclassname(); \
  }


// =====================================================================
// =====================================================================


namespace openfluid { namespace builderext {


class DLLEXPORT PluggableBuilderExtension : public openfluid::ware::PluggableWare
{

  protected:

    openfluid::fluidx::AdvancedFluidXDescriptor* mp_AdvancedDesc;

    openfluid::ware::WareParams_t m_Config;

  public:

    PluggableBuilderExtension() : PluggableWare(openfluid::ware::PluggableWare::OTHER)
    {

    }


    virtual ~PluggableBuilderExtension()
    {
      finalizeWare();
    }


    /**
      Internally called by the framework.
    */
    void initializeWare(const openfluid::ware::WareID_t& ID)
    {
      if (m_Initialized) return;

      PluggableWare::initializeWare(ID);
    }


    virtual void setConfiguration(const openfluid::ware::WareParams_t& Config)
    { m_Config = Config; }


    void setFluidXDescriptor(openfluid::fluidx::AdvancedFluidXDescriptor* Desc)
    { mp_AdvancedDesc = Desc; }


    openfluid::ware::WareID_t getID() const
    { return OPENFLUID_GetWareID(); }


    virtual bool initialize() = 0;
};


// =====================================================================
// =====================================================================


typedef PluggableBuilderExtension* (*GetPluggableBuilderExtensionBodyProc)();

typedef BuilderExtensionSignature* (*GetPluggableBuilderExtensionSignatureProc)();


} } // namespaces


#endif /* __PLUGGABLEBUILDEREXTENSION_HPP__ */
