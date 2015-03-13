/**
  @file %%PARAMSUIROOTHPPFILENAME%%
*/


#ifndef %%PARAMSUIHPPHEADERGUARD%%
#define %%PARAMSUIHPPHEADERGUARD%%


#include <openfluid/ui/ware/ParameterizationWidget.hpp>


// =====================================================================
// =====================================================================


class %%PARAMSUICLASSNAME%%: public openfluid::ui::ware::ParameterizationWidget
{
  Q_OBJECT;


  public:

    %%PARAMSUICLASSNAME%%();

    ~%%PARAMSUICLASSNAME%%();

    void update();

};


#endif /* %%PARAMSUIHPPHEADERGUARD%% */

