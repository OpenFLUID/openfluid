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
  @file PluggableParameterizationExtension.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIBUILDEREXT_PLUGGABLEPARAMETERIZATIONEXTENSION_HPP__
#define __OPENFLUID_UIBUILDEREXT_PLUGGABLEPARAMETERIZATIONEXTENSION_HPP__


#include <QWidget>

#include <openfluid/builderext/PluggableBuilderExtension.hpp>


/**
  Macro for the beginning of definition of signature for parameterization extension
*/
#ifndef BEGIN_BUILDEREXT_SIGNATURE
#define BEGIN_BUILDEREXT_SIGNATURE(id) \
  extern "C" { \
  OPENFLUID_PLUGIN openfluid::builderext::BuilderExtensionSignature* WARESIGNATURE_PROC_DECL() \
    { \
      openfluid::builderext::BuilderExtensionSignature* Signature =\
        new openfluid::builderext::BuilderExtensionSignature(); \
      Signature->ID = (id); \
      Signature->Role = openfluid::builderext::ExtensionRole::PARAMETERIZATION; \
      Signature->BuildInfo.SDKVersion = openfluid::config::VERSION_FULL; \
      Signature->BuildInfo.BuildType = (WAREBUILD_BUILD_TYPE); \
      Signature->BuildInfo.CompilerID = (WAREBUILD_COMPILER_ID); \
      Signature->BuildInfo.CompilerVersion = (WAREBUILD_COMPILER_VERSION); \
      Signature->BuildInfo.CompilationFlags = (WAREBUILD_COMPILATION_FLAGS);
#endif

/**
  Macro for the end of definition of signature hook
*/
#ifndef END_BUILDEREXT_SIGNATURE
#define END_BUILDEREXT_SIGNATURE \
      return Signature; \
    } \
}
#endif


// =====================================================================
// =====================================================================


namespace openfluid { namespace ui { namespace builderext {


class OPENFLUID_API PluggableParameterizationExtension : public QWidget, 
                                                         public openfluid::builderext::PluggableBuilderExtension
{
  Q_OBJECT;

  protected:

    const openfluid::fluidx::FluidXDescriptor* mp_Desc;

    openfluid::ware::WareParams_t* mp_Params;


  signals:

    void changed();


  public:

    PluggableParameterizationExtension() : 
      QWidget(), PluggableBuilderExtension(),
      mp_Desc(nullptr), mp_Params(nullptr)
    {

    }


    void linkParams(openfluid::ware::WareParams_t* Params)
    {
      mp_Params = Params;
    }


    virtual ~PluggableParameterizationExtension()
    {

    }


    void setFluidXDescriptor(const openfluid::fluidx::FluidXDescriptor* Desc)
    {
      mp_Desc = Desc;
    }


    virtual void update() = 0;

};


} } }  // namespaces


#endif /* __OPENFLUID_UIBUILDEREXT_PLUGGABLEPARAMETERIZATIONEXTENSION_HPP__ */
