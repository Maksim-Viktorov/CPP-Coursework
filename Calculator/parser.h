#ifndef PARSER
#define PARSER

#include <string_view>
#include <stdexcept>
#include "../polish_notation/expressions.h"
#include "../tokenize/tokenize.h"

class UnknownSymbolError : public std::runtime_error {
 public:
  explicit UnknownSymbolError(const std::string& symbol = "") : std::runtime_error("UnknownSymbolError: " + symbol) {
  }
};

class WrongExpressionError : public std::runtime_error {
 public:
  explicit WrongExpressionError(const std::string& msg = "") : std::runtime_error("WrongExpressionError: " + msg) {
  }
};

Expression ParseExpression(const std::vector<Token>& tokens, size_t& idx);

Expression ParseAddendum(const std::vector<Token>& tokens, size_t& idx);

Expression ParseMultiplier(const std::vector<Token>& tokens, size_t& idx);
#endif  // PARSER
