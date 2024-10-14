#include "calculator.h"

int CalculateExpression(const std::string_view& input) {
  std::vector<Token> tokens = Tokenize(input);
  size_t idx = 0;
  Expression expr(ParseExpression(tokens, idx));
  if (idx != tokens.size()) {
    if (std::holds_alternative<UnknownToken>(tokens[idx])) {
      throw UnknownSymbolError{};
    }
    throw WrongExpressionError{};
  }
  return expr->Calculate();
}
