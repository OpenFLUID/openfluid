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
  @file PreferencesManager.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
  @author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __OPENFLUID_BASE_PREFERENCESMANAGER_HPP__
#define __OPENFLUID_BASE_PREFERENCESMANAGER_HPP__


#include <map>
#include <vector>
#include <string>

#include <openfluid/core/DateTime.hpp>
#include <openfluid/utils/SingletonMacros.hpp>
#include <openfluid/tools/SettingsBackend.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace base {


// =====================================================================
// =====================================================================


class OPENFLUID_API PreferencesManager
{

  OPENFLUID_SINGLETON_DEFINITION(PreferencesManager)

  private:

    static std::string m_SettingsFile;

    std::unique_ptr<openfluid::tools::SettingsBackend> m_Settings;

    static const std::string m_SettingsRole;

    PreferencesManager();

    ~PreferencesManager()
    { }

    void updateSettingsFile(const std::string& FilePath) const;

    void loadSettings();

    void setDefaultValues();

    void setBuilderExtraPaths(const std::string& Key, const std::vector<std::string>& Paths);

    void addBuilderExtraPath(const std::string& Key, const std::string& Path);

    void removeBuilderExtraPath(const std::string& Key, const std::string& Path);

    std::vector<std::string> getBuilderExtraPaths(const std::string& Key) const;

    static std::string guessLanguage();


  public:

    struct SyntaxHighlightingRule_t
    {
      std::string Color;
      std::vector<std::string> Decoration;
    };

    enum class ExternalToolContext { WORKSPACE, WARE, FILE };

    struct ExternalTool_t
    {
      std::string Name;

      std::map<ExternalToolContext,std::string> Commands;

      std::string getCommand(ExternalToolContext Ctxt) const
      {
        auto it = Commands.find(Ctxt);
        if (it != Commands.end())
        {
          return (*it).second;
         } 
         return "";
      }
    };

    typedef std::map<std::string, SyntaxHighlightingRule_t> SyntaxHighlightingRules_t;

    /** 
       Used only if another file is used as the settings file
       instead of the default one (e.g. for testing). To be set before the first call of instance().
    */
    static void setSettingsFile(const std::string& FilePath);

    std::string getSettingsFile() const;


    void setUILanguage(const std::string& Lang); 

    std::string getUILanguage() const; 

    static std::vector<std::string> getAvailableUILanguages();

    static bool isUILanguageAvailable(const std::string& Lang);


    void setWorkspacesPaths(const std::vector<std::string>& Paths);

    std::vector<std::string> getWorkspacesPaths() const;

    std::string getCurrentWorkspacePath() const;

     
    void setBuilderExtraSimulatorsPaths(const std::vector<std::string>& Paths);

    void addBuilderExtraSimulatorsPath(const std::string& Path);

    void removeBuilderExtraSimulatorsPath(const std::string& Path);

    std::vector<std::string> getBuilderExtraSimulatorsPaths() const;


    void setBuilderExtraExtensionsPaths(const std::vector<std::string>& Paths);

    void addBuilderExtraExtensionsPath(const std::string& Path);

    void removeBuilderExtraExtensionsPath(const std::string& Path);

    std::vector<std::string> getBuilderExtraExtensionsPaths() const;


    void setBuilderExtraObserversPaths(const std::vector<std::string>& Paths);

    void addBuilderExtraObserversPath(const std::string& Path);

    void removeBuilderExtraObserversPath(const std::string& Path);

    std::vector<std::string> getBuilderExtraObserversPaths() const;


    void setBuilderDeltaT(openfluid::core::Duration_t DeltaT);

    openfluid::core::Duration_t getBuilderDeltaT() const;


    void setBuilderBeginDate(const std::string& Begin);

    std::string getBuilderBeginDate() const;

    void setBuilderEndDate(const std::string& End);

    std::string getBuilderEndDate() const;


    int getBuilderDockArea() const;

    void setBuilderDockArea(int Area);

    int getBuilderToolBarArea() const;

    void setBuilderToolBarArea(int Area);


    bool isBuilderWareRemovalConfirm() const;

    void setBuilderWareRemovalConfirm(bool Confirm);

    bool isBuilderParamRemovalConfirm() const;

    void setBuilderParamRemovalConfirm(bool Confirm);

    bool isBuilderWaresWatchersActive() const;

    void setBuilderWaresWatchersActive(bool Active);

    bool isBuilderAutomaticSaveBeforeRun() const;

    void setBuilderAutomaticSaveBeforeRun(bool AutoSave);

    bool isBuilderSpatialUnitsRemovalConfirm() const;

    void setBuilderSpatialUnitsRemovalConfirm(bool Confirm);

    bool isBuilderSpatialConnsRemovalConfirm() const;

    void setBuilderSpatialConnsRemovalConfirm(bool Confirm);

    bool isBuilderSpatialAttrsRemovalConfirm() const;

    void setBuilderSpatialAttrsRemovalConfirm(bool Confirm);


    bool isWaresdevAutomaticSaveBeforeBuild() const;

    void setWaresdevAutomaticSaveBeforeBuild(bool AutoSave);

    std::list<ExternalTool_t> getWaresdevExternalTools() const;

    std::list<ExternalTool_t> getWaresdevExternalToolsInContext(const ExternalToolContext Context) const;

    void setWaresdevExternalTools(const std::list<ExternalTool_t>& Tools);

    void setWaresdevTextEditorDefaults(bool ForceReset);

    bool isWaresdevSyntaxHighlightingEnabled() const;

    void setWaresdevSyntaxHighlightingEnabled(bool Enabled);

    SyntaxHighlightingRules_t getWaresdevSyntaxHighlightingRules() const;

    void setWaresdevSyntaxHighlightingRules(const SyntaxHighlightingRules_t& Rules);

    bool isWaresdevCurrentlineHighlightingEnabled() const;

    void setWaresdevCurrentlineHighlightingEnabled(bool Enabled);

    std::string getWaresdevCurrentlineColor() const;

    void setWaresdevCurrentlineColor(const std::string& Color);

    std::string getWaresdevFontName() const;

    void setWaresdevFontName(const std::string& FontName);

    bool isWaresdevLineWrappingEnabled() const;

    void setWaresdevLineWrappingEnabled(bool Enabled);

    int getWaresdevIndentSpaceNb() const;

    bool isWaresdevSpaceTabDisplayEnabled() const;

    void setWaresdevSpaceTabDisplayEnabled(bool Enabled);

    bool isWaresdevCarriageReturnDisplayEnabled() const;

    void setWaresdevCarriageReturnDisplayEnabled(bool Enabled);

    void setWaresdevIndentSpaceNb(int SpaceNumber);

    std::string getWaresdevConfigureEnv(const std::string& Name) const;

    void setWaresdevConfigureEnv(const std::string& Name, const std::string& Value);

    std::string getWaresdevConfigureOptions() const;

    void setWaresdevConfigureOptions(const std::string& Options);

    std::string getWaresdevConfigureGenerator() const;

    void setWaresdevConfigureGenerator(const std::string& Generator);

    std::string getWaresdevBuildEnv(const std::string& Name) const;

    void setWaresdevBuildEnv(const std::string& Name, const std::string& Value);

    bool isWaresdevShowCommandEnv(const std::string& Name) const;

    void setWaresdevShowCommandEnv(const std::string& Name, bool Enabled);

    bool isWaresdevGitSslNoVerify() const;

    void setWaresdevGitSslNoVerify(bool NoVerify);

    std::string getWaresdevImportHubUrl() const;

    void setWaresdevImportHubUrl(const std::string& Url);

    std::string getWaresdevImportHubUsername() const;

    void setWaresdevImportHubUsername(const std::string& Username);
};


} } //namespaces


#endif /* __OPENFLUID_BASE_PREFERENCESMANAGER_HPP__ */
