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
  @file ProjectCheckInfos.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include "ProjectCheckInfos.hpp"


ProjectPartCheckInfos::ProjectPartCheckInfos()
{
  m_Status = ProjectStatusLevel::PRJ_OK;
}


// =====================================================================
// =====================================================================


ProjectStatusLevel ProjectPartCheckInfos::getStatus() const
{
  return m_Status;
}


// =====================================================================
// =====================================================================


QStringList ProjectPartCheckInfos::getMessages() const
{
  return m_Messages;
}


// =====================================================================
// =====================================================================


void ProjectPartCheckInfos::updateStatus(ProjectStatusLevel Level)
{
  if (Level > m_Status)
  {
    m_Status = Level;
  }
}


// =====================================================================
// =====================================================================


void ProjectPartCheckInfos::addMessage(const QString& Msg)
{
  m_Messages.append(Msg);
}


// =====================================================================
// =====================================================================


void ProjectPartCheckInfos::clear()
{
  m_Messages.clear();
  m_Status = ProjectStatusLevel::PRJ_OK;
}


// =====================================================================
// =====================================================================


ProjectCheckInfos::ProjectCheckInfos()
{
  m_Infos[PartInfo::PART_MODELDEF] = ProjectPartCheckInfos();
  m_Infos[PartInfo::PART_MODELPARAMS] = ProjectPartCheckInfos();
  m_Infos[PartInfo::PART_SPATIALSTRUCT] = ProjectPartCheckInfos();
  m_Infos[PartInfo::PART_SPATIALATTRS] = ProjectPartCheckInfos();
  m_Infos[PartInfo::PART_DATASTORE] = ProjectPartCheckInfos();
  m_Infos[PartInfo::PART_MONITORING] = ProjectPartCheckInfos();
  m_Infos[PartInfo::PART_RUNCONFIG] = ProjectPartCheckInfos();
}


// =====================================================================
// =====================================================================


bool ProjectCheckInfos::isOKForSimulation() const
{
  return (m_Infos.at(PartInfo::PART_MODELDEF).getStatus() < ProjectStatusLevel::PRJ_DISABLED &&
          m_Infos.at(PartInfo::PART_MODELPARAMS).getStatus() < ProjectStatusLevel::PRJ_DISABLED &&
          m_Infos.at(PartInfo::PART_SPATIALSTRUCT).getStatus() < ProjectStatusLevel::PRJ_DISABLED &&
          m_Infos.at(PartInfo::PART_SPATIALATTRS).getStatus() < ProjectStatusLevel::PRJ_DISABLED &&
          m_Infos.at(PartInfo::PART_DATASTORE).getStatus() < ProjectStatusLevel::PRJ_DISABLED &&
          m_Infos.at(PartInfo::PART_MONITORING).getStatus() < ProjectStatusLevel::PRJ_DISABLED &&
          m_Infos.at(PartInfo::PART_RUNCONFIG).getStatus() < ProjectStatusLevel::PRJ_DISABLED);
}


// =====================================================================
// =====================================================================


ProjectStatusLevel ProjectCheckInfos::getOverallStatus() const
{
  return (qMax(m_Infos.at(PartInfo::PART_MODELDEF).getStatus(),
               qMax(m_Infos.at(PartInfo::PART_MODELPARAMS).getStatus(),
                    qMax(m_Infos.at(PartInfo::PART_SPATIALSTRUCT).getStatus(),
                         qMax(m_Infos.at(PartInfo::PART_SPATIALATTRS).getStatus(),
                              qMax(m_Infos.at(PartInfo::PART_DATASTORE).getStatus(),
                                   qMax(m_Infos.at(PartInfo::PART_MONITORING).getStatus(),
                                        m_Infos.at(PartInfo::PART_RUNCONFIG).getStatus())))))));
}


// =====================================================================
// =====================================================================


void ProjectCheckInfos::clear()
{
  m_Infos[PartInfo::PART_MODELDEF].clear();
  m_Infos[PartInfo::PART_MODELPARAMS].clear();
  m_Infos[PartInfo::PART_SPATIALSTRUCT].clear();
  m_Infos[PartInfo::PART_SPATIALATTRS].clear();
  m_Infos[PartInfo::PART_DATASTORE].clear();
  m_Infos[PartInfo::PART_MONITORING].clear();
  m_Infos[PartInfo::PART_RUNCONFIG].clear();
}
