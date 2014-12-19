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
 \file WareSrcContainer.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_WARESDEV_WARESRCCONTAINER_HPP__
#define __OPENFLUID_WARESDEV_WARESRCCONTAINER_HPP__

#include <openfluid/dllexport.hpp>

#include <QString>
#include <QObject>

#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/waresdev/OStreamMsgStream.hpp>

class QProcess;

namespace openfluid { namespace waresdev {

class OPENFLUID_API WareSrcContainer: public QObject
{
  Q_OBJECT

  public:

    enum ConfigMode
    {
      CONFIG_DEBUG, CONFIG_RELEASE
    };

    enum BuildMode
    {
      BUILD_WITHINSTALL, BUILD_NOINSTALL
    };

  private:

    QString m_AbsolutePath;

    /**
     * Absolute path of the CMake config file if it exists, otherwise an empty string
     */
    QString m_AbsoluteCMakeConfigPath;

    /**
     * Absolute path of the main .cpp as set in the CMakeLists.txt file, if this .cpp file exists, otherwise an empty string
     */
    QString m_AbsoluteMainCppPath;

    QString m_CMakePath;

    openfluid::waresdev::WareSrcMsgStream* mp_Stream;

    ConfigMode m_ConfigMode;

    BuildMode m_BuildMode;

    QString m_BuildDirPath;

    QProcess* mp_Process;

    QString m_OFVersion;

    /**
     * @throw openfluid::base::FrameworkException
     */
    void findCMake();

    void runCommand(const QString& Command);

  private slots:

    void processOutput();

    void processFinishedOutput(int ExitCode);

  public:

    WareSrcContainer(const QString& AbsolutePath, WareSrcManager::WareType Type,
                     const QString& WareName,
                     openfluid::waresdev::WareSrcMsgStream& Stream =
                         *new openfluid::waresdev::OStreamMsgStream());

    ~WareSrcContainer();

    static QString searchMainCppFileName(const QString& CMakeFileContent);

    /**
     * Return the absolute paths of:
     * - the CMake config file if it exists on disk,
     * - the main .cpp file as set in the CMakeLists.txt, if this .cpp exists on disk.
     * If no .cpp file was found (because it doesn't exists, it wasn't found in CMake file, or CMake file doesn't exist):
     * - the first .cpp file found in this ware directory (not recursive).
     * @return a list of existing absolute paths into this ware directory. It can be empty.
     */
    QStringList getDefaultFiles();

    /**
     * Return the absolute path of the main .cpp as set in the CMakeLists.txt file, if this .cpp file exists, otherwise an empty string
     */
    QString getMainCppFile();

    QString getAbsolutePath() const;

    void setMsgStream(openfluid::waresdev::WareSrcMsgStream& Stream);

    void setConfigMode(ConfigMode Mode);

    void setBuildMode(BuildMode Mode);

    void configure();

    void build();
};


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCCONTAINER_HPP__ */
