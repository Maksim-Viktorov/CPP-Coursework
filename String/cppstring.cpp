#include <utility>
#include "cppstring.h"

void String::FromCstr(const char* cstr, size_t size) {
  Reserve(size);
  size_ = size;
  for (size = 0; size < size_; ++size) {
    ptr_[size] = cstr[size];
  }
  if (capacity_ > 0) {
    ptr_[size_] = '\0';
  }
}

String::String() = default;

String::String(size_t size, char symbol) {
  Resize(size, symbol);
}

String::String(const char* cstr) {
  size_t size;
  for (size = 0; cstr[size] != '\0'; ++size) {
  }
  FromCstr(cstr, size);
}

String::String(const char* cstr, size_t size) {
  FromCstr(cstr, size);
}

String::String(const String& other) {
  FromCstr(other.ptr_, other.size_);
}

String::~String() {
  delete[] ptr_;
}

String& String::operator=(const String& other) {
  FromCstr(other.ptr_, other.size_);
  return *this;
}

String& String::operator+=(const String& other) {
  Reserve(size_ + other.size_);
  for (size_t i = 0; i < other.size_; ++i) {
    ptr_[size_ + i] = other.ptr_[i];
  }
  size_ += other.size_;
  if (capacity_ > 0) {
    ptr_[size_] = '\0';
  }
  return *this;
}

const char& String::operator[](size_t i) const {
  return ptr_[i];
}

char& String::operator[](size_t i) {
  return ptr_[i];
}

const char& String::At(size_t i) const {
  if (i >= size_) {
    throw StringOutOfRange{};
  }
  return ptr_[i];
}

char& String::At(size_t i) {
  if (i >= size_) {
    throw StringOutOfRange{};
  }
  return ptr_[i];
}

const char& String::Front() const {
  return ptr_[0];
}

char& String::Front() {
  return ptr_[0];
}

const char& String::Back() const {
  return ptr_[size_ - 1];
}

char& String::Back() {
  return ptr_[size_ - 1];
}

const char* String::CStr() const {
  return ptr_;
}

const char* String::Data() const {
  return ptr_;
}

char* String::CStr() {
  return ptr_;
}

char* String::Data() {
  return ptr_;
}

bool String::Empty() const {
  return size_ == 0;
}

size_t String::Size() const {
  return size_;
}

size_t String::Length() const {
  return size_;
}

size_t String::Capacity() const {
  return capacity_;
}

void String::Clear() {
  size_ = 0;
}

void String::Swap(String& other) {
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
  std::swap(ptr_, other.ptr_);
}

char String::PopBack() {
  if (size_ > 0) {
    char c = ptr_[--size_];
    ptr_[size_] = '\0';
    return c;
  }
  return '\0';
}

void String::PushBack(char symbol) {
  if (size_ >= capacity_) {
    Reserve(capacity_ + 1);
  }
  ptr_[size_++] = symbol;
  ptr_[size_] = '\0';
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity > capacity_) {
    size_t capacity = 1;
    while (capacity < new_capacity) {
      capacity *= 2;
    }
    capacity_ = capacity;
    auto ptr = new char[capacity + 1];
    for (size_t i = 0; i < size_; ++i) {
      ptr[i] = ptr_[i];
    }
    delete[] ptr_;
    ptr_ = ptr;
  }
}

void String::Resize(size_t new_size, char symbol) {
  Reserve(new_size);
  for (; size_ < new_size; ++size_) {
    ptr_[size_] = symbol;
  }
  size_ = new_size;
  if (capacity_ > 0) {
    ptr_[size_] = '\0';
  }
}

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    capacity_ = size_;
    auto ptr = new char[capacity_ + 1];
    for (size_t i = 0; i < size_; ++i) {
      ptr[i] = ptr_[i];
    }
    ptr_[size_] = '\0';
    delete[] ptr_;
    ptr_ = ptr;
  }
}

String operator+(const String& str1, const String& str2) {
  String str = str1;
  str += str2;
  return str;
}

bool operator<(const String& str1, const String& str2) {
  size_t size = std::min(str1.Size(), str2.Size());
  for (size_t i = 0; i < size; ++i) {
    if (str2[i] < str1[i]) {
      return false;
    }
    if (str1[i] < str2[i]) {
      return true;
    }
  }
  return str1.Size() < str2.Size();
}

bool operator>(const String& str1, const String& str2) {
  return str2 < str1;
}

bool operator<=(const String& str1, const String& str2) {
  return !(str2 < str1);
}

bool operator>=(const String& str1, const String& str2) {
  return !(str1 < str2);
}

bool operator==(const String& str1, const String& str2) {
  return !(str2 < str1) && !(str1 < str2);
}

bool operator!=(const String& str1, const String& str2) {
  return (str2 < str1) || (str1 < str2);
}

std::ostream& operator<<(std::ostream& out, const String& str) {
  return out.write(str.Data(), str.Size());
}
