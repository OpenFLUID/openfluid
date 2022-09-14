/**
  @file WareUI.hpp
*/


#ifndef __WAREUI_HPP__
#define __WAREUI_HPP__


#include <openfluid/ui/builderext/PluggableParameterizationExtension.hpp>


class %%PARAMSUICLASSNAME%%: public openfluid::ui::builderext::PluggableParameterizationExtension
{
  Q_OBJECT;


  public:

    %%PARAMSUICLASSNAME%%();

    ~%%PARAMSUICLASSNAME%%();

    void update();

};


#endif /* __WAREUI_HPP__ */

