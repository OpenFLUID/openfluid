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
  \file ElidedLabel.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __ELIDEDLABEL_HPP__
#define __ELIDEDLABEL_HPP__


#include <openfluid/dllexport.hpp>
#include <QLabel>


namespace openfluid { namespace ui { namespace common {


/**
  Single line elided label class, based on QLabel
*/
class DLLEXPORT ElidedLabel : public QLabel
{
  Q_OBJECT

  private:

    QString m_CachedElidedText;

    Qt::TextElideMode m_ElideMode;


  protected:

    void paintEvent(QPaintEvent *Ev);

    void resizeEvent(QResizeEvent *Ev);


  protected:

    void cacheElidedText(int w);


  public:

    ElidedLabel(QWidget* Parent=0, Qt::WindowFlags Flags=0);

    ElidedLabel(const QString& Text, QWidget* Parent=0, Qt::WindowFlags Flags=0);

    ElidedLabel(const QString& Text, Qt::TextElideMode ElideMode = Qt::ElideRight,
                QWidget* Parent=0, Qt::WindowFlags Flags=0);

    void setElideMode(Qt::TextElideMode ElideMode);

    Qt::TextElideMode getElideMode() const
    { return m_ElideMode; }

    void setText(const QString& Text);
};


} } } // namespaces

#endif /* __ELIDEDLABEL_HPP__ */
