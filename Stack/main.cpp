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
  template<class InputIt>
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
  template<class... Args>
  void Emplace( Args&&... args) {
    data_.emplace_back(args... );
  }
  void Pop() {
    data_.pop_back();
  }
  void Swap(Stack& other) noexcept {
    data_.swap(other.data_);
  }
};
#endif  // STACK

#define CATCH_CONFIG_MAIN

#include <list>
#include <vector>

#define REQUIRE(X) if(!(X)) { std::cout << __LINE__ << " error\n"; }
#define REQUIRE_FALSE(X) if((X)) { std::cout << __LINE__ << " error\n"; }
#define STR(x) STR2(x)
#define STR2(x) # x
#define REQUIRE_THROWS_AS(x, e) try { if(x) {}; throw std::runtime_error(STR(__LINE__) " error"); } catch (e& ex) {}

int main() {

#define TEST_CASE(a, b) std::cout << a << " " << b << '\n';
#define SECTION(a) std::cout << a << '\n';


TEST_CASE("Default", "[Stack]") {
  {
    Stack<std::vector<int>> s;
    REQUIRE(s.Empty());
    s.Push({1, 2, 3});
    s.Top()[1] = -2;
    REQUIRE(s.Top() == (std::vector{1, -2, 3}));
    s.Emplace(10, 1);
    REQUIRE(s.Top() == std::vector(10, 1));
    s.Push(s.Top());
    REQUIRE(s.Top() == std::vector(10, 1));
    REQUIRE(s.Size() == 3);
    s.Pop();
    s.Pop();
    REQUIRE(s.Size() == 1);
    REQUIRE(s.Top() == (std::vector{1, -2, 3}));
    s.Pop();
    REQUIRE(s.Size() == 0);
    REQUIRE(s.Empty());
  }

  {
    std::deque<int> d{1, 2, 3};
    Stack<int> s(d);
    REQUIRE_FALSE(s.Empty());
    REQUIRE(s.Top() == 3);
    s.Top() = 4;
    REQUIRE(s.Top() == 4);
    s.Push(-4);
    REQUIRE(s.Size() == 4);
    REQUIRE(s.Top() == -4);
    Stack<int> ss({-1, -1, -1});
    s.Swap(ss);

    REQUIRE(s.Size() == 3);
    REQUIRE(s.Top() == -1);
    REQUIRE(ss.Size() == 4);
    REQUIRE(ss.Top() == -4);
  }

  {
    std::deque<int> d{1, 2, 3};
    Stack<int> s(std::move(d));
    REQUIRE_FALSE(s.Empty());
    REQUIRE(s.Top() == 3);
    s.Top() = 4;
    REQUIRE(s.Top() == 4);
  }

  {
    std::deque<int> d{1, 2, 3};
    const Stack<int> s(d.begin(), d.end());
    REQUIRE_FALSE(s.Empty());
    REQUIRE(s.Top() == 3);
    REQUIRE(s.Size() == 3);
  }
}

TEST_CASE("List", "[Stack]") {
  {
    Stack<std::vector<int>, std::list<std::vector<int>>> s;
    REQUIRE(s.Empty());
    s.Push({1, 2, 3});
    s.Top()[1] = -2;
    REQUIRE(s.Top() == (std::vector{1, -2, 3}));
    s.Emplace(10, 1);
    REQUIRE(s.Top() == std::vector(10, 1));
    s.Push(s.Top());
    REQUIRE(s.Top() == std::vector(10, 1));
    REQUIRE(s.Size() == 3);
    s.Pop();
    s.Pop();
    REQUIRE(s.Size() == 1);
    REQUIRE(s.Top() == (std::vector{1, -2, 3}));
    s.Pop();
    REQUIRE(s.Size() == 0);
    REQUIRE(s.Empty());
  }

  {
    std::list<int> d{1, 2, 3};
    Stack<int, std::list<int>> s(d);
    REQUIRE_FALSE(s.Empty());
    REQUIRE(s.Top() == 3);
    s.Top() = 4;
    REQUIRE(s.Top() == 4);
    s.Push(-4);
    REQUIRE(s.Size() == 4);
    REQUIRE(s.Top() == -4);
    Stack<int, std::list<int>> ss({-1, -1, -1});
    s.Swap(ss);

    REQUIRE(s.Size() == 3);
    REQUIRE(s.Top() == -1);
    REQUIRE(ss.Size() == 4);
    REQUIRE(ss.Top() == -4);
  }

  {
    std::list<int> d{1, 2, 3};
    Stack<int, std::list<int>> s(std::move(d));
    REQUIRE_FALSE(s.Empty());
    REQUIRE(s.Top() == 3);
    s.Top() = 4;
    REQUIRE(s.Top() == 4);
  }

  {
    std::list<int> d{1, 2, 3};
    const Stack<int, std::list<int>> s(d.begin(), d.end());
    REQUIRE_FALSE(s.Empty());
    REQUIRE(s.Top() == 3);
    REQUIRE(s.Size() == 3);
  }
}

}
