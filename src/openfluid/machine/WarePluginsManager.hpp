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
  \file WarePluginsManager.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __WAREPLUGINSMANAGER_HPP__
#define __WAREPLUGINSMANAGER_HPP__

#include <vector>
#include <map>
#include <string>

#include <QLibrary>

#include <openfluid/ware/PluggableWare.hpp>
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
class DLLEXPORT WarePluginsManager
{
  private:

    M* buildWareContainerWithSignatureOnly(const std::string& ID)
    {

      std::string PluginFilename = ID+getPluginFilenameSuffix()+openfluid::config::PLUGINS_EXT;
      std::string PluginFile = getPluginFullPath(PluginFilename);
      M* Plug = NULL;

      if (m_LoadedPlugins.find(PluginFilename) == m_LoadedPlugins.end())
      {
        m_LoadedPlugins[PluginFilename] = new QLibrary(QString::fromStdString(PluginFile));
      }

      QLibrary* PlugLib = m_LoadedPlugins[PluginFilename];


      // library loading
      if(PlugLib)
      {
        Plug = new M();
        Plug->Filename = PluginFile;

        GetWareABIVersionProc ABIVersionProc = (GetWareABIVersionProc)PlugLib->resolve(WAREABIVERSION_PROC_NAME);

        if (ABIVersionProc)
        {
          Plug->Verified = (openfluid::tools::CompareVersions(openfluid::config::FULL_VERSION,ABIVersionProc(),false) == 0);
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
              throw openfluid::base::FrameworkException("WarePluginsManager::buildWareContainerWithSignatureOnly","Signature from plugin file " + PluginFilename + " cannot be instanciated");

            Plug->Verified = (Plug->Signature->ID == ID);

            Plug->Body = 0;
          }
          else throw openfluid::base::FrameworkException("WarePluginsManager::buildWareContainerWithSignatureOnly","Format error in plugin file " + PluginFilename);
        }
        else throw openfluid::base::FrameworkException("WarePluginsManager::buildWareContainerWithSignatureOnly","Compatibility version mismatch for plugin file " + PluginFilename);
      }
      else throw openfluid::base::FrameworkException("WarePluginsManager::buildWareContainerWithSignatureOnly","Unable to load plugin from file " + PluginFilename);

      return Plug;
    }


    // =====================================================================
    // =====================================================================


    S* getWareSignature(const std::string& PluginFilename)
    {
      std::string PluginFile = getPluginFullPath(PluginFilename);
      S* Plug = NULL;

      if (m_LoadedPlugins.find(PluginFilename) == m_LoadedPlugins.end())
      {
        m_LoadedPlugins[PluginFilename] = new QLibrary(QString::fromStdString(PluginFile));
      }

      QLibrary* PlugLib = m_LoadedPlugins[PluginFilename];


      // library loading
      if(PlugLib)
      {
        Plug = new M();
        Plug->Filename = PluginFile;

        GetWareABIVersionProc ABIVersionProc = (GetWareABIVersionProc)PlugLib->resolve(WAREABIVERSION_PROC_NAME);

        if (ABIVersionProc)
        {
          Plug->Verified = (openfluid::tools::CompareVersions(openfluid::config::FULL_VERSION,ABIVersionProc(),false) == 0);
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
              throw openfluid::base::FrameworkException("WarePluginsManager::getWareSignature","Signature from plugin file " + PluginFilename + " cannot be instanciated");

            Plug->Verified = QString::fromStdString(PluginFilename).startsWith(QString::fromStdString(Plug->Signature->ID));
          }
          else throw openfluid::base::FrameworkException("WarePluginsManager::getWareSignature","Format error in plugin file " + PluginFilename);
        }
      }
      else throw openfluid::base::FrameworkException("WarePluginsManager::getWareSignature","Unable to find plugin file " + PluginFilename);

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

    virtual ~WarePluginsManager()
    {

    }


    // =====================================================================
    // =====================================================================


    virtual std::string getPluginFullPath(const std::string& Filename) = 0;


    // =====================================================================
    // =====================================================================


    virtual std::vector<std::string> getPluginsSearchPaths() = 0;


    // =====================================================================
    // =====================================================================


    virtual std::string getPluginFilenameSuffix() = 0;


    // =====================================================================
    // =====================================================================


    /**
      Lists available wares
    */
    std::vector<S*> getAvailableWaresSignatures(const std::string Pattern = "")
    {
      std::vector<S*> PluginsContainers;
      std::vector<std::string> PluginsPaths = getPluginsSearchPaths();
      std::vector<std::string> PluginFiles;
      std::vector<std::string> TmpFiles;
      unsigned int i,j;


      for (i=0;i<PluginsPaths.size();i++)
      {
        TmpFiles = openfluid::tools::GetFilesBySuffixAndExt(PluginsPaths[i],getPluginFilenameSuffix(),openfluid::config::PLUGINS_EXT,false,true);
        for (j=0;j<TmpFiles.size();j++) PluginFiles.push_back(TmpFiles[j]);
      }


      S* CurrentPlug;

      for (i=0;i<PluginFiles.size();i++)
      {
        CurrentPlug = getWareSignature(PluginFiles[i]);

        if (CurrentPlug != NULL && CurrentPlug->Verified)
        {
          if (Pattern != "")
          {
            if (openfluid::tools::WildcardMatching(Pattern,CurrentPlug->Signature->ID))
              PluginsContainers.push_back(CurrentPlug);
          }
          else PluginsContainers.push_back(CurrentPlug);
        }
      }

      return PluginsContainers;
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
      std::string PluginFilename = Item->Filename;

      if (m_LoadedPlugins.find(PluginFilename) == m_LoadedPlugins.end())
      {
        m_LoadedPlugins[PluginFilename] = new QLibrary(QString::fromStdString(PluginFilename));
      }

      QLibrary* PlugLib = m_LoadedPlugins[PluginFilename];


      // library loading
      if(PlugLib)
      {
        BP BodyProc = (BP)PlugLib->resolve(WAREBODY_PROC_NAME);

        // checks if the handle proc exists
        if(BodyProc)
        {
          Item->Body = BodyProc();

          if (Item->Body == NULL)
            throw openfluid::base::FrameworkException("WarePluginsManager::completeSignatureWithWareBody","Ware from plugin file " + PluginFilename + " cannot be instanciated");

        }
        else throw openfluid::base::FrameworkException("completeSignatureWithWareBody","Format error in plugin file " + PluginFilename);
      }
      else throw openfluid::base::FrameworkException("completeSignatureWithWareBody","Unable to find plugin file " + PluginFilename);
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




#endif /* __WAREPLUGINSMANAGER_HPP__ */
