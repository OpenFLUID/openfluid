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
  @file ProjectCheckInfos.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_PROJECTCHECKINFOS_HPP__
#define __OPENFLUID_BUILDERAPP_PROJECTCHECKINFOS_HPP__

#include <map>

#include <QStringList>


enum ProjectStatusLevel { PRJ_OK, PRJ_WARNING, PRJ_ERROR };


class ProjectPartCheckInfos
{
  private:

    ProjectStatusLevel m_Status;

    QStringList m_Messages;


  public:

    ProjectPartCheckInfos();

    ProjectStatusLevel getStatus() const;

    QStringList getMessages() const;

    void setStatus(ProjectStatusLevel Level);

    void addMessage(const QString& Msg);

    void clear();
};


// =====================================================================
// =====================================================================


class ProjectCheckInfos
{
  public:

    enum PartInfo { PART_MODELDEF, PART_MODELPARAMS,
                    PART_SPATIALSTRUCT, PART_SPATIALATTRS,
                    PART_DATASTORE,
                    PART_MONITORING,
                    PART_RUNCONFIG};


  private:

    std::map<PartInfo,ProjectPartCheckInfos> m_Infos;


  public:

    ProjectCheckInfos();

    bool isOKForSimulation() const;

    ProjectStatusLevel getOverallStatus() const;

    inline ProjectPartCheckInfos& part(PartInfo Part) { return m_Infos[Part]; };

    inline const ProjectPartCheckInfos& part(PartInfo Part) const { return m_Infos.at(Part); };

    void clear();
};


#endif /* __OPENFLUID_BUILDERAPP_PROJECTCHECKINFOS_HPP__ */
