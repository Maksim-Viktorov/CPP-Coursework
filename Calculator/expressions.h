#ifndef EXPRESSIONS
#define EXPRESSIONS

#include <algorithm>
#include <utility>
#include <memory>

class IExpression {

 public:
  virtual ~IExpression() = default;
  virtual int Calculate() const = 0;
};

using Expression = std::unique_ptr<IExpression>;

class Constant : public IExpression {
  int val_;

 public:
  explicit Constant(int val) : val_(val) {
  }
  int Calculate() const override {
    return val_;
  }
};

class IUnaryOperation : public IExpression {
  Expression operand_;

 public:
  explicit IUnaryOperation(Expression expression) : operand_(std::move(expression)) {
  }
  int Calculate() const override {
    return Operation(operand_->Calculate());
  }
  virtual int Operation(int operand) const = 0;
};

class IBinaryOperation : public IExpression {
  Expression lhs_, rhs_;

 public:
  IBinaryOperation(Expression lhs, Expression rhs) : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {
  }
  int Calculate() const override {
    return Operation(lhs_->Calculate(), rhs_->Calculate());
  }
  virtual int Operation(int lhs, int rhs) const = 0;
};

class Sum : public IBinaryOperation {

 public:
  Sum(Expression lhs, Expression rhs) : IBinaryOperation(std::move(lhs), std::move(rhs)) {
  }
  int Operation(int lhs, int rhs) const override {
    return lhs + rhs;
  }
};

class Subtract : public IBinaryOperation {

 public:
  Subtract(Expression lhs, Expression rhs) : IBinaryOperation(std::move(lhs), std::move(rhs)) {
  }
  int Operation(int lhs, int rhs) const override {
    return lhs - rhs;
  }
};

class Multiply : public IBinaryOperation {

 public:
  Multiply(Expression lhs, Expression rhs) : IBinaryOperation(std::move(lhs), std::move(rhs)) {
  }
  int Operation(int lhs, int rhs) const override {
    return lhs * rhs;
  }
};

class Divide : public IBinaryOperation {

 public:
  Divide(Expression lhs, Expression rhs) : IBinaryOperation(std::move(lhs), std::move(rhs)) {
  }
  int Operation(int lhs, int rhs) const override {
    return lhs / rhs;
  }
};

class Residual : public IBinaryOperation {

 public:
  Residual(Expression lhs, Expression rhs) : IBinaryOperation(std::move(lhs), std::move(rhs)) {
  }
  int Operation(int lhs, int rhs) const override {
    return lhs % rhs;
  }
};

class Minimum : public IBinaryOperation {

 public:
  Minimum(Expression lhs, Expression rhs) : IBinaryOperation(std::move(lhs), std::move(rhs)) {
  }
  int Operation(int lhs, int rhs) const override {
    return std::min(lhs, rhs);
  }
};

class Maximum : public IBinaryOperation {

 public:
  Maximum(Expression lhs, Expression rhs) : IBinaryOperation(std::move(lhs), std::move(rhs)) {
  }
  int Operation(int lhs, int rhs) const override {
    return std::max(lhs, rhs);
  }
};

class AbsoluteValue : public IUnaryOperation {

 public:
  explicit AbsoluteValue(Expression operand) : IUnaryOperation(std::move(operand)) {
  }
  int Operation(int operand) const override {
    return (operand > 0 ? operand : -operand);
  }
};

class Square : public IUnaryOperation {

 public:
  explicit Square(Expression operand) : IUnaryOperation(std::move(operand)) {
  }
  int Operation(int operand) const override {
    return operand * operand;
  }
};
#endif  // EXPRESSIONS
