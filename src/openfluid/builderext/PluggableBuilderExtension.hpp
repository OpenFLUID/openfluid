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
#include <set>

#ifndef __PLUGGABLEBUILDEREXTENSION_HPP__
#define __PLUGGABLEBUILDEREXTENSION_HPP__


#include <gtkmm/widget.h>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <boost/preprocessor/comparison/equal.hpp>

#include <openfluid/dllexport.hpp>
#include <openfluid/config.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/guicommon/PreferencesPanel.hpp>


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
  Prefs hook name
*/
#define EXTPREFS_PROC_NAME "GetExtensionPrefs"

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
    DLLEXPORT openfluid::builderext::BuilderExtensionPrefs* GetExtensionPrefs(); \
    DLLEXPORT std::set<std::string> GetDefaultConfig(); \
  }


#define EXT_PREFS_CLASS_DEFINED(seq) \
  BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(seq),2)

#define EXT_RETURN_NEW(list) \
  return new BOOST_PP_SEQ_ELEM(1,BOOST_PP_SEQ_PUSH_BACK(list,dummy))();

#define EXT_RETURN_NULL return (openfluid::builderext::BuilderExtensionPrefs*)0;

/**
  Macro for definition of extension hook
  @param[in] pluginclassnames The names of the classes to instantiate
  in the form of a group of adjacent parenthesized elements:
  DEFINE_EXTENSION_HOOKS((pluginclassname)) or DEFINE_EXTENSION_HOOKS((pluginclassname) (pluginprefsclassname))
*/
#define DEFINE_EXTENSION_HOOKS(pluginclassnames) \
  std::string GetExtensionSDKVersion() \
  { \
    return std::string(openfluid::config::FULL_VERSION); \
  } \
  \
  openfluid::builderext::PluggableBuilderExtension* GetExtension() \
  { \
    openfluid::builderext::PluggableBuilderExtension* Ext = new BOOST_PP_SEQ_ELEM(0,pluginclassnames)(); \
      Ext->setDefaultConfiguration(GetDefaultConfig()); \
    return Ext; \
  } \
  \
  openfluid::builderext::BuilderExtensionPrefs* GetExtensionPrefs() \
  { \
    BOOST_PP_IF(EXT_PREFS_CLASS_DEFINED(pluginclassnames),EXT_RETURN_NEW(pluginclassnames),EXT_RETURN_NULL) \
  }

#define DEFINE_EXTENSION_INFOS(id,shortname,name,desc,authors,authorsctct,type) \
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
    BEI.Type = (type); \
    \
    return BEI; \
  }


#define ADD_TO_SET(r,data,elem) \
  data.insert(elem);

#define DEFINE_EXTENSION_DEFAULT_CONFIG(seq)       \
    std::set<std::string> GetDefaultConfig()       \
    {                                              \
      std::set<std::string> config;                \
      BOOST_PP_SEQ_FOR_EACH(ADD_TO_SET,config,seq) \
      return config;                               \
    }


// =====================================================================
// =====================================================================


namespace openfluid { namespace builderext {



typedef std::map<std::string,std::string> ExtensionConfig_t;


// =====================================================================
// =====================================================================


class DLLEXPORT PluggableBuilderExtension
{
  protected:

    ExtensionConfig_t m_Config;

    sigc::signal<void> m_signal_ChangedOccurs;

    openfluid::machine::SimulationBlob* mp_SimulationBlob;

    openfluid::machine::ModelInstance* mp_ModelInstance;


  public:

    enum ExtensionType { WorkspaceTab, ModelessWindow, ModalWindow,
                         SpatialgraphImporter, InputdataImporter, EventsImporter, ExtraImporter, MixedImporter,
                         SimulationListener, HomeLauncher };


    PluggableBuilderExtension() : mp_SimulationBlob(NULL) { };


    virtual ~PluggableBuilderExtension() { };


    void setSimulationBlobAndModel(openfluid::machine::SimulationBlob* Blob, openfluid::machine::ModelInstance* Model)
      { mp_SimulationBlob = Blob; mp_ModelInstance = Model; };


    sigc::signal<void> signal_ChangedOccurs()
    {
      return m_signal_ChangedOccurs;
    }

    void setDefaultConfiguration(std::set<std::string> DefaultConfig)
    {
      for(std::set<std::string>::iterator it = DefaultConfig.begin() ; it != DefaultConfig.end() ; ++it)
      {
        std::vector<std::string> Splitted = openfluid::tools::SplitString(*it,"=");

        if(Splitted.size() != 2)
          throw openfluid::base::OFException("OpenFLUID framework","PluggableBuilderExtension::setDefaultConfig",
              "Configuration element \"" + Splitted[0] + "\" is not well formatted.");

        m_Config[Splitted[0]] = Splitted[1];
      }
    }

    /**
      Returns the type of the extension. This must be overridden.
      @return the type of the extension
    */
    virtual ExtensionType getType() const = 0;


    /**
      Returns true if the extension is configurable, false otherwise.
      @return true if the extension is configurable
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
      Returns the main widget of the extension. The kind of widget depends on the
      extension type.
      This must be overridden in derived extensions
      @return a pointer to the main widget
    */
    virtual Gtk::Widget* getExtensionAsWidget() = 0;


    virtual void show() = 0;


    /**
      Returns true if the extension is currently ready to use (showtime!).
      Default is false, but this should be overridden in derived extensions
      @return  a boolean giving the state of the extension
    */
    virtual bool isReadyForShowtime() const { return false; };

};


// =====================================================================
// =====================================================================


class DLLEXPORT BuilderExtensionInfos
{
  public:

    std::string ID;

    std::string Name;

    std::string ShortName;

    std::string Description;

    std::string Authors;

    std::string AuthorsContact;

    PluggableBuilderExtension::ExtensionType Type;

    BuilderExtensionInfos()
    : ID(""), Name(""), ShortName(""), Description(""), Authors(""), AuthorsContact(""),
      Type(PluggableBuilderExtension::ModalWindow)
    {

    }
};


// =====================================================================
// =====================================================================


class DLLEXPORT BuilderExtensionPrefs : public openfluid::guicommon::PreferencesPanel
{
  public:

    BuilderExtensionPrefs(Glib::ustring PanelTitle) :
      openfluid::guicommon::PreferencesPanel(PanelTitle) { };

    virtual ~BuilderExtensionPrefs() { };

};


// =====================================================================
// =====================================================================


typedef PluggableBuilderExtension* (*GetExtensionProc)();

typedef BuilderExtensionInfos (*GetExtensionInfosProc)();

typedef BuilderExtensionPrefs* (*GetExtensionPrefsProc)();

typedef std::string (*GetExtensionSDKVersionProc)();


} } // namespaces

#endif /* __PLUGGABLEBUILDEREXTENSION_HPP__ */
