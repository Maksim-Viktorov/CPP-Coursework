#ifndef VECTOR
#define VECTOR
#define VECTOR_MEMORY_IMPLEMENTED

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

class VectorOutOfRange : public std::out_of_range {

 public:
  VectorOutOfRange() : std::out_of_range("VectorOutOfRange") {
  }
};

template <typename T>
class Vector {
  size_t size_ = 0;
  size_t capacity_ = 0;
  char* ptr_ = nullptr;
  Vector(bool, size_t capacity) : capacity_(capacity) {
    if (capacity_) {
      ptr_ = new char[capacity_ * sizeof(T)];
    }
  }
  template <class... Args>
  void EmplaceBack_(Args&&... args) {  // NOLINT
    new (static_cast<void*>(reinterpret_cast<T*>(ptr_) + size_)) ValueType(std::forward<Args>(args)...);
    ++size_;
  }

 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using RValue = T&&;
  using SizeType = size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  Vector() = default;
  explicit Vector(SizeType size) {
    Vector tmp(true, size);
    while (tmp.size_ < size) {
      tmp.EmplaceBack_();
    }
    Swap(tmp);
  }
  Vector(SizeType size, ConstReference value) {
    Vector tmp(true, size);
    while (tmp.size_ < size) {
      tmp.EmplaceBack_(value);
    }
    Swap(tmp);
  }
  template <class InputIt, class = std::enable_if_t<std::is_base_of_v<
                               std::forward_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>
  Vector(InputIt begin, InputIt end) {
    SizeType size = std::distance(begin, end);
    Vector tmp(true, size);
    while (tmp.size_ < size) {
      tmp.EmplaceBack_(*begin++);
    }
    Swap(tmp);
  }
  Vector(std::initializer_list<ValueType> list) : Vector(list.begin(), list.end()) {
  }
  Vector(const Vector& other) : Vector(other.begin(), other.end()) {
  }
  Vector(Vector&& other) noexcept {
    size_ = other.size_;
    capacity_ = other.capacity_;
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  ~Vector() {
    Clear();
    delete[] ptr_;
  }
  Vector& operator=(const Vector& other) {
    if (this != &other) {
      Vector tmp(other.begin(), other.end());
      Swap(tmp);
    }
    return *this;
  }
  Vector& operator=(Vector&& other) noexcept {
    Clear();
    delete[] ptr_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
  }
  ConstReference operator[](SizeType i) const {
    return Data()[i];
  }
  Reference operator[](SizeType i) {
    return Data()[i];
  }
  ConstReference At(SizeType i) const {
    if (i >= size_) {
      throw VectorOutOfRange{};
    }
    return Data()[i];
  }
  Reference At(SizeType i) {
    if (i >= size_) {
      throw VectorOutOfRange{};
    }
    return Data()[i];
  }
  ConstReference Front() const {
    return Data()[0];
  }
  Reference Front() {
    return Data()[0];
  }
  ConstReference Back() const {
    return Data()[size_ - 1];
  }
  Reference Back() {
    return Data()[size_ - 1];
  }
  ConstPointer Data() const {
    return static_cast<ConstPointer>(static_cast<const void*>(ptr_));
  }
  Pointer Data() {
    return static_cast<Pointer>(static_cast<void*>(ptr_));
  }
  bool Empty() const {
    return size_ == 0;
  }
  SizeType Size() const {
    return size_;
  }
  SizeType Capacity() const {
    return capacity_;
  }
  void Clear() {
    for (; size_ != 0;) {
      PopBack();
    }
  }
  void Swap(Vector& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }
  void PopBack() {
    Data()[--size_].~ValueType();
  }
  void PushBack(ConstReference value) {
    EmplaceBack(value);
  }
  void PushBack(RValue value) {
    EmplaceBack(std::move(value));
  }
  template <class... Args>
  void EmplaceBack(Args&&... args) {
    if (size_ >= capacity_) {
      SizeType new_capacity = capacity_ * 2;
      if (new_capacity == 0) {
        new_capacity = 1;
      }
      Reserve(new_capacity);
    }
    EmplaceBack_(std::forward<Args>(args)...);
  }
  void Reserve(SizeType new_capacity) {
    if (new_capacity > capacity_) {
      Vector tmp(true, new_capacity);
      while (tmp.size_ < size_) {
        tmp.EmplaceBack_();
      }
      for (size_t i = 0; i < size_; ++i) {
        tmp[i] = std::move(Data()[i]);
      }
      Swap(tmp);
    }
  }
  void Resize(SizeType new_size) {
    if (new_size > capacity_) {
      Reserve(new_size);
    }
    SizeType old_size = size_;
    try {
      while (size_ < new_size) {
        EmplaceBack_();
      }
    } catch (...) {
      while (size_ > old_size) {
        PopBack();
      }
      throw;
    }
    while (size_ > new_size) {
      PopBack();
    }
  }
  void Resize(SizeType new_size, ConstReference value) {
    if (new_size > capacity_) {
      Reserve(new_size);
    }
    SizeType old_size = size_;
    try {
      while (size_ < new_size) {
        EmplaceBack_(value);
      }
    } catch (...) {
      while (size_ > old_size) {
        PopBack();
      }
      throw;
    }
    while (size_ > new_size) {
      PopBack();
    }
  }
  void ShrinkToFit() {
    if (capacity_ > size_) {
      Vector tmp(true, size_);
      for (; tmp.size_ < size_;) {
        tmp.EmplaceBack_(std::move(Data()[tmp.size_]));
      }
      Swap(tmp);
    }
  }
  Iterator begin() {  // NOLINT
    return Data();
  }
  Iterator end() {  // NOLINT
    return Data() + size_;
  }
  ConstIterator begin() const {  // NOLINT
    return Data();
  }
  ConstIterator end() const {  // NOLINT
    return Data() + size_;
  }
  ConstIterator cbegin() const {  // NOLINT
    return Data();
  }
  ConstIterator cend() const {  // NOLINT
    return Data() + size_;
  }
  ReverseIterator rbegin() {  // NOLINT
    return ReverseIterator(Data() + size_);
  }
  ReverseIterator rend() {  // NOLINT
    return ReverseIterator(Data());
  }
  ConstReverseIterator rbegin() const {  // NOLINT
    return ConstReverseIterator(Data() + size_);
  }
  ConstReverseIterator rend() const {  // NOLINT
    return ConstReverseIterator(Data());
  }
  ConstReverseIterator crbegin() const {  // NOLINT
    return ConstReverseIterator(Data() + size_);
  }
  ConstReverseIterator crend() const {  // NOLINT
    return ConstReverseIterator(Data());
  }
};

template <typename T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
  size_t size = std::min(lhs.Size(), rhs.Size());
  for (size_t i = 0; i < size; ++i) {
    if (rhs[i] < lhs[i]) {
      return false;
    }
    if (lhs[i] < rhs[i]) {
      return true;
    }
  }
  return lhs.Size() < rhs.Size();
}

template <typename T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
  return rhs < lhs;
}

template <typename T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
  return !(rhs < lhs);
}

template <typename T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
  return !(lhs < rhs);
}

template <typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
  return !(rhs < lhs) && !(lhs < rhs);
}

template <typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
  return (rhs < lhs) || (lhs < rhs);
}
#endif  // VECTOR
