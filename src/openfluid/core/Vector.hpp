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
  @file Vector.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_CORE_VECTOR_HPP__
#define __OPENFLUID_CORE_VECTOR_HPP__


#include <iostream>
#include <openfluid/dllexport.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace core {


/**
  Template class for vector data
*/
template <class T>
class OPENFLUID_API Vector
{
  public:

    typedef T* iterator;

    typedef const T* const_iterator;


  protected :

    T* m_Data;

    unsigned long m_Size;

    bool allocate(unsigned long Size);

    void init();

    static void copy(const Vector& Source, Vector& Dest);


  public :

	/**
	  Default constructor, creates an empty vector
	*/
    Vector();

	/**
	  Copy constructor
	*/
    Vector(const Vector &Vector);

	/**
	  Constructor, creates a vector containing Size elements
	*/
    Vector(unsigned long Size);

	/**
	  Constructor, creates a vector containing Size elements, initialized with value InitValue
	*/
    Vector(unsigned long Size, T InitValue);

    /**
      Constructor, creates a vector of size Size, containing Data
    */
    Vector(T* Data, unsigned long Size);

    /**
      Destructor
    */
    virtual ~Vector();

    /**
      Returns the size of the vector
      @return size of the vector
    */
    unsigned long getSize() const
    { return m_Size; };

    /**
      Returns the size of the vector
      @return size of the vector
    */
    unsigned long size() const
    { return getSize(); };

    /**
      Returns a pointer to the content of the vector (like C arrays)
    */
    T* data() const { return m_Data; };

    /**
      Sets data for the vector from Data and with size Size
    */
    void setData(T* Data, unsigned long Size);

    /**
      Returns the element of the vector for index Index
    */
    T getElement(unsigned long Index) const;

    /**
      Returns the element of the vector for index Index
    */
    inline T at(unsigned long Index) const { return getElement(Index); };

    /**
      Returns the element of the vector for index Index
    */
    inline T get(unsigned long Index) const { return getElement(Index); };


    /**
      Sets a new value for element at the given index
    */
    void setElement(unsigned long Index, T Element);

    /**
      Sets a new value for element at the given index
    */
    inline void set(unsigned long Index, T Element) { setElement(Index,Element); };


    /**
      Operator to set a new value for element given between []
    */
    T& operator[](unsigned long Index);

    /**
      Allocation operator
    */
    Vector<T>& operator=(const Vector &A);

    /**
      Fills the vector with given value
    */
    void fill(const T& Val);


    /**
      Clears the vector (empty and size is 0)
    */
    void clear();

    /**
      Returns an iterator referring to the first element in the vector
      @return an iterator to the first element in the vector
    */
    inline iterator begin()
    { return &m_Data[0]; }

    /**
      Returns a constant iterator referring to the first element in the vector
      @return a constant iterator to the first element in the vector
    */
    inline const_iterator begin() const
    { return &m_Data[0]; }

    /**
      Returns an iterator referring to the past-the-end element in the vector
      @return an iterator to the past-the-end element in the vector
    */
    inline iterator end()
    { return &m_Data[m_Size]; }

    /**
      Returns a constant iterator referring to the past-the-end element in the vector
      @return a constant iterator to the past-the-end element in the vector
    */
    inline const_iterator end() const
    { return &m_Data[m_Size]; }

};


// =====================================================================
// =====================================================================


template <class T>
Vector<T>::Vector()
{
  init();
}


// =====================================================================
// =====================================================================


template <class T>
Vector<T>::Vector(const Vector &A)
{
  init();

  if (!allocate(A.m_Size))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");

  std::copy(A.m_Data, A.m_Data + A.m_Size, m_Data);
}


// =====================================================================
// =====================================================================


template <class T>
Vector<T>::Vector(unsigned long Size)
{
  init();

  if (!allocate(Size))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");
}


// =====================================================================
// =====================================================================


template <class T>
Vector<T>::Vector(unsigned long Size, T InitValue)
{
  init();

  if (!allocate(Size))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");


  if (m_Data != nullptr)
  {
    unsigned long i;
    for (i=0;i<m_Size;i++) m_Data[i] = InitValue;
  }
}


// =====================================================================
// =====================================================================


template <class T>
Vector<T>::Vector(T* Data, unsigned long Size)
{
  init();

  if (!allocate(Size))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");

  std::copy(Data, Data + Size, m_Data);

}


// =====================================================================
// =====================================================================


template <class T>
Vector<T>::~Vector()
{
  clear();
}


// =====================================================================
// =====================================================================


template <class T>
bool Vector<T>::allocate(unsigned long Size)
{

  if (Size > 0)
  {
    m_Data = new T[Size];
    if (m_Data != nullptr)
      m_Size = Size;
    else
      return false;
  }

  return true;
}


// =====================================================================
// =====================================================================


template <class T>
void Vector<T>::setData(T* Data, unsigned long Size)
{
  clear();

  if (!allocate(Size))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");

  std::copy(Data, Data + Size, m_Data);
}


// =====================================================================
// =====================================================================


template <class T>
T Vector<T>::getElement(unsigned long Index) const
{
  if (Index >= m_Size)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"element access range error");

  return m_Data[Index];
}


// =====================================================================
// =====================================================================


template <class T>
void Vector<T>::setElement(unsigned long Index, T Element)
{
  if (Index >= m_Size)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"element access range error");
  m_Data[Index] = Element;
}


// =====================================================================
// =====================================================================


template <class T>
T& Vector<T>::operator[](unsigned long Index)
{
  if (Index >= m_Size)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"element access range error");

  return m_Data[Index];
}


// =====================================================================
// =====================================================================


template <class T>
Vector<T>& Vector<T>::operator=(const Vector &A)
{
  if (this == &A) // in case somebody tries assign array to itself
    return *this;

  clear();

  allocate(A.m_Size);
  std::copy(A.m_Data, A.m_Data + A.m_Size, m_Data);

  return *this;
}


// =====================================================================
// =====================================================================


template <class T>
void Vector<T>::init()
{
  m_Data = nullptr;
  m_Size = 0;
}


// =====================================================================
// =====================================================================


template <class T>
void Vector<T>::fill(const T& Val)
{
  std::fill(m_Data, m_Data + m_Size,Val);
}


// =====================================================================
// =====================================================================


template <class T>
void Vector<T>::clear()
{
  delete [] m_Data;
  init();
}


// =====================================================================
// =====================================================================


template <class T>
void Vector<T>::copy(const Vector& Source, Vector& Dest)
{
  Dest.clear();
  Dest.allocate(Source.m_Size);
  for (unsigned long i = 0; i < Source.m_Size;i++)
  {
    Dest.m_Data[i] = Source.m_Data[i];
  }
}


} }  // namespaces


#endif
