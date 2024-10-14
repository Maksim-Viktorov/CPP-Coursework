#include "tokenize.h"

bool operator==(const PlusToken&, const PlusToken&) {
  return true;
}

bool operator==(const MinusToken&, const MinusToken&) {
  return true;
}

bool operator==(const MultiplyToken&, const MultiplyToken&) {
  return true;
}

bool operator==(const DivideToken&, const DivideToken&) {
  return true;
}

bool operator==(const ResidualToken&, const ResidualToken&) {
  return true;
}

bool operator==(const OpeningBracketToken&, const OpeningBracketToken&) {
  return true;
}

bool operator==(const ClosingBracketToken&, const ClosingBracketToken&) {
  return true;
}

bool operator==(const SqrToken&, const SqrToken&) {
  return true;
}

bool operator==(const MaxToken&, const MaxToken&) {
  return true;
}

bool operator==(const MinToken&, const MinToken&) {
  return true;
}

bool operator==(const AbsToken&, const AbsToken&) {
  return true;
}

bool operator==(const NumberToken& first, const NumberToken& second) {
  return first.value == second.value;
}

bool operator==(const UnknownToken& first, const UnknownToken& second) {
  return first.value == second.value;
}

std::vector<std::string> Split(const std::string_view& input, const char c) {
  std::vector<std::string> split;
  for (size_t i = 0; i < input.size(); ++i) {
    std::string str;
    while (input[i] != c && i < input.size()) {
      str += input[i++];
    }
    if (!str.empty()) {
      split.emplace_back(str);
    }
  }
  return split;
}

bool Isnumber(const std::string& str) {
  if (str[0] != '+' && str[0] != '-' && (str[0] > '9' || str[0] < '0')) {
    return false;
  }
  size_t i = (str[0] <= '9' && str[0] >= '0' ? 0 : 1);
  if (i == 1 && str.size() < 2) {
    return false;
  }
  for (; i < str.size(); ++i) {
    if (str[i] > '9' || str[i] < '0') {
      return false;
    }
  }
  return true;
}

std::vector<Token> Tokenize(const std::string_view& str) {
  std::vector<std::string> split(Split(str, ' '));
  std::vector<Token> vect;
  for (size_t i = 0; i < split.size(); ++i) {
    if (Isnumber(split[i])) {
      vect.emplace_back(NumberToken(std::stoi(split[i])));
    } else if (split[i] == "+") {
      vect.emplace_back(PlusToken());
    } else if (split[i] == "-") {
      vect.emplace_back(MinusToken());
    } else if (split[i] == "*") {
      vect.emplace_back(MultiplyToken());
    } else if (split[i] == "/") {
      vect.emplace_back(DivideToken());
    } else if (split[i] == "%") {
      vect.emplace_back(ResidualToken());
    } else if (split[i] == "(") {
      vect.emplace_back(OpeningBracketToken());
    } else if (split[i] == ")") {
      vect.emplace_back(ClosingBracketToken());
    } else if (split[i] == "abs") {
      vect.emplace_back(AbsToken());
    } else if (split[i] == "sqr") {
      vect.emplace_back(SqrToken());
    } else if (split[i] == "max") {
      vect.emplace_back(MaxToken());
    } else if (split[i] == "min") {
      vect.emplace_back(MinToken());
    } else {
      vect.emplace_back(UnknownToken(split[i]));
    }
  }
  return vect;
}
