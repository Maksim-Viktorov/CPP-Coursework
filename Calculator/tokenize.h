#ifndef TOKENIZE
#define TOKENIZE

#include <string>
#include <string_view>
#include <variant>
#include <vector>

struct PlusToken {};

bool operator==(const PlusToken& first, const PlusToken& second);

struct MinusToken {};

bool operator==(const MinusToken& first, const MinusToken& second);

struct MultiplyToken {};

bool operator==(const MultiplyToken& first, const MultiplyToken& second);

struct DivideToken {};

bool operator==(const DivideToken& first, const DivideToken& second);

struct ResidualToken {};

bool operator==(const ResidualToken& first, const ResidualToken& second);

struct OpeningBracketToken {};

bool operator==(const OpeningBracketToken& first, const OpeningBracketToken& second);

struct ClosingBracketToken {};

bool operator==(const ClosingBracketToken& first, const ClosingBracketToken& second);

struct SqrToken {};

bool operator==(const SqrToken& first, const SqrToken& second);

struct MaxToken {};

bool operator==(const MaxToken& first, const MaxToken& second);

struct MinToken {};

bool operator==(const MinToken& first, const MinToken& second);

struct AbsToken {};

bool operator==(const AbsToken& first, const AbsToken& second);

struct NumberToken {
  int64_t value;
  NumberToken(int64_t number) : value(number) {  // NOLINT
  }
};

bool operator==(const NumberToken& first, const NumberToken& second);

struct UnknownToken {
  std::string value;
  UnknownToken(std::string str) : value(str) {  // NOLINT
  }
};

bool operator==(const UnknownToken& first, const UnknownToken& second);

using Token = std::variant<PlusToken, MinusToken, MultiplyToken, DivideToken, ResidualToken, OpeningBracketToken,
                           ClosingBracketToken, SqrToken, MaxToken, MinToken, AbsToken, NumberToken, UnknownToken>;

std::vector<Token> Tokenize(const std::string_view& str);
#endif  // TOKENIZE
