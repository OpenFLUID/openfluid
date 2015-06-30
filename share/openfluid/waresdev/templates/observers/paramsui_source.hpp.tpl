/**
  @file %%PARAMSUIROOTHPPFILENAME%%
*/


#ifndef %%PARAMSUIHPPHEADERGUARD%%
#define %%PARAMSUIHPPHEADERGUARD%%


#include <openfluid/builderext/PluggableParameterizationExtension.hpp>


class %%PARAMSUICLASSNAME%%: public openfluid::builderext::PluggableParameterizationExtension
{
  Q_OBJECT;


  public:

    %%PARAMSUICLASSNAME%%();

    ~%%PARAMSUICLASSNAME%%();

    void update();

};


#endif /* %%PARAMSUIHPPHEADERGUARD%% */

