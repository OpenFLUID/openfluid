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

#include <QLibrary>
#include <QFileInfo>

#include <openfluid/base/Environment.hpp>
#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace machine {


typedef std::string (*GetWareABIVersionProc)();

typedef std::string (*GetWareLinkUIDProc)();


// =====================================================================
// =====================================================================


/**
  Management class for pluggable ware
  @tparam SignatureType class defining the container for ware signature only
  @tparam ItemType class defining the container for ware signature and body
  @tparam SignatureProc procedure definition for instantiation of the signature
  @tparam BodyProc procedure definition for instantiation of the body
*/
template<class SignatureType, class ItemType, typename SignatureProc, typename BodyProc>
class OPENFLUID_API WarePluginsManager
{
  private:

    QLibrary* loadPluginLibrary(const std::string& FullFilePath)
    {
      std::string PluginFileName = QFileInfo(QString::fromStdString(FullFilePath)).fileName().toStdString();

      if (m_LoadedPluginsLibraries.find(PluginFileName) == m_LoadedPluginsLibraries.end())
      {
        m_LoadedPluginsLibraries[PluginFileName] = std::make_unique<QLibrary>(QString::fromStdString(FullFilePath));
      }

      return m_LoadedPluginsLibraries[PluginFileName].get();
    }


    // =====================================================================
    // =====================================================================


    ItemType* buildWareContainerWithSignatureOnly(const std::string& ID)
    {
      std::string PluginFilename = ID+getPluginFilenameSuffix()+openfluid::config::PLUGINS_EXT;
      std::string PluginFullPath = getPluginFullPath(PluginFilename);
      ItemType* WareItem = nullptr;

      QLibrary* PlugLib = loadPluginLibrary(PluginFullPath);

      // library loading
      if (PlugLib && PlugLib->load())
      {
        WareItem = new ItemType();
        WareItem->FileFullPath = PluginFullPath;

        GetWareABIVersionProc ABIVersionProc = (GetWareABIVersionProc)PlugLib->resolve(WAREABIVERSION_PROC_NAME);

        if (ABIVersionProc)
        {
          WareItem->Verified =
              (openfluid::tools::compareVersions(openfluid::config::VERSION_FULL,ABIVersionProc(),false) == 0);
        }
        else
        {
          WareItem->Verified = false;
        }


        if (WareItem->Verified)
        {
          BodyProc BProc = (BodyProc)PlugLib->resolve(WAREBODY_PROC_NAME);
          SignatureProc SProc = (SignatureProc)PlugLib->resolve(WARESIGNATURE_PROC_NAME);

          // checks if the handle procs exist
          if (SProc && BProc)
          {
            WareItem->Signature = SProc();

            if (WareItem->Signature == nullptr)
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                        "Signature from plugin file " + PluginFilename +
                                                        " cannot be instanciated");

            WareItem->Verified = (WareItem->Signature->ID == ID);

            WareItem->Body = 0;

            GetWareLinkUIDProc LinkUIDProc = (GetWareLinkUIDProc)PlugLib->resolve(WARELINKUID_PROC_NAME);

            if (LinkUIDProc)
              WareItem->LinkUID = LinkUIDProc();
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
                                                    "Compatibility version mismatch for plugin file " + PluginFilename);
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Unable to load plugin from file " + PluginFilename);
      }

      return WareItem;
    }


    // =====================================================================
    // =====================================================================


    SignatureType* getWareSignature(const std::string& PluginFilename)
    {
      std::string PluginFullPath = getPluginFullPath(PluginFilename);
      SignatureType* Sign = nullptr;

      openfluid::base::ExceptionContext ECtxt =
          openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION)
          .addInfos({{"pluginfullpath",PluginFullPath},{"pluginfilename",PluginFilename}});

      // library loading
      QLibrary* PlugLib = loadPluginLibrary(PluginFullPath);


      if (PlugLib)
      {
        if (PlugLib->load())
        {
          Sign = new SignatureType();
          Sign->FileFullPath = PluginFullPath;

          GetWareABIVersionProc ABIVersionProc = (GetWareABIVersionProc)PlugLib->resolve(WAREABIVERSION_PROC_NAME);

          if (ABIVersionProc)
          {
            Sign->Verified =
              (openfluid::tools::compareVersions(openfluid::config::VERSION_FULL,ABIVersionProc(),false) == 0);
          }
          else
          {
            Sign->Verified = false;
          }

          if (Sign->Verified)
          {
            BodyProc BProc = (BodyProc)PlugLib->resolve(WAREBODY_PROC_NAME);
            SignatureProc SProc = (SignatureProc)PlugLib->resolve(WARESIGNATURE_PROC_NAME);

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

              Sign->Verified =
                  QString::fromStdString(PluginFilename).startsWith(QString::fromStdString(Sign->Signature->ID));

              GetWareLinkUIDProc LinkUIDProc = (GetWareLinkUIDProc)PlugLib->resolve(WARELINKUID_PROC_NAME);

              if (LinkUIDProc)
                Sign->LinkUID = LinkUIDProc();
            }
            else
            {
              throw openfluid::base::FrameworkException(ECtxt,"Format error in plugin file");
            }
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(ECtxt,PlugLib->errorString().toStdString());
        }
      }
      else
      {
        throw openfluid::base::FrameworkException(ECtxt,"Unable to find plugin file");
      }

      return Sign;
    }


    // =====================================================================
    // =====================================================================


  protected:

    std::map<std::string,std::unique_ptr<QLibrary>> m_LoadedPluginsLibraries;


    WarePluginsManager()
    {
      openfluid::base::Environment::init();
    }


    // =====================================================================
    // =====================================================================


  public:

    class PluginsSearchResults
    {
      public:

        std::vector<SignatureType*> AvailablePlugins;

        std::map<std::string,std::string> ErroredFiles;
    };


    virtual ~WarePluginsManager()
    {

    }


    // =====================================================================
    // =====================================================================


    virtual std::string getPluginFullPath(const std::string& Filename) const = 0;


    // =====================================================================
    // =====================================================================


    virtual std::vector<std::string> getPluginsSearchPaths() const = 0;


    // =====================================================================
    // =====================================================================


    virtual std::string getPluginFilenameSuffix() const = 0;


    // =====================================================================
    // =====================================================================


    /**
      Lists available wares
    */
    PluginsSearchResults getAvailableWaresSignatures(const std::string& Pattern = "")
    {
      PluginsSearchResults SearchResults;
      std::vector<std::string> PluginsPaths = getPluginsSearchPaths();
      std::vector<std::string> PluginFiles;
      std::vector<std::string> TmpFiles;
      unsigned int i,j;


      for (i=0;i<PluginsPaths.size();i++)
      {
        TmpFiles = openfluid::tools::findFilesBySuffixAndExtension(PluginsPaths[i],
                                                                   getPluginFilenameSuffix(),
                                                                   openfluid::config::PLUGINS_EXT,false,true);

        for (j=0;j<TmpFiles.size();j++)
          PluginFiles.push_back(TmpFiles[j]);
      }


      SignatureType* CurrentPlug = nullptr;

      for (i=0;i<PluginFiles.size();i++)
      {
        try
        {
          CurrentPlug = getWareSignature(PluginFiles[i]);

          if (CurrentPlug && CurrentPlug->Verified)
          {
            if (Pattern.empty())
            {
              SearchResults.AvailablePlugins.push_back(CurrentPlug);
            }
            else if (openfluid::tools::matchWithWildcard(Pattern,CurrentPlug->Signature->ID))
            {
              SearchResults.AvailablePlugins.push_back(CurrentPlug);
            }
          }
        }
        catch (openfluid::base::FrameworkException& E)
        {
          SearchResults.ErroredFiles[E.getContext().at("pluginfullpath")] = E.getMessage();
        }
      }

      return SearchResults;
    }


    // =====================================================================
    // =====================================================================


    ItemType* loadWareSignatureOnly(const std::string& ID)
    {
      ItemType* WareItem = buildWareContainerWithSignatureOnly(ID);

      if (WareItem != nullptr && WareItem->Verified)
      {
        return WareItem;
      }

      return nullptr;
    }


    // =====================================================================
    // =====================================================================


    void completeSignatureWithWareBody(ItemType* WareItem)
    {
      std::string PluginFullPath = WareItem->FileFullPath;

      QLibrary* PlugLib = loadPluginLibrary(PluginFullPath);

      // library loading
      if (PlugLib && PlugLib->load())
      {
        BodyProc BProc = (BodyProc)PlugLib->resolve(WAREBODY_PROC_NAME);

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
                                                     "Unable to find plugin file " + PluginFullPath);
      }
    }


    // =====================================================================
    // =====================================================================


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
