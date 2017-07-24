##
#
#  This file is part of OpenFLUID software
#  Copyright(c) 2007, INRA - Montpellier SupAgro
#
#
# == GNU General Public License Usage ==
#
#  OpenFLUID is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  OpenFLUID is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.
#
#
# == Other Usage ==
#
#  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
#  license, and requires a written agreement between You and INRA.
#  Licensees for Other Usage of OpenFLUID may use this file in accordance
#  with the terms contained in the written agreement between You and INRA.
#  
##


#### This file is deprecated and is for compatibility only ####


# Replace by using the FIND_PACKAGE(Qt5) command provided by Qt5 for CMake, 
# then use the Qt5::<module> notation in your target to be linked to Qt5
# 
# Example of CMake.in.cmake for linking an OpenFLUID simulator to Qt5 libraries
#
#  FIND_PACKAGE(Qt5 REQUIRED Core)
#  SET(SIM_LINK_LIBS Qt5::Core)
#
# More information on the Qt5 dedicated page : http://doc.qt.io/qt-5/cmake-manual.html
#


FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Network Xml Svg REQUIRED)
MESSAGE(STATUS "Found Qt5 (version ${Qt5_VERSION})")
CMAKE_POLICY(SET CMP0020 OLD)

SET(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS}
                ${Qt5Network_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS} ${Qt5Svg_INCLUDE_DIRS})
                  
SET(QT_QTCORE_LIBRARIES Qt5::Core)
SET(QT_QTGUI_LIBRARIES Qt5::Widgets)
SET(QT_QTNETWORK_LIBRARIES Qt5::Network)
SET(QT_QTXML_LIBRARIES Qt5::Xml)
SET(QT_QTSVG_LIBRARIES Qt5::Svg)

SET(QT_QTCORE_LIBRARY Qt5::Core)
SET(QT_QTGUI_LIBRARY Qt5::Widgets)
SET(QT_QTNETWORK_LIBRARY Qt5::Network)
SET(QT_QTXML_LIBRARY Qt5::Xml)
SET(QT_QTSVG_LIBRARY Qt5::Svg)

 