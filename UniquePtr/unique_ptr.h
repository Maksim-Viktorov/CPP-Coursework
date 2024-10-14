#ifndef UNIQUEPTR
#define UNIQUEPTR

#include <utility>

template <typename T>
class UniquePtr {
  T* ptr_ = nullptr;

 public:
  UniquePtr() = default;
  explicit UniquePtr(T* ptr) : ptr_(ptr) {
  }
  UniquePtr(const UniquePtr&) = delete;
  ~UniquePtr() {
    delete ptr_;
  }
  UniquePtr& operator=(const UniquePtr&) = delete;
  UniquePtr(UniquePtr&& other) noexcept {
    ptr_ = other.Release();
  }
  UniquePtr& operator=(UniquePtr&& other) noexcept {
    UniquePtr tmp(std::move(other));
    Swap(tmp);
    return *this;
  }
  T* Release() {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }
  void Reset(T* ptr = nullptr) {
    delete ptr_;
    ptr_ = ptr;
  }
  void Swap(UniquePtr& other) {
    std::swap(ptr_, other.ptr_);
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
#endif  // UNIQUEPTR
