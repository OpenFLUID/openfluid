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
  \file Matrix.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MATRIX_HPP___
#define __MATRIX_HPP___



#include <iostream>
#include <openfluid/dllexport.hpp>
#include <openfluid/base/OFException.hpp>
#include <boost/multi_array.hpp>

namespace openfluid { namespace core {


/**
  Template class for vector data
*/
template <class T>
class DLLEXPORT Matrix
{
  protected :

    boost::multi_array<T, 2> m_Data;

    unsigned long m_Size1;

    unsigned long m_Size2;


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
    Matrix(unsigned long Size1,unsigned long Size2);

  /**
    Constructor, creates a Matrix containing Size elements, initialized with value InitValue
  */
    Matrix(unsigned long Size1,unsigned long Size2, T InitValue);

    /**
      Destructor
    */
    virtual ~Matrix() {};

    /**
      Returns the size of the 1st dimension of the Matrix
    */
    unsigned long getSize1() const { return m_Size1; };

    /**
      Returns the size of the 2nd dimension of the Matrix
    */
    unsigned long getSize2() const { return m_Size2; };

    /**
      Returns the full size of the Matrix
    */
    unsigned long getSize() const { return (m_Size1 * m_Size2); };

    /**
      Returns the full size of the Matrix
    */
    unsigned long size() const { return (m_Size1 * m_Size2); };


    /**
      Returns a pointer to the content of the Matrix (like C arrays)
    */
    T* getData() const { return m_Data.data(); };

    /**
      Returns the element of the Matrix for index Index
    */
    T getElement(unsigned long Index1, unsigned long Index2) const;

    /**
      Returns the element of the Matrix for index Index
    */
    inline T at(unsigned long Index1, unsigned long Index2) const { return getElement(Index1,Index2); };

    /**
      Returns the element of the Matrix for index Index
    */
    inline T get(unsigned long Index1, unsigned long Index2) const { return getElement(Index1,Index2); };


    /**
      Sets a new value for element at the given index
    */
    void setElement(unsigned long Index1, unsigned long Index2, T Element);

    /**
      Sets a new value for element at the given index
    */
    inline void set(unsigned long Index1, unsigned long Index2, T Element) { setElement(Index1,Index2,Element); };

    /**
      Allocation operator
    */
    Matrix<T>& operator = (const Matrix &A);

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
  m_Data(boost::extents[0][0],boost::fortran_storage_order()),
  m_Size1(0),m_Size2(0)
{
}


// =====================================================================
// =====================================================================

template <class T>
Matrix<T>::Matrix(const Matrix &A):
  m_Data(boost::extents[A.m_Size1][A.m_Size2],boost::fortran_storage_order()),
  m_Size1(A.m_Size1),m_Size2(A.m_Size2)
{
  m_Data = A.m_Data;
}


// =====================================================================
// =====================================================================

template <class T>
Matrix<T>::Matrix(unsigned long Size1, unsigned long Size2) :
  m_Data(boost::extents[Size1][Size2],boost::fortran_storage_order()),
  m_Size1(Size1),m_Size2(Size2)
{
  fill(0);
}


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>::Matrix(unsigned long Size1, unsigned long Size2, T InitValue) :
  m_Data(boost::extents[Size1][Size2],boost::fortran_storage_order()),
  m_Size1(Size1),m_Size2(Size2)
{
  fill(InitValue);
}


// =====================================================================
// =====================================================================


template <class T>
T Matrix<T>::getElement(unsigned long Index1, unsigned long Index2) const
{
  if (Index1 >= m_Size1 || Index2 >= m_Size2) throw openfluid::base::OFException("OpenFLUID framework","Matrix::getElement","element access range error");
  return m_Data[Index1][Index2];
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::setElement(unsigned long Index1, unsigned long Index2, T Element)
{
  if (Index1 >= m_Size1 || Index2 >= m_Size2) throw openfluid::base::OFException("OpenFLUID framework","Matrix::setElement","element access range error");
  m_Data[Index1][Index2] = Element;
}



// =====================================================================
// =====================================================================

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix &A)
{

  if (this == &A) return *this; // in case somebody tries assign array to itself

  m_Data.resize(boost::extents[A.m_Size1][A.m_Size2]);
  m_Data = A.m_Data;
  m_Size1 = A.m_Size1;
  m_Size2 = A.m_Size2;

  return *this;
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::fill(const T& Val)
{
  for (unsigned long i=0;i<m_Size1;i++)
    for (unsigned long j=0;j<m_Size2;j++)
      m_Data[i][j] = Val;
}


// =====================================================================
// =====================================================================

template <class T>
void Matrix<T>::clear()
{
  m_Data.resize(boost::extents[0][0]);
  m_Size1=0;
  m_Size2=0;
}



} }

#endif /* __MATRIX_HPP___ */
