/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*!
  \file Tensor2.h
  \brief Declaration file for the second order tensor class

  This file is the declaration file for the second order tensor class. A second order tensor is a like a matrix with the following form:
  \f[ T=\left[\begin{array}{ccc}
  T_{11} & T_{12} & T_{13}\\
  T_{21} & T_{22} & T_{23}\\
  T_{31} & T_{32} & T_{33}
  \end{array}\right] \f]
  \ingroup dnlMaths
*/

#ifndef __dnlMaths_Tensor2_h__
#define __dnlMaths_Tensor2_h__

#include <string>
#include <string.h>
#include <Errors.h>
#include <Macros.h>

class Vec3D;
class SymTensor2;
class Matrix;

/*!
  \brief Declaration of the second order tensor class

  The second order tensor is a like a matrix with the following form:
  \f[ T=\left[\begin{array}{ccc}
  T_{11} & T_{12} & T_{13}\\
  T_{21} & T_{22} & T_{23}\\
  T_{31} & T_{32} & T_{33}
  \end{array}\right] \f]
  \ingroup dnlMaths
*/
class Tensor2
{
  static const Tensor2Index _internalIndexes;
  friend class Vec3D;
  friend class SymTensor2;
  friend class Matrix;

  double _data[9]; //!< Data storage for 9 double

private:
  bool indexOK(short, short) const;

public:
  Tensor2();
  Tensor2(double, double, double, double, double, double, double, double, double);
  Tensor2(const Tensor2 &);
  ~Tensor2();

  bool operator!=(const Tensor2 &) const;
  bool operator==(const Tensor2 &) const;
  double operator()(short, short) const;
  Tensor2 operator-() const;
  Tensor2 operator-(const Tensor2 &) const;
  Tensor2 operator*(const double)const;
  Tensor2 operator*(const Tensor2 &)const;
  Tensor2 operator*(const SymTensor2 &)const;
  Tensor2 operator/(const double) const;
  Tensor2 operator+(const Tensor2 &) const;
  Vec3D operator*(const Vec3D &)const;

#ifndef SWIG
  double &operator()(short, short);
  friend Tensor2 operator*(const double &, const Tensor2 &);
  Tensor2 &operator=(const double &);
  Tensor2 &operator=(const Tensor2 &);
  Tensor2 &operator=(const SymTensor2 &);
  void operator-=(const Tensor2 &);
  void operator*=(const double);
  void operator/=(const double);
  void operator+=(const Tensor2 &);
#endif

  double doubleProduct(const Tensor2) const;
  double doubleProduct() const;
  double getDeterminant() const;
  double getJ2() const;
  double getMisesEquivalent() const;
  double getNorm() const;
  double getThirdTrace() const;
  double getTrace() const;
  double maxAbsoluteValue();
  double maxValue();
  double minAbsoluteValue();
  double minValue();
  Tensor2 getCofactor() const;
  Tensor2 getDeviator() const;
  Tensor2 getInverse() const;
  Tensor2 getMinor() const;
  Tensor2 getSkewSymetricPart() const;
  Tensor2 getSymetricPart() const;
  Tensor2 getTranspose() const;
  Tensor2 singleProduct() const;
  SymTensor2 singleProductTxN() const;
  SymTensor2 singleProductNxT() const;
  Tensor2 singleProduct(const Tensor2) const;
  Vec3D columnSum() const;
  Vec3D getColumn(short) const;
  Vec3D getRow(short) const;
  Vec3D rowSum() const;
  Vec3D solve(const Vec3D &) const;
  void numpyRead(std::string);
  void numpyReadZ(std::string, std::string);
  void numpyWrite(std::string, bool = false) const;
  void numpyWriteZ(std::string, std::string, bool = false) const;
  void polarDecompose(SymTensor2 &, Tensor2 &) const;
  void polarDecomposeLnU(SymTensor2 &, Tensor2 &) const;
  void setToUnity();
  void setToValue(const double);
  void setToZero();

  // gestion des flux entree et sortie
#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, Tensor2 &);
  friend std::ofstream &operator<<(std::ofstream &, const Tensor2 &);
  friend std::ostream &operator<<(std::ostream &, const Tensor2 &);
  void print(std::ostream &) const;
  void read(std::ifstream &);
  void write(std::ofstream &) const;
#endif
};

//------inline functions-------------------------------------------------------

/*!
  \brief tests if the couple of indexes is ok
*/
//-----------------------------------------------------------------------------
inline bool Tensor2::indexOK(short i, short j) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < 3) && (j >= 0) && (j < 3))
  {
    return (true);
  }

  printf("Tensor2::indexOK\nindice(s) [%d,%d] out of allowed range [0-2,0-2]", i, j);
  exit(-1);

  // no sense
  return (false);
}

/*!
  \brief Access to the values T[i,j] of a second order tensor

  \param i short for row
  \param j short for column
  \return Value of the second order tensor T[i,j]
*/
//-----------------------------------------------------------------------------
inline double &Tensor2::operator()(short i, short j)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j);
#endif
  return _data[_internalIndexes.index[i][j]];
}

/*!
  \brief Access to the values T[i,j] of a second order tensor (Read only method)

  \param i short for row
  \param j short for column
  \return Value of the second order tensor T[i,j]
*/
//-----------------------------------------------------------------------------
inline double Tensor2::operator()(short i, short j) const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j);
#endif
  return _data[_internalIndexes.index[i][j]];
}

/*!
  \brief Fill a second order tensor with a scalar value

  This method is a surdefinition of the = operator for the second order tensor class.
  \code
  Tensor2 t1;
  t1 = setToValue(1.0); // All components of the tensor are set to 1.0
  \endcode
  \param val double value to give to all components of the second order tensor
*/
//-----------------------------------------------------------------------------
inline void Tensor2::setToValue(const double val)
//-----------------------------------------------------------------------------
{
  _data[0] = val;
  _data[1] = val;
  _data[2] = val;
  _data[3] = val;
  _data[4] = val;
  _data[5] = val;
  _data[6] = val;
  _data[7] = val;
  _data[8] = val;
}

/*!
  \brief Addition of 2 second order tensors

  This method defines the addition of 2 second order tensors.
  \code
  Tensor2 t1,t2;
  t2 += t1; // sum of two second order tensors
  \endcode
  \param tensor Second order tensor to add
*/
//-----------------------------------------------------------------------------
inline void Tensor2::operator+=(const Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  // calcul de la somme
  _data[0] += tensor._data[0];
  _data[1] += tensor._data[1];
  _data[2] += tensor._data[2];
  _data[3] += tensor._data[3];
  _data[4] += tensor._data[4];
  _data[5] += tensor._data[5];
  _data[6] += tensor._data[6];
  _data[7] += tensor._data[7];
  _data[8] += tensor._data[8];
}

/*!
  \brief Difference of 2 second order tensors

  This method defines the difference of 2 second order tensors.
  \code
  Tensor2 t1,t2;
  t2 -= t1; // difference of two second order tensors
  \endcode
  \param tensor Second order tensor to substract
*/
//-----------------------------------------------------------------------------
inline void Tensor2::operator-=(const Tensor2 &tensor)
//-----------------------------------------------------------------------------
{
  // calcul de la difference
  _data[0] -= tensor._data[0];
  _data[1] -= tensor._data[1];
  _data[2] -= tensor._data[2];
  _data[3] -= tensor._data[3];
  _data[4] -= tensor._data[4];
  _data[5] -= tensor._data[5];
  _data[6] -= tensor._data[6];
  _data[7] -= tensor._data[7];
  _data[8] -= tensor._data[8];
}

/*!
  \brief Multiplication of a second order tensor by a scalar value

  This method defines the multiplication of a second order tensor by a scalar value  
  \code
  Tensor2 t1;
  double l;
  t1 *= l; // multiplication by a scalar
  \endcode
  \param val Scalar value to use for the multiplication
*/
//-----------------------------------------------------------------------------
inline void Tensor2::operator*=(const double val)
//-----------------------------------------------------------------------------
{
  _data[0] *= val;
  _data[1] *= val;
  _data[2] *= val;
  _data[3] *= val;
  _data[4] *= val;
  _data[5] *= val;
  _data[6] *= val;
  _data[7] *= val;
  _data[8] *= val;
}

/*!
  \brief Division of a second order tensor by a scalar value

  This method defines the division of a second order tensor by a scalar value  
  \code
  Tensor2 t1;
  double l;
  t1 /= l; // division by a scalar
  \endcode
  \param val Scalar value to use for the division
*/
//-----------------------------------------------------------------------------
inline void Tensor2::operator/=(const double val)
//-----------------------------------------------------------------------------
{
  _data[0] /= val;
  _data[1] /= val;
  _data[2] /= val;
  _data[3] /= val;
  _data[4] /= val;
  _data[5] /= val;
  _data[6] /= val;
  _data[7] /= val;
  _data[8] /= val;
}

/*!
  \brief Returns the trace of a second order tensor

  This method returns the trace of a second order tensor, i.e. the sum of all the terms of the diagonal:
  \f[ _data = tr[T] = T_{11}+T_{22}+T_{33} \f]
  \return getTrace of a second order tensor
*/
//-----------------------------------------------------------------------------
inline double Tensor2::getTrace() const
//-----------------------------------------------------------------------------
{
  return (_data[0] + _data[4] + _data[8]);
}

/*!
  \brief Returns the average value of the trace of a second order tensor

  This method returns average value of the trace of a second order tensor, i.e. the sum of all the terms of the diagonal divided by 3:
    \f[ _data = \frac{1}{3} tr[T] =  \frac{1}{3} \left( T_{11}+T_{22}+T_{33} \right) \f]
  \return getTrace of a second order tensor divided by 3
*/
//-----------------------------------------------------------------------------
inline double Tensor2::getThirdTrace() const
//-----------------------------------------------------------------------------
{
  return ((_data[0] + _data[4] + _data[8]) / 3.0);
}

/*!
  \brief Returns the von-Mises stress of a second order tensor

  This method returns the von-Mises stress of a second order tensor defined by:
  \f[ \overline{\sigma} = \frac {1}{\sqrt{2}}\sqrt{(s_{11}-s_{22})^2+(s_{22}-s_{33})^2+(s_{33}-s_{11})^2+6(s_{12}^2+s_{23}^2+s_{31}^2)}\f]
  \return von-Mises stress of a second order tensor
*/
//-----------------------------------------------------------------------------
inline double Tensor2::getMisesEquivalent() const
//-----------------------------------------------------------------------------
{
  exit(-1); // No sense, since this is not a symmetric tensor2
  return (dnlVonMises(_data[0], _data[4], _data[8], _data[1], _data[2], _data[5]));
}

/*!
  \brief Norm of a second order tensor

  This method returns the norm of a second order tensor defined by:
  \f[ \left\Vert s \right\Vert  = \sqrt {s_{ij}:s_{ij}} \f]
  \code
  Tensor2 tensor1;
  double s;
  s = tensor1.getNorm(); // norm du tenseur
  \endcode
  \return norm of a second order tensor
*/
//-----------------------------------------------------------------------------
inline double Tensor2::getNorm() const
//-----------------------------------------------------------------------------
{
  return sqrt(dnlSquare(_data[0]) + dnlSquare(_data[1]) + dnlSquare(_data[2]) +
              dnlSquare(_data[3]) + dnlSquare(_data[4]) + dnlSquare(_data[5]) +
              dnlSquare(_data[6]) + dnlSquare(_data[7]) + dnlSquare(_data[8]));
}

/*!
  \brief J2 norm of a second order tensor

  This method returns the J2 norm of a second order tensor defined by:
  \f[ n = \sqrt {\frac{3}{2}} \left\Vert s \right\Vert  = \sqrt {\frac{3}{2} s_{ij}:s_{ij}} \f]
  \code
  Tensor2 tensor1;
  double s;
  s = tensor1.getJ2(); // J2 norm of the tensor
  \endcode
*/
//-----------------------------------------------------------------------------
inline double Tensor2::getJ2() const
//-----------------------------------------------------------------------------
{
  return dnlSqrt32 * sqrt(dnlSquare(_data[0]) + dnlSquare(_data[1]) + dnlSquare(_data[2]) +
                          dnlSquare(_data[3]) + dnlSquare(_data[4]) + dnlSquare(_data[5]) +
                          dnlSquare(_data[6]) + dnlSquare(_data[7]) + dnlSquare(_data[8]));
}

/*!
  \brief Determinant of a second order tensor

  This method returns the determinant of a second second order tensor.
  The result of this operation is a scalar value defined by:
  \f[ D = T_{11} T_{22} T_{33} + T_{21} T_{32} T_{13} + T_{31} T_{12} T_{23} - T_{31} T_{22} T_{13} - T_{11} T_{32} T_{23} - T_{21} T_{12} T_{33} \f]
  \return Scalar value containing the determinant of a second order tensor
*/
//-----------------------------------------------------------------------------
inline double Tensor2::getDeterminant() const
//-----------------------------------------------------------------------------
{
  return (_data[0] * _data[4] * _data[8] + _data[3] * _data[7] * _data[2] + _data[6] * _data[1] * _data[5] - _data[6] * _data[4] * _data[2] - _data[0] * _data[7] * _data[5] - _data[3] * _data[1] * _data[8]);
}

#endif
