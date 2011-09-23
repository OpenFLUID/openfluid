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
 \file Layer.hpp
 \brief Header of ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

#ifndef __LAYER_HPP__
#define __LAYER_HPP__

#include <iostream>
#include <vector>
#include <set>
#include <gtkmm/comboboxtext.h>
#include <openfluid/core/CoreRepository.hpp>

#include "ICLayer.hpp"
#include "LayerType.hpp"

//class ICLayer;
class WidgetLayerObject;

class Layer
{

  private:

    ICLayer* mp_ICLayer;
    WidgetLayerObject* mp_WidgetLayerObject;
    const LayerType::LayerTypes m_LayerType;

    std::string m_ClassName;
    std::string m_FileName;
    std::string m_FolderUri;

    int m_SizeLine;

    double m_Red;
    double m_Green;
    double m_Blue;
    double m_Alpha;

    std::pair<std::pair<double, double>, std::pair<double, double> >
        m_MinMaxLayer;

    bool m_IsSelected;
    bool m_IsDisplay;
    bool m_LoadShapeFile;

    //***************Signal WidgetObjectBase*********************
    void whenOnUpLayerButtonClicked();
    void whenOnDownLayerButtonClicked();
    void whenOnRemoveLayerButtonClicked();
    void whenOnIsDisplayButtonChecked();
    void whenOnIsSelectedLayerClicked();
    //***************Signal WidgetExpanderBase*******************
    void whenOnWidgetExpanderBaseChanged(int, double, double, double, double);

    typedef sigc::signal<void> mtype_SignalLayer;
    typedef sigc::signal<void,std::string> mtype_SignalLayerString;

    mtype_SignalLayerString m_signal_UpLayerButtonClicked;
    mtype_SignalLayerString m_signal_DownLayerButtonClicked;
    mtype_SignalLayerString m_signal_RemoveLayerButtonClicked;
    mtype_SignalLayerString m_signal_IsSelectedLayerClicked;
    mtype_SignalLayer m_signal_IsDisplayButtonChecked;
    mtype_SignalLayer m_signal_WidgetExpanderBaseChanged;

    void onComboChanged(Gtk::ComboBoxText*);

  public:

    Layer(const LayerType::LayerTypes);
    ~Layer();

    void addNewLayer(std::pair<std::pair<std::string, std::string>, std::string>);
    void loadShapefile(std::string, std::string);
    void initialiseLayerContext(Cairo::RefPtr<Cairo::Context>, double);
    void draw(Cairo::RefPtr<Cairo::Context>, double, std::set<int>);

    int isObjectSelected(double, double, double);
    std::set<int> ObjectSelected();

    Gtk::Widget* asWidget();

    mtype_SignalLayerString signalUpLayerButtonClicked();
    mtype_SignalLayerString signalDownLayerButtonClicked();
    mtype_SignalLayerString signalRemoveLayerButtonClicked();
    mtype_SignalLayerString signalIsSelectedLayerClicked();
    mtype_SignalLayer signalIsDisplayButtonChecked();
    mtype_SignalLayer signalWidgetExpanderBaseChanged();

    void update(openfluid::core::CoreRepository&);

    //accessor
    //get
    std::pair<std::pair<double, double>, std::pair<double, double> >
    getMinMaxLayer();
    std::pair<std::pair<double, double>, std::pair<double, double> >
        getMinMaxSelection(std::set<int>);
    bool getIsDisplay();
    bool getIsSelected();
    bool getLoadShapeFile();
    std::string getClassName();
    std::string getFileName();
    std::string getFolderUri();
    ICLayer* getICLayer();
    //set
    void setIsSelected(bool);
    void setWidgetUpSensitive(bool);
    void setWidgetDownSensitive(bool);
};

#endif /* __LAYER_HPP__ */
