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
  @file ExceptionsObs.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableObserver.hpp>


// =====================================================================
// =====================================================================


DECLARE_OBSERVER_PLUGIN


// =====================================================================
// =====================================================================


BEGIN_OBSERVER_SIGNATURE("tests.exceptions")

  DECLARE_NAME("test observer for exceptions");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

END_OBSERVER_SIGNATURE


// =====================================================================
// =====================================================================


class ExceptionsObserver : public openfluid::ware::PluggableObserver
{
  private:

    std::string m_PartStr;


  public:

    ExceptionsObserver() : PluggableObserver()
    {

    }

    // =====================================================================
    // =====================================================================


    ~ExceptionsObserver()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      if (Params.find("part") != Params.end())
        m_PartStr = Params.at("part").toString();

      if (m_PartStr == "initparams")
        OPENFLUID_RaiseError(m_PartStr);
    }


    // =====================================================================
    // =====================================================================


    void onPrepared()
    {
      if (m_PartStr == "onprepared")
        OPENFLUID_RaiseError(m_PartStr);
    }


    // =====================================================================
    // =====================================================================


    void onInitializedRun()
    {
      if (m_PartStr == "oninitializedrun")
        OPENFLUID_RaiseError(m_PartStr);
    }


    // =====================================================================
    // =====================================================================


    void onStepCompleted()
    {
      if (m_PartStr == "onstepcompleted")
        OPENFLUID_RaiseError(m_PartStr);
    }


    // =====================================================================
    // =====================================================================


    void onFinalizedRun()
    {
      if (m_PartStr == "onfinalizedrun")
        OPENFLUID_RaiseError(m_PartStr);
    }


};


// =====================================================================
// =====================================================================


DEFINE_OBSERVER_CLASS(ExceptionsObserver)
