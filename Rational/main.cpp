#include <cmath>
#include <iostream>
#include <stdexcept>
#include "rational.h"
#include <sstream>

void RationalEqual(const Rational& rational, int64_t numerator, int64_t denominator) {
  if(!(rational.GetNumerator() == numerator)) std::cout << "1error\n";
  if(!(rational.GetDenominator() == denominator)) std::cout << "2error\n";
}

int main() {
  Rational a(3, 5);
  Rational b(3, -5);
  Rational c;
  Rational d = c * a;
  std::cout << d;
  return 0;
}
