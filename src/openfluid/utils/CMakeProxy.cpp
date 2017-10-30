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

#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/utils/ExternalProgram.hpp>


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
    m_ExecutablePath = ExternalProgram::getRegisteredProgram(ExternalProgram::CMakeProgram).getFullProgramPath();

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


QString CMakeProxy::getConfigureCommand(const QString& BuildDir, const QString& SrcDir,
                                        const std::map<QString,QString>& Variables,
                                        const QString& Generator, const std::vector<QString>& Options)
{
  if (!isAvailable())
    return "";


  QString ArgsOpts;

  if (!Generator.isEmpty())
    ArgsOpts += QString(" -G \"%1\"").arg(Generator);


  for (const auto& Var : Variables)
    ArgsOpts += QString(" -D%1=%2").arg(Var.first).arg(Var.second);

  for (const auto& Opt : Options)
    ArgsOpts += QString(" %1").arg(Opt);

  QString Cmd = QString("\"%1\" -E chdir \"%2\" \"%1\" \"%3\"%4")
                .arg(m_ExecutablePath)
                .arg(BuildDir)
                .arg(SrcDir)
                .arg(ArgsOpts);

  return Cmd;
}


// =====================================================================
// =====================================================================


QString CMakeProxy::getBuildCommand(const QString& BuildDir,
                                    const QString& Target,
                                    const QString& CMakeOptions, const QString& OtherOptions)
{
  if (!isAvailable())
    return "";


  QString TargetOpt;
  QString CMakeOpts;
  QString OtherOpts;

  if (!Target.isEmpty())
    TargetOpt = QString(" --target %1").arg(Target);

  if (!CMakeOptions.isEmpty())
    CMakeOpts = " " + CMakeOptions;

  if (!OtherOptions.isEmpty())
    OtherOpts = QString(" -- %1").arg(OtherOptions);

  QString Cmd = QString("\"%1\" -E chdir \"%2\" \"%1\" --build .%3%4%5")
                  .arg(m_ExecutablePath)
                  .arg(BuildDir)
                  .arg(TargetOpt)
                  .arg(CMakeOpts)
                  .arg(OtherOpts);

  return Cmd;
}


// =====================================================================
// =====================================================================


QString CMakeProxy::getTarCompressCommand(const QString& WorkDir,
                                          const QString& TarFilePath, const QStringList& RelativePathsToCompress,
                                          const QString& Options)
{
  if (!isAvailable())
    return "";

  return QString("\"%1\" -E chdir \"%2\" \"%1\" -E tar cf%3 \"%4\" \"%5\"")
         .arg(m_ExecutablePath)
         .arg(WorkDir)
         .arg(Options)
         .arg(TarFilePath)
         .arg(RelativePathsToCompress.join("\" \""));
}


// =====================================================================
// =====================================================================


QString CMakeProxy::getTarUncompressCommand(const QString& WorkDir, const QString& TarFilePath,
                                            const QString& Options)
{
  if (!isAvailable())
    return "";

  return QString ("\"%1\" -E chdir \"%2\" \"%1\" -E tar xf%3 \"%4\"")
         .arg(m_ExecutablePath)
         .arg(WorkDir)
         .arg(Options)
         .arg(TarFilePath);
}


} }  // namespaces
