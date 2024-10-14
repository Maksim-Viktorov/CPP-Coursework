#ifndef ENUMERATE
#define ENUMERATE
#define TEMPORARY_ENUMERATE_IMPLEMENTED

#include <utility>

namespace detail {  // NOLINT

template <typename It, typename Reference>
class Iterator {
  int64_t idx_ = 0;
  It iterator_;

 public:
  using difference_type = void;                               // NOLINT
  using value_type = typename std::pair<int64_t, Reference>;  // NOLINT
  using pointer = void;                                       // NOLINT
  using reference = typename std::pair<int64_t, Reference>;   // NOLINT
  using iterator_category = std::input_iterator_tag;          // NOLINT
  Iterator() = delete;
  Iterator(It iterator) : iterator_(iterator) {  // NOLINT
  }
  reference operator*() const {
    return value_type(idx_, *iterator_);
  }
  Iterator& operator++() {
    ++idx_;
    ++iterator_;
    return *this;
  }
  bool operator==(const Iterator& other) const {
    return iterator_ == other.iterator_;
  }
  bool operator!=(const Iterator& other) const {
    return !(*this == other);
  }
};

template <typename Container, typename It, typename Reference>
class Generator {
  Container& container_;

 public:
  using value_type = typename std::pair<int64_t, Reference>;                                 // NOLINT
  using pointer = void;                                                                      // NOLINT
  using reference = typename std::pair<int64_t, Reference>;                                  // NOLINT
  using const_reference = typename std::pair<int64_t, typename Container::const_reference>;  // NOLINT
  using iterator = Iterator<It, Reference>;                                                  // NOLINT
  using const_iterator = Iterator<typename Container::const_iterator, const Reference>;      // NOLINT
  Generator() = delete;
  explicit Generator(Container& container) : container_(container) {
  }
  iterator begin() {  // NOLINT
    return container_.begin();
  }
  iterator end() {  // NOLINT
    return container_.end();
  }
  const_iterator begin() const {  // NOLINT
    return container_.begin();
  }
  const_iterator end() const {  // NOLINT
    return container_.end();
  }
};

template <typename Container>
class RvalueGenerator {
  Container container_;

 public:
  using value_type = typename std::pair<int64_t, typename Container::reference>;                             // NOLINT
  using pointer = void;                                                                                      // NOLINT
  using reference = typename std::pair<int64_t, typename Container::reference>;                              // NOLINT
  using const_reference = typename std::pair<int64_t, typename Container::const_reference>;                  // NOLINT
  using iterator = Iterator<typename Container::iterator, typename Container::reference>;                    // NOLINT
  using const_iterator = Iterator<typename Container::const_iterator, typename Container::const_reference>;  // NOLINT
  RvalueGenerator() = delete;
  explicit RvalueGenerator(Container&& container) : container_(std::move(container)) {
  }
  iterator begin() {  // NOLINT
    return container_.begin();
  }
  iterator end() {  // NOLINT
    return container_.end();
  }
  const_iterator begin() const {  // NOLINT
    return container_.begin();
  }
  const_iterator end() const {  // NOLINT
    return container_.end();
  }
};
}  // namespace detail

template <typename Container>
auto Enumerate(Container& container) {
  return detail::Generator<Container, typename Container::iterator, typename Container::reference>(container);
}

template <typename Container>
auto Enumerate(const Container& container) {
  return detail::Generator<const Container, typename Container::const_iterator, typename Container::const_reference>(
      container);
}

template <typename Container>
auto Enumerate(Container&& container) {
  return detail::RvalueGenerator<Container>(std::move(container));
}
#endif  // ENUMERATE
