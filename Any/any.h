#ifndef ANY
#define ANY

#include <memory>
#include <stdexcept>

class BadAnyCast : public std::bad_cast {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "BadAnyCast error";
  }
};

struct IHolder {
  virtual ~IHolder() = default;
  virtual IHolder* Clone() = 0;
};

class Any;

template <typename T>
class AnyHolder : public IHolder {
  template <typename U>
  friend U& AnyCast(const Any& any);
  T val_;

 public:
  explicit AnyHolder(const T& val) : val_(val) {
  }
  IHolder* Clone() override {
    return new AnyHolder(val_);
  }
};

class Any {
  template <typename U>
  friend U& AnyCast(const Any& any);
  std::unique_ptr<IHolder> ptr_ = nullptr;

 public:
  Any() = default;
  Any(const Any& other) {
    ptr_.reset(other.ptr_->Clone());
  }
  Any(Any&& other) noexcept {
    ptr_ = std::move(other.ptr_);
  }
  template <typename T>
  Any(const T& val) {  // NOLINT
    ptr_ = std::make_unique<AnyHolder<T>>(val);
  }
  template <typename T>
  Any& operator=(const T& val) {
    ptr_ = std::make_unique<AnyHolder<T>>(val);
    return *this;
  }
  Any& operator=(const Any& other) {
    ptr_.reset(other.ptr_->Clone());
    return *this;
  }
  Any& operator=(Any&& other) noexcept {
    ptr_.reset(other.ptr_.release());
    return *this;
  }
  void Swap(Any& other) {
    ptr_.swap(other.ptr_);
  }
  void Reset() {
    ptr_.reset();
  }
  bool HasValue() const {
    return (ptr_ != nullptr);
  }
};

template <typename T>
T& AnyCast(const Any& any) {
  auto ptr = dynamic_cast<AnyHolder<T>*>(any.ptr_.get());
  if (!ptr) {
    throw BadAnyCast{};
  }
  return ptr->val_;
}

#endif  // ANY
