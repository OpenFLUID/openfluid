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
  \file UnitsClassWidget.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __UNITSCLASSWIDGET_HPP__
#define __UNITSCLASSWIDGET_HPP__


namespace Ui
{
  class UnitsClassWidget;
}


#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>

#include <QFrame>
#include <QMouseEvent>
#include <QColor>


class UnitsClassWidget : public QFrame
{
  Q_OBJECT;

  private slots:

    void toggleShowHideStyle();

    void notifyUpClicked();

    void notifyDownClicked();

    void notifyRemoveClicked();

    void changeVisible();

    void changeLineColor();

    void changeFillColor();

    void changeLineWidth(int Width);


  private:

     Ui::UnitsClassWidget* ui;

     bool m_Selected;

     QString m_ClassName;

     static QString m_ColorButtonStyleSheet;

     int m_LineWidth;

     QColor m_LineColor;

     QColor m_FillColor;

     openfluid::fluidx::DatastoreItemDescriptor* mp_LayerSource;

     void mousePressEvent(QMouseEvent* Event);


  signals:

      void selectionRequested(QString ClassName);

      void upClicked(QString);

      void downClicked(QString);

      void removeClicked(QString);

      void styleChanged(QString);


  public:

    UnitsClassWidget(const QString& ClassName,
                     const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& DSList,
                     QWidget* Parent = NULL);

    ~UnitsClassWidget();

    void setSelected(bool Selected);

    QString getClassName() const
    { return m_ClassName; }

    void setUpButtonEnabled(bool Enabled);

    void setDownButtonEnabled(bool Enabled);

    void linkToDatastoreItem(const std::list<openfluid::fluidx::DatastoreItemDescriptor*>& DSList);

    int getLineWidth() const
    { return m_LineWidth; }

    QColor getLineColor() const
    { return m_LineColor; }

    QColor getFillColor() const
    { return m_FillColor; }

    bool isLayerVisible() const;

    void setLayerVisible();

    const openfluid::fluidx::DatastoreItemDescriptor* getLayerSource() const
    { return mp_LayerSource; }

};



#endif /* __UNITSCLASSWIDGET_HPP__ */
