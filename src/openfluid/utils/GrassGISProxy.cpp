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
  @file GrassGISProxy.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QString>
#include <QProcess>
#include <QFile>
#include <QTextStream>

#include <openfluid/utils/GrassGISProxy.hpp>
#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/base/Environment.hpp>


namespace openfluid { namespace utils {


GrassGISProxy::GrassGISProxy(const QString& GISBase, const QString& Location, const QString& Mapset) :
  m_GISBase(GISBase), m_Location(Location), m_Mapset(Mapset)
{
  findGrassGISProgram();
}


// =====================================================================
// =====================================================================


GrassGISProxy::~GrassGISProxy()
{

}


// =====================================================================
// =====================================================================


void GrassGISProxy::findGrassGISProgram()
{
  if (m_ExecutablePath.isEmpty())
  {
    m_ExecutablePath = ExternalProgram::getRegisteredProgram(
      ExternalProgram::RegisteredPrograms::GrassGis).getFullProgramPath();

    if (!m_ExecutablePath.isEmpty())
    {
      QProcess Pcs;

      Pcs.setProcessChannelMode(QProcess::MergedChannels);

      Pcs.start(QString("\"%1\" --help").arg(m_ExecutablePath));

      Pcs.waitForStarted(-1);
      Pcs.waitForReadyRead(-1);
      Pcs.waitForFinished(-1);

      QStringList Output = QString::fromUtf8(Pcs.readAll()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

      if (!Output.isEmpty() && Output[0].startsWith("GRASS GIS "))
      {
        m_Version = Output[0];
        m_Version.remove(0,10);
      }
    }
  }
}


// =====================================================================
// =====================================================================


QString GrassGISProxy::getCommandLine(const QString& Command,
                                      const std::map<QString,QString> Arguments,
                                      const std::vector<QString> Options)
{
  QString Cmd = Command;

  for (auto& Arg : Arguments)
  {
    Cmd += " " + Arg.first + "=" + Arg.second;
  }

  for (auto& Opt : Options)
  {
    Cmd += " " + Opt;
  }

  return Cmd;
}


// =====================================================================
// =====================================================================


QString GrassGISProxy::writeJobFile(const QStringList& Lines, const QString& DirPath)
{
  QString FullFilePath =
      QString::fromStdString(openfluid::tools::Filesystem::makeUniqueFile(DirPath.toStdString(),"grassproxy-job.sh"));

  if (!FullFilePath.isEmpty())
  {
    QFile JobFile(FullFilePath);

    if (JobFile.open(QIODevice::ReadWrite))
    {
      QTextStream Out(&JobFile);

      Out << "#!/bin/sh\n\n";

      Out << Lines.join("\n") << "\n";

      JobFile.close();

      JobFile.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadGroup|QFile::ReadOther);

      return FullFilePath;
    }
  }

  return "";
}


// =====================================================================
// =====================================================================


bool GrassGISProxy::isAvailable()
{
  findGrassGISProgram();

  return (!m_ExecutablePath.isEmpty() && !m_Version.isEmpty());
}


// =====================================================================
// =====================================================================


int GrassGISProxy::executeGrassJob(const QString& JobFilePath) const
{
  if (!isLocationExist())
  {
    return -99;
  }

  QProcessEnvironment PcsEnv = QProcessEnvironment::systemEnvironment();
  PcsEnv.insert("GRASS_BATCH_JOB",JobFilePath);

  QProcess Pcs;
  Pcs.setProcessEnvironment(PcsEnv);

  if (m_OutputFile.isEmpty())
  {
    Pcs.setStandardOutputFile(JobFilePath+".out");
  }
  else
  {
    Pcs.setStandardOutputFile(m_OutputFile);
  }

  if (m_ErrorFile.isEmpty())
  {
    Pcs.setStandardErrorFile(JobFilePath+".err");
  }
  else
  {
    Pcs.setStandardErrorFile(m_ErrorFile);
  }


  QString Path = QString("%1/%2/%3").arg(m_GISBase).arg(m_Location).arg(m_Mapset);

  QString Command = QString("\"%1\" \"%2\"").arg(m_ExecutablePath).arg(Path);

  Pcs.start(Command);
  Pcs.waitForStarted(-1);
  Pcs.waitForFinished(-1);

  return Pcs.exitCode();
}


// =====================================================================
// =====================================================================


int GrassGISProxy::executeGrassJobReturningData(const QString& JobFilePath, QStringList& Lines) const
{
  if (!isLocationExist())
  {
    return -99;
  }

  QProcessEnvironment PcsEnv = QProcessEnvironment::systemEnvironment();
  PcsEnv.insert("GRASS_BATCH_JOB",JobFilePath);

  QProcess Pcs;
  Pcs.setProcessEnvironment(PcsEnv);


  QString Path = QString("%1/%2/%3").arg(m_GISBase).arg(m_Location).arg(m_Mapset);

  QString Command = QString("\"%1\" \"%2\"").arg(m_ExecutablePath).arg(Path);

  Pcs.start(Command);
  Pcs.waitForStarted(-1);
  Pcs.waitForFinished(-1);

  if (Pcs.exitCode() == 0)
  {
    Lines = QString(Pcs.readAllStandardOutput()).split("\n");
  }

  return Pcs.exitCode();
}


// =====================================================================
// =====================================================================


void GrassGISProxy::appendTask(const QString& Command,
                               const std::map<QString,QString> Arguments,
                               const std::vector<QString> Options)
{
  m_JobLines << getCommandLine(Command,Arguments,Options);
}


// =====================================================================
// =====================================================================


int GrassGISProxy::runJob()
{
  int RetValue = -1;

  QString JobFilePath = writeJobFile(m_JobLines,QString::fromStdString(openfluid::base::Environment::getTempDir()));

  if (!JobFilePath.isEmpty())
  {
    RetValue = executeGrassJob(JobFilePath);
    clearTasks();
  }

  return RetValue;
}


// =====================================================================
// =====================================================================


int GrassGISProxy::runSingleTask(const QString& Command,
                                 const std::map<QString,QString> Arguments,
                                 const std::vector<QString> Options) const
{
  int RetValue = -1;

  QString JobFilePath = writeJobFile(QStringList() << getCommandLine(Command,Arguments,Options),
                                     QString::fromStdString(openfluid::base::Environment::getTempDir()));

  if (!JobFilePath.isEmpty())
  {
    RetValue = executeGrassJob(JobFilePath);
  }

  return RetValue;
}


// =====================================================================
// =====================================================================


bool GrassGISProxy::isLocationExist() const
{
  QString Path = QString("%1/%2").arg(m_GISBase).arg(m_Location);

  return openfluid::tools::Filesystem::isDirectory(Path.toStdString());
}


// =====================================================================
// =====================================================================


bool GrassGISProxy::createLocation(const QString& EPSG) const
{
  QProcessEnvironment PcsEnv = QProcessEnvironment::systemEnvironment();

  QProcess Pcs;
  Pcs.setProcessEnvironment(PcsEnv);

  QString Path = QString("%1/%2").arg(m_GISBase).arg(m_Location);

  QString Command = QString("\"%1\" -c \"\%2\" -e \"%3\"").arg(m_ExecutablePath).arg(EPSG).arg(Path);

  Pcs.start(Command);
  Pcs.waitForStarted(-1);
  Pcs.waitForFinished(-1);

  return (Pcs.exitCode() == 0);
}


// =====================================================================
// =====================================================================


std::map<std::string,double> GrassGISProxy::region() const
{
  std::map<std::string,double> Values;

  QString JobFilePath = writeJobFile(QStringList() << getCommandLine("g.region",{},{"-gucep"}),
                                       QString::fromStdString(openfluid::base::Environment::getTempDir()));

  if (!JobFilePath.isEmpty())
  {
    QStringList Lines;
    if (executeGrassJobReturningData(JobFilePath,Lines) == 0)
    {
      bool ConvertOK;
      double ConvertedVal;

      for (QString& Ln : Lines)
      {
        QStringList Parts = Ln.split("=",QString::SkipEmptyParts);

        if (Parts.size() == 2)
        {
          ConvertedVal = Parts[1].toDouble(&ConvertOK);
          if (ConvertOK)
          {
            Values[Parts[0].toStdString()] = ConvertedVal;
          }
        }
      }
    }
  }

  return Values;
}


// =====================================================================
// =====================================================================


std::map<std::string,std::string> GrassGISProxy::gisenv() const
{
  std::map<std::string,std::string> Values;

  QString JobFilePath = writeJobFile(QStringList() << getCommandLine("g.gisenv",{},{"-n","--q"}),
                                       QString::fromStdString(openfluid::base::Environment::getTempDir()));

  if (!JobFilePath.isEmpty())
  {
    QStringList Lines;
    if (executeGrassJobReturningData(JobFilePath,Lines) == 0)
    {
      for (QString& Ln : Lines)
      {
        QStringList Parts = Ln.split("=",QString::SkipEmptyParts);

        if (Parts.size() == 2)
        {
            Values[Parts[0].toStdString()] = Parts[1].toStdString();
        }
      }
    }
  }

  return Values;
}


// =====================================================================
// =====================================================================


std::vector<std::string> GrassGISProxy::mapsets() const
{
  std::vector<std::string> Values;


  QString JobFilePath = writeJobFile(QStringList() << getCommandLine("g.mapsets",{{"separator","'\\n'"}},{"-l"}),
                                       QString::fromStdString(openfluid::base::Environment::getTempDir()));

  if (!JobFilePath.isEmpty())
  {
    QStringList Lines;
    if (executeGrassJobReturningData(JobFilePath,Lines) == 0)
    {
      for (QString& Ln : Lines)
      {
        if (!Ln.trimmed().isEmpty())
        {
          Values.push_back(Ln.toStdString());
        }
      }
    }
  }

  return Values;
}


} }  // namespaces
