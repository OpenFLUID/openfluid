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
  @file WareSrcUIContainer.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WARESRCUICONTAINER_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCUICONTAINER_HPP__


#include <QObject>
#include <QProcess>

#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/waresdev/WareSrcContainerOps.hpp>
#include <openfluid/ui/waresdev/WareSrcMsgParser.hpp>
#include <openfluid/ui/waresdev/WareSrcMsgStream.hpp>
#include <openfluid/tools/Timer.hpp>
#include <openfluid/utilsq/CMakeProxy.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class WareSrcProcess : public QProcess
{

  public:

    enum class Type { NONE, CONFIGURE, BUILD};

    void setType(Type PT)
    { 
      m_Type = PT; 
    }

    Type getType() const
    { 
      return m_Type; 
    }


  private:

    Type m_Type = Type::NONE;

};


// =====================================================================
// =====================================================================


class OPENFLUID_API WareSrcUIContainer: public QObject, 
                                        public openfluid::waresdev::WareSrcContainer, 
                                        public openfluid::waresdev::WareSrcContainerOps
{
  Q_OBJECT


  private slots:

    void processStandardOutput();

    void processErrorOutput();

    void processFinishedOutput(int ExitCode);


  private:

    WareSrcProcess* mp_Process;

    WareSrcMsgStream* mp_Stream;

    openfluid::tools::Timer m_ProcessTimer;

    WareSrcMsgParser* mp_CurrentParser;

    QList<WareSrcMsgParser::WareSrcMsg> m_Messages;  

    void runCommand(const openfluid::utils::CMakeProxy::CommandInfos& CmdInfos, const QProcessEnvironment& Env,
                    WareSrcProcess::Type CmdType = WareSrcProcess::Type::NONE);


  signals:

    void processLaunched();

    void processFinished();

    void configureProcessLaunched(openfluid::ware::WareType Type, const QString& ID);

    void configureProcessFinished(openfluid::ware::WareType Type, const QString& ID);

    void buildProcessLaunched(openfluid::ware::WareType Type, const QString& ID);

    void buildProcessFinished(openfluid::ware::WareType Type, const QString& ID);


  public:

    WareSrcUIContainer(const QString& AbsolutePath, openfluid::ware::WareType Type, const QString& WareID);

    virtual ~WareSrcUIContainer();

    QString getAbsolutePath() const
    {
      return QString::fromStdString(openfluid::waresdev::WareSrcContainer::getAbsolutePath());
    }

    QString getBuildDirPath() const
    {
      return QString::fromStdString(openfluid::waresdev::WareSrcContainer::getBuildDirPath());
    }

    QString getConfigureGenerator() const
    {
      return QString::fromStdString(openfluid::waresdev::WareSrcContainer::getConfigureGenerator());
    }

    QString getConfigureExtraOptions() const
    {
      return QString::fromStdString(openfluid::waresdev::WareSrcContainer::getConfigureExtraOptions());
    }

    QString getBuildTarget() const
    {
      return QString::fromStdString(openfluid::waresdev::WareSrcContainer::getBuildTarget());
    }

    QString getGenerateDocTarget() const
    {
      return QString::fromStdString(openfluid::waresdev::WareSrcContainer::getGenerateDocTarget());
    }

    std::map<QString,QString> getConfigureVariables() const
    {
      std::map<QString,QString> Vars;

      for (const auto& V : openfluid::waresdev::WareSrcContainer::getConfigureVariables())
      {
        Vars[QString::fromStdString(V.first)] = QString::fromStdString(V.second);
      }

      return Vars;
    }

    void setMsgStream(WareSrcMsgStream& Stream);

    void configure();

    void build();

    void generateDoc();

    QProcessEnvironment getConfigureEnvironment() const;

    QProcessEnvironment getBuildEnvironment() const;

    QList<WareSrcMsgParser::WareSrcMsg> getMessages();

    bool isProcessRunning() const;
};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESRCUICONTAINER_HPP__ */
