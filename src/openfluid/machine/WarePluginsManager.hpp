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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_WAREPLUGINSMANAGER_HPP__
#define __OPENFLUID_MACHINE_WAREPLUGINSMANAGER_HPP__

#include <vector>
#include <map>
#include <string>

#include <QLibrary>
#include <QFileInfo>

#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/tools/FileHelpers.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/config.hpp>


namespace openfluid { namespace machine {


typedef std::string (*GetWareABIVersionProc)();


// =====================================================================
// =====================================================================


/**
  Management class for pluggable ware
  @tparam S class defining the container for ware signature only
  @tparam M class defining the container for ware signature and body
  @tparam SP procedure definition for instantiation of the signature
  @tparam BP procedure definition for instantiation of the body
*/
template<class S, class M, typename SP, typename BP>
class OPENFLUID_API WarePluginsManager
{
  private:

    QLibrary* loadWare(const std::string& FullFilePath)
    {
      std::string PluginFileName = QFileInfo(QString::fromStdString(FullFilePath)).fileName().toStdString();

      if (m_LoadedPlugins.find(PluginFileName) == m_LoadedPlugins.end())
      {
        m_LoadedPlugins[PluginFileName] = new QLibrary(QString::fromStdString(FullFilePath));
      }

      return m_LoadedPlugins[PluginFileName];
    }


    // =====================================================================
    // =====================================================================


    M* buildWareContainerWithSignatureOnly(const std::string& ID)
    {

      std::string PluginFilename = ID+getPluginFilenameSuffix()+openfluid::config::PLUGINS_EXT;
      std::string PluginFullPath = getPluginFullPath(PluginFilename);
      M* Plug = NULL;

      QLibrary* PlugLib = loadWare(PluginFullPath);

      // library loading
      if (PlugLib && PlugLib->load())
      {
        Plug = new M();
        Plug->FileFullPath = PluginFullPath;

        GetWareABIVersionProc ABIVersionProc = (GetWareABIVersionProc)PlugLib->resolve(WAREABIVERSION_PROC_NAME);

        if (ABIVersionProc)
        {
          Plug->Verified =
              (openfluid::tools::compareVersions(openfluid::config::FULL_VERSION,ABIVersionProc(),false) == 0);
        }
        else Plug->Verified = false;


        if (Plug->Verified)
        {
          BP BodyProc = (BP)PlugLib->resolve(WAREBODY_PROC_NAME);
          SP SignatureProc = (SP)PlugLib->resolve(WARESIGNATURE_PROC_NAME);

          // checks if the handle procs exist
          if (SignatureProc && BodyProc)
          {
            Plug->Signature = SignatureProc();

            if (Plug->Signature == NULL)
              throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                        "Signature from plugin file " + PluginFilename +
                                                        " cannot be instanciated");

            Plug->Verified = (Plug->Signature->ID == ID);

            Plug->Body = 0;

            Plug->WithParametersWidget = PlugLib->resolve(WAREPARAMSWIDGET_PROC_NAME);
          }
          else
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Format error in plugin file " + PluginFilename);
        }
        else
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "Compatibility version mismatch for plugin file " + PluginFilename);
      }
      else
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                  "Unable to load plugin from file " + PluginFilename);

      return Plug;
    }


    // =====================================================================
    // =====================================================================


    S* getWareSignature(const std::string& PluginFilename)
    {
      std::string PluginFullPath = getPluginFullPath(PluginFilename);
      S* Plug = NULL;

      openfluid::base::ExceptionContext ECtxt =
          openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION)
          .addInfos({{"pluginfullpath",PluginFullPath},{"pluginfilename",PluginFilename}});

      // library loading
      QLibrary* PlugLib = loadWare(PluginFullPath);


      if (PlugLib)
      {
        if (PlugLib->load())
        {
          Plug = new M();
          Plug->FileFullPath = PluginFullPath;

          GetWareABIVersionProc ABIVersionProc = (GetWareABIVersionProc)PlugLib->resolve(WAREABIVERSION_PROC_NAME);

          if (ABIVersionProc)
          {
            Plug->Verified =
                (openfluid::tools::compareVersions(openfluid::config::FULL_VERSION,ABIVersionProc(),false) == 0);
          }
          else Plug->Verified = false;


          if (Plug->Verified)
          {
            BP BodyProc = (BP)PlugLib->resolve(WAREBODY_PROC_NAME);
            SP SignatureProc = (SP)PlugLib->resolve(WARESIGNATURE_PROC_NAME);

            // checks if the handle procs exist
            if (SignatureProc && BodyProc)
            {
              try
              {
                Plug->Signature = SignatureProc();
              }
              catch (openfluid::base::FrameworkException& E)
              {
                throw openfluid::base::FrameworkException(ECtxt,E.getMessage());
              }

              if (Plug->Signature == NULL)
                throw openfluid::base::FrameworkException(ECtxt,"Signature cannot be instanciated from plugin file");

              Plug->Verified =
                  QString::fromStdString(PluginFilename).startsWith(QString::fromStdString(Plug->Signature->ID));

              Plug->WithParametersWidget = PlugLib->resolve(WAREPARAMSWIDGET_PROC_NAME);
            }
            else
              throw openfluid::base::FrameworkException(ECtxt,"Format error in plugin file");
          }
        }
        else
        {
          throw openfluid::base::FrameworkException(ECtxt,PlugLib->errorString().toStdString());
        }
      }
      else
        throw openfluid::base::FrameworkException(ECtxt,"Unable to find plugin file");

      return Plug;
    }


    // =====================================================================
    // =====================================================================


  protected:

    std::map<std::string,QLibrary*> m_LoadedPlugins;


    WarePluginsManager()
    {

    }


    // =====================================================================
    // =====================================================================


  public:

    class PluginsSearchResults
    {
      public:
        std::vector<S*> AvailablePlugins;

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


      S* CurrentPlug = NULL;

      for (i=0;i<PluginFiles.size();i++)
      {
        try
        {
          CurrentPlug = getWareSignature(PluginFiles[i]);
        }
        catch (openfluid::base::FrameworkException& E)
        {
          SearchResults.ErroredFiles[E.getContext().at("pluginfullpath")] = E.getMessage();
        }

        if (CurrentPlug != NULL && CurrentPlug->Verified)
        {
          if (Pattern != "")
          {
            if (openfluid::tools::matchWithWildcard(Pattern,CurrentPlug->Signature->ID))
              SearchResults.AvailablePlugins.push_back(CurrentPlug);
          }
          else SearchResults.AvailablePlugins.push_back(CurrentPlug);
        }
      }

      return SearchResults;
    }


    // =====================================================================
    // =====================================================================


    M* loadWareSignatureOnly(const std::string& ID)
    {
      M* Plug = buildWareContainerWithSignatureOnly(ID);

      if (Plug != NULL && Plug->Verified) return Plug;

      return NULL;
    }


    // =====================================================================
    // =====================================================================


    void completeSignatureWithWareBody(M* Item)
    {
      std::string PluginFullPath = Item->FileFullPath;

      QLibrary* PlugLib = loadWare(PluginFullPath);

      // library loading
      if (PlugLib && PlugLib->load())
      {
        BP BodyProc = (BP)PlugLib->resolve(WAREBODY_PROC_NAME);

        // checks if the handle proc exists
        if(BodyProc)
        {
          Item->Body = BodyProc();

          if (Item->Body == NULL)
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Ware from plugin file " + PluginFullPath +
                                                      " cannot be instanciated");

        }
        else throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                       "Format error in plugin file " + PluginFullPath);
      }
      else throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                     "Unable to find plugin file " + PluginFullPath);
    }


    // =====================================================================
    // =====================================================================


    void* getParameterizationWidget(const S* Item)
    {
      std::string PluginFullPath = Item->FileFullPath;

      QLibrary* PlugLib = loadWare(PluginFullPath);

      // library loading
      if (PlugLib && PlugLib->load())
      {
        typedef void* (*GetParametersWidgetProc)();
        GetParametersWidgetProc ParamsWidgetProc =
            (GetParametersWidgetProc)PlugLib->resolve(WAREPARAMSWIDGET_PROC_NAME);

        // checks if the handle proc exists
        if(ParamsWidgetProc)
        {
          return ParamsWidgetProc();
        }
        else
          return NULL;
      }
      else
        return NULL;
    }



    // =====================================================================
    // =====================================================================


    void unloadAllWares()
    {
      std::map<std::string,QLibrary*>::iterator it;

      for (it=m_LoadedPlugins.begin();it != m_LoadedPlugins.end(); ++it)
      {
        it->second->unload();
        delete (it->second);
      }

      m_LoadedPlugins.clear();
    }

};



} } //namespaces




#endif /* __OPENFLUID_MACHINE_WAREPLUGINSMANAGER_HPP__ */
