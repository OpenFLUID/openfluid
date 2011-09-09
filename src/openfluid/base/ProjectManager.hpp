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
  \file ProjectManager.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __PROJECTMANAGER_HPP___
#define __PROJECTMANAGER_HPP___


#include <openfluid/dllexport.hpp>
#include <glibmm/ustring.h>

namespace openfluid { namespace base {


// =====================================================================
// =====================================================================

class DLLEXPORT ProjectManager
{
  private:

    static ProjectManager* mp_Singleton;

    Glib::ustring m_Path;

    Glib::ustring m_Name;
    Glib::ustring m_Description;
    Glib::ustring m_Authors;
    Glib::ustring m_CreationDate;
    Glib::ustring m_LastModDate;
    bool m_IsIncOutputDir;

    Glib::ustring m_OutputDir;
    Glib::ustring m_InputDir;

    bool m_IsOpened;

    Glib::ustring m_KeyFileGroupName;


    ProjectManager();

    Glib::ustring getNow();

    static std::string getFilePathFromProjectPath(Glib::ustring ProjectPath);

    static std::string getInputDirFromProjectPath(Glib::ustring ProjectPath);

    static std::string getOuputDirFromProjectPath(Glib::ustring ProjectPath);

  public:

    static ProjectManager* getInstance();

    ~ProjectManager();

    Glib::ustring getPath() const { return m_Path; };

    Glib::ustring getName() const { return m_Name; };

    void setName(const Glib::ustring& Name) { m_Name = Name; };

    Glib::ustring getDescription() const { return m_Description; };

    void setDescription(const Glib::ustring& Description) { m_Description = Description; };

    Glib::ustring getAuthors() const { return m_Authors; };

    void setAuthors(const Glib::ustring& Authors) { m_Authors = Authors; };

    Glib::ustring getCreationDate() const { return m_CreationDate; };

    void setCreationDate(const Glib::ustring& CreationDate) { m_CreationDate = CreationDate; };

    void setCreationDateAsNow() { m_CreationDate = getNow(); };

    Glib::ustring getLastModDate() const { return m_LastModDate; };

    void setLastModDate(const Glib::ustring& LastModDate) { m_LastModDate = LastModDate; };

    Glib::ustring getOutputDir() { return m_OutputDir; };

    Glib::ustring getInputDir() const { return m_InputDir; };

    bool isIncrementalOutputDir() const { return m_IsIncOutputDir; };

    void setIncrementalOutputDir(const bool Inc) { m_IsIncOutputDir = Inc; };

    bool open(const Glib::ustring& Path);

    bool create(const Glib::ustring& Path, const Glib::ustring& Name,
                const Glib::ustring& Description, const Glib::ustring& Authors,
                const bool Inc);

    bool isOpened() const { return m_IsOpened; };

    bool save();

    void close();

    static bool isProject(const Glib::ustring& Path);

    void updateOutputDir();

};


} } //namespaces


#endif /* __PROJECTMANAGER_HPP___ */
