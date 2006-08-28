/**
  \file ValueFactory.h
  \brief header de la classe ValueFactory
  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/




#ifndef __VALUEFACTORY_H__
#define __VALUEFACTORY_H__


namespace mhydasdk { namespace core {

/**
  \brief Classe ValueFactory (conversion de valeurs numériques)

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
template <class T>
class ValueFactory
{

  private:

    T m_Multiplier;

  public:

    /**
      Constructeur
    */
    ValueFactory(T Multiplier) { m_Multiplier = Multiplier;};

    /**
      Constructeur par défaut
    */
    ValueFactory() { m_Multiplier = 1;};


    T create(T Input) {return (Input * m_Multiplier);};

    T getMin(T Val1, T Val2) { if (Val1 < Val2) return Val1; else return Val2;};

    T getMax(T Val1, T Val2) { if (Val1 > Val2) return Val1; else return Val2;};

};


} } // namespace mhydasdk::core


#endif

