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
  @file ProjectManager.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BASE_PROJECTMANAGER_HPP__
#define __OPENFLUID_BASE_PROJECTMANAGER_HPP__


#include <openfluid/dllexport.hpp>
#include <string>
#include <QString>
#include <QVariant>
#include <QSettings>

namespace openfluid { namespace base {


// =====================================================================
// =====================================================================

class OPENFLUID_API ProjectManager
{
  private:

    static ProjectManager* mp_Singleton;

    QSettings* mp_PrjFile;

    std::string m_Path;

    std::string m_Name;
    std::string m_Description;
    std::string m_Authors;
    std::string m_CreationDate;
    std::string m_LastModDate;
    bool m_IsIncOutputDir;

    std::string m_OutputDir;
    std::string m_InputDir;

    bool m_IsOpened;

    static QString m_GroupName;


    ProjectManager();

    static std::string getNow();

    static std::string getFilePathFromProjectPath(std::string ProjectPath);

    static std::string getInputDirFromProjectPath(std::string ProjectPath);

    static std::string getOuputDirFromProjectPath(std::string ProjectPath);

    static bool checkProject(const std::string& ProjectPath);


  public:

    static ProjectManager* instance();

    ~ProjectManager();

    std::string getPath() const
    { return m_Path; };

    std::string getName() const
    { return m_Name; };

    void setName(const std::string& Name)
    { m_Name = Name; };

    std::string getDescription() const
    { return m_Description; };

    void setDescription(const std::string& Description)
    { m_Description = Description; };

    std::string getAuthors() const
    { return m_Authors; };

    void setAuthors(const std::string& Authors)
    { m_Authors = Authors; };

    std::string getCreationDate() const
    { return m_CreationDate; };

    void setCreationDate(const std::string& CreationDate)
    { m_CreationDate = CreationDate; };

    void setCreationDateAsNow()
    { m_CreationDate = getNow(); };

    std::string getLastModDate() const
    { return m_LastModDate; };

    void setLastModDate(const std::string& LastModDate)
    { m_LastModDate = LastModDate; };

    std::string getOutputDir() const
    { return m_OutputDir; };

    std::string getInputDir() const
    { return m_InputDir; };

    bool isIncrementalOutputDir() const
    { return m_IsIncOutputDir; };

    void setIncrementalOutputDir(const bool Inc)
    { m_IsIncOutputDir = Inc; };

    bool open(const std::string& Path);

    bool create(const std::string& Path, const std::string& Name,
                const std::string& Description, const std::string& Authors,
                const bool Inc);

    bool isOpened() const
    { return m_IsOpened; };

    bool save();

    void close();

    static bool isProject(const std::string& Path);

    static bool getProjectInfos(const std::string& Path,
                                std::string& Name, std::string& Description, std::string& Authors,
                                std::string& CreationDate, std::string& LastModDate);

    void updateOutputDir();

    QVariant getConfigValue(const QString& Group, const QString& Key) const;

    void setConfigValue(const QString& Group, const QString& Key, const QVariant& Value);

};


} } //namespaces


#endif /* __OPENFLUID_BASE_PROJECTMANAGER_HPP__ */
