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


#ifndef SRC_OPENFLUID_WARESDEV_WARESRCCONTAINER_HPP_
#define SRC_OPENFLUID_WARESDEV_WARESRCCONTAINER_HPP_

#include <QString>

#include <openfluid/waresdev/WareSrcManager.hpp>


namespace openfluid { namespace waresdev {

class WareSrcContainer
{
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

  public:

    WareSrcContainer(const QString& AbsolutePath, WareSrcManager::WareType Type,
                     const QString& WareName);

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
};


} }  // namespaces


#endif /* SRC_OPENFLUID_WARESDEV_WARESRCCONTAINER_HPP_ */
