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
  @file SVGFileGeneratorWorker.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __SVGFILEGENERATORWORKER_HPP__
#define __SVGFILEGENERATORWORKER_HPP__


#include <QObject>
#include <QMap>

#include <openfluid/fluidx/SpatialDomainDescriptor.hpp>


class SVGFileGeneratorWorker : public QObject
{

  Q_OBJECT;

  private:

    const QString m_GVFileName;

    const QString m_SVGFileName;

    const openfluid::fluidx::SpatialDomainDescriptor* mp_DomainDesc;

    QMap<QString,QString> m_Options;


  signals:

    void finished();


  public slots:

    void run();


  public:

    SVGFileGeneratorWorker(const QString& GVFileName, const QString& SVGFileName,
                           openfluid::fluidx::SpatialDomainDescriptor* DomainDesc);

    ~SVGFileGeneratorWorker();

    void setOption(const QString& Name, const QString& Value);

};


#endif /* __SVGFILEGENERATORWORKER_HPP__ */
