#ifndef STRING
#define STRING

#include <iostream>
#include <stdexcept>

class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
  }
};

class String {
  size_t size_ = 0;
  size_t capacity_ = 0;
  char* ptr_ = nullptr;
  void FromCstr(const char* cstr, size_t size);

 public:
  String();
  explicit String(size_t size, char symbol);
  String(const char* cstr);  // NOLINT
  explicit String(const char* cstr, size_t size);
  String(const String& other);
  ~String();
  String& operator=(const String& other);
  String& operator+=(const String& other);
  const char& operator[](size_t i) const;
  char& operator[](size_t i);
  const char& At(size_t i) const;
  char& At(size_t i);
  const char& Front() const;
  char& Front();
  const char& Back() const;
  char& Back();
  const char* CStr() const;
  const char* Data() const;
  char* CStr();
  char* Data();
  bool Empty() const;
  size_t Size() const;
  size_t Length() const;
  size_t Capacity() const;
  void Clear();
  void Swap(String& other);
  char PopBack();
  void PushBack(char symbol);
  void Reserve(size_t new_capacity);
  void Resize(size_t new_size, char symbol);
  void ShrinkToFit();
};

String operator+(const String& str1, const String& str2);
bool operator<(const String& str1, const String& str2);
bool operator>(const String& str1, const String& str2);
bool operator<=(const String& str1, const String& str2);
bool operator>=(const String& str1, const String& str2);
bool operator==(const String& str1, const String& str2);
bool operator!=(const String& str1, const String& str2);
std::ostream& operator<<(std::ostream& out, const String& str);
#endif  // STRING
