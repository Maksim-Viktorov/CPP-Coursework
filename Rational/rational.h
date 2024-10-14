#ifndef RATIONAL_H
#define RATIONAL_H
#include <cmath>
#include <iostream>
#include <stdexcept>

class RationalDivisionByZero : public std::runtime_error {
 public:
  RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
  }
};

class Rational {
  int32_t num_;
  int32_t den_;
  void Normalize(int32_t num, int32_t den);

 public:
  Rational();             // NOLINT
  Rational(int32_t num);  // NOLINT
  Rational(int32_t num, int32_t den);

  int32_t GetNumerator() const;
  int32_t GetDenominator() const;
  void SetNumerator(int32_t num);
  void SetDenominator(int32_t den);

  Rational& operator+=(const Rational& second);
  Rational& operator-=(const Rational& second);
  Rational& operator*=(const Rational& second);
  Rational& operator/=(const Rational& second);

  Rational operator+() const;
  Rational operator-() const;
  Rational& operator++();
  Rational& operator--();
  Rational operator++(int);
  Rational operator--(int);
};

Rational operator+(const Rational& first, const Rational& second);
Rational operator-(const Rational& first, const Rational& second);
Rational operator*(const Rational& first, const Rational& second);
Rational operator/(const Rational& first, const Rational& second);

bool operator<(const Rational& first, const Rational& second);
bool operator>(const Rational& first, const Rational& second);
bool operator==(const Rational& first, const Rational& second);
bool operator!=(const Rational& first, const Rational& second);
bool operator<=(const Rational& first, const Rational& second);
bool operator>=(const Rational& first, const Rational& second);

std::ostream& operator<<(std::ostream& stream, const Rational& val);
std::istream& operator>>(std::istream& stream, Rational& val);
#endif
