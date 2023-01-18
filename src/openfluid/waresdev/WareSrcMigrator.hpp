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
 @file WareSrcMigrator.hpp
 
 @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_WARESDEV_WARESRCMIGRATOR_HPP__
#define __OPENFLUID_WARESDEV_WARESRCMIGRATOR_HPP__


#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/TemplateProcessor.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/waresdev/WareSrcMigratorListener.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace waresdev {


/**
  Class to manage migration of wares source code
*/
class OPENFLUID_API WareSrcMigrator
{
  private:

    struct WareMigrationInfo
    {
      openfluid::ware::WareType WareType = openfluid::ware::WareType::UNDEFINED;

      openfluid::tools::Path SignatureFile;
      std::string SignatureContent;
      openfluid::tools::TemplateProcessor::Data TemplateData;

      openfluid::tools::Path UISignatureFile;
      std::string UISignatureContent;

      openfluid::tools::Path DocFile;
      std::string DocContent;

      std::string CMakeDefaultWareVar;
      std::string CMakeWareSuffix;
    };


    openfluid::tools::Path m_SrcPathObj;

    openfluid::tools::Path m_DestPathObj;

    openfluid::tools::Path m_WorkPathObj;

    const bool m_InPlace;

    WareSrcMigratorListener* mp_Listener;

    
    void processSignature(const WareSrcMigrator::WareMigrationInfo& Info,
                          const openfluid::tools::Path& WorkPathObj) const;

    void dispatchExistingFiles(const WareSrcMigrator::WareMigrationInfo& Info) const;

    std::pair<openfluid::tools::TemplateProcessor::Data,std::string> 
    processCMakeFiles(const WareSrcMigrator::WareMigrationInfo& Info) const;

    WareMigrationInfo prepareMigration();

    void processSources(const WareMigrationInfo& Info) const;

    void processDoc(const WareMigrationInfo& Info) const;


  public:

    /**
      Constructs a migrator
      @param[in] SrcPath The path to the sources to migrate
      @param[in] Listener The Listener for the migration process (must not be nullptr)
      @param[in] DestPath The path to the migrated sources (optional). Il not given or empty, 
                          the sources are migrated in the original sources path
    */
    WareSrcMigrator(const std::string& SrcPath, WareSrcMigratorListener* Listener, 
                    const std::string& DestPath = "");
    
    /**
      Performs the migration process
    */
    void performMigration();
};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCMIGRATOR_HPP__ */
