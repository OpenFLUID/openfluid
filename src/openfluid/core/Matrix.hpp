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
  @file Matrix.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_CORE_MATRIX_HPP__
#define __OPENFLUID_CORE_MATRIX_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace core {


/**
  Template class for matrix data
*/
template <class T>
class OPENFLUID_API Matrix
{
  protected :

    T* m_Data;

    unsigned long m_ColsNbr;

    unsigned long m_RowsNbr;

    bool allocate(unsigned long ColsNbr, unsigned long RowsNbr);

    void init();


  public :

    /**
      Default constructor, creates an empty Matrix
    */
    Matrix();

    /**
      Copy constructor
     */
    Matrix(const Matrix &Matrix);

    /**
      Constructor, creates a Matrix containing Size elements
    */
    Matrix(unsigned long ColsNbr,unsigned long RowsNbr);

    /**
      Constructor, creates a Matrix containing Size elements, initialized with value InitValue
    */
    Matrix(unsigned long ColsNbr,unsigned long RowsNbr, T InitValue);

    /**
      Destructor
    */
    virtual ~Matrix();

    /**
      Returns the number of columns of the Matrix
      @return number of columns
    */
    inline unsigned long getColsNbr() const
    { return m_ColsNbr; };

    /**
      Returns the number of rows of the Matrix
      @return number of rows
    */
    inline unsigned long getRowsNbr() const { return m_RowsNbr; };

    /**
      Returns the full size of the Matrix (number of columns x number of rows)
      @return size of the Matrix
    */
    inline unsigned long getSize() const
    { return (m_ColsNbr * m_RowsNbr); };

    /**
      Returns the full size of the Matrix
    */
    inline unsigned long size() const
    { return (m_ColsNbr * m_RowsNbr); };


    /**
      Returns a pointer to the content of the Matrix (like C arrays)
    */
    T* data() const
    { return static_cast<T*>(m_Data); };

    /**
      Sets data from a pointer to a content (like C arrays)
    */
    void setData(T* Data, unsigned long ColsNbr, unsigned long RowsNbr);

    /**
      Returns the element of the Matrix for index Index
    */
    T getElement(unsigned long ColIndex, unsigned long RowIndex) const;

    /**
      Returns the element of the Matrix for index Index
    */
    inline T at(unsigned long ColIndex, unsigned long RowIndex) const
    { return getElement(ColIndex,RowIndex); };

    /**
      Returns the element of the Matrix for index Index
    */
    inline T get(unsigned long ColIndex, unsigned long RowIndex) const
    { return getElement(ColIndex,RowIndex); };


    /**
      Sets a new value for element at the given index
    */
    void setElement(unsigned long ColIndex, unsigned long RowIndex, T Element);

    /**
      Sets a new value for element at the given index
    */
    inline void set(unsigned long ColIndex, unsigned long RowIndex, T Element)
    { setElement(ColIndex,RowIndex,Element); };

    /**
      Allocation operator
    */
    Matrix<T>& operator=(const Matrix &A);

    /**
      Fills the Matrix with given value
    */
    void fill(const T& Val);

    /**
      Clears the Matrix (empty and size is 0)
    */
    void clear();

};


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>::Matrix():
  m_ColsNbr(0),m_RowsNbr(0)
{
  init();
}


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>::Matrix(const Matrix &A)
{
  init();

  if (!allocate(A.m_ColsNbr,A.m_RowsNbr))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");

  std::copy(A.m_Data, A.m_Data + (A.m_ColsNbr*A.m_RowsNbr), m_Data);
}


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>::Matrix(unsigned long ColsNbr, unsigned long RowsNbr)
{
  init();

  if (!allocate(ColsNbr,RowsNbr))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");

  fill(0);
}


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>::Matrix(unsigned long ColsNbr, unsigned long RowsNbr, T InitValue)
{
  init();

  if (!allocate(ColsNbr,RowsNbr))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");

  fill(InitValue);
}


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>::~Matrix()
{
  clear();
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::init()
{
  m_Data = nullptr;
  m_ColsNbr = 0;
  m_RowsNbr = 0;
}


// =====================================================================
// =====================================================================


template <class T>
bool Matrix<T>::allocate(unsigned long ColsNbr, unsigned long RowsNbr)
{
  if (ColsNbr > 0 && RowsNbr > 0)
  {
    m_Data = new T[ColsNbr*RowsNbr];
    if (m_Data)
    {
      m_RowsNbr = RowsNbr;
      m_ColsNbr = ColsNbr;
    }
    else
      return false;
  }

  return true;
}

// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::setData(T* Data, unsigned long ColsNbr, unsigned long RowsNbr)
{
  clear();

  if (!allocate(ColsNbr,RowsNbr))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");

  std::copy(Data, Data + (ColsNbr*RowsNbr), m_Data);
}


// =====================================================================
// =====================================================================


template <class T>
T Matrix<T>::getElement(unsigned long ColIndex, unsigned long RowIndex) const
{
  if (ColIndex >= m_ColsNbr || RowIndex >= m_RowsNbr)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"element access range error");

  return m_Data[ColIndex*m_RowsNbr+RowIndex];
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::setElement(unsigned long ColIndex, unsigned long RowIndex, T Element)
{
  if (ColIndex >= m_ColsNbr || RowIndex >= m_RowsNbr)
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"element access range error");

  m_Data[ColIndex*m_RowsNbr+RowIndex] = Element;
}



// =====================================================================
// =====================================================================

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix &A)
{

  if (this == &A) // in case somebody tries assign array to itself
    return *this;

  clear();

  if (!allocate(A.m_ColsNbr,A.m_RowsNbr))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");

  std::copy(A.m_Data, A.m_Data + (A.m_ColsNbr*A.m_RowsNbr), m_Data);

  return *this;
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::fill(const T& Val)
{
  for (unsigned long i=0;i<m_RowsNbr;i++)
    for (unsigned long j=0;j<m_ColsNbr;j++)
      m_Data[i*m_ColsNbr+j] = Val;
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::clear()
{
  delete [] m_Data;
  init();
}



} }

#endif /* __OPENFLUID_CORE_MATRIX_HPP__ */
