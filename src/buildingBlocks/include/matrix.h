#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <concepts>
#include <cstdint>

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

  Matrix<rows, cols>& operator*=(const Matrix<rows, cols>& rhs);
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
  return this->elements==rhs.elements;
}

template <uint8_t rows, uint8_t cols> 
  Matrix<rows, cols>& Matrix<rows, cols>::operator*=(const Matrix<rows, cols>& rhs){
    Matrix<rows,cols>results{};

    for (std::size_t rowIndex{0}; rowIndex < rows; rowIndex++){
      for (std::size_t colIndex{0}; colIndex < cols; colIndex++){
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
std::ostream& operator<<(std::ostream& os, const Matrix<rows, cols>& rhs) {
  // Test screen output to see if correct   
  std::cout << std::setw(14) << "The Matrix:" << '\n';
  for( int a = 0; a < rows; ++a) {
      for( int b = 0; b < cols; ++b)
      {
          std::cout << ' ' << rhs.at(a,b) << ' ';
      }
      std::cout << '\n';
  }

  return os;
}

#endif //MATRIX_H