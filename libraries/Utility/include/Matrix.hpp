#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <concepts>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <execution>

#include "Tuple.hpp"
#include "FloatUtils.hpp"

namespace raytracer {
namespace utility {

template <uint8_t rows, uint8_t cols> 
class Matrix
{
private:
  std::array<double, rows*cols> elements;
public:
  template<typename... T>
  explicit Matrix(T... data) noexcept: elements{static_cast<double>(data)...}{};
  explicit Matrix(const std::array<double, rows*cols>& data) noexcept : elements(data) {}

  bool operator==(const Matrix& rhs) const noexcept;

  const double& at(const std::size_t& row, const std::size_t& column) const noexcept;
  double& at(const std::size_t& row, const std::size_t& column) noexcept;

  static Matrix<rows, cols> identity() noexcept;

  Matrix<rows, cols>& operator*=(const Matrix<rows, cols>& rhs) noexcept;
  Matrix<rows, cols> transpose() const noexcept;
  double determinant() const noexcept;
  bool invertible() const noexcept;
};

template <uint8_t rows, uint8_t cols>
inline const double& Matrix<rows, cols>::at(const std::size_t& row, const std::size_t& column) const noexcept{
  return elements[row*cols+column];
}

template <uint8_t rows, uint8_t cols> 
inline double& Matrix<rows, cols>::at(const std::size_t& row, const std::size_t& column) noexcept{
  return const_cast<double&>(const_cast<const Matrix*>(this)->at(row, column));
}

template <uint8_t rows, uint8_t cols> 
bool Matrix<rows, cols>::operator==(const Matrix& rhs) const noexcept{
  return std::equal(std::execution::unseq, this->elements.cbegin(), this->elements.cend(),
                                           rhs.elements.cbegin()  , rhs.elements.cend(), 
                    floatNearlyEqual<double>);
}

template <uint8_t rows, uint8_t cols> 
  Matrix<rows, cols>& Matrix<rows, cols>::operator*=(const Matrix<rows, cols>& rhs) noexcept{
    Matrix<rows,cols>results{};

    for(std::size_t rowIndex{0}; rowIndex < rows; rowIndex++){
      for(std::size_t colIndex{0}; colIndex < cols; colIndex++){
        for(std::size_t accumulateIndex{0}; accumulateIndex < cols; accumulateIndex++){
          results.at(rowIndex, colIndex) += this->at(rowIndex, accumulateIndex) * rhs.at(accumulateIndex, colIndex);
        }
      }
    }
    this->elements.swap(results.elements);
    return *this;
  }

template <uint8_t rows, uint8_t cols> 
inline Matrix<rows, cols> operator*(Matrix<rows, cols> lhs, const Matrix<rows, cols>& rhs) noexcept
{
  lhs *= rhs;
  return lhs;
}

template <uint8_t rows, uint8_t cols>
typename std::enable_if<rows==4, Tuple>::type
operator*(const Matrix<rows, cols>& lhs, const Tuple& rhs) noexcept{
  Tuple result{
    Tuple{lhs.at(0, 0), lhs.at(0, 1), lhs.at(0, 2), lhs.at(0, 3)}.dot(rhs),
    Tuple{lhs.at(1, 0), lhs.at(1, 1), lhs.at(1, 2), lhs.at(1, 3)}.dot(rhs),
    Tuple{lhs.at(2, 0), lhs.at(2, 1), lhs.at(2, 2), lhs.at(2, 3)}.dot(rhs),
    Tuple{lhs.at(3, 0), lhs.at(3, 1), lhs.at(3, 2), lhs.at(3, 3)}.dot(rhs)
  };

  return result;
}

template <uint8_t rows, uint8_t cols> 
std::ostream& operator<<(std::ostream& os, const Matrix<rows, cols>& rhs) noexcept {
  for(std::size_t rowIndex{0}; rowIndex < rows; rowIndex++){
    for(std::size_t colIndex{0}; colIndex < cols; colIndex++){
      std::cout << ' ' << rhs.at(rowIndex, colIndex) << ' ';
    }
    std::cout << '\n';
  }

  return os;
}

template<>
inline Matrix<4,4> Matrix<4,4>::identity() noexcept{
  return Matrix<4, 4>{
        1., 0., 0., 0. ,
        0., 1., 0., 0. ,
        0., 0., 1., 0. ,
        0., 0., 0., 1.};
}

template <uint8_t rows, uint8_t cols>
Matrix<rows, cols> Matrix<rows, cols>::transpose() const noexcept{
  Matrix<rows, cols> result{};

  for(std::size_t rowIndex{0}; rowIndex < rows; rowIndex++){
    for(std::size_t colIndex{0}; colIndex < cols; colIndex++){
      result.at(colIndex, rowIndex) = this->at(rowIndex, colIndex);
    }
  }

  return result;
}

template<>
inline double Matrix<1,1>::determinant() const noexcept{
  auto determinant = this->at(0,0);

  return determinant;
}

template<>
inline double Matrix<2,2>::determinant() const noexcept{
  auto determinant = this->at(0,0)*this->at(1,1) - this->at(0,1)*this->at(1,0);

  return determinant;
}

template<>
inline double Matrix<3,3>::determinant() const noexcept{
  return
    + this->at(0,0) * (this->at(1,1) * this->at(2,2) - this->at(2,1) * this->at(1,2))
    - this->at(1,0) * (this->at(0,1) * this->at(2,2) - this->at(2,1) * this->at(0,2))
    + this->at(2,0) * (this->at(0,1) * this->at(1,2) - this->at(1,1) * this->at(0,2));
}

template<>
inline double Matrix<4,4>::determinant() const noexcept{
  const double SubFactor00 = this->at(2,2) * this->at(3,3) - this->at(3,2) * this->at(2,3);
  const double SubFactor01 = this->at(2,1) * this->at(3,3) - this->at(3,1) * this->at(2,3);
  const double SubFactor02 = this->at(2,1) * this->at(3,2) - this->at(3,1) * this->at(2,2);
  const double SubFactor03 = this->at(2,0) * this->at(3,3) - this->at(3,0) * this->at(2,3);
  const double SubFactor04 = this->at(2,0) * this->at(3,2) - this->at(3,0) * this->at(2,2);
  const double SubFactor05 = this->at(2,0) * this->at(3,1) - this->at(3,0) * this->at(2,1);

  const Tuple determinantCoefficients{
    + (this->at(1,1) * SubFactor00 - this->at(1,2) * SubFactor01 + this->at(1,3) * SubFactor02),
    - (this->at(1,0) * SubFactor00 - this->at(1,2) * SubFactor03 + this->at(1,3) * SubFactor04),
    + (this->at(1,0) * SubFactor01 - this->at(1,1) * SubFactor03 + this->at(1,3) * SubFactor05),
    - (this->at(1,0) * SubFactor02 - this->at(1,1) * SubFactor04 + this->at(1,2) * SubFactor05)
  };

  return
    this->at(0,0) * determinantCoefficients.x() + this->at(0,1) * determinantCoefficients.y() +
    this->at(0,2) * determinantCoefficients.z() + this->at(0,3) * determinantCoefficients.w();
}

template <uint8_t rows, uint8_t cols>
double Matrix<rows,cols>::determinant() const noexcept{
  double determinant{};

  for(std::size_t colIndex{0}; colIndex < cols; colIndex++){
    determinant += this->at(0, colIndex)*cofactor(*this, 0, colIndex);
  }

  return determinant;
}

template <uint8_t rows, uint8_t cols>
typename std::enable_if<(rows>2 && cols>2), Matrix<rows-1, cols-1>>::type
inline submatrix(const Matrix<rows, cols>& matrix, const std::size_t& skipRow, const std::size_t& skipColumn) noexcept {
  Matrix<rows-1, cols-1> result{};

  std::size_t resultRowIndex{};
  std::size_t resultColIndex{};
  for(std::size_t rowIndex{0}; rowIndex < rows; rowIndex++){
    if(rowIndex==skipRow){continue;}
    resultColIndex = 0;
    for(std::size_t colIndex{0}; colIndex < cols; colIndex++){
      if(colIndex==skipColumn){continue;}
      result.at(resultRowIndex, resultColIndex++) = matrix.at(rowIndex, colIndex);
    }
    resultRowIndex++;
  }

  return result;
}

template<uint8_t rows, uint8_t cols>
double minor(const Matrix<rows, cols>& matrix, const std::size_t& row, const std::size_t& column) noexcept{
    auto subMatrix =  submatrix(matrix, row, column);
    return subMatrix.determinant();  
}

template<uint8_t rows, uint8_t cols>
inline double cofactor(const Matrix<rows, cols>& matrix, const std::size_t& row, const std::size_t& column) noexcept{
  double cofactor = (row+column)%2 ? -1*minor(matrix, row, column) : minor(matrix, row, column);

  return cofactor;
}

template<uint8_t rows, uint8_t cols>
bool Matrix<rows, cols>::invertible() const noexcept{
  if(this->determinant() != 0){return true;}
  else{return false;}
}

template<uint8_t rows, uint8_t cols>
typename std::enable_if<(rows==4 && cols==4), Matrix<rows, cols>>::type
inline inverse(const Matrix<rows,cols>& matrix) noexcept{
  const double Coef00 = matrix.at(2,2) * matrix.at(3,3) - matrix.at(3,2) * matrix.at(2,3);
  const double Coef02 = matrix.at(1,2) * matrix.at(3,3) - matrix.at(3,2) * matrix.at(1,3);
  const double Coef03 = matrix.at(1,2) * matrix.at(2,3) - matrix.at(2,2) * matrix.at(1,3);

  const double Coef04 = matrix.at(2,1) * matrix.at(3,3) - matrix.at(3,1) * matrix.at(2,3);
  const double Coef06 = matrix.at(1,1) * matrix.at(3,3) - matrix.at(3,1) * matrix.at(1,3);
  const double Coef07 = matrix.at(1,1) * matrix.at(2,3) - matrix.at(2,1) * matrix.at(1,3);

  const double Coef08 = matrix.at(2,1) * matrix.at(3,2) - matrix.at(3,1) * matrix.at(2,2);
  const double Coef10 = matrix.at(1,1) * matrix.at(3,2) - matrix.at(3,1) * matrix.at(1,2);
  const double Coef11 = matrix.at(1,1) * matrix.at(2,2) - matrix.at(2,1) * matrix.at(1,2);

  const double Coef12 = matrix.at(2,0) * matrix.at(3,3) - matrix.at(3,0) * matrix.at(2,3);
  const double Coef14 = matrix.at(1,0) * matrix.at(3,3) - matrix.at(3,0) * matrix.at(1,3);
  const double Coef15 = matrix.at(1,0) * matrix.at(2,3) - matrix.at(2,0) * matrix.at(1,3);

  const double Coef16 = matrix.at(2,0) * matrix.at(3,2) - matrix.at(3,0) * matrix.at(2,2);
  const double Coef18 = matrix.at(1,0) * matrix.at(3,2) - matrix.at(3,0) * matrix.at(1,2);
  const double Coef19 = matrix.at(1,0) * matrix.at(2,2) - matrix.at(2,0) * matrix.at(1,2);

  const double Coef20 = matrix.at(2,0) * matrix.at(3,1) - matrix.at(3,0) * matrix.at(2,1);
  const double Coef22 = matrix.at(1,0) * matrix.at(3,1) - matrix.at(3,0) * matrix.at(1,1);
  const double Coef23 = matrix.at(1,0) * matrix.at(2,1) - matrix.at(2,0) * matrix.at(1,1);

  const Tuple Fac0{Coef00, Coef00, Coef02, Coef03};
  const Tuple Fac1{Coef04, Coef04, Coef06, Coef07};
  const Tuple Fac2{Coef08, Coef08, Coef10, Coef11};
  const Tuple Fac3{Coef12, Coef12, Coef14, Coef15};
  const Tuple Fac4{Coef16, Coef16, Coef18, Coef19};
  const Tuple Fac5{Coef20, Coef20, Coef22, Coef23};

  const Tuple Vec0{matrix.at(1,0), matrix.at(0,0), matrix.at(0,0), matrix.at(0,0)};
  const Tuple Vec1{matrix.at(1,1), matrix.at(0,1), matrix.at(0,1), matrix.at(0,1)};
  const Tuple Vec2{matrix.at(1,2), matrix.at(0,2), matrix.at(0,2), matrix.at(0,2)};
  const Tuple Vec3{matrix.at(1,3), matrix.at(0,3), matrix.at(0,3), matrix.at(0,3)};

  const Tuple SignA{+1, -1, +1, -1};
  const Tuple SignB{-1, +1, -1, +1};

  const double oneOverDeterminant = static_cast<double>(1)/matrix.determinant();

  const Tuple Inv0{(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2) * oneOverDeterminant * SignA};
  const Tuple Inv1{(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4) * oneOverDeterminant * SignB};
  const Tuple Inv2{(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5) * oneOverDeterminant * SignA};
  const Tuple Inv3{(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5) * oneOverDeterminant * SignB};

  const Matrix<4,4> inverse{Inv0.x(), Inv0.y(), Inv0.z(), Inv0.w(),
                            Inv1.x(), Inv1.y(), Inv1.z(), Inv1.w(),
                            Inv2.x(), Inv2.y(), Inv2.z(), Inv2.w(),
                            Inv3.x(), Inv3.y(), Inv3.z(), Inv3.w()}; 

  return inverse;
}

} // namespace utility
} // namespace raytracer

#endif //MATRIX_H