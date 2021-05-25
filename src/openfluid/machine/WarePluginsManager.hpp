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
#include <openfluid/machine/WarePluginsSearchResults.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace machine {


typedef std::string* (*GetWareABIVersionProc)();

typedef std::string* (*GetWareLinkUIDProc)();


// =====================================================================
// =====================================================================


/**
  Management class for pluggable wares
  @tparam SignatureInstanceType class defining the container for ware signature only
  @tparam ItemInstanceType class defining the container for ware signature and body
  @tparam SignatureProc procedure definition for instantiation of the signature
  @tparam BodyProc procedure definition for instantiation of the body
*/
template<class SignatureInstanceType, class ItemInstanceType, typename SignatureProc, typename BodyProc>
class OPENFLUID_API WarePluginsManager
{
  private:

    /**
      Loads the plugin of a ware from the given file path. 
      The plugin is automatically cached to avoid multiple loadings.
      @param[in] FullFilePath The path to the plugin file to load
      @return a DynamicLib pointer to the loaded or cached ware
    */
    DynamicLib* loadPluginLibrary(const std::string& FullFilePath)
    {
      std::string PluginFileName = openfluid::tools::Filesystem::filename(FullFilePath);

      if (m_LoadedPluginsLibraries.find(PluginFileName) == m_LoadedPluginsLibraries.end())
      {
        m_LoadedPluginsLibraries[PluginFileName] = std::make_unique<DynamicLib>(FullFilePath);
      }

      return m_LoadedPluginsLibraries[PluginFileName].get();
    }


    // =====================================================================
    // =====================================================================


    ItemInstanceType* buildWareContainerWithSignatureOnly(const std::string& ID, bool StrictABICheck = true)
    {
      std::string PluginFilename = ID+getPluginFilenameSuffix()+openfluid::config::PLUGINS_EXT;
      std::string PluginFullPath = getPluginFullPath(PluginFilename);
      ItemInstanceType* WareItem = nullptr;

      // library loading
      DynamicLib* PlugLib = loadPluginLibrary(PluginFullPath);

      if (PlugLib  != nullptr)
      {
        if (PlugLib->load())  
        {
          WareItem = new ItemInstanceType();
          WareItem->FileFullPath = PluginFullPath;

          GetWareABIVersionProc ABIVersionProc = PlugLib->getSymbol<GetWareABIVersionProc>(WAREABIVERSION_PROC_NAME);

          if (ABIVersionProc)
          {
            std::unique_ptr<std::string> StrPtr(ABIVersionProc());
            WareItem->Verified = 
              (openfluid::tools::compareVersions(openfluid::config::VERSION_FULL,*StrPtr,StrictABICheck) == 0);
          }
          else
          {
            WareItem->Verified = false;
          }


          if (WareItem->Verified)
          {
            BodyProc BProc = PlugLib->getSymbol<BodyProc>(WAREBODY_PROC_NAME);
            SignatureProc SProc = PlugLib->getSymbol<SignatureProc>(WARESIGNATURE_PROC_NAME);

            // checks if the handle procs exist
            if (SProc && BProc)
            {
              WareItem->Signature = SProc();

              if (WareItem->Signature == nullptr)
              {
                throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                          "Signature from plugin file " + PluginFilename +
                                                          " cannot be instanciated");
              }

              WareItem->Verified = (WareItem->Signature->ID == ID);

              WareItem->Body = 0;

              GetWareLinkUIDProc LinkUIDProc = PlugLib->getSymbol<GetWareLinkUIDProc>(WARELINKUID_PROC_NAME);

              if (LinkUIDProc)
              {
                std::unique_ptr<std::string> StrPtr(LinkUIDProc());
                WareItem->LinkUID = *StrPtr;
              }
            }
            else
            {
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                        "Format error in plugin file " + PluginFilename);
            }
          }
          else
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Compatibility version mismatch for plugin file " +
                                                      PluginFilename);
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "Unable to load plugin from file " + PluginFilename);
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Unable to find file " + PluginFilename);
      }

      return WareItem;
    }


    // =====================================================================
    // =====================================================================


    SignatureInstanceType* getWareSignature(const std::string& PluginFilename, bool StrictABICheck = true)
    {
      std::string PluginFullPath = getPluginFullPath(PluginFilename);
      SignatureInstanceType* Sign = nullptr;

      openfluid::base::ExceptionContext ECtxt =
          openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION)
          .addInfos({{"pluginfullpath",PluginFullPath},{"pluginfilename",PluginFilename}});

      // library loading
      DynamicLib* PlugLib = loadPluginLibrary(PluginFullPath);

      if (PlugLib != nullptr)
      {
        if (PlugLib->load())
        {
          Sign = new SignatureInstanceType();
          Sign->FileFullPath = PluginFullPath;

          GetWareABIVersionProc ABIVersionProc = PlugLib->getSymbol<GetWareABIVersionProc>(WAREABIVERSION_PROC_NAME);

          if (ABIVersionProc)
          {
            std::unique_ptr<std::string> StrPtr(ABIVersionProc());
            Sign->Verified = 
              (openfluid::tools::compareVersions(openfluid::config::VERSION_FULL,*StrPtr,StrictABICheck) == 0); 
          }
          else
          {
            Sign->Verified = false;
          }

          if (Sign->Verified)
          {
            BodyProc BProc = PlugLib->getSymbol<BodyProc>(WAREBODY_PROC_NAME);
            SignatureProc SProc = PlugLib->getSymbol<SignatureProc>(WARESIGNATURE_PROC_NAME);

            // checks if the handle procs exist
            if (SProc && BProc)
            {
              try
              {
                Sign->Signature = SProc();
              }
              catch (openfluid::base::FrameworkException& E)
              {
                throw openfluid::base::FrameworkException(ECtxt,E.getMessage());
              }

              if (Sign->Signature == nullptr)
              {
                throw openfluid::base::FrameworkException(ECtxt,"Signature cannot be instanciated from plugin file");
              }

              Sign->Verified = (PluginFilename.find(Sign->Signature->ID) == 0);

              GetWareLinkUIDProc LinkUIDProc = PlugLib->getSymbol<GetWareLinkUIDProc>(WARELINKUID_PROC_NAME);

              if (LinkUIDProc)
              {
                std::unique_ptr<std::string> StrPtr(LinkUIDProc());
                Sign->LinkUID = *StrPtr;
              }
            }
            else
            {
              throw openfluid::base::FrameworkException(ECtxt,"Format error in plugin file");
            }
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(ECtxt,PlugLib->getLatestErrorMsg());
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(ECtxt,"Unable to find plugin file " + PluginFilename);
      }

      return Sign;
    }


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

    typedef WarePluginsSearchResults<SignatureInstanceType> PluginsSearchResults;

    virtual ~WarePluginsManager()
    {

    }


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
      Lists available wares
      @param[in] Pattern if not empty, the list of available wares is filtered using the given pattern 
                         based on wildcard matching
      @return The list of found wares
    */
    PluginsSearchResults getAvailableWaresSignatures(const std::string& Pattern = "")
    {
      PluginsSearchResults SearchResults;
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


      SignatureInstanceType* CurrentPlug = nullptr;

      for (const auto& File: PluginFiles)
      {
        try
        {
          CurrentPlug = getWareSignature(File);

          if (CurrentPlug && CurrentPlug->Verified)
          {
            if (Pattern.empty())
            {
              SearchResults.appendAvailable(CurrentPlug);
            }
            else if (openfluid::tools::matchWithWildcard(Pattern,CurrentPlug->Signature->ID))
            {
              SearchResults.appendAvailable(CurrentPlug);
            }
          }
        }
        catch (openfluid::base::FrameworkException& E)
        {
          SearchResults.appendErrored(E.getContext().at("pluginfullpath"),E.getMessage());
        }
      }

      return SearchResults;
    }


    // =====================================================================
    // =====================================================================


    /**
      Loads only the signature of a ware given by its ID
      @param[in] ID The ID of the ware to load
      @return The ware container including the signature
    */
    ItemInstanceType* loadWareSignatureOnly(const std::string& ID)
    {
      ItemInstanceType* WareItem = buildWareContainerWithSignatureOnly(ID);

      if (WareItem != nullptr && WareItem->Verified)
      {
        return WareItem;
      }

      return nullptr;
    }


    // =====================================================================
    // =====================================================================


    /**
      Loads only the body of a ware if the signature is already loaded
      @param[inout] WareItem The ware container to complete which already includes the signature
    */
    void completeSignatureWithWareBody(ItemInstanceType* WareItem)
    {
      std::string PluginFullPath = WareItem->FileFullPath;

      DynamicLib* PlugLib = loadPluginLibrary(PluginFullPath);

      if (PlugLib != nullptr) 
      {
        if (PlugLib->load())
        {
          BodyProc BProc = PlugLib->getSymbol<BodyProc>(WAREBODY_PROC_NAME);

          // checks if the handle proc exists
          if (BProc)
          {
            WareItem->Body.reset(BProc());

            if (WareItem->Body == nullptr)
            {
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                        "Ware from plugin file " + PluginFullPath +
                                                        " cannot be instanciated");
            }

          }
          else
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                        "Format error in plugin file " + PluginFullPath);
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Unable to load plugin file " + PluginFullPath);
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Unable to find plugin file " + PluginFullPath);
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Unloads all already loaded wares and clears the cache
    */
    void unloadAllWares()
    {
      for (auto it=m_LoadedPluginsLibraries.begin();it != m_LoadedPluginsLibraries.end(); ++it)
      {
        it->second.get()->unload();
      }

      m_LoadedPluginsLibraries.clear();
    }

};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_WAREPLUGINSMANAGER_HPP__ */
