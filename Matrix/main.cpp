#include <sstream>
#include <array>
#include <utility>

#ifndef MATRIX_H
#define MATRIX_H
#include <stdexcept>
#include <iostream>

class MatrixOutOfRange : public std::out_of_range {
 public:
  MatrixOutOfRange() : std::out_of_range("MatrixOutOfRange") {
  }
};

template <class T, size_t L, size_t R>
class Matrix {
 public:
  T matrix[L][R];
  size_t ColumnsNumber() const {
    return R;
  }
  size_t RowsNumber() const {
    return L;
  }
  const T& operator()(size_t line, size_t row) const {
    return matrix[line][row];
  }
  T& operator()(size_t line, size_t row) {
    return matrix[line][row];
  }
  T& At(size_t line, size_t row) {
    if (line >= L || line < 0 || row >= R || row < 0) {
      throw MatrixOutOfRange{};
    }
    return matrix[line][row];
  }
  const T& At(size_t line, size_t row) const {
    if (line >= L || line < 0 || row >= R || row < 0) {
      throw MatrixOutOfRange{};
    }
    return matrix[line][row];
  }
  Matrix operator+(const Matrix& second) const {
    Matrix temp;
    for (size_t i = 0; i < L; ++i) {
      for (size_t j = 0; j < R; ++j) {
        temp(i, j) = matrix[i][j] + second(i, j);
      }
    }
    return temp;
  }
  Matrix operator-(const Matrix& second) const {
    Matrix temp;
    for (size_t i = 0; i < L; ++i) {
      for (size_t j = 0; j < R; ++j) {
        temp(i, j) = matrix[i][j] - second(i, j);
      }
    }
    return temp;
  }
  template <size_t N>
  Matrix<T, L, N> operator*(const Matrix<T, R, N>& second) const {
    Matrix<T, L, N> temp;
    for (size_t i = 0; i < L; ++i) {
      for (size_t j = 0; j < N; ++j) {
        temp(i, j) = 0;
        for (size_t k = 0; k < R; ++k) {
          temp(i, j) += matrix[i][k] * second(k, j);
        }
      }
    }
    return temp;
  }
  Matrix& operator+=(const Matrix& second) {
    for (size_t i = 0; i < L; ++i) {
      for (size_t j = 0; j < R; ++j) {
        matrix[i][j] += second(i, j);
      }
    }
    return *this;
  }
  Matrix& operator-=(const Matrix& second) {
    for (size_t i = 0; i < L; ++i) {
      for (size_t j = 0; j < R; ++j) {
        matrix[i][j] -= second(i, j);
      }
    }
    return *this;
  }
  Matrix& operator*=(const Matrix<T, R, R>& second) {
    Matrix temp;
    for (size_t i = 0; i < L; ++i) {
      for (size_t j = 0; j < R; ++j) {
        temp(i, j) = 0;
        for (size_t k = 0; k < R; ++k) {
          temp(i, j) += matrix[i][k] * second(k, j);
        }
      }
    }
    *this = temp;
    return *this;
  }
  bool operator==(const Matrix& second) const {
    for (size_t i = 0; i < L; ++i) {
      for (size_t j = 0; j < R; ++j) {
        if (matrix[i][j] != second(i, j)) {
          return false;
        }
      }
    }
    return true;
  }
  bool operator!=(const Matrix& second) const {
    return !(*this == second);
  }
  Matrix operator*(T val) const {
    Matrix temp;
    for (size_t i = 0; i < L; ++i) {
      for (size_t j = 0; j < R; ++j) {
        temp(i, j) = matrix[i][j] * val;
      }
    }
    return temp;
  }
  Matrix operator/(T val) const {
    Matrix temp;
    for (size_t i = 0; i < L; ++i) {
      for (size_t j = 0; j < R; ++j) {
        temp(i, j) = matrix[i][j] / val;
      }
    }
    return temp;
  }
  Matrix& operator*=(T val) {
    *this = *this * val;
    return *this;
  }
  Matrix& operator/=(T val) {
    *this = *this / val;
    return *this;
  }
};

template <class T, size_t L, size_t R>
Matrix<T, R, L> GetTransposed(const Matrix<T, L, R>& m) {
  Matrix<T, R, L> temp;
  for (size_t i = 0; i < R; ++i) {
    for (size_t j = 0; j < L; ++j) {
      temp(i, j) = m(j, i);
    }
  }
  return temp;
}

template <class T, size_t L, size_t R>
Matrix<T, L, R> operator*(T val, const Matrix<T, L, R>& m) {
  return m * val;
}

template <class T, size_t L, size_t R>
std::ostream& operator<<(std::ostream& stream, const Matrix<T, L, R>& m) {
  for (size_t i = 0; i < L; ++i) {
    for (size_t j = 0; j < R; ++j) {
      stream << m(i, j);
      if (j < R - 1) {
        stream << ' ';
      }
    }
    stream << '\n';
  }
  return stream;
}

template <class T, size_t L, size_t R>
std::istream& operator>>(std::istream& stream, Matrix<T, L, R>& m) {
  for (size_t i = 0; i < L; ++i) {
    for (size_t j = 0; j < R; ++j) {
      stream >> m(i, j);
    }
  }
  return stream;
}
#endif

#define REQUIRE(X) if(!(X)) { std::cout << __LINE__ << " error\n"; }

template <class T, size_t N, size_t M>
void EqualMatrix(const Matrix<T, N, M> &matrix, const std::array<std::array<T, M>, N>& arr) {
  for (size_t i = 0u; i < N; ++i) {
    for (size_t j = 0u; j < M; ++j) {
      REQUIRE(matrix(i, j) == arr[i][j]);
    }
  }
}

namespace std {
  template <class T>
  constexpr const T& as_const(T& t) noexcept
  {
      return t;
  }
}

void Test() {
  static_assert(sizeof(Matrix<int, 17, 2>) == sizeof(int) * 34);

  Matrix<int, 1, 2> a{1, -1};
  Matrix<int, 2, 1> b{-1, 2};
  REQUIRE(std::as_const(a).RowsNumber() == 1);
  REQUIRE(std::as_const(a).ColumnsNumber() == 2);
  REQUIRE(std::as_const(b).RowsNumber() == 2);
  REQUIRE(std::as_const(b).ColumnsNumber() == 1);

  // a = [1, -1]
  // b = [-1 ]
  //     [ 2 ]

  a(0, 1) = -2;
  REQUIRE(std::as_const(a)(0, 1) == -2);

  b.At(1, 0) = 3;
  REQUIRE(std::as_const(b).At(1, 0) == 3);

  // a = [1, -2]
  // b = [-1 ]
  //     [ 3 ]

  //REQUIRE_THROWS_AS(std::as_const(a).At(5, 5), MatrixOutOfRange);  // NOLINT

  EqualMatrix(a += a, {2, -4});
  EqualMatrix(b + b, {-2, 6});

  // a = [2, -4]
  // b = [-1 ]
  //     [ 3 ]

  EqualMatrix(a -= (GetTransposed(b) + GetTransposed(b)), {4, -10});
  EqualMatrix(b - b, {0, 0});

  // a = [4, -10]
  // b = [-1 ]
  //     [ 3 ]

  EqualMatrix(a * b, {-34});
  EqualMatrix(b *= Matrix<int, 1, 1>{-1}, {1, -3});

  // a = [4, -10]
  // b = [ 1 ]
  //     [-3 ]

  EqualMatrix(a *= -1l, {-4, 10});
  EqualMatrix(2 * b, {2, -6});

  // a = [-4, 10]
  // b = [ 1 ]
  //     [-3 ]

  EqualMatrix(a / 2ul, {-2, 5});
  EqualMatrix(b /= -1l, {-1, 3});

  // a = [-4, 10]
  // b = [-1 ]
  //     [ 3 ]

  REQUIRE(a == a);
  REQUIRE(a != GetTransposed(b));

  std::stringstream is{"-5 1\n0 10"};

  Matrix<int, 2, 2> matrix{};
  is >> matrix;
  EqualMatrix(matrix, std::array<std::array<int, 2>, 2>{-5, 1, 0, 10});

  std::stringstream os;
  os << std::as_const(matrix);
  std::cout << os.str() << '\n';
  REQUIRE(os.str() == "-5 1\n0 10\n");
}

int main()
{
  Test();
  Matrix<int, 2, 3> a{{{1, 2, 3}, {4, 5, 6}}};
  std::cout << a;
  Matrix<int, 3, 2> b = GetTransposed(a);
  std::cout << b;
  // a + b      CE
  std::cout << (a + a);     // O
  // a * a      CE
  std::cout << (a * b);     // Ok
  // a *= b  // CE - в матрицу a нельзя присвоить матрицу другого размера
  b *= Matrix<int, 2, 2>{1, 2, 3, 4}; // Ok
  std::cout << b;
  return 0;
}
