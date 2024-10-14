#ifndef SHAREDPTR
#define SHAREDPTR

#include <stdexcept>
#include <utility>

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
  friend class WeakPtr<T>;
  struct Counter {
    size_t strong = 1;
    size_t weak = 0;
  };
  T* ptr_ = nullptr;
  Counter* counter_ = nullptr;

 public:
  SharedPtr() : counter_(new Counter) {
  }
  SharedPtr(T* ptr) : ptr_(ptr), counter_(new Counter) {  // NOLINT
  }
  SharedPtr(const SharedPtr& other) {
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    ++counter_->strong;
  }
  ~SharedPtr() {
    if (--counter_->strong == 0) {
      if (counter_->weak == 0) {
        delete counter_;
      }
      delete ptr_;
    }
  }
  explicit SharedPtr(const WeakPtr<T>& weak) {
    if (weak.Expired()) {
      throw BadWeakPtr{};
    }
    ptr_ = weak.ptr_;
    counter_ = weak.counter_;
    ++counter_->strong;
  }
  SharedPtr& operator=(const SharedPtr& other) {
    SharedPtr tmp(other);
    Swap(tmp);
    return *this;
  }
  SharedPtr(SharedPtr&& other) noexcept {
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    other.ptr_ = nullptr;
    other.counter_ = new Counter;
  }
  SharedPtr& operator=(SharedPtr&& other) noexcept {
    SharedPtr tmp(std::move(other));
    Swap(tmp);
    return *this;
  }
  void Reset(T* ptr = nullptr) {
    SharedPtr tmp(ptr);
    Swap(tmp);
  }
  void Swap(SharedPtr& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(counter_, other.counter_);
  }
  size_t UseCount() const {
    return (ptr_ ? counter_->strong : 0);
  }
  T* Get() const {
    return ptr_;
  }
  T& operator*() const {
    return *ptr_;
  }
  T* operator->() const {
    return ptr_;
  }
  explicit operator bool() const {
    return ptr_ != nullptr;
  }
};

#define WEAK_PTR_IMPLEMENTED

template <typename T>
class WeakPtr {
  friend class SharedPtr<T>;
  T* ptr_ = nullptr;
  typename SharedPtr<T>::Counter* counter_ = nullptr;

 public:
  WeakPtr() : counter_(new typename SharedPtr<T>::Counter) {
    counter_->strong = 0;
    counter_->weak = 1;
  }
  WeakPtr(const SharedPtr<T>& other) {  // NOLINT
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    ++counter_->weak;
  }
  WeakPtr(const WeakPtr& other) {
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    ++counter_->weak;
  }
  ~WeakPtr() {
    if (--counter_->weak == 0) {
      if (counter_->strong == 0) {
        delete counter_;
      }
    }
  }
  WeakPtr& operator=(const WeakPtr& other) {
    WeakPtr tmp(other);
    Swap(tmp);
    return *this;
  }
  WeakPtr(WeakPtr&& other) noexcept {
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    other.ptr_ = nullptr;
    other.counter_ = new typename SharedPtr<T>::Counter;
    other.counter_->strong = 0;
    other.counter_->weak = 1;
  }
  WeakPtr& operator=(WeakPtr&& other) noexcept {
    WeakPtr tmp(std::move(other));
    Swap(tmp);
    return *this;
  }
  void Reset(T* ptr = nullptr) {
    WeakPtr tmp(ptr);
    Swap(tmp);
  }
  void Swap(WeakPtr& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(counter_, other.counter_);
  }
  size_t UseCount() const {
    return (ptr_ ? counter_->strong : 0);
  }
  bool Expired() const {
    return UseCount() == 0;
  }
  SharedPtr<T> Lock() const {
    return (Expired() ? nullptr : SharedPtr<T>(*this));
  }
};
#endif  // SHAREDPTR
