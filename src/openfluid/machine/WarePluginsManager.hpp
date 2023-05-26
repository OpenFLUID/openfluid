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
  @file WarePluginsManager.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_WAREPLUGINSMANAGER_HPP__
#define __OPENFLUID_MACHINE_WAREPLUGINSMANAGER_HPP__


#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>

#include <openfluid/machine/DynamicLib.hpp>
#include <openfluid/machine/WareContainer.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace machine {


typedef std::string* (*GetWareABIVersionProc)();

typedef std::string* (*GetWareLinkUIDProc)();


// =====================================================================
// =====================================================================


/**
  Management class for pluggable wares
  @tparam SignatureType class defining the signature type for the ware category
  @tparam BodyType class defining the body type for the ware category
  @tparam SignatureProc procedure definition for instantiation of the signature
  @tparam BodyProc procedure definition for instantiation of the body
*/
template<class SignatureType, class BodyType, typename SignatureProc, typename BodyProc>
class OPENFLUID_API WarePluginsManager
{
  private:

    /**
      Loads the plugin of a ware from the given file path. 
      The plugin is automatically cached to avoid multiple loadings.
      @param[in] FullFilePath The path to the plugin file to load
      @return a DynamicLib pointer to the loaded or cached ware
    */
    std::unique_ptr<DynamicLib>& loadPluginLibrary(const std::string& FullFilePath)
    {
      std::string PluginFileName = openfluid::tools::FilesystemPath(FullFilePath).filename();

      if (m_LoadedPluginsLibraries.find(PluginFileName) == m_LoadedPluginsLibraries.end())
      {
        m_LoadedPluginsLibraries[PluginFileName] = std::make_unique<DynamicLib>(FullFilePath);
      }

      return m_LoadedPluginsLibraries[PluginFileName];
    }


    // =====================================================================
    // =====================================================================


    std::string buildPluginFilename(const openfluid::ware::WareID_t& ID) const
    {
      return (ID+getPluginFilenameSuffix()+openfluid::config::PLUGINS_EXT);
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a container for the first plugin found with the given filename 
      according to current ware type and search paths
      @param[in] Filename The plugin filename (only filename, no path)
      @param[in] StrictABICheck If true, strict checking of ABI version of the plugins is enabled (default). 
                                Performs loose checking if false.
      @return A container corresponding to the found plugin. Container is marked as not valid if no plugin found
    */
    WareContainer<SignatureType> buildWareContainerFromFilename(const std::string& Filename,bool StrictABICheck = true)
    {
      WareContainer<SignatureType> Container = createContainer();
      std::string PluginFullPath = getPluginFullPath(Filename);

      if (!PluginFullPath.empty())
      {
        auto& PlugLib = loadPluginLibrary(PluginFullPath);

        if (PlugLib)
        {
          Container.setPath(PluginFullPath);

          if (PlugLib->isLoaded() || PlugLib->load())  
          {
            GetWareABIVersionProc ABIVersionProc = 
              PlugLib->template getSymbol<GetWareABIVersionProc>(WAREABIVERSION_PROC_NAME);

            bool Verified = false;
            if (ABIVersionProc)
            {
              std::unique_ptr<std::string> ABIPtr(ABIVersionProc());
              Verified = 
                (openfluid::tools::compareOpenFLUIDVersions(openfluid::config::VERSION_FULL,*ABIPtr,
                                                            StrictABICheck) == 0);
            }

            if (Verified)
            {
              bool hasBodyProc = PlugLib->hasSymbol(WAREBODY_PROC_NAME);
              SignatureProc SProc = PlugLib->template getSymbol<SignatureProc>(WARESIGNATURE_PROC_NAME);

              // checks if the handle procs exist
              if (SProc && hasBodyProc)
              {
                auto SignPtr = SProc();

                if (SignPtr)
                {
                  Verified = (Filename == buildPluginFilename(SignPtr->ID));

                  if (Verified)
                  {
                    Container.setSignature(SignPtr);

                    GetWareLinkUIDProc LinkUIDProc = 
                      PlugLib->template  getSymbol<GetWareLinkUIDProc>(WARELINKUID_PROC_NAME);

                    if (LinkUIDProc)
                    {
                      std::unique_ptr<std::string> StrPtr(LinkUIDProc());
                      Container.setLinkUID(*StrPtr);
                    }
                  }
                  else
                  {
                    Container.setMessage("ID mismatch in signature");
                  }
                }
                else
                {
                  Container.setMessage("signature cannot be instanciated");
                }
              }
              else
              {
                Container.setMessage("plugin format error");
              }
            }
            else
            {
              Container.setMessage("plugin ABI version mismatch");
            }
          }
          else
          {
            Container.setMessage(PlugLib->getLatestErrorMsg());
          }

          Container.validate();
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "unable to find plugin file " + Filename);
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "empty path for plugin file " + Filename);
      }

      return Container;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns a container for the first plugin found with the given ID according to current ware type and search paths
      @param[in] ID The plugin ID
      @param[in] StrictABICheck If true, strict checking of ABI version of the plugins is enabled (default). 
                                Performs loose checking if false.
      @return A container corresponding to the found plugin. Container is marked as not valid if no plugin found
    */
    WareContainer<SignatureType> buildWareContainerFromID(const std::string& ID, bool StrictABICheck = true)
    {
      return buildWareContainerFromFilename(buildPluginFilename(ID),StrictABICheck);
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns an empty built container
      @return The container
    */
    virtual WareContainer<SignatureType> createContainer() const = 0;


    // =====================================================================
    // =====================================================================


  protected:

    std::map<std::string,std::unique_ptr<DynamicLib>> m_LoadedPluginsLibraries;

    /**
      Default constructor
    */
    WarePluginsManager()
    {
      openfluid::base::Environment::init();
    }


    // =====================================================================
    // =====================================================================


  public:

    virtual ~WarePluginsManager()
    {  }


    // =====================================================================
    // =====================================================================


    /**
      Returns the full path of the plugin from its filename
      @param[in] Filename The filename of the plugin
      @return The full path of the plugin, empty if not found
    */
    virtual std::string getPluginFullPath(const std::string& Filename) const = 0;


    // =====================================================================
    // =====================================================================


    /**
      Returns ordered search paths for plugins
      @return The search paths
    */
    virtual std::vector<std::string> getPluginsSearchPaths() const = 0;


    // =====================================================================
    // =====================================================================


    /**
      Returns the filename suffix for the plugins
      @return The filename suffix
    */
    virtual std::string getPluginFilenameSuffix() const = 0;


    // =====================================================================
    // =====================================================================


    /**
      Loads only the signature of a ware given by its ID
      @param[in] ID The ID of the ware to load
      @return The ware container including the signature
    */
    WareContainer<SignatureType> loadPlugin(const std::string& ID)
    {
      try 
      {
        return buildWareContainerFromID(ID);
      }
      catch (openfluid::base::FrameworkException&)
      {
        return createContainer();
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Lists available wares
      @param[in] IDPattern if not empty, the list of available wares is filtered using the given pattern 
                           based on wildcard matching
      @return The list of found wares
    */
    std::vector<WareContainer<SignatureType>> loadPlugins(const std::string& IDPattern = "")
    {
      std::vector<WareContainer<SignatureType>> Containers;
      std::vector<std::string> PluginsPaths = getPluginsSearchPaths();
      std::vector<std::string> PluginFiles;
      std::vector<std::string> TmpFiles;

      for (const auto& Path: PluginsPaths)
      {
        TmpFiles = openfluid::tools::Filesystem::findFilesBySuffixAndExtension(Path,
                                                                               getPluginFilenameSuffix(),
                                                                               openfluid::config::PLUGINS_EXT,
                                                                               false,true);

        for (const auto& File: TmpFiles)
        {
          PluginFiles.push_back(File);
        }
      }


      for (const auto& File: PluginFiles)
      {
        try
        {
          auto Container = buildWareContainerFromFilename(File);

          if (Container.isValid() &&
              (IDPattern.empty() || openfluid::tools::matchWithWildcard(IDPattern,Container.signature()->ID)))
          {
            Containers.emplace_back(std::move(Container));
          }
        }
        catch (openfluid::base::FrameworkException& E)
        {          
          throw E;
        }
      }

      return Containers;
    }


    // =====================================================================
    // =====================================================================


    /**
      Loads only the body of a ware if the signature is already loaded
      @param[in] Container The ware container which already includes the signature
      @return the instanciated body of the ware
    */
    BodyType* getWareBody(const WareContainer<SignatureType>& Container)
    {
      std::string PluginFullPath = Container.getPath();

      std::unique_ptr<DynamicLib>& PlugLib = loadPluginLibrary(PluginFullPath);

      if (PlugLib) 
      {
        if (PlugLib->isLoaded() || PlugLib->load())
        {
          BodyProc BProc = PlugLib->template getSymbol<BodyProc>(WAREBODY_PROC_NAME);

          // checks if the handle proc exists
          if (BProc)
          {
            BodyType* Body = BProc();
            return Body;
          }
          else
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "format error in plugin file " + PluginFullPath);
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "unable to load plugin file " + PluginFullPath);
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "unable to find plugin file " + PluginFullPath);
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Unloads all already loaded plugins and clears the cache
    */
    void unloadAll()
    {
      for (const auto& PlugLib : m_LoadedPluginsLibraries)
      {
        PlugLib.second->unload();
      }

      m_LoadedPluginsLibraries.clear();
    }

};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_WAREPLUGINSMANAGER_HPP__ */
