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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_CORE_MATRIX_HPP__
#define __OPENFLUID_CORE_MATRIX_HPP__


#include <memory>

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

    std::unique_ptr<T[]> m_Data;

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
    Matrix(const Matrix& Other);

    /**
      Constructor, creates a Matrix containing Size elements
    */
    Matrix(unsigned long ColsNbr,unsigned long RowsNbr);

    /**
      Constructor, creates a Matrix containing Size elements, initialized with value InitValue
    */
    Matrix(unsigned long ColsNbr,unsigned long RowsNbr,const T& InitValue);

    /**
      Assignment operator
    */
    Matrix<T>& operator=(const Matrix& Other);

    /**
      Move assignment operator
    */
    Matrix<T>& operator=(Matrix&& Other);

    /**
      Destructor
    */
    virtual ~Matrix() = default;

    /**
      Returns the number of columns of the Matrix
      @return number of columns
    */
    inline unsigned long getColsNbr() const
    {
      return m_ColsNbr;
    }

    /**
      Returns the number of rows of the Matrix
      @return number of rows
    */
    inline unsigned long getRowsNbr() const
    {
      return m_RowsNbr;
    }

    /**
      Returns the full size of the Matrix (number of columns x number of rows)
      @return size of the Matrix
    */
    inline unsigned long getSize() const
    {
      return (m_ColsNbr * m_RowsNbr);
    }

    /**
      Returns the full size of the Matrix
    */
    inline unsigned long size() const
    {
      return (m_ColsNbr * m_RowsNbr);
    }


    /**
      Returns a pointer to the content of the Matrix (like C arrays)
    */
    T* data() const
    {
      return static_cast<T*>(m_Data.get());
    }

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
    {
      return getElement(ColIndex,RowIndex);
    }

    /**
      Returns the element of the Matrix for index Index
    */
    inline T get(unsigned long ColIndex, unsigned long RowIndex) const
    { return getElement(ColIndex,RowIndex); };


    /**
      Sets a new value for element at the given index
    */
    void setElement(unsigned long ColIndex, unsigned long RowIndex, const T& Val);

    /**
      Sets a new value for element at the given index
    */
    inline void set(unsigned long ColIndex, unsigned long RowIndex, const T& Val)
    {
      setElement(ColIndex,RowIndex,Val);
    }

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
Matrix<T>::Matrix(const Matrix& Other)
{
  init();

  if (!allocate(Other.m_ColsNbr,Other.m_RowsNbr))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");
  }

  std::copy(Other.m_Data.get(), Other.m_Data.get() + (Other.m_ColsNbr*Other.m_RowsNbr), m_Data.get());
}


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>::Matrix(unsigned long ColsNbr, unsigned long RowsNbr)
{
  init();

  if (!allocate(ColsNbr,RowsNbr))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");
  }
}


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>::Matrix(unsigned long ColsNbr, unsigned long RowsNbr, const T& InitValue)
{
  init();

  if (!allocate(ColsNbr,RowsNbr))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");
  }

  fill(InitValue);
}


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix& Other)
{

  if (this != &Other)
  {
    clear();

    if (!allocate(Other.m_ColsNbr,Other.m_RowsNbr))
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");
    }

    std::copy(Other.m_Data.get(), Other.m_Data.get() + (Other.m_ColsNbr*Other.m_RowsNbr), m_Data.get());
  }

  return *this;
}


// =====================================================================
// =====================================================================


template <class T>
Matrix<T>& Matrix<T>::operator=(Matrix&& Other)
{

  if (this != &Other)
  {
    clear();

    m_ColsNbr = Other.m_ColsNbr;
    m_RowsNbr = Other.m_RowsNbr;
    m_Data = std::move(Other.m_Data);

    Other.m_ColsNbr = 0;
    Other.m_RowsNbr = 0;
  }

  return *this;
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::init()
{
  m_Data.reset();
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
    m_Data.reset();
    m_Data = std::move(std::make_unique<T[]>(ColsNbr*RowsNbr));
    if (m_Data)
    {
      m_RowsNbr = RowsNbr;
      m_ColsNbr = ColsNbr;
    }
    else
    {
      return false;
    }
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
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Cannot allocate memory");
  }

  std::copy(Data, Data + (ColsNbr*RowsNbr), m_Data.get());
}


// =====================================================================
// =====================================================================


template <class T>
T Matrix<T>::getElement(unsigned long ColIndex, unsigned long RowIndex) const
{
  if (ColIndex >= m_ColsNbr || RowIndex >= m_RowsNbr)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"element access range error");
  }

  return m_Data[ColIndex*m_RowsNbr+RowIndex];
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::setElement(unsigned long ColIndex, unsigned long RowIndex, const T& Val)
{
  if (ColIndex >= m_ColsNbr || RowIndex >= m_RowsNbr)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"element access range error");
  }

  m_Data[ColIndex*m_RowsNbr+RowIndex] = Val;
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::fill(const T& Val)
{
  std::fill(m_Data.get(), m_Data.get() + (m_ColsNbr*m_RowsNbr),Val);
}


// =====================================================================
// =====================================================================


template <class T>
void Matrix<T>::clear()
{
  init();
}


} }

#endif /* __OPENFLUID_CORE_MATRIX_HPP__ */
