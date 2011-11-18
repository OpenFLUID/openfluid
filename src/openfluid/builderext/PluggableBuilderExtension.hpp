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


#include <string>

#ifndef __PLUGGABLEBUILDEREXTENSION_HPP__
#define __PLUGGABLEBUILDEREXTENSION_HPP__


#include <gtkmm/widget.h>

#include <openfluid/dllexport.hpp>
#include <openfluid/config.hpp>
#include <openfluid/machine/SimulationBlob.hpp>


// =====================================================================
// =====================================================================

/**
  Extension hook name
*/
#define EXTENSION_PROC_NAME "GetExtension"

/**
  Infos hook name
*/
#define EXTINFOS_PROC_NAME "GetExtensionInfos"

/**
  SDK version hook name
*/
#define EXTSDKVERSION_PROC_NAME "GetExtensionSDKVersion"


// =====================================================================
// =====================================================================


/**
  Macro for declaration of extension and infos hooks
*/
#define DECLARE_EXTENSION_HOOKS \
  extern "C" \
  { \
    DLLEXPORT std::string GetExtensionSDKVersion(); \
    DLLEXPORT openfluid::builderext::PluggableBuilderExtension* GetExtension(); \
    DLLEXPORT openfluid::builderext::BuilderExtensionInfos GetExtensionInfos(); \
  }


/**
  Macro for definition of extension hook
  @param[in] pluginclassname The name of the class to instanciate
*/
#define DEFINE_EXTENSION_HOOKS(pluginclassname) \
  std::string GetExtensionSDKVersion() \
  { \
    return std::string(openfluid::config::FULL_VERSION); \
  } \
  \
  openfluid::builderext::PluggableBuilderExtension* GetExtension() \
  { \
    return new pluginclassname(); \
  }


#define DEFINE_EXTENSION_INFOS(id,shortname,name,desc,authors,authorsctct) \
  openfluid::builderext::BuilderExtensionInfos GetExtensionInfos() \
  { \
    openfluid::builderext::BuilderExtensionInfos BEI; \
    \
    BEI.ID = (id); \
    BEI.ShortName = (shortname); \
    BEI.Name = (name); \
    BEI.Description = (desc); \
    BEI.Authors = (authors); \
    BEI.AuthorsContact = (authorsctct); \
    \
    return BEI; \
  }



// =====================================================================
// =====================================================================


namespace openfluid { namespace builderext {


class DLLEXPORT BuilderExtensionInfos
{
  public:

    std::string ID;

    std::string Name;

    std::string ShortName;

    std::string Description;

    std::string Authors;

    std::string AuthorsContact;

    BuilderExtensionInfos()
    : ID(""), Name(""), ShortName(""), Description(""), Authors(""), AuthorsContact("")
    {

    }
};


// =====================================================================
// =====================================================================


typedef std::map<std::string,std::string> ExtensionConfig_t;


// =====================================================================
// =====================================================================


class DLLEXPORT PluggableBuilderExtension
{
  protected:

    ExtensionConfig_t m_Config;

    Gtk::Widget* m_PrefsPanelWidget;

    openfluid::machine::SimulationBlob* mp_SimulationBlob;


  public:

    enum ExtensionType { WorkspaceTab, ModelessWindow, ModalWindow,
                         SpatialgraphImporter, InputdataImporter, EventsImporter, ExtraImporter, MixedImporter,
                         SimulationListener, HomeLauncher };


    PluggableBuilderExtension() : m_PrefsPanelWidget(NULL), mp_SimulationBlob(NULL) { };

    virtual ~PluggableBuilderExtension() { };


    void setSimulationBlob(openfluid::machine::SimulationBlob* Blob) { mp_SimulationBlob = Blob; };

    /**
      Returns the type of the extension. This must be overridden.
      @returns the type of the extension
    */
    virtual ExtensionType getType() const = 0;


    /**
      Returns true if the extension is configurable, false otherwise.
      @returns true if the extension is configurable
    */
    virtual bool isConfigurable() { return false; };


    /**
      Retrieves the configuration information from the extension.
      @return the configuration information
    */
    ExtensionConfig_t getConfiguration() const { return m_Config; };


    /**
      Gives the configuration information to the extension.
      @param[in] Config the configuration information
    */
    void setConfiguration(const ExtensionConfig_t& Config) { m_Config = Config; };


    /**
      Returns the main widget for the preferences panel of the extension.
      Default is NULL, so no preference panel will be shown for this extension.
      This should be overridden in derived extensions.
      @returns a pointer to widget for the preferences panel
    */
    Gtk::Widget* getPrefsPanelAsWidget() { return m_PrefsPanelWidget; };


    /**
      Returns the main widget of the extension. The kind of widget depends on the
      extension type.
      This must be overridden in derived extensions
      @returns a pointer to the main widget
    */
    virtual Gtk::Widget* getExtensionAsWidget() = 0;

    virtual void show() = 0;


    /**
      Returns true if the extension is currently ready to use (showtime!).
      Default is false, but this should be overridden in derived extensions
      @returns  a boolean giving the state of the extension
    */
    virtual bool isReadyForShowtime() const { return false; };

};



// =====================================================================
// =====================================================================


typedef PluggableBuilderExtension* (*GetExtensionProc)();

typedef BuilderExtensionInfos (*GetExtensionInfosProc)();

typedef std::string (*GetExtensionSDKVersionProc)();


} } // namespaces

#endif /* __PLUGGABLEBUILDEREXTENSION_HPP__ */
