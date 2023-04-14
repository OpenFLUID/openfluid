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
  @file WareMigrationListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WAREMIGRATIONLISTENER_HPP__
#define __OPENFLUID_UIWARESDEV_WAREMIGRATIONLISTENER_HPP__


#include <iostream>
#include <iomanip>

#include <QObject>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/waresdev/WareSrcMigratorListener.hpp>
#include <openfluid/tools/Console.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class WareMigrationListener : public QObject, public openfluid::waresdev::WareSrcMigratorListener
{
  Q_OBJECT;

  private:

    bool m_Verbose = false;

    bool m_IsFirstStageMsg = true;
    QString m_CurrentMsg = "";

    int m_Progress = 0;
    double m_ProgressRatio = 100/8; //Compute percent since there are 8 steps

    void flushMsg(const std::string& Channel="info");

    void incrementProgress();

    void displayStatus(const openfluid::base::Listener::Status& Status);

    void displayStartMsg(const std::string& Msg);


  signals:

    void info(QString Message);

    void warning(QString Message);

    void error(QString Message);

    void progressed(int Value);

    void finished();


  public:

    CPPCLASS_DEFAULT_FIVE(WareMigrationListener)

    void setVerbose(bool Enabled);

    void onPrecheckStart();

    void onPrecheckEnd(openfluid::base::Listener::Status Status);

    void onPrepareStart();

    void onPrepareEnd(openfluid::base::Listener::Status Status);

    void onProcessSignatureStart();

    void onProcessSignatureEnd(openfluid::base::Listener::Status Status);

    void onProcessCMakeConfigStart();

    void onProcessCMakeConfigEnd(openfluid::base::Listener::Status Status);

    void onCreateStructureStart();

    void onCreateStructureEnd(openfluid::base::Listener::Status Status);

    void onDispatchFilesStart();

    void onDispatchFilesEnd(openfluid::base::Listener::Status Status);

    void onCleanSrcFilesStart();

    void onCleanSrcFilesEnd(openfluid::base::Listener::Status Status);

    void onDocStart();

    void onDocEnd(openfluid::base::Listener::Status Status);

    void stageMessage(const std::string& Msg);
};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WAREMIGRATIONLISTENER_HPP__ */
