#include "parser.h"

Expression ParseExpression(const std::vector<Token>& tokens, size_t& idx) {
  auto lhs = ParseAddendum(tokens, idx);
  while (idx < tokens.size()) {
    const Token& token = tokens[idx++];
    if (std::holds_alternative<PlusToken>(token)) {
      auto rhs = ParseAddendum(tokens, idx);
      lhs = std::move(std::make_unique<Sum>(std::move(lhs), std::move(rhs)));
    } else if (std::holds_alternative<MinusToken>(token)) {
      auto rhs = ParseAddendum(tokens, idx);
      lhs = std::move(std::make_unique<Subtract>(std::move(lhs), std::move(rhs)));
    } else {
      --idx;
      break;
    }
  }
  return lhs;
}

Expression ParseAddendum(const std::vector<Token>& tokens, size_t& idx) {
  auto lhs = ParseMultiplier(tokens, idx);
  while (idx < tokens.size()) {
    const Token& token = tokens[idx++];
    if (std::holds_alternative<MultiplyToken>(token)) {
      auto rhs = ParseMultiplier(tokens, idx);
      lhs = std::move(std::make_unique<Multiply>(std::move(lhs), std::move(rhs)));
    } else if (std::holds_alternative<DivideToken>(token)) {
      auto rhs = ParseMultiplier(tokens, idx);
      lhs = std::move(std::make_unique<Divide>(std::move(lhs), std::move(rhs)));
    } else if (std::holds_alternative<ResidualToken>(token)) {
      auto rhs = ParseMultiplier(tokens, idx);
      lhs = std::move(std::make_unique<Residual>(std::move(lhs), std::move(rhs)));
    } else {
      --idx;
      break;
    }
  }
  return lhs;
}

Expression ParseMultiplier(const std::vector<Token>& tokens, size_t& idx) {
  if (idx >= tokens.size()) {
    throw WrongExpressionError{};
  }
  const Token& token = tokens[idx++];
  if (std::holds_alternative<NumberToken>(token)) {
    return std::make_unique<Constant>(std::get<NumberToken>(token).value);
  }
  if (std::holds_alternative<OpeningBracketToken>(token)) {
    auto expr = ParseExpression(tokens, idx);
    if (idx >= tokens.size()) {
      throw WrongExpressionError{};
    }
    const Token& token = tokens[idx++];
    if (std::holds_alternative<ClosingBracketToken>(token)) {
      return expr;
    }
  }
  if (std::holds_alternative<UnknownToken>(token)) {
    throw UnknownSymbolError{};
  }
  throw WrongExpressionError{};
}
