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
  @file CMakeProxy.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QProcess>

#include <openfluid/utilsq/CMakeProxy.hpp>
#include <openfluid/utilsq/ExternalProgram.hpp>


namespace openfluid { namespace utils {


CMakeProxy::CMakeProxy()
{
  findCMakeProgram();
}


// =====================================================================
// =====================================================================


CMakeProxy::~CMakeProxy()
{

}


// =====================================================================
// =====================================================================


void CMakeProxy::findCMakeProgram()
{
  if (m_ExecutablePath.isEmpty())
  {
    m_ExecutablePath = ExternalProgram::getRegisteredProgram(
      ExternalProgram::RegisteredPrograms::CMake).getFullProgramPath();

    if (!m_ExecutablePath.isEmpty())
    {
      QProcess Pcs;

      Pcs.start(QString("\"%1\" --version").arg(m_ExecutablePath));

      Pcs.waitForReadyRead(-1);
      Pcs.waitForFinished(-1);

      QStringList Output = QString::fromUtf8(Pcs.readAll()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

      if (!Output.isEmpty() && Output[0].startsWith("cmake version "))
      {
        m_Version = Output[0];
        m_Version.remove(0,14);
      }
    }
  }
}


// =====================================================================
// =====================================================================


bool CMakeProxy::isAvailable()
{
  findCMakeProgram();

  return (!m_ExecutablePath.isEmpty() && !m_Version.isEmpty());
}


// =====================================================================
// =====================================================================


CMakeProxy::CommandInfos CMakeProxy::getConfigureCommand(const QString& BuildDir, const QString& SrcDir,
                                                         const std::map<QString,QString>& Variables,
                                                         const QString& Generator, const QStringList& Options)
{
  CMakeProxy::CommandInfos Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  Cmd.Args << "-E" << "chdir" << BuildDir; // cd to build directory
  Cmd.Args << m_ExecutablePath << SrcDir;  // cmake configure command with the sources directory 

  if (!Generator.isEmpty())
  {
    Cmd.Args << "-G" << Generator;
  }

  for (const auto& Var : Variables)
  {
    Cmd.Args << QString("-D%1=%2").arg(Var.first).arg(Var.second);
  }

  for (const auto& Opt : Options)
  {
    Cmd.Args << Opt;
  }

  return Cmd;
}


// =====================================================================
// =====================================================================


CMakeProxy::CommandInfos CMakeProxy::getBuildCommand(const QString& BuildDir,
                                                     const QString& Target,
                                                     const unsigned int Jobs,
                                                     const QStringList& CMakeOptions, 
                                                     const QStringList& OtherOptions)
{
  CMakeProxy::CommandInfos Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  Cmd.Args << "-E" << "chdir" << BuildDir; // cd to build directory
  Cmd.Args << m_ExecutablePath << "--build" << "."; // build command in the current directory 

  if (!Target.isEmpty())
  {
    Cmd.Args << "--target" << Target;
  }

  for (const auto& Opt : CMakeOptions)
  {
    Cmd.Args << Opt;
  }

  // Add Jobs option
  if (Jobs || !OtherOptions.empty())
  {
    Cmd.Args << "--";

    if (Jobs)
    {
      Cmd.Args << "-j" << QString("%1").arg(Jobs);
    }

    for (const auto& Opt : OtherOptions)
    {
      Cmd.Args << Opt;
    }
  }

  return Cmd;
}


// =====================================================================
// =====================================================================


CMakeProxy::CommandInfos CMakeProxy::getTarCompressCommand(const QString& WorkDir,
                                                           const QString& TarFilePath, 
                                                           const QStringList& RelativePathsToCompress,
                                                           const QString& Options)
{
  CMakeProxy::CommandInfos Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  Cmd.Args << "-E" << "chdir" << WorkDir; // cd to work directory
  Cmd.Args << m_ExecutablePath << "-E" << "tar"; // tar command
  Cmd.Args << "cf"+Options;
  Cmd.Args << TarFilePath;
  Cmd.Args << RelativePathsToCompress;

  return Cmd;
}


// =====================================================================
// =====================================================================


CMakeProxy::CommandInfos CMakeProxy::getTarUncompressCommand(const QString& WorkDir, const QString& TarFilePath,
                                                             const QString& Options)
{
  CMakeProxy::CommandInfos Cmd;

  if (!isAvailable())
  {
    return Cmd;
  }

  Cmd.Program = m_ExecutablePath;

  Cmd.Args << "-E" << "chdir" << WorkDir; // cd to work directory
  Cmd.Args << m_ExecutablePath << "-E" << "tar"; // tar command
  Cmd.Args << "xf"+Options;
  Cmd.Args << TarFilePath;

  return Cmd;
}


} }  // namespaces
