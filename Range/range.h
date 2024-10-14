#ifndef RANGE
#define RANGE
#define REVERSE_RANGE_IMPLEMENTED

#include <cstdint>
#include <iterator>

class Iterator {
  int64_t value_;
  int64_t step_;

 public:
  friend bool operator==(const Iterator& lhs, const Iterator& rhs);
  using difference_type = int64_t;                            // NOLINT
  using value_type = int64_t;                                 // NOLINT
  using pointer = void;                                       // NOLINT
  using reference = int64_t;                                  // NOLINT
  using iterator_category = std::bidirectional_iterator_tag;  // NOLINT
  Iterator() = delete;
  Iterator(int64_t value, int64_t step) : value_(value), step_(step) {
  }
  reference operator*() const {
    return value_;
  }
  Iterator& operator++() {
    value_ += step_;
    return *this;
  }
  Iterator operator++(int) {
    auto tmp = *this;
    value_ += step_;
    return tmp;
  }
  Iterator& operator--() {
    value_ -= step_;
    return *this;
  }
  Iterator operator--(int) {
    auto tmp = *this;
    value_ -= step_;
    return tmp;
  }
};

inline bool operator!=(const Iterator& lhs, const Iterator& rhs) {
  return !(lhs == rhs);
}

inline bool operator==(const Iterator& lhs, const Iterator& rhs) {
  return lhs.value_ == rhs.value_ && lhs.step_ == rhs.step_;
}

class Generator {
  int64_t begin_ = 0;
  int64_t end_ = 0;
  int64_t step_ = 1;

 public:
  Generator() = delete;
  explicit Generator(int64_t end) : end_(end) {
    if (end_ < 0) {
      end_ = 0;
    }
  }
  Generator(int64_t begin, int64_t end) : begin_(begin), end_(end) {
    if (end_ < begin_) {
      end_ = begin_;
    }
  }
  Generator(int64_t begin, int64_t end, int64_t step) : begin_(begin), end_(end), step_(step) {
    if (step_ > 0 && end_ < begin_) {
      end_ = begin_;
    } else if (step_ < 0 && begin_ < end_) {
      end_ = begin_;
    }
    if (step_ == 0) {
      end_ = begin_;
    } else {
      auto rem = (end_ - begin_) % step_;
      if (rem) {
        end_ += step_ - rem;
      }
    }
  }
  Iterator begin() const {  // NOLINT
    return Iterator(begin_, step_);
  }
  Iterator end() const {  // NOLINT
    return Iterator(end_, step_);
  }
  std::reverse_iterator<Iterator> rbegin() const {  // NOLINT
    return std::make_reverse_iterator<Iterator>(end());
  }
  std::reverse_iterator<Iterator> rend() const {  // NOLINT
    return std::make_reverse_iterator<Iterator>(begin());
  }
};

inline Generator Range(int64_t end) {
  return Generator(end);
}

inline Generator Range(int64_t begin, int64_t end) {
  return Generator(begin, end);
}

inline Generator Range(int64_t begin, int64_t end, int64_t step) {
  return Generator(begin, end, step);
}
#endif  // RANGE
