#ifndef STACK
#define STACK

#include <deque>

template <typename T, typename Container = std::deque<T> >
class Stack {
  Container data_;

 public:
  Stack() = default;
  Stack(const Stack& other) = default;
  Stack(Stack&& other) = default;
  explicit Stack(const Container& container) {
    data_ = container;
  }
  explicit Stack(Container&& container) {
    data_ = std::move(container);
  }
  template <class InputIt>
  Stack(InputIt begin, InputIt end) {
    data_ = Container(begin, end);
  }
  Stack& operator=(const Stack& other) = default;
  Stack& operator=(Stack&&) = default;
  ~Stack() = default;
  T& Top() {
    return data_.back();
  }
  const T& Top() const {
    return data_.back();
  }
  bool Empty() const {
    return data_.empty();
  }
  size_t Size() const {
    return data_.size();
  }
  void Push(const T& value) {
    data_.push_back(value);
  }
  template <class... Args>
  void Emplace(Args&&... args) {
    data_.emplace_back(std::forward<Args>(args)...);
  }
  void Pop() {
    data_.pop_back();
  }
  void Swap(Stack& other) noexcept {
    data_.swap(other.data_);
  }
};
#endif  // STACK
