#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <concepts>
#include <cstdint>
#include <algorithm>
#include <iostream>

#include "include/tuple.h"
#include "include/maths.h"

template <uint8_t rows, uint8_t cols> 
class Matrix
{
private:
  std::array<double, rows*cols> elements;
public:
  template<typename... T>
  explicit Matrix(T... data): elements{data...}{};
  Matrix(const Matrix<rows, cols>& other): elements{other.elements}{};

  bool operator==(const Matrix& rhs) const;

  const double& at(const std::size_t& row, const std::size_t& column) const;
  double& at(const std::size_t& row, const std::size_t& column);

  static Matrix<rows, cols> identity();

  Matrix<rows, cols>& operator*=(const Matrix<rows, cols>& rhs);
  Matrix<rows, cols> transpose() const;
  double determinant() const;
  bool invertible() const;
};

template <uint8_t rows, uint8_t cols>
const double& Matrix<rows, cols>::at(const std::size_t& row, const std::size_t& column) const{
  return elements.at(row*cols+column);
}

template <uint8_t rows, uint8_t cols> 
double& Matrix<rows, cols>::at(const std::size_t& row, const std::size_t& column){
  return const_cast<double&>(const_cast<const Matrix*>(this)->at(row, column));
}

template <uint8_t rows, uint8_t cols> 
bool Matrix<rows, cols>::operator==(const Matrix& rhs) const{
  return std::equal(this->elements.cbegin(), this->elements.cend(),
                    rhs.elements.cbegin()  , rhs.elements.cend(), 
                    floatEqual<double>);
}

template <uint8_t rows, uint8_t cols> 
  Matrix<rows, cols>& Matrix<rows, cols>::operator*=(const Matrix<rows, cols>& rhs){
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
inline Matrix<rows, cols> operator*(Matrix<rows, cols> lhs, const Matrix<rows, cols>& rhs)
{
  lhs *= rhs;
  return lhs;
}

template <uint8_t rows, uint8_t cols>
typename std::enable_if<rows==4, Tuple>::type
operator*(const Matrix<rows, cols>& lhs, const Tuple& rhs){
  Tuple result{
    Tuple{lhs.at(0, 0), lhs.at(0, 1), lhs.at(0, 2), lhs.at(0, 3)}.dot(rhs),
    Tuple{lhs.at(1, 0), lhs.at(1, 1), lhs.at(1, 2), lhs.at(1, 3)}.dot(rhs),
    Tuple{lhs.at(2, 0), lhs.at(2, 1), lhs.at(2, 2), lhs.at(2, 3)}.dot(rhs),
    Tuple{lhs.at(3, 0), lhs.at(3, 1), lhs.at(3, 2), lhs.at(3, 3)}.dot(rhs)
  };

  return result;
}

template <uint8_t rows, uint8_t cols> 
std::ostream& operator<<(std::ostream& os, const Matrix<rows, cols>& rhs) {
  for(std::size_t rowIndex{0}; rowIndex < rows; rowIndex++){
    for(std::size_t colIndex{0}; colIndex < cols; colIndex++){
      std::cout << ' ' << rhs.at(rowIndex, colIndex) << ' ';
    }
    std::cout << '\n';
  }

  return os;
}

template<>
inline Matrix<4,4> Matrix<4,4>::identity(){
  return Matrix<4, 4>{
        1.f, 0.f, 0.f, 0.f ,
        0.f, 1.f, 0.f, 0.f ,
        0.f, 0.f, 1.f, 0.f ,
        0.f, 0.f, 0.f, 1.f};
}

template <uint8_t rows, uint8_t cols>
Matrix<rows, cols> Matrix<rows, cols>::transpose() const{
  Matrix<rows, cols> result{};

  for(std::size_t rowIndex{0}; rowIndex < rows; rowIndex++){
    for(std::size_t colIndex{0}; colIndex < cols; colIndex++){
      result.at(colIndex, rowIndex) = this->at(rowIndex, colIndex);
    }
  }

  return result;
}

template<>
inline double Matrix<1,1>::determinant() const{
  auto determinant = this->at(0,0);

  return determinant;
}

template<>
inline double Matrix<2,2>::determinant() const{
  auto determinant = this->at(0,0)*this->at(1,1) - this->at(0,1)*this->at(1,0);

  return determinant;
}

template <uint8_t rows, uint8_t cols>
double Matrix<rows,cols>::determinant() const{
  double determinant{};

  for(std::size_t colIndex{0}; colIndex < cols; colIndex++){
    determinant += this->at(0, colIndex)*cofactor(*this, 0, colIndex);
  }

  return determinant;
}

template <uint8_t rows, uint8_t cols>
typename std::enable_if<(rows>2 && cols>2), Matrix<rows-1, cols-1>>::type
submatrix(const Matrix<rows, cols>& matrix, const std::size_t& skipRow, const std::size_t& skipColumn) {
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
double minor(const Matrix<rows, cols>& matrix, const std::size_t& row, const std::size_t& column){
    auto subMatrix =  submatrix(matrix, row, column);
    return subMatrix.determinant();  
}

template<uint8_t rows, uint8_t cols>
inline double cofactor(const Matrix<rows, cols>& matrix, const std::size_t& row, const std::size_t& column){
  double cofactor = (row+column)%2 ? -1*minor(matrix, row, column) : minor(matrix, row, column);

  return cofactor;
}

template<uint8_t rows, uint8_t cols>
bool Matrix<rows, cols>::invertible() const{
  if(this->determinant() != 0){return true;}
  else{return false;}
}

template<uint8_t rows, uint8_t cols>
typename std::enable_if<(rows>=2 && cols>=2), Matrix<rows, cols>>::type
inverse(const Matrix<rows, cols>& matrix){
  Matrix<rows, cols> result{};
  auto determinant = matrix.determinant();

  for(std::size_t rowIndex{0}; rowIndex < rows; rowIndex++){
    for(std::size_t colIndex{0}; colIndex < cols; colIndex++){
      auto cofact = cofactor(matrix, rowIndex, colIndex);
      result.at(colIndex, rowIndex) = cofact/determinant;
    }
  }

  return result;
}
#endif //MATRIX_H