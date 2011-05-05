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
 \file ResViewerModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ResViewerModel.hpp"

#include <boost/foreach.hpp>

// =====================================================================
// =====================================================================


void ResViewerModelImpl::extractVariablesNames()
{
  m_VarNames.clear();

  if (mp_SetDesc->isAllScalars())
    m_VarNames = mp_Unit->getScalarVariables()->getVariablesNames();
  else
    m_VarNames = mp_SetDesc->getScalars();

  std::vector<std::string> VectorVars;
  if (mp_SetDesc->isAllVectors())

    VectorVars = mp_Unit->getVectorVariables()->getVariablesNames();
  else
    VectorVars = mp_SetDesc->getVectors();

  m_VarNames.insert(m_VarNames.end(), VectorVars.begin(), VectorVars.end());
}

// =====================================================================
// =====================================================================


ResViewerModelImpl::ResViewerModelImpl() :
  mp_RunDesc(0), mp_SetDesc(0), mp_OutDesc(0), mp_Unit(0), mp_SimStatus(0),
      m_Precision(0), m_ShowFiles(true), m_ColSep(""), m_CommentChar(""),
      m_DateFormat("")
{
}

// =====================================================================
// =====================================================================


sigc::signal<void> ResViewerModelImpl::signal_FromAppInit()
{
  return m_signal_FromAppInit;
}

// =====================================================================
// =====================================================================


sigc::signal<void> ResViewerModelImpl::signal_FromAppClear()
{
  return m_signal_FromAppClear;
}

// =====================================================================
// =====================================================================


void ResViewerModelImpl::setEngineRequirements(
    openfluid::base::RunDescriptor& RunDesc,
    openfluid::base::OutputDescriptor& OutDesc)
{
  mp_RunDesc = &RunDesc;
  mp_OutDesc = &OutDesc;
}

// =====================================================================
// =====================================================================


void ResViewerModelImpl::initialize(
    openfluid::base::OutputSetDescriptor* SetDesc, openfluid::core::Unit* Unit)
{
  if (SetDesc && Unit)
  {
    mp_SetDesc = SetDesc;
    mp_Unit = Unit;

    mp_SimStatus = new openfluid::base::SimulationStatus(
        mp_RunDesc->getBeginDate(), mp_RunDesc->getEndDate(),
        mp_RunDesc->getDeltaT());

    m_Precision = mp_SetDesc->getPrecision();

    extractVariablesNames();

    m_signal_FromAppInit.emit();
  }
}

// =====================================================================
// =====================================================================


void ResViewerModelImpl::initialize(
    openfluid::base::OutputSetDescriptor* SetDesc, openfluid::core::Unit* Unit,
    std::vector<std::string> Vars, bool ShowFiles)
{
  if (SetDesc && Unit)
  {
    mp_SetDesc = SetDesc;
    mp_Unit = Unit;

    mp_SimStatus = new openfluid::base::SimulationStatus(
        mp_RunDesc->getBeginDate(), mp_RunDesc->getEndDate(),
        mp_RunDesc->getDeltaT());

    m_Precision = mp_SetDesc->getPrecision();

    m_VarNames = Vars;

    m_ShowFiles = ShowFiles;

    if (mp_SetDesc && mp_OutDesc)
    {
      for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
      {
        for (unsigned int j = 0; j
            < mp_OutDesc->getFileSets()[i].getSets().size(); j++)
        {
          if (mp_OutDesc->getFileSets()[i].getSets()[j].getName()
              == mp_SetDesc->getName())
          {
            m_ColSep = mp_OutDesc->getFileSets()[i].getColSeparator();
            m_CommentChar = mp_OutDesc->getFileSets()[i].getCommentChar();
            m_DateFormat = mp_OutDesc->getFileSets()[i].getDateFormat();
            break;
          }
        }
      }
    }

    m_signal_FromAppInit.emit();
  }
}

// =====================================================================
// =====================================================================


void ResViewerModelImpl::clear()
{
  mp_SetDesc = 0;
  mp_OutDesc = 0;
  mp_Unit = 0;
  mp_SimStatus = 0;
  m_Precision = 0;
  m_ColSep = "";
  m_CommentChar = "";
  m_DateFormat = "";
  m_VarNames.clear();
  m_signal_FromAppClear.emit();
}

// =====================================================================
// =====================================================================


openfluid::core::Unit* ResViewerModelImpl::getUnit()
{
  return mp_Unit;
}

// =====================================================================
// =====================================================================


std::vector<std::string> ResViewerModelImpl::getVarNames()
{
  return m_VarNames;
}

// =====================================================================
// =====================================================================


openfluid::base::SimulationStatus* ResViewerModelImpl::getSimStatus()
{
  return mp_SimStatus;
}

// =====================================================================
// =====================================================================


unsigned int ResViewerModelImpl::getPrecision()
{
  return m_Precision;
}

// =====================================================================
// =====================================================================


std::string ResViewerModelImpl::getSetName()
{
  return mp_SetDesc->getName();
}

// =====================================================================
// =====================================================================

std::string ResViewerModelImpl::getColSep()
{
  return m_ColSep;
}

// =====================================================================
// =====================================================================


std::string ResViewerModelImpl::getCommentChar()
{
  return m_CommentChar;
}

// =====================================================================
// =====================================================================


std::string ResViewerModelImpl::getDateFormat()
{
  return m_DateFormat;
}

// =====================================================================
// =====================================================================


bool ResViewerModelImpl::getShowFiles()
{
  return m_ShowFiles;
}

// =====================================================================
// =====================================================================


void ResViewerModelSub::setSetDescriptor(
    openfluid::base::OutputSetDescriptor* SetDesc)
{
  mp_SetDesc = SetDesc;
}

// =====================================================================
// =====================================================================


void ResViewerModelSub::setUnit(openfluid::core::Unit* Unit)
{
  mp_Unit = Unit;
}

// =====================================================================
// =====================================================================


void ResViewerModelSub::extractVariablesNames()
{
  ResViewerModelImpl::extractVariablesNames();
}
