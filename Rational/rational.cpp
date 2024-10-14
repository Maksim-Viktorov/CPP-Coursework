#include "rational.h"

int32_t Gcd(int32_t a, int32_t b) {
  int32_t k;
  k = std::max(a, b);
  b = std::min(a, b);
  if (b == 0) {
    return k;
  }
  a = k;
  while (true) {
    k = a % b;
    if (k == 0) {
      break;
    }
    a = b;
    b = k;
  }
  return b;
}

int32_t Lcm(int32_t a, int32_t b) {
  int32_t k = Gcd(a, b);
  return a * (b / k);
}

void Rational::Normalize(int32_t num, int32_t den) {
  if (den == 0) {
    throw RationalDivisionByZero{};
  }
  if (den < 0) {
    num = -num;
    den = -den;
  }
  int32_t gcd = Gcd(std::abs(num), den);
  num_ = num / gcd;
  den_ = den / gcd;
}

Rational::Rational() : num_(0), den_(1) {  // NOLINT
}
Rational::Rational(int32_t num) : num_(num), den_(1) {  // NOLINT
}
Rational::Rational(int32_t num, int32_t den) {
  Normalize(num, den);
}
int32_t Rational::GetNumerator() const {
  return num_;
}
int32_t Rational::GetDenominator() const {
  return den_;
}
void Rational::SetNumerator(int32_t num) {
  Normalize(num, den_);
}
void Rational::SetDenominator(int32_t den) {
  Normalize(num_, den);
}

Rational& Rational::operator+=(const Rational& second) {
  int32_t lcm = Lcm(den_, second.den_);
  Normalize(num_ * (lcm / den_) + second.num_ * (lcm / second.den_), lcm);
  return *this;
}

Rational& Rational::operator-=(const Rational& second) {
  int32_t lcm = Lcm(den_, second.den_);
  Normalize(num_ * (lcm / den_) - second.num_ * (lcm / second.den_), lcm);
  return *this;
}

Rational& Rational::operator*=(const Rational& second) {
  if (num_ == 0 || second.num_ == 0) {
    num_ = 0;
    den_ = 1;
    return *this;
  }
  int32_t g = Gcd(num_, second.den_);
  int32_t k = Gcd(second.num_, den_);
  Normalize((num_ / g) * (second.num_ / k), (den_ / k) * (second.den_ / g));
  return *this;
}

Rational& Rational::operator/=(const Rational& second) {
  if (second.num_ == 0) {
    throw RationalDivisionByZero{};
  }
  if (num_ == 0) {
    return *this;
  }
  int32_t g = Gcd(num_, second.num_);
  int32_t k = Gcd(second.den_, den_);
  Normalize((num_ / g) * (second.den_ / k), (den_ / k) * (second.num_ / g));
  return *this;
}

Rational Rational::operator+() const {
  return Rational(num_, den_);
}
Rational Rational::operator-() const {
  return Rational(-num_, den_);
}

Rational& Rational::operator++() {
  Normalize(num_ + den_, den_);
  return *this;
}
Rational& Rational::operator--() {
  Normalize(num_ - den_, den_);
  return *this;
}
Rational Rational::operator++(int) {
  Rational r(num_, den_);
  Normalize(num_ + den_, den_);
  return r;
}
Rational Rational::operator--(int) {
  Rational r(num_, den_);
  Normalize(num_ - den_, den_);
  return r;
}

Rational operator+(const Rational& first, const Rational& second) {
  Rational r = first;
  r += second;
  return r;
}

Rational operator-(const Rational& first, const Rational& second) {
  Rational r = first;
  r -= second;
  return r;
}

Rational operator*(const Rational& first, const Rational& second) {
  Rational r = first;
  r *= second;
  return r;
}

Rational operator/(const Rational& first, const Rational& second) {
  Rational r = first;
  r /= second;
  return r;
}

bool operator<(const Rational& first, const Rational& second) {
  Rational r = first - second;
  return r.GetNumerator() < 0;
}

bool operator>(const Rational& first, const Rational& second) {
  return second < first;
}

bool operator==(const Rational& first, const Rational& second) {
  return !(first < second || second < first);
}

bool operator!=(const Rational& first, const Rational& second) {
  return first < second || second < first;
}

bool operator<=(const Rational& first, const Rational& second) {
  return !(second < first);
}

bool operator>=(const Rational& first, const Rational& second) {
  return !(first < second);
}

std::ostream& operator<<(std::ostream& stream, const Rational& val) {
  stream << val.GetNumerator();
  if (val.GetDenominator() != 1) {
    stream << '/' << val.GetDenominator();
  }
  return stream;
}

std::istream& operator>>(std::istream& stream, Rational& val) {
  int32_t num = 0, den = 1;
  stream >> num;
  if (stream.peek() == '/') {
    char c;
    stream >> c >> den;
  }
  val = Rational(num, den);
  return stream;
}
