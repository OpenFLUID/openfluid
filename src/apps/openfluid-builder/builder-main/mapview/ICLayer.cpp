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
 \file ICLayer.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#include "ICLayer.hpp"


ICLayer::ICLayer()
{
}

// =====================================================================
// =====================================================================

ICLayer::~ICLayer()
{
}

// =====================================================================
// =====================================================================

int ICLayer::gettype()
{ return m_type; }

// =====================================================================
// =====================================================================


int ICLayer::getPosition()
{ return m_Position; }

// =====================================================================
// =====================================================================


int ICLayer::getId()
{ return m_Id; }

// =====================================================================
// =====================================================================

int ICLayer::getSizeLine()
{ return m_SizeLine; }

// =====================================================================
// =====================================================================

bool ICLayer::getIsDisplay()
{ return m_IsDisplay; }

// =====================================================================
// =====================================================================

double ICLayer::getmaxX()
{ return m_maxX; }

// =====================================================================
// =====================================================================

double ICLayer::getmaxY()
{ return m_maxY; }

// =====================================================================
// =====================================================================

double ICLayer::getminX()
{ return m_minX; }

// =====================================================================
// =====================================================================

double ICLayer::getminY()
{ return m_minY; }

// =====================================================================
// =====================================================================

double ICLayer::getBlue()
{ return m_Blue; }

// =====================================================================
// =====================================================================

double ICLayer::getGreen()
{ return m_Green; }

// =====================================================================
// =====================================================================

double ICLayer::getRed()
{ return m_Red; }

// =====================================================================
// =====================================================================

double ICLayer::getAlpha()
{ return m_Alpha; }

// =====================================================================
// =====================================================================

std::vector<OGRGeometry*> ICLayer::getObjectGeo()
{ return m_ObjectGeo; }

// =====================================================================
// =====================================================================

void ICLayer::addObjectGeo(OGRGeometry* ObjectGeo)
{
  m_ObjectGeo.push_back(ObjectGeo);
}

// =====================================================================
// =====================================================================

void ICLayer::settype(int type)
{ m_type = type; }

// =====================================================================
// =====================================================================

void ICLayer::setId(int Id)
{ m_Id = Id; }

// =====================================================================
// =====================================================================

void ICLayer::setPosition(int Position)
{ m_Position = Position; }

// =====================================================================
// =====================================================================

void ICLayer::setSizeLine(int SizeLine)
{ m_SizeLine = SizeLine; }

// =====================================================================
// =====================================================================


void ICLayer::setIsDisplay(bool IsDisplay)
{ m_IsDisplay = IsDisplay; }

// =====================================================================
// =====================================================================


void ICLayer::setmaxX(double maxX)
{ m_maxX = maxX; }

// =====================================================================
// =====================================================================

void ICLayer::setmaxY(double maxY)
{ m_maxY = maxY; }

// =====================================================================
// =====================================================================

void ICLayer::setminX(double minX)
{ m_minX = minX; }

// =====================================================================
// =====================================================================

void ICLayer::setminY(double minY)
{ m_minY = minY; }

// =====================================================================
// =====================================================================

void ICLayer::setBlue(double Blue)
{ m_Blue = Blue; }

// =====================================================================
// =====================================================================

void ICLayer::setGreen(double Green)
{ m_Green = Green; }

// =====================================================================
// =====================================================================

void ICLayer::setRed(double Red)
{ m_Red = Red; }

// =====================================================================
// =====================================================================

void ICLayer::setAlpha(double Alpha)
{ m_Alpha = Alpha; }

// =====================================================================
// =====================================================================

